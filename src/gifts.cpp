#include "gifts.h"
#include "main.h"

Gifts::Gifts(float x, float y, float z, color_t color) {
    this->position = glm::vec3(x, y, z);
    this->rotation = 0;
    nbarrel = -1;
    int pos=0,i,k,n=6;
            //Any polygon can be created by changing of n
    GLfloat g_vertex_buffer_data[9*n];
    float pi = 3.14159, angle = 0, theta=(2*pi)/n;
    for(i=0;i<n;i++){
        g_vertex_buffer_data[pos++]= 0.0f;
        g_vertex_buffer_data[pos++]= 0.0f;
        g_vertex_buffer_data[pos++]= 0.0f;
        for(k=0;k<2;k++){
            g_vertex_buffer_data[pos++]= 8*cos(angle);
            g_vertex_buffer_data[pos++]= 8*sin(angle);
            g_vertex_buffer_data[pos++]= 0.0f;
            angle += theta;
        }
        angle-=theta;
    }
    this->object = create3DObject(GL_TRIANGLES, 3*n, g_vertex_buffer_data, color, GL_FILL);
}

void Gifts::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 1, 0));
    rotate          = rotate * glm::translate(glm::vec3(0, 0, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Gifts::set_position(float x, float y, float z) {
    this->position = glm::vec3(x, y, z);
}

void Gifts::tick() {
    this->rotation +=2.0;
}

bounding_box_t Gifts::bounding_box() {
    float x = this->position.x, y = this->position.y, z = this->position.z;
    //float w = this->radius, h = this->radius;
    bounding_box_t bbox = { x, y, z, 16, 16, 16 };
    return bbox;
}
