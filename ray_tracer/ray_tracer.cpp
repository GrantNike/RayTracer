/*
Grant Nike
6349302
January 10th
COSC 3P98 Final Project

Uses code from FreeImageTest.c from OpenGLExamples on 3P98 website
*/

#include "header.h"
#include "test_scene.cpp"

#define MAX_DEPTH 25

typedef struct {
    //All the shapes in the scene
    std::vector<shape*> shapes;
    //All the lights in the scene
    std::vector<light> lights;
    //Colour of the background of the scene
    glm::vec3 background_colour;
    //Ambient lighting coefficient of the scene
    glm::vec3 environment_ambient;
    //Size of the image
    int width = 1920;
    int height = 1080;
}glob;
glob global;

/*************************************************************************
 * MAKE SURE TO NORMALIZE ALL VECTORS!!!!!!!!!!!!
 ************************************************************************/
//Determines the intersection point of the ray vector closest to the eye
shape::intersection RayIntersection(glm::vec3 ray_vector,glm::vec3 eye_position,std::vector<shape*> &shapes){
    shape::intersection forReturn;
    forReturn.isIntersection = false;
    std::vector<shape::intersection> intersected_objects;
    //Loop through all objects in scene looking for intersections with ray_vector
    for(int i=0;i<global.shapes.size();i++){
        shape::intersection current = shapes[i]->getIntersection(ray_vector,eye_position);
        if(current.isIntersection){
            intersected_objects.push_back(current);
        }
    }
    //Check list of intersections for one closest to eye position, return this intersection
    if(intersected_objects.size()>0){
        float min = glm::distance(intersected_objects[0].hit_point,eye_position);
        forReturn = intersected_objects[0];
        for(int i=1;i<intersected_objects.size();i++){
            float distance = glm::distance(intersected_objects[0].hit_point,eye_position);
            if(distance < min){
                min = distance;
                forReturn = intersected_objects[i];
            }
        }
    }
    //if no intersections return false
    return forReturn;
}

//Determines local lighting effects at hit point
glm::vec3 shade(shape::intersection hit,glm::vec3 eye_position,glm::vec3 environment_ambient,std::vector<light> lights){
    shape* object_hit = hit.hit_object;
    //Initialize minimum values for diffuse and specular lighting
    glm::vec3 diffuse_lighting(0,0,0);
    glm::vec3 specular_lighting(0,0,0);
    //Compute diffuse and specular lighting for all light sources in scene
    for(light l:lights){
        //Only calculate diffuse and specular lighting if there is a clear line of sight from the light source to the shape
        //(creates shadows)
        //if(!object_hit.light_blocked(l,hit,global.shapes)){
            glm::vec3 light_intensity = l.get_intensity();
            glm::vec3 light_position = l.get_position();

            /*Compute diffuse lighting for current light source*/
            //Diffuse lighting coefficient
            glm::vec3 diffuse_coefficient = object_hit->get_diffuse();
            //Cosine of the angle between the vector of the light hitting the object and the normal vector of the hit point
            float cosine_A = glm::dot(glm::normalize(light_position-hit.hit_point),hit.hit_normal);
            //The diffuse lighting at the hit point from light l
            glm::vec3 diffuse = diffuse_coefficient * glm::clamp(light_intensity * cosine_A,0.0f,255.0f);
            //Take the highest intesity diffuse lighting from all light sources as the final diffuse value
            diffuse_lighting = glm::max(diffuse,diffuse_lighting);

            /*Compute specular lighting for current light source*/
            //Specular lighting coefficients
            glm::vec3 specular_coefficient = object_hit->get_specular();
            float specular_highlight = object_hit->get_highlight();
            //The ray which goes from the eye to the hit point of the shape
            glm::vec3 incident_ray = glm::normalize(eye_position-hit.hit_point);
            //The ray which reflects off the surface of the object starting with the vector of the light source hitting the hit point
            glm::vec3 reflection_ray = glm::reflect(glm::normalize(hit.hit_point-light_position),hit.hit_normal);
            //The cosine of the angle between the incident ray and the reflection ray
            float cosine_B = glm::dot(incident_ray,reflection_ray);
            //The specular lighting at the hit point from the light l
            glm::vec3 specular = specular_coefficient * glm::clamp(light_intensity * powf(cosine_B,specular_highlight),0.0f,255.0f);
            //Take the highest intesity specular lighting from all light sources as the final specular value
            specular_lighting = glm::max(specular,specular_lighting);
        //}
    }
    //Compute ambient lighting
    glm::vec3 ambient_lighting = glm::clamp(object_hit->get_ambient(),glm::vec3(0.0f),glm::vec3(255.0f));
    //Return new colour with added lighting effects
    return glm::clamp(ambient_lighting + diffuse_lighting + specular_lighting,glm::vec3(0.0f),glm::vec3(255.0f));
}

/*************************************************************************
 * MAKE SURE TO NORMALIZE ALL VECTORS!!!!!!!!!!!!
 ************************************************************************/
//Returns the colour of the given ray_vector
glm::vec3 RayTrace(glm::vec3 ray_vector,float rec_depth,glm::vec3 eye_position,std::vector<shape*> shapes,std::vector<light> lights){
    //Values will be used to get a final colour value if the ray has intersected an object
    glm::vec3 local_colour;
    glm::vec3 reflect_colour;
    glm::vec3 trans_colour;
    //Values will be set at some point
    shape::intersection hit;
    
    if(rec_depth > MAX_DEPTH){return global.background_colour;}
    else {
        hit = RayIntersection(ray_vector,eye_position,shapes); 
        if(hit.isIntersection){
            local_colour = shade(hit,eye_position,global.environment_ambient,lights);
            /*if (hit_object.isReflective()){
                glm::vec3 reflection_vector = calc_reflection(ray_vector,hit_object,hit_point,hit_normal);
                reflect_colour = RayTrace((hit_point,reflection_vector),rec_depth+1);
            }
            if(hit_object.isRefractive()){
                glm::vec3 trans_vector = calc_transmission(ray_vector,hit_object,hit_point,hit_normal);
                trans_colour = RayTrace((hit_point,trans_vector),rec_depth+1);
            }*/
            return glm::clamp(local_colour+reflect_colour+trans_colour,0.0f,255.0f);
        }
        else{return global.background_colour;}
    }
}
//Returns ray vector that goes through given screen coordinates
glm::vec3 get_ray(int x, int y,glm::vec3 eye_position){
    glm::vec3 screen_position(x,y,0);
    glm::vec3 ray_vector = glm::normalize(screen_position-eye_position);
}

pixel* create_image(){
    setup settings = setup_camera();
    int width = settings.width;
    int height = settings.height;

    pixel *image = (pixel *)malloc(height*width*sizeof(pixel *));
    glm::vec3 eye_position = settings.eye_ray_origin;
    
    for(int i=0;i<width;i++){
        for(int j=0;j<height;j++){
            //Find ray for current pixel
            glm::vec3 ray_vector = get_ray(i,j,eye_position);
            //Get colour at ray hit point
            glm::vec3 pixel_colour = RayTrace(ray_vector,1,eye_position,settings.shapes,settings.lights);
            //Set colour values in image
            image[i*global.width +j].r = glm::clamp(pixel_colour[0]*255.0f,0.0f,255.0f);
            image[i*global.width +j].g = glm::clamp(pixel_colour[1]*255.0f,0.0f,255.0f);
            image[i*global.width +j].b = glm::clamp(pixel_colour[2]*255.0f,0.0f,255.0f);
        }
    }

    return image;
}

//write image to file
//From FreeImageTest.c
void write_img(char *name, pixel *data, int width, int height) {
	FIBITMAP *image;
	RGBQUAD aPixel;

	image = FreeImage_Allocate(width, height, 24, 0, 0, 0);
	if (!image) {
		perror("FreeImage_Allocate");
		return;
	}
	for (int i = 0; i < width; i++) {
		for (int j = 0; j < height; j++) {
			aPixel.rgbRed = data[i*width +j].r;
			aPixel.rgbGreen = data[i*width +j].g;
			aPixel.rgbBlue = data[i*width +j].b;

			FreeImage_SetPixelColor(image, i, j, &aPixel);
		}
	}
	if (!FreeImage_Save(FIF_PNG, image, name, 0)) {
		perror("FreeImage_Save");
	}
	FreeImage_Unload(image);
}

void set_image(){
    pixel *image = (pixel *)malloc(global.height*global.width*sizeof(pixel *));

    for(int i=0;i<global.width;i++){
        for(int j=0;j<global.height;j++){
            image[i*global.width +j].r = glm::clamp((float)(rand()%255),0.0f,255.0f);
            image[i*global.width +j].g = glm::clamp((float)(rand()%255),0.0f,255.0f);
            image[i*global.width +j].b = glm::clamp((float)(rand()%255),0.0f,255.0f);
        }
    }

    write_img("Test Image.png",image,global.width,global.height);
}

int main (int argc , char **argv){
    global.background_colour = glm::vec3(1,1,1);

    pixel* image = create_image();
    write_img("Ray Traced Image.png",image,global.width,global.height);
    //set_image();
}