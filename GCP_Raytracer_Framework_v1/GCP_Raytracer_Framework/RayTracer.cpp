#include "RayTracer.h"

glm::vec3 RayTracer::traceRay(Ray _ray)
{
	finalIntersection finalInfo;
	glm::vec3 closestPoint{ 0 };
	float distance = 9999999999;
	int whichObj = 0;
	bool hasIntersected = false;

	for (int ei = 0; ei < m_objsInScene.size(); ei++)
	{
		finalIntersection info = m_objsInScene.at(ei)->rayIntersect(_ray);

		if (info.hasIntersected)
		{
			hasIntersected = true;

			if (glm::distance(_ray.origin, info.intersectionPos) < distance)
			{
				distance = glm::distance(_ray.origin, info.intersectionPos);
				closestPoint = info.intersectionPos;
				whichObj = ei;
				finalInfo = info;
			}
		}
	}

	if (hasIntersected)
	{
		glm::vec3 lightPos{ 600.0f, 600.0f, -120.0f };
		glm::vec3 lightDir = glm::normalize(lightPos - finalInfo.intersectionPos);
		
		Ray ray = Ray(finalInfo.intersectionPos, lightDir);
		for (int i = 0; i < m_objsInScene.size(); i++)
		{
			if (i == whichObj)
				continue;

			finalIntersection info = m_objsInScene.at(i)->rayIntersect(ray);
			if (info.hasIntersected)
			{
				return glm::vec3(0.0f, 0.0f, 0.0f);
			}
		}

		glm::vec3 diffuse{ 0.9f };
		glm::vec3 shade = glm::dot(lightDir, finalInfo.surfaceNormal) * m_objsInScene.at(whichObj)->colour * diffuse;
		shade += specularLighting(finalInfo, lightDir, _ray);

		shade.x = glm::min(shade.x, 1.0f);
		shade.y = glm::min(shade.y, 1.0f);
		shade.z = glm::min(shade.z, 1.0f);

		return shade;
	}
	else
		return glm::vec3(0.0f, 0.0f, 0.0f);
}

void RayTracer::addObject(Sphere* _obj)
{
	m_objsInScene.push_back(_obj);
}

glm::vec3 RayTracer::specularLighting(finalIntersection _info, glm::vec3 _lightDir, Ray _ray)
{
	glm::vec3 eyeDir = _ray.direction;
	glm::vec3 halfVec = glm::normalize(_lightDir - eyeDir);

	glm::vec3 lightColour{ 1.0f };
	float shininess = 40.0f;

	glm::vec3 spec = lightColour * facing(_info.surfaceNormal, _lightDir) * glm::pow(glm::max(glm::dot(_info.surfaceNormal, halfVec), 0.0f), shininess);

	return spec;
}

float RayTracer::facing(glm::vec3 _surfNorm, glm::vec3 _lightDir)
{
	if (glm::dot(_surfNorm, _lightDir) > 0)
		return 1;
	else
		return 0;
}
