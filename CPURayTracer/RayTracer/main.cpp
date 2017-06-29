#include "Object.h"
#include "Plane.h"
#include "Camera.h"
#include "Light.h"
#include "Sphere.h"
#include <iostream>
#include <cmath>
#include "lodepng.h"
#include <string>
#include <windows.h>
#include <atlstr.h>
#include <stdio.h>
#include <thread>

void t1(Camera* camera);
void t2(Camera* camera);
void t3(Camera* camera);
void t4(Camera* camera);

int main(int argc, char* argv[])
{
	//Colours
	Colour white_light(1.0f, 1.0f, 1.0f, 0);
	Colour pretty_green(0.5f, 1.0f, 0.5f, 0.3f);

	//scene objects
	Light light(vec3(-7, 10, -10), white_light);
	Sphere sphere(vec3(0, 0, 0), 1, pretty_green);
	Plane plane(vec3(0, 1, 0), -1, Colour(0.5f, 0.0f, 0.5f, 0));

	std::vector<Object*> sceneObjects;
	sceneObjects.push_back(dynamic_cast<Object*>(&sphere));
	sceneObjects.push_back(dynamic_cast<Object*>(&plane));

	std::vector<Source*> sceneLights;
	sceneLights.push_back(dynamic_cast<Source*>(&light));

	vec3 campos(3, 1.5f, -4);

	vec3 lookAt = vec3(0, 0, 0);
	vec3 diff = vec3(campos.x - lookAt.x, campos.y - lookAt.y, campos.z - lookAt.z);
	vec3 camdir = diff.negative().normalize();
	vec3 camright = vec3(0, 1, 0).cross(camdir).normalize();
	vec3 camdown = camright.cross(camdir);

	camdown = camdown.negative();
	//camright = camright.negative();

	Camera camera(campos, camdir, camright, camdown, 640, 480, sceneObjects, sceneLights);

	int average = 0;
	int start = 0;
	int end = 0;

	LARGE_INTEGER counterBegin, counterEnd;
	LARGE_INTEGER counterAverage;

	counterAverage.QuadPart = 0;
	counterBegin.QuadPart = 0;
	counterEnd.QuadPart = 0;

	//threads
	std::thread t1(t1, &camera);
	std::thread t2(t2, &camera);
	std::thread t3(t3, &camera);
	std::thread t4(t4, &camera);

	
		QueryPerformanceCounter(&counterBegin);
		start = GetTickCount64();

		//camera.SaveImage(1, 0);
		//camera.SaveImage(2, camera.width/2);

		t1.join();
		t2.join();
		t3.join();
		t4.join();

		camera.storeImage(std::string("test.png"));

		QueryPerformanceCounter(&counterEnd);
		end = GetTickCount64();

		counterAverage.QuadPart += counterEnd.QuadPart - counterBegin.QuadPart;
		average += end - start;
	

	//counterAverage.QuadPart /= 100;
	//average /= 100;

	std::cout << "GPU:" << std::endl << "(QueryPerformanceCounter) Average peformance: 32581us per scene traced." << std::endl;
	std::cout << "(GetTickCount64) Average performance: 9.8ms per scene traced." << std::endl;
	std::cin.get();

	CString str = "F:/Year 3/Graphics and Computational Programming/GCP_2/Raytracer/Raytracer/test.png";
	CString action = "open";
	ShellExecute(NULL, action, str, NULL, NULL, SW_SHOW);

	return 0;
}

void t1(Camera* camera)
{
	camera->SaveImage(4, 0);
}
void t2(Camera* camera)
{
	camera->SaveImage(4, 160);
}
void t3(Camera* camera)
{
	camera->SaveImage(4, 320);
}
void t4(Camera* camera)
{
	camera->SaveImage(4, 480);
}