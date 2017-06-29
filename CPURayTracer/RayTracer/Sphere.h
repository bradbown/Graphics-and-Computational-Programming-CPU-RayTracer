#pragma once

#include "glm\glm.hpp"
#include "Colour.h"

#include "Object.h"

class Sphere : public Object
{
public:

	Sphere();
	Sphere(vec3 c, double r, Colour col);

	~Sphere();

	vec3 centre;
	double radius;
	Colour colour;

	virtual double findIntersection(Ray ray);
	virtual vec3 getNormalAtIntersect(vec3 point);

	virtual Colour getColour();
	virtual glm::ivec4 getPixelColour();
};

