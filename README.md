# VoxelSpace

This is a voxel engine that works by casting rays on an heightmap.

## Controls

* Arrow keys: Move
* Space: Fly up
* C: Fly down
* Page up: Look up
* Page down: Look down
* End: Reset vertical aiming
* Escape: Quit

## Command line

* `-colormap`: Specifies the 24-bit bitmap that is used to color the ground
* `-heightmap`: Specifies the 24-bit bitmap that defines the height of each pixels
* `-geometry`: Specifies the window's resolution using two numbers, x and y
* `-delay`: Number of milliseconds to wait before drawing the next frame
* `-precision`: Amount of detail in the terrain as the pixels get farther
* `-distance`: Maximum drawing distance, the limit to which you can see

The game can be compiled with the `run.sh` script. (written in Bash)

Example of usage with command line parameters:

`./run.sh  -geometry 800 600 -delay 20 -distance 500 -precision 0.005 -colormap colormap.bmp -heightmap heightmap.bmp`

## Screenshot
![screenshot at 2017-12-07 21-17-52](https://user-images.githubusercontent.com/6194072/33748556-3befb7a2-db97-11e7-9b8e-4c914389f820.png)
