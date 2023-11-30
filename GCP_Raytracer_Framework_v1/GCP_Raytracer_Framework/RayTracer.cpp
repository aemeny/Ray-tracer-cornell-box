#include "RayTracer.h"
#include <random>

glm::vec3 RayTracer::traceRay(Ray _ray, int _numRay, bool _firstRun)
{
	finalIntersection Info = findClosestObject(_ray);

	if (Info.hasIntersected && _numRay > 0)
	{
		//init values
		glm::vec3 shade{ 0 };
		glm::vec3 diffuse{ 0.8f };
		glm::vec3 lightPos = {3.0f, -3.0f, 0};
		glm::vec3 lightDir = glm::normalize(lightPos - Info.intersectionPos);

		//look for shadows and return shadow colour
		glm::vec3 shadowColour = inShadowCheck(Info, lightPos, 16);

		//Bounce multiple rays for reflection
		glm::vec3 rayDirection = _ray.direction - (2.0f * Info.surfaceNormal * glm::dot(_ray.direction, Info.surfaceNormal));
		Ray newRay = Ray(Info.intersectionPos, rayDirection); //create new ray based on reflection angle
		glm::vec3 bounceColour = traceRay(newRay, _numRay - 1, false);

		//Calculate specular lighting
		glm::vec3 specLighting = specularLighting(Info, lightDir, _ray);
		if (specLighting.x > 0.99f && specLighting.y > 0.99f && specLighting.z > 0.99f) //if in spec colour keep as white
			shade += glm::dot(lightDir, Info.surfaceNormal) * 1.0f;
		else
			shade += glm::dot(lightDir, Info.surfaceNormal) * (m_objsInScene.at(Info.objIndex)->colour + bounceColour) * diffuse;

		shade += specLighting; // Add spec lighting
		shade *= shadowColour; // Add shadow lighting

		//control shade values so they dont go out of scope
		if (shade.x < 0.0f) { shade.x = 0.0f; } else { shade.x = glm::min(shade.x, 1.0f); }
		if (shade.y < 0.0f) { shade.y = 0.0f; } else { shade.y = glm::min(shade.y, 1.0f); }
		if (shade.z < 0.0f) { shade.z = 0.0f; } else { shade.z = glm::min(shade.z, 1.0f); }

		return shade;
	}
	else if (_firstRun)
		return glm::vec3(0.0f, 0.0f, 0.2f); //if miss on first ray colour background else add nothing
	else
		return glm::vec3(0); //If no bounces left / no sphere to hit return nothing
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

glm::vec3 RayTracer::inShadowCheck(finalIntersection _info, glm::vec3 _light, int _lightSamples)
{
	glm::vec3 finalShadeColour(1);

	for (int i = 0; i < _lightSamples; i++)
	{
		float u = rand();
		float v = rand();
		float theta = 2 * M_PI * u;
		float phi = acos(2 * v - 1);
		float x = _light.x + (0.5f * sin(phi) * cos(theta));
		float y = _light.y + (0.5f * sin(phi) * sin(theta));
		float z = _light.z + (0.5f * cos(phi));

		glm::vec3 lightPos = { x, y, z };

		glm::vec3 lightDir = glm::normalize(lightPos - _info.intersectionPos);
		Ray ray = Ray(_info.intersectionPos, lightDir);

		for (int i = 0; i < m_objsInScene.size(); i++)
		{
			if (i == _info.objIndex) //if the object is itself
				continue;
			else if (m_objsInScene.at(i)->radius == NULL) //if object is a plane
				continue;

			finalIntersection info = m_objsInScene.at(i)->rayIntersect(ray);
			if (info.hasIntersected)
			{
				finalShadeColour -= glm::vec3(1) / (float)_lightSamples;
			}
		}
	}

	return finalShadeColour;
}

glm::vec3 RayTracer::specularLighting(finalIntersection _info, glm::vec3 _lightDir, Ray _ray)
{
	glm::vec3 eyeDir = _ray.direction;
	glm::vec3 halfVec = glm::normalize(_lightDir - eyeDir);

	glm::vec3 lightColour{ 0.8f };
	float shininess = 40.0f;

	if (glm::dot(_info.surfaceNormal, _lightDir) > 0)
		return lightColour * glm::pow(glm::max(glm::dot(_info.surfaceNormal, halfVec), 0.0f), shininess);
	else
		return glm::vec3(0);
	
}
