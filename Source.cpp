#define SOFTWARE_VERSION "ver 0.5.3"

/*
*　　　　　　　　┏┓　　　┏┓+ +
*　　　　　　　┏┛┻━━━┛┻┓ + +
*　　　　　　　┃　　　　　　　┃
*　　　　　　　┃　　　━　　　┃ ++ + + +
*　　　　　　 ████━████ ┃+
*　　　　　　　┃　　　　　　　┃ +
*　　　　　　　┃　　　┻　　　┃
*　　　　　　　┃　　　　　　　┃ + +
*　　　　　　　┗━┓　　　┏━┛
*　　　　　　　　　┃　　　┃
*　　　　　　　　　┃　　　┃ + + + +
*　　　　　　　　　┃　　　┃　　　　Code is far away from bug with the animal protecting
*　　　　　　　　　┃　　　┃ + 　　　　神獸保佑,程式碼無bug
*　　　　　　　　　┃　　　┃
*　　　　　　　　　┃　　　┃　　+
*　　　　　　　　　┃　 　　┗━━━┓ + +
*　　　　　　　　　┃ 　　　　　　　┣┓
*　　　　　　　　　┃ 　　　　　　　┏┛
*　　　　　　　　　┗┓┓┏━┳┓┏┛ + + + +
*　　　　　　　　　　┃┫┫　┃┫┫
*　　　　　　　　　　┗┻┛　┗┻┛+ + + +
*/


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
	bool has_event = game->scrollCallback(window, xoffset, yoffset);

	if (!has_event) {
		camera.zoom -= yoffset * 0.7;
		if (camera.zoom <= 0) {
			camera.zoom = 0.1;
		}
	}
}


