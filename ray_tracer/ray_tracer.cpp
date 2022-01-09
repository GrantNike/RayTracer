/*
Grant Nike
6349302
January 10th
COSC 3P98 Final Project

Uses code from FreeImageTest.c from OpenGLExamples on 3P98 website
*/

#include "header.h"
#include "test_scene.cpp"
#include "SceneSettings.cpp"

#define MAX_DEPTH 25

typedef struct {
    glm::vec3 lookat;
	float lookatDist;
	float xAxisAngle, yAxisAngle;
	float znear, fov;
	int renderWidth, renderHeight;
    const double PI = 3.141592653589793238463;
}glob;
glob global;

class view{
    public:
    view(glm::vec3 lookat, float lookatDist, float xAxisAngle, float yAxisAngle){
        glm::mat4 identityMatrix(1.0f);
        xRotateMatrix = glm::rotate(identityMatrix, (float)(xAxisAngle * global.PI / 180.0f), glm::vec3(1, 0, 0));
        yRotateMatrix = glm::rotate(identityMatrix, (float)(yAxisAngle * global.PI / 180.0f), glm::vec3(0, 1, 0));
        tempEye = xRotateMatrix * yRotateMatrix * glm::vec4(0, 0, lookatDist, 1);
        eye = glm::vec3(tempEye[0], tempEye[1], tempEye[2]);
        eyed = glm::normalize(lookat - eye);
        eyeOffset = eye + eyed * global.znear;
    }

    glm::mat4 xRotateMatrix;
    glm::mat4 yRotateMatrix;
    glm::vec4 tempEye;
    glm::vec3 eye;
    glm::vec3 eyed;
    glm::vec3 eyeOffset;
};

/*************************************************************************
 * MAKE SURE TO NORMALIZE ALL VECTORS!!!!!!!!!!!!
 ************************************************************************/

//Determines local lighting effects at hit point
glm::vec3 shade(std::vector<shape*> &shapes,std::vector<light*> &lights,glm::vec3 &hit_point, glm::vec3 &normal_vector, glm::vec3 &incident_vector,int shape_index){
    shape* object_hit = shapes[shape_index];
    //Initialize minimum values for diffuse and specular lighting
    glm::vec3 diffuse_lighting(0,0,0);
    glm::vec3 specular_lighting(0,0,0);
    //Compute diffuse and specular lighting for all light sources in scene
    for(auto l:lights){
        //Only calculate diffuse and specular lighting if there is a clear line of sight from the light source to the shape
        //(creates shadows)
        if(!object_hit->light_blocked(*l,hit_point,shapes)){
            glm::vec3 light_intensity = l->get_intensity();
            glm::vec3 light_position = l->get_position();

            /*Compute diffuse lighting for current light source*/
            //Diffuse lighting coefficient
            glm::vec3 diffuse_coefficient = object_hit->get_diffuse();
            //Cosine of the angle between the vector of the light hitting the object and the normal vector of the hit point
            float cosine_A = glm::dot(glm::normalize(light_position-hit_point),normal_vector);
            //The diffuse lighting at the hit point from light l
            glm::vec3 diffuse = diffuse_coefficient * glm::clamp(light_intensity * cosine_A,0.0f,1.0f);
            //Take the highest intesity diffuse lighting from all light sources as the final diffuse value
            diffuse_lighting = glm::max(diffuse,diffuse_lighting);

            /*Compute specular lighting for current light source*/
            //Specular lighting coefficients
            glm::vec3 specular_coefficient = object_hit->get_specular();
            float specular_highlight = object_hit->get_highlight();
            //The ray which reflects off the surface of the object starting with the vector of the light source hitting the hit point
            glm::vec3 reflection_ray = glm::reflect(glm::normalize(hit_point-light_position),normal_vector);
            //The cosine of the angle between the incident ray and the reflection ray
            float cosine_B = glm::dot(incident_vector,reflection_ray);
            //The specular lighting at the hit point from the light l
            glm::vec3 specular = specular_coefficient * glm::clamp(light_intensity * powf(cosine_B,specular_highlight),0.0f,1.0f);
            //Take the highest intesity specular lighting from all light sources as the final specular value
            specular_lighting = glm::max(specular,specular_lighting);
        }
    }
    //Compute ambient lighting
    glm::vec3 ambient_lighting = glm::clamp(object_hit->get_ambient(),0.0f,1.0f);
    //Return new colour with added lighting effects
    glm::vec3 final_colour = glm::clamp(ambient_lighting + diffuse_lighting + specular_lighting,glm::vec3(0),glm::vec3(1));
    //std::cout<<final_colour[0]<<","<<final_colour[1]<<","<<final_colour[2]<<std::endl;
    return final_colour;
}

//Determines the intersection point of the ray vector closest to the eye
bool RayIntersection(glm::vec3 &ray_vector,glm::vec3 &eye_position,std::vector<shape*> &shapes,glm::vec3 &hit_point, glm::vec3 &normal_vector, glm::vec3 &incident_vector, int &shape_index){
    bool is_intersection = false;
    float min = std::numeric_limits<float>::max();
    float final_shape_index;
    glm::vec3 final_hit_point;
    glm::vec3 final_normal_vector;
    glm::vec3 final_incident_vector;

    //Loop through all objects in scene looking for intersections with ray_vector
    for(int i=0;i<shapes.size();i++){
        bool current = shapes[i]->getIntersection(ray_vector,eye_position,final_hit_point,final_normal_vector,final_incident_vector);
        float distance = glm::distance(final_hit_point,eye_position);
        if(current && distance < min){
            is_intersection = true;
            min = distance;
            shape_index = i;
            hit_point = final_hit_point;
            normal_vector = final_normal_vector;
            incident_vector = final_incident_vector;
        }
    }

    return is_intersection;
}
/*************************************************************************
 * MAKE SURE TO NORMALIZE ALL VECTORS!!!!!!!!!!!!
 ************************************************************************/
//Returns the colour of the given ray_vector
glm::vec3 RayTrace(glm::vec3 &ray_vector,const int &rec_depth,glm::vec3 &eye_position,std::vector<shape*> &shapes,std::vector<light*> &lights,glm::vec3 &background_colour){
    //std::vector<shape*> shapes = settings.shapes;
    //std::vector<light*> lights = settings.lights;
    //Values will be used to get a final colour value if the ray has intersected an object
    glm::vec3 local_colour;
    glm::vec3 reflect_colour;
    glm::vec3 trans_colour;
    //Values will be set at some point
    glm::vec3 hit_point;
    glm::vec3 normal_vector;
    glm::vec3 incident_ray;
    int shape_index;
    
    if(rec_depth > MAX_DEPTH){return background_colour;}
    else {
        bool is_intersection = RayIntersection(ray_vector,eye_position,shapes,hit_point,normal_vector,incident_ray,shape_index);
        if(is_intersection){
            local_colour = shade(shapes,lights,hit_point,normal_vector,incident_ray,shape_index);
            /*if (hit_object.isReflective()){
                glm::vec3 reflection_vector = calc_reflection(ray_vector,hit_object,hit_point,hit_normal);
                reflect_colour = RayTrace((hit_point,reflection_vector),rec_depth+1);
            }
            if(hit_object.isRefractive()){
                glm::vec3 trans_vector = calc_transmission(ray_vector,hit_object,hit_point,hit_normal);
                trans_colour = RayTrace((hit_point,trans_vector),rec_depth+1);
            }*/
            return glm::clamp(local_colour,0.0f,1.0f);
        }
        else{return background_colour;}
    }
}
//Returns ray vector that goes through given screen coordinates
glm::vec3 get_ray(int x, int y,view screen, float xinc, float yinc, float half, float aspectRatio){
    float curr_x = (x + 0.5f) * xinc - half;
    float curr_y = (y + 0.5f) * yinc - half * aspectRatio;

    // Calculate the proper ray from the camera to this pixel
    glm::vec4 tween(curr_x, curr_y, 0, 1);
    // Accomodate for the position of the camera
    tween = screen.xRotateMatrix * screen.yRotateMatrix * tween;
    glm::vec3 tempTween(tween[0], tween[1], tween[2]);
    glm::vec3 ret = tempTween + screen.eyeOffset;
    ret = glm::normalize(ret - screen.eye);
    return ret;
}

pixel* create_image(std::vector<shape*> shapes, std::vector<light*> lights, test *s,view screen, float xinc, float yinc, float half, float aspectRatio){
    int width = global.renderWidth;
    int height = global.renderHeight;

    pixel *image = (pixel *)malloc(height*width*sizeof(pixel *));
    glm::vec3 eye_position = screen.eye;
    glm::vec3 background_colour = s->get_background_colour();
    glm::vec3 environment_ambient = s->get_environment_ambient();
    glm::vec3 ray_vector;
    
    for(int i=0;i<width;i++){
        for(int j=0;j<height;j++){
            //Find ray for current pixel
            ray_vector = get_ray(i,j,screen,xinc,yinc,half,aspectRatio);
            //Get colour at ray hit point
            glm::vec3 pixel_colour = RayTrace(ray_vector,1,eye_position,shapes,lights,background_colour);
            //Set colour values in image
            image[i*width +j].r = glm::clamp(pixel_colour[0]*255,0.0f,255.0f);
            image[i*width +j].g = glm::clamp(pixel_colour[1]*255,0.0f,255.0f);
            image[i*width +j].b = glm::clamp(pixel_colour[2]*255,0.0f,255.0f);
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

int main (int argc , char **argv){
    std::vector<shape *> shapes;
    std::vector<light *> lights;
    test *s = new test(shapes,lights);

    global.lookat = glm::vec3(0, 0, 0);
    global.lookatDist = 50.0f;
    global.xAxisAngle = -20.0f;
    global.yAxisAngle = 0.0f;
    global.znear = 1.0f;
    global.fov = 30.0f;
    global.renderWidth = 1920;
    global.renderHeight = 1080;

    float xinc;
	float yinc;
	float half;
	float aspectRatio;

    half = tanf((float)M_PI * (global.fov / 2.0f) / 180.0f) * global.znear;
    // How much to increase x for each pixel
    xinc = half / (global.renderWidth * 0.5f);
    // How much to increase y for each pixel
    yinc = 2 * half / global.renderWidth;
    aspectRatio = (float)global.renderHeight / global.renderWidth;
    
    view screen(global.lookat,global.lookatDist,global.xAxisAngle,global.yAxisAngle);

    pixel* image = create_image(shapes,lights,s,screen,xinc,yinc,half,aspectRatio);
    char* name = "Ray Traced Image.png";
    write_img(name,image,global.renderWidth,global.renderHeight);
    //set_image();
}