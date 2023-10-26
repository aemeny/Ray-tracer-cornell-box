#pragma once
#include "Sphere.h"
#include "Camera.h"

class RayTracer
{
private:
	std::vector<Sphere*> m_objsInScene;

	glm::vec3 specularLighting(finalIntersection _info, glm::vec3 _lightDir, Ray _ray);

	float facing(glm::vec3 _surfNorm, glm::vec3 _lightDir);

public:
	glm::vec3 traceRay(Ray _ray);
	void addObject(Sphere* _obj);
};

