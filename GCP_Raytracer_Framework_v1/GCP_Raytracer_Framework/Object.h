#pragma once
#include "Ray.h"

struct finalIntersection
{
	glm::vec3 intersectionPos{ 0,0,0 };
	glm::vec3 surfaceNormal{ 0,0,0 };
	bool hasIntersected{ false };
	int objIndex = 0;
};

class Object
{
public:
	Object(glm::vec3 _position, glm::vec3 _colour, float _shiny);

	virtual finalIntersection rayIntersect(Ray _ray);

	glm::vec3 position;
	glm::vec3 colour;
	float shiny;
};

