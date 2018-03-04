#include "main.h"

#ifndef Barrel_H
#define Barrel_H


class Barrel {
public:
    Barrel() {}
    Barrel(float x, float y, float z, float size, color_t color);
    glm::vec3 position;
    int ngift;
    float size;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y, float z);
    //void tick();
    //double speed;
private:
    VAO *object;
};

#endif // Barrel_H
