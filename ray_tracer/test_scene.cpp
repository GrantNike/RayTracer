#include <vector>
#include <math.h>
#include <iostream>
#include <glm/glm.hpp>
#include <FreeImage.h>

#include "shapes.cpp"

class test{
    public:
    test(std::vector<shape*> &shapes, std::vector<light*> &lights){
        //Set up eye and screen
        eye_ray_origin = glm::vec3(960,540,-70);
        bottom_left = glm::vec3(0,0,0);
        top_right = glm::vec3(1920,1080,0);
        width = 1920;
        height = 1080;

        //Define one simple sphere
        //Sphere specific parameters
        glm::vec3 position(0,0,0);
        float radius = 3;
        //Lighting parameters
        glm::vec3 ambient(0.215, 0.1745, 0.215);
        glm::vec3 diffuse(0.9142, 0.0757, 0.1757);
        glm::vec3 specular(0.2, 0.2, 0.2);
        float spec_highlight = 5.0f;
        sphere *s = new sphere(position,radius,ambient,diffuse,specular,spec_highlight);
        shapes.push_back(s);

        //Define triangle
        /*glm::vec3 T1(960.0,590.0,120.0);
        glm::vec3 T2(910.0,490.0,120.0);
        glm::vec3 T3(710.0,490.0,120.0);
        polygon *p = new polygon(T1,T2,T3,ambient,diffuse,specular,spec_highlight);
        initial_settings.shapes.push_back(p);*/

        //Define one light
        glm::vec3 light_position(0,3,10);
        glm::vec3 intensity(1,1,1);
        light *l = new light(light_position,intensity);
        lights.push_back(l);

        //Define another light for specular reflection
        glm::vec3 light_position2(360,540,50);
        light *l2 = new light(light_position2,intensity);
        //lights.push_back(l2);
    }

    /*std::vector<shape*> get_shapes(){
        return shapes;
    }

    std::vector<light*> get_lights(){
        return lights;
    }*/

    glm::vec3 get_environment_ambient(){
        return environment_ambient;
    }

    glm::vec3 get_background_colour(){
        return background_colour;
    }

    glm::vec3 get_eye_ray_origin(){
        return eye_ray_origin;
    }

    glm::vec3 get_bottom_left(){
        return bottom_left;
    }

    glm::vec3 get_top_right(){
        return top_right;
    }

    int get_width(){
        return width;
    }

    int get_height(){
        return height;
    }

    private:
    //All the shapes in the scene
    //std::vector<shape*> shapes;
    //All the lights in the scene
    //std::vector<light*> lights;
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