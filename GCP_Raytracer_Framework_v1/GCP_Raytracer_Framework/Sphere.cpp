#include "Sphere.h"

Sphere::~Sphere() {}

void Sphere::init(glm::vec3 _position, glm::vec3 _colour, float _shiny, float _radius, glm::vec3 _normal)
{
	position = _position;
	colour = _colour;
	shiny = _shiny;
	radius = _radius;
	normal = _normal; 
}

finalIntersection Sphere::rayIntersect(Ray _ray)
{
	finalIntersection info;

	//Makes sure ray origin is not inside of the sphere
	if (glm::length(position - _ray.origin) < radius)
	{
		return info;
	}

	float d = shortestDistance(_ray);
	if (d > radius)
	{
		return info;
	}

	glm::vec3 a = _ray.origin;
	glm::vec3 n = _ray.direction;
	float x = glm::sqrt((radius * radius) - (d * d));

	float sLength = glm::dot(position - a, n) - x;
	if (sLength < 0)
	{
		return info;
	}

	glm::vec3 closestPoint = findClosestPoint(_ray, x);

	info.intersectionPos = closestPoint;
	info.hasIntersected = true;
	info.surfaceNormal = glm::normalize(closestPoint - position);

	return info;
}

float Sphere::shortestDistance(Ray _ray)
{
	return glm::length(position - _ray.origin - glm::dot(position - _ray.origin, _ray.direction) * _ray.direction);
}

glm::vec3 Sphere::findClosestPoint(Ray _ray, float _x)
{
	glm::vec3 a = _ray.origin;
	glm::vec3 n = _ray.direction;

	glm::vec3 x = a + (glm::dot(position - a, n) - _x) * n;

	return x;
}

glm::vec3 Sphere::getNormal(glm::vec3 _intersectPoint)
{
	return glm::normalize(_intersectPoint - position);
}
