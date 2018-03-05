#include "rocks.h"
#include "main.h"

Rocks::Rocks(float x, float y, float z, color_t color) {
    this->position = glm::vec3(x, y, z);
    this->rotation = 0;
    static const GLfloat vertex_buffer_data[] = {
        -5.0f,-5.0f,-5.0f,
        -5.0f,-5.0f, 5.0f,
        -5.0f, 5.0f, 5.0f,
         5.0f, 5.0f,-5.0f,
        -5.0f,-5.0f,-5.0f,
        -5.0f, 5.0f,-5.0f,
         5.0f,-5.0f, 5.0f,
        -5.0f,-5.0f,-5.0f,
         5.0f,-5.0f,-5.0f,
         5.0f, 5.0f,-5.0f,
         5.0f,-5.0f,-5.0f,
        -5.0f,-5.0f,-5.0f,
        -5.0f,-5.0f,-5.0f,
        -5.0f, 5.0f, 5.0f,
        -5.0f, 5.0f,-5.0f,
         5.0f,-5.0f, 5.0f,
        -5.0f,-5.0f, 5.0f,
        -5.0f,-5.0f,-5.0f,
        -5.0f, 5.0f, 5.0f,
        -5.0f,-5.0f, 5.0f,
         5.0f,-5.0f, 5.0f,
         5.0f, 5.0f, 5.0f,
         5.0f,-5.0f,-5.0f,
         5.0f, 5.0f,-5.0f,
         5.0f,-5.0f,-5.0f,
         5.0f, 5.0f, 5.0f,
         5.0f,-5.0f, 5.0f,
         5.0f, 5.0f, 5.0f,
         5.0f, 5.0f,-5.0f,
        -5.0f, 5.0f,-5.0f,
         5.0f, 5.0f, 5.0f,
        -5.0f, 5.0f,-5.0f,
        -5.0f, 5.0f, 5.0f,
         5.0f, 5.0f, 5.0f,
        -5.0f, 5.0f, 5.0f,
         5.0f,-5.0f, 5.0f
    };

    this->object = create3DObject(GL_TRIANGLES, 36, vertex_buffer_data, color, GL_FILL);
}

void Rocks::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Rocks::set_position(float x, float y, float z) {
    this->position = glm::vec3(x, y, z);
}

/*void Rocks::tick() {
    this->position.x -= speed;
    // this->position.y -= speed;
}*/

bounding_box_t Rocks::bounding_box() {
    float x = this->position.x, y = this->position.y, z = this->position.z;
    bounding_box_t bbox = { x, y, z, 13, 5, 13 };
    return bbox;
}
