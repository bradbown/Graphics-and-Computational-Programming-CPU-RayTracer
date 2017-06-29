#include "Light.h"

Light::Light()
{
	position = vec3(0, 0, 0);
	colour = Colour(1, 1, 1, 0);
}

Light::Light(vec3 pos, Colour col)
{
	this->position = pos;
	this->colour = col;
}

Light::~Light()
{
}

