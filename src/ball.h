#include "main.h"

#ifndef BALL_H
#define BALL_H


class Ball {
public:
    Ball() {}
    Ball(float x, float y, float z, color_t color);
    glm::vec3 position;
    float rotation;
    void right();
    float level_angle;
    float angular_speed;
    float acceleration_y;
    float speed_y;
    float health;
    int points;
    void shm();
    void left();
    void forward(float angle);
    void backward(float angle);
    void jump();
    void draw(glm::mat4 VP);
    void set_position(float x, float y, float z);
    void tick();
    float speed;
    float speedy;
    float gravity;
    bounding_box_t bounding_box();
private:
    VAO *object;
    VAO *object1;
    VAO *object2;
    VAO *object3;
    VAO *object4;
    VAO *object5;
    VAO *object6;
    VAO *object7;
    VAO *object8;
};

#endif // BALL_H
