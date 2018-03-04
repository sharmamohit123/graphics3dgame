#include "waves.h"
#include "main.h"

Waves::Waves(float x, float y, float z, color_t color) {
    this->position = glm::vec3(x, y, z);
    this->rotation = 0;
    static const GLfloat vertex_buffer_data[] = {
        0.0, 0.0, 0.0,
        0.0, 0.0,-3.0,
       -15.0, 0.0, 6.0,

        0.0, 0.0, 0.0,
        0.0, 0.0,-3.0,
        15.0, 0.0, 6.0,
    };

    this->object = create3DObject(GL_TRIANGLES, 6, vertex_buffer_data, color, GL_FILL);
}

void Waves::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 0, 1));
    rotate          = rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Waves::set_position(float x, float y, float z) {
    this->position = glm::vec3(x, y, z);
}

/*void Waves::tick() {
    this->position.x -= speed;
    // this->position.y -= speed;
}*/

bounding_box_t Waves::bounding_box() {
    float x = this->position.x, y = this->position.y, z = this->position.z;
    //float w = this->radius, h = this->radius;
    bounding_box_t bbox = { x, y, z, 4, 4, 4 };
    return bbox;
}
