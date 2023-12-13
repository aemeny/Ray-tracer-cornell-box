#pragma once
#include "Ray.h"

// Camera Class
struct Camera
{
private:
	// Values for matrixies 
	glm::mat4 m_inverseViewMatrix;
	glm::mat4 m_inverseProjectionMatrix;
	glm::mat4 m_transformationMatrix;

	float m_windowHeight;
	float m_windowWidth;
	float m_FOV;

public:
	Camera(float _height, float _width, float _FOV); // Constructor
	Ray getRay(glm::vec2 windowPos); // Create ray based on window position
	float mapping(float xold, float xistart, float xiend, float xostart, float xoend); // Map postion out of pixel space
};

