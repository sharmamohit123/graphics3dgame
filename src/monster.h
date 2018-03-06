#include "main.h"

#ifndef Monster_H
#define Monster_H


class Monster {
public:
    Monster() {}
    Monster(float x, float y, float z, float size, color_t color);
    glm::vec3 position;
    glm::vec3 start;
    int health;
    int kill_score;
    float size;
    float rotation;
    void draw(glm::mat4 VP);
    void set_position(float x, float y, float z);
    void tick();
    bounding_box_t bounding_box();
private:
    VAO *object;
    VAO *object1;
};

#endif // Monster_H
