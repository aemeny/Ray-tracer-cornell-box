#pragma once
#include<list>
#include "Sphere.h"
#include "Plane.h"
#include "Camera.h"

// Class used to trace rays and calculate colour of a given point
struct RayTracer
{
private:
	std::vector<std::shared_ptr<Object>> m_objsInScene; // List of objects in the scene

	glm::vec3 specularLighting(finalIntersection _info, glm::vec3 _lightDir, Ray _ray); // Calculation for adding specular lighting

	finalIntersection findClosestObject(Ray _ray); // Finding closest object a given ray intersects with

	glm::vec3 inShadowCheck(finalIntersection _info, std::list<glm::vec3> _lightPos, int _lightSamples); // Checks if given position is in shadows from light sources

	glm::vec3 randomPointInSphere(float _radius); // Calculates a random point within a sphere given its radius

public:
	glm::vec3 traceRay(Ray _ray, int _numRay, int _globalIllItr, int _shadowItr, bool _firstRun); // Main function that returns the final colour of a given point in the world

	template <typename T>
	void addObject(glm::vec3 _pos, glm::vec3 _col, glm::vec3 _col2, float _shiny, float _radius, float _reflectivity, glm::vec3 _norm = glm::vec3(NULL)) // Defualt constructor for objects
	{
		std::shared_ptr<T> rtn = std::make_shared<T>(); // Create shared pointer for object

		rtn->init(_pos, _col, _col2, _shiny, _radius, _norm, _reflectivity); // Use override function to set values for object
		
		m_objsInScene.push_back(rtn); // Add object to the list of objetc in the scene
	}
};




