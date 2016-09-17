//
//  hw2_main.cpp
//  Homework 2 Assignment
//
//  Adam N. Morris
//  Originally created by Rafael Radkowski on 5/28/15.
//  Copyright (c) 2015 -. All rights reserved.
//

// stl include
#include <iostream>
#include <string>

// GLEW include
#include <GL/glew.h>

// GLM include files
#define GLM_FORCE_INLINE

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

// glfw includes
#include <GLFW/glfw3.h>

// include local files
#include "controls.h"
#include "HCI557Common.h"
#include "CoordSystem.h"

// this line tells the compiler to use the namespace std.
// Each object, command without a namespace is assumed to be part of std. 
using namespace std;

static const string vs_string =
        "#version 410 core                                                 \n"
                "                                                                   \n"
                "uniform mat4 projectionMatrix;                                    \n"
                "uniform mat4 viewMatrix;                                           \n"
                "uniform mat4 modelMatrix;                                          \n"
                "in vec3 in_Position;                                               \n"
                "                                                                   \n"
                "in vec3 in_Color;                                                  \n"
                "out vec3 pass_Color;                                               \n"
                "                                                                  \n"
                "void main(void)                                                   \n"
                "{                                                                 \n"
                "    gl_Position = projectionMatrix * viewMatrix * modelMatrix * vec4(in_Position, 1.0);  \n"
                "    pass_Color = in_Color;                                         \n"
                "}                                                                 \n";

// Fragment shader source code. This determines the colors in the fragment generated in the shader pipeline. In this case, it colors the inside of our triangle specified by our vertex shader.
static const string fs_string =
        "#version 410 core                                                 \n"
                "                                                                  \n"
                "in vec3 pass_Color;                                                 \n"
                "out vec4 color;                                                    \n"
                "void main(void)                                                   \n"
                "{                                                                 \n"
                "    color = vec4(pass_Color, 1.0);                               \n"
                "}                                                                 \n";

/// Camera control matrices
glm::mat4 projectionMatrix; // Store the projection matrix
glm::mat4 viewMatrix; // Store the view matrix
glm::mat4 modelMatrix; // Store the model matrix

// The handle to the window object
GLFWwindow *window;

// Define some of the global variables we're using for this sample
GLuint program;

const int TRIANGLE_STRIP_ARRAY_LENGTH = 45*3;
const int TRIANGLE_LIST_ARRAY_LENGTH = 288;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//// Fill this functions with your model code.

// USE THESE vertex array objects to define your objects
unsigned int vaoID[2];

unsigned int vboID[4];

/*!
 ADD YOUR CODE TO CREATE THE TRIANGLE STRIP MODEL TO THIS FUNCTION
 */
unsigned int createTriangleStripModel(void) {
    // use the vertex array object vaoID[0] for this model representation
    float *triangleStripModelVertices = new float[TRIANGLE_STRIP_ARRAY_LENGTH]{
            0.0, 0.0, 0.0, // Begin Lower Sides, Position = 0
            0.0, 1.0, 0.0,
            3.0, 0.0, 0.0,
            3.0, 1.0, 0.0,
            3.0, 0.0, 1.0,
            3.0, 1.0, 1.0,
            1.0, 0.0, 1.0,
            1.0, 1.0, 1.0,
            1.0, 0.0, 3.0,
            1.0, 1.0, 3.0,
            0.0, 0.0, 3.0,
            0.0, 1.0, 3.0,
            0.0, 0.0, 0.0,
            0.0, 1.0, 0.0, // End Lower Sides, Begin Upper Section, Position = 41
            0.0, 2.0, 0.0,
            1.0, 1.0, 0.0,
            1.0, 2.0, 0.0,
            3.0, 1.0, 0.0,
            1.0, 2.0, 1.0,
            3.0, 1.0, 1.0,
            1.0, 1.0, 1.0,
            1.0, 2.0, 1.0,
            0.0, 1.0, 1.0,
            0.0, 2.0, 1.0,
            0.0, 1.0, 0.0,
            0.0, 2.0, 0.0,
            0.0, 2.0, 1.0,
            1.0, 2.0, 0.0,
            1.0, 2.0, 1.0, // End Upper Section, Position = 86
            0.0, 1.0, 1.0, // Begin Lower Section Top, Position = 87
            1.0, 1.0, 1.0,
            0.0, 1.0, 3.0,
            1.0, 1.0, 3.0, // End Lower Section Top, Position = 98
            0.0, 0.0, 3.0, // Begin Lower First Section Bottom, Position = 99
            1.0, 0.0, 3.0,
            0.0, 0.0, 1.0,
            1.0, 0.0, 1.0,
            0.0, 0.0, 0.0,
            1.0, 0.0, 0.0, // End Lower First Section Bottom, Position = 115
            1.0, 0.0, 1.0, // Begin Lower Second Section Bottom, Position = 116
            3.0, 0.0, 1.0,
            1.0, 0.0, 0.0,
            3.0, 0.0, 0.0,
            // These last two values are a little hack to get the video card to completely draw the
            //   TRIANGLE_STRIP. I was having a problem where the card would not draw the last triangle.
            3.0, 0.0, 1.0,
            3.0, 0.5, 0.5, // End Lower Second Section Bottom, Position = 134
    };

    float *triangleStripModelColors = new float[TRIANGLE_STRIP_ARRAY_LENGTH]{
            1.0, 0.0, 0.0,
            1.0, 0.0, 0.0,
            1.0, 1.0, 0.0,
            1.0, 0.0, 0.0,
            1.0, 0.0, 0.0,
            1.0, 0.0, 0.0,
            1.0, 0.0, 0.0,
            1.0, 1.0, 0.0,
            1.0, 0.0, 0.0,
            1.0, 0.0, 0.0,
            1.0, 0.0, 0.0,
            1.0, 1.0, 0.0,
            1.0, 0.0, 0.0,
            1.0, 0.0, 0.0,
            0.0, 0.0, 1.0,
            0.0, 0.0, 1.0,
            0.0, 0.0, 1.0,
            0.0, 0.0, 1.0,
            0.0, 0.0, 1.0,
            0.0, 0.0, 1.0,
            0.0, 1.0, 1.0,
            0.0, 0.0, 1.0,
            0.0, 0.0, 1.0,
            0.0, 0.0, 1.0,
            0.0, 1.0, 1.0,
            0.0, 0.0, 1.0,
            0.0, 0.0, 1.0,
            0.0, 1.0, 1.0,
            0.0, 0.0, 1.0,
            1.0, 1.0, 0.0,
            1.0, 0.0, 0.0,
            1.0, 0.0, 0.0,
            1.0, 0.0, 0.0,
            1.0, 1.0, 0.0,
            1.0, 0.0, 0.0,
            1.0, 0.0, 0.0,
            1.0, 0.0, 0.0,
            1.0, 0.0, 0.0,
            1.0, 0.0, 0.0,
            1.0, 1.0, 0.0,
            1.0, 0.0, 0.0,
            1.0, 0.0, 0.0,
            1.0, 1.0, 0.0,
            1.0, 0.0, 0.0,
            1.0, 0.0, 0.0,
    };

    glGenVertexArrays(2, &vaoID[0]);
    glBindVertexArray(vaoID[0]);

    glGenBuffers(2, vboID);

    // Vertices
    glBindBuffer(GL_ARRAY_BUFFER, vboID[0]);
    glBufferData(GL_ARRAY_BUFFER, TRIANGLE_STRIP_ARRAY_LENGTH * sizeof(GLfloat), triangleStripModelVertices, GL_STATIC_DRAW);
    glVertexAttribPointer((GLuint) 0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    // Colors
    glBindBuffer(GL_ARRAY_BUFFER, vboID[1]);
    glBufferData(GL_ARRAY_BUFFER, TRIANGLE_STRIP_ARRAY_LENGTH * sizeof(GLfloat), triangleStripModelColors, GL_STATIC_DRAW);
    glVertexAttribPointer((GLuint) 1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    delete[] triangleStripModelVertices;
    delete[] triangleStripModelColors;

    return 1;
}

/*!
 ADD YOUR CODE TO CREATE A MODEL USING PRIMITIVES OF YOUR CHOISE TO THIS FUNCTION
 */
unsigned int createTriangleListModel(void) {
    // use the vertex array object vaoID[1] for this model representation
    float *triangleListModelVertices = new float[TRIANGLE_LIST_ARRAY_LENGTH]{
            0.0, 0.0, 0.0, // Triangle 1, First Side
            0.0, 1.0, 0.0,
            1.0, 1.0, 0.0,
            1.0, 1.0, 0.0, // Triangle 2
            1.0, 0.0, 0.0,
            0.0, 0.0, 0.0,
            1.0, 0.0, 0.0, // Triangle 3
            1.0, 1.0, 0.0,
            3.0, 1.0, 0.0,
            3.0, 1.0, 0.0, // Triangle 4
            3.0, 0.0, 0.0,
            1.0, 0.0, 0.0,
            3.0, 0.0, 1.0, // Triangle 5
            3.0, 1.0, 1.0,
            3.0, 1.0, 0.0,
            3.0, 1.0, 0.0, // Triangle 6
            3.0, 0.0, 0.0,
            3.0, 0.0, 1.0,
            1.0, 1.0, 1.0, // Triangle 7
            1.0, 0.0, 1.0,
            3.0, 1.0, 1.0,
            3.0, 1.0, 1.0, // Triangle 8
            3.0, 0.0, 1.0,
            1.0, 0.0, 1.0,
            1.0, 1.0, 3.0, // Triangle 9
            1.0, 0.0, 3.0,
            1.0, 1.0, 1.0,
            1.0, 1.0, 1.0, // Triangle 10
            1.0, 0.0, 1.0,
            1.0, 0.0, 3.0,
            0.0, 1.0, 3.0, // Triangle 11
            0.0, 0.0, 3.0,
            1.0, 1.0, 3.0,
            1.0, 1.0, 3.0, // Triangle 12
            1.0, 0.0, 3.0,
            0.0, 0.0, 3.0,
            0.0, 1.0, 3.0, // Triangle 13
            0.0, 0.0, 3.0,
            0.0, 1.0, 0.0,
            0.0, 1.0, 0.0, // Triangle 14
            0.0, 0.0, 0.0,
            0.0, 0.0, 3.0,
            0.0, 2.0, 0.0, // Triangle 15
            0.0, 1.0, 0.0,
            1.0, 2.0, 0.0,
            1.0, 2.0, 0.0, // Triangle 16
            1.0, 1.0, 0.0,
            0.0, 1.0, 0.0,
            0.0, 2.0, 1.0, // Triangle 17
            0.0, 1.0, 1.0,
            0.0, 2.0, 0.0,
            0.0, 2.0, 0.0, // Triangle 18
            0.0, 1.0, 0.0,
            0.0, 1.0, 1.0,
            0.0, 2.0, 1.0, // Triangle 19
            0.0, 1.0, 1.0,
            1.0, 2.0, 1.0,
            1.0, 2.0, 1.0, // Triangle 20
            1.0, 1.0, 1.0,
            0.0, 1.0, 1.0,
            1.0, 2.0, 1.0, // Triangle 21
            1.0, 1.0, 1.0,
            3.0, 1.0, 1.0,
            1.0, 2.0, 0.0, // Triangle 22, Last Side
            1.0, 1.0, 0.0,
            3.0, 1.0, 0.0,
            1.0, 2.0, 0.0, // Triangle 23, First Top, Angled
            3.0, 1.0, 0.0,
            1.0, 2.0, 1.0,
            1.0, 2.0, 1.0, // Triangle 24, Angled
            3.0, 1.0, 1.0,
            3.0, 1.0, 0.0,
            1.0, 2.0, 0.0, // Triangle 25
            0.0, 2.0, 0.0,
            0.0, 2.0, 1.0,
            0.0, 2.0, 1.0, // Triangle 26
            1.0, 2.0, 1.0,
            1.0, 2.0, 0.0,
            1.0, 1.0, 1.0, // Triangle 27
            0.0, 1.0, 1.0,
            0.0, 1.0, 3.0,
            0.0, 1.0, 3.0, // Triangle 28, Last Top
            1.0, 1.0, 3.0,
            1.0, 1.0, 1.0,
            3.0, 0.0, 0.0, // Triangle 29, First Bottom
            0.0, 0.0, 0.0,
            0.0, 0.0, 1.0,
            0.0, 0.0, 1.0, // Triangle 30
            3.0, 0.0, 1.0,
            3.0, 0.0, 0.0,
            1.0, 0.0, 1.0, // Triangle 31
            0.0, 0.0, 1.0,
            0.0, 0.0, 3.0,
            0.0, 0.0, 3.0, // Triangle 32, Last Bottom
            1.0, 0.0, 3.0,
            1.0, 0.0, 1.0,
    };

    // THIS IS A NEATO-BURRITO PATTERN, DID THIS FOR DEBUGGING BUT GONNA KEEP IT
    float *triangleListModelColors = new float[TRIANGLE_LIST_ARRAY_LENGTH]{
            0.0, 1.0, 0.0,
            0.0, 1.0, 0.0,
            1.0, 1.0, 0.0,
            0.0, 1.0, 0.0,
            0.0, 1.0, 0.0,
            1.0, 1.0, 0.0,
            0.0, 1.0, 0.0,
            0.0, 1.0, 0.0,
            1.0, 1.0, 0.0,
            0.0, 1.0, 0.0,
            0.0, 1.0, 0.0,
            1.0, 1.0, 0.0,
            0.0, 1.0, 0.0,
            0.0, 1.0, 0.0,
            1.0, 1.0, 0.0,
            0.0, 1.0, 0.0,
            0.0, 1.0, 0.0,
            1.0, 1.0, 0.0,
            0.0, 1.0, 0.0,
            0.0, 1.0, 0.0,
            1.0, 1.0, 0.0,
            0.0, 1.0, 0.0,
            0.0, 1.0, 0.0,
            1.0, 1.0, 0.0,
            0.0, 1.0, 0.0,
            0.0, 1.0, 0.0,
            1.0, 1.0, 0.0,
            0.0, 1.0, 0.0,
            0.0, 1.0, 0.0,
            1.0, 1.0, 0.0,
            0.0, 1.0, 0.0,
            0.0, 1.0, 0.0,
            1.0, 1.0, 0.0,
            0.0, 1.0, 0.0,
            0.0, 1.0, 0.0,
            1.0, 1.0, 0.0,
            0.0, 1.0, 0.0,
            0.0, 1.0, 0.0,
            1.0, 1.0, 0.0,
            0.0, 1.0, 0.0,
            0.0, 1.0, 0.0,
            1.0, 1.0, 0.0,
            0.0, 1.0, 0.0,
            0.0, 1.0, 0.0,
            1.0, 1.0, 0.0,
            0.0, 1.0, 0.0,
            0.0, 1.0, 0.0,
            1.0, 1.0, 0.0,
            0.0, 1.0, 0.0,
            0.0, 1.0, 0.0,
            1.0, 1.0, 0.0,
            0.0, 1.0, 0.0,
            0.0, 1.0, 0.0,
            1.0, 1.0, 0.0,
            0.0, 1.0, 0.0,
            0.0, 1.0, 0.0,
            1.0, 1.0, 0.0,
            0.0, 1.0, 0.0,
            0.0, 1.0, 0.0,
            1.0, 1.0, 0.0,
            0.0, 1.0, 0.0,
            0.0, 1.0, 0.0,
            1.0, 1.0, 0.0,
            0.0, 1.0, 0.0,
            0.0, 1.0, 0.0,
            1.0, 1.0, 0.0,
            0.0, 1.0, 0.0,
            0.0, 1.0, 0.0,
            1.0, 1.0, 0.0,
            0.0, 1.0, 0.0,
            0.0, 1.0, 0.0,
            1.0, 1.0, 0.0,
            0.0, 1.0, 0.0,
            0.0, 1.0, 0.0,
            1.0, 1.0, 0.0,
            0.0, 1.0, 0.0,
            0.0, 1.0, 0.0,
            1.0, 1.0, 0.0,
            0.0, 1.0, 0.0,
            0.0, 1.0, 0.0,
            1.0, 1.0, 0.0,
            0.0, 1.0, 0.0,
            0.0, 1.0, 0.0,
            1.0, 1.0, 0.0,
            0.0, 1.0, 0.0,
            0.0, 1.0, 0.0,
            1.0, 1.0, 0.0,
            0.0, 1.0, 0.0,
            0.0, 1.0, 0.0,
            1.0, 1.0, 0.0,
            0.0, 1.0, 0.0,
            0.0, 1.0, 0.0,
            1.0, 1.0, 0.0,
            0.0, 1.0, 0.0,
            0.0, 1.0, 0.0,
            1.0, 1.0, 0.0,
    };

    glGenVertexArrays(2, &vaoID[1]);
    glBindVertexArray(vaoID[1]);

    glGenBuffers(2, vboID);

    // Vertices
    glBindBuffer(GL_ARRAY_BUFFER, vboID[0]);
    glBufferData(GL_ARRAY_BUFFER, TRIANGLE_LIST_ARRAY_LENGTH * sizeof(GLfloat), triangleListModelVertices, GL_STATIC_DRAW);
    glVertexAttribPointer((GLuint) 0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(0);

    // Colors
    glBindBuffer(GL_ARRAY_BUFFER, vboID[1]);
    glBufferData(GL_ARRAY_BUFFER, TRIANGLE_LIST_ARRAY_LENGTH * sizeof(GLfloat), triangleListModelColors, GL_STATIC_DRAW);
    glVertexAttribPointer((GLuint) 1, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);

    delete[] triangleListModelVertices;
    delete[] triangleListModelColors;

    return 1;
}


/*!
 ADD YOUR CODE TO RENDER THE TRIANGLE STRIP MODEL TO THIS FUNCTION
 */
void renderTriangleStripModel(void) {
    // Bind the buffer and switch it to an active buffer
    glBindVertexArray(vaoID[0]);

    // Draw the triangles
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 42); // Lower Sides
    glDrawArrays(GL_TRIANGLE_STRIP, 41, 48); // Upper Section
    glDrawArrays(GL_TRIANGLE_STRIP, 87, 12); // Lower Section Top
    glDrawArrays(GL_TRIANGLE_STRIP, 99, 18); // Bottom
    glDrawArrays(GL_TRIANGLE_STRIP, 116, 18);

    // Unbind our Vertex Array Object
    glBindVertexArray(0);
}


/*!
 ADD YOUR CODE TO RENDER THE TRIANGLE STRIP MODEL TO THIS FUNCTION
 */
void renderTriangleListModel(void) {

    // Bind the buffer and switch it to an active buffer
    glBindVertexArray(vaoID[1]);

    // Draw the triangles
    glDrawArrays(GL_TRIANGLES, 0, TRIANGLE_LIST_ARRAY_LENGTH);

    // Unbind our Vertex Array Object
    glBindVertexArray(0);
}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*!
 This function creates the two models
 */
void setupScene(void) {

    createTriangleStripModel();
    renderTriangleStripModel();
    createTriangleListModel();
}


int main(int argc, const char *argv[]) {

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// Init glfw, create a window, and init glew

    // Init the GLFW Window
    window = initWindow();

    // Init the glew api
    initGlew();

    // Prepares some defaults
    CoordSystemRenderer *coordinate_system_renderer = new CoordSystemRenderer(10.0);

    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    //// The Shader Program starts here

    // Vertex shader source code. This draws the vertices in our window. We have 3 vertices since we're drawing an triangle.
    // Each vertex is represented by a vector of size 4 (x, y, z, w) coordinates.
    static const string vertex_code = vs_string;
    static const char *vs_source = vertex_code.c_str();

    // Fragment shader source code. This determines the colors in the fragment generated in the shader pipeline. In this case, it colors the inside of our triangle specified by our vertex shader.
    static const string fragment_code = fs_string;
    static const char *fs_source = fragment_code.c_str();

    // This next section we'll generate the OpenGL program and attach the shaders to it so that we can render our triangle.
    program = glCreateProgram();

    // We create a shader with our fragment shader source code and compile it.
    GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fs, 1, &fs_source, NULL);
    glCompileShader(fs);

    // We create a shader with our vertex shader source code and compile it.
    GLuint vs = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vs, 1, &vs_source, NULL);
    glCompileShader(vs);

    // We'll attach our two compiled shaders to the OpenGL program.
    glAttachShader(program, vs);
    glAttachShader(program, fs);

    glLinkProgram(program);

    // We'll specify that we want to use this program that we've attached the shaders to.
    glUseProgram(program);

    //// The Shader Program ends here
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    /// IGNORE THE NEXT PART OF THIS CODE
    /// IGNORE THE NEXT PART OF THIS CODE
    /// IGNORE THE NEXT PART OF THIS CODE
    // It controls the virtual camera

    // Set up our green background color
    static const GLfloat clear_color[] = {0.6f, 0.7f, 1.0f, 1.0f};
    static const GLfloat clear_depth[] = {1.0f, 1.0f, 1.0f, 1.0f};

    projectionMatrix = glm::perspective(1.1f, (float) 800 / (float) 600, 0.1f, 100.f);
    modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f,
                                                            0.0f)); // Create our model matrix which will halve the size of our model
    viewMatrix = glm::lookAt(glm::vec3(1.0f, 0.0f, 5.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    int projectionMatrixLocation = glGetUniformLocation(program,
                                                        "projectionMatrix"); // Get the location of our projection matrix in the shader
    int viewMatrixLocation = glGetUniformLocation(program,
                                                  "viewMatrix"); // Get the location of our view matrix in the shader
    int modelMatrixLocation = glGetUniformLocation(program,
                                                   "modelMatrix"); // Get the location of our model matrix in the shader

    glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE,
                       &projectionMatrix[0][0]); // Send our projection matrix to the shader
    glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]); // Send our view matrix to the shader
    glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]); // Send our model matrix to the shader

    //// The Shader Program ends here
    //// START TO READ AGAIN
    //// START TO READ AGAIN
    //// START TO READ AGAIN
    ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

    glBindAttribLocation(program, 0, "in_Position");
    glBindAttribLocation(program, 1, "in_Color");

    // this creates the scene
    setupScene();

    int i = 0;

    // Enable depth test
    // ignore this line, it allows us to keep the distance value after we proejct each object to a 2d canvas.
    glEnable(GL_DEPTH_TEST);

    // This is our render loop. As long as our window remains open (ESC is not pressed), we'll continue to render things.
    while (!glfwWindowShouldClose(window)) {

        // Clear the entire buffer with our green color (sets the background to be green).
        glClearBufferfv(GL_COLOR, 0, clear_color);
        glClearBufferfv(GL_DEPTH, 0, clear_depth);

        // this draws the coordinate system
        coordinate_system_renderer->draw();

        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
        //// This generate the object
        // Enable the shader program
        glUseProgram(program);

        // this changes the camera location
        glm::mat4 rotated_view = viewMatrix * GetRotationMatrix();
        glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &rotated_view[0][0]); // send the view matrix to our shader

        // This moves the model to the right
        modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(2.0f, 0.0f, 0.0f));
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]); // Send our model matrix to the shader

        // This line renders your triangle strip model
        renderTriangleStripModel();

        // This moves the model to the left
        modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(-2.0f, -0.0f, 0.0f));
        glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]); // Send our model matrix to the shader

        // This line renders your Polygon model
        renderTriangleListModel();

        // disable the shader program
        glUseProgram(0);

        //// This generate the object
        ///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

        // Swap the buffers so that what we drew will appear on the screen.
        glfwSwapBuffers(window);
        glfwPollEvents();

    }

    // delete the coordinate system object
    delete coordinate_system_renderer;

    // Program clean up when the window gets closed.
    glDeleteVertexArrays(2, vaoID);
    glDeleteProgram(program);
}

