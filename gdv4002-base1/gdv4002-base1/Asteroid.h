#pragma once

#include "GameObject2D.h"

class Asteroid : public GameObject2D {
public:
	Asteroid(glm::vec2 initPosition, float initOrientation, glm::vec2 initSize, GLuint initTextureID);
};
