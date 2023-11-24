#pragma once
#include "Object.h"

class Plane : Object
{
public:
	glm::vec3 normal;

	Plane(glm::vec3 _position, glm::vec3 _colour, float _shiny, glm::vec3 _normal);

	finalIntersection rayIntersect(Ray _ray) override;
};

