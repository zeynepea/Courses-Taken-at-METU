#include "helper.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/matrix_inverse.hpp"
#include "glm/gtx/rotate_vector.hpp"


static GLFWwindow * win = NULL;

// Shaders
GLuint idProgramShader;
GLuint idFragmentShader;
GLuint idVertexShader;
GLuint idJpegTextureMap;
GLuint idJpegHeightMap;
GLuint idMVPMatrix;

GLuint idMVMatrix;
GLuint idNormalMatrix;
GLuint idPos;
GLuint idTextureMap;
GLuint idHeightMap;
GLuint idWidthTexture;
GLuint idHeightTexture;
GLuint idHeightFactor;
GLuint idLightPos;

int widthTexture, heightTexture;

glm::vec3 pos;
glm::vec3 v;
glm::vec3 w;
glm::vec3 u;



GLfloat camera_speed;

int widthDisplay = 1000;
int heightDisplay = 1000;
glm::vec3 LightPos;
//glm::vec3 LightPos = glm::vec3(widthDisplay / 2,100, heightDisplay / 2);

glm::mat4 viewMatrix;
glm::mat4 MVPMatrix;
glm::mat4 normalMatrix;
glm::mat4 ModelMatrix = glm::mat4(1.0f);
glm::mat4 projectionMatrix = glm::perspective(45.0f, 1.0f, 0.1f, 1000.0f);

GLfloat heightFactor = 10.0f;

int numberOfVertices;
glm::vec3* vertexArray;

bool fullscreen = false;

void vertexProcessing(){

    pos += w * camera_speed;
    viewMatrix = glm::lookAt(pos, pos + w * 0.1f, v);
    MVPMatrix = projectionMatrix * viewMatrix * ModelMatrix;
    normalMatrix = glm::inverseTranspose(viewMatrix);
}

void resetUniformMVP(){

    glUniformMatrix4fv(idMVMatrix, 1, GL_FALSE, &viewMatrix[0][0]);
    glUniformMatrix4fv(idMVPMatrix, 1, GL_FALSE, &MVPMatrix[0][0]);
    glUniformMatrix4fv(idNormalMatrix, 1, GL_FALSE, &normalMatrix[0][0]);
    glUniform3fv(idPos, 1, &pos[0]);
}


void fillVertexArray(){

    numberOfVertices = 6 * widthTexture * heightTexture;
    vertexArray = new glm::vec3[numberOfVertices];

    glm::vec3 v0, v1, v2, v3;
    int index = 0;

    for(int i = 0; i < widthTexture; i++){
        for(int j = 0; j < heightTexture; j++){

            v0 = glm::vec3(i, 0, j);
            vertexArray[index++] = v0;

            v1 = glm::vec3(i+1, 0, j+1);
            vertexArray[index++] = v1;

            v2 = glm::vec3(i+1, 0, j);
            vertexArray[index++] = v2;

            v3 = glm::vec3(i, 0, j+1);
            vertexArray[index++] = v3;

            vertexArray[index++] = v0,
            vertexArray[index++] = v1;
        }
    }
}


void setUniformVariables(){

    idMVMatrix = glGetUniformLocation(idProgramShader, "MV");
    glUniformMatrix4fv(idMVMatrix, 1, GL_FALSE, &viewMatrix[0][0]);

    idMVPMatrix = glGetUniformLocation(idProgramShader, "MVP");
    glUniformMatrix4fv(idMVPMatrix, 1, GL_FALSE, &MVPMatrix[0][0]);

    idNormalMatrix = glGetUniformLocation(idProgramShader, "normalMatrix");
    glUniformMatrix4fv(idNormalMatrix, 1, GL_FALSE, &normalMatrix[0][0]);

    idPos = glGetUniformLocation(idProgramShader, "cameraPosition");
    glUniform3fv(idPos, 1, &pos[0]);

    idTextureMap = glGetUniformLocation(idProgramShader, "textureMap");
    glUniform1i(idTextureMap, 0);

    idHeightMap = glGetUniformLocation(idProgramShader, "heightMap");
    glUniform1i(idHeightMap, 1);

    idWidthTexture = glGetUniformLocation(idProgramShader, "widthTexture");
    glUniform1i(idWidthTexture, widthTexture);

    idHeightTexture = glGetUniformLocation(idProgramShader, "heightTexture");
    glUniform1i(idHeightTexture, heightTexture);

    idHeightFactor = glGetUniformLocation(idProgramShader, "heightFactor");
    glUniform1f(idHeightFactor, heightFactor);

    idLightPos = glGetUniformLocation(idProgramShader, "light_pos");
    glUniform3fv(idLightPos,1, &LightPos[0]);
}

void toggleFullScreen(){

    if(fullscreen){
        glfwSetWindowMonitor(win, nullptr, 0, 0, widthDisplay, heightDisplay, 0);
    }
    else{
        GLFWmonitor* monitor = glfwGetPrimaryMonitor();
        const GLFWvidmode* videoMode = glfwGetVideoMode(monitor);
        glfwSetWindowMonitor (win, monitor, 0, 0, videoMode->width, videoMode->height, GL_DONT_CARE);
        glViewport(0, 0, videoMode->width, videoMode->height);
    }
    fullscreen = !fullscreen;
}

void changeYaw(GLfloat x){

    u = glm::rotate(u, x, v);
    w = glm::rotate(w, x, v);
}

void changePitch(GLfloat x){
    
    v = glm::rotate(v, x, u);
    w = glm::rotate(w, x, u);
}

void changeHeightFactor(GLfloat f){

    heightFactor += f;
    glUniform1f(idHeightFactor, heightFactor);
}

void changeLigtPosY(GLfloat f){

     LightPos = glm::vec3(LightPos[0], LightPos[1] + f, LightPos[2]);
     glUniform3fv(idLightPos,1, &LightPos[0]);
}

void changeLigtPosX(GLfloat f){

     LightPos = glm::vec3(LightPos[0] + f, LightPos[1] , LightPos[2]);
     glUniform3fv(idLightPos,1, &LightPos[0]);
}

void changeLigtPosZ(GLfloat f){

     LightPos = glm::vec3(LightPos[0], LightPos[1] , LightPos[2]+ f);
     glUniform3fv(idLightPos,1, &LightPos[0]);
}

void setCameraToInitPostion(){

    pos = glm::vec3(widthTexture/2, widthTexture/10, -widthTexture/4);
    camera_speed = 0.0f;
    v = glm::vec3(0.0, 1.0, 0.0);
    w = glm::vec3(0.0, 0.0, 1.0);
    u = cross(v, w);
}

void setPlaneToInitPostion(){

    heightFactor = 10.0f;
    glUniform1f(idHeightFactor, heightFactor);
}

static void keypress(GLFWwindow *win, int key, int scode, int act, int mods){

    if(act == GLFW_PRESS || act == GLFW_REPEAT){
        switch(key){

            case GLFW_KEY_P:
                toggleFullScreen();
                break;

            case GLFW_KEY_ESCAPE:
                glfwSetWindowShouldClose(win, GLFW_TRUE);
                break;
            
            case GLFW_KEY_I:{
                setCameraToInitPostion();
                setPlaneToInitPostion();
                break;
            }

            case GLFW_KEY_Y:
                camera_speed += 0.01;
                break;

            case GLFW_KEY_H:
                camera_speed -= 0.01;
                break;
            
            case GLFW_KEY_R:
                changeHeightFactor(0.5f);
                break;

            case GLFW_KEY_F:
                changeHeightFactor(-0.5f);
                break;

            case GLFW_KEY_A:
                changeYaw(0.05f);
                break;

            case GLFW_KEY_D:
                changeYaw(-0.05f);
                break;

            case GLFW_KEY_S:
                changePitch(0.05f);
                break;

            case GLFW_KEY_W:
                changePitch(-0.05f);
                break;

            case GLFW_KEY_T:
                changeLigtPosY(5.0f);
                break;

            case GLFW_KEY_G:
                changeLigtPosY(-5.0f);
                break;
            
            case GLFW_KEY_RIGHT:
                changeLigtPosX(5.0f);
                break;

            case GLFW_KEY_LEFT:
                changeLigtPosX(-5.0f);
                break;
            
            case GLFW_KEY_UP:
                changeLigtPosZ(5.0f);
                break;

            case GLFW_KEY_DOWN:
                changeLigtPosZ(-5.0f);
                break;
        }
    }
}

void resizeWindow(GLFWwindow* window, int width, int height) {

    glViewport(0, 0, width, height );
}

static void errorCallback(int error,
  const char * description) {
  fprintf(stderr, "Error: %s\n", description);
}

int main(int argc, char * argv[]) {

    if (argc != 3) {
        printf("Only two textures image expected!\n");
        exit(-1);
    }

    glfwSetErrorCallback(errorCallback);

    if (!glfwInit()) {
        exit(-1);
    }

    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

    win = glfwCreateWindow(1000, 1000, "CENG477 - HW3", NULL, NULL);

    if (!win) {
        glfwTerminate();
        exit(-1);
    }
    glfwMakeContextCurrent(win);

    GLenum err = glewInit();
    if (err != GLEW_OK) {
        fprintf(stderr, "Error: %s\n", glewGetErrorString(err));

        glfwTerminate();
        exit(-1);
    }

    glEnable(GL_DEPTH_TEST);
    glDepthFunc(GL_LESS);

    glEnable(GL_LIGHTING);
    glEnable(GL_LIGHT0);

    initShaders();
    glUseProgram(idProgramShader);
    initHeightMap(argv[1], & widthTexture, & heightTexture);
    initTextureMap(argv[2], & widthTexture, & heightTexture);


    setCameraToInitPostion();
    LightPos = glm::vec3(widthTexture / 2,100, heightTexture / 2);
    vertexProcessing();

    fillVertexArray();
    setUniformVariables();

    glfwSetKeyCallback(win, keypress);
    glfwSetWindowSizeCallback(win, resizeWindow); 

    while (!glfwWindowShouldClose(win)) {

        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        vertexProcessing();
        resetUniformMVP();

        glEnableClientState(GL_VERTEX_ARRAY);
        glVertexPointer(3, GL_FLOAT, 0, vertexArray);
        glDrawArrays(GL_TRIANGLES, 0, numberOfVertices);
        glDisableClientState(GL_VERTEX_ARRAY);

        glfwSwapBuffers(win);
        glfwPollEvents();
  }

  glfwDestroyWindow(win);
  glfwTerminate();

  return 0;
}
