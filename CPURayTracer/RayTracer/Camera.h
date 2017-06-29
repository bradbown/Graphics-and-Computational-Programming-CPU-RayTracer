#pragma once

#include "Ray.h"
#include "Triangle.h"
#include "Colour.h"
#include "Object.h"
#include "Source.h"
#include <vector>
#include <iostream>
#include "glm\glm.hpp"
#include "glm\gtc\type_ptr.hpp"
#include "glm\gtc\matrix_transform.hpp"

class Camera : public Ray
{
public:
	Camera();
	Camera(vec3 pos, vec3 dir, vec3 r, vec3 d, int width, int height, std::vector<Object*> scene, std::vector<Source*> lights);
	~Camera();

	Colour colour;
	Colour red;
	glm::ivec4 currentPixel;

	int width;
	int height;
	double aspectratio;
	double ambientocclusion;
	double accuracy;
	int fov;

	vec3 position;
	vec3 direction;
	vec3 right;
	vec3 down;

	std::vector<glm::vec4>pixelArray;
	std::vector<Object*> scene_objects;
	std::vector<Source*> sceneLights;

	int nearestObjectIndex(std::vector<double> objectIntersections);

	glm::mat4 viewMatrix;
	glm::mat4 projMatrix;

	bool SaveImage(int size, int start);
	bool storeImage(std::string filename);

	Colour getColourAt(vec3 intersectionPos, vec3 intersectingRayDir, std::vector<Object*> scene_objects, std::vector<Source*> sceneLights, int nearestObjectI, double accuracy, double ambientocclusion);

	double t;

	std::vector<unsigned char> pixelData;

};

