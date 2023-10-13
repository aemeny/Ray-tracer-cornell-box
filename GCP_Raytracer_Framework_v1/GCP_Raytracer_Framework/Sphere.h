#pragma once
#include "Ray.h"

struct finalIntersection
{
	glm::vec3 intersectionPos;
	bool hasIntersected;
};

class Sphere
{
public:
	glm::vec3 position;
	glm::vec3 colour;
	float radius;

	Sphere(glm::vec3 _position, glm::vec3 _colour, float _radius);
	finalIntersection rayIntersect(Ray _ray);
	glm::vec3 findClosestPoint(Ray _Ray, glm::vec3 _p);
	bool hasIntersection(float _distance);
	float shortestDistance(Ray _Ray);
	glm::vec3 rayClosestIntersectionPoint(Ray _ray);
	glm::vec3 shade(glm::vec3 _intersection);
	glm::vec3 getNormal(glm::vec3 _point);
};

