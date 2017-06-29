#include "Plane.h"

Plane::Plane()
{
	normal = vec3(1, 0, 0);
	distance = 0.f;
	colour = Colour(0.5f, 0.5f, 0.5f, 0);
}

Plane::Plane(vec3 normal, double d, Colour col)
{
	this->normal = normal;
	this->distance = d;
	this->colour = col;
}


Plane::~Plane()
{
}

vec3 Plane::getNormalAtIntersect(vec3 point)
{
	return normal;
}

double Plane::findIntersection(Ray ray)
{
	vec3 rayDirection = ray.direction;
	double a = rayDirection.dot(normal);

	if (a == 0)
	{
		//ray is parallel to plane
		return -1;
	}
	else
	{
		double b = normal.dot(ray.origin.add(normal.mult(distance).negative()));
		return -1 * b / a;
	}
}

Colour Plane::getColour()
{
	return colour;
}

glm::ivec4 Plane::getPixelColour()
{
	glm::vec4 _colour = glm::vec4(this->colour.red, this->colour.green, this->colour.blue, this->colour.special);
	return glm::ivec4(glm::clamp(_colour, 0.f, 1.f) * 255.f);
}




