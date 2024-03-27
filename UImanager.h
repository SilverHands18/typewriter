#pragma once
#include <iostream>
#include<fstream>
#include <string>
#include<vector>
#include<windows.h>
#include <map>
#include <functional>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL_ttf.h>
class UImanager
{
public:


	struct Vector2i //integer 2D Vector (does not use decimals since it does not use floating point numbers)
	{
		int x = 32;
		int y = 32;
	};
	struct FontStyle
	{
		int size;
		SDL_Color color;
		const char* style;

	};
	struct Button
	{
		SDL_Rect r;		
		std::string message = "Button";
		std::string action = "Save";
		std::string failAction;
		SDL_Color* RegularCol;
		SDL_Color* ActiveCol;
		SDL_Color* PressedCol;
		FontStyle* Fstyle;
		
	};
	struct panel
	{
		SDL_Rect r;
		std::string text;
		SDL_Color* col;
		FontStyle* Fstyle;
	};



	static bool PointInRect(Vector2i p, SDL_Rect* r);
	static float Lerp(float a, float b, float t);





};

