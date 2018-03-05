#include "monster.h"
#include "main.h"
#include "math.h"

Monster::Monster(float x, float y, float z, float size, color_t color) {
    this->position = glm::vec3(x, y, z);
    radius = 500;
    this->start = glm::vec3(x, y, z-radius);
    this->size = size;
    health = 100;
    r_angle = 0;
    //rotation = 20;
    // Our vertices. Three consecutive floats give a 3D vertex; Three consecutive vertices give a triangle.
    // A cube has 6 faces with 2 triangles each, so this makes 6*2=12 triangles, and 12*3 vertices
    int pos=0,i,k,n=100;
    float j=0, tradius;
    int mv = 20*this->size/2;
    GLfloat vertex_buffer_data[mv*9*n+9*n+108];
    GLfloat g_vertex_buffer_data[mv*9*n+9*n];
    float pi = 3.14159, angle = 0, theta=(2*pi)/n;
    for(j=-4*this->size;j<=4*this->size;j+=0.8){
        for(i=0;i<n;i++){
            g_vertex_buffer_data[pos++]= j;
            g_vertex_buffer_data[pos++]= 0.0f;
            g_vertex_buffer_data[pos++]= 0.0f;
            for(k=0;k<2;k++){
                tradius = sqrt(16*this->size*this->size-j*j);
                g_vertex_buffer_data[pos++]= j;
                g_vertex_buffer_data[pos++]= tradius*cos(angle);
                g_vertex_buffer_data[pos++]= tradius*sin(angle);
                angle += theta;
            }
            angle-=theta;
            //j+=0.001;
     }
    }

    pi = 3.14159, angle = 0, theta=(2*pi)/n;
    for(i=0;i<n;i++){
        g_vertex_buffer_data[pos++]= 0.0f;
        g_vertex_buffer_data[pos++]= 0.0f;
        g_vertex_buffer_data[pos++]= 0.0f;
        for(k=0;k<2;k++){
            g_vertex_buffer_data[pos++]= 4*this->size*cos(angle);
            g_vertex_buffer_data[pos++]= 4*this->size*sin(angle);
            g_vertex_buffer_data[pos++]= 0.0f;
            angle += theta;
        }
        angle-=theta;
    }
    static const GLfloat l_vertex_buffer_data[] = {
        -8.0f,-8.0f-16.0f,-8.0f,
        -8.0f,-8.0f-16.0f, 8.0f,
        -8.0f, 8.0f-16.0f, 8.0f,
         8.0f, 8.0f-16.0f,-8.0f,
        -8.0f,-8.0f-16.0f,-8.0f,
        -8.0f, 8.0f-16.0f,-8.0f,
         8.0f,-8.0f-16.0f, 8.0f,
        -8.0f,-8.0f-16.0f,-8.0f,
         8.0f,-8.0f-16.0f,-8.0f,
         8.0f, 8.0f-16.0f,-8.0f,
         8.0f,-8.0f-16.0f,-8.0f,
        -8.0f,-8.0f-16.0f,-8.0f,
        -8.0f,-8.0f-16.0f,-8.0f,
        -8.0f, 8.0f-16.0f, 8.0f,
        -8.0f, 8.0f-16.0f,-8.0f,
         8.0f,-8.0f-16.0f, 8.0f,
        -8.0f,-8.0f-16.0f, 8.0f,
        -8.0f,-8.0f-16.0f,-8.0f,
        -8.0f, 8.0f-16.0f, 8.0f,
        -8.0f,-8.0f-16.0f, 8.0f,
         8.0f,-8.0f-16.0f, 8.0f,
         8.0f, 8.0f-16.0f, 8.0f,
         8.0f,-8.0f-16.0f,-8.0f,
         8.0f, 8.0f-16.0f,-8.0f,
         8.0f,-8.0f-16.0f,-8.0f,
         8.0f, 8.0f-16.0f, 8.0f,
         8.0f,-8.0f-16.0f, 8.0f,
         8.0f, 8.0f-16.0f, 8.0f,
         8.0f, 8.0f-16.0f,-8.0f,
        -8.0f, 8.0f-16.0f,-8.0f,
         8.0f, 8.0f-16.0f, 8.0f,
        -8.0f, 8.0f-16.0f,-8.0f,
        -8.0f, 8.0f-16.0f, 8.0f,
         8.0f, 8.0f-16.0f, 8.0f,
        -8.0f, 8.0f-16.0f, 8.0f,
         8.0f,-8.0f-16.0f, 8.0f,

         4.0f,15.0f, 0.0f,
         5.0f, 7.0f, 0.0f,
         3.0f, 7.0f, 0.0f,

        -4.0f,15.0f, 0.0f,
        -5.0f, 7.0f, 0.0f,
        -3.0f, 7.0f, 0.0f,
        };

    for(i=0;i<pos+126;i++){
        if(i<pos)
            vertex_buffer_data[i] = g_vertex_buffer_data[i];
        else
            vertex_buffer_data[i] = l_vertex_buffer_data[i-pos];
    }

    this->object = create3DObject(GL_TRIANGLES, mv*3*n+3*n, vertex_buffer_data, color, GL_FILL);
    this->object1 = create3DObject(GL_TRIANGLES, 42, vertex_buffer_data+pos,COLOR_BOTTOM , GL_FILL);
}

void Monster::draw(glm::mat4 VP) {
    Matrices.model = glm::mat4(1.0f);
    glm::mat4 translate = glm::translate (this->position);    // glTranslatef
    glm::mat4 rotate    = glm::rotate((float) (this->rotation * M_PI / 180.0f), glm::vec3(0, 1, 0));
    // No need as coords centered at 0, 0, 0 of cube arouund which we waant to rotate
    // rotate          = rotate * glm::translate(glm::vec3(0, -0.6, 0));
    glm::mat4 scale    = glm::scale(glm::vec3(this->size, this->size, this->size));
    Matrices.model *= (translate * rotate * scale);
    glm::mat4 MVP = VP * Matrices.model;
    glUniformMatrix4fv(Matrices.MatrixID, 1, GL_FALSE, &MVP[0][0]);
    draw3DObject(this->object);
    draw3DObject(this->object1);
}

void Monster::set_position(float x, float y, float z) {
    this->position = glm::vec3(x, y, z);
}

void Monster::tick() {
    float pi = 3.14159;
    //printf("startx=%f starty=%f\n", this->start.x, this->start.z);
    this->position.x = this->start.x + radius*sin(r_angle*pi/180);
    this->position.z = this->start.z + radius*cos(r_angle*pi/180);
    //printf("posx=%f posy=%f\n", this->position.x, this->position.z);
    r_angle+=0.5;
    // this->position.y -= speed;
}

bounding_box_t Monster::bounding_box() {
    float x = this->position.x, y = this->position.y, z = this->position.z;
    //float w = this->radius, h = this->radius;
    bounding_box_t bbox = { x, y, z, 16, 24, 16};
    return bbox;
}

