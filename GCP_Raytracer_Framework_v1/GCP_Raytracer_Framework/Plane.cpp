#include "Plane.h"

Plane::Plane(glm::vec3 _position, glm::vec3 _colour, float _shiny, glm::vec3 _normal) :
	Object(_position, _colour, _shiny), normal(_normal)
{}

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
