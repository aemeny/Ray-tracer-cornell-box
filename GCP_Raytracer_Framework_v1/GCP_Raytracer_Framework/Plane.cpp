#include "Plane.h"

Plane::~Plane() {}
// Initialise plane with all given values
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
// Override function that takes a ray in to check if it intersects with the given plane
finalIntersection Plane::rayIntersect(Ray _ray)
{
	finalIntersection info;
	glm::vec3 n = glm::normalize(_ray.direction);
	glm::vec3 a = _ray.origin;

	// if the dot product of the rays direction and the planes normal is >= 0 no intersection has occured and return empty info
	if (glm::dot(n, normal) >= 0)
	{
		return info;
	}

	// If intersected calculate intersect position on the plane
	float t = glm::dot(position - a, normal) / glm::dot(n, normal);
	glm::vec3 intersectPos = a + (t * n);

	//set info values and return
	info.hasIntersected = true;
	info.intersectionPos = intersectPos;
	info.surfaceNormal = normal;

	return info;
}
