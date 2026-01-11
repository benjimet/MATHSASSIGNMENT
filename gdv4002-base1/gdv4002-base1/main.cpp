#include "Engine.h"


void updateGame(GLFWwindow* window, double tDelta);
void keyboardHandler(GLFWwindow* window, int key, int scancode, int action, int mods);


// Global variables
GameObject2D* player = nullptr;
glm::vec2 playerVelocity = glm::vec2(0.0f, 0.0f);
float rotationSpeed = 3.0f;
float accelerationSpeed = 5.0f;
float maxSpeed = 10.0f;


int main(void) {

	// Initialise the engine (create window, setup OpenGL backend)
	int initResult = engineInit("GDV4002 - Applied Maths for Games", 1024, 1024);

	// If the engine initialisation failed report error and exit
	if (initResult != 0) {

		printf("Cannot setup game window!!!\n");
		return initResult; // exit if setup failed
	}

	//
	// Setup game scene objects here
	//

	player = addObject("player", glm::vec2(0.0f, 0.0f), 0.0f, glm::vec2(0.3f, 0.3f), "Resources/Textures/player1_ship.png");
	
	//
	// Set callback functions
	//
	
	setUpdateFunction(updateGame, false);
	setKeyboardHandler(keyboardHandler);

	// Enter main loop - this handles update and render calls
	engineMainLoop();

	// When we quit (close window for example), clean up engine resources
	engineShutdown();

	// return success :)
	return 0;
}

void updateGame(GLFWwindow* window, double tDelta) {
	
	if (!player) return;
	
	if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) {
		player->orientation -= rotationSpeed * tDelta;
	}
	
	if (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) {
		player->orientation += rotationSpeed * tDelta;
	}
	
	if (glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) {
		glm::vec2 forward = glm::vec2(cos(player->orientation), sin(player->orientation));
		playerVelocity += forward * accelerationSpeed * (float)tDelta;
		float speed = glm::length(playerVelocity);
		if (speed > maxSpeed) {
			playerVelocity = glm::normalize(playerVelocity) * maxSpeed;
		}
	}
	
	player->position += playerVelocity * (float)tDelta;
	
	float halfWidth = getViewplaneWidth() * 0.5f;
	float halfHeight = getViewplaneHeight() * 0.5f;
	
	if (player->position.x > halfWidth) {
		player->position.x = -halfWidth;
	}
	else if (player->position.x < -halfWidth) {
		player->position.x = halfWidth;
	}
	
	if (player->position.y > halfHeight) {
		player->position.y = -halfHeight;
	}
	else if (player->position.y < -halfHeight) {
		player->position.y = halfHeight;
	}
}

void keyboardHandler(GLFWwindow* window, int key, int scancode, int action, int mods) {
	
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}
