#include "Physics.h"
#include <iostream>


#define MAX_DT 0.032
#define MIN_DT 0.001
#define TERMINAL_SPEED 150
#define ALLOW_DIST 0.0001
const float Physics::g = 9.8;

void Physics::displace(Entity * obj,float dt)
{
	if (dt > MAX_DT) {
		dt = MAX_DT;
	}
	if (dt < MIN_DT) {
		dt = MIN_DT;
	}

	if (obj->e_type == PLAYER) {
	//	cout << obj->rigid.data.position.x << " " << obj->rigid.data.position.y << " " << obj->rigid.data.position.z << endl;
	}



	if (!obj->rigid._is_static) {
		//Force
		DataEvent<Entity *> *force_event = new DataEvent<Entity *>();
		force_event->setTarget(obj);//加上重力影響
		if (obj->rigid.data.velocity.y > -TERMINAL_SPEED) {//test before ball with surface collision done
			force_event->setV(glm::vec3(0.0f, -g*dt, 0.0f));
		}

		//Force
		//Displace
		MotionEvent<Entity *> *dis_event = new MotionEvent<Entity *>();
		dis_event->setTarget(obj);
		dis_event->setDisplace(obj->rigid.data.velocity.x*dt + force_event->_d_v.x*dt, obj->rigid.data.velocity.y*dt + force_event->_d_v.y*dt, obj->rigid.data.velocity.z*dt + force_event->_d_v.z*dt);
		if (dis_event->dx()*dis_event->dx() + dis_event->dy()*dis_event->dy() + dis_event->dz()*dis_event->dz() < 0.000001) {
			dis_event->setDisplace(0, 0, 0);
		}
		//Displace
		

		//Collision
		if (obj->rigid.type != NO_COLLIDE) {
			//find collision pair
			std::vector<Entity *> collide_list = ObjectManager::aabb_tree.collisionWith(obj->_aabb_node);
			//std::vector<Entity *> collide_list = ObjectManager::object_list;
			//if(obj->e_type==PLAYER)
			//	cout << collide_list.size() << endl;

			for (int i = 0; i < collide_list.size(); ++i) {
				if (collide_list.at(i)->_id == obj->_id) {
					continue;
				}

				int is_collided = collisionDetect(obj, collide_list.at(i), dis_event);
				
				//	vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv		physics collision		vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv//
				if (is_collided == -1) {//如重疊則縮小時間差重新計算，若 dt 太小仍碰撞則給予反向位移，計算完後進行碰撞反應
					float small_dt = dt;
					glm::vec3 normal;
					if (obj->rigid.type == SPHERE && (collide_list.at(i)->rigid.type == RECTANGLE || collide_list.at(i)->rigid.type == PLATFORM)) {
						normal = collide_list.at(i)->rigid.getNormal().at(0);
						float a, b, c, d;
						a = normal.x;
						b = normal.y;
						c = normal.z;

						float x0 = obj->rigid.data.position.x;
						float y0 = obj->rigid.data.position.y;
						float z0 = obj->rigid.data.position.z;
						d = collide_list.at(i)->rigid.getNormal().at(0).w;
						if (a*x0 + b*y0 + c*z0 + d < 0) {
							normal *= -1;
						}
					}

					glm::vec3 re_dis = abs(glm::dot(glm::vec3(dis_event->dx(), dis_event->dy(), dis_event->dz()), normal))/10*normal;
					int count = 0;
					while (collisionDetect(obj, collide_list.at(i), dis_event) == -1) {
						small_dt /= 2;
						++count;
						if ((obj->rigid.type == SPHERE &&collide_list.at(i)->rigid.type == SPHERE)) {
							//                try to use force to solve multiple ball collision problem                            //
							//                                           unsolve                                                   //
							//obj->rigid.data.velocity.x*small_dt, obj->rigid.data.velocity.y*small_dt, obj->rigid.data.velocity.z*small_dt
							dis_event->setDisplace(obj->rigid.data.velocity.x*small_dt, obj->rigid.data.velocity.y*small_dt, obj->rigid.data.velocity.z*small_dt);
							if (small_dt < 0.001) {
								dis_event->setDisplace(0, 0, 0);
								break;
							}
						}
						else if(obj->rigid.type == SPHERE && (collide_list.at(i)->rigid.type == RECTANGLE || collide_list.at(i)->rigid.type == PLATFORM)){
							dis_event->plusDisplace(re_dis);
							if (count == 10) {
								break;
							}
						}
					}
					is_collided = 1;
				}
				//	^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^		physics collision		^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^//
				
				//	vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv		physics collision		vvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvvv//
				if (is_collided == 1) {
					//球碰撞反應
					if (obj->rigid.type == SPHERE &&collide_list.at(i)->rigid.type == SPHERE) {
						float dx = (obj->rigid.data.position.x + dis_event->dx() - collide_list.at(i)->rigid.data.position.x);
						float dy = (obj->rigid.data.position.y + dis_event->dy() - collide_list.at(i)->rigid.data.position.y);
						float dz = (obj->rigid.data.position.z + dis_event->dz() - collide_list.at(i)->rigid.data.position.z);
						float restrict = (obj->rigid._restitution_coeffient + collide_list.at(i)->rigid._restitution_coeffient) / 2;
						glm::vec3 normal = glm::normalize(glm::vec3(dx, dy, dz));//B指向A
						//normal.z *= -1;//左手坐標系
						//normal.x *= -1;

						glm::vec3 relate_v = (obj->rigid.data.velocity + force_event->_d_v) - collide_list.at(i)->rigid.data.velocity;
						float F;
						F = (-(1 + restrict)*(glm::dot(normal, relate_v))) / (((1 / obj->rigid._mass + 1 / collide_list.at(i)->rigid._mass)));//其實是衝量

						DataEvent<Entity *> *collide_event1_A = new DataEvent<Entity *>();
						collide_event1_A->setTarget(obj);
						collide_event1_A->setV(F*normal / obj->rigid._mass);
						DataEvent<Entity *> *collide_event1_B = new DataEvent<Entity *>();
						collide_event1_B->setTarget(collide_list.at(i));
						collide_event1_B->setV(-F*normal / collide_list.at(i)->rigid._mass);

						collide_event1_A->use();
						collide_event1_B->use();

						delete collide_event1_A;
						delete collide_event1_B;
					}
					//球碰撞反應
					//球對面碰撞反應

					if (obj->rigid.type == SPHERE && (collide_list.at(i)->rigid.type == RECTANGLE || collide_list.at(i)->rigid.type == PLATFORM)) {
						glm::vec3 normal;
						float a, b, c, d;//surface equation coefficient

						normal = collide_list.at(i)->rigid.getNormal().at(0);
						glm::vec3 acc;
						if (obj->rigid.data.velocity.x != 0 || obj->rigid.data.velocity.y != 0 || obj->rigid.data.velocity.z != 0) {//摩擦力
							//acc = obj->rigid._friction_coeffient * glm::cross(glm::cross(glm::normalize(obj->rigid.data.velocity), normal), normal) * g;
							//if (acc.length() > glm::vec3(obj->rigid.data.velocity + force_event->_d_v).length()) {
								//acc /= acc.length();
								//acc *= (obj->rigid.data.velocity.length() + force_event->_d_v.length());
								//acc *= 0;
							//}
							//std::cerr << a.x << "\t" << a.y << "\t" << a.z << endl;
							//force_event->_d_v *= 0.99;
						}
						if (obj->e_type == PLAYER && obj->rigid.data.velocity.length() * 10 > acc.length() * dt) {//unfixed
							//acc.x *= 1.26;
							//acc.y *= 1.26;
							//acc.z *= 1.26;
						}
						//force_event->plusV(glm::vec3(acc.x*dt, acc.y*dt, acc.z*dt));//摩擦力		//<-----------------------待修正

						a = normal.x;
						b = normal.y;
						c = normal.z;
						//低FPS元凶	<----------------------------------------------------------------------------//
						d = collide_list.at(i)->rigid.getNormal().at(0).w;

						float x0 = obj->rigid.data.position.x;
						float y0 = obj->rigid.data.position.y;
						float z0 = obj->rigid.data.position.z;
						if (a*x0 + b*y0 + c*z0 + d < 0) {
							normal *= -1;
						}
						//normal.z *= -1;
						float restrict = (obj->rigid._restitution_coeffient + collide_list.at(i)->rigid._restitution_coeffient) / 2;
						glm::vec3 relate_v = (obj->rigid.data.velocity + force_event->_d_v) - collide_list.at(i)->rigid.data.velocity;
						float F;
						F = (-(1 + restrict)*(glm::dot(normal, relate_v))) / (((1 / obj->rigid._mass + 1 / collide_list.at(i)->rigid._mass)));
						DataEvent<Entity *> *collide_event1_A = new DataEvent<Entity *>();
						collide_event1_A->setTarget(obj);
						collide_event1_A->setV(F*normal / obj->rigid._mass);

						//DataEvent<Entity *> *collide_event1_B = new DataEvent<Entity *>();
						//collide_event1_B->setTarget(collide_list.at(i));
						//collide_event1_B->setV(-F*normal / collide_list.at(i)->rigid._mass);

						collide_event1_A->use();
						//collide_event1_B->use();

						delete collide_event1_A;
						//delete collide_event1_B;

					}
					//球對面碰撞反應
				}
				//	^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^		physics collision		^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^^//
			}

		}

		//Collision


		force_event->use();
		dis_event->use();
		delete force_event;
		delete dis_event;		
	}



	//View Frustum Culling
	if (obj->isVisible()) {
		DrawEvent<Entity *> *draw = new DrawEvent<Entity *>();
		draw->setTarget(obj);
		//if (obj->rigid.type == SPHERE) {
		//	draw->setDrawType(GL_LINE_STRIP);
		//}
		if ((obj->shader == "texture_ins" || obj->shader == "texture")) {
			EventManager::texture_render_event.push_back(draw);
		}
		else if (obj->shader == "color_ins") {
			EventManager::color_render_event.push_back(draw);
		}
	}
	//View Frustum Culling

}



int Physics::collisionDetect(Entity * A, Entity * B, MotionEvent<Entity*>* event)
{
	//球對球碰撞偵測
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
	//
	//球對面碰撞偵測
	if ((A->rigid.type == SPHERE && B->rigid.type == RECTANGLE)) {

		glm::vec3 normal;
		float a, b, c, d;//surface equation coefficient

		normal = B->rigid.getNormal().at(0);
		a = normal.x;
		b = normal.y;
		c = normal.z;
		d = B->rigid.getNormal().at(0).w;

		float dist;
		float x0 = A->rigid.data.position.x + event->dx();
		float y0 = A->rigid.data.position.y + event->dy();
		float z0 = A->rigid.data.position.z + event->dz();
		float r = A->rigid.getRadius() - 0.1;
		dist = abs((a*x0 + b*y0 + c*z0 + d) * B->rigid.getNormalLengthInverse());//點到面的距離 
		if (dist < r) {

			glm::vec3 proj = A->rigid.data.position - normal*dist;//圓心在平面上投影點

			//將平面及投影點轉換到 XY 平面，再依照座標軸比較投影點是否位於矩形內部
			glm::vec3 line1 = B->rigid.getVertices().at(3) - B->rigid.getVertices().at(0);
			glm::vec3 line2 = B->rigid.getVertices().at(1) - B->rigid.getVertices().at(0);
			glm::mat3 transMat;
			transMat[0][0] = line1[0];
			transMat[1][0] = line1[1];
			transMat[2][0] = line1[2];
			transMat[0][1] = line2[0];
			transMat[1][1] = line2[1];
			transMat[2][1] = line2[2];
			transMat[0][2] = B->rigid.getNormal().at(0)[0];
			transMat[1][2] = B->rigid.getNormal().at(0)[1];
			transMat[2][2] = B->rigid.getNormal().at(0)[2];
			float x0, x1, x2, y0, y1, y2;
			glm::vec3 newPoint1 = transMat * B->rigid.getVertices().at(0);
			glm::vec3 newPoint2 = transMat * B->rigid.getVertices().at(2);
			glm::vec3 newPoint = transMat * proj;
			x0 = newPoint.x;
			y0 = newPoint.y;
			x1 = newPoint1.x;
			y1 = newPoint1.y;
			x2 = newPoint2.x;
			y2 = newPoint2.y;

			if (x0 > x1 - r && x0 < x2 + r && y0 > y1 - r && y0 < y2 + r) {
				if (dist < r - ALLOW_DIST) {
					return -1;
				}
				else {
					return 1;
				}
			}
		}
		//
		return 0;
	}
	//球對地板碰撞偵測
	if (A->rigid.type == SPHERE&&B->rigid.type == PLATFORM) {
		float dist = A->rigid.data.position.y - A->rigid.getRadius();
		float r = A->rigid.getRadius();

		if (dist < 0) {
			if (dist <  - ALLOW_DIST) {
				return -1;
			}
			else {
				return 1;
			}
		}
		//
		return 0;
	}


	return 0;//無碰撞物體回傳值
}


bool Physics::GJK(std::vector<glm::vec3> vertices1, std::vector<glm::vec3> vertices2, float radius)
{
	std::vector<glm::vec3> point_set;
	for (int i = 0; i < vertices1.size(); ++i) {
		for (int j = 0; j < vertices2.size(); ++j) {
			point_set.push_back(vertices1.at(i) - vertices2.at(j));
		}
	}
	std::vector<bool> has_find;
	has_find.resize(point_set.size());
	has_find.at(0) = true;

	//傳進 support 檢查是否相交
	bool end = false;

	glm::vec3 first_point = point_set.at(0);
	glm::vec3 dir2origin = -first_point;
	while (1) {
		glm::vec3 second_point = GJKSupport(point_set, has_find, dir2origin, end);
		if (end) {
			return end;
		}
		if (glm::dot(dir2origin, second_point) < 0) {
			return false;
		}
		float p = glm::dot(dir2origin, second_point - first_point);
		dir2origin = -(first_point + p*(second_point - first_point));
		first_point = second_point;
	}
	return false;
}

glm::vec3 Physics::GJKSupport(std::vector<glm::vec3> vertices, std::vector<bool> &has_find, glm::vec3 dir, bool &end)
{
	//檢查所有點和至原點向量的內積並回傳內積結果最大的點
	//若最大點已找過代表包含原點，設 end 為 true
	float dot_result = -100;
	int result = 0;
	for (int i = 0; i < vertices.size(); ++i){
		float tmp = glm::dot(vertices.at(i), dir);
		if (tmp > dot_result) {
			result = i;
			dot_result = tmp;
		}
	}
	if (has_find.at(result)) {
		end = true;
	}
	else {
		has_find.at(result) = true;
	}
	if (vertices.at(result) == glm::vec3(0.0f, 0.0f, 0.0f)) {
		end = true;
	}
	return vertices.at(result);
}

float Physics::qsqrt(float number)
{
	long i;
	float x2, y;
	const float threehalfs = 1.5F;

	x2 = number * 0.5F;
	y = number;
	i = *(long *)&y;                       // evil floating point bit level hacking（对浮点数的邪恶位元hack）
	i = 0x5f3759df - (i >> 1);               // what the fuck?（这他妈的是怎么回事？）
	y = *(float *)&i;
	y = y * (threehalfs - (x2 * y * y));   // 1st iteration （第一次迭代）
										   //      y  = y * ( threehalfs - ( x2 * y * y ) );   // 2nd iteration, this can be removed（第二次迭代，可以删除）

	return y;
}

Physics::Physics()
{
}


Physics::~Physics()
{
}
