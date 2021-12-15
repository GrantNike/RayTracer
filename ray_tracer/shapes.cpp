/*
Grant Nike
6349302
January 10th
COSC 3P98 Final Project
*/

#include <string>
#include <glm/glm.hpp>


class shape{
    public:
    //Gives the type of the shape
    std::string getType();
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
    private:
    glm::vec3 position;
    float radius;
    float radius_squared;
};

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
