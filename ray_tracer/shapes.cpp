/*
Grant Nike
6349302
January 10th
COSC 3P98 Final Project
*/

#include <string>
#include <glm/glm.hpp>
#include <vector>

#include "lights.cpp"

/*************************************************************************
 * MAKE SURE TO NORMALIZE ALL VECTORS!!!!!!!!!!!!
 ************************************************************************/
class shape{
    public:
    typedef struct{
        bool isIntersection;
        glm::vec3 hit_point;
        glm::vec3 hit_normal;
        shape* hit_object;
    }intersection;
    //Gets intersection of ray with an object in scene
    virtual intersection getIntersection(glm::vec3 ray_vector,glm::vec3 eye_position) = 0;
    //Check if there is another shape between the shape and the given light source
    bool light_blocked(light &light, intersection hit, std::vector<shape*> &shapes){
        glm::vec3 light_position = light.get_position();
        glm::vec3 light_vector = glm::normalize(hit.hit_point-light_position);
        float light_to_shape = glm::distance(hit.hit_point,light_position);
        for(shape *s:shapes){
            if(s == this) continue;
            intersection between = getIntersection(light_vector,light_position);

            if(between.isIntersection){
                float between_distance = glm::distance(between.hit_point,light_position);
                if(between_distance < light_to_shape){
                    return true;
                }
            }
        }
        return false;
    }
    bool isReflective(){
        return reflective;
    }
    bool isRefractive(){
        return refractive;
    }
    glm::vec3 get_diffuse(){
        return diffuse_coefficient;
    }
    glm::vec3 get_specular(){
        return specular_coefficient;
    }
    float get_highlight(){
        return specular_highlight_coefficient;
    }
    glm::vec3 get_ambient(){
        return ambient_coefficient;
    }
    protected:
    glm::vec3 colour;
    glm::vec3 diffuse_coefficient;
    glm::vec3 specular_coefficient;
    float specular_highlight_coefficient;
    glm::vec3 ambient_coefficient;
    bool reflective;
    bool refractive;
    float reflection_coeff;
    float refraction_index;
};

/*************************************************************************
 * MAKE SURE TO NORMALIZE ALL VECTORS!!!!!!!!!!!!
 ************************************************************************/
class sphere : public shape {
    public:
    sphere(glm::vec3 position, float radius,glm::vec3 ambient,glm::vec3 diffuse,glm::vec3 specular, float specular_highlight){
        this->position = position;
        this->radius = radius;
        radius_squared = radius * radius;
        this->ambient_coefficient = ambient;
        this->specular_coefficient = specular;
        this->specular_highlight_coefficient = specular_highlight;
        this->diffuse_coefficient = diffuse;
    }
    std::string getType(){
        return "sphere";
    }
    float getRadius(){
        return radius;
    }
    glm::vec3 getPosition(){
        return position;
    }
    intersection getIntersection(glm::vec3 ray_vector,glm::vec3 eye_position){
        intersection forReturn;
        forReturn.isIntersection = false;
        //For B and C in Quadratic equation for solving circle intersection
        float x0minusxc = eye_position[0]-position[0];
        float y0minusyc = eye_position[1]-position[1];
        float z0minuszc = eye_position[2]-position[2];
        float xd = ray_vector[0];
        float yd = ray_vector[1];
        float zd = ray_vector[2];
        float B = 2*( xd*x0minusxc + yd*y0minusyc + zd*z0minuszc);
        float C =  (x0minusxc*x0minusxc) + (y0minusyc*y0minusyc) + (z0minuszc*z0minuszc) - radius_squared;
        //Discriminant of quadratic equation
        float discriminant = B*B - 4*C;
        if(discriminant >=0){
            float t0 = (-B-sqrt(discriminant))/2.0f;
            float t1 = (-B+sqrt(discriminant))/2.0f;
            bool t0positive = t0 >= 0;
            bool t1positive = t1 >= 0;
            //t is the smallest of the positive roots of the quadratic equation
            float t;
            if(t0positive && t1positive){
                t = std::min(t0,t1);
            }
            else if(t0positive){
                t = t0;
            }
            else if(t1positive){
                t = t1;
            }
            else{
                //Return no intersection
                return forReturn;
            }
            forReturn.isIntersection = true;
            //forReturn.hit_object = this;
            //Calculate hit point of ray on sphere
            float xi = eye_position[0] + xd*t;
            float yi = eye_position[1] + yd*t;
            float zi = eye_position[2] + zd*t;
            forReturn.hit_point = glm::vec3(xi,yi,zi);
            forReturn.hit_point = glm::normalize(forReturn.hit_point);
            //Calculate normal vector at hit point
            float xnormal = (xi-position[0])/radius;
            float ynormal = (yi-position[1])/radius;
            float znormal = (zi-position[2])/radius;
            forReturn.hit_normal = glm::vec3(xnormal,ynormal,znormal);
            //Normalize normal vector
            forReturn.hit_normal = glm::normalize(forReturn.hit_normal);
            forReturn.hit_object = this;
            //Return data
            return forReturn;
        }
        else{
            //Return no intersection
            return forReturn;
        }
    }
    private:
    glm::vec3 position;
    float radius;
    float radius_squared;
};
/*************************************************************************
 * MAKE SURE TO NORMALIZE ALL VECTORS!!!!!!!!!!!!
 ************************************************************************/
class plane : public shape {
    /*
      Point P = (x1,y1,z1)
      Normal Vector = (A,B,C)
      Equation of plane = A(X-x1) +B(Y-y1) + C(Z-z1) = d
                        = AX + BY + CZ = (Ax1+By1+Cz1) <- d = dot product(point,normal vector)
    */
    public:
    plane(glm::vec3 point, glm::vec3 normal_vector,glm::vec3 ambient,glm::vec3 diffuse,glm::vec3 specular,float specular_highlight){
        this->point = point;
        this->normal_vector = normal_vector;
        d = glm::dot(point,normal_vector);
        this->ambient_coefficient = ambient;
        this->specular_coefficient = specular;
        this->specular_highlight_coefficient = specular_highlight;
        this->diffuse_coefficient = diffuse;
    }
    //For use by the polygon class
    plane(glm::vec3 point, glm::vec3 normal_vector){
        this->point = point;
        this->normal_vector = normal_vector;
        d = glm::dot(point,normal_vector);
    }
    plane(){}
    std::string getType(){
        return "plane";
    }
    glm::vec3 getPoint(){
        return point;
    }
    glm::vec3 getNormal(){
        return normal_vector;
    }
    float getD(){
        return d;
    }
    intersection getIntersection(glm::vec3 ray_vector,glm::vec3 eye_position){
        intersection forReturn;
        forReturn.isIntersection = false;
        float normalDotEye = glm::dot(normal_vector,eye_position);
        //If it equals 0, then the ray is parallel to the plane, 
        //if it is greater than 0 then the normal of the plane is pointing away from the ray
        if(normalDotEye >= 0) return forReturn;
        else{
            float t = -1.0f*(normalDotEye + d)/(glm::dot(normal_vector,ray_vector));
            //If t<0 then plane is behind ray
            if(t<0) return forReturn;
            else{
                forReturn.isIntersection = true;
                float xi = eye_position[0]+ray_vector[0]*t;
                float yi = eye_position[1]+ray_vector[1]*t;
                float zi = eye_position[2]+ray_vector[2]*t;
                forReturn.hit_point = glm::vec3(xi,yi,zi);
                forReturn.hit_normal = normal_vector;
                forReturn.hit_object = this;
                return forReturn;
            }
        }
    }
    private:
    glm::vec3 point;
    glm::vec3 normal_vector;
    float d;
};
/*************************************************************************
 * MAKE SURE TO NORMALIZE ALL VECTORS!!!!!!!!!!!!
 ************************************************************************/
class polygon : public shape {
    public:
    polygon(glm::vec3 T1,glm::vec3 T2, glm::vec3 T3, glm::vec3 ambient,glm::vec3 diffuse,glm::vec3 specular,float specular_highlight){
        this->T1 = T1;
        this->T2 = T2;
        this->T3 = T3;
        //Find normal vector of polygon using cross product of two polgon vertices
        glm::vec3 normal_vector = glm::cross(T1,T2);
        plane p(T1,normal_vector);
        this->p = p;
        this->ambient_coefficient = ambient;
        this->specular_coefficient = specular;
        this->specular_highlight_coefficient = specular_highlight;
        this->diffuse_coefficient = diffuse;
    }
    std::string getType(){
        return "polygon";
    }
    intersection getIntersection(glm::vec3 ray_vector,glm::vec3 eye_position){
        intersection forReturn;
        forReturn.isIntersection = false;
        intersection plane_intersect = p.getIntersection(ray_vector,eye_position);
        if(plane_intersect.isIntersection){
            //Polygon interior test using barycentric coordinates
            glm::vec3 P = plane_intersect.hit_point;
            glm::vec3 v0 = T2 - T1;
            glm::vec3 v1 = T3 - T1;
            float dot00 = glm::dot(v0,v0);
            float dot01 = glm::dot(v0,v1);
            float dot02 = glm::dot(v0,P);
            float dot11 = glm::dot(v1,v1);
            float dot12 = glm::dot(v1,P);
            float invDenom = 1/((dot00*dot11)-(dot01*dot01));
            float alpha = (dot11 * dot02 - dot01 * dot12) * invDenom;
            float beta = (dot00 * dot12 - dot01 * dot02) * invDenom;
            if(alpha >= 0 && alpha <= 1 && beta >= 0 && beta <= 1){
                forReturn.isIntersection = true;
                forReturn.hit_point = P;
                forReturn.hit_normal = plane_intersect.hit_normal;
                forReturn.hit_object = this;
            }
        }
        return forReturn;
    }
    private:
    //The plane the polygon is laying upon
    plane p;
    //The vertices of the polygon
    glm::vec3 T1;
    glm::vec3 T2;
    glm::vec3 T3;
};
