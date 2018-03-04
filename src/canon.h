#include "main.h"

#ifndef Canon_H
#define Canon_H


class Canon {
public:
    Canon() {}
    Canon(float x, float y, float z, color_t color);
    glm::vec3 position;
    //float radius;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y, float z);
    void tick();
    void fire(float angle);
    float angle;
    float speedx;
    float speedy;
    float gravity;
    int release;
    bounding_box_t bounding_box();
private:
    VAO *object;
};

#endif // Canon_H
