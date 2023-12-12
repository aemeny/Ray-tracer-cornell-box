#pragma once
#include "Object.h"

struct Plane : Object
{
public:
	~Plane();

	void init(glm::vec3 _pos, glm::vec3 _col, glm::vec3 _col2, float _shiny, float _radius, glm::vec3 _norm, float _reflectivity) override;

	finalIntersection rayIntersect(Ray _ray) override;
};

