#include "UImanager.h"



struct Vector2i;

struct Rect;
struct Button;
struct FontStyle;

//AABB test to determine if a point is in a rectangle 
bool UImanager::PointInRect(UImanager::Vector2i p,SDL_Rect* r)
{
		return (p.x > r->x)&&(p.x <= r->x + r->w) && (p.y > r->y) && (p.y <= r->y + r->h);
}
//linear interpolation 
 float UImanager::Lerp(float a, float b, float t)
{
	return a + (b - a) * t;
}


