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
// texture.h
// Texture loader

#ifndef _TEXTURE_H
#define _TEXTURE_H

#include <SDL/SDL.h>

#include <string>
using namespace std;

class Texture
{
private:
	string _Name;
	SDL_Surface* _Image;
public:
	Texture() = delete;
	Texture(const string& Path);
	~Texture();

	string Name() const;
	void* Pixels() const;
	Uint8 Pixel(int x, int y, int c) const;
	unsigned short Width() const;
	unsigned short Height() const;
};

#endif
