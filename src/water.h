#include "main.h"

#ifndef Water_H
#define Water_H


class Water {
public:
    Water() {}
    Water(float x, float y, float z, color_t color);
    glm::vec3 position;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y, float z);
    void tick();
    double speed;
private:
    VAO *object;
    /*VAO *object1;
    VAO *object2;
    VAO *object3;
    VAO *object4;
    VAO *object5;*/
};

#endif // Water_H
