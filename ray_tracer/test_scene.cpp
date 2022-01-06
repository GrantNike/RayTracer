#include <vector>
#include <math.h>
#include <iostream>
#include <glm/glm.hpp>
#include <FreeImage.h>

#include "shapes.cpp"

struct setup{
    //All the shapes in the scene
    std::vector<shape*> shapes;
    //All the lights in the scene
    std::vector<light> lights;
    //Colour of the background of the scene
    glm::vec3 background_colour = glm::vec3(0,0,0);
    //Ambient lighting coefficient of the scene
    glm::vec3 environment_ambient = glm::vec3(0.2,0.2,0.2);
    glm::vec3 eye_ray_origin;
    glm::vec3 bottom_left;
    glm::vec3 top_right;
    //Size of the image
    int width;
    int height;
};

setup setup_camera(){
    setup initial_settings;
    //Set up eye and screen
    initial_settings.eye_ray_origin = glm::vec3(960.0,540.0,-70.0);
    initial_settings.bottom_left = glm::vec3(0.0,0.0,0.0);
    initial_settings.top_right = glm::vec3(1920.0,1080.0,0);
    initial_settings.width = 1920;
    initial_settings.height = 1080;

    //Define one simple sphere
    //Sphere specific parameters
    glm::vec3 position(960.0,540.0,100.0);
    float radius = 100.0;
    //Lighting parameters
    glm::vec3 ambient(0.0215, 0.1745, 0.0215);
    glm::vec3 diffuse(0.8142, 0.0757, 0.1757);
    glm::vec3 specular(0.2, 0.2, 0.2);
    float spec_highlight = 5.0f;
    sphere *s = new sphere(position,radius,ambient,diffuse,specular,spec_highlight);
    initial_settings.shapes.push_back(s);

    //Define one light
    glm::vec3 light_position(960.0,940.0,100.0);
    glm::vec3 intensity(1,1,1);
    light l(light_position,intensity);
    initial_settings.lights.push_back(l);

    return initial_settings;
}