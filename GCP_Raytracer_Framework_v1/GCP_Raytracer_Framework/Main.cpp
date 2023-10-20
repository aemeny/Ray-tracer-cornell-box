#include "GCP_GFX_Framework.h"
#include "RayTracer.h"
#include "Ray.h"
#include "Sphere.h"
#include "Camera.h"
#include <random>

int main(int argc, char* argv[])
{
	// Set window size
	glm::ivec2 winSize(640, 480);

	// This will handle rendering to screen
	GCP_Framework _myFramework;

	// Initialises SDL and OpenGL and sets up a framebuffer
	if (!_myFramework.Init(winSize))
	{
		return -1;
	}

	float radius = 50;

	//glm::vec3 pos = glm::vec3(1.0f, 1.0f, -4.0f);
	glm::vec3 pos = glm::vec3(50.0f, 50.0f, -60.0f);
	glm::vec3 colour = glm::vec3(1.0f, 0.0f, 1.0f);
	Sphere sphere = Sphere(pos, colour, radius);

	//glm::vec3 pos2 = glm::vec3(-1.0f, -1.0f, -4.0f);
	glm::vec3 pos2 = glm::vec3(50.0f, 400.0f, -60.0f);
	glm::vec3 colour2 = glm::vec3(0.0f, 1.0f, 1.0f);
	Sphere sphere2 = Sphere(pos2, colour2, radius);

	//glm::vec3 pos3 = glm::vec3(-1.0f, 1.0f, -4.0f);
	glm::vec3 pos3 = glm::vec3(500.0f, 50.0f, -60.0f);
	glm::vec3 colour3 = glm::vec3(1.0f, 1.0f, 0.0f);
	Sphere sphere3 = Sphere(pos3, colour3, radius);

	//glm::vec3 pos4 = glm::vec3(1.0f, -1.0f, -4.0f);
	glm::vec3 pos4 = glm::vec3(500.0f, 400.0f, -60.0f);
	glm::vec3 colour4 = glm::vec3(0.5f, 1.0f, 0.5f);
	Sphere sphere4 = Sphere(pos4, colour4, radius);

	Camera camera = Camera(winSize.y, winSize.x, 60);
	RayTracer rayTracer;
	rayTracer.addObject(&sphere);
	rayTracer.addObject(&sphere2);
	rayTracer.addObject(&sphere3);
	rayTracer.addObject(&sphere4);

	//How many samples for antialiasing
	int sampleSize = 10;
	glm::vec3 finalColour;

	for (int i = 0; i < winSize.x; i++)
	{
		for (int j = 0; j < winSize.y; j++)
		{
			//Resets final colour after pixel is drawn
			finalColour = { 0.0f, 0.0f, 0.0f };

			for (int s = 0; s < sampleSize; s++)
			{
				//Gets a random point between 0 - 1
				float r1 = i + (float)rand() / (float)RAND_MAX;
				float r2 = j + (float)rand() / (float)RAND_MAX;

				glm::ivec2 pos = { i, j };
				Ray ray = camera.getRay(pos);

				//On intersect add colour for antialiasing
				finalColour += rayTracer.traceRay(ray);
			}
			//Decrease final colour to an adverage of area
			finalColour /= sampleSize;
			//Draws pixel to screen with final colour
			_myFramework.DrawPixel(glm::ivec2(i,j), finalColour);
		}
	}

	// Pushes the framebuffer to OpenGL and renders to screen
	// Also contains an event loop that keeps the window going until it's closed
	_myFramework.ShowAndHold();
	return 0;


}
