#include "RayTracer.h"

glm::vec3 RayTracer::traceRay(Ray _ray)
{
	finalIntersection info;
	glm::vec3 closestPoint;
	int whichObj = 0;

	for (size_t ei = 0; ei < m_objsInScene.size(); ei++)
	{
		info = m_objsInScene.at(ei)->rayIntersect(_ray);

		if (ei == 0)
		{
			closestPoint = info.intersectionPos;
		}

		if (info.intersectionPos.x < closestPoint.x)
		{
			closestPoint = info.intersectionPos;
			whichObj = ei;
		}
	}

	return m_objsInScene.at(whichObj)->shade(closestPoint);
}
