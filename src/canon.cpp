#include "canon.h"
#include "main.h"

Canon::Canon(float x, float y, float z, color_t color) {
    this->position = glm::vec3(x, y, z);
    this->rotation = 0;
    release = 0;
    speedx = 0;
    speedy = 0;
    gravity = 0;
    angle = 0;
    static const GLfloat vertex_buffer_data[] = {
        -2.0f,-2.0f,-2.0f,
        -2.0f,-2.0f, 2.0f,
        -2.0f, 2.0f, 2.0f,
         2.0f, 2.0f,-2.0f,
        -2.0f,-2.0f,-2.0f,
        -2.0f, 2.0f,-2.0f,
         2.0f,-2.0f, 2.0f,
        -2.0f,-2.0f,-2.0f,
         2.0f,-2.0f,-2.0f,
         2.0f, 2.0f,-2.0f,
         2.0f,-2.0f,-2.0f,
        -2.0f,-2.0f,-2.0f,
        -2.0f,-2.0f,-2.0f,
        -2.0f, 2.0f, 2.0f,
        -2.0f, 2.0f,-2.0f,
         2.0f,-2.0f, 2.0f,
        -2.0f,-2.0f, 2.0f,
        -2.0f,-2.0f,-2.0f,
        -2.0f, 2.0f, 2.0f,
        -2.0f,-2.0f, 2.0f,
         2.0f,-2.0f, 2.0f,
         2.0f, 2.0f, 2.0f,
         2.0f,-2.0f,-2.0f,
         2.0f, 2.0f,-2.0f,
         2.0f,-2.0f,-2.0f,
         2.0f, 2.0f, 2.0f,
         2.0f,-2.0f, 2.0f,
         2.0f, 2.0f, 2.0f,
         2.0f, 2.0f,-2.0f,
        -2.0f, 2.0f,-2.0f,
         2.0f, 2.0f, 2.0f,
        -2.0f, 2.0f,-2.0f,
        -2.0f, 2.0f, 2.0f,
         2.0f, 2.0f, 2.0f,
        -2.0f, 2.0f, 2.0f,
         2.0f,-2.0f, 2.0f
    };

    this->object = create3DObject(GL_TRIANGLES, 36, vertex_buffer_data, color, GL_FILL);
}

void Canon::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Canon::set_position(float x, float y, float z) {
    this->position = glm::vec3(x, y, z);
}

void Canon::tick() {
    this->position.x -= speedx*sin(angle);
     this->position.z -= speedx*cos(angle);
    speedy += gravity;
    this->position.y += speedy;
    if(this->position.y<55){
        //this->position.y = 62;
        speedy = 0;
        gravity = 0;
        speedx = 0;
        release = 0;
    }
}

void Canon::fire(float theta, float alpha){
    float pi = 3.145;
    speedx = 10*cos(alpha*pi/180);
    speedy = 10*sin(alpha*pi/180);
    gravity = -0.5;
    angle = theta*pi/180;
}

bounding_box_t Canon::bounding_box() {
    float x = this->position.x, y = this->position.y, z = this->position.z;
    //float w = this->radius, h = this->radius;
    bounding_box_t bbox = { x, y, z, 8, 8, 8 };
    return bbox;
}
