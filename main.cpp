//
//  main.cpp
//  Basketball game
//
//  Created by ratul on 28/10/23.
//

#define _CRT_SECURE_NO_DEPRECATE
#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <stdio.h>

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "shader.h"
#include "camera.h"
#include "basic_camera.h"
#include "pointLight.h"
#include "directionalLight.h"
#include "SpotLight.h"
#include "cube.h"
#include "triangle.h"
#include "sphere.h"
#include "stb_image.h"

#include <iostream>
#include <vector>

using namespace std;

void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods);
void mouse_button_callback(GLFWwindow* window, int button, int action, int mods);
void mouse_callback(GLFWwindow* window, double xpos, double ypos);
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset);
void processInput(GLFWwindow* window);
unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax);
//void bed(Shader& lightingShader, glm::mat4 alTogether, Cube& cube);
void scsToWcs(float sx, float sy, float wcsv[3]);
unsigned int drawControlPoints();
void saveControlPoints();
long long nCr(int n, int r);
void BezierCurve(double t, float xy[2], GLfloat ctrlpoints[], int L);
unsigned int hollowBezier(GLfloat ctrlpoints[], int L, float tx, float ty);
void drawCube(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model, float r, float g, float b);
void bed(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether);


// settings
const unsigned int SCR_WIDTH = 800;
const unsigned int SCR_HEIGHT = 600;

// modelling transform
float rotateAngle_X = 0.0;
float rotateAngle_Y = 0.0;
float rotateAngle_Z = 0.0;
float rotateAxis_X = 0.0;
float rotateAxis_Y = 0.0;
float rotateAxis_Z = 1.0;
float translate_X = 0.0;
float translate_Y = 0.0;
float translate_Z = 0.0;
float scale_X = 0.5;
float scale_Y = 0.5;
float scale_Z = 0.5;

//Human coordinate
float x_position = 0.0f;
float z_position = 0.0f;
float human_speed = 0.005f;
float horizontal_angle = 0.0f;   // must be in degree
float vertical_angle = 45.0f;

//Ball information
float ball_x_position = 0.0f;
float ball_y_position = 0.0f;
float ball_z_position = 0.0f;
float ball_velocity = 0.411998f;         // unit: m/s
float ball_del_x = 0.0f;
float ball_del_y = 0.0f;
float ball_del_z = 0.0f;
float ball_animation = false;
float ball_animation_speed = 0.06f;

const float g = .09; // unit: m/s^2
float u = 0.f;


//Ring position
float ring_x_position = 1.3f;
float ring_y_position = 0.3f;
float ring_z_position = 0.0f;

//Error
float considerble_distance = 2.0f;

//Scoring
float score = 0;


glm::mat4 model = glm::mat4(1.0f);
glm::mat4 view = glm::mat4(1.0f);
glm::mat4 projection = glm::mat4(1.0f);

// viewport
GLint viewport[4];

/////////////////////////////////////////////////////////////
float wcsClkDn[3], wcsClkUp[3];
vector <float> cntrlPoints = {//ankon
{
-1.2350, 1.5050, 5.1000,
-1.3800, 1.4700, 5.1000,
-1.4900, 1.3800, 5.1000,
-1.5000, 1.2900, 5.1000,
-1.4450, 1.1850, 5.1000,
-1.3100, 1.1600, 5.1000,
-1.1550, 1.1800, 5.1000,
-1.0650, 1.2100, 5.1000,
-0.9700, 1.2600, 5.1000,
-0.9650, 1.3550, 5.1000,
-1.0300, 1.4650, 5.1000,
-1.1550, 1.5100, 5.1000,
-1.3100, 1.5050, 5.1000,
-1.4300, 1.4400, 5.1000,
-0.9400, 1.3100, 5.1000,
-0.9450, 1.2150, 5.1000,
-0.9500, 1.1550, 5.1000,
-0.8800, 1.0350, 5.1000,
-0.8200, 0.8850, 5.1000,
-0.7800, 0.7600, 5.1000,
-0.7700, 0.6700, 5.1000,
-0.7300, 0.5550, 5.1000,
-0.7300, 0.4700, 5.1000,
-0.7050, 0.3800, 5.1000,
-0.6850, 0.2800, 5.1000,
-0.7000, 0.2050, 5.1000,
-0.7250, 0.1450, 5.1000,
-0.7900, 0.1000, 5.1000,
}
};
vector <float> coordinates;
vector <float> normals;
vector <int> indices;
vector <float> vertices;
class point
{
public:
    point()
    {
        x = 0;
        y = 0;
    }
    int x;
    int y;
} clkpt[2];
int mouseButtonFlag = 0;
FILE* fp;
const double pi = 3.14159265389;
const int nt = 40;
const int ntheta = 20;
bool showControlPoints = true;
bool loadBezierCurvePoints = true;
bool showHollowBezier = false;
bool lineMode = false;
unsigned int bezierVAO;

// camera
Camera camera(glm::vec3(0.0f, 0.0f, 0.9f));
// camera
//Camera camera(glm::vec3(0.0f, 1.1f, 5.2f));
float lastX = SCR_WIDTH / 2.0f;
float lastY = SCR_HEIGHT / 2.0f;
bool firstMouse = true;


float eyeX = 0.0, eyeY = 1.0, eyeZ = 1.0;
float lookAtX = 0.0, lookAtY = 0.0, lookAtZ = 0.0;
glm::vec3 V = glm::vec3(0.0f, 1.0f, 0.0f);
BasicCamera basic_camera(eyeX, eyeY, eyeZ, lookAtX, lookAtY, lookAtZ, V);

// lights
// positions of the point lights
glm::vec3 pointLightPositions[] = {
    glm::vec3(1.3f,  0.5f,  1.3f),
    glm::vec3(1.3f,  0.5f,  -1.3f),
    glm::vec3(-1.3f,  0.5f,  1.3f),
    glm::vec3(-1.3f,  0.5f,  -1.3f)
};
PointLight pointlight1(

    pointLightPositions[0].x, pointLightPositions[0].y, pointLightPositions[0].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.14f,  //k_l
    0.07f, //k_q
    1       // light number
);
PointLight pointlight2(

    pointLightPositions[1].x, pointLightPositions[1].y, pointLightPositions[1].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.14f,  //k_l
    0.07f, //k_q
    2       // light number
);
PointLight pointlight3(

    pointLightPositions[2].x, pointLightPositions[2].y, pointLightPositions[2].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.14f,  //k_l
    0.07f, //k_q
    3       // light number
);
PointLight pointlight4(

    0, pointLightPositions[3].y, pointLightPositions[3].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.14f,  //k_l
    0.07f, //k_q
    4       // light number
);
PointLight pointlight5(

    0, pointLightPositions[0].y+1.0, pointLightPositions[0].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.14f,  //k_l
    0.07f, //k_q
    1       // light number
);

PointLight pointlight6(

    pointLightPositions[0].x, pointLightPositions[2].y+1.0, pointLightPositions[2].z,  // position
    0.05f, 0.05f, 0.05f,     // ambient
    0.8f, 0.8f, 0.8f,     // diffuse
    1.0f, 1.0f, 1.0f,        // specular
    1.0f,   //k_c
    0.14f,  //k_l
    0.07f, //k_q
    1       // light number
);

DirLight dirLight(
    -0.2f, -1.0f, -0.3f,
    0.1f, 0.1f, 0.1f,        //ambient
    0.2f, 0.2f, 0.2f,     // diffuse
    0.1f, 0.1f, 0.1f
);

SpotLight spotLight(
    0.0f, 2.0f, 0.0f,        // co-ordinate of spotlight
    0.0f, -1.0f, 0.0f,      // Direction of spot light
    0.3f, 0.3f, 0.3f,        //ambient
    0.65f, 0.65f, 0.65f,     // diffuse
    0.5f, 0.5f, 0.5f,       //specular
    1.0f,
    0.09f,
    0.032f,
    12.5f,
    15.0f
);


// light settings
bool dirLightOn = true;
bool pointLightOn = true;
bool spotLightOn = true;
bool ambientToggle = true;
bool diffuseToggle = true;
bool specularToggle = true;


// timing
float deltaTime = 0.0f;    // time between current frame and last frame
float lastFrame = 0.0f;


void delay(float secs)
{
    float end = clock() / CLOCKS_PER_SEC + secs;
    while ((clock() / CLOCKS_PER_SEC) < end);
}




void drawCourt(Cube& cube, Shader& lightingShaderWithTexture)
{
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.4f, 0.0f, -0.75f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.8f, 0.01f, 1.5f));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;


    glm::mat4 modelMatrixForContainer = glm::mat4(1.0f);
    modelMatrixForContainer = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);

}

void drawBackboard(Cube& cube, Shader& lightingShaderWithTexture)
{
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
    translateMatrix = glm::translate(identityMatrix, glm::vec3(1.4f, 0.3f, -0.09f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.01f, 0.10f, 0.18f));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;

    glm::mat4 modelMatrixForContainer = glm::mat4(1.0f);
    modelMatrixForContainer = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);



    translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.4f, 0.3f, -0.09f));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.01f, 0.10f, 0.18f));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;

    modelMatrixForContainer = glm::mat4(1.0f);
    modelMatrixForContainer = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    cube.drawCubeWithTexture(lightingShaderWithTexture, modelMatrixForContainer);


}

void drawGallery(Triangle& triangle, Cube& cube, Shader& lightingShaderWithTexture)
{
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;

    // back gallery
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.4f - 0.3f, 0.0f, -0.75f - 0.3f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.3f, 0.25f, 1.5f + 0.6f));
    model = translateMatrix * scaleMatrix * rotateYMatrix;

    triangle.drawTriangleWithTexture(lightingShaderWithTexture, model);


    // front gallery
    translateMatrix = glm::translate(identityMatrix, glm::vec3(1.4f + 0.3, 0.0f, 0.75f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(180.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.3f, 0.25f, 1.5f + 0.26f));
    model = translateMatrix * scaleMatrix * rotateYMatrix;

    triangle.drawTriangleWithTexture(lightingShaderWithTexture, model);


    // left gallery
    translateMatrix = glm::translate(identityMatrix, glm::vec3(1.4f + 0.3f, 0.0f, -0.75f - 0.3f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(-90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.8f + 0.3f + 0.3f, 0.25f, 0.3f));
    model = translateMatrix * scaleMatrix * rotateYMatrix;
    triangle.drawTriangleWithTexture(lightingShaderWithTexture, model);

    //right gallery
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-1.4f - 0.2f, 0.0f, -0.75f + 1.8f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(90.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(2.8f + 0.0f + 0.0f, 0.25f, 0.3f));
    model = translateMatrix * scaleMatrix * rotateYMatrix;

    triangle.drawTriangleWithTexture(lightingShaderWithTexture, model);
}


void drawPlayer(Shader& shader, glm::mat4 alTogether,Shader& shader2, Shader& shader3) {


    glm::mat4 identityMatrix = glm::mat4(1.0f);
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
    Cube cube1(glm::vec3(0.5, 0.5, 0.5), glm::vec3(0.5, 0.5, 0.5), glm::vec3(0.5, 0.5, 0.5), 0);
    string diffuseMapPath = "Blue_Tshirt.jpg";
    string specularMapPath = "whiteBackground.png";
    Cube cube = Cube(loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR), loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR), 100.0f, 0.0f, 0.0f, 2.0f, 1.0f);

    //    Triangle triangle(glm::vec3(0.5, 0.5, 0.5), glm::vec3(0.5, 0.5, 0.5), glm::vec3(0.5, 0.5, 0.5), 30);

       //0,0
        //cube1 (Head)
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.025f+0.025f, 0.15f, -0.025f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.04f, 0.04f, 0.04f));
    model = alTogether * translateMatrix * scaleMatrix;
    //cube.drawCube(shader, model, 1.0f, 0.0f, 0.0f);

    //head
    //Sphere(float radius = 1.0f, int sectorCount = 36, int stackCount = 18, glm::vec3 amb = glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3 diff = glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3 spec = glm::vec3(0.5f, 0.5f, 0.5f), float shiny = 32.0f) : verticesStride(24)
    //{
      //  set(radius, sectorCount, stackCount, amb, diff, spec, shiny);

     Sphere sphere = Sphere(1.0,36,18, glm::vec3(0.6,0.6,0.6), glm::vec3(0.6, 0.6, 0.6), glm::vec3(0.6, 0.6, 0.6),6);

   // glm::mat4 modelForSphere = glm::mat4(1.0f);
    //modelForSphere = glm::translate(model, glm::vec3(1.5f, 1.2f, 0.5f));
  //  modelForSphere = glm::translate(model, glm::vec3(x_position, ball_y_position, z_position)) * glm::scale(identityMatrix, glm::vec3(0.1f, 0.1f, 0.1f)) * glm::translate(identityMatrix, glm::vec3(0, 2.8, 0));
    // modelForSphere = glm::translate(model, glm::vec3(1.5f, 1.2f, 0.5f));
    sphere.drawSphere(shader3, model);


    //void drawCubeWithTexture(Shader & lightingShaderWithTexture, glm::mat4 model = glm::mat4(1.0f))
    //cube2 (Body)
   //  translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.015f, 0.05f, -0.05f));
   //  scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.03f, 0.1f, 0.1f));
   //  model = alTogether * translateMatrix * scaleMatrix;
  //   cube.drawCube(shader, model, 0.9f, 1.0f, 0.9f);

    //body
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.015f, 0.1f, -0.05f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.03f, 0.05f, 0.1f));
    model = alTogether * translateMatrix * scaleMatrix;
    cube.drawCubeWithTexture(shader, model);

    //cube3 (Hands)
    //translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.05f, 0.05f));
    //scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.02f, 0.07f, 0.02f));
    //model = alTogether * translateMatrix * scaleMatrix;
   // cube.drawCube(shader, model, 0.0f, 0.0f, 1.0f);

    //hand1
   // diffuseMapPath = "hand1.jpg";
    //specularMapPath = "whiteBackground.pn
   // cube = Cube(loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR), loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR), 100.0f, 0.0f, 0.0f, 2.0f, 1.0f);
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.12f, 0.05f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(-0.08f, 0.02f, 0.02f));
    //rotateYMatrix = glm::rotate(identityMatrix, glm::radians(135.0f), glm::vec3(0.0f, 1.0f, 0.0f));
    model = alTogether * translateMatrix * scaleMatrix ;
    cube.drawCubeWithTexture(shader, model);

     translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.06f, -0.05f - 0.02f));
     scaleMatrix = glm::scale(identityMatrix, glm::vec3(-0.08f, 0.02f, 0.02f));
     model = alTogether * translateMatrix * scaleMatrix;
     cube.drawCubeWithTexture(shader, model);

    //cube4 (Legs)
      //  diffuseMapPath = "container2.jpg";
      //specularMapPath = "whiteBackground.png";
     // cube = Cube(loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR), loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR), 100.0f, 0.0f, 0.0f, 2.0f, 1.0f);
      translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.015f, 0.0f, 0.01f));
      scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.03f, 0.05f, 0.03f));
     
      model = alTogether * translateMatrix * scaleMatrix;
      cube1.drawCube(shader2, model, 0.0f, 0.0f, 0.0f);

      translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.015f, 0.0f, -0.01f - 0.03f));
      scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.03f, 0.05f, 0.03f));
      model = alTogether * translateMatrix * scaleMatrix;
      cube1.drawCube(shader2, model, 0.0f, 0.0f, 0.0f);

}

void drawFlag(Shader& shader, glm::mat4 alTogether, Shader& shader1)
{
    glm::mat4 identityMatrix = glm::mat4(1.0f);
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
    Cube cube1(glm::vec3(0.5, 0.5, 0.5), glm::vec3(0.5, 0.5, 0.5), glm::vec3(0.5, 0.5, 0.5), 32);
    string diffuseMapPath = "aus.png";
    string specularMapPath = "whiteBackground.png";
    Cube cube = Cube(loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR), loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR), 100.0f, 0.0f, 0.0f, 2.0f, 1.0f);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.2f, 0.0));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.16f, 0.1f, 0.0f));
    model = alTogether * translateMatrix * scaleMatrix;

    cube.drawCubeWithTexture(shader, model);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f,0.0));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.01f, 0.3f, 0.01f));
    model = alTogether * translateMatrix * scaleMatrix;
    cube1.drawCube(shader1, model,0.0,0.0,0.0);


}

void drawBanner(Shader& shader, glm::mat4 alTogether, Cube& cube, Shader& shader1)
{
    glm::mat4 identityMatrix = glm::mat4(1.0f);
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
    Cube cube1(glm::vec3(0.5, 0.5, 0.5), glm::vec3(0.5, 0.5, 0.5), glm::vec3(0.5, 0.5, 0.5), 32);
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 0.0));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.01f, 0.3f, 0.01f));
    model = alTogether * translateMatrix * scaleMatrix;
    cube1.drawCube(shader, model, 0.0, 0.0, 0.0);
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.2f, 0.0));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.0f, 0.1f, 0.16f));
    model = alTogether * translateMatrix * scaleMatrix;
    //model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;


    glm::mat4 modelMatrixForContainer = glm::mat4(1.0f);
    modelMatrixForContainer = glm::translate(model, glm::vec3(0.0f, 0.0f, 0.0f));
    cube.drawCubeWithTexture(shader1, modelMatrixForContainer);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 0.16f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.01f, 0.3f, 0.01f));
    model = alTogether * translateMatrix * scaleMatrix;
    cube1.drawCube(shader, model, 0.0, 0.0, 0.0);
}

void drawNext(Shader& shader, glm::mat4 alTogether, Shader& shader2, Shader& shader3) {


    glm::mat4 identityMatrix = glm::mat4(1.0f);
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
    // Cube cube(glm::vec3(0.5, 0.5, 0.5), glm::vec3(0.5, 0.5, 0.5), glm::vec3(0.5, 0.5, 0.5), 0);
    string diffuseMapPath = "Blue_Tshirt.jpg";
    string specularMapPath = "whiteBackground.png";
    Cube cube = Cube(loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR), loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR), 100.0f, 0.0f, 0.0f, 2.0f, 1.0f);

    //    Triangle triangle(glm::vec3(0.5, 0.5, 0.5), glm::vec3(0.5, 0.5, 0.5), glm::vec3(0.5, 0.5, 0.5), 30);

       //0,0
        //cube1 (Head)
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.025f + 0.025f, 0.15f, -0.025f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.04f, 0.04f, 0.04f));
    model = alTogether * translateMatrix * scaleMatrix;
    //cube.drawCube(shader, model, 1.0f, 0.0f, 0.0f);

    //head
    //Sphere(float radius = 1.0f, int sectorCount = 36, int stackCount = 18, glm::vec3 amb = glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3 diff = glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3 spec = glm::vec3(0.5f, 0.5f, 0.5f), float shiny = 32.0f) : verticesStride(24)
    //{
      //  set(radius, sectorCount, stackCount, amb, diff, spec, shiny);

    Sphere sphere = Sphere(1.0, 36, 18, glm::vec3(0.6, 0.6, 0.6), glm::vec3(0.6, 0.6, 0.6), glm::vec3(0.6, 0.6, 0.6), 6);

    // glm::mat4 modelForSphere = glm::mat4(1.0f);
     //modelForSphere = glm::translate(model, glm::vec3(1.5f, 1.2f, 0.5f));
   //  modelForSphere = glm::translate(model, glm::vec3(x_position, ball_y_position, z_position)) * glm::scale(identityMatrix, glm::vec3(0.1f, 0.1f, 0.1f)) * glm::translate(identityMatrix, glm::vec3(0, 2.8, 0));
     // modelForSphere = glm::translate(model, glm::vec3(1.5f, 1.2f, 0.5f));
    sphere.drawSphere(shader3, model);


    //void drawCubeWithTexture(Shader & lightingShaderWithTexture, glm::mat4 model = glm::mat4(1.0f))
    //cube2 (Body)
   //  translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.015f, 0.05f, -0.05f));
   //  scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.03f, 0.1f, 0.1f));
   //  model = alTogether * translateMatrix * scaleMatrix;
  //   cube.drawCube(shader, model, 0.9f, 1.0f, 0.9f);

    //body
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.015f, 0.042f, -0.05f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.03f, 0.1f, 0.1f));
    model = alTogether * translateMatrix * scaleMatrix;
    cube.drawCubeWithTexture(shader, model);

    //cube3 (Hands)
    //translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.05f, 0.05f));
    //scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.02f, 0.07f, 0.02f));
    //model = alTogether * translateMatrix * scaleMatrix;
   // cube.drawCube(shader, model, 0.0f, 0.0f, 1.0f);

    //hand1
   // diffuseMapPath = "hand1.jpg";
    //specularMapPath = "whiteBackground.png";
   // cube = Cube(loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR), loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR), 100.0f, 0.0f, 0.0f, 2.0f, 1.0f);
    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.05f, 0.05f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.02f, 0.07f, 0.02f));
    model = alTogether * translateMatrix * scaleMatrix;
    cube.drawCubeWithTexture(shader, model);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(0.0f, 0.05f, -0.05f - 0.00f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.02f, -0.07f, 0.02f));
    model = alTogether * translateMatrix * scaleMatrix;
    cube.drawCubeWithTexture(shader, model);

    //cube4 (Legs)
      //  diffuseMapPath = "container2.jpg";
      //specularMapPath = "whiteBackground.png";
     // cube = Cube(loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR), loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR), 100.0f, 0.0f, 0.0f, 2.0f, 1.0f);
    translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.015f, 0.0f, 0.01f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.03f, 0.05f, 0.03f));
    model = alTogether * translateMatrix * scaleMatrix;
    cube.drawCube(shader2, model, 0.0f, 0.0f, 0.0f);

    translateMatrix = glm::translate(identityMatrix, glm::vec3(-0.015f, 0.0f, -0.01f - 0.03f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(0.03f, 0.05f, 0.03f));
    model = alTogether * translateMatrix * scaleMatrix;
    cube.drawCube(shader2, model, 0.0f, 0.0f, 0.0f);

}

void drawBasketBall(Shader& lightingShader)
{
  //  Sphere ball = Sphere(1, 36, 18, glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.5f, 0.5f, 0.5f), glm::vec3(0.0f, 0.0f, 0.0f));

 //   glm::mat4 model, identityMatrix;
 //   glm::mat4 modelForBall = glm::mat4(1.0f);
 //   modelForBall = glm::translate(model, glm::vec3(ball_x_position, ball_y_position, ball_z_position)) * glm::scale(identityMatrix, glm::vec3(0.4f, 0.0f, 0.0`````f));
 //   ball.drawSphere(lightingShader, modelForBall);
}

void drawRing(Shader& lightingShader, float s, float tx, float ty, float tz)//ankon
{

    // be sure to activate shader when setting uniforms/drawing objects
    lightingShader.use();
    lightingShader.setVec3("viewPos", camera.Position);


    // activate shader
    lightingShader.use();
    projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
    //projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
    lightingShader.setMat4("projection", projection);

    // camera/view transformation
        //view = camera.GetViewMatrix();
    glm::mat4 view = basic_camera.createViewMatrix();
    lightingShader.setMat4("view", view);



    // Modelling Transformation
    glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
    glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
    translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X, translate_Y, translate_Z));
    rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
    rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
    rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
    scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X, scale_Y, scale_Z));
    model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
    lightingShader.setMat4("model", model);

    for (int i = 0; i < (int)cntrlPoints.size(); i += 3)
    {
        cntrlPoints[i] /= s;
        cntrlPoints[i + 1] /= s;
        cntrlPoints[i + 2] /= s;
    }

    for (int i = 0; i < (int)cntrlPoints.size(); i += 3)
    {
        //cntrlPoints[i] -= tx;
       //// cntrlPoints[i + 1] -= ty;
       // cntrlPoints[i + 2] -= tz;
    }


    if (loadBezierCurvePoints)
    {
        bezierVAO = hollowBezier(cntrlPoints.data(), ((unsigned int)cntrlPoints.size() / 3) - 1, tx, ty);
        loadBezierCurvePoints = false;
        showHollowBezier = true;
    }
    if (showHollowBezier)
    {
        lightingShader.use();
        lightingShader.setVec3("material.ambient", glm::vec3(1.0f, 0.66f, 0.0f));
        lightingShader.setVec3("material.diffuse", glm::vec3(1.0f, 0.66f, 0.0f));
        lightingShader.setVec3("material.specular", glm::vec3(1.0f, 0.66f, 0.0f));
        lightingShader.setFloat("material.shininess", 48.0f);

        glBindVertexArray(bezierVAO);
        glDrawElements(GL_TRIANGLES,                    // primitive type
            (unsigned int)indices.size(),          // # of indices
            GL_UNSIGNED_INT,                 // data type
            (void*)0);                       // offset to indices

        // unbind VAO
        glBindVertexArray(0);
    }
}




int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "Basketball", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetMouseButtonCallback(window, mouse_button_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_NORMAL);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram //R
    // ------------------------------------
    Shader lightingShaderWithTexture("vertexShaderForPhongShadingWithTexture.vs", "fragmentShaderForPhongShadingWithTexture.fs");
    Shader lightingShader("vertexShaderForPhongShading.vs", "fragmentShaderForPhongShading.fs");
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    string diffuseMapPath = "court.jpg";
    string specularMapPath = "whiteBackground.png";


    unsigned int diffMap = loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    unsigned int specMap = loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR);
    Cube cube = Cube(diffMap, specMap, 32.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    //making cube for court
    diffuseMapPath = "court.jpg";
    specularMapPath = "whiteBackground.png";
    Cube cubeCourt = Cube(loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR), loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR), 30.0f, 0.0f, 0.0f, 1.0f, 1.0f);

    // making cube for backboard
    diffuseMapPath = "backboard.jpg";
    specularMapPath = "whiteBackground.png";
    Cube cubeBackboard = Cube(loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR), loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR), 100.0f, 0.0f, 0.0f, 1.0f, 1.0f);



    // making Triangle for gallery
    diffuseMapPath = "gallery.jpg";
    specularMapPath = "whiteBackground.png";
    Triangle triangleGallery = Triangle(loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR), loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR), 100.0f, 0.0f, 0.0f, 2.0f, 1.0f);

    // making wall for gallery
    diffuseMapPath = "gallery.jpg";
    specularMapPath = "whiteBackground.png";
    Cube cubeGallery = Cube(loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR), loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR), 200.0f, 0.0f, 0.0f, 2.0f, 1.0f);

    diffuseMapPath = "download.jpg";
    specularMapPath = "whiteBackground.png";
    Cube flagGallery = Cube(loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR), loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR), 200.0f, 0.0f, 0.0f, 2.0f, 1.0f);

    diffuseMapPath = "aus.jpg";
    specularMapPath = "whiteBackground.png";
    Cube flagGallery2 = Cube(loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR), loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR), 200.0f, 0.0f, 0.0f, 2.0f, 1.0f);

    diffuseMapPath = "top.png";
    specularMapPath = "whiteBackground.png";
    Cube  cubeBanner = Cube(loadTexture(diffuseMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR), loadTexture(specularMapPath.c_str(), GL_REPEAT, GL_REPEAT, GL_LINEAR_MIPMAP_LINEAR, GL_LINEAR), 200.0f, 0.0f, 0.0f, 2.0f, 1.0f);

    float cube_vertices[] = {
        // positions      // normals
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,

        1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

        0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,

        0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,

        0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f
    };
    unsigned int cube_indices[] = {
        0, 3, 2,
        2, 1, 0,

        4, 5, 7,
        7, 6, 4,

        8, 9, 10,
        10, 11, 8,

        12, 13, 14,
        14, 15, 12,

        16, 17, 18,
        18, 19, 16,

        20, 21, 22,
        22, 23, 20
    };

    float octagon[] = {
0.0f, 0.0f, 0.0f, 0.0f, 0.0f, 0.0f,
-1.0f, 0.0f, -3.0f, 1.0f, 0.0f, 0.0f, // RED
1.0f, 0.0f, -3.0f, 0.0f, 1.0f, 0.0f, // GREEN
3.0f, 0.0f, -1.0f, 0.0f, 0.0f, 1.0f, // BLUE
3.0f, 0.0f, 1.0f, 1.0f, 1.0f, 1.0f, // WHITE
1.0f, 0.0f, 3.0f, 1.0f, 0.0f, 1.0f, // PINK
-1.0f, 0.0f, 3.0f, 1.0f, 1.0f, 0.0f, // KHOYERI
-3.0f, 0.0f, 1.0f, 0.0f, 1.0f, 1.0f, // SKY BLUE
-3.0f, 0.0f, -1.0f, .5f, 1.0f, 0.0f, // YELLOW

0.0f, -1.0f, 0.0f, 0.0f, 0.0f, 0.0f,
-1.0f, -1.0f, -3.0f, 1.0f, 0.0f, 0.0f, // RED
1.0f, -1.0f, -3.0f, 0.0f, 1.0f, 0.0f, // GREEN
3.0f, -1.0f, -1.0f, 0.0f, 0.0f, 1.0f, // BLUE
3.0f, -1.0f, 1.0f, 1.0f, 1.0f, 1.0f, // WHITE
1.0f, -1.0f, 3.0f, 1.0f, 0.0f, 1.0f, // PINK
-1.0f, -1.0f, 3.0f, 1.0f, 1.0f, 0.0f, // KHOYERI
-3.0f, -1.0f, 1.0f, 0.0f, 1.0f, 1.0f, // SKY BLUE
-3.0f, -1.0f, -1.0f, .5f, 1.0f, 0.0f, // YELLOW
    };
    unsigned int oct_indices[] = {
        1,2,3,
        1,3,4,
        1,4,5,
        1,5,6,
        1,6,7,
        1,7,8,


        10,11,12,
        10,12,13,
        10,13,14,
        10,14,15,
        10,15,16,
        10,16,17,


        12,2,11,
        12,3,2,

        /*11,9,18,
        11,2,9,*/

        13,3,12,
        13,4,3,

        14,4,13,
        14,5,4,

        15,5,14,
        15,6,5,

        16,6,15,
        16,7,6,

        17,7,16,
        17,8,7,

        /*18,8,17,
        18,9,8*/


        17,8,10,
        8,1,10,

        1,2,11,
        1,11,10
    };

    unsigned int cubeVAO, cubeVBO, cubeEBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glGenBuffers(1, &cubeEBO);

    glBindVertexArray(cubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);


    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // vertex normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);

    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    //for octagon
    unsigned int VBOoct, VAOoct, EBOoct;
    glGenVertexArrays(1, &VAOoct);
    glGenBuffers(1, &VBOoct);
    glGenBuffers(1, &EBOoct);
    glBindVertexArray(VAOoct);
    glBindBuffer(GL_ARRAY_BUFFER, VBOoct);
    glBufferData(GL_ARRAY_BUFFER, sizeof(octagon), octagon, GL_STATIC_DRAW);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBOoct);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(oct_indices), oct_indices, GL_STATIC_DRAW);
    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);
    //color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);

    Sphere sphere = Sphere();




    //    Sphere sphere = Sphere();

    //    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);


        // render loop
        // -----------
    while (!glfwWindowShouldClose(window))
    {
      
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(1.0f, 1.0f, 1.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Modelling Transformation
        glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
        translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X, translate_Y, translate_Z));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X, scale_Y, scale_Z));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;

        lightingShader.use();
     
        lightingShader.setVec3("viewPos", camera.Position);

        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
        lightingShader.setMat4("projection", projection);

        // camera/view transformation
        //glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 view = basic_camera.createViewMatrix();
        lightingShader.setMat4("view", view);

        

        lightingShader.use();

        //drawing ring
        drawRing(lightingShader, 9.6, -2.64, 0.55, 0);

        //drawHuman(lightingShader, glm::translate(identityMatrix, glm::vec3(x_position, 0.0f, z_position))* glm::rotate(identityMatrix, glm::radians(horizontal_angle), glm::vec3(0.0f, 1.0f, 0.0f))); //c

        //drawRing2(lightingShader, 9.6, -1.64, 0.55, 0);
       


        // be sure to activate shader when setting uniforms/drawing objects
        lightingShaderWithTexture.use();
        lightingShaderWithTexture.setVec3("viewPos", camera.Position);

        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        // camera/view transformation
       // view = camera.GetViewMatrix();
        view = basic_camera.createViewMatrix();



        // point light 1
        pointlight1.setUpPointLight(lightingShader);
        // point light 2
        pointlight2.setUpPointLight(lightingShader);
        // point light 3
        pointlight3.setUpPointLight(lightingShader);
        // point light 4
        pointlight4.setUpPointLight(lightingShader);


        // pass projection matrix to shader (note that in this case it could change every frame)
        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);

        // camera/view transformation
         //view = camera.GetViewMatrix();
        view = basic_camera.createViewMatrix();








        // pass projection matrix to shader (note that in this case it could change every frame)
        lightingShaderWithTexture.use();
        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
        lightingShaderWithTexture.setMat4("projection", projection);


       // view = basic_camera.createViewMatrix();
        lightingShaderWithTexture.setMat4("view", view);

        //point light 1
        pointlight1.setUpPointLight(lightingShaderWithTexture);
        // // point light 2
        pointlight2.setUpPointLight(lightingShaderWithTexture);
        // point light 3
        pointlight3.setUpPointLight(lightingShaderWithTexture);
        // point light 4
        pointlight4.setUpPointLight(lightingShaderWithTexture);
        // point light 45
      //  pointlight5.setUpPointLight(lightingShaderWithTexture);
        // point light 46
      //  pointlight6.setUpPointLight(lightingShaderWithTexture);
        // Directional light
        dirLight.setUpDirLight(lightingShaderWithTexture);

        // Spot light
        spotLight.setUpSpotLight(lightingShaderWithTexture);

        drawBasketBall(lightingShaderWithTexture);

        drawCourt(cubeCourt, lightingShaderWithTexture);

        drawBackboard(cubeBackboard, lightingShaderWithTexture);

        drawGallery(triangleGallery, cubeGallery, lightingShaderWithTexture);

        ourShader.use();
     ourShader.setMat4("projection", projection);
     ourShader.setMat4("view", view);
//
        
      drawPlayer(lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(x_position, 0.0f, z_position))* glm::rotate(identityMatrix, glm::radians(horizontal_angle), glm::vec3(0.0f, 1.0f, 0.0f)), ourShader, lightingShader); //c

      drawNext(lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(0.5, 0.0f, 0.0))* glm::rotate(identityMatrix, glm::radians(0.0f), glm::vec3(0.0f, 1.0f, 0.0f)), ourShader, lightingShader); //c
      drawNext(lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(0.6, 0.0f, 0.0))* glm::rotate(identityMatrix, glm::radians(horizontal_angle), glm::vec3(0.0f, 1.0f, 0.0f)), ourShader, lightingShader); //c
      drawNext(lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(0.7, 0.0f, 0.0))* glm::rotate(identityMatrix, glm::radians(horizontal_angle), glm::vec3(0.0f, 1.0f, 0.0f)), ourShader, lightingShader); //c
       drawNext(lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(0.8, 0.0f, 0.0))* glm::rotate(identityMatrix, glm::radians(horizontal_angle), glm::vec3(0.0f, 1.0f, 0.0f)), ourShader, lightingShader); //c
      drawNext(lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(0.9, 0.0f, 0.0))* glm::rotate(identityMatrix, glm::radians(horizontal_angle), glm::vec3(0.0f, 1.0f, 0.0f)), ourShader, lightingShader); //c
        
      drawFlag(lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(-0.2f, 0.0f, -0.66f))* glm::rotate(identityMatrix, glm::radians(horizontal_angle), glm::vec3(0.0f, 0.1f, 0.0f)), ourShader);
      drawFlag(lightingShaderWithTexture, glm::translate(identityMatrix, glm::vec3(0.2f, 0.0f, -0.66f))* glm::rotate(identityMatrix, glm::radians(horizontal_angle), glm::vec3(0.0f, 0.1f, 0.0f)), ourShader);

       drawBanner(ourShader, glm::translate(identityMatrix, glm::vec3(-1.4f, 0.0f, -0.6f))* glm::rotate(identityMatrix, glm::radians(horizontal_angle), glm::vec3(0.0f, 0.1f, 0.0f)), cubeBanner, lightingShaderWithTexture);
       //drawFlag(ourShader, glm::translate(identityMatrix, glm::vec3(0.2f, 0.0f, -0.66f))* glm::rotate(identityMatrix, glm::radians(horizontal_angle), glm::vec3(0.0f, 0.1f, 0.0f)), flagGallery2, lightingShaderWithTexture);


        
        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();
        lightingShader.setVec3("viewPos", camera.Position);

        // point light 1
        pointlight1.setUpPointLight(lightingShader);
        // point light 2
        pointlight2.setUpPointLight(lightingShader);
        // point light 3
        pointlight3.setUpPointLight(lightingShader);
        // point light 4
        pointlight4.setUpPointLight(lightingShader);

        // activate shader
        lightingShader.use();

        // pass projection matrix to shader (note that in this case it could change every frame)
        projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
        lightingShader.setMat4("projection", projection);

        // camera/view transformation
        //glm::mat4 view = camera.GetViewMatrix();
        view = basic_camera.createViewMatrix();
        lightingShader.setMat4("view", view);

        translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X, translate_Y, translate_Z));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X, scale_Y, scale_Z));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);

    
        if (ball_animation == false) {
            ball_x_position = x_position;
            ball_y_position = 0.1;
            ball_z_position = z_position;
        }
        if (ball_animation == true)
        {
            //             for x axis
            ball_del_x = ball_velocity * cos(glm::radians(vertical_angle)) * cos(glm::radians(horizontal_angle)) * ball_animation_speed;
            ball_x_position -= ball_del_x;

            //            for y axis
            ball_del_y = (u - 0.5f * g) * ball_animation_speed;
            u -= g * ball_animation_speed;
            ball_y_position += ball_del_y;

            //            for z axis
            ball_del_z = -ball_velocity * cos(glm::radians(vertical_angle)) * sin(glm::radians(horizontal_angle)) * ball_animation_speed;
            ball_z_position += ball_del_z;

            if (ball_y_position <= 0.04)
            {
                ball_animation = false;
                delay(2.0);
            }
        }

        //drawing ball

        glm::mat4 modelForSphere = glm::mat4(1.0f);
        //modelForSphere = glm::translate(model, glm::vec3(1.5f, 1.2f, 0.5f));
        modelForSphere = glm::translate(model, glm::vec3(ball_x_position, ball_y_position, ball_z_position)) * glm::scale(identityMatrix, glm::vec3(0.1f, 0.1f, 0.1f))* glm::translate(identityMatrix, glm::vec3(-1.5, 0.7, 0));
       // modelForSphere = glm::translate(model, glm::vec3(1.5f, 1.2f, 0.5f));
        sphere.drawSphere(lightingShader, modelForSphere);

        float distance = sqrt((ring_x_position - ball_x_position) * (ring_x_position - ball_x_position) + (ring_y_position - ball_y_position) * (ring_y_position - ball_y_position) + (ring_z_position - ball_z_position) * (ring_z_position - ball_z_position));

        cout << "Current required distance " << distance << endl;

        if (x_position <= considerble_distance && ball_del_y < 0 && ball_y_position==0 && distance==0)
        {
            score += 1;
            ball_animation = false;
            //        vertical_angle = 0;
            //        horizontal_angle = 0;
        }



        //cout << "ball position " << ball_x_position << "  " << ball_y_position << "  " << ball_z_position << endl;
       // cout << "ball velocity " << ball_velocity << "m/s" << endl;
       // cout << "horizontal angle " << horizontal_angle << "      " << cos(glm::radians(horizontal_angle)) << endl;
       // cout << "verticle angle " << vertical_angle << "      " << cos(glm::radians(vertical_angle)) << "  " << sin(glm::radians(vertical_angle)) << endl;
       // cout << "del Y " << ball_del_y << endl;
        cout << "Score  " << score << endl;
      

        // also draw the lamp object(s)
        ourShader.use();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);
       // drawFlag(ourShader, glm::translate(identityMatrix, glm::vec3(0.0f, 0.0f, 0.0f)) * glm::rotate(identityMatrix, glm::radians(horizontal_angle), glm::vec3(0.0f, 0.0f, 2.0f)));
      


        // we now draw as many light bulbs as we have point lights.
        glBindVertexArray(lightCubeVAO);

        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------


    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}


/*


int main()
{
    // glfw: initialize and configure
    // ------------------------------
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);

#ifdef __APPLE__
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

    // glfw window creation
    // --------------------
    GLFWwindow* window = glfwCreateWindow(SCR_WIDTH, SCR_HEIGHT, "CSE 4208: Computer Graphics Laboratory", NULL, NULL);
    if (window == NULL)
    {
        std::cout << "Failed to create GLFW window" << std::endl;
        glfwTerminate();
        return -1;
    }
    glfwMakeContextCurrent(window);
    glfwSetFramebufferSizeCallback(window, framebuffer_size_callback);
    glfwSetKeyCallback(window, key_callback);
    glfwSetCursorPosCallback(window, mouse_callback);
    glfwSetScrollCallback(window, scroll_callback);

    // tell GLFW to capture our mouse
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN);

    // glad: load all OpenGL function pointers
    // ---------------------------------------
    if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress))
    {
        std::cout << "Failed to initialize GLAD" << std::endl;
        return -1;
    }

    // configure global opengl state
    // -----------------------------
    glEnable(GL_DEPTH_TEST);

    // build and compile our shader zprogram
    // ------------------------------------
    Shader lightingShader("vertexShaderForPhongShading.vs", "fragmentShaderForPhongShading.fs");
    //Shader lightingShader("vertexShaderForGouraudShading.vs", "fragmentShaderForGouraudShading.fs");
    Shader ourShader("vertexShader.vs", "fragmentShader.fs");

    // set up vertex data (and buffer(s)) and configure vertex attributes
    // ------------------------------------------------------------------

    float cube_vertices[] = {
        // positions      // normals
        0.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        1.0f, 0.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        1.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 0.0f, -1.0f,

        1.0f, 0.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 1.0f, 0.0f, 0.0f,
        1.0f, 1.0f, 1.0f, 1.0f, 0.0f, 0.0f,

        0.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,
        0.0f, 1.0f, 1.0f, 0.0f, 0.0f, 1.0f,

        0.0f, 0.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 1.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, -1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 0.0f, -1.0f, 0.0f, 0.0f,

        1.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,
        1.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 1.0f, 1.0f, 0.0f, 1.0f, 0.0f,

        0.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        1.0f, 0.0f, 0.0f, 0.0f, -1.0f, 0.0f,
        1.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f, -1.0f, 0.0f
    };
    unsigned int cube_indices[] = {
        0, 3, 2,
        2, 1, 0,

        4, 5, 7,
        7, 6, 4,

        8, 9, 10,
        10, 11, 8,

        12, 13, 14,
        14, 15, 12,

        16, 17, 18,
        18, 19, 16,

        20, 21, 22,
        22, 23, 20
    };

    unsigned int cubeVAO, cubeVBO, cubeEBO;
    glGenVertexArrays(1, &cubeVAO);
    glGenBuffers(1, &cubeVBO);
    glGenBuffers(1, &cubeEBO);

    glBindVertexArray(cubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(cube_vertices), cube_vertices, GL_STATIC_DRAW);

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(cube_indices), cube_indices, GL_STATIC_DRAW);


    // position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    // vertex normal attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)12);
    glEnableVertexAttribArray(1);

    // second, configure the light's VAO (VBO stays the same; the vertices are the same for the light object which is also a 3D cube)
    unsigned int lightCubeVAO;
    glGenVertexArrays(1, &lightCubeVAO);
    glBindVertexArray(lightCubeVAO);

    glBindBuffer(GL_ARRAY_BUFFER, cubeVBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, cubeEBO);
    // note that we update the lamp's position attribute's stride to reflect the updated buffer data
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 6 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    Sphere sphere = Sphere();

    //glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);



    //ourShader.use();
    //lightingShader.use();

    // render loop
    // -----------
    while (!glfwWindowShouldClose(window))
    {
        // per-frame time logic
        // --------------------
        float currentFrame = static_cast<float>(glfwGetTime());
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;

        // input
        // -----
        processInput(window);

        // render
        // ------
        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // be sure to activate shader when setting uniforms/drawing objects
        lightingShader.use();
        lightingShader.setVec3("viewPos", camera.Position);

        // point light 1
        pointlight1.setUpPointLight(lightingShader);
        // point light 2
        pointlight2.setUpPointLight(lightingShader);
        // point light 3
        pointlight3.setUpPointLight(lightingShader);
        // point light 4
        pointlight4.setUpPointLight(lightingShader);

        // activate shader
        lightingShader.use();

        // pass projection matrix to shader (note that in this case it could change every frame)
        glm::mat4 projection = glm::perspective(glm::radians(camera.Zoom), (float)SCR_WIDTH / (float)SCR_HEIGHT, 0.1f, 100.0f);
        //glm::mat4 projection = glm::ortho(-2.0f, +2.0f, -1.5f, +1.5f, 0.1f, 100.0f);
        lightingShader.setMat4("projection", projection);

        // camera/view transformation
        //glm::mat4 view = camera.GetViewMatrix();
        glm::mat4 view = basic_camera.createViewMatrix();
        lightingShader.setMat4("view", view);

        // Modelling Transformation
        glm::mat4 identityMatrix = glm::mat4(1.0f); // make sure to initialize matrix to identity matrix first
        glm::mat4 translateMatrix, rotateXMatrix, rotateYMatrix, rotateZMatrix, scaleMatrix, model;
        translateMatrix = glm::translate(identityMatrix, glm::vec3(translate_X, translate_Y, translate_Z));
        rotateXMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_X), glm::vec3(1.0f, 0.0f, 0.0f));
        rotateYMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Y), glm::vec3(0.0f, 1.0f, 0.0f));
        rotateZMatrix = glm::rotate(identityMatrix, glm::radians(rotateAngle_Z), glm::vec3(0.0f, 0.0f, 1.0f));
        scaleMatrix = glm::scale(identityMatrix, glm::vec3(scale_X, scale_Y, scale_Z));
        model = translateMatrix * rotateXMatrix * rotateYMatrix * rotateZMatrix * scaleMatrix;
        lightingShader.setMat4("model", model);

        //glBindVertexArray(cubeVAO);
        //glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
        //glDrawArrays(GL_TRIANGLES, 0, 36);

       // bed(cubeVAO, lightingShader, model);

        glm::mat4 modelForSphere = glm::mat4(1.0f);
        modelForSphere = glm::translate(model, glm::vec3(1.5f, 1.2f, 0.5f));
        sphere.drawSphere(lightingShader, modelForSphere);

        // also draw the lamp object(s)
        ourShader.use();
        ourShader.setMat4("projection", projection);
        ourShader.setMat4("view", view);

        // we now draw as many light bulbs as we have point lights.
        glBindVertexArray(lightCubeVAO);
        //for (unsigned int i = 0; i < 4; i++)
        //{
          //  model = glm::mat4(1.0f);
          //  model = glm::translate(model, pointLightPositions[i]);
          //  model = glm::scale(model, glm::vec3(0.2f)); // Make it a smaller cube
          //  ourShader.setMat4("model", model);
          //  ourShader.setVec3("color", glm::vec3(0.8f, 0.8f, 0.8f));
          //  glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
            //glDrawArrays(GL_TRIANGLES, 0, 36);
       // }

        // glfw: swap buffers and poll IO events (keys pressed/released, mouse moved etc.)
        // -------------------------------------------------------------------------------
        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    // optional: de-allocate all resources once they've outlived their purpose:
    // ------------------------------------------------------------------------
    glDeleteVertexArrays(1, &cubeVAO);
    glDeleteVertexArrays(1, &lightCubeVAO);
    glDeleteBuffers(1, &cubeVBO);
    glDeleteBuffers(1, &cubeEBO);

    // glfw: terminate, clearing all previously allocated GLFW resources.
    // ------------------------------------------------------------------
    glfwTerminate();
    return 0;
}

*/



// process all input: query GLFW whether relevant keys are pressed/released this frame and react accordingly
// ---------------------------------------------------------------------------------------------------------
void processInput(GLFWwindow* window)
{
    if (glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
        glfwSetWindowShouldClose(window, true);


    // Human movement change control
    if (glfwGetKey(window, GLFW_KEY_X) == GLFW_PRESS) {
        //camera.ProcessKeyboard(FORWARD, deltaTime);
        x_position += human_speed;
    }
    if (glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) {
       // camera.ProcessKeyboard(BACKWARD, deltaTime);
        x_position -= human_speed;
    }
    if (glfwGetKey(window, GLFW_KEY_Z) == GLFW_PRESS) {
       // camera.ProcessKeyboard(LEFT, deltaTime);
        z_position += human_speed;
    }
    if (glfwGetKey(window, GLFW_KEY_C) == GLFW_PRESS) {
      //  camera.ProcessKeyboard(RIGHT, deltaTime);
        z_position -= human_speed;
    }

    // vertical angle change control
    if (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS)
    {
        horizontal_angle += 0.1;
    }
    if (glfwGetKey(window, GLFW_KEY_B) == GLFW_PRESS)
    {
        horizontal_angle -= 0.1;
    }

    // ball velocity change control
    if (glfwGetKey(window, GLFW_KEY_UP) == GLFW_PRESS)
    {
        ball_velocity += 0.001;
    }
    if (glfwGetKey(window, GLFW_KEY_DOWN) == GLFW_PRESS)
    {
        ball_velocity -= 0.001;
    }



    // ball velocity change control
    if (glfwGetKey(window, GLFW_KEY_M) == GLFW_PRESS)
    {
        ball_velocity += 0.001;
    }
    if (glfwGetKey(window, GLFW_KEY_N) == GLFW_PRESS)
    {
        ball_velocity -= 0.001;
    }

    // fire ball
    if (glfwGetKey(window, GLFW_KEY_ENTER) == GLFW_PRESS)
    {
        delay(0.5f);
        ball_animation = !ball_animation;

        ball_x_position = x_position;
        ball_y_position = 0.1f;
        ball_z_position = z_position;

        u = ball_velocity * sin(glm::radians(vertical_angle));
    }





        // Eye change control
    if (glfwGetKey(window, GLFW_KEY_H) == GLFW_PRESS)
    {
        eyeX += 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    {
        eyeX -= 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_T) == GLFW_PRESS)
    {
        eyeZ += 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_G) == GLFW_PRESS)
    {
        eyeZ -= 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_R) == GLFW_PRESS)
    {
        eyeY += 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }
    if (glfwGetKey(window, GLFW_KEY_Y) == GLFW_PRESS)
    {
        eyeY -= 2.5 * deltaTime;
        basic_camera.changeEye(eyeX, eyeY, eyeZ);
    }


    // look at point change control
    if (glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
    {
        lookAtX += 2 * deltaTime;
        basic_camera.changeLookAt(lookAtX, lookAtY, lookAtZ);
    }
    if (glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
    {
        lookAtX -= 2 * deltaTime;
        basic_camera.changeLookAt(lookAtX, lookAtY, lookAtZ);
    }
    if (glfwGetKey(window, GLFW_KEY_K) == GLFW_PRESS)
    {
        lookAtZ += 2 * deltaTime;
        basic_camera.changeLookAt(lookAtX, lookAtY, lookAtZ);
    }
    if (glfwGetKey(window, GLFW_KEY_I) == GLFW_PRESS)
    {
        lookAtZ -= 2 * deltaTime;
        basic_camera.changeLookAt(lookAtX, lookAtY, lookAtZ);
    }
    if (glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
    {
        lookAtY += 2 * deltaTime;
        basic_camera.changeLookAt(lookAtX, lookAtY, lookAtZ);
    }
    if (glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
    {
        lookAtY -= 2 * deltaTime;
        basic_camera.changeLookAt(lookAtX, lookAtY, lookAtZ);
    }

}

void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_1 && action == GLFW_PRESS)
    {
        if (pointLightOn)
        {
            pointlight1.turnOff();
            pointlight2.turnOff();
            pointlight3.turnOff();
            pointlight4.turnOff();
            pointLightOn = !pointLightOn;
        }
        else
        {
            pointlight1.turnOn();
            pointlight2.turnOn();
            pointlight3.turnOn();
            pointlight4.turnOn();
            pointLightOn = !pointLightOn;
        }
    }
    if (key == GLFW_KEY_2 && action == GLFW_PRESS)
    {
        if (dirLightOn)
        {
            dirLight.turnOff();
            dirLightOn = !dirLightOn;
        }
        else
        {
            dirLight.turnOn();
            dirLightOn = !dirLightOn;
        }
    }
    if (key == GLFW_KEY_3 && action == GLFW_PRESS)
    {
        if (spotLightOn)
        {
            spotLight.turnOff();
            spotLightOn = !spotLightOn;
        }
        else
        {
            spotLight.turnOn();
            spotLightOn = !spotLightOn;
        }
    }

    if (key == GLFW_KEY_4 && action == GLFW_PRESS)
    {
        pointlight1.turnOn();
        pointlight2.turnOn();
        pointlight3.turnOn();
        pointlight4.turnOn();
        dirLight.turnOn();
        spotLight.turnOn();
        pointLightOn = true;
        dirLightOn = true;
        spotLightOn = true;
    }


    if (key == GLFW_KEY_5 && action == GLFW_PRESS)
    {
        pointlight1.turnOff();
        pointlight2.turnOff();
        pointlight3.turnOff();
        pointlight4.turnOff();
        dirLight.turnOff();
        spotLight.turnOff();
        pointLightOn = false;
        dirLightOn = false;
        spotLightOn = false;
    }


    if (key == GLFW_KEY_6 && action == GLFW_PRESS)
    {
        pointlight1.turnAmbientOn();
        pointlight2.turnAmbientOn();
        pointlight3.turnAmbientOn();
        pointlight4.turnAmbientOn();
        dirLight.turnAmbientOn();
        spotLight.turnAmbientOn();
        pointLightOn = true;
        dirLightOn = true;
        spotLightOn = true;
    }


    if (key == GLFW_KEY_7 && action == GLFW_PRESS)
    {
        pointlight1.turnDiffuseOn();
        pointlight2.turnDiffuseOn();
        pointlight3.turnDiffuseOn();
        pointlight4.turnDiffuseOn();
        dirLight.turnDiffuseOn();
        spotLight.turnDiffuseOn();
        pointLightOn = true;
        dirLightOn = true;
        spotLightOn = true;
    }

    if (key == GLFW_KEY_7 && action == GLFW_PRESS)
    {
        pointlight1.turnSpecularOn();
        pointlight2.turnSpecularOn();
        pointlight3.turnSpecularOn();
        pointlight4.turnSpecularOn();
        dirLight.turnSpecularOn();
        spotLight.turnSpecularOn();
        pointLightOn = true;
        dirLightOn = true;
        spotLightOn = true;
    }



    //if (key == GLFW_KEY_7 && action == GLFW_PRESS)
    {
        //saveControlPoints();
    }
   // if (key == GLFW_KEY_8 && action == GLFW_PRESS)
    {
       // showControlPoints = !showControlPoints;
    }
  //  if (key == GLFW_KEY_9 && action == GLFW_PRESS)
    {
        //loadBezierCurvePoints = !loadBezierCurvePoints;
    }
  //  if (key == GLFW_KEY_0 && action == GLFW_PRESS)
    {
      //  lineMode = !lineMode;
    }

}

// glfw: whenever the window size changed (by OS or user resize) this callback function executes
// ---------------------------------------------------------------------------------------------
void framebuffer_size_callback(GLFWwindow* window, int width, int height)
{
    // make sure the viewport matches the new window dimensions; note that width and
    // height will be significantly larger than specified on retina displays.
    glViewport(0, 0, width, height);
}


// glfw: whenever the mouse moves, this callback is called
// -------------------------------------------------------
void mouse_callback(GLFWwindow* window, double xposIn, double yposIn)
{
    float xpos = static_cast<float>(xposIn);
    float ypos = static_cast<float>(yposIn);

    if (firstMouse)
    {
        lastX = xpos;
        lastY = ypos;
        firstMouse = false;
    }

    float xoffset = xpos - lastX;
    float yoffset = lastY - ypos; // reversed since y-coordinates go from bottom to top

    lastX = xpos;
    lastY = ypos;

    camera.ProcessMouseMovement(xoffset, yoffset);
}

// glfw: whenever the mouse scroll wheel scrolls, this callback is called
// ----------------------------------------------------------------------
void scroll_callback(GLFWwindow* window, double xoffset, double yoffset)
{
    camera.ProcessMouseScroll(static_cast<float>(yoffset));
}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
    double x, y;
    //getting cursor position
    glfwGetCursorPos(window, &x, &y);
    if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
    {
        if (mouseButtonFlag != 1)
        {
            mouseButtonFlag = 1;
            clkpt[0].x = x;
            clkpt[0].y = y;
        }
        scsToWcs(clkpt[0].x, clkpt[0].y, wcsClkDn);
        cout << "\nD: " << x << " " << y << " wcs: " << wcsClkDn[0] << " " << wcsClkDn[1] << " " << wcsClkDn[2];
    }
    else if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_RELEASE)
    {
        if (mouseButtonFlag == 1)
        {
            clkpt[1].x = x;
            clkpt[1].y = y;
            mouseButtonFlag = 0;
        }
        scsToWcs(clkpt[1].x, clkpt[1].y, wcsClkUp);
        cout << "\nU: " << x << " " << y << " wcs: " << wcsClkUp[0] << " " << wcsClkUp[1] << " " << wcsClkUp[2];
        cntrlPoints.push_back(wcsClkUp[0]);
        cntrlPoints.push_back(wcsClkUp[1]);
        cntrlPoints.push_back(wcsClkUp[2]);
    }
}



unsigned int loadTexture(char const* path, GLenum textureWrappingModeS, GLenum textureWrappingModeT, GLenum textureFilteringModeMin, GLenum textureFilteringModeMax)
{
    unsigned int textureID;
    glGenTextures(1, &textureID);

    int width, height, nrComponents;
    stbi_set_flip_vertically_on_load(true);
    unsigned char* data = stbi_load(path, &width, &height, &nrComponents, 0);
    if (data)
    {
        GLenum format;
        if (nrComponents == 1)
            format = GL_RED;
        else if (nrComponents == 3)
            format = GL_RGB;
        else
            format = GL_RGBA;

        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexImage2D(GL_TEXTURE_2D, 0, format, width, height, 0, format, GL_UNSIGNED_BYTE, data);
        glGenerateMipmap(GL_TEXTURE_2D);

        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, textureWrappingModeS);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, textureWrappingModeT);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, textureFilteringModeMin);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, textureFilteringModeMax);

        stbi_image_free(data);
    }
    else
    {
        std::cout << "Texture failed to load at path: " << path << std::endl;
        stbi_image_free(data);
    }

    return textureID;
}

// This function converts the screen coordinates to world coordinates
// ---------------------------------------------------------------------------------------------
void scsToWcs(float sx, float sy, float wcsv[3])
{

    glm::vec4 winCoordinateVector; //vector to hold screen x,y,z coordinates
    glm::vec4 worldCoordinateVector; //vector to hold world x,y,z coordinates

    glGetIntegerv(GL_VIEWPORT, viewport); //get the viewport info

    // setup screen coordinates values
    winCoordinateVector.x = sx;
    winCoordinateVector.y = (float)viewport[3] - (float)sy; // reversed since y-coordinates go from bottom to top
    winCoordinateVector.z = 0.0f;
    winCoordinateVector.w = 1.0f;

    // prepare viewport matrix
    float v_l = viewport[0], v_b = viewport[1], v_r = viewport[2], v_t = viewport[3];
    glm::mat4 viewportMat = glm::mat4(1.0f);
    viewportMat[0].x = (v_r - v_l) / 2.0f;  viewportMat[1].x = 0.0f; viewportMat[2].x = 0.0f;  viewportMat[3].x = (v_r + v_l) / 2.0f;
    viewportMat[0].y = 0.0f; viewportMat[1].y = (v_t - v_b) / 2.0f;  viewportMat[2].y = 0.0f;  viewportMat[3].y = (v_t + v_b) / 2.0f;
    viewportMat[0].z = 0.0f; viewportMat[1].z = 0.0f; viewportMat[2].z = 0.5f;  viewportMat[3].z = 0.5f;
    viewportMat[0].w = 0.0f; viewportMat[1].w = 0.0f; viewportMat[2].w = 0.0f;  viewportMat[3].w = 1.0f;

    //get the world coordinates from the screen coordinates
    worldCoordinateVector = glm::inverse(viewportMat * projection * view * model) * winCoordinateVector;
    wcsv[0] = worldCoordinateVector.x / worldCoordinateVector.w;
    wcsv[1] = worldCoordinateVector.y / worldCoordinateVector.w;
    wcsv[2] = worldCoordinateVector.z / worldCoordinateVector.w;
}

unsigned int drawControlPoints()
{
    unsigned int controlPointVAO;
    unsigned int controlPointVBO;

    glGenVertexArrays(1, &controlPointVAO);
    glGenBuffers(1, &controlPointVBO);

    glBindVertexArray(controlPointVAO);

    glBindBuffer(GL_ARRAY_BUFFER, controlPointVBO);
    glBufferData(GL_ARRAY_BUFFER, (unsigned int)cntrlPoints.size() * sizeof(float), cntrlPoints.data(), GL_STATIC_DRAW);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
    glEnableVertexAttribArray(0);

    return controlPointVAO;
}

void saveControlPoints()
{
    if (cntrlPoints.size())
    {
        printf("save3d");
        fp = fopen("points you can copy.txt", "w");
        if (fp == NULL)
        {
            cout << "File Opening Failed" << endl;
        }
        else
        {
            fprintf(fp, "{\n");
            size_t count = cntrlPoints.size();
            for (int i = 0; i < count / 3; i++)
            {
                int j = i * 3;
                fprintf(fp, "%0.4f, %0.4f, %0.4f,\n", cntrlPoints[j], cntrlPoints[j + 1], cntrlPoints[j + 2]);
            }
            fprintf(fp, "}");
            fclose(fp);
        }
    }
}

long long nCr(int n, int r)
{
    if (r > n / 2)
        r = n - r; // because C(n, r) == C(n, n - r)
    long long ans = 1;
    int i;

    for (i = 1; i <= r; i++)
    {
        ans *= n - r + i;
        ans /= i;
    }

    return ans;
}

//polynomial interpretation for N points
void BezierCurve(double t, float xy[2], GLfloat ctrlpoints[], int L)
{
    double y = 0;
    double x = 0;
    t = t > 1.0 ? 1.0 : t;
    for (int i = 0; i < L + 1; i++)
    {
        long long ncr = nCr(L, i);
        double oneMinusTpow = pow(1 - t, double(L - i));
        double tPow = pow(t, double(i));
        double coef = oneMinusTpow * tPow * ncr;
        x += coef * ctrlpoints[i * 3];
        y += coef * ctrlpoints[(i * 3) + 1];

    }
    xy[0] = float(x);
    xy[1] = float(y);
}

unsigned int hollowBezier(GLfloat ctrlpoints[], int L, float tx, float ty)
{
    int i, j;
    float x, y, z, r;                //current coordinates
    float theta;
    float nx, ny, nz, lengthInv;    // vertex normal


    const float dtheta = 2 * pi / ntheta;        //angular step size

    float t = 0;
    float dt = 1.0 / nt;
    float xy[2];

    for (i = 0; i <= nt; ++i)              //step through y
    {
        BezierCurve(t, xy, ctrlpoints, L);
        r = xy[0];
        y = xy[1];
        theta = 0;
        t += dt;
        lengthInv = 1.0 / r;

        for (j = 0; j <= ntheta; ++j)
        {
            double cosa = cos(theta);
            double sina = sin(theta);
            z = r * cosa;
            x = r * sina;

            coordinates.push_back(x);
            coordinates.push_back(y);
            coordinates.push_back(z);

            // normalized vertex normal (nx, ny, nz)
            // center point of the circle (0,y,0)
            nx = (x - 0) * lengthInv;
            ny = (y - y) * lengthInv;
            nz = (z - 0) * lengthInv;

            normals.push_back(nx);
            normals.push_back(ny);
            normals.push_back(nz);

            theta += dtheta;
        }
    }

    // generate index list of triangles
    // k1--k1+1
    // |  / |
    // | /  |
    // k2--k2+1

    int k1, k2;
    for (int i = 0; i < nt; ++i)
    {
        k1 = i * (ntheta + 1);     // beginning of current stack
        k2 = k1 + ntheta + 1;      // beginning of next stack

        for (int j = 0; j < ntheta; ++j, ++k1, ++k2)
        {
            // k1 => k2 => k1+1
            indices.push_back(k1);
            indices.push_back(k2);
            indices.push_back(k1 + 1);

            // k1+1 => k2 => k2+1
            indices.push_back(k1 + 1);
            indices.push_back(k2);
            indices.push_back(k2 + 1);
        }
    }

    size_t count = coordinates.size();
    for (int i = 0; i < count; i += 3)
    {
        vertices.push_back(coordinates[i] + tx);//ankon
        vertices.push_back(coordinates[i + 1] + ty);//ankon
        vertices.push_back(coordinates[i + 2]);//ankon

        vertices.push_back(normals[i]);
        vertices.push_back(normals[i + 1]);
        vertices.push_back(normals[i + 2]);
    }

    unsigned int bezierVAO;
    glGenVertexArrays(1, &bezierVAO);
    glBindVertexArray(bezierVAO);

    // create VBO to copy vertex data to VBO
    unsigned int bezierVBO;
    glGenBuffers(1, &bezierVBO);
    glBindBuffer(GL_ARRAY_BUFFER, bezierVBO);           // for vertex data
    glBufferData(GL_ARRAY_BUFFER,                   // target
        (unsigned int)vertices.size() * sizeof(float), // data size, # of bytes
        vertices.data(),   // ptr to vertex data
        GL_STATIC_DRAW);                   // usage

    // create EBO to copy index data
    unsigned int bezierEBO;
    glGenBuffers(1, &bezierEBO);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, bezierEBO);   // for index data
    glBufferData(GL_ELEMENT_ARRAY_BUFFER,           // target
        (unsigned int)indices.size() * sizeof(unsigned int),             // data size, # of bytes
        indices.data(),               // ptr to index data
        GL_STATIC_DRAW);                   // usage

    // activate attrib arrays
    glEnableVertexAttribArray(0);
    glEnableVertexAttribArray(1);

    // set attrib arrays with stride and offset
    int stride = 24;     // should be 24 bytes
    glVertexAttribPointer(0, 3, GL_FLOAT, false, stride, (void*)0);
    glVertexAttribPointer(1, 3, GL_FLOAT, false, stride, (void*)(sizeof(float) * 3));

    // unbind VAO, VBO and EBO
    glBindVertexArray(0);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);

    return bezierVAO;
}

void drawCube(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 model = glm::mat4(1.0f), float r = 1.0f, float g = 1.0f, float b = 1.0f)
{
    lightingShader.use();

    lightingShader.setVec3("material.ambient", glm::vec3(r, g, b));
    lightingShader.setVec3("material.diffuse", glm::vec3(r, g, b));
    lightingShader.setVec3("material.specular", glm::vec3(0.5f, 0.5f, 0.5f));
    lightingShader.setFloat("material.shininess", 32.0f);

    lightingShader.setMat4("model", model);

    glBindVertexArray(cubeVAO);
    glDrawElements(GL_TRIANGLES, 36, GL_UNSIGNED_INT, 0);
}

void bed(unsigned int& cubeVAO, Shader& lightingShader, glm::mat4 alTogether)
{
    float baseHeight = 0.3;
    float width = 1;
    float length = 2;
    float pillowWidth = 0.3;
    float pillowLength = 0.15;
    float blanketWidth = 0.8;
    float blanketLength = 0.7;
    float headHeight = 0.6;

    //base
    glm::mat4 model = glm::mat4(1.0f);
    glm::mat4 translate = glm::mat4(1.0f);
    glm::mat4 translate2 = glm::mat4(1.0f);
    glm::mat4 scale = glm::mat4(1.0f);
    scale = glm::scale(model, glm::vec3(width, baseHeight, length));
    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
    model = alTogether * scale * translate;
    drawCube(cubeVAO, lightingShader, model, 0.545, 0.271, 0.075);

    //foam
    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    translate2 = glm::translate(model, glm::vec3(0, baseHeight, 0));
    scale = glm::scale(model, glm::vec3(width, 0.06, length));
    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
    model = alTogether * translate2 * scale * translate;
    drawCube(cubeVAO, lightingShader, model, 0.804, 0.361, 0.361);

    //pillow 1
    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    translate2 = glm::translate(model, glm::vec3((width / 2) - (0.1 + pillowWidth / 2), baseHeight + 1 * 0.06, (length / 2) - (0.025 + pillowWidth / 2)));
    scale = glm::scale(model, glm::vec3(pillowWidth, 0.04, pillowLength));
    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
    model = alTogether * translate2 * scale * translate;
    drawCube(cubeVAO, lightingShader, model, 1, 0.647, 0);

    //pillow 2
    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    translate2 = glm::translate(model, glm::vec3((-width / 2) + (0.1 + pillowWidth / 2), baseHeight + 1 * 0.06, (length / 2) - (0.025 + pillowWidth / 2)));
    scale = glm::scale(model, glm::vec3(pillowWidth, 0.04, pillowLength));
    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
    model = alTogether * translate2 * scale * translate;
    drawCube(cubeVAO, lightingShader, model, 1, 0.647, 0);

    //blanket
    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    translate2 = glm::translate(model, glm::vec3(0, baseHeight + 1 * 0.06, -(length / 2 - 0.025) + blanketLength / 2));
    scale = glm::scale(model, glm::vec3(blanketWidth, 0.015, blanketLength));
    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
    model = alTogether * translate2 * scale * translate;
    drawCube(cubeVAO, lightingShader, model, 0.541, 0.169, 0.886);

    //head
    model = glm::mat4(1.0f);
    translate = glm::mat4(1.0f);
    translate2 = glm::mat4(1.0f);
    scale = glm::mat4(1.0f);
    translate2 = glm::translate(model, glm::vec3(0, 0, (length / 2 - 0.02 / 2) + 0.02));
    scale = glm::scale(model, glm::vec3(width, headHeight, 0.02));
    translate = glm::translate(model, glm::vec3(-0.5, 0, -0.5));
    model = alTogether * translate2 * scale * translate;
    drawCube(cubeVAO, lightingShader, model, 0.545, 0.271, 0.075);

}





