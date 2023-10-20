#include "RayTracer.h"



glm::vec3 RayTracer::traceRay(Ray _ray)
{
	finalIntersection finalInfo;
	glm::vec3 closestPoint{ 0 };
	float distance = 9999999999;
	int whichObj = 0;
	bool hasIntersected = false;

	for (size_t ei = 0; ei < m_objsInScene.size(); ei++)
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
		glm::vec3 shade = m_objsInScene.at(whichObj)->shade(closestPoint, finalInfo.surfaceNormal);
		shade += totalIllumination(finalInfo);

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

glm::vec3 RayTracer::totalIllumination(finalIntersection _info)
{
	return specularLighting(_info); // emissive + ambient + diffuse + specular
}

glm::vec3 RayTracer::specularLighting(finalIntersection _info)
{
	glm::vec3 lightDirection = glm::vec3(-1.0f, -1.5f, 1.0f);
	glm::vec3 eyeDirection = glm::vec3(0.0f, 0.0f, -1.0f);
	glm::vec3 halfVec = glm::normalize(lightDirection + eyeDirection);

	glm::vec3 lightColour = glm::vec3(1);
	//glm::vec3 surfSpecCol = glm::vec3(0.0f, 1.0f, 0.0f);
	float shininess = 40.0f;

	glm::vec3 spec = lightColour * facing(_info.surfaceNormal, lightDirection) * glm::pow(glm::max(glm::dot(_info.surfaceNormal, halfVec), 0.0f), shininess);

	return spec;
}

float RayTracer::facing(glm::vec3 _surfNorm, glm::vec3 _lightDir)
{
	if (glm::dot(_surfNorm, _lightDir) > 0)
		return 1;
	else
		return 0;
}
