#pragma once

#include <math.h>

class vec3
{
public:
	vec3();
	vec3(double, double, double);
	~vec3();

	double x, y, z;

	vec3 normalize();
	vec3 negative();
	double dot(vec3 vec);
	vec3 cross(vec3 vec);
	vec3 add(vec3 vec);
	vec3 mult(double scalar);
	double magnitude();
};

