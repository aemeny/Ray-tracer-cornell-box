#pragma once
#include "Sphere.h"
#include "Plane.h"
#include "Camera.h"

class RayTracer
{
private:
	std::vector<std::shared_ptr<Sphere>> m_objsInScene;

	glm::vec3 specularLighting(finalIntersection _info, glm::vec3 _lightDir, Ray _ray);

	finalIntersection findClosestObject(Ray _ray);

	bool inShadowCheck(finalIntersection _info, glm::vec3 _lightDir);

public:
	glm::vec3 traceRay(Ray _ray, int _numRay, bool _firstRun);
	void addObject(std::shared_ptr<Sphere> _obj);
};




