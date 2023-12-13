#include "Object.h"

// Override init
void Object::init(glm::vec3 _pos, glm::vec3 _col, glm::vec3 _col2, float _shiny, float _radius, glm::vec3 _norm, float _reflectivity)
{}

// Override intersection
finalIntersection Object::rayIntersect(Ray _ray)
{
	return finalIntersection();
}
