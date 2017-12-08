#!/bin/bash
g++ -g main.cpp sdlclg.cpp texture.cpp -std=c++14 -lSDL -o voxels && strip voxels && ./voxels $@
