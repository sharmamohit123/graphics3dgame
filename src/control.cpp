#include "control.h"
#include "main.h"

Control::Control(float x, float y, float z, color_t color) {
    this->position = glm::vec3(x, y, z);
    this->xrotation = 0;
    this->yrotation = 0;
    this->zrotation = 0;
    static const GLfloat vertex_buffer_data[] = {
        1.0f, 0.0f,0.0f,  
        -1.0f,0.0f, 0.0f, 
        1.0f, 0.0f,10.0f,
        1.0f, 0.0f,10.0f,
       -1.0f, 0.0f,10.0f,
        -1.0f,0.0f, 0.0f, 
    };

    this->object = create3DObject(GL_TRIANGLES, 6, vertex_buffer_data, color, GL_FILL);
}

void Control::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->yrotation * M_PI / 180.0f), glm::vec3(0, 1, 0));
    rotate    *= glm::rotate((float) (this->xrotation * M_PI / 180.0f), glm::vec3(1, 0, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Control::set_position(float x, float y, float z) {
    this->position = glm::vec3(x, y, z);
}

/*void Control::tick() {
    this->position.x -= speed;
    // this->position.y -= speed;
}*/

bounding_box_t Control::bounding_box() {
    float x = this->position.x, y = this->position.y, z = this->position.z;
    //float w = this->radius, h = this->radius;
    bounding_box_t bbox = { x, y, z, 4, 4, 4 };
    return bbox;
}
