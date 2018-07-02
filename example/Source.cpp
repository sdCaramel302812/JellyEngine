#define SOFTWARE_VERSION "ver 0.0.1"

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

/*		版本控制規範
*
*		master		0.1.0 -----------------------------------------------------------------> 0.2.0
*					  |																	/
*		develop		  `--> 0.1.1 ----------------> 0.1.2 ----------------------> 0.1.3 -
*							|				   /	|						 /
*		A					`--> 0.1.1A(date) -		`--> 0.1.2A(date) -------
*							|												/
*		B					`--> 0.1.1B(date) ---------> 0.1.1B(date) ------
*
*
*
*/

//#include "../include/Jelly2D/Define.h"
#include "../include/Jelly2D/Include.h"
#include "GameExtend.h"

//GameExtend *game = new GameExtend();
//Camera camera(2);
extern Game *game;
extern Camera camera;

void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);



int main() {
	game = new Game();
	glfwSetCursorPosCallback(game->scene->window, mouse_callback);
	glfwSetMouseButtonCallback(game->scene->window, mouse_button_callback);
	glfwSetScrollCallback(game->scene->window, scroll_callback);


	game->gameLoop();
	return 0;
}

//*
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
//*/