#pragma once
#include "RayTracer.h"

class Program
{
private:
	Sphere* sphere;
	Sphere* sphere2;
	Sphere* sphere3;
	Sphere* sphere4;

	Camera* camera;
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

