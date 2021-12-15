/*
Grant Nike
6349302
January 10th
COSC 3P98 Final Project
*/

#include <vector>
#include <math.h>
#include <iostream>
#include <glm/glm.hpp>
#include <FreeImage.h>

#include "shapes.cpp"
#include "lights.cpp"


//Returns true if the ray vector intersects the given shape at the given point
bool RayIntersection(glm::vec3 ray_vector);
//Returns the colour of the object at the given point
glm::vec3 shade(intersection hit);
//Returns the reflection vector of the given ray vector bouncing off of the given object
glm::vec3 calc_reflection(glm::vec3 ray_vector,intersection hit);
//Returns the refraction vector of the given ray bending through the given object
glm::vec3 calc_transmission(glm::vec3 ray_vector,intersection hit);