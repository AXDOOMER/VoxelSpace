// Copyright (C) 2013-2017  Alexandre-Xavier Labonté-Lamoureux
// Copyright (C) 2017 Sebastian Macke
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in all
// copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
// SOFTWARE.
// 
// main.cpp
// Main code of the engine, including the rendering.

#include "texture.h"	// Texture loader
#include "sdlclg.h"	// Function with French names
#include <cmath>	// cos, sin
using namespace std;

// Data Structures
struct Point
{
	float x;
	float y;
};

struct Camera
{
	float x = 512; // x position on the map
	float y = 800; // y position on the map
	float height = 0; // height of the camera
	float angle = 0.0; // direction of the camera
	float horizon = 100; // horizon position (look up and down)
	float distance = 400;   // viewable distance on map
	float scale_height = 240;
};

// Functions
auto Normalize(auto data, int min, int max)
{
	if (data < min)
		data += max;
	if (data >= max)
		data -= max;

	return data;
}

void DrawVerticalLine(int i, int ytop, const int ybuffer[], Point pleft, Texture* map)
{
    if (ytop < 0) ytop = 0;
    if (ytop > ybuffer[i]) return;

	int r = map->Pixel((int)pleft.x, (int)pleft.y, 0);
	int g = map->Pixel((int)pleft.x, (int)pleft.y, 1);
	int b = map->Pixel((int)pleft.x, (int)pleft.y, 2);

	DessinerRectangle(i, ytop, 1, ybuffer[i] - ytop, b, g, r);
}

// https://github.com/s-macke/VoxelSpace
void Render(Point p, float phi, float height, float horizon, float scale_height, float distance,
int screen_width, int screen_height, Texture* heightmap, Texture* colormap, float precision)
{
	// precalculate viewing angle parameters
	float sinphi = sin(phi);
	float cosphi = cos(phi);

	// initialize visibility array. Y position for each column on screen
	int ybuffer[screen_width] = {0};
	for (int i = 0; i < screen_width; i++)
	{
		ybuffer[i] = screen_height;
	}

	// Draw from front to the back (low z coordinate to high z coordinate)
	float dz = 1.0f;
	float z = 1.0f;
	while (z < distance)
	{
		// Find line on map. This calculation corresponds to a field of view of 90°
		Point pleft = { (-cosphi*z - sinphi*z) + p.x, ( sinphi*z - cosphi*z) + p.y};
		Point pright = { ( cosphi*z - sinphi*z) + p.x, (-sinphi*z - cosphi*z) + p.y};

		// segment the line
		float dx = (pright.x - pleft.x) / screen_width;
		float dy = (pright.y - pleft.y) / screen_width;

		float invz = 1.0f / z * scale_height;

		// Raster line and draw a vertical line for each segment
		for (int i = 0; i < screen_width; i++)
		{
			pleft.x = Normalize(pleft.x, 0, heightmap->Width());
			pleft.y = Normalize(pleft.y, 0, heightmap->Height());

			int height_on_screen = (height - heightmap->Pixel((int)pleft.x, (int)pleft.y, 0)) * invz + horizon;
			if (height_on_screen < ybuffer[i])
			{
				DrawVerticalLine(i, height_on_screen, ybuffer, pleft, colormap);
				ybuffer[i] = height_on_screen;
			}
			pleft.x += dx;
			pleft.y += dy;
		}

		// Go to next line and increase step size when you are far away
		z += dz;
		dz += precision;
	}
}

// Check for obvious errors. This will abort the execution if an error is found.
void checkDataCorrect(Camera cam, Texture* heightmap, Texture* colormap, int screen_width, int screen_height, int delay, float precision)
{
	VerifierErreur(heightmap->Width() != heightmap->Height(), "Heightmap is not square. It must be square.");

	VerifierErreur(colormap->Width() != colormap->Height(), "Colormap is not square. It must be square.");

	VerifierErreur(heightmap->Width() != colormap->Width(), "Heigthmap and colormap don't have the same dimensions.");

	VerifierErreur(screen_width < 1 || screen_height < 1, "Screen dimension invalid. At least one is null or negative.");

	VerifierErreur(cam.distance > heightmap->Width() / 2, "Visibility distance too large. This risks causing a segfault.");

	VerifierErreur(precision < 0, "Rendering precision cannot be negative.");
}

// check for command line arguments
int FindArgumentPosition(int argc, char *argv[], string Argument)
{
	for (int i = 1; i < argc; i++)
		if (strcmp(argv[i], Argument.c_str()) == 0)
			return i;
	return 0;
}

// MAIN
int main(int argc, char *argv[])
{
	int screen_width = 640;
	int screen_height = 480;
	string colormap_name = "colormap.bmp";
	string heightmap_name = "heightmap.bmp";
	int delay = 30;	// wait 30 ms
	float precision = 0.01f;

	// Check command line parameters. Will cause a 'Segmentation fault' if not used properly.
	if (FindArgumentPosition(argc, argv, "-colormap") > 0)
	{
		colormap_name = argv[(FindArgumentPosition(argc, argv, "-colormap") + 1)];
	}
	if (FindArgumentPosition(argc, argv, "-heightmap") > 0)
	{
		heightmap_name = argv[(FindArgumentPosition(argc, argv, "-heightmap") + 1)];
	}
	if (FindArgumentPosition(argc, argv, "-geometry") > 0)
	{
		screen_width = atoi(argv[(FindArgumentPosition(argc, argv, "-geometry") + 1)]);
		screen_height = atoi(argv[(FindArgumentPosition(argc, argv, "-geometry") + 2)]);
	}
	if (FindArgumentPosition(argc, argv, "-delay") > 0)
	{
		delay = atoi(argv[(FindArgumentPosition(argc, argv, "-delay") + 1)]);
	}
	if (FindArgumentPosition(argc, argv, "-precision") > 0)
	{
		precision = atof(argv[(FindArgumentPosition(argc, argv, "-precision") + 1)]);
	}

	// Create a graphic window
	InitialiserAffichage("VoxelSpace", screen_width, screen_height);

	// Load the images for the map
	Texture* colormap = new Texture(colormap_name);
	Texture* heightmap = new Texture(heightmap_name);
	const int ImageUnderGround = ChargerImage("crash.bmp");

	// Variables
	int e = EVAucun;  // Initialized to 'nothing'
	bool Quit = false;

	Camera cam;
	cam.horizon = cam.horizon * (screen_height / (float)240);
	cam.scale_height = 240 * (screen_height / (float)480);
	const int max_horizon = cam.horizon * 2;
	// Spawn player above ground
	cam.height = heightmap->Pixel((int)cam.x, (int)cam.y, 0) + 10;
	// Adjust the viewable distance, 400 is the default
	cam.distance = (heightmap->Width() / 2 < cam.distance) ? heightmap->Width() / 2 : cam.distance;
	// Change viewable distance if required
	if (FindArgumentPosition(argc, argv, "-distance") > 0)
	{
		cam.distance = atof(argv[(FindArgumentPosition(argc, argv, "-distance") + 1)]);
	}

	// Check for errors. Abort if error.
	checkDataCorrect(cam, heightmap, colormap, screen_width, screen_height, delay, precision);

    // main loop
    while (!Quit)
    {
		// Reset window
		RemplirFenetre(32, 32, 64);

		// Get the pressed keys
		e = LireEvenement();

		if (e == EVQuitter)
		{
			Quit = true;
		}
		else
		{
			// Do stuff
			switch(e)
			{
			case EVHaut:
				cam.x -= 30 * sin(cam.angle);
				cam.y -= 30 * cos(cam.angle);
				break;

			case EVBas:
				cam.x += 30 * sin(cam.angle);
				cam.y += 30 * cos(cam.angle);
				break;

			case EVGauche:
				cam.angle += 0.20f;
				break;

			case EVDroite:
				cam.angle -= 0.20f;
				break;

			case EVEspace:
				cam.height += 10.0f;
				break;

			case EV_PageUp:
				if (cam.horizon <= max_horizon)
					cam.horizon += 20.0f;
				break;

			case EV_PageDown:
				if (cam.horizon >= 0)
				cam.horizon -= 20.0f;
				break;

			case EV_End:
				cam.horizon = max_horizon / 2;
				break;

			case EV_F3:
				Screenshot();
				break;

			case EV_R:
				// Reset the player's height above ground
				cam.height = heightmap->Pixel((int)cam.x, (int)cam.y, 0) + 10;
				break;

			case EV_C:
				cam.height -= 10.0f;
				break;
			}
        }

		e = EVAucun;

		// Draw screen
		// Call the render function with the camera parameters:
		// position, viewing angle, height, horizon line position, 
		// scaling factor for the height, the largest distance, 
		// screen width and the screen height parameter
		Render( {cam.x, cam.y}, cam.angle, cam.height, cam.horizon, cam.scale_height, 
			cam.distance, screen_width, screen_height, heightmap, colormap, precision);

		// Draw message if player under ground
		cam.x = Normalize(cam.x, 0, heightmap->Width());
		cam.y = Normalize(cam.y, 0, heightmap->Height());
		if (cam.height < heightmap->Pixel((int)cam.x, (int)cam.y, 0))
		{
			AfficherImage(ImageUnderGround, 0, 0);
		}

		// Refresh window
		RafraichirFenetre();
		// Wait 30 ms (by default)
		Attendre(delay);
	}

	delete colormap;
	delete heightmap;

	// Close the window
	QuitterAffichage();
	return 0;
}
