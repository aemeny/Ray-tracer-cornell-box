#include "Sphere.h"

Sphere::Sphere(glm::vec3 _position, glm::vec3 _colour, float _radius)
	: position(_position), colour(_colour), radius(_radius)
{}

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

	//Check for intersection
	glm::vec3 closestPoint = findClosestPoint(_ray, position);
	float t = glm::dot((closestPoint - _ray.origin), _ray.direction) / glm::dot(_ray.direction, _ray.direction);

	if (t < 0)
	{
		return info;
	}

	//If intersected return position
	if (hasIntersection(shortestDistance(_ray)))
	{
		info.hasIntersected = true;
		glm::vec3 intersectionPoint = rayClosestIntersectionPoint(_ray);
		info.intersectionPos = intersectionPoint;
		return info;
	}
	else
	{
		return info;
	}

}

glm::vec3 Sphere::rayClosestIntersectionPoint(Ray _ray)
{
	float d = shortestDistance(_ray);
	float x = sqrtf((radius * radius) - (d * d));
	glm::vec3 intersectionPoint = _ray.origin + (glm::dot((position - _ray.origin), _ray.direction) - x) * _ray.direction;
	return intersectionPoint;
}

float Sphere::shortestDistance(Ray _ray)
{
	glm::vec3 closest_Point = findClosestPoint(_ray, position);
	float distance;

	distance = glm::length(position - closest_Point);

	return distance;
}

bool Sphere::hasIntersection(float _distance)
{
	if (_distance > radius)
	{
		return false;
	}
	else
	{
		return true;
	}
}

glm::vec3 Sphere::findClosestPoint(Ray _ray, glm::vec3 _p)
{
	glm::vec3 a = _ray.origin;
	glm::vec3 n = _ray.direction;

	glm::vec3 x = a + glm::dot((_p - a), n) * n;

	return x;
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
