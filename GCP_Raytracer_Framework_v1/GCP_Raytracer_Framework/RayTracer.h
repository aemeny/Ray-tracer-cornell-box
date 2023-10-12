#pragma once
#include "Sphere.h"

class RayTracer
{
	std::vector<Sphere*> m_objsInScene;

	glm::vec3 traceRay(Ray _ray);
};

