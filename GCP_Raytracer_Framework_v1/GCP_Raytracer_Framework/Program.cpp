#include "Program.h"

#include <random>
#include <thread>

Program::Program() {}

Program::~Program() {}

int Program::init()
{
	// Set window size
	winSize = glm::ivec2(900, 900);

	// Initialises SDL and OpenGL and sets up a framebuffer
	if (!_myFramework.Init(winSize))
	{
		return -1;
	}

	//Samples for antialiasing
	sampleSize = 10;

	//Sphere default radius
	float radius = 1.5f;

	//Shiny defualt value
	float shiny = 0.9f;

	glm::vec3 pos = glm::vec3(-2.0f, 2.0f, -20.0f);
	glm::vec3 colour = glm::vec3(1.0f, 1.0f, 1.0f);
	std::shared_ptr<Sphere> sphere = rayTracer.addObject<Sphere>(pos, colour, shiny, radius);

	glm::vec3 pos2 = glm::vec3(-2.0f, -3.0f, -20.0f);
	glm::vec3 colour2 = glm::vec3(0.0f, 0.0f, 1.0f);
	std::shared_ptr<Sphere> sphere2 = rayTracer.addObject<Sphere>(pos2, colour2, shiny, radius);

	glm::vec3 pos3 = glm::vec3(3.0f, 3.0f, -20.0f);
	glm::vec3 colour3 = glm::vec3(1.0f, 0.0f, 0.0f);
	std::shared_ptr<Sphere> sphere3 = rayTracer.addObject<Sphere>(pos3, colour3, shiny, radius);

	glm::vec3 pos4 = glm::vec3(1.3f, -0.7f, -15.0f);
	glm::vec3 colour4 = glm::vec3(0.0f, 1.0f, 0.0f);
	std::shared_ptr<Sphere> sphere4 = rayTracer.addObject<Sphere>(pos4, colour4, shiny, radius - 0.5f);

	glm::vec3 pos5 = glm::vec3(0.0f, 0.0f, -25.0f);
	glm::vec3 colour5 = glm::vec3(1.0f, 1.0f, 1.0f);
	glm::vec3 norm = glm::vec3(0.0f, 0.0f, 1.0f);
	std::shared_ptr<Plane> plane = rayTracer.addObject<Plane>(pos5, colour5, shiny, NULL, norm);
	
	glm::vec3 pos6 = glm::vec3(0.0f, 6.0f, 0.0f);
	glm::vec3 colour6 = glm::vec3(0.6f, 0.0f, 0.6f);
	glm::vec3 norm2 = glm::vec3(0.0f, -1.0f, 0.0f);
	std::shared_ptr<Plane> plane2 = rayTracer.addObject<Plane>(pos6, colour6, shiny, NULL, norm2);

	camera = std::make_shared<Camera>(winSize.y, winSize.x, 40);
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


	std::thread threads[10];

	int xPos = 0;
	int yPos = 0;
	int index = 0;
	for (size_t i = 0; i < 5; i++)
	{
		for (size_t j = 0; j < 2; j++)
		{
			threads[index] = std::thread(threadFunc, xPos, xPos + winSize.x / 5, yPos, yPos + winSize.y / 2);
			index++;
			yPos += winSize.y / 2;
		}
		yPos = 0;
		xPos += winSize.x / 5;
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
