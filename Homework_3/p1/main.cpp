/* HW3_Lighting
 * Adam Morris
 * Based on code from Rafael Radkowski
 */

// stl include
#include <iostream>

// GLEW include
#include <GL/glew.h>

// GLM include files
#define GLM_FORCE_INLINE

// glfw includes
#include <GLFW/glfw3.h>

// include local files
//#include "headers/controls.h"
#include "headers/HCI557Common.h"
#include "headers/CoordSystem.h"

#include "headers/Shaders.h"
#include "headers/SphereBase.h"
#include "headers/RedSphere.h"
#include "headers/BlueSphere.h"
#include "headers/GreenSphere.h"
#include "headers/YellowSphere.h"

using namespace std;

// The handle to the window object
GLFWwindow*         window;

// Define some of the global variables we're using for this sample
GLuint program;

RedSphere* redSphere;
BlueSphere* blueSphere;
GreenSphere* greenSphere;
YellowSphere* yellowSphere;

void setupScene(void) {
    // I could make a factory method to put the construction and init together, but holding off for times sake.
    redSphere = new RedSphere(0.0f, 0.0, 0.0);
    redSphere->init();

    blueSphere = new BlueSphere(25.0f, 0.0, 0.0);
    blueSphere->init();

    greenSphere = new GreenSphere(50.0f, 0.0, 0.0);
    greenSphere->init();

    yellowSphere = new YellowSphere(75.0f, 0.0, 0.0);
    yellowSphere->init();
}

int main(int argc, const char * argv[])
{
    window = initWindow();
    initGlew();

	CoordSystemRenderer* coordinate_system_renderer = new CoordSystemRenderer(10.0);

    program = LoadAndCreateShaderProgram("shaders/test_vs.glsl", "shaders/test_fs.glsl");

    // Set up our green background color
    static const GLfloat clear_color[] = { 0.6f, 0.7f, 1.0f, 1.0f };
    static const GLfloat clear_depth[] = { 1.0f, 1.0f, 1.0f, 1.0f };

    // this creates the scene
    setupScene();

    int i=0;

    SetViewAsLookAt(glm::vec3(35.0f, 0.0f, 75.0f), glm::vec3(35.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

    // Enable depth test
    // ignore this line, it allows us to keep the distance value after we proejct each object to a 2d canvas.
    glEnable(GL_DEPTH_TEST);

    // This is our render loop. As long as our window remains open (ESC is not pressed), we'll continue to render things.
    while(!glfwWindowShouldClose(window))
    {
        // Clear the entire buffer with our green color (sets the background to be green).
        glClearBufferfv(GL_COLOR , 0, clear_color);
        glClearBufferfv(GL_DEPTH , 0, clear_depth);

        // this draws the coordinate system
//		coordinate_system_renderer->draw();

        redSphere->draw();
        blueSphere->draw();
        greenSphere->draw();
        yellowSphere->draw();

        // Swap the buffers so that what we drew will appear on the screen.
        glfwSwapBuffers(window);
        glfwPollEvents();

    }

	// delete the coordinate system object
	delete coordinate_system_renderer;
    delete redSphere;

    // Program clean up when the window gets closed.
//    glDeleteVertexArrays(2, mVaoID);
    glDeleteProgram(program);
}

