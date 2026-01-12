#include "Engine.h"
#include "Asteroid.h"
#include "Bullet.h"
#include <glm/gtc/constants.hpp>
#include <cstdlib>
#include <ctime>


void updateGame(GLFWwindow* window, double tDelta);
void keyboardHandler(GLFWwindow* window, int key, int scancode, int action, int mods);


// Global variables
GameObject2D* player = nullptr;
glm::vec2 playerVelocity = glm::vec2(0.0f, 0.0f);
float rotationSpeed = 3.0f;
float accelerationSpeed = 4.0f;
float maxSpeed = 6.0f;
float bulletSpeed = 8.0f;
static bool previousMouseState = false;


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
		
	// Spawn asteroids
	srand((unsigned int)time(nullptr));
	for (int i = 0; i < 5; i++) {
		float x = ((float)rand() / RAND_MAX - 0.5f) * getViewplaneWidth();
		float y = ((float)rand() / RAND_MAX - 0.5f) * getViewplaneHeight();
		float rotation = (float)rand() / RAND_MAX * glm::two_pi<float>();
		float size = 0.2f + ((float)rand() / RAND_MAX) * 0.2f;
		
		// Random velocity (different speed and direction for each asteroid)
		float speed = 0.5f + ((float)rand() / RAND_MAX) * 2.0f; // 0.5 to 2.5
		float direction = (float)rand() / RAND_MAX * glm::two_pi<float>();
		glm::vec2 velocity = glm::vec2(cos(direction), sin(direction)) * speed;
		
		// Random rotation speed
		float rotationSpeed = -2.0f + ((float)rand() / RAND_MAX) * 4.0f; // -2 to 2 radians per second
		
		GLuint asteroidTexture = loadTexture("Resources/Textures/asteroid.png");
		Asteroid* asteroid = new Asteroid(glm::vec2(x, y), rotation, glm::vec2(size, size), asteroidTexture, velocity, rotationSpeed);
		addObject("asteroid", asteroid);
	}
	
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
	
	// Shooting bullets (left mouse button)
	bool currentMouseState = (glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);
	if (currentMouseState && !previousMouseState) {
		// Spawn bullet at player position in facing direction
		glm::vec2 forward = glm::vec2(cos(player->orientation), sin(player->orientation));
		glm::vec2 bulletVelocity = forward * bulletSpeed;
		
		GLuint bulletTexture = loadTexture("Resources/Textures/bullet.png");
		Bullet* bullet = new Bullet(player->position, player->orientation, glm::vec2(0.1f, 0.1f), bulletTexture, bulletVelocity);
		addObject("bullet", bullet);
	}
	previousMouseState = currentMouseState;
	
	// bye-bye bullets
	GameObjectCollection bullets = getObjectCollection("bullet");
	for (int i = 0; i < bullets.objectCount; i++) {
		if (bullets.objectArray[i]) {
			GameObject2D* bullet = bullets.objectArray[i];
			if (bullet->position.x > halfWidth || bullet->position.x < -halfWidth ||
				bullet->position.y > halfHeight || bullet->position.y < -halfHeight) {
				deleteObject(bullet);
			}
		}
	}
}

void keyboardHandler(GLFWwindow* window, int key, int scancode, int action, int mods) {
	
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS) {
		glfwSetWindowShouldClose(window, true);
	}
}
