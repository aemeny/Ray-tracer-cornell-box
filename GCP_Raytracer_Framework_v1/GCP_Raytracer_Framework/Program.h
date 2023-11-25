#pragma once
#include "RayTracer.h"

struct Program
{
private:
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

