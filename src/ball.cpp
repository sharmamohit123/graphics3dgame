#include "ball.h"
#include "main.h"

Ball::Ball(float x, float y, float z, color_t color) {
    this->position = glm::vec3(x, y, z);
    this->rotation = 0;
    speed = 5;
    this->angular_speed = 0.25;
    speedy = 0;
    this->speed_y = 0.02;
    this->level_angle = 0;
    this->acceleration_y = -0.0001;
    gravity = 0;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    static const GLfloat vertex_buffer_data[] = {
        -6.0,-2.0f, 4.0f,
         6.0, 2.0f, 3.0f,
        -6.0, 2.0f, 4.0f,
        -6.0,-2.0f, 4.0f,
         6.0, 2.0f, 3.0f,
         6.0,-2.0f, 3.0f,

         6.0, 2.0f, 3.0f,
         6.0,-2.0f,-3.0f,
         6.0,-2.0f, 3.0f,
         6.0, 2.0f, 3.0f,
         6.0,-2.0f,-3.0f,
         6.0, 2.0f,-3.0f,

         6.0, 2.0f,-3.0f,
        -6.0,-2.0f,-4.0f,
         6.0,-2.0f,-3.0f,
         6.0, 2.0f,-3.0f,
        -6.0,-2.0f,-4.0f,
        -6.0, 2.0f,-4.0f,

        -6.0,-2.0f,-4.0f,
        -6.0, 2.0f, 4.0f,
        -6.0, 2.0f,-4.0f,
        -6.0,-2.0f,-4.0f,
        -6.0, 2.0f, 4.0f,
        -6.0,-2.0f, 4.0f,

        -6.0,-2.0f, 4.0f,
         6.0,-2.0f,-3.0f,
         6.0,-2.0f, 3.0f,
        -6.0,-2.0f, 4.0f,
         6.0,-2.0f,-3.0f,
        -6.0,-2.0f,-4.0f,

         6.0, 2.0f, 3.0f,
         6.0,-2.0f, 3.0f,
        12.0, 3.0f, 0.0f,

         6.0,-2.0f, 3.0f,
         6.0,-2.0f,-3.0f,
        12.0, 3.0f, 0.0f,

         6.0,-2.0f,-3.0f,
         6.0, 2.0f,-3.0f,
        12.0, 3.0f, 0.0f,

         6.0, 2.0f,-3.0f,
         6.0, 2.0f, 3.0f,
        12.0, 3.0f, 0.0f,

        //sail coordinates

        5.0f,-2.0f, 0.5f,
        6.0f,-2.0f, 0.5f,
        5.5f, 20.0f, 0.0f,

        6.0f,-2.0f, 0.5f,
        6.0f,-2.0f,-0.5f,
        5.5f, 20.0f, 0.0f,

        6.0f,-2.0f,-0.5f,
        5.0f,-2.0f,-0.5f,
        5.5f, 20.0f, 0.0f,

        5.0f,-2.0f,-0.5f,
        5.0f,-2.0f, 0.5f,
        5.5f, 20.0f, 0.0f,

        5.5f,17.0f, 0.0f,
        5.5f, 9.0f, 0.0f,
        5.5f, 9.0f,-8.0f,

        5.5f,17.0f, 0.0f,
        5.5f, 11.0f, 0.0f,
        5.5f, 11.0f, 6.0f
    };

    this->object = create3DObject(GL_TRIANGLES, 6, vertex_buffer_data, COLOR_BALL1, GL_FILL);
    this->object1 = create3DObject(GL_TRIANGLES, 6, vertex_buffer_data+18, COLOR_BALL2, GL_FILL);
    this->object2 = create3DObject(GL_TRIANGLES, 6, vertex_buffer_data+36, COLOR_BALL3, GL_FILL);
    this->object3 = create3DObject(GL_TRIANGLES, 6, vertex_buffer_data+54, COLOR_BALL4, GL_FILL);
    this->object4 = create3DObject(GL_TRIANGLES, 6, vertex_buffer_data+72, COLOR_BALL8, GL_FILL);
    this->object5 = create3DObject(GL_TRIANGLES, 12, vertex_buffer_data+90, COLOR_BALL6, GL_FILL);
    this->object6 = create3DObject(GL_TRIANGLES, 12, vertex_buffer_data+126, COLOR_BROWN, GL_FILL);
    this->object7 = create3DObject(GL_TRIANGLES, 3, vertex_buffer_data+162, COLOR_BALL1, GL_FILL);
    this->object8 = create3DObject(GL_TRIANGLES, 3, vertex_buffer_data+171, COLOR_BALL2, GL_FILL);
}

void Ball::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate1   = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 1, 0));
    glm::mat4 rotate2   = glm::rotate((float) (this->level_angle * M_PI / 180.0f), glm::vec3(
                                          this->position.x,
                                          this->position.y,
                                          this->position.z - 2));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    glm::mat4 rotate = rotate1 * rotate2;
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
    draw3DObject(this->object1);
    draw3DObject(this->object2);
    draw3DObject(this->object3);
    draw3DObject(this->object4);
    draw3DObject(this->object5);
    draw3DObject(this->object6);
    draw3DObject(this->object7);
    draw3DObject(this->object8);
}

void Ball::set_position(float x, float y, float z) {
    this->position = glm::vec3(x, y, z);
}

void Ball::tick() {
    //this->rotation += speed;
    //this->position.z -= 0.5;
    // this->position.y -= speed;
    speedy += gravity;
    this->position.y += speedy;
    if(this->position.y<60){
        this->position.y = 60;
        speedy = 0;
        gravity = 0;
    }
}

void Ball::right(){
    //this->position.x += speed;
    this->rotation -= speed*0.6;
}

void Ball::left(){
    this->rotation += speed*0.6;
}

void Ball::forward(float angle){
    this->position.z -= speed*cos(angle);
    this->position.x -= speed*sin(angle);
}

void Ball::backward(float angle){
    this->position.z += speed*cos(angle);
    this->position.x += speed*sin(angle);
}

void Ball::jump(){
    speedy = 1.5;
    gravity = -0.05;
}

void Ball::shm(){
    if(this->acceleration_y > 0){
        if(this->position.y + speed_y > 60.7){
            acceleration_y = -acceleration_y;
            speed_y = 0.02;
        }
        else{
            this->position.y += speed_y;
            speed_y -= acceleration_y;
        }
    }
    if(this->acceleration_y < 0){
        if(this->position.y - speed_y < 60.1){
            acceleration_y = -acceleration_y;
            speed_y = 0.02;
        }
        else{
            this->position.y -= speed_y;
            speed_y += acceleration_y;
        }
    }
    if(level_angle < -5.0f || level_angle > 5.0f){
        angular_speed = -angular_speed;
    }
    this->level_angle += angular_speed;
}



