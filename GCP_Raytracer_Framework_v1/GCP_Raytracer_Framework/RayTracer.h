#pragma once
#include<list>
#include "Sphere.h"
#include "Plane.h"
#include "Camera.h"

struct RayTracer
{
private:
	std::vector<std::shared_ptr<Object>> m_objsInScene;

	glm::vec3 specularLighting(finalIntersection _info, glm::vec3 _lightDir, Ray _ray);

	finalIntersection findClosestObject(Ray _ray);

	glm::vec3 inShadowCheck(finalIntersection _info, std::list<glm::vec3> _lightPos, int _lightSamples);

public:
	glm::vec3 traceRay(Ray _ray, int _numRay, int _monteCarloItr, bool _firstRun);

	template <typename T>
	void addObject(glm::vec3 _pos, glm::vec3 _col, float _shiny, float _radius, float _reflectivity, glm::vec3 _norm = glm::vec3(NULL))
	{
		std::shared_ptr<T> rtn = std::make_shared<T>();

		rtn->init(_pos, _col, _shiny, _radius, _norm, _reflectivity);
		
		m_objsInScene.push_back(rtn);
	}
};




