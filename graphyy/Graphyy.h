#pragma once
#include <iostream>
#include <string>
#include <map>
#include <vector>
#include <cmath>
#include <SDL.h>
#include <SDL_ttf.h>

#undef main

#ifndef GRAPHYY_DEV
#define GRAPHYY_DEV 1
#endif

#define GRAPHYY_VER "1.0"
#define WINDOW_W 160
#define WINDOW_H 90

#ifndef GraphyyFunc
double GraphyyFunc(std::vector<double> vals, bool* fail)
{
	// TODO: make sure to include some check to ensure the subscript doesnt go out of range if you write your own function!!!!
	if (vals.size() > 0) return sin(vals[0]);
	else
	{
		*fail = true;
		return 0;
	}
}
#endif