#pragma once
#include "Object.h"

struct Sphere : Object
{
public:
	~Sphere();
	// Override functions form parent Object
	void init(glm::vec3 _pos, glm::vec3 _col, glm::vec3 _col2, float _shiny, float _radius, glm::vec3 _norm, float _reflectivity) override; // Initialise override
	finalIntersection rayIntersect(Ray _ray) override; // Ray intersection with plane

private:
	glm::vec3 findClosestPoint(Ray _Ray, float _x); // Find closest point from ray
	float shortestDistance(Ray _Ray); // Shortest Distance from ray 
};

