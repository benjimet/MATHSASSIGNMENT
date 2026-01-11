#pragma once

#include "GameObject2D.h"

class Asteroid : public GameObject2D {
private:
	glm::vec2 velocity;
	float rotationSpeed;

public:
	Asteroid(glm::vec2 initPosition, float initOrientation, glm::vec2 initSize, GLuint initTextureID, glm::vec2 initVelocity, float initRotationSpeed);
	
	void update(double tDelta) override;
};
