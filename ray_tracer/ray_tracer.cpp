/*
Grant Nike
6349302
January 10th
COSC 3P98 Final Project
*/

#include "header.h"

#define MAX_DEPTH 25

typedef struct {
    std::vector<shape> shapes;
    std::vector<light> lights;
    glm::vec3 background_colour;
    int width = 1920;
    int height = 1080;
}glob;
glob global;

/*************************************************************************
 * MAKE SURE TO NORMALIZE ALL VECTORS!!!!!!!!!!!!
 ************************************************************************/
//Determines the intersection point of the ray vector closest to the eye
intersection RayIntersection(glm::vec3 ray_vector,glm::vec3 eye_position){
    intersection forReturn;
    forReturn.isIntersection = false;
    std::vector<intersection> intersected_objects;
    //Loop through all objects in scene looking for intersections with ray_vector
    for(int i=0;i<global.shapes.size();i++){
        intersection current = global.shapes[i].getIntersection(ray_vector,eye_position);
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
/*glm::vec3 shade(intersection hit){
    //Compute diffuse lighting 
    //Compute specular lighting 
    //Compute ambient lighting
    //Return new colour with added lighting effects
}*/
/*************************************************************************
 * MAKE SURE TO NORMALIZE ALL VECTORS!!!!!!!!!!!!
 ************************************************************************/
//Returns the colour of the given ray_vector
glm::vec3 RayTrace(glm::vec3 ray_vector,float rec_depth,glm::vec3 eye_position){
    //Values will be used to get a final colour value if the ray has intersected an object
    glm::vec3 local_colour;
    glm::vec3 reflect_colour;
    glm::vec3 trans_colour;
    //Values will be set at some point
    intersection hit;
    
    if(rec_depth > MAX_DEPTH){return global.background_colour;}
    else {
        hit = RayIntersection(ray_vector,eye_position); 
        if(hit.isIntersection){
            //local_colour = shade(hit);
            /*if (hit_object.isReflective()){
                glm::vec3 reflection_vector = calc_reflection(ray_vector,hit_object,hit_point,hit_normal);
                reflect_colour = RayTrace((hit_point,reflection_vector),rec_depth+1);
            }
            if(hit_object.isRefractive()){
                glm::vec3 trans_vector = calc_transmission(ray_vector,hit_object,hit_point,hit_normal);
                trans_colour = RayTrace((hit_point,trans_vector),rec_depth+1);
            }*/
            return glm::clamp(local_colour+reflect_colour+trans_colour,0.0f,256.0f);
        }
        else{return global.background_colour;}
    }
}

//write image to file
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
            image[i*global.width +j].r = glm::clamp((float)(rand()%700),0.0f,256.0f);
            image[i*global.width +j].g = glm::clamp((float)(rand()%700),0.0f,256.0f);
            image[i*global.width +j].b = glm::clamp((float)(rand()%700),0.0f,256.0f);
        }
    }

    write_img("Test Image.png",image,global.width,global.height);
}

int main (int argc , char **argv){
    set_image();
}