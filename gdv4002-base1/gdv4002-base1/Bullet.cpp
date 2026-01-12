#include "Bullet.h"

Bullet::Bullet(glm::vec2 initPosition, float initOrientation, glm::vec2 initSize, GLuint initTextureID, glm::vec2 initVelocity)
	: GameObject2D(initPosition, initOrientation, initSize, initTextureID) {
	velocity = initVelocity;
}

void Bullet::update(double tDelta) {
	position += velocity * (float)tDelta;
}


