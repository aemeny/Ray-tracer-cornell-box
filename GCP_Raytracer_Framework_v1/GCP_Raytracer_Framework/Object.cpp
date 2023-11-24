#include "Object.h"

Object::Object(glm::vec3 _position, glm::vec3 _colour, float _shiny) :
	position(_position), colour(_colour), shiny(_shiny)
{}

finalIntersection Object::rayIntersect(Ray _ray)
{
	return finalIntersection();
}
