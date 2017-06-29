#include "vec3.h"



vec3::vec3()
{
}

vec3::vec3(double x , double y, double z)
{
	this->x = x;
	this->y = y;
	this->z = z;
}


vec3::~vec3()
{
}

vec3 vec3::normalize()
{
	double magnitude = sqrt((x*x) + (y*y) + (z*z));
	return vec3(x/magnitude, y/magnitude, z/magnitude);
}

vec3 vec3::negative()
{
	return vec3(-x, -y, -z);
}

double vec3::dot(vec3 vec)
{
	return x*vec.x +  y*vec.y + z*vec.z;
}

vec3 vec3::cross(vec3 vec)
{
	return vec3(y*vec.z - z*vec.y, z*vec.x - x*vec.z, x*vec.y - y*vec.x);
}

vec3 vec3::add(vec3 vec)
{
	return vec3(x + vec.x, y + vec.y, z + vec.z);
}

vec3 vec3::mult(double scalar)
{
	return vec3(x*scalar, y*scalar, z*scalar);
}

double vec3::magnitude()
{
	return sqrt((x*x) + (y*y) + (z*z));
}

