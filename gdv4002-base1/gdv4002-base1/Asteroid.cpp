#include "Asteroid.h"
#include "Engine.h"

Asteroid::Asteroid(glm::vec2 initPosition, float initOrientation, glm::vec2 initSize, GLuint initTextureID, glm::vec2 initVelocity, float initRotationSpeed)
	: GameObject2D(initPosition, initOrientation, initSize, initTextureID) {
	velocity = initVelocity;
	rotationSpeed = initRotationSpeed;
}

void Asteroid::update(double tDelta) {
	// Rotate the asteroid
	orientation += rotationSpeed * (float)tDelta;
	
	// Move the asteroid
	position += velocity * (float)tDelta;
	
	// World wrapping (same as player)
	float halfWidth = getViewplaneWidth() * 0.5f;
	float halfHeight = getViewplaneHeight() * 0.5f;
	
	if (position.x > halfWidth) {
		position.x = -halfWidth;
	}
	else if (position.x < -halfWidth) {
		position.x = halfWidth;
	}
	
	if (position.y > halfHeight) {
		position.y = -halfHeight;
	}
	else if (position.y < -halfHeight) {
		position.y = halfHeight;
	}
}
