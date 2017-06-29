#pragma once

#include "glm\glm.hpp"
#include "Colour.h"
#include "vec3.h"
#include "Source.h"

class Light : public Source
{
public:
	Light();
	Light(vec3 pos, Colour col);

	~Light();

	vec3 position;
	Colour colour;

	virtual vec3 getLightPosition() { return position; }
	virtual Colour getLightColor() { return colour; }

};

