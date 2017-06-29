#pragma once

#include "glm\glm.hpp"
#include "Colour.h"
#include "Ray.h"
#include "vec3.h"

class Object
{
public:

	Object();

	~Object();

	Colour colour;

	virtual double findIntersection(Ray ray);

	virtual vec3 getNormalAtIntersect(vec3 point);

	virtual Colour getColour();

	virtual glm::ivec4 getPixelColour();
};

