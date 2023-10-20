#pragma once
#include "Ray.h"

struct finalIntersection
{
	glm::vec3 intersectionPos{ 0,0,0 };
	glm::vec3 surfaceNormal{ 0,0,0 };
	bool hasIntersected{ false };
};

class Sphere
{
public:
	glm::vec3 position;
	glm::vec3 colour;
	float radius;

	Sphere(glm::vec3 _position, glm::vec3 _colour, float _radius);
	finalIntersection rayIntersect(Ray _ray);
	glm::vec3 findClosestPoint(Ray _Ray, float _x);
	float shortestDistance(Ray _Ray);
	glm::vec3 getNormal(glm::vec3 _point);
};

