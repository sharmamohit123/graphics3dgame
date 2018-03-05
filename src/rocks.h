#include "main.h"

#ifndef Rocks_H
#define Rocks_H


class Rocks {
public:
    Rocks() {}
    Rocks(float x, float y, float z, color_t color);
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

#endif // Rocks_H
