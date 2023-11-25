#pragma once
#include "GCP_GFX_Framework.h"

struct Ray
{
public:
	glm::vec3 origin;
	glm::vec3 direction;

	Ray(glm::vec3 _origin, glm::vec3 _direction);

};

