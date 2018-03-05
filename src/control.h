#include "main.h"

#ifndef Control_H
#define Control_H


class Control {
public:
    Control() {}
    Control(float x, float y, float z, color_t color);
    glm::vec3 position;
    float xrotation;
    float yrotation;
    float zrotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y, float z);
    bounding_box_t bounding_box();
private:
    VAO *object;
};

#endif // Control_H
