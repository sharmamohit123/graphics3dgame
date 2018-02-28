#include "water.h"
#include "main.h"
#include "limits.h"

Water::Water(float x, float y, float z, color_t color) {
    this->position = glm::vec3(x, y, z);
    this->rotation = 0;
    //speed = 1;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    /*static const GLfloat vertex_buffer_data[] = {
        -100.0f,-20.0f,-100.0f, // triangle 1 : begin
        -100.0f,-20.0f, 100.0f,
        -100.0f, 20.0f, 100.0f, // triangle 1 : end
         100.0f, 20.0f,-100.0f, // triangle 2 : begin
        -100.0f,-20.0f,-100.0f,
        -100.0f, 20.0f,-100.0f, // triangle 2 : end
         100.0f,-20.0f, 100.0f,
        -100.0f,-20.0f,-100.0f,
         100.0f,-20.0f,-100.0f,
         100.0f, 20.0f,-100.0f,
         100.0f,-20.0f,-100.0f,
        -100.0f,-20.0f,-100.0f,
        -100.0f,-20.0f,-100.0f,
        -100.0f, 20.0f, 100.0f,
        -100.0f, 20.0f,-100.0f,
         100.0f,-20.0f, 100.0f,
        -100.0f,-20.0f, 100.0f,
        -100.0f,-20.0f,-100.0f,
        -100.0f, 20.0f, 100.0f,
        -100.0f,-20.0f, 100.0f,
         100.0f,-20.0f, 100.0f,
         100.0f, 20.0f, 100.0f,
         100.0f,-20.0f,-100.0f,
         100.0f, 20.0f,-100.0f,
         100.0f,-20.0f,-100.0f,
         100.0f, 20.0f, 100.0f,
         100.0f,-20.0f, 100.0f,
         100.0f, 20.0f, 100.0f,
         100.0f, 20.0f,-100.0f,
        -100.0f, 20.0f,-100.0f,
         100.0f, 20.0f, 100.0f,
        -100.0f, 20.0f,-100.0f,
        -100.0f, 20.0f, 100.0f,
         100.0f, 20.0f, 100.0f,
        -100.0f, 20.0f, 100.0f,
         100.0f,-20.0f, 100.0f
    };*/
    int pos=0,i,k,n=100;
    float j;
            //Any polygon can be created by changing of n
            GLfloat g_vertex_buffer_data[180*n];
            float pi = 3.14159, angle = 0, theta=(2*pi)/n;
            for(j=0;j<2;j+=0.1){
                for(i=0;i<n;i++){
                    g_vertex_buffer_data[pos++]= 0.0f;
                    g_vertex_buffer_data[pos++]= 10*j;
                    g_vertex_buffer_data[pos++]= 0.0f;
                    for(k=0;k<2;k++){
                        g_vertex_buffer_data[pos++]= INT_MAX*cos(angle);
                        g_vertex_buffer_data[pos++]= 10*j;
                        g_vertex_buffer_data[pos++]= INT_MAX*sin(angle);
                        angle += theta;
                    }
                    angle-=theta;
                }
            }

    /*this->object = create3DObject(GL_TRIANGLES, 6, vertex_buffer_data, color, GL_FILL);
    this->object1 = create3DObject(GL_TRIANGLES, 6, vertex_buffer_data+18, COLOR_BALL2, GL_FILL);
    this->object2 = create3DObject(GL_TRIANGLES, 6, vertex_buffer_data+36, COLOR_BALL3, GL_FILL);
    this->object3 = create3DObject(GL_TRIANGLES, 6, vertex_buffer_data+54, COLOR_BALL4, GL_FILL);
    this->object4 = create3DObject(GL_TRIANGLES, 6, vertex_buffer_data+72, COLOR_BALL5, GL_FILL);
    this->object5 = create3DObject(GL_TRIANGLES, 6, vertex_buffer_data+90, COLOR_BALL6, GL_FILL);*/
    this->object = create3DObject(GL_TRIANGLES, 60*n, g_vertex_buffer_data, color, GL_FILL);
}

void Water::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 1, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    Matrices.model *= (translate * rotate);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
}

void Water::set_position(float x, float y, float z) {
    this->position = glm::vec3(x, y, z);
}

/*void Water::tick() {
    //this->rotation += speed;
    this->position.x -= 0.1;
    // this->position.y -= speed;
}*/

