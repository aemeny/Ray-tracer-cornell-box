#include "Sphere.h"

Sphere::Sphere(glm::vec3 _position, glm::vec3 _colour, float _radius)
{
	position = _position;
	colour = _colour;
	radius = _radius;
}

finalIntersection Sphere::rayIntersect(Ray _ray)
{
	finalIntersection info;
	info.intersectionPos = { 0,0,0 };
	info.hasIntersected = false;

	//Makes sure the sphere is not behind the camera
	if (glm::dot(position - _ray.origin, _ray.direction) < 0)
	{
		return info;
	}
	//Makes sure ray origin is not inside of the sphere
	if (glm::length(position - _ray.origin) < radius)
	{
		return info;
	}



	info.hasIntersected = true;

	return info;
}

glm::vec3 Sphere::shade(glm::vec3 intersection)
{
	//Return as red for example
	return glm::vec3(1.0, 0.0, 0.0);
}


glm::vec3 Sphere::getNormal(glm::vec3 _point)
{
	return glm::vec3();
}
