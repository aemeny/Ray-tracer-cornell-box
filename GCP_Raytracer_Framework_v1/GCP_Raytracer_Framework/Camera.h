#pragma once
#include "Ray.h"

class Camera
{
private:
	glm::mat4 m_inverseViewMatrix;
	glm::mat4 m_inverseProjectionMatrix;
	glm::mat4 m_transformationMatrix;

	float m_windowHeight;
	float m_windowWidth;
	float m_FOV;

public:
	Camera(float _height, float _width, float _FOV);
	Ray getRay(glm::ivec2 windowPos);
	float mapping(float xold, float xistart, float xiend, float xostart, float xoend);
};

