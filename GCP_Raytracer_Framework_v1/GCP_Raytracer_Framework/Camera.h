#pragma once
#include "Ray.h"

class Camera
{
	glm::mat4 viewMatrix;

	Ray getRay(glm::ivec2 windowPos);
};

