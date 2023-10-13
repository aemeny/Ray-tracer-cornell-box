#include "GCP_GFX_Framework.h"
#include "RayTracer.h"
#include "Ray.h"
#include "Sphere.h"
#include "Camera.h"

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


	glm::vec3 pos = glm::vec3(winSize.x / 2, winSize.y / 2, -60);
	glm::vec3 colour = glm::vec3(1.0f, 0.0f, 0.0f);
	float radius = 50;
	Sphere sphere = Sphere(pos, colour, radius);

	for (int i = 0; i < winSize.x; i++)
	{
		for (int j = 0; j < winSize.y; j++)
		{
			Ray ray = Ray(glm::vec3(i, j, 0), glm::vec3(0, 0, -1));

			finalIntersection info;
			info = sphere.rayIntersect(ray);

			if (info.hasIntersected)
			{
				// Draws a single pixel
				_myFramework.DrawPixel(glm::ivec2(i, j), sphere.shade(info.intersectionPos));
			}

		}
	}

	// Pushes the framebuffer to OpenGL and renders to screen
	// Also contains an event loop that keeps the window going until it's closed
	_myFramework.ShowAndHold();
	return 0;


}
