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


	glm::vec3 pos = glm::vec3(0.0f, 0.0f, -3.0f);
	glm::vec3 colour = glm::vec3(1.0f, 0.0f, 0.0f);
	float radius = 1.0f;
	Sphere sphere = Sphere(pos, colour, radius);
	Camera camera = Camera(480, 640, 90);

	for (int i = 0; i < winSize.x; i++)
	{
		for (int j = 0; j < winSize.y; j++)
		{
			glm::ivec2 pos = { i, j };
			Ray ray = camera.getRay(pos);

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
