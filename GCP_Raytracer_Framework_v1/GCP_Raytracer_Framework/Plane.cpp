#include "Plane.h"

Plane::~Plane() {}

void Plane::init(glm::vec3 _position, glm::vec3 _colour, glm::vec3 _colour2, float _shiny, float _radius, glm::vec3 _normal, float _reflectivity)
{
	position = _position;
	colour = _colour;
	colour2 = _colour2;
	shiny = _shiny;
	normal = _normal;
	radius = _radius;
	reflectivity = _reflectivity;
}

finalIntersection Plane::rayIntersect(Ray _ray)
{
	finalIntersection info;
	glm::vec3 n = glm::normalize(_ray.direction);
	glm::vec3 a = _ray.origin;

	if (glm::dot(n, normal) >= 0)
	{
		return info;
	}

	float t = glm::dot(position - a, normal) / glm::dot(n, normal);

	glm::vec3 intersectPos = a + (t * n);

	info.hasIntersected = true;
	info.intersectionPos = intersectPos;
	info.surfaceNormal = normal;

	return info;
}
