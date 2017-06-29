#pragma once

#include "glm\glm.hpp"
#include "vec3.h"

class Ray
{
public:
	Ray();
	Ray(vec3 origin, vec3 direction);

	~Ray();

	vec3 origin;
	vec3 direction;

};

