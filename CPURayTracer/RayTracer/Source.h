#pragma once

#include "vec3.h"
#include "Colour.h"

class Source
{
public:
	Source();
	~Source();

	virtual vec3 getLightPosition() { return vec3(0, 0, 0); }
	virtual Colour getLightColor() { return Colour(1, 1, 1, 0); }
};


