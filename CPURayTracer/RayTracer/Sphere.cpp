#include "Sphere.h"


Sphere::Sphere()
{
	centre = vec3(0, 0, 0);
	radius = 1.0f;
	colour = Colour(0.5f, 0.5f, 0.5f, 0);
}

Sphere::Sphere(vec3 c, double r, Colour col)
{
	centre = c;
	this->radius = r;
	this->colour = col;
}


Sphere::~Sphere()
{
}

double Sphere::findIntersection(Ray ray)
{
	double a = 1;
	double b = (2 * (ray.origin.x - centre.x) * ray.direction.x) + (2 * (ray.origin.y - centre.y) * ray.direction.y) + (2 * (ray.origin.z - centre.z) * ray.direction.z);
	double c = pow(ray.origin.x - centre.x, 2) + pow(ray.origin.y - centre.y, 2) + pow(ray.origin.z - centre.z, 2) - (radius * radius);

	double discriminant = b * b - 4 * c;

	if (discriminant > 0)
	{
		//the ray intersects the sphere

		double root1 = ((-1 * b - sqrt(discriminant)) / 2) - 0.0000001;

		if (root1 > 0)
		{
			return root1;
		}
		else
		{
			double root2 = ((sqrt(discriminant) - b) / 2) - 0.0000001;
			return root2;
		}
	}
	else
	{
		//the ray missed the sphere
		return -1;
	}
}

vec3 Sphere::getNormalAtIntersect(vec3 point)
{
	vec3 temp = point.add(centre.negative()).normalize();
	
	return temp;
}

Colour Sphere::getColour()
{
	return colour;
}

glm::ivec4 Sphere::getPixelColour()
{
	glm::vec4 _colour = glm::vec4(this->colour.red, this->colour.green, this->colour.blue, this->colour.special);
	return glm::ivec4(glm::clamp(_colour, 0.f, 1.f) * 255.f);
}
