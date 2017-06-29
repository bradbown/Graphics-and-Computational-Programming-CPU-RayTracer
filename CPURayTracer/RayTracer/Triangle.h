#pragma once

#include "glm\glm.hpp"
#include "Ray.h"

class Triangle
{
public:
	Triangle();
	~Triangle();

	vec3 a = vec3(150, 0, -1);
	vec3 b = vec3(0, 300, -1);
	vec3 c = vec3(300, 300, -1);

};

