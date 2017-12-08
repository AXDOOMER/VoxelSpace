// Copyright (C) 2014-2017 Alexandre-Xavier Labonté-Lamoureux
//
// This library is free software; you can redistribute it and/or modify
// it under the terms of the GNU Lesser General Public License as published by
// the Free Software Foundation; either version 3 of the License, or
// (at your option) any later version.
//
// This library is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the 
// GNU Lesser General Public License for more details.
//
// You should have received a copy of the GNU Lesser General Public License
// along with this library. If not, see <http://www.gnu.org/licenses/>.
//
// texture.cpp
// Texture loader

#include "texture.h"

#include <SDL/SDL.h>

#include <string>
#include <iostream>
#include <stdexcept>
#include <string>	// to_string
#include <utility>	// swap
using namespace std;

// Only handles 24-bit bitmaps (.BMP)
Texture::Texture(const string& Path)
{
	// Surface: Blue, Green, Red
	_Image = SDL_LoadBMP(Path.c_str());

	if (!_Image)
	{
		throw runtime_error("Error loading texture: " + Path);
	}

	_Name = Path;

	if (to_string(_Image->format->BitsPerPixel) != "24")
	{
		throw runtime_error("Error, texture is not 24 bits per pixel: " + Path);
	}
}

string Texture::Name() const
{
	return _Name;
}

void* Texture::Pixels() const
{
	return _Image->pixels;
}

Uint8 Texture::Pixel(int x, int y, int c) const
{
	Uint8 *pixels = (Uint8 *)_Image->pixels;
	unsigned int i = x + _Image->w * y;
	return pixels[i*3 + c];
}

unsigned short Texture::Width() const
{
	return _Image->w;
}

unsigned short Texture::Height() const
{
	return _Image->h;
}

Texture::~Texture() {
	SDL_FreeSurface(_Image);
}
