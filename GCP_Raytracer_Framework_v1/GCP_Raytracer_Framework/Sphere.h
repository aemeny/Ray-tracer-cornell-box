#pragma once
#include "Object.h"

struct Sphere : Object
{
public:
	~Sphere();
	void init(glm::vec3 _pos, glm::vec3 _col, float _shiny, float _radius, glm::vec3 _norm) override;
	
	finalIntersection rayIntersect(Ray _ray) override;

	glm::vec3 findClosestPoint(Ray _Ray, float _x);
	float shortestDistance(Ray _Ray);
	glm::vec3 getNormal(glm::vec3 _point);
};

