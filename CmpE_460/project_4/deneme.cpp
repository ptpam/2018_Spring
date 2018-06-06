// Includes
#include <vector>
#include <stdio.h>
#include <stdarg.h>
#include <math.h>
#include <array>
#include "TeapotControlPoints.h"
#include "Shader.h"
#include "GeometricDefinitions.h"
#include "Object3D.h"
#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/type_ptr.hpp>
#define GL_GLEXT_PROTOTYPES
#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif


Object3D::Object3D()
{
    vlist = nullptr;
    tlist = nullptr;
}


Object3D::~Object3D()
{
    if (vlist != nullptr)
    {
        delete[] vlist;
        vlist = nullptr;
    }
    if (tlist != nullptr)
    {
        delete[] tlist;
        tlist = nullptr;
    }
}

GLFWwindow* window;
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mode)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

int factorial(int n)
{
    if(n < 0 ) n = 0;
    return (n == 1 || n == 0) ? 1 : factorial(n - 1) * n;
}
float binomial_coefficient(int i, int n) {
    if(n < 0 ) n = 0;
    if(i < 0 ) i = 0;
    return 1.0f * factorial(n) / (factorial(i) * factorial(n-i));
}
float bernstein_polynomial(int i, int n, float u) {
    return binomial_coefficient(i, n) * powf(u, i) * powf(1-u, n-i);
}
void build_control_points_k(int p, struct position3D control_points_k[][ORDER+1]) {
    for (int i = 0; i <= ORDER; i++) {
        for (int j = 0; j <= ORDER; j++) {
            control_points_k[i][j] = teapot_cp_vertices[teapot_patches[p][i][j] - 1];

        }
    }
}

Vertex compute_position(struct position3D control_points_k[][ORDER+1], float u, float v) {
    Vertex result = *new Vertex();
    for (int i = 0; i <= ORDER; i++) {
        float poly_i = bernstein_polynomial(i, ORDER, u);
        for (int j = 0; j <= ORDER; j++) {
            float poly_j = bernstein_polynomial(j, ORDER, v);
            result.x += poly_i * poly_j * control_points_k[i][j].x;
            result.y += poly_i * poly_j * control_points_k[i][j].y;
            result.z += poly_i * poly_j * control_points_k[i][j].z;
            result.r = 0; //default colour 
            result.g = 0;
            result.b = 0;
        }
    }
    return result;
}
#define RESU 15 //resolution in u axis
#define RESV 15 //resolution in v axis

void Object3D::build_teapot() {
    vlist = new Vertex[TEAPOT_NB_PATCHES * RESU*RESV]; //vertex list
    tlist = new Triangle[TEAPOT_NB_PATCHES * (RESU-1)*(RESV-1) * 2]; //triangle list

    //calculate vertices
    for (int p = 0; p < TEAPOT_NB_PATCHES; p++) {
        struct position3D control_points_k[ORDER+1][ORDER+1];
        build_control_points_k(p, control_points_k);
        for (int ru = 0; ru <= RESU-1; ru++) {
            float u = 1.0 * ru / (RESU-1);
            for (int rv = 0; rv <= RESV-1; rv++) {
                float v = 1.0 * rv / (RESV-1);
                vlist[p*RESU*RESV + ru*RESV + rv] = compute_position(control_points_k, u, v);
                vlist[p*RESU*RESV + ru*RESV + rv].r = 0.6;
                vlist[p*RESU*RESV + ru*RESV + rv].g = 1.0 * p / TEAPOT_NB_PATCHES;
                vlist[p*RESU*RESV + ru*RESV + rv].b = 1.0 * p / TEAPOT_NB_PATCHES;
            }
        }
    }
    //calculate triangle vertex orders or namely triangles
    int n = 0;
    Triangle tmpTrg = *new Triangle();
    tmpTrg.nverts = 3;
    for (int p = 0; p < TEAPOT_NB_PATCHES; p++) {
        for (int ru = 0; ru < RESU-1; ru++)
            for (int rv = 0; rv < RESV-1; rv++) {
                // ABCD is a square
                // triangle in the order ABC is the first one
                tmpTrg.verts = new int[tmpTrg.nverts];
                tmpTrg.verts[0] = p*RESU*RESV +  ru   *RESV +  rv   ;
                tmpTrg.verts[1] = p*RESU*RESV +  ru   *RESV + (rv+1);
                tmpTrg.verts[2] = p*RESU*RESV + (ru+1)*RESV + (rv+1);
                tlist[n] = tmpTrg;
                n++;
                // triangle in the order CDA is the second one
                tmpTrg.verts = new int[tmpTrg.nverts];
                tmpTrg.verts[0] = p*RESU*RESV + (ru+1)*RESV + (rv+1);
                tmpTrg.verts[1] = p*RESU*RESV + (ru+1)*RESV +  rv   ;
                tmpTrg.verts[2] = p*RESU*RESV +  ru   *RESV +  rv   ;

                tlist[n] = tmpTrg;
                n++;
            }
        }
}

void init(int w, int h)
{
    glfwInit();
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // macOSX requirement :
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
    glfwWindowHint(GLFW_RESIZABLE, GL_FALSE);
    glfwWindowHint(GLFW_SAMPLES, 4);

    window = glfwCreateWindow(w, h, "OpenGLTeapot", nullptr, nullptr); // Windowed
    glfwMakeContextCurrent(window);
    //640, 480, 8, 8, 8, 0, 24, 0, GLFW_FULLSCREEN
    glfwSetKeyCallback(window, key_callback);

    // Initialize GLEW to setup the OpenGL Function pointers
    glewExperimental = GL_TRUE;
    glewInit();

    // Define the viewport dimensions
    glViewport(0, 0, w, h);

    // Enable depth test
    glEnable(GL_DEPTH_TEST);
}

void Object3D::CreateObject()
{

    int attributeCount = 6;
    vertexCount        = TEAPOT_NB_PATCHES * RESU*RESV;
    triangleCount      = TEAPOT_NB_PATCHES * (RESU-1)*(RESV-1) * 2;
    build_teapot();

    //Bind the vertex and index buffers
    glGenVertexArrays(1, &VAO);
    glGenBuffers(1, &VBO);
    glGenBuffers(1, &EBO);

    // Bind our Vertex Array Object first, then bind and set our buffers and pointers.
    glBindVertexArray(VAO);
    //Convert our vertex list into a continuous array, copy the vertices into the vertex buffer.

    float* vertexData = new float[vertexCount * attributeCount];
    for (int i = 0; i < vertexCount; i++){
        float temp_vertex [6];
        temp_vertex[0] = vlist[i].x;
        temp_vertex[1] = vlist[i].y;
        temp_vertex[2] = vlist[i].z;
        temp_vertex[3] = vlist[i].r;
        temp_vertex[4] = vlist[i].g;
        temp_vertex[5] = vlist[i].b;

        memcpy(&vertexData[i*attributeCount], 
            temp_vertex, sizeof(float)*attributeCount);
    }

    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(float)*attributeCount*vertexCount, vertexData, GL_STATIC_DRAW);
    //Copy the index data found in the list of triangles into the element array buffer (index array)
    //We are using a triangles, so we need triangleCount * 3 indices.
    int* indexData = new int[triangleCount * 3];
    for (int i = 0; i < triangleCount; i++){
        memcpy(&indexData[i * 3], tlist[i].verts, sizeof(int) * 3);
    }

    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(int)*3*triangleCount, indexData, GL_STATIC_DRAW);
    // Position attribute
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, attributeCount * sizeof(GLfloat), (GLvoid*)0);
    glEnableVertexAttribArray(0);
    // Color attribute
    glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, attributeCount * sizeof(GLfloat), (GLvoid*)(3 * sizeof(GLfloat)));
    glEnableVertexAttribArray(1);
    // Unbind VAO
    glBindVertexArray(0);

    // Delete temporary buffers
    delete[] vertexData;
    delete[] indexData;
}

int main()
{
    int screenWidth = 800;
    int screenHeight = 600;
    init(screenWidth, screenHeight);
    std::vector<Object3D*> listOfObjects;

    Object3D* pObj = new Object3D();


    pObj->CreateObject();
    listOfObjects.push_back(pObj);

    //Create the shaders. 

    Shader shader("VertexShader.vs", "FragmentShader.fs");


    while (!glfwWindowShouldClose(window))
    {
        glfwPollEvents();
        // Clear the colorbuffer
        glClearColor(0.0f, 0.4f, 0.7f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

        // Use the shader
        shader.Use();
        // Transformations
        // Create camera transformation
        glm::mat4 view;
        glm::vec3 cameraPos = glm::vec3(0.0f, 150.0f, 100.0f);
        glm::vec3 cameraTarget = glm::vec3(0.0, 80.0f, 20.0f);
        glm::vec3 cameraUp = glm::vec3(-0.2f, 1.0f, 0.0f);
        view = glm::lookAt(cameraPos, cameraTarget, cameraUp);
        // Create projection transformation
        glm::mat4 projection;
        projection = glm::perspective<float>(90.0, (float)screenWidth / (float)screenHeight, 0.1f, 1000.0f);
        // Get the uniform locations
        GLint modelLoc = glGetUniformLocation(shader.Program, "model");
        GLint viewLoc = glGetUniformLocation(shader.Program, "view");
        GLint projLoc = glGetUniformLocation(shader.Program, "projection");
        // Pass the view and projection matrices to the shaders
        glUniformMatrix4fv(viewLoc, 1, GL_FALSE, glm::value_ptr(view));
        glUniformMatrix4fv(projLoc, 1, GL_FALSE, glm::value_ptr(projection));
        // Put the bottom of the object on XZ plane and scale it up
        pObj->modelMatrix = glm::scale(glm::mat4(1.0f), glm::vec3(30.0f));
        pObj->modelMatrix = glm::rotate(pObj->modelMatrix, -90.0f, glm::vec3(1.0f, 0.0f,0.0f));
        int i = 0;
        for (auto pObj : listOfObjects)
        {
            glBindVertexArray(pObj->VAO);

            glUniformMatrix4fv(modelLoc, 1, GL_FALSE, glm::value_ptr(pObj->modelMatrix));
            glDrawElements(GL_TRIANGLES, (listOfObjects[i]->vertexCount) * 6, GL_UNSIGNED_INT, (void*)(sizeof(Vertex)));
            glBindVertexArray(0);
            i++;
        }
        // Swap the buffers

        glfwSwapBuffers(window);

    }
    for (auto pObj : listOfObjects)
    {
        glDeleteVertexArrays(1, &pObj->VAO);
        glDeleteBuffers(1, &pObj->VBO);
        glDeleteBuffers(1, &pObj->EBO);
        delete pObj;
    }
    glfwTerminate();
    return 0;
}