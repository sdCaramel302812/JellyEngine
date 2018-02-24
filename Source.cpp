#define SOFTWARE_VERSION "ver 0.4.5"

#include "Define.h"
#include "Include.h"
#include "Game.h"

Game *game = new Game();

Camera camera(2);

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);



int main() {
	glfwSetCursorPosCallback(game->scene->window, mouse_callback);
	glfwSetMouseButtonCallback(game->scene->window, mouse_button_callback);
	glfwSetScrollCallback(game->scene->window, scroll_callback);

	Render::addTexture("./background.jpg", "background");
	Render::addTexture("./wall.jpg", "wall");
	//BackgroundObject *grass = new BackgroundObject("wall", glm::vec3(0, 0.5, 0), glm::vec3(1, 0.5, 1));
	//BackgroundObject *background = new BackgroundObject("background", glm::vec3(-25, 0, -25), glm::vec3(25, 0, 25));
	//ObjectManager::addEntity(grass);
	//ObjectManager::addEntity(background);
	glm::vec4 pos1 = camera.getProjectMatrix()*camera.getViewMatrix()*glm::vec4(0, 0.5, 0, 1);
	glm::vec4 pos2 = camera.getProjectMatrix()*camera.getViewMatrix()*glm::vec4(0, 0, 0, 1);


	WallUI *wall = new WallUI(glm::vec3(0, 1, 0));
	wall->setPoint2(glm::vec3(-10, 1, -10));
	cout << wall->x() << "\t" << wall->y() << endl;
	cout << wall->width() << "\t" << wall->height() << endl;
	cout << wall->_point1.x << "\t" << wall->_point1.y << "\t" << wall->_point1.z << endl;
	ObjectManager::addUI(wall);

	game->gameLoop();
	return 0;
}


void mouse_callback(GLFWwindow* window, double xpos, double ypos) {
	game->mouseCallback(window, xpos, ypos);
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods) {
	game->mouseButtonCallback(window, button, action, mods);
}

void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
	camera.zoom -= yoffset * 0.7;
	if (camera.zoom <= 0) {
		camera.zoom = 0.1;
	}
}


