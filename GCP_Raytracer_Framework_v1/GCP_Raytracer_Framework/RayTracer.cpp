#include "RayTracer.h"

glm::vec3 RayTracer::traceRay(Ray _ray)
{

	finalIntersection finalInfo = findClosestObject(_ray);

	if (finalInfo.hasIntersected)
	{
		//look for shadows and return black if in shadow
		glm::vec3 lightPos{ 300.0f, 300.0f, 150.0f };
		glm::vec3 lightDir = glm::normalize(lightPos - finalInfo.intersectionPos);
		
		if (inShadowCheck(finalInfo, lightDir))
			return glm::vec3(0);

		//calculate first ray intersect colour
		glm::vec3 diffuse{ 0.9f };
		glm::vec3 reflectionColour = reflectionLighting(finalInfo, _ray);

		glm::vec3 shade = glm::dot(lightDir, finalInfo.surfaceNormal) * (m_objsInScene.at(finalInfo.objIndex)->colour + reflectionColour) * diffuse;
		
		shade += specularLighting(finalInfo, lightDir, _ray);
		//shade *= reflectionColour;

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

finalIntersection RayTracer::findClosestObject(Ray _ray)
{
	finalIntersection finalInfo;
	glm::vec3 closestPoint{ 0 };
	float distance = 9999999999;
	int objIndex = 0;
	bool checkIntersect = false;

	for (int ei = 0; ei < m_objsInScene.size(); ei++)
	{
		finalIntersection info = m_objsInScene.at(ei)->rayIntersect(_ray);

		if (info.hasIntersected)
		{
			checkIntersect = true;

			if (glm::distance(_ray.origin, info.intersectionPos) < distance)
			{
				distance = glm::distance(_ray.origin, info.intersectionPos);
				closestPoint = info.intersectionPos;
				objIndex = ei;
				finalInfo = info;
			}
		}
	}

	finalInfo.objIndex = objIndex;
	finalInfo.hasIntersected = checkIntersect;

	return finalInfo;
}

bool RayTracer::inShadowCheck(finalIntersection _info, glm::vec3 _lightDir)
{
	Ray ray = Ray(_info.intersectionPos, _lightDir);
	for (int i = 0; i < m_objsInScene.size(); i++)
	{
		if (i == _info.objIndex)
			continue;

		finalIntersection info = m_objsInScene.at(i)->rayIntersect(ray);
		if (info.hasIntersected)
		{
			return true;
		}
	}

	return false;
}

glm::vec3 RayTracer::reflectionLighting(finalIntersection _info, Ray _oldRay)
{
	int reflectionBounces = 5;
	glm::vec3 finalShade(0);
	glm::vec3 lightPos{ 300.0f, 300.0f, 150.0f };

	glm::vec3 rayDirection = _oldRay.direction - (2.0f * _info.surfaceNormal * glm::dot(_oldRay.direction, _info.surfaceNormal));
	Ray ray = Ray(_info.intersectionPos, rayDirection);

	for (int i = 0; i < reflectionBounces; i++)
	{
		finalIntersection info = findClosestObject(ray);						 
																				 
		if (info.hasIntersected)												 
		{																		 
			glm::vec3 lightDir = glm::normalize(lightPos - info.intersectionPos);

			finalShade += ((m_objsInScene.at(info.objIndex)->colour)) / (float)(reflectionBounces + 1);

			rayDirection = ray.direction - (2.0f * info.surfaceNormal * glm::dot(ray.direction, info.surfaceNormal));
			ray = Ray(info.intersectionPos, rayDirection);
		}
		else
			break;
	}

	return finalShade;
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
