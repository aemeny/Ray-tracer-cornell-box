#pragma once
#include "RayTracer.h"
#include "ThreadPool.h"

// Main class for running the program and holding values
struct Program
{
private:
	// Objects values
	std::shared_ptr<Camera> camera; 
	std::shared_ptr<ThreadPool> threadPool; 
	RayTracer rayTracer;
	GCP_Framework _myFramework;

	// Window Size
	glm::ivec2 winSize;

	// Changing values for performance / calculations
	int sampleSize;
	int numbOfRays;
	int globalIllItr;
	int shadowItr;

	// Distance rendered across screen value
	int xPos;

	// File Values
	bool fileDirtyFlag = true; // Dirty flag to create file once
	std::ofstream testResults; // File

	void runProgram(); // Run program to create threads and start calling for rendering
	void assignThreadTask(); // Task assigned to threads when needed. Calculates and sets colour for one collumn of the screen at once
public:
	~Program(); // Deconstructor

	void init(int _sampleSize, int _numOfRays, int _globalIllItr, int _shadowItr); // Initialise for changing values and objects in scene
	void writeToFile(int _time); // Writes time taken to results file using values set for this run
};

