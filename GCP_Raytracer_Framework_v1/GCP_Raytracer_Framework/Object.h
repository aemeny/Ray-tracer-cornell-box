#pragma once
#include "Ray.h"

struct finalIntersection
{
	glm::vec3 intersectionPos{ 0,0,0 };
	glm::vec3 surfaceNormal{ 0,0,0 };
	bool hasIntersected{ false };
	int objIndex = 0;
};

struct Object
{
public:

	virtual void init(glm::vec3 _pos, glm::vec3 _col, glm::vec3 _col2, float _shiny, float _radius, glm::vec3 _norm, float _reflectivity);
	virtual finalIntersection rayIntersect(Ray _ray);

	glm::vec3 position;
	glm::vec3 colour;
	glm::vec3 colour2;
	float shiny;
	float radius;
	glm::vec3 normal;
	float reflectivity;
};

