#ifndef MAIN_H
#define MAIN_H

#include <iostream>
#include <cmath>
#include <fstream>
#include <vector>

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtx/transform.hpp>
#include <glm/gtc/matrix_transform.hpp>

struct color_t {
    int r;
    int g;
    int b;
};

// nonedit.cpp
GLFWwindow *initGLFW(int width, int height);
GLuint     LoadShaders(const char *vertex_file_path, const char *fragment_file_path);
struct VAO *create3DObject(GLenum primitive_mode, int numVertices, const GLfloat *vertex_buffer_data, const GLfloat *color_buffer_data, GLenum fill_mode = GL_FILL);
struct VAO *create3DObject(GLenum primitive_mode, int numVertices, const GLfloat *vertex_buffer_data, const GLfloat red, const GLfloat green, const GLfloat blue, GLenum fill_mode = GL_FILL);
struct VAO *create3DObject(GLenum primitive_mode, int numVertices, const GLfloat *vertex_buffer_data, const color_t color, GLenum fill_mode = GL_FILL);
void       draw3DObject(struct VAO *vao);

// input.cpp
void keyboard(GLFWwindow *window, int key, int scancode, int action, int mods);
void keyboardChar(GLFWwindow *window, unsigned int key);
void mouseButton(GLFWwindow *window, int button, int action, int mods);
void scroll_callback(GLFWwindow *window, double xoffset, double yoffset);
void cursor_position_callback(GLFWwindow* window, double xpos, double ypos);

// other_handlers.cpp
void error_callback(int error, const char *description);
void quit(GLFWwindow *window);
void reshapeWindow(GLFWwindow *window, int width, int height);

// Types
struct VAO {
    GLuint VertexArrayID;
    GLuint VertexBuffer;
    GLuint ColorBuffer;

    GLenum PrimitiveMode;
    GLenum FillMode;
    int    NumVertices;
};
typedef struct VAO VAO;

struct GLMatrices {
    glm::mat4 projection;
    glm::mat4 model;
    glm::mat4 view;
    GLuint    MatrixID;
};

extern GLMatrices Matrices;

// ---- Logic ----

enum direction_t { DIR_UP, DIR_RIGHT, DIR_DOWN, DIR_LEFT };

struct bounding_box_t {
    float x;
    float y;
    float z;
    float width;
    float height;
    float length;
};

bool detect_collision(bounding_box_t a, bounding_box_t b);
void change_camera();
void speed_camera();
void heli_camera(float x, float y);
void aim_canon(float x, float y);
void zoom_camera(int type);
void show_explosion();
void canon_fire();
void check_collision(int move);
void move_enemy();
void if_collision();

extern float screen_zoom, screen_center_x, screen_center_y;
void reset_screen();

// Audio
void audio_init(const char *string);
void audio_play();
void audio_close();

// ---- Colors ----
extern const color_t COLOR_RED;
extern const color_t COLOR_DARK_BLUE;
extern const color_t COLOR_YELLOW;
extern const color_t COLOR_GREEN;
extern const color_t COLOR_BLACK;
extern const color_t COLOR_BLUE;
extern const color_t COLOR_BACKGROUND;
extern const color_t COLOR_BALL1;
extern const color_t COLOR_BALL2;
extern const color_t COLOR_BALL3;
extern const color_t COLOR_BALL4;
extern const color_t COLOR_BALL5;
extern const color_t COLOR_BALL6;
extern const color_t COLOR_BALL7;
extern const color_t COLOR_BALL8;
extern const color_t COLOR_MAGNET;
extern const color_t COLOR_WAVE;
extern const color_t COLOR_BROWN;
extern const color_t COLOR_GREY;
extern const color_t COLOR_DARK_RED;
extern const color_t COLOR_WHITE;
extern const color_t COLOR_BOTTOM;
extern const color_t COLOR_PURPLE;

#endif
