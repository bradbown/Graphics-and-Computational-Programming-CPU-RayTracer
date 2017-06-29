#pragma once

#include "glm\glm.hpp"
#include "Colour.h"

#include "Object.h"

class Plane : public Object
{
public:

	Plane();
	Plane(vec3 normal, double d, Colour col);

	~Plane();

	vec3 normal;
	double distance;
	Colour colour;
	
	virtual vec3 getNormalAtIntersect(vec3 point);
	virtual double findIntersection(Ray ray);

	virtual Colour getColour();
	virtual glm::ivec4 getPixelColour();
};

