#include "main.h"
#include "timer.h"
#include "ball.h"
#include "water.h"
#include "barrel.h"
#include "waves.h"
#include "canon.h"
#include "gifts.h"
#include "rocks.h"
#include "monster.h"
#include "control.h"
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
Waves wave;
Canon canon;
Gifts gift[30];
Rocks rock[100];
Monster enemy[20];
Monster boss;
Monster explosion;
Control control;

float screen_zoom = 1, screen_center_x = 0, screen_center_y = 0;
float camera_rotation_angle = 0, camera_x=0, camera_y=90, camera_z=100, target_x=0, target_y=90, target_z=0;
int view = 1, i, ran_x, ran_z, n=100, m=50, g=20, r=100, e = 10, j, wind_time=0, fire = 0;
const float pi = 3.14159;
bool collide = 0, explode = 0, boat_move=0;

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
    control.draw(VP);
    for(i=0;i<e;i++)
        enemy[i].draw(VP);
    if(boat_move)
        wave.draw(VP);
    for(i=0;i<m;i++)
        barrel[i].draw(VP);
    for(i=0;i<r;i++)
        rock[i].draw(VP);
    for(i=0;i<g;i++)
        gift[i].draw(VP);
    if(canon.release)
        canon.draw(VP);
    if(boat.points>1000)
        boss.draw(VP);
    explosion.draw(VP);
}

void tick_input(GLFWwindow *window) {
    boat_move = 0;
    int left  = glfwGetKey(window, GLFW_KEY_LEFT);
    int right = glfwGetKey(window, GLFW_KEY_RIGHT);
    int up = glfwGetKey(window, GLFW_KEY_UP);
    int down = glfwGetKey(window, GLFW_KEY_DOWN);
    int jump = glfwGetKey(window, GLFW_KEY_SPACE);
    int fire = glfwGetKey(window, GLFW_KEY_F);
    if (left) {
        check_collision(3);
        control.yrotation += boat.speed*0.4;
    }
    if(right){
        check_collision(4);
        control.yrotation -= boat.speed*0.4;
    }
    if(up){
        //boat.forward((boat.rotation-90)*(pi/180));
        check_collision(1);
        if(boat.position.y<=61)
            boat_move = 1;
    }
    if(down){
        //boat.backward((boat.rotation-90)*(pi/180));
        check_collision(2);
        if(boat.position.y<=61)
            boat_move = 1;
    }
    if(jump){
        if(boat.position.y<=61)
            boat.jump();
    }
    if(fire){
        canon_fire();
    }
}

void tick_elements() {
    boat.tick();
    for(i=0;i<g;i++)
        gift[i].tick();
    boat.shm();
    for(i=0;i<g;i++)
        enemy[i].tick();
    boss.tick();
    //printf("%.2f\n",boat.position.y);
    wave.set_position(boat.position.x, boat.position.y, boat.position.z);
    wave.rotation = boat.rotation-90;
    control.set_position(boat.position.x, boat.position.y, boat.position.z);
    if(!canon.release){
       canon.set_position(boat.position.x, boat.position.y, boat.position.z);
    }
    else{
        canon.tick();
    }

    for(i=0;i<g;i++){
        if (detect_collision(boat.bounding_box(), gift[i].bounding_box())) {
            //printf("collide\n");
            boat.points+=10;
            barrel[gift[i].nbarrel].ngift = -1;
            for(j=0;j<m;j++){
                if(barrel[j].ngift==-1){
                    gift[i].set_position(barrel[j].position.x, barrel[j].position.y, barrel[j].position.z);
                    barrel[j].ngift = i;
                    gift[i].nbarrel = j;
                    break;
                }
            }
        }
    }
    if(wind_time>500 && wind_time<800){
        boat.rotation+=0.2;
        control.yrotation+=0.2;
    }
    if(collide){
        boat.health -=0.1;
        printf("health=%f\n",boat.health);
    }
    if(boat.health<=0)
        quit(window);
}

/* Initialize the OpenGL rendering properties */
/* Add all the models to be created here */
void initGL(GLFWwindow *window, int width, int height) {
    /* Objects should be created before any other gl function and shaders */
    // Create the models

    boat = Ball(0, 60, 40, COLOR_RED);
    sea  =  Water(0, 40, 0, COLOR_BLUE);
    boat.rotation = 90;
    control = Control(0, 60, 40, COLOR_BALL5);
    explosion = Monster(0, 60, 0, 2, COLOR_DARK_RED);
    explosion.size = 0;
    //for(i=0;i<n;i++){
        //ran_x = rand() % 2000-1000;
        //ran_z = rand() % 3000-4000;
        wave = Waves(0, 60 ,45, COLOR_WAVE);
    //}
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
    for(i=0;i<r;i++){
        ran_x = rand() % 10000-5000;
        if(ran_x>-100 && ran_x<0)
            ran_x-=100;
        if(ran_x<100 && ran_x>0)
            ran_x+=100;
        ran_z = rand() % 10000-5000;
        if(ran_z>-160 && ran_z<60)
            ran_z-=100;
        if(ran_x<160 && ran_x>60)
            ran_x+=100;
        rock[i] = Rocks(ran_x, 60 , ran_z, COLOR_GREY);
    }
    canon = Canon(0, 62, 40, COLOR_DARK_BLUE);

    for(i=0;i<g;i++){
        int g_barrel = rand()%m;
        while(barrel[g_barrel].ngift!=-1)
            g_barrel = rand()%m;
        gift[i] = Gifts(barrel[g_barrel].position.x+20, barrel[g_barrel].position.y+30, barrel[g_barrel].position.z, COLOR_BALL5);
        gift[i].nbarrel = g_barrel;
    }
    for(i=0;i<e;i++){
        ran_x = rand() % 5000-2500;
        if(ran_x>-100 && ran_x<0)
            ran_x-=100;
        if(ran_x<100 && ran_x>0)
            ran_x+=100;
        ran_z = rand() % 5000-2500;
        if(ran_z>-160 && ran_z<60)
            ran_z-=100;
        if(ran_x<160 && ran_x>60)
            ran_x+=100;
        enemy[i] = Monster(ran_x, 80+16, ran_z, 2, COLOR_BALL3);
    }
    boss = Monster(boat.position.x, 68, boat.position.z+500, 2, COLOR_BALL4);
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
            wind_time+=1;
            show_explosion();
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

void canon_fire(){
    if(!canon.release && !explode){
        //audio_init("assets/song1.mp3");
        //audio_play();
        canon.release = 1;
        canon.fire(control.yrotation+180, -1*control.xrotation);
    }
}

void check_collision(int move){
    float angle = (boat.rotation-90)*pi/180;
    if(move==1){
        boat.position.z -= boat.speed*cos(angle);
        boat.position.x -= boat.speed*sin(angle);
        if(collide==0){
            for(i=0;i<r;i++){
                if(detect_collision(boat.bounding_box(), rock[i].bounding_box())){
                    collide = 1;
                    break;
                }
            }
        }
        else{
            for(i=0;i<r;i++){
                if(detect_collision(boat.bounding_box(), rock[i].bounding_box())){
                    boat.position.z += boat.speed*cos(angle);
                    boat.position.x += boat.speed*sin(angle);
                    collide = 1;
                    break;
                }
                else{
                    collide = 0;
                }
            }
        }
    }
    if(move==2){
        boat.position.z += boat.speed*cos(angle);
        boat.position.x += boat.speed*sin(angle);
        if(collide==0){
            for(i=0;i<r;i++){
                if(detect_collision(boat.bounding_box(), rock[i].bounding_box())){
                    collide = 1;
                    break;
                }
            }
        }
        else{
            for(i=0;i<r;i++){
                if(detect_collision(boat.bounding_box(), rock[i].bounding_box())){
                    boat.position.z -= boat.speed*cos(angle);
                    boat.position.x -= boat.speed*sin(angle);
                    collide = 1;
                    break;
                }
                else{
                    collide = 0;
                }
            }
        }
    }
    if(move==3){
        boat.rotation += boat.speed*0.4;
        if(collide==0){
            for(i=0;i<r;i++){
                if(detect_collision(boat.bounding_box(), rock[i].bounding_box())){
                    collide = 1;
                    break;
                }
            }
        }
        else{
            for(i=0;i<r;i++){
                if(detect_collision(boat.bounding_box(), rock[i].bounding_box())){
                    boat.rotation -= boat.speed*0.4;
                    collide = 1;
                    break;
                }
                else{
                    collide = 0;
                }
            }
        }
    }
    if(move==4){
        boat.rotation -= boat.speed*0.4;
        if(collide==0){
            for(i=0;i<r;i++){
                if(detect_collision(boat.bounding_box(), rock[i].bounding_box())){
                    collide = 1;
                    break;
                }
            }
        }
        else{
            for(i=0;i<r;i++){
                if(detect_collision(boat.bounding_box(), rock[i].bounding_box())){
                    boat.rotation += boat.speed*0.4;
                    collide = 1;
                    break;
                }
                else{
                    collide = 0;
                }
            }
        }
    }
}

void show_explosion(){
    for(i=0;i<r;i++){
        if (detect_collision(canon.bounding_box(), rock[i].bounding_box()) && canon.release) {
            explosion.set_position(canon.position.x, 60, canon.position.z);
            explosion.size = 2;
            explode = 1;
            rock[i].position -= 500;
        }
    }
    if(canon.position.y < 60 && explode == 0){
        explosion.set_position(canon.position.x, 60, canon.position.z);
        explosion.size = 2;
        explode = 1;
    }
    if(explosion.size>0){
        explosion.size-=0.03;
    }
    if(explosion.size<=0){
        explode = 0;
    }
}

void change_camera(){
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
        camera_x = boat.position.x+35*sin(theta);
        camera_y = boat.position.y+10;
        camera_z = boat.position.z+35*cos(theta);

        target_x = boat.position.x+40*sin(theta+pi);
        target_y = boat.position.y+10;
        target_z = boat.position.z+40*cos(theta+pi);
        //printf("cx=%f cy=%f cz=%f\n", camera_x, camera_y, camera_z);
        //printf("tx=%f ty=%f tz=%f\n", target_x, target_y, target_z);
    }
    else if(view==3){
        camera_x = boat.position.x;
        camera_y = 200;
        camera_z = boat.position.z;

        target_x = boat.position.x+1;
        target_y = boat.position.y;
        target_z = boat.position.z;
        //printf("cx=%f cy=%f cz=%f\n", camera_x, camera_y, camera_z);
        //printf("tx=%f ty=%f tz=%f\n", target_x, target_y, target_z);
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

void aim_canon(float x, float y) {
    if (view != 2) {
        control.yrotation = (boat.rotation-90)-((3*x/5) - 180)+180;
        printf("%f   %f\n",x,y);
        //control.zrotation = -((3*x/5) - 180);
       control.xrotation = (3*y/20) - 90;
    }
    printf("yrot=%f xrot=%f\n", control.yrotation, control.xrotation);
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
