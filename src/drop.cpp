#include "drop.h"
#include "main.h"

Drop::Drop(float x, float y, float z, color_t color) {
    this->position = glm::vec3(x, y, z);
    this->rotation = 0;
    static const GLfloat vertex_buffer_data[] = {
        -12.0, 0.0,-12.0,
         12.0, 0.0,-12.0,
         0.0, 16.0, 0.0,

         12.0, 0.0, 12.0,
         12.0, 0.0,-12.0,
         0.0, 16.0, 0.0,

         12.0, 0.0, 12.0,
        -12.0, 0.0, 12.0,
         0.0, 16.0, 0.0,

        -12.0, 0.0, 12.0,
        -12.0, 0.0,-12.0,
         0.0, 16.0, 0.0

    };

    this->object = create3DObject(GL_TRIANGLES, 12, vertex_buffer_data, color, GL_FILL);
}

void Drop::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 1, 0));
    rotate          = rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Drop::set_position(float x, float y, float z) {
    this->position = glm::vec3(x, y, z);
}

/*void Drop::tick() {
    this->position.x -= speed;
    // this->position.y -= speed;
}*/

bounding_box_t Drop::bounding_box() {
    float x = this->position.x, y = this->position.y, z = this->position.z;
    //float w = this->radius, h = this->radius;
    bounding_box_t bbox = { x, y, z, 6, 4, 6 };
    return bbox;
}
