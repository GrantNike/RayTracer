/*
Grant Nike
6349302
January 10th
COSC 3P98 Final Project
*/

#include <string>
#include <glm/glm.hpp>

/*************************************************************************
 * MAKE SURE TO NORMALIZE ALL VECTORS!!!!!!!!!!!!
 ************************************************************************/
class shape{
    public:
    //Gives the type of the shape
    std::string getType();
    intersection getIntersection(glm::vec3 ray_vector,glm::vec3 eye_position);
    bool isReflective(){
        return reflective;
    }
    bool isRefractive(){
        return refractive;
    }
    private:
    glm::vec3 colour;
    glm::vec3 diffuse;
    glm::vec3 specular;
    glm::vec3 ambient;
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
    sphere(glm::vec3 position, float radius){
        this->position = position;
        this->radius = radius;
        radius_squared = radius * radius;
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
            forReturn.hit_object = this;
            //Calculate hit point of ray on sphere
            float xi = eye_position[0] + xd*t;
            float yi = eye_position[1] + yd*t;
            float zi = eye_position[2] + zd*t;
            forReturn.hit_point = glm::vec3(xi,yi,zi);
            //Calculate normal vector at hit point
            float xnormal = (xi-position[0])/radius;
            float ynormal = (yi-position[1])/radius;
            float znormal = (zi-position[2])/radius;
            forReturn.hit_normal = glm::vec3(xnormal,ynormal,znormal);
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
    plane(glm::vec3 point, glm::vec3 normal_vector){
        this->point = point;
        this->normal_vector = normal_vector;
        d = glm::dot(point,normal_vector);
    }
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
    polygon(glm::vec3 T1,glm::vec3 T2, glm::vec3 T3){
        this->T1 = T1;
        this->T2 = T2;
        this->T3 = T3;
        //Find normal vector of polygon using cross product of two polgon vertices
        glm::vec3 normal_vector = glm::cross(T1,T2);
        plane p(T1,normal_vector);
        this->p = p;
    }
    std::string getType(){
        return "polygon";
    }
    private:
    //The plane the polygon is laying upon
    shape p;
    //The vertices of the polygon
    glm::vec3 T1;
    glm::vec3 T2;
    glm::vec3 T3;
};
