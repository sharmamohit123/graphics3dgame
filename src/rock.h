#include "main.h"

#ifndef Rock_H
#define Rock_H


class Rock {
public:
    Rock() {}
    Rock(float x, float y, float z, float size, color_t color);
    glm::vec3 position;
    float size;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y, float z);
    //void tick();
    //double speed;
private:
    VAO *object;
};

#endif // Rock_H
