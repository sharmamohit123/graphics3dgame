#include "main.h"

#ifndef Waves_H
#define Waves_H


class Waves {
public:
    Waves() {}
    Waves(float x, float y, float z, color_t color);
    glm::vec3 position;
    //float radius;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y, float z);
    //void tick();
    //double speed;
    bounding_box_t bounding_box();
private:
    VAO *object;
};

#endif // Waves_H
