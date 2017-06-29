#include "Ray.h"

Ray::Ray()
{
	origin = vec3(0, 0, 0);
	direction = vec3(1, 0, 0);
}

Ray::Ray(vec3 origin, vec3 direction)
{
	this->origin = origin;
	this->direction = direction;
}

Ray::~Ray()
{
}

