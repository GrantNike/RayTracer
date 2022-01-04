/*
Grant Nike
6349302
January 10th
COSC 3P98 Final Project
*/

class light{
    public:
    light(glm::vec3 position,glm::vec3 intensity){
        this->position = position;
        this->intensity = intensity;
    }
    glm::vec3 get_position(){
        return position;
    }
    glm::vec3 get_intensity(){
        return intensity;
    }
    private:
    glm::vec3 position;
    glm::vec3 intensity;
};