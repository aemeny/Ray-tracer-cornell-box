#include "Program.h"

#include <random>

Program::Program() {}

Program::~Program() {}

int Program::init()
{
	// Set window size
	winSize = glm::ivec2(470, 470);

	// Initialises SDL and OpenGL and sets up a framebuffer
	if (!_myFramework.Init(winSize))
	{
		return -1;
	}

	//set seed for random
	srand(time(0));

	//set for thread loop
	xPos = 0;

	//Samples for antialiasing
	sampleSize = 30;

	//Sphere default radius
	float radius = 1.7f;

	//Default reflectivity
	float reflectivity = 1.0f; //Sphere
	float reflectivity3 = 5.0f; //Sphere 2
	float reflectivity2 = 0.0f; //Wall

	//Shiny defualt value
	float shiny = 30.0f; //Sphere
	float shiny2 = 300.0f; //Wall

	/*glm::vec3 pos = glm::vec3(-2.0f, 2.0f, -18.0f);
	glm::vec3 colour = glm::vec3(0.0f, 0.0f, 0.0f);
	rayTracer.addObject<Sphere>(pos, colour, 100.0f, radius, reflectivity3);

	pos = glm::vec3(-2.0f, -3.0f, -18.0f);
	colour = glm::vec3(0.0f, 0.0f, 0.7f);
	rayTracer.addObject<Sphere>(pos, colour, shiny, radius, reflectivity);

	pos = glm::vec3(3.0f, 3.0f, -18.0f);
	colour = glm::vec3(0.7f, 0.0f, 0.0f);
	rayTracer.addObject<Sphere>(pos, colour, shiny, radius, reflectivity);

	pos = glm::vec3(1.3f, -0.7f, -10.0f);
	colour = glm::vec3(0.0f, 0.7f, 0.0f);
	rayTracer.addObject<Sphere>(pos, colour, shiny * 2.0f, radius - 0.5f, reflectivity);*/

	glm::vec3 pos = glm::vec3(0.0f, 4.25f, -26.0f);
	glm::vec3 colour = glm::vec3(0.7f, 0.7f, 0.7f);
	rayTracer.addObject<Sphere>(pos, colour, shiny2, 3.75f, reflectivity2);

	//BACK WALL
	pos = glm::vec3(0.0f, 0.0f, -32.0f);
	colour = glm::vec3(0.1f, 0.1f, 0.6f);
	glm::vec3 norm = glm::vec3(0.0f, 0.0f, 1.0f);
	rayTracer.addObject<Plane>(pos, colour, shiny2, NULL, reflectivity2, norm);

	//FRONT WALL
	pos = glm::vec3(0.0f, 0.0f, 5.0f);
	colour = glm::vec3(0.0f, 0.0f, 0.0f);
	norm = glm::vec3(0.0f, 0.0f, -1.0f);
	rayTracer.addObject<Plane>(pos, colour, shiny2, NULL, reflectivity2, norm);

	//FLOOR
	pos = glm::vec3(0.0f, 8.0f, 0.0f);
	colour = glm::vec3(0.7f, 0.7f, 0.7f);
	norm = glm::vec3(0.0f, -1.0f, 0.0f);
	rayTracer.addObject<Plane>(pos, colour, shiny2, NULL, reflectivity2, norm);

	//LEFT WALL
	pos = glm::vec3(-8.0f, 0.0f, 0.0f);
	colour = glm::vec3(0.7f, 0.0f, 0.0f);
	norm = glm::vec3(1.0f, 0.0f, 0.0f);
	rayTracer.addObject<Plane>(pos, colour, shiny2, NULL, reflectivity2, norm);

	//RIGHT WALL
	pos = glm::vec3(8.0f, 0.0f, 0.0f);
	colour = glm::vec3(0.0f, 0.7f, 0.0f);
	norm = glm::vec3(-1.0f, 0.0f, 0.0f);
	rayTracer.addObject<Plane>(pos, colour, shiny2, NULL, reflectivity2, norm);

	//ROOF
	pos = glm::vec3(0.0f, -8.0f, 0.0f);
	colour = glm::vec3(0.8f, 0.8f, 0.8f);
	norm = glm::vec3(0.0f, 1.0f, 0.0f);
	rayTracer.addObject<Plane>(pos, colour, shiny2, NULL, reflectivity2, norm);


	camera = std::make_shared<Camera>(winSize.y, winSize.x, 45);
}

void Program::runProgram()
{
	//Thread pooling
	threadPool = std::make_shared<ThreadPool>();

	//create threads and keep them looping waiting for tasks
	threadPool->start();

	//loop across the screen width giving each thread a new line
	while (xPos <= winSize.x)
	{
		//when a thread isn't busy assign a new task
		if (!threadPool->busy())
		{
			threadPool->queueJob([this] { this->assignThreadTask(); });
		}
	}

	//scene is loaded so terminate all threads
	threadPool->stop();


	std::cout << "DONE";

	// Pushes the framebuffer to OpenGL and renders to screen
	// Also contains an event loop that keeps the window going until it's closed
	_myFramework.ShowAndHold();
}

void Program::assignThreadTask()
{
	//How many samples for antialiasing
	glm::vec3 finalColour;
	
	xPos++;
	int x = xPos;

	srand(time(0) + xPos);

	for (int y = 0; y < winSize.y; y++)
	{
		//Resets final colour after pixel is drawn
		finalColour = { 0.0f, 0.0f, 0.0f };

		for (int s = 0; s < sampleSize; s++)
		{
			//Gets a random point between 0 - 1
			float r1 = (float)x + (float)rand() / (float)RAND_MAX;
			float r2 = (float)y + (float)rand() / (float)RAND_MAX;

			glm::vec2 pos = { r1, r2 };
			Ray ray = camera->getRay(pos);

			//On intersect add colour for antialiasing
			finalColour += rayTracer.traceRay(ray, 1, 50, true); //ray to pass, numRays, monteCarloItr, firstRun
		}
		//Decrease final colour to an adverage of area
		finalColour /= sampleSize;
		//Draws pixel to screen with final colour
		_myFramework.DrawPixel(glm::ivec2(x, y), finalColour);
	}

	//print percentage complete as threads finish
	std::cout << ((float)x / (float)winSize.x) * 100.0f << "%" << std::endl;
}
