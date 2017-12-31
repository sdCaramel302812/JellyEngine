#include "Physics.h"
#include <iostream>

#define MAX_DT 0.016
#define MIN_DT 0.001
#define TERMINAL_SPEED 15
#define ALLOW_DIST 0.001
const float Physics::g = 9.8;

void Physics::displace(Entity * obj,float dt)
{
	if (dt > MAX_DT) {
		dt = MAX_DT;
	}
	if (dt < MIN_DT) {
		dt = MIN_DT;
	}
	//Force
	DataEvent<Entity *> *force_event = new DataEvent<Entity *>();
	force_event->setTarget(obj);
	if (obj->rigid.data.velocity.y > -TERMINAL_SPEED && !obj->rigid._is_static) {//test before ball with surface collision done
		force_event->setV(glm::vec3(0.0f, -g*dt, 0.0f));
	}

	//Force
	//Displace
	MotionEvent<Entity *> *dis_event = new MotionEvent<Entity *>();
	dis_event->setTarget(obj);//加上重力影響
	dis_event->setDisplace(obj->rigid.data.velocity.x*dt + force_event->_d_v.x*dt, obj->rigid.data.velocity.y*dt + force_event->_d_v.y*dt, obj->rigid.data.velocity.z*dt + force_event->_d_v.z*dt);
	if (dis_event->dx()*dis_event->dx() + dis_event->dy()*dis_event->dy() + dis_event->dz()*dis_event->dz() < 0.0000001) {
		dis_event->setDisplace(0, 0, 0);
	}
	//Displace

	if (obj->e_type == PLAYER) {
		//cout << obj->rigid.data.velocity.x << " " << obj->rigid.data.velocity.y << " " << obj->rigid.data.velocity.z << endl;
	}

	//Collision
	for (int i = 0; i < ObjectManager::object_list.size(); ++i) {
		if (obj->_id != ObjectManager::object_list.at(i)->_id) {
			int tmp = collisionDetect(obj, ObjectManager::object_list.at(i), dis_event);

			if (tmp == -1) {//如重疊則縮小時間差重新計算，若 dt 太小仍碰撞則給予反向位移，計算完後進行碰撞反應
					float small_dt = dt;
					glm::vec3 normal;
					if(!(obj->rigid.type == SPHERE && ObjectManager::object_list.at(i)->rigid.type == SPHERE)) {
						glm::vec3 dir1;
						glm::vec3 dir2;
						float dist;
						if (obj->rigid.type == RECTANGLE) {
							dir1 = obj->rigid.getVertices().at(0) - obj->rigid.getVertices().at(1);
							dir2 = obj->rigid.getVertices().at(1) - obj->rigid.getVertices().at(2);
							normal = glm::cross(dir1, dir2);
						}
						else {
							dir1 = ObjectManager::object_list.at(i)->rigid.getVertices().at(0) - ObjectManager::object_list.at(i)->rigid.getVertices().at(1);
							dir2 = ObjectManager::object_list.at(i)->rigid.getVertices().at(1) - ObjectManager::object_list.at(i)->rigid.getVertices().at(2);
							normal = glm::cross(dir1, dir2);
						}
						float a, b, c, d;
						a = normal.x;
						b = normal.y;
						c = normal.z;
						normal = glm::normalize(normal);
						if (obj->rigid.type == RECTANGLE) {
							float x0 = ObjectManager::object_list.at(i)->rigid.data.position.x;
							float y0 = ObjectManager::object_list.at(i)->rigid.data.position.y;
							float z0 = ObjectManager::object_list.at(i)->rigid.data.position.z;
							d = -(a*obj->rigid.getVertices().at(0).x + b*obj->rigid.getVertices().at(0).y + c*obj->rigid.getVertices().at(0).z);
							if (a*x0 + b*y0 + c*z0 + d < 0) {
								normal *= -1;
							}
						}
						else {
							float x0 = obj->rigid.data.position.x;
							float y0 = obj->rigid.data.position.y;
							float z0 = obj->rigid.data.position.z;
							d = -(a*ObjectManager::object_list.at(i)->rigid.getVertices().at(0).x + b*ObjectManager::object_list.at(i)->rigid.getVertices().at(0).y + c*ObjectManager::object_list.at(i)->rigid.getVertices().at(0).z);
							if (a*x0 + b*y0 + c*z0 + d < 0) {
								normal *= -1;
							}
						}
					}
					

					glm::vec3 re_dis = abs(glm::dot(glm::vec3(dis_event->dx(), dis_event->dy(), dis_event->dz()), normal))*normal;
					int count = 0;
					while (collisionDetect(obj, ObjectManager::object_list.at(i), dis_event) == -1) {
						small_dt /= 2;
						++count;

						if ((obj->rigid.type == SPHERE && ObjectManager::object_list.at(i)->rigid.type == SPHERE)) {
							dis_event->setDisplace(obj->rigid.data.velocity.x*small_dt, obj->rigid.data.velocity.y*small_dt, obj->rigid.data.velocity.z*small_dt);
							if (small_dt < 0.001) {
								dis_event->setDisplace(0, 0, 0);
								break;
							}
						}
						else{
							dis_event->plusDisplace(re_dis);
							if (count == 10) {
								break;
							}
						}

					}
					tmp = 1;
			}
			if (tmp == 1) {
				//球碰撞反應
				if (obj->rigid.type == SPHERE && ObjectManager::object_list.at(i)->rigid.type == SPHERE) {
					float dx = (obj->rigid.data.position.x + dis_event->dx() - ObjectManager::object_list.at(i)->rigid.data.position.x);
					float dy = (obj->rigid.data.position.y + dis_event->dy() - ObjectManager::object_list.at(i)->rigid.data.position.y);
					float dz = (obj->rigid.data.position.z + dis_event->dz() - ObjectManager::object_list.at(i)->rigid.data.position.z);
					float dist = (dx*dx + dy*dy + dz*dz);    //平方
					if (dist < 0.00001) {
						dist = 1;
					}

					float restrict = (obj->rigid._restitution_coeffient + ObjectManager::object_list.at(i)->rigid._restitution_coeffient) / 2;
					glm::vec3 normal = glm::normalize(glm::vec3(dx * dx / dist, dy * dy / dist, dz *dz / dist));//B指向A
					normal.z *= -1;//左手坐標系
					normal.x *= -1;

					glm::vec3 relate_v = (obj->rigid.data.velocity + force_event->_d_v) - ObjectManager::object_list.at(i)->rigid.data.velocity;
					float F;
					F = (-(1 + restrict)*(glm::dot(normal, relate_v))) / (((1 / obj->rigid._mass + 1 / ObjectManager::object_list.at(i)->rigid._mass)));//其實是衝量

					DataEvent<Entity *> *collide_event1_A = new DataEvent<Entity *>();
					collide_event1_A->setTarget(obj);
					collide_event1_A->setV(F*normal / obj->rigid._mass);
					DataEvent<Entity *> *collide_event1_B = new DataEvent<Entity *>();
					collide_event1_B->setTarget(ObjectManager::object_list.at(i));
					collide_event1_B->setV(-F*normal / ObjectManager::object_list.at(i)->rigid._mass);
					MotionEvent<Entity *> *collide_event2_A = new MotionEvent<Entity *>();
					collide_event2_A->setTarget(obj);
					collide_event2_A->setDisplace(obj->rigid.data.velocity*dt + F*normal*dt / obj->rigid._mass);

					MotionEvent<Entity *> *collide_event2_B = new MotionEvent<Entity *>();
					collide_event2_B->setTarget(ObjectManager::object_list.at(i));
					collide_event2_B->setDisplace(ObjectManager::object_list.at(i)->rigid.data.velocity*dt - F*normal*dt / ObjectManager::object_list.at(i)->rigid._mass);

					collide_event1_A->use();
					collide_event1_B->use();

					delete collide_event1_A;
					delete collide_event1_B;
					delete collide_event2_A;
					delete collide_event2_B;
				}
				//球碰撞反應
				//球對面碰撞反應
//*
				if ((obj->rigid.type == SPHERE && ObjectManager::object_list.at(i)->rigid.type == RECTANGLE) || (ObjectManager::object_list.at(i)->rigid.type == SPHERE && obj->rigid.type == RECTANGLE)) {
					glm::vec3 dir1;
					glm::vec3 dir2;
					glm::vec3 normal;
					float a, b, c, d;//surface equation coefficient
					float dist;
					if (obj->rigid.type == RECTANGLE) {
						dir1 = obj->rigid.getVertices().at(0) - obj->rigid.getVertices().at(1);
						dir2 = obj->rigid.getVertices().at(1) - obj->rigid.getVertices().at(2);
						normal = glm::cross(dir1, dir2);
						normal = glm::normalize(normal);
					}
					else {
						dir1 = ObjectManager::object_list.at(i)->rigid.getVertices().at(0) - ObjectManager::object_list.at(i)->rigid.getVertices().at(1);
						dir2 = ObjectManager::object_list.at(i)->rigid.getVertices().at(1) - ObjectManager::object_list.at(i)->rigid.getVertices().at(2);
						normal = glm::cross(dir1, dir2);
						normal = glm::normalize(normal);
						glm::vec3 a;
						if (obj->rigid.data.velocity.x != 0 || obj->rigid.data.velocity.y != 0 || obj->rigid.data.velocity.z != 0) {//摩擦力
								a = obj->rigid._friction_coeffient * glm::cross(glm::cross(glm::normalize(obj->rigid.data.velocity), normal), normal);
						}
						if (obj->e_type == PLAYER) {
							a.x *= 20;
							a.y *= 20;
							a.z *= 20;
						}
						force_event->plusV(glm::vec3(a.x*dt, a.y*dt, a.z*dt));//摩擦力
					}
					
					a = normal.x;
					b = normal.y;
					c = normal.z;
					if (obj->rigid.type == RECTANGLE) {
						d = -(a*obj->rigid.getVertices().at(0).x + b*obj->rigid.getVertices().at(0).y + c*obj->rigid.getVertices().at(0).z);
					}
					else {
						d = -(a*ObjectManager::object_list.at(i)->rigid.getVertices().at(0).x + b*ObjectManager::object_list.at(i)->rigid.getVertices().at(0).y + c*ObjectManager::object_list.at(i)->rigid.getVertices().at(0).z);
					}
					if (obj->rigid.type == SPHERE) {
						float x0 = obj->rigid.data.position.x;
						float y0 = obj->rigid.data.position.y;
						float z0 = obj->rigid.data.position.z;
						if (a*x0 + b*y0 + c*z0 + d < 0) {
							normal *= -1;
						}
					}
					else {
						float x0 = ObjectManager::object_list.at(i)->rigid.data.position.x;
						float y0 = ObjectManager::object_list.at(i)->rigid.data.position.y;
						float z0 = ObjectManager::object_list.at(i)->rigid.data.position.z;
						if (a*x0 + b*y0 + c*z0 + d < 0) {
							normal *= -1;
						}
					}
					//normal.z *= -1;
					float restrict = (obj->rigid._restitution_coeffient + ObjectManager::object_list.at(i)->rigid._restitution_coeffient) / 2;
					glm::vec3 relate_v = (obj->rigid.data.velocity + force_event->_d_v) - ObjectManager::object_list.at(i)->rigid.data.velocity;
					float F;
					F = (-(1 + restrict)*(glm::dot(normal, relate_v))) / (((1 / obj->rigid._mass + 1 / ObjectManager::object_list.at(i)->rigid._mass)));
					DataEvent<Entity *> *collide_event1_A = new DataEvent<Entity *>();
					collide_event1_A->setTarget(obj);
					collide_event1_A->setV(F*normal / obj->rigid._mass);

					DataEvent<Entity *> *collide_event1_B = new DataEvent<Entity *>();
					collide_event1_B->setTarget(ObjectManager::object_list.at(i));
					collide_event1_B->setV(-F*normal / ObjectManager::object_list.at(i)->rigid._mass);

					MotionEvent<Entity *> *collide_event2_A = new MotionEvent<Entity *>();
					collide_event2_A->setTarget(obj);
					collide_event2_A->setDisplace(obj->rigid.data.velocity*dt + F*normal*dt / obj->rigid._mass);

					MotionEvent<Entity *> *collide_event2_B = new MotionEvent<Entity *>();
					collide_event2_B->setTarget(ObjectManager::object_list.at(i));
					collide_event2_B->setDisplace(ObjectManager::object_list.at(i)->rigid.data.velocity*dt - F*normal*dt / ObjectManager::object_list.at(i)->rigid._mass);

					collide_event1_A->use();
					collide_event1_B->use();

					delete collide_event1_A;
					delete collide_event1_B;
					delete collide_event2_A;
					delete collide_event2_B;
				}
				//*/
				//球對面碰撞反應
			}
		}
	}
	//Collision

	force_event->use();
	dis_event->use();
	delete force_event;
	delete dis_event;

	if (glm::length(obj->rigid.data.velocity) < 0.005) {
	//	obj->rigid.data.velocity = glm::vec3(0.0f, 0.0f, 0.0f);
	}

	//View Frustum Culling
	//*
	DrawEvent<Entity *> *draw = new DrawEvent<Entity *>();
	draw->setTarget(obj);
	if (obj->rigid.type == SPHERE) {
		draw->setDrawType(GL_LINE_STRIP);
	}
	EventManager::render_event.push(draw);
	//*/
	//View Frustum Culling
}



int Physics::collisionDetect(Entity * A, Entity * B, MotionEvent<Entity*>* event)
{
	if (A->rigid.type == SPHERE&&B->rigid.type == SPHERE) {
		float dx = abs(A->rigid.data.position.x + event->dx() - B->rigid.data.position.x);
		float dy = abs(A->rigid.data.position.y + event->dy() - B->rigid.data.position.y);
		float dz = abs(A->rigid.data.position.z + event->dz() - B->rigid.data.position.z);
		float dist = (dx*dx + dy*dy + dz*dz);//平方
		//cout <<	dist << endl;
		if (dist < pow(A->rigid.getRadius() + B->rigid.getRadius() - ALLOW_DIST, 2)) {
			return -1;
		}
		else if (dist <= pow(A->rigid.getRadius() + B->rigid.getRadius(), 2)) {
			return 1;
		}
		else {
			return 0;
		}
	}
	if ((A->rigid.type == SPHERE && B->rigid.type == RECTANGLE) || (B->rigid.type == SPHERE && A->rigid.type == RECTANGLE)) {
		glm::vec3 dir1;
		glm::vec3 dir2;
		glm::vec3 normal;
		float a, b, c, d;//surface equation coefficient
		float dist;
		if (A->rigid.type == RECTANGLE) {
			dir1 = A->rigid.getVertices().at(0) - A->rigid.getVertices().at(1);
			dir2 = A->rigid.getVertices().at(1) - A->rigid.getVertices().at(2);
		}
		else {
			dir1 = B->rigid.getVertices().at(0) - B->rigid.getVertices().at(1);
			dir2 = B->rigid.getVertices().at(1) - B->rigid.getVertices().at(2);
		}
		normal = glm::cross(dir1, dir2);
		a = normal.x;
		b = normal.y;
		c = normal.z;
		if (A->rigid.type == RECTANGLE) {
			d = -(a*A->rigid.getVertices().at(0).x + b*A->rigid.getVertices().at(0).y + c*A->rigid.getVertices().at(0).z);
		}
		else {
			d = -(a*B->rigid.getVertices().at(0).x + b*B->rigid.getVertices().at(0).y + c*B->rigid.getVertices().at(0).z);
		}
		if (A->rigid.type == SPHERE) {
			float x0 = A->rigid.data.position.x + event->dx();
			float y0 = A->rigid.data.position.y + event->dy();
			float z0 = A->rigid.data.position.z + event->dz();
			float r = A->rigid.getRadius();
			dist = abs(pow(a*x0 + b*y0 + c*z0 + d, 2) / (a*a + b*b + c*c));//點到面的距離  //平方
			if (dist < r * r) {
				glm::vec3 proj = A->rigid.data.position - glm::normalize(normal)*dist;//圓心在平面上投影點
				glm::vec3 line1 = B->rigid.getVertices().at(0) - B->rigid.getVertices().at(3);
				glm::vec3 line2 = B->rigid.getVertices().at(2) - B->rigid.getVertices().at(1);
				glm::vec3 line3 = B->rigid.getVertices().at(1) - B->rigid.getVertices().at(0);
				glm::vec3 line4 = B->rigid.getVertices().at(3) - B->rigid.getVertices().at(2);
				glm::vec3 cross1 = glm::cross(line1, proj - B->rigid.getVertices().at(3));
				glm::vec3 cross2 = glm::cross(line2, proj - B->rigid.getVertices().at(1));
				glm::vec3 cross3 = glm::cross(line3, proj - B->rigid.getVertices().at(0));
				glm::vec3 cross4 = glm::cross(line4, proj - B->rigid.getVertices().at(2));
				float dist2line1 = (pow(cross1.x, 2) + pow(cross1.y, 2) + pow(cross1.z, 2)) / (pow(line1.x, 2) + pow(line1.y, 2) + pow(line1.z, 2));  //平方
				float dist2line2 = (pow(cross2.x, 2) + pow(cross2.y, 2) + pow(cross2.z, 2)) / (pow(line2.x, 2) + pow(line2.y, 2) + pow(line2.z, 2));  //平方
				float dist2line3 = (pow(cross3.x, 2) + pow(cross3.y, 2) + pow(cross3.z, 2)) / (pow(line3.x, 2) + pow(line3.y, 2) + pow(line3.z, 2));  //平方
				float dist2line4 = (pow(cross4.x, 2) + pow(cross4.y, 2) + pow(cross4.z, 2)) / (pow(line4.x, 2) + pow(line4.y, 2) + pow(line4.z, 2));  //平方
				//投影點距四邊距離皆小於面長度時即發生碰撞
				if (dist2line1 < r + (pow(line3.x, 2) + pow(line3.y, 2) + pow(line3.z, 2)) && dist2line2 < (pow(line3.x, 2) + pow(line3.y, 2) + pow(line3.z, 2)) && dist2line3 < (pow(line1.x, 2) + pow(line1.y, 2) + pow(line1.z, 2)) && dist2line4 < (pow(line1.x, 2) + pow(line1.y, 2) + pow(line1.z, 2))) {  //平方
					if (dist < pow(r - ALLOW_DIST, 2)) {
						return -1;
					}
					else {
						return 1;
					}
				}
			}
		}

		return 0;
	}

	
	
	return 0;//無碰撞物體回傳值
}


Physics::Physics()
{
}


Physics::~Physics()
{
}
