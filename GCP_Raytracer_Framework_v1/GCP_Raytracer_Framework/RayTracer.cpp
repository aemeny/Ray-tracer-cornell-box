#include "RayTracer.h"
#include <random>
#include <algorithm>

// Main function for tracing a ray and returning the colour of the given intersect point
glm::vec3 RayTracer::traceRay(Ray _ray, int _numRay, int _globalIllItr, int _shadowItr, bool _firstRun)
{
	// Using given ray check which object has been intersected with
	finalIntersection Info = findClosestObject(_ray);

	// If the ray intersected with an object and we have another ray to shoot
	if (Info.hasIntersected && _numRay > 0)
	{
		//init values
		glm::vec3 finalShade{ 0 }; // Returned colour value
		glm::vec3 bounceColour{ 0 }; // Colour of bounced rays
		glm::vec3 diffuse{ 1.0f }; // Diffuse multiplier
		std::list<glm::vec3> lightPosList = { {0.0f, -3.0f, -24.5f}, {0.0f, 0.0f, 0.0f} }; // List of poistions for multiple light sources 
		glm::vec3 globalIllCol{ 0 }; // Global illumination final colour to be added
		glm::vec3 globalIllSpecCol{ 0 }; // Global illumuination specular colour
		
		// Check for shadows and return shadow multiplier
		glm::vec3 shadowColour = inShadowCheck(Info, lightPosList, _shadowItr);

		// Bounce multiple rays for reflection
		glm::vec3 rayDirection = _ray.direction - (2.0f * Info.surfaceNormal * glm::dot(_ray.direction, Info.surfaceNormal)); // Create new ray direction based of reflection
		Ray newRay = Ray(Info.intersectionPos, rayDirection); // Create new ray using new direction and start postion
		bounceColour = (traceRay(newRay, _numRay - 1, 0, _shadowItr, false) * 0.5f) * m_objsInScene.at(Info.objIndex)->reflectivity; // Recursive function to find colour of bounced ray, multiplied by the object reflectivity 

		// Global Illumination
		for (int i = 0; i < _globalIllItr; i++)
		{
			// Find random direction to shoot ray
			glm::vec3 randDir = glm::normalize(randomPointInSphere(1.0f));

			// Make sure ray doesn't shoot backwards
			if (glm::dot(Info.surfaceNormal, randDir) < 0)
			{
				randDir *= -1.0f;
			}

			// Create A new ray for illumination and trace it to get colour of the next intersection
			Ray illuminationRay(Info.intersectionPos, randDir);
			glm::vec3 illuminationRayCol = traceRay(illuminationRay, 1, 0, _shadowItr, false) * 2.0f;

			// Add colour of intersected object with illumination ray
			globalIllCol += glm::max(glm::dot(randDir, Info.surfaceNormal), 0.0f) * illuminationRayCol * (m_objsInScene.at(Info.objIndex)->colour);

			glm::vec3 halfVec = glm::normalize(randDir - rayDirection);
			globalIllSpecCol += 0.8f * powf(glm::max(glm::dot(Info.surfaceNormal, halfVec), 0.0f), m_objsInScene.at(Info.objIndex)->shiny);
		}
		if (globalIllCol != glm::vec3(0)) // Check for divid by zero error
		{
			globalIllCol = globalIllCol / (float)_globalIllItr * 2.0f;
		}
		
		// Loop for each light in the scene
		for (glm::vec3 currLightPos : lightPosList)
		{
			// Calculate direction toward the light from the current position
			glm::vec3 lightDir = glm::normalize(currLightPos - Info.intersectionPos);
			glm::vec3 shade{ 0 };

			// Calculate specular lighting
			glm::vec3 specLighting = specularLighting(Info, lightDir, _ray);
			if (specLighting.x > 0.99f && specLighting.y > 0.99f && specLighting.z > 0.99f) //if in spec colour keep as white
				shade += glm::dot(lightDir, Info.surfaceNormal);
			else
				shade += (glm::dot(lightDir, Info.surfaceNormal) * (shadowColour + bounceColour) * diffuse) * 0.8f;

			shade += specLighting; // Add spec lighting
			shade += globalIllCol + globalIllSpecCol; // Add global illumination lighting

			// Control shade values so they dont go out of scope
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
		return glm::vec3(0.0f, 0.0f, 0.2f); // If the ray misses on first run return background colour
	else
		return glm::vec3(0); // If no bounces left / no object hit, return nothing
}

// Find closest object that the given ray intersects with
finalIntersection RayTracer::findClosestObject(Ray _ray)
{
	// Init values
	finalIntersection finalInfo;
	glm::vec3 closestPoint{ 0 };
	float distance = FLT_MAX;
	int objIndex = 0;
	bool checkIntersect = false;

	// Loop through ech object in the scene and check if they have intersected. If intersected check if its the closest in distance and save the infomation
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

// Check if the given point is in shadow from samples points and return objects colour
glm::vec3 RayTracer::inShadowCheck(finalIntersection _info, std::list <glm::vec3> _lightPos, int _lightSamples)
{
	glm::vec3 finalShadeMultiplier(0); // Final shade starts at 0 (in complete shadow)
	glm::vec3 step = m_objsInScene.at(_info.objIndex)->colour / ((float)_lightSamples * _lightPos.size());; // Step set as object colour divided by the amount of shadow sample iterations

	// Loop for ever given sample
	for (int i = 0; i < _lightSamples; i++)
	{
		// Find random point in the spherical lights radius
		glm::vec3 randPoint = randomPointInSphere(0.4f); //Parameter: Light radius

		// For each light in scene add the random offset postion to the lights position
		for(glm::vec3 j : _lightPos)
		{
			float x = j.x + randPoint.x;
			float y = j.y + randPoint.y;
			float z = j.z + randPoint.z;

			glm::vec3 ranLightPos = { x, y, z };

			// Next calculate the direction from new point in the light to the intersect position
			glm::vec3 lightDir = glm::normalize(ranLightPos - _info.intersectionPos);
			Ray ray = Ray(_info.intersectionPos, lightDir);

			// Check distance to the light from intersect position (We wont want to cast shadows form objects behind the light)
			float distanceToLight = glm::distance(j, _info.intersectionPos);

			bool intersected = false;
			for (int i = 0; i < m_objsInScene.size(); i++)
			{
				// For each object in the scene that isnt itself and infront of the light check for  intersection
				if (i != _info.objIndex) // If the object is itself
				{
					finalIntersection info = m_objsInScene.at(i)->rayIntersect(ray);

					float distanceToObject = glm::distance(_info.intersectionPos, info.intersectionPos);

					if (distanceToObject < distanceToLight) // Infront of light
					{
						if (info.hasIntersected) // Has intersected
						{
							intersected = true;
							break;
						}
					}
				}
			}
			if (!intersected) // If an object is NOT in the the way of this rays sample add step colour to final multiplier
			{
				finalShadeMultiplier += step;
			}
		}
	}

	return finalShadeMultiplier;
}

// Calculate random point in a sphere given its radius
glm::vec3 RayTracer::randomPointInSphere(float _radius)
{
	float u = 2 * ((float)rand() / (RAND_MAX)) - 1;
	float phi = 2 * M_PI * (float)rand() / (RAND_MAX);
	float xRand = (_radius * cos(phi) * (1.0f - powf(u, 2)) * 0.5f);
	float yRand = (_radius * sin(phi) * (1.0f - powf(u, 2)) * 0.5f);
	float z = u;

	return glm::vec3(xRand, yRand, z);
}

// Lighting calulation for adding specular lights
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
