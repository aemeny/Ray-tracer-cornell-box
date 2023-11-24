#include "Program.h"

#include <random>
#include <thread>

Program::Program() {}

Program::~Program() {}

int Program::init()
{
	// Set window size
	winSize = glm::ivec2(1000, 1000);

	// Initialises SDL and OpenGL and sets up a framebuffer
	if (!_myFramework.Init(winSize))
	{
		return -1;
	}

	//Samples for antialiasing
	sampleSize = 100;

	//Sphere default radius
	float radius = 0.8f;

	glm::vec3 pos = glm::vec3(-1.05f, 1.05f, -5.3f);
	//glm::vec3 pos = glm::vec3(175.0f, 175.0f, -120.0f);
	glm::vec3 colour = glm::vec3(1.0f, 1.0f, 1.0f);
	sphere = std::make_shared<Sphere>(pos, colour, radius);

	glm::vec3 pos2 = glm::vec3(-1.05f, -1.05f, -5.3f);
	//glm::vec3 pos2 = glm::vec3(175.0f, 425.0f, -120.0f);
	glm::vec3 colour2 = glm::vec3(0.0f, 0.0f, 1.0f);
	sphere2 = std::make_shared<Sphere>(pos2, colour2, radius);

	glm::vec3 pos3 = glm::vec3(1.05f, 1.05f, -5.3f);
	//glm::vec3 pos3 = glm::vec3(425.0f, 175.0f, -120.0f);
	glm::vec3 colour3 = glm::vec3(1.0f, 0.0f, 0.0f);
	sphere3 = std::make_shared<Sphere>(pos3, colour3, radius);

	glm::vec3 pos4 = glm::vec3(1.05f, -0.6f, -5.3f);
	//glm::vec3 pos4 = glm::vec3(425.0f, 350.0f, -120.0f);
	glm::vec3 colour4 = glm::vec3(0.0f, 1.0f, 0.0f);
	sphere4 = std::make_shared<Sphere>(pos4, colour4, radius - 0.2f);

	camera = std::make_shared<Camera>(winSize.y, winSize.x, 45);

	rayTracer.addObject(sphere);
	rayTracer.addObject(sphere2);
	rayTracer.addObject(sphere3);
	rayTracer.addObject(sphere4);
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
						finalColour += rayTracer.traceRay(ray, 5, true);
					}
					//Decrease final colour to an adverage of area
					finalColour /= sampleSize;
					//Draws pixel to screen with final colour
					_myFramework.DrawPixel(glm::ivec2(x, y), finalColour);
				}
			}
		};


	std::thread threads[6];

	int xPos = 0;
	int yPos = 0;
	int index = 0;
	for (size_t i = 0; i < 3; i++)
	{
		for (size_t j = 0; j < 2; j++)
		{
			threads[index] = std::thread(threadFunc, xPos, xPos + winSize.x / 3, yPos, yPos + winSize.y / 2);
			index++;
			yPos += winSize.y / 2;
		}
		yPos = 0;
		xPos += winSize.x / 3;
	}

	for (size_t i = 0; i < index; i++)
	{
		threads[i].join();
	}

	// Pushes the framebuffer to OpenGL and renders to screen
	// Also contains an event loop that keeps the window going until it's closed
	_myFramework.ShowAndHold();
}
