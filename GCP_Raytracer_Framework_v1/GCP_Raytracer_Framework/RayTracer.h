#pragma once
#include "Sphere.h"

class RayTracer
{
private:
	std::vector<Sphere*> m_objsInScene;

public:
	glm::vec3 traceRay(Ray _ray);
	void addObject(Sphere* _obj);
};

