#include "RayTracer.h"
#include <random>
#include <algorithm>

glm::vec3 RayTracer::traceRay(Ray _ray, int _numRay, int _globalIllItr, bool _firstRun)
{
	finalIntersection Info = findClosestObject(_ray);

	if (Info.hasIntersected && _numRay > 0)
	{
		//init values
		glm::vec3 finalShade{ 0 }; 
		glm::vec3 bounceColour{ 0 };
		glm::vec3 diffuse{ 1.0f };
		std::list<glm::vec3> lightPosList = { {3.0f, -3.0f, -1.0f}, {-3.0f, 3.0f, -1.0f} }; 
		glm::vec3 globalIllCol{ 0 };
		glm::vec3 globalIllSpecCol{ 0 };
		
		//look for shadows and return shadow colour
		glm::vec3 shadowColour = inShadowCheck(Info, lightPosList, 30);

		//Bounce multiple rays for reflection
		glm::vec3 rayDirection = _ray.direction - (2.0f * Info.surfaceNormal * glm::dot(_ray.direction, Info.surfaceNormal));
		Ray newRay = Ray(Info.intersectionPos, rayDirection); //create new ray based on reflection angle
		bounceColour = (traceRay(newRay, _numRay - 1, 0, false) * 0.5f) * m_objsInScene.at(Info.objIndex)->reflectivity;

		//Global Illumination
		for (int i = 0; i < _globalIllItr; i++)
		{
			glm::vec3 randDir = glm::normalize(randomPointInSphere(1.0f));

			if (glm::dot(Info.surfaceNormal, randDir) < 0)
			{
				randDir *= -1.0f;
			}

			Ray illuminationRay(Info.intersectionPos, randDir);
			glm::vec3 illuminationRayCol = traceRay(illuminationRay, 1, 0, false) * 2.0f;

			globalIllCol += glm::max(glm::dot(randDir, Info.surfaceNormal), 0.0f) * illuminationRayCol * (m_objsInScene.at(Info.objIndex)->colour);

			glm::vec3 halfVec = glm::normalize(randDir - rayDirection);
			globalIllSpecCol += 0.8f * powf(glm::max(glm::dot(Info.surfaceNormal, halfVec), 0.0f), m_objsInScene.at(Info.objIndex)->shiny);
		}
		if (globalIllCol != glm::vec3(0))
		{
			globalIllCol = globalIllCol / (float)_globalIllItr * 2.0f;
		}
		
		for (glm::vec3 currLightPos : lightPosList)
		{
			glm::vec3 lightDir = glm::normalize(currLightPos - Info.intersectionPos);
			glm::vec3 shade{ 0 };

			//Calculate specular lighting
			glm::vec3 specLighting = specularLighting(Info, lightDir, _ray);
			if (specLighting.x > 0.99f && specLighting.y > 0.99f && specLighting.z > 0.99f) //if in spec colour keep as white
				shade += glm::dot(lightDir, Info.surfaceNormal);
			else
				shade += (glm::dot(lightDir, Info.surfaceNormal) * (shadowColour + bounceColour) * diffuse) * 0.8f;

			shade += specLighting; // Add spec lighting
			shade += globalIllCol + globalIllSpecCol; // Add global illumination lighting

			//control shade values so they dont go out of scope
			if (shade.x < 0.0f) { shade.x = 0.0f; }
			else { shade.x = glm::min(shade.x, 1.0f); }
			if (shade.y < 0.0f) { shade.y = 0.0f; }
			else { shade.y = glm::min(shade.y, 1.0f); }
			if (shade.z < 0.0f) { shade.z = 0.0f; }
			else { shade.z = glm::min(shade.z, 1.0f); }

			finalShade += shade / (float)lightPosList.size();
		}
		return finalShade;
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
	float distance = 9999999999999.0f;
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

glm::vec3 RayTracer::inShadowCheck(finalIntersection _info, std::list <glm::vec3> _lightPos, int _lightSamples)
{
	glm::vec3 finalShadeMultiplier(0);
	glm::vec3 step = m_objsInScene.at(_info.objIndex)->colour / ((float)_lightSamples * _lightPos.size());

	for (int i = 0; i < _lightSamples; i++)
	{
		glm::vec3 randPoint = randomPointInSphere(0.8f); //Light radius

		for(glm::vec3 j : _lightPos)
		{
			float x = j.x + randPoint.x;
			float y = j.y + randPoint.y;
			float z = randPoint.z;

			glm::vec3 ranLightPos = { x, y, z };

			glm::vec3 lightDir = glm::normalize(ranLightPos - _info.intersectionPos);
			Ray ray = Ray(_info.intersectionPos, lightDir);

			bool intersected = false;
			for (int i = 0; i < m_objsInScene.size(); i++)
			{
				if (i == _info.objIndex) //if the object is itself
					continue;
				else if (m_objsInScene.at(i)->radius == NULL) //if object is a plane
					continue;

				finalIntersection info = m_objsInScene.at(i)->rayIntersect(ray);
				if (info.hasIntersected)
				{
					intersected = true;
					break;
				}
			}
			if (!intersected)
			{
				finalShadeMultiplier += step;
			}
		}
	}

	return finalShadeMultiplier;
}

glm::vec3 RayTracer::randomPointInSphere(float _radius)
{
	float u = 2 * ((float)rand() / (RAND_MAX)) - 1;
	float phi = 2 * M_PI * (float)rand() / (RAND_MAX);
	float xRand = (_radius * cos(phi) * (1.0f - powf(u, 2)) * 0.5f);
	float yRand = (_radius * sin(phi) * (1.0f - powf(u, 2)) * 0.5f);
	float z = u;

	return glm::vec3(xRand, yRand, z);
}

glm::vec3 RayTracer::specularLighting(finalIntersection _info, glm::vec3 _lightDir, Ray _ray)
{
	glm::vec3 eyeDir = _ray.direction;
	glm::vec3 halfVec = glm::normalize(_lightDir - eyeDir);

	glm::vec3 lightColour{ 0.7f };

	if (glm::dot(_info.surfaceNormal, _lightDir) > 0)
		return lightColour * glm::pow(glm::max(glm::dot(_info.surfaceNormal, halfVec), 0.0f), m_objsInScene[_info.objIndex]->shiny);
	else
		return glm::vec3(0);
	
}
