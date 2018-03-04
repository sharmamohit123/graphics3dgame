#include "main.h"
#include "timer.h"
#include "ball.h"
#include "water.h"
#include "barrel.h"
#include "waves.h"
#include "canon.h"
#include "gifts.h"
using namespace std;

GLMatrices Matrices;
GLuint     programID;
GLFWwindow *window;

/**************************
* Customizable functions *
**************************/

Ball boat;
Water sea;
Barrel barrel[100];
Waves wave[100];
Canon canon;
Gifts gift[30];

float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;
float camera_rotation_angle = 0, camera_x=0, camera_y=90, camera_z=100, target_x=0, target_y=90, target_z=0;
int view = 1, i, ran_x, ran_z, n=100, m=50, g=20;
const float pi = 3.14159;

Timer t60(1.0 / 60);

/* Render the scene with openGL */
/* Edit this function according to your assignment */
void draw() {
    // clear the color and depth in the frame buffer
    glClear (GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    // use the loaded shader program
    // Don't change unless you know what you are doing
    glUseProgram (programID);

    // Eye - Location of camera. Don't change unless you are sure!!
    //glm::vec3 eye (5*cos(camera_rotation_angle*M_PI/180.0f), 0, 5*sin(camera_rotation_angle*M_PI/180.0f));
    glm::vec3 eye(camera_x, camera_y, camera_z);
    // Target - Where is the camera looking at.  Don't change unless you are sure!!
    glm::vec3 target (target_x, target_y, target_z);
    // Up - Up vector defines tilt of camera.  Don't change unless you are sure!!
    glm::vec3 up (0, 1, 0);

    // Compute Camera matrix (view)
    Matrices.view = glm::lookAt( eye, target, up ); // Rotating Camera for 3D
    // Don't change unless you are sure!!
    // Matrices.view = glm::lookAt(glm::vec3(0, 0, 3), glm::vec3(0, 0, 0), glm::vec3(0, 1, 0)); // Fixed camera for 2D (ortho) in XY plane

    // Compute ViewProject matrix as view/camera might not be changed for this frame (basic scenario)
    // Don't change unless you are sure!!
    glm::mat4 VP = Matrices.projection * Matrices.view;

    // Send our transformation to the currently bound shader, in the "MVP" uniform
    // For each model you render, since the MVP will be different (at least the M part)
    // Don't change unless you are sure!!
    glm::mat4 MVP;  // MVP = Projection * View * Model

    // Scene render
    sea.draw(VP);
    boat.draw(VP);
    for(i=0;i<n;i++)
        wave[i].draw(VP);
    for(i=0;i<m;i++)
        barrel[i].draw(VP);
    for(i=0;i<g;i++)
        gift[i].draw(VP);
    if(canon.release)
        canon.draw(VP);
}

void tick_input(GLFWwindow *window) {
    int left  = glfwGetKey(window, GLFW_KEY_LEFT);
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    int up = glfwGetKey(window, GLFW_KEY_UP);
    int down = glfwGetKey(window, GLFW_KEY_DOWN);
    int jump = glfwGetKey(window, GLFW_KEY_SPACE);
    int fire = glfwGetKey(window, GLFW_KEY_F);
    if (left) {
        boat.left();
        //speed_camera(2);
    }
    if(right){
        boat.right();
        //speed_camera(3);
    }
    if(up){
        boat.forward((boat.rotation-90)*(pi/180));
        //speed_camera(1);
    }
    if(down){
        boat.backward((boat.rotation-90)*(pi/180));
    }
    if(jump){
        if(boat.position.y<=62)
            boat.jump();
    }
    if(fire && !canon.release){
        //audio_init("assets/song1.mp3");
        //audio_play();
        canon.release = 1;
        canon.fire((boat.rotation-90)*(pi/180));
    }
}

void tick_elements() {
    boat.tick();
    for(i=0;i<g;i++)
        gift[i].tick();
    boat.shm();
    printf("%.2f\n",boat.position.y);
    if(!canon.release){
       canon.set_position(boat.position.x, boat.position.y, boat.position.z);
    }
    else{
        canon.tick();
    }
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models

    boat = Ball(0, 60, 40, COLOR_RED);
    sea  =  Water(0, 40, 0, COLOR_BLUE);
    boat.rotation = 90;
    printf("max=%d\n", INT_MAX);
    for(i=0;i<n;i++){
        ran_x = rand() % 2000-1000;
        ran_z = rand() % 3000-4000;
        wave[i] = Waves(ran_x, 60 , ran_z, COLOR_WAVE);
    }
    for(i=0;i<m;i++){
        ran_x = rand() % 10000-5000;
        if(ran_x>-500 && ran_x<0)
            ran_x-=500;
        if(ran_x<500 && ran_x>0)
            ran_x+=500;
        ran_z = rand() % 10000-5000;
        if(ran_z>-560 && ran_z<60)
            ran_z-=500;
        if(ran_x<560 && ran_x>60)
            ran_x+=500;
        barrel[i] = Barrel(ran_x, 60 , ran_z, 6, COLOR_BROWN);
    }
    canon = Canon(0, 62, 40, COLOR_DARK_BLUE);

    for(i=0;i<g;i++){
        int g_barrel = rand()%m;
        while(barrel[g_barrel].ngift!=-1)
            g_barrel = rand()%m;
        gift[i] = Gifts(barrel[g_barrel].position.x+20, barrel[g_barrel].position.y+40, barrel[g_barrel].position.z, COLOR_BALL5);
    }
    // Create and compile our GLSL program from the shaders
    programID = LoadShaders("Sample_GL.vert", "Sample_GL.frag");
    // Get a handle for our "MVP" uniform
    Matrices.MatrixID = glGetUniformLocation(programID, "MVP");


    reshapeWindow (window, width, height);

    // Background color of the scene
    glClearColor (COLOR_BACKGROUND.r / 256.0, COLOR_BACKGROUND.g / 256.0, COLOR_BACKGROUND.b / 256.0, 0.0f); // R, G, B, A
    glClearDepth (1.0f);

    glEnable (GL_DEPTH_TEST);
    glDepthFunc (GL_LEQUAL);

    cout << "VENDOR: " << glGetString(GL_VENDOR) << endl;
    cout << "RENDERER: " << glGetString(GL_RENDERER) << endl;
    cout << "VERSION: " << glGetString(GL_VERSION) << endl;
    cout << "GLSL: " << glGetString(GL_SHADING_LANGUAGE_VERSION) << endl;
}


int main(int argc, char **argv) {
    srand(time(0));
    int width  = 600;
    int height = 600;

    window = initGLFW(width, height);

    initGL (window, width, height);

    //audio_init("assets/song.mp3");
    /* Draw in loop */
    while (!glfwWindowShouldClose(window)) {
         //audio_play();
        // Process timers

        if (t60.processTick()) {
            // 60 fps
            // OpenGL Draw commands
            draw();
            // Swap Frame Buffer in double buffering
            glfwSwapBuffers(window);

            tick_elements();
            tick_input(window);
            speed_camera();
        }

        // Poll for Keyboard and mouse events
        glfwPollEvents();
    }

    quit(window);
}

bool detect_collision(bounding_box_t a, bounding_box_t b) {
    return (abs(a.x - b.x) * 2 < (a.length + b.length)) &&
            (abs(a.z - b.z) * 2 < (a.width + b.width)) &&
           (abs(a.y - b.y) * 2 < (a.height + b.height));
}

void reset_screen() {
    /*float top    = screen_center_y + 50 / screen_zoom;
    float bottom = screen_center_y - 50 / screen_zoom;
    float left   = screen_center_x - 50 / screen_zoom;
    float right  = screen_center_x + 50 / screen_zoom;
    Matrices.projection = glm::ortho(left, right, bottom, top, 0.1f, 500.0f);*/
    Matrices.projection = glm::perspective(45.0f, 1.0f, 50.0f, 10000.0f);
}

void change_camera(){
    /*float theta = (boat.rotation-90)*(pi/180);
    if(type==0){
        view = 0;
        camera_x = 0;
        camera_y = 75;
        camera_z = 20;
        target_x = 0;
        target_y = 75;
        target_z = 0;
    }
    else if(type==1){
        view = 1;
        camera_x = boat.position.x+80*sin(theta);
        camera_y = 90;
        camera_z = boat.position.z+80*cos(theta);
        target_x = 0;
        target_y = 90;
        target_z = 0;
    }*/
    view = (view+1)%5;
}

void speed_camera(){
    float theta = (boat.rotation-90)*(pi/180);
    if(view==1){
        camera_x = boat.position.x+80*sin(theta);
        camera_y = 90;
        camera_z = boat.position.z+80*cos(theta);

        target_x = boat.position.x+sin(theta+pi);
        target_y = 90;
        target_z = boat.position.z+cos(theta+pi);
    }
    else if(view==0){
        camera_x = boat.position.x+30*sin(theta);
        camera_y = boat.position.y+10;
        camera_z = boat.position.z+30*cos(theta);

        target_x = boat.position.x+40*sin(theta+pi);
        target_y = boat.position.y+10;
        target_z = boat.position.z+40*cos(theta+pi);
        printf("cx=%f cy=%f cz=%f\n", camera_x, camera_y, camera_z);
        printf("tx=%f ty=%f tz=%f\n", target_x, target_y, target_z);
    }
    else if(view==3){
        camera_x = boat.position.x;
        camera_y = 200;
        camera_z = boat.position.z;

        target_x = boat.position.x+1;
        target_y = boat.position.y;
        target_z = boat.position.z;
        printf("cx=%f cy=%f cz=%f\n", camera_x, camera_y, camera_z);
        printf("tx=%f ty=%f tz=%f\n", target_x, target_y, target_z);
    }
    else if(view==4){
        camera_x = boat.position.x+100;
        camera_y = 100;
        camera_z = boat.position.z;

        target_x = boat.position.x;
        target_y = boat.position.y;
        target_z = boat.position.z;
    }
    else if(view==2){
        camera_y = 100;
    }

}

void heli_camera(float x, float y){
    if(view==2){
        target_x = boat.position.x+(x-300);
        if(y<=300){
            target_y = boat.position.y+(300-y)/2;
        }
    }
}

void zoom_camera(int type){
    if(view==2){
        float l = target_x-camera_x;
        float m = target_y-camera_y;
        float n = target_z-camera_z;
        if(type==1){
            if(camera_z-10>target_z)
                camera_z-=10;
        }
        else if(type==-1){
            camera_z+=10;
        }
        camera_x = l*(camera_z-target_z)/n+target_x;
        camera_y = m*(camera_z-target_z)/n+target_y;
    }
}
