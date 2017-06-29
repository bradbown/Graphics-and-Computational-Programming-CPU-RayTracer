#include "Object.h"



Object::Object()
{
}


Object::~Object()
{
}

double Object::findIntersection(Ray ray)
{
	return 0;
}

vec3 Object::getNormalAtIntersect(vec3 point)
{
	return vec3(0, 0, 0);
}

Colour Object::getColour()
{
	return Colour(0.0f, 0.0f, 0.0f, 0);
}

glm::ivec4 Object::getPixelColour()
{
	return glm::ivec4(0.0f, 0.0f, 0.0f, 0);
}
