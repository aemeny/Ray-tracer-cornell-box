#pragma once
#include "Object.h"

class Sphere : Object
{
public:
	float radius; 

	Sphere(glm::vec3 _position, glm::vec3 _colour, float _shiny, float _radius);
	
	finalIntersection rayIntersect(Ray _ray) override;

	glm::vec3 findClosestPoint(Ray _Ray, float _x);
	float shortestDistance(Ray _Ray);
	glm::vec3 getNormal(glm::vec3 _point);
};

