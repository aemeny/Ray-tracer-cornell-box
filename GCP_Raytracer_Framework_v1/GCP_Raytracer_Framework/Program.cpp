#include "Program.h"

#include <random>
#include <thread>

Program::Program() {}

Program::~Program() {}

int Program::init()
{
	// Set window size
	winSize = glm::ivec2(1200, 1200);

	// Initialises SDL and OpenGL and sets up a framebuffer
	if (!_myFramework.Init(winSize))
	{
		return -1;
	}

	//Samples for antialiasing
	sampleSize = 100;

	//Sphere default radius
	float radius = 1.5f;

	//Shiny defualt value
	float shiny = 30.0f;
	float shiny2 = 100.0f;

	glm::vec3 pos = glm::vec3(-2.0f, 2.0f, -18.0f);
	glm::vec3 colour = glm::vec3(1.0f, 1.0f, 1.0f);
	rayTracer.addObject<Sphere>(pos, colour, shiny, radius);

	pos = glm::vec3(-2.0f, -3.0f, -18.0f);
	colour = glm::vec3(0.0f, 0.0f, 1.0f);
	rayTracer.addObject<Sphere>(pos, colour, shiny, radius);

	pos = glm::vec3(3.0f, 3.0f, -18.0f);
	colour = glm::vec3(1.0f, 0.0f, 0.0f);
	rayTracer.addObject<Sphere>(pos, colour, shiny, radius);

	pos = glm::vec3(1.3f, -0.7f, -10.0f);
	colour = glm::vec3(0.0f, 1.0f, 0.0f);
	rayTracer.addObject<Sphere>(pos, colour, shiny, radius - 0.5f);

	pos = glm::vec3(0.0f, 0.0f, -25.0f);
	colour = glm::vec3(0.0f, 0.0f, 0.0f);
	glm::vec3 norm = glm::vec3(0.0f, 0.0f, 1.0f);
	rayTracer.addObject<Plane>(pos, colour, shiny2, NULL, norm);

	pos = glm::vec3(0.0f, 0.0f, 5.0f);
	colour = glm::vec3(0.0f, 0.0f, 0.0f);
	norm = glm::vec3(0.0f, 0.0f, -1.0f);
	rayTracer.addObject<Plane>(pos, colour, shiny2, NULL, norm);
	
	pos = glm::vec3(0.0f, 8.0f, 0.0f);
	colour = glm::vec3(0.8f, 0.0f, 0.8f);
	norm = glm::vec3(0.0f, -1.0f, 0.0f);
	rayTracer.addObject<Plane>(pos, colour, shiny2, NULL, norm);

	pos = glm::vec3(-8.0f, 0.0f, 0.0f);
	colour = glm::vec3(0.8f, 0.8f, 0.0f);
	norm = glm::vec3(1.0f, 0.0f, 0.0f);
	rayTracer.addObject<Plane>(pos, colour, shiny2, NULL, norm);

	pos = glm::vec3(8.0f, 0.0f, 0.0f);
	colour = glm::vec3(0.0f, 0.8f, 0.8f);
	norm = glm::vec3(-1.0f, 0.0f, 0.0f);
	rayTracer.addObject<Plane>(pos, colour, shiny2, NULL, norm);

	pos = glm::vec3(0.0f, -8.0f, 0.0f);
	colour = glm::vec3(0.0f, 0.8f, 0.0f);
	norm = glm::vec3(0.0f, 1.0f, 0.0f);
	rayTracer.addObject<Plane>(pos, colour, shiny2, NULL, norm);


	camera = std::make_shared<Camera>(winSize.y, winSize.x, 60);
}

void Program::runProgram()
{
	auto threadFunc = [&](int _xstart, int _xend, int _ystart, int _yend)
		{
			//How many samples for antialiasing
			glm::vec3 finalColour;

			for (int x = _xstart; x < _xend; x++)
			{
				for (int y = _ystart; y < _yend; y++)
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
						finalColour += rayTracer.traceRay(ray, 3, true);
					}
					//Decrease final colour to an adverage of area
					finalColour /= sampleSize;
					//Draws pixel to screen with final colour
					_myFramework.DrawPixel(glm::ivec2(x, y), finalColour);
				}
			}
		};


	std::thread threads[22];

	int xPos = 0;
	int yPos = 0;
	int index = 0;
	for (size_t i = 0; i < 10; i++)
	{
		for (size_t j = 0; j < 2; j++)
		{
			threads[index] = std::thread(threadFunc, xPos, xPos + winSize.x / 10, yPos, yPos + winSize.y / 2);
			index++;
			yPos += winSize.y / 2;
		}
		yPos = 0;
		xPos += winSize.x / 10;
	}

	for (size_t i = 0; i < index; i++)
	{
		threads[i].join();
	}

	std::cout << "DONE";

	// Pushes the framebuffer to OpenGL and renders to screen
	// Also contains an event loop that keeps the window going until it's closed
	_myFramework.ShowAndHold();
}
