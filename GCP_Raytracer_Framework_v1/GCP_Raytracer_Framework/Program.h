#pragma once
#include "RayTracer.h"
#include "ThreadPool.h"

struct Program
{
private:
	std::shared_ptr<Camera> camera;
	std::shared_ptr<ThreadPool> threadPool;
	RayTracer rayTracer;
	GCP_Framework _myFramework;

	glm::ivec2 winSize;

	int sampleSize;
	int xPos;

public:
	Program();
	~Program();

	int init();
	void runProgram();
	void assignThreadTask();
};

