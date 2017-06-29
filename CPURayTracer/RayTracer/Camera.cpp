#include "Camera.h"
#include "lodepng.h"

Camera::Camera()
{
	position = vec3(0, 0, 0);
	direction = vec3(0, 0, 1);
	right = vec3(0, 0, 0);
	down = vec3(0, 0, 0);

	ambientocclusion = 0.2f;
	accuracy = 0.000001f;
	fov = 45;

	pixelData = std::vector<unsigned char>(width * height * 4);
}

Camera::Camera(vec3 pos, vec3 dir, vec3 r, vec3 d, int width, int height, std::vector<Object*> scene, std::vector<Source*> lights)
{
	position = pos;
	direction = dir;
	right = r;
	down = d;
	this->width = width;
	this->height = height;
	this->scene_objects = scene;
	this->sceneLights = lights;
	aspectratio = (double)width / (double)height;

	ambientocclusion = 0.2f;
	accuracy = 0.000001f;
	fov = 45;

	projMatrix = glm::perspective(fov * (3.14159265358979f / 180.f), ((float)width / (float)height), 0.1f, 100.f);
	
	pixelData = std::vector<unsigned char>(width * height * 4);

	colour = Colour(1, 1, 1, 0);
	red = Colour(1, 0, 0, 0);
	for (int y = 0; y < height; y++)
	{
		for (int x = 0; x < width; x++)
		{
			pixelArray.push_back(glm::vec4(colour.red, colour.green, colour.blue, 255));
		}
	}
}

Camera::~Camera()
{
	pixelArray.empty();
	pixelData.empty();
}

int Camera::nearestObjectIndex(std::vector<double> objectIntersections)
{
	int indexOfMinimumValue;

	if (objectIntersections.size() == 0)
	{
		//if there are no intersections
		return -1;
	}
	else if (objectIntersections.size() == 1)
	{
		if (objectIntersections.at(0) > 0)
		{
			return 0;
		}
		else
		{
			return -1;
		}
	}
	else
	{
		double max = 0;
		for (int i = 0; i < objectIntersections.size(); i++)
		{
			if (max < objectIntersections.at(i))
			{
				max = objectIntersections.at(i);
			}
		}
		if (max > 0)
		{
			for (int index = 0; index < objectIntersections.size(); index++)
			{
				if (objectIntersections.at(index) > 0 && objectIntersections.at(index) <= max)
				{
					max = objectIntersections.at(index);
					indexOfMinimumValue = index;
				}
			}
			return indexOfMinimumValue;
		}
		else
		{
			return -1;
		}
	}
}

bool Camera::SaveImage(int size, int start)
{
	//std::vector<unsigned char> pixelData(width * height * 4);
	
	double xamnt, yamnt;

	for (int y = 0; y < height; y++)
	{
		for (int x = start; x < (width / size) + start; x++)
		{
			if (width > height) 
			{
				// the image is wider than it is tall
				xamnt = ((x + 0.5) / width)*aspectratio - (((width - height) / (double)height) / 2);
				yamnt = ((height - y) + 0.5) / height;
			}
			else if (height > width) 
			{
				// the imager is taller than it is wide
			
				xamnt = (x + 0.5) / width;
				yamnt = (((height - y) + 0.5) / height) / aspectratio - (((height - width) / (double)width) / 2);
			}
			else 
			{
				// the image is square
				xamnt = (x + 0.5) / width;
				yamnt = ((height - y) + 0.5) / height;
			}
			vec3 camRayOri = position;
			vec3 camRayDir = direction.add(right.mult(xamnt - 0.5f).add(down.mult(yamnt - 0.5f))).normalize();
			Ray camRay(camRayOri, camRayDir);

			std::vector<double> intersections;

			for (int i = 0; i < scene_objects.size(); i++)
			{
				intersections.push_back(scene_objects[i]->findIntersection(camRay));
			}
			
			int nearestObjectI = nearestObjectIndex(intersections);

			//std::cout << nearestObjectI;

			int index = (y * width * 4 + x * 4);
			if (nearestObjectI == -1)
			{
				currentPixel = pixelArray[y * x];
			}
			else
			{
				if (intersections.at(nearestObjectI) > accuracy)
				{
					vec3 intersectionPos = camRayOri.add(camRayDir.mult(intersections.at(nearestObjectI)));
					vec3 intersectingRayDir = camRayDir;
				
					Colour colourAtIntersection = getColourAt(intersectionPos, intersectingRayDir, scene_objects, sceneLights, nearestObjectI, accuracy, ambientocclusion);
					
					glm::vec4 finalColour = glm::vec4(colourAtIntersection.getColourRed(), colourAtIntersection.getColourGreen(), colourAtIntersection.getColourBlue(), 255);
					glm::ivec4 finalPixelColour = glm::ivec4(glm::clamp(finalColour, 0.f, 1.f) * 255.f);
				
					currentPixel = finalPixelColour;
				}
				//else
				//{
				//	currentPixel = scene_objects.at(nearestObjectI)->getPixelColour();
				//}
			}
			pixelData[index + 0] = currentPixel.r;
			pixelData[index + 1] = currentPixel.g;
			pixelData[index + 2] = currentPixel.b;
			pixelData[index + 3] = 255;
		}
	}	
	return false;
}

bool Camera::storeImage(std::string filename)
{

	// we're going to encode with a state rather than a convenient function, because enforcing a color type requires setting options
	lodepng::State state;
	// input color type
	state.info_raw.colortype = LCT_RGBA;
	state.info_raw.bitdepth = 8;
	// output color type
	state.info_png.color.colortype = LCT_RGBA;
	state.info_png.color.bitdepth = 8;
	state.encoder.auto_convert = 0; // without this, it would ignore the output color type specified above and choose an optimal one instead

									//encode and save

	unsigned int error = lodepng::encode(filename, pixelData, width, height);
	//pixelData.empty();

	if (error) std::cout << "WARNING: SaveFrameBufferToPNG encoder error " << error << ": " << lodepng_error_text(error) << std::endl;
	return !error;
}

Colour Camera::getColourAt(vec3 intersectionPos, vec3 intersectingRayDir, std::vector<Object*> scene_objects, std::vector<Source*> sceneLights, int nearestObjectI, double accuracy, double ambientocclusion)
{
	
	//Colour nearestObjectColour = scene_objects.at(nearestObjectI)->getColour();
	//vec3 nearestObjectNormal = scene_objects.at(nearestObjectI)->getNormalAtIntersect(intersectionPos);
	//
	//Colour finalPixelColour = nearestObjectColour.ColourScalar(ambientocclusion);
	//
	//for (int i = 0; i < sceneLights.size(); i++)
	//{
	//	vec3 lightDirection = sceneLights.at(i)->getLightPosition().add(intersectionPos.negative()).normalize();
	//
	//	float cosine_angle = nearestObjectNormal.dot(lightDirection);
	//
	//	if (cosine_angle > 0)
	//	{
	//		//test for shadows
	//		bool shadowed = false;
	//		
	//		vec3 distanceToLight = sceneLights.at(i)->getLightPosition().add(intersectionPos.negative()).normalize();
	//		float distanceToLightMagnitude = distanceToLight.magnitude();
	//
	//		Ray shadowRay(intersectionPos, sceneLights.at(i)->getLightPosition().add(intersectionPos.negative()).normalize());
	//
	//		std::vector<double> intersectionValues;
	//
	//		for (int i = 0; i < intersectionValues.size() && shadowed == false; i++)
	//		{
	//			intersectionValues.push_back(scene_objects.at(i)->findIntersection(shadowRay));
	//		}
	//
	//		for (int i = 0; i < intersectionValues.size(); i++)
	//		{
	//			if (intersectionValues.at(i) > accuracy)
	//			{
	//				if (intersectionValues.at(i) <= distanceToLightMagnitude)
	//				{
	//					shadowed = true;
	//				}
	//				//break;
	//			}
	//			break;
	//		}
	//
	//		if (shadowed == false)
	//		{
	//			finalPixelColour = finalPixelColour.ColourAdd(nearestObjectColour.ColourMultiply(sceneLights.at(i)->getLightColor().ColourScalar(cosine_angle)));
	//			
	//			if (nearestObjectColour.special > 0 && nearestObjectColour.special <= 1)
	//			{
	//				double dot1 = nearestObjectNormal.dot(intersectingRayDir.negative());
	//				vec3 scalar1 = nearestObjectNormal.mult(dot1);
	//				vec3 add1 = scalar1.add(intersectingRayDir);
	//				vec3 scalar2 = add1.mult(2);
	//				vec3 add2 = intersectingRayDir.negative().add(scalar2);
	//				vec3 reflectionDir = add2.normalize();
	//			
	//				double specular = reflectionDir.dot(lightDirection);
	//				if (specular > 0)
	//				{
	//					specular = pow(specular, 10);
	//					finalPixelColour = finalPixelColour.ColourAdd(sceneLights.at(i)->getLightColor().ColourScalar(specular*nearestObjectColour.special));
	//				}
	//			}
	//		}
	//	}
	//}
	//
	//return finalPixelColour;


	//Colour winning_object_color = scene_objects.at(nearestObjectI)->getColour();
	//vec3 winning_object_normal = scene_objects.at(nearestObjectI)->getNormalAtIntersect(intersectionPos);
	//
	//Colour final_color = winning_object_color.ColourScalar(ambientocclusion);
	//
	////if (winning_object_color.special > 0 && winning_object_color.special <= 1) {
	//	// reflection from objects with specular intensity
	//	double dot1 = winning_object_normal.dot(intersectingRayDir.negative());
	//	vec3 scalar1 = winning_object_normal.mult(dot1);
	//	vec3 add1 = scalar1.add(intersectingRayDir);
	//	vec3 scalar2 = add1.mult(2);
	//	vec3 add2 = intersectingRayDir.negative().add(scalar2);
	//	vec3 reflection_direction = add2.normalize();
	//
	//	Ray reflection_ray(intersectionPos, reflection_direction);
	//
	//	// determine what the ray intersects with first
	//	std::vector<double> reflection_intersections;
	//
	//	for (int reflection_index = 0; reflection_index < scene_objects.size(); reflection_index++) 
	//	{
	//		reflection_intersections.push_back(scene_objects.at(reflection_index)->findIntersection(reflection_ray));
	//	}
	//
	//	int index_of_winning_object_with_reflection = nearestObjectIndex(reflection_intersections);
	//
	//	if (index_of_winning_object_with_reflection != -1) {
	//		// reflection ray missed everthing else
	//		if (reflection_intersections.at(index_of_winning_object_with_reflection) > accuracy) {
	//			// determine the position and direction at the point of intersection with the reflection ray
	//			// the ray only affects the color if it reflected off something
	//
	//			vec3 reflection_intersection_position = intersectionPos.add(reflection_direction.mult(reflection_intersections.at(index_of_winning_object_with_reflection)));
	//			vec3 reflection_intersection_ray_direction = reflection_direction;
	//
	//			Colour reflection_intersection_color = getColourAt(reflection_intersection_position, reflection_intersection_ray_direction, scene_objects, sceneLights, index_of_winning_object_with_reflection, accuracy, ambientocclusion);
	//
	//			final_color = final_color.ColourAdd(reflection_intersection_color.ColourScalar(winning_object_color.special));
	//		}
	//	}
	//}
	//
	//for (int light_index = 0; light_index < sceneLights.size(); light_index++) 
	//{
	//	vec3 light_direction = sceneLights.at(light_index)->getLightPosition().add(intersectionPos.negative()).normalize();
	//
	//	float cosine_angle = winning_object_normal.dot(light_direction);
	//
	//	if (cosine_angle > 0) 
	//	{
	//		// test for shadows
	//		bool shadowed = false;
	//
	//		vec3 distance_to_light = sceneLights.at(light_index)->getLightPosition().add(intersectionPos.negative()).normalize();
	//		float distance_to_light_magnitude = distance_to_light.magnitude();
	//
	//		Ray shadow_ray(intersectionPos, sceneLights.at(light_index)->getLightPosition().add(intersectionPos.negative()).normalize());
	//
	//		std::vector<double> secondary_intersections;
	//
	//		for (int object_index = 0; object_index < scene_objects.size() && shadowed == false; object_index++) 
	//		{
	//			secondary_intersections.push_back(scene_objects.at(object_index)->findIntersection(shadow_ray));
	//		}
	//
	//		for (int c = 0; c < secondary_intersections.size(); c++) 
	//		{
	//			if (secondary_intersections.at(c) > accuracy) 
	//			{
	//				if (secondary_intersections.at(c) <= distance_to_light_magnitude) 
	//				{
	//					shadowed = true;
	//				}
	//			}
	//			break;
	//		}
	//
	//		if (shadowed == false) 
	//		{
	//			final_color = final_color.ColourAdd(winning_object_color.ColourMultiply(sceneLights.at(light_index)->getLightColor()).ColourScalar(cosine_angle));
	//
	//			if (winning_object_color.special > 0 && winning_object_color.special <= 1) {
	//				// special [0-1]
	//				double dot1 = winning_object_normal.dot(intersectingRayDir.negative());
	//				vec3 scalar1 = winning_object_normal.mult(dot1);
	//				vec3 add1 = scalar1.add(intersectingRayDir);
	//				vec3 scalar2 = add1.mult(2);
	//				vec3 add2 = intersectingRayDir.negative().add(scalar2);
	//				vec3 reflection_direction = add2.normalize();
	//
	//				double specular = reflection_direction.dot(light_direction);
	//				if (specular > 0)
	//				{
	//					specular = pow(specular, 10);
	//					final_color = final_color.ColourAdd(sceneLights.at(light_index)->getLightColor().ColourScalar(specular*winning_object_color.special));
	//				}
	//			}
	//		}
	//	}
	//}
	//
	//return final_color.clip();
	

	Colour nearestObjectColour = scene_objects.at(nearestObjectI)->getColour();
	vec3 nearestObjectNormal = scene_objects.at(nearestObjectI)->getNormalAtIntersect(intersectionPos);
	
	Colour finalColour = nearestObjectColour.ColourScalar(ambientocclusion);
	
	for (int light_index = 0; light_index < sceneLights.size(); light_index++)
	{
		vec3 lightDir = sceneLights.at(light_index)->getLightPosition().add(intersectionPos.negative()).normalize();
	
		float cosine_angle = nearestObjectNormal.dot(lightDir);
	
		if (cosine_angle > 0)
		{
			bool shadowed = false;
			vec3 distanceToLight = sceneLights.at(light_index)->getLightPosition().add(intersectionPos.negative()).normalize();
			float distanceToLightMagnitude = distanceToLight.magnitude();
	
			Ray shadowRay(intersectionPos, sceneLights.at(light_index)->getLightPosition().add(intersectionPos.negative()).normalize());
	
			std::vector<double> secondaryIntersections;
	
			for (int object_index = 0; object_index < scene_objects.size() && shadowed == false; object_index++)
			{
				secondaryIntersections.push_back(scene_objects.at(object_index)->findIntersection(shadowRay));
			}
			
			for (int c = 0; c < secondaryIntersections.size(); c++)
			{
				if (secondaryIntersections.at(c) > accuracy)
				{
					if (secondaryIntersections.at(c) <= distanceToLightMagnitude)
					{
						shadowed = true;
					}
				}
				break;
			}
			if (shadowed == false)
			{
				finalColour = finalColour.ColourAdd(nearestObjectColour.ColourMultiply(sceneLights.at(light_index)->getLightColor()).ColourScalar(cosine_angle));
	
				if (nearestObjectColour.getColourSpecial() > 0 && nearestObjectColour.getColourSpecial() <= 1)
				{
					double dot1 = nearestObjectNormal.dot(intersectingRayDir.negative());
					vec3 scalar1 = nearestObjectNormal.mult(dot1);
					vec3 add1 = scalar1.add(intersectingRayDir);
					vec3 scalar2 = add1.mult(2);
					vec3 add2 = intersectingRayDir.negative().add(scalar2);
					vec3 reflectionDirection = add2.normalize();
	
					double specular = reflectionDirection.dot(lightDir);
					if (specular > 0)
					{
						specular = pow(specular, 10);
						finalColour = finalColour.ColourAdd(sceneLights.at(light_index)->getLightColor().ColourScalar(specular * nearestObjectColour.getColourSpecial()));
					}
				}
			}
		}
	}
	
	return finalColour.clip();
}

