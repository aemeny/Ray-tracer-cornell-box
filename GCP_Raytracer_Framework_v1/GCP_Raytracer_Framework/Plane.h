#pragma once
#include "Object.h"

struct Plane : Object
{
public:
	~Plane();
	// Override functions form parent Object
	void init(glm::vec3 _pos, glm::vec3 _col, float _shiny, float _radius, glm::vec3 _norm, float _reflectivity) override; // Initialise override
	finalIntersection rayIntersect(Ray _ray) override; // Ray intersection with plane
};

