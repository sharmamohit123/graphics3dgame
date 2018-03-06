#include "main.h"

#ifndef Blast_H
#define Blast_H


class Blast {
public:
    Blast() {}
    Blast(float x, float y, float z, float size, color_t color);
    glm::vec3 position;
    float size;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y, float z);
    bounding_box_t bounding_box();
private:
    VAO *object;
};

#endif // Blast_H
