#include "Camera.h"
#include <GLM/gtc/matrix_transform.hpp>

/* Camera construtor
		sets init values
		creates matrixes and aspect ratio
	*/
Camera::Camera(float _height, float _width, float _FOV) 
	: m_windowHeight(_height), m_windowWidth(_width)
	, m_FOV(_FOV)
{
	float fixedNP = 0.1f; // Near Plane
	float fixedFP = 50.0f; // Far Plane
	float aspectRatio = _width / _height;
	glm::mat4 projectionMatrix = glm::perspective(glm::radians(_FOV), aspectRatio, fixedNP, fixedFP);
	m_inverseProjectionMatrix = glm::inverse(projectionMatrix);

	glm::mat4 viewMatrix = glm::mat4(1.0);
	m_inverseViewMatrix = glm::inverse(viewMatrix);

	m_transformationMatrix = m_inverseViewMatrix * m_inverseProjectionMatrix;
}

// Creates the initial ray from the camera
Ray Camera::getRay(glm::vec2 windowPos)
{
	glm::vec2 output;
	// Map pixel coordinates
	output.x = mapping(windowPos.x, 0, m_windowWidth, -1, 1);
	output.y = mapping(windowPos.y, 0, m_windowHeight, 1, -1);
	// Set planes based on position
	glm::vec4 nearPlane = { output.x, output.y, -1, 1 }; 
	glm::vec4 farPlane = { output.x, output.y, 1, 1 };

	glm::vec4 transformNear = m_transformationMatrix * nearPlane; 
	glm::vec4 transformFar = m_transformationMatrix * farPlane; 

	glm::vec3 nearProjection; 
	nearProjection = transformNear / transformNear.w;

	glm::vec3 farProjection; 
	farProjection = transformFar / transformFar.w;

	// Using projections from the screen position, create the direction for the new ray
	glm::vec3 direction = farProjection - nearProjection; 

	// Create ray starting at the near projection with the direction into the scene
	Ray ray = Ray(nearProjection, glm::normalize(direction)); 

	return ray;
}
// Maps camera space
float Camera::mapping(float xold, float xistart, float xiend, float xostart, float xoend)
{
	return (xold - xistart) * ((xoend - xostart) / (xiend - xistart)) + xostart;
}
