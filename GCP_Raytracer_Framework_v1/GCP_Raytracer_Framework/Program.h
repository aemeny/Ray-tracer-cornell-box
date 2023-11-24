#pragma once
#include "RayTracer.h"

class Program
{
private:
	std::shared_ptr<Sphere> sphere;
	std::shared_ptr<Sphere> sphere2;
	std::shared_ptr<Sphere> sphere3;
	std::shared_ptr<Sphere> sphere4;
	std::shared_ptr<Plane> plane;

	std::shared_ptr<Camera> camera;
	RayTracer rayTracer;
	GCP_Framework _myFramework;

	glm::ivec2 winSize;

	int sampleSize;

public:
	Program();
	~Program();

	int init();
	void runProgram();
};

