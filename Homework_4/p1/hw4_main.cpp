//
//  main.cpp
//  Texture template for Homework 4
//
//  Created by Rafael Radkowski on 5/28/15.
//  Copyright (c) 2015 -. All rights reserved.
//



/*---------------------------------------------------------------------------------
Note,

- The code gives you some hints what to do. I added some comments
- The shader code is stored in two files. HW4_shader.vs/.fs
  You must edit this shader programs in order to get the shader code to work with textures.

In addition:
- The class (file) ShaderFileUtils provides functions to load shader code from a file.
- The class (file) ImgLoader provides a function to load a bitmap .bmp image from a file.

-----------------------------------------------------------------------------------*/

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
#include "ShaderFileUtils.h"
#include "ImgLoader.h"



// this line tells the compiler to use the namespace std.
// Each object, command without a namespace is assumed to be part of std. 
using namespace std;


/// Camera control matrices
glm::mat4 projectionMatrix; // Store the projection matrix
glm::mat4 viewMatrix; // Store the view matrix
glm::mat4 modelMatrix; // Store the model matrix




// The handle to the window object
GLFWwindow*         window;


// Define some of the global variables we're using for this sample
GLuint program;

GLuint texture;


// USE THESE vertex array objects to define your objects
unsigned int vaoID[2];

unsigned int vboID[4];




/**
This function checks your shader code
@param shader - the id of the shader
@param shader_type - type = GL_FRAGMENT_SHADER or GL_VERTEX_SHADER
*/
bool CheckShaderLocal(GLuint shader, GLenum shader_type)
{
	GLint compiled = 0;
	glGetShaderiv(shader, GL_COMPILE_STATUS, &compiled);

	if (!compiled)
	{
		GLint info_len = 0;

		glGetShaderiv(shader, GL_INFO_LOG_LENGTH, &info_len);

		if (info_len)
		{
			char* buf = (char*)malloc(info_len);
			if (buf) {
				glGetShaderInfoLog(shader, info_len, NULL, buf);
				cout << "Could not compile shader shader_type " << shader_type << ":\n" << buf << endl;
				free(buf);
			}
			glDeleteShader(shader);
			shader = 0;
		}
		return false;
	}
	return true;

}




/*!
This function creates a single square
 */
void createSquare(void)
{
	// Vertices for our square
	// x, y, z, u, v
	float* vertices = new float[30]{
		-0.5, -0.5, 0.5, 0.0, 0.0, // Bottom left corner

		-0.5, 0.5, 0.5, 0.0, 1.0, // Top left corner

		0.5, 0.5, 0.5, 1.0, 1.0, // Top Right corner

		0.5, -0.5, 0.5, 1.0, 0.0, // Bottom right corner

		-0.5, -0.5, 0.5, 0.0, 0.0, // Bottom left corner

		0.5, 0.5, 0.5, 1.0, 1.0, // Top Right corner
	}; 




	/*---------------------------------------------------------------------------------
	  TODO:
	  - Add texture coordinates to the vertices above
	  - Assign the texture coordinates to a vertex buffer objects as discussed in class.
		Note, they ned to be inside the same vbo in which the vertices are defined.
	-----------------------------------------------------------------------------------*/



	glGenVertexArrays(2, &vaoID[0]); // Create our Vertex Array Object
	glBindVertexArray(vaoID[0]); // Bind our Vertex Array Object so we can use it


	glGenBuffers(2, vboID); // Generate our Vertex Buffer Object

							// vertices
	glBindBuffer(GL_ARRAY_BUFFER, vboID[0]);
	glBufferData(GL_ARRAY_BUFFER, 30 * sizeof(GLfloat), vertices, GL_STATIC_DRAW); // The magic number 90 corresponds to the number of verticies multiplied by 5 to include the UV map.

	glVertexAttribPointer((GLuint)0, 3, GL_FLOAT, GL_FALSE, 5 * sizeof(GLfloat), 0); // Set up our vertex attributes pointer
	glEnableVertexAttribArray(0); // Disable our Vertex Array Object

	// Texture
	glVertexAttribPointer((GLuint)1, 2, GL_FLOAT, GL_TRUE, 5 * sizeof(GLfloat), (const GLvoid*)(3 * sizeof(GLfloat)));
	glEnableVertexAttribArray(1);

	glBindVertexArray(0); // Disable our Vertex Buffer Object


	delete[] vertices; // Delete our vertices from memory


	/*---------------------------------------------------------------------------------
	TODO:
	- Load the texture
	- Create a texture object
	- Set the correct texture unit active
	- Set all texture parameters as requried
	- Load the texture to your graphics card
	- Create a texture uniform variable
	-----------------------------------------------------------------------------------*/

	#pragma region Texture Create
	unsigned char* textureData = ImgLoader::Load(".\\programming_meme.bmp");
	
	glGenTextures(1, &texture);
	
	glBindTexture(GL_TEXTURE_2D, texture);
	
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_REPEAT);
	
	// Our image is 3 channels.
	glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, 256, 256, 0, GL_BGR, GL_UNSIGNED_BYTE, textureData);
	#pragma endregion 
}







/*!
 This code renders the plane
 */
void renderSquare(void)
{

	glUseProgram(program);

	glBindVertexArray(vaoID[0]); // Bind our Vertex Array Object

	glDrawArrays(GL_TRIANGLES, 0, 6); // Draw our square

	glBindVertexArray(0); // Unbind our Vertex Array Object

	glUseProgram(0);

}



///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////


/*!
 This function creates the two models
 */
void setupScene(void) {

	createSquare();

}




int main(int argc, const char * argv[])
{

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//// Init glfw, create a window, and init glew

	// Init the GLFW Window
	window = initWindow();


	// Init the glew api
	initGlew();

	// Prepares some defaults
	CoordSystemRenderer* coordinate_system_renderer = new CoordSystemRenderer(10.0);

	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
	//// The Shader Program starts here

	// Vertex shader source code. This draws the vertices in our window. We have 3 vertices since we're drawing an triangle.
	// Each vertex is represented by a vector of size 4 (x, y, z, w) coordinates.
	static const string vertex_code = ShaderFileUtils::LoadFromFile(".\\hw4_shader.vs");
	static const char * vs_source = vertex_code.c_str();

	// Fragment shader source code. This determines the colors in the fragment generated in the shader pipeline. In this case, it colors the inside of our triangle specified by our vertex shader.
	static const string fragment_code = ShaderFileUtils::LoadFromFile(".\\hw4_shader.fs");
	static const char * fs_source = fragment_code.c_str();

	// This next section we'll generate the OpenGL program and attach the shaders to it so that we can render our triangle.
	program = glCreateProgram();

	// We create a shader with our fragment shader source code and compile it.
	GLuint fs = glCreateShader(GL_FRAGMENT_SHADER);
	glShaderSource(fs, 1, &fs_source, NULL);
	glCompileShader(fs);
	bool ret = CheckShaderLocal(fs, GL_VERTEX_SHADER);
	if (!ret) { cout << "Problems compiling GL_VERTEX_SHADER" << endl; }

	// We create a shader with our vertex shader source code and compile it.
	GLuint vs = glCreateShader(GL_VERTEX_SHADER);
	glShaderSource(vs, 1, &vs_source, NULL);
	glCompileShader(vs);
	ret = CheckShaderLocal(vs, GL_VERTEX_SHADER);
	if (!ret) { cout << "Problems compiling GL_VERTEX_SHADER" << endl; }

	// We'll attach our two compiled shaders to the OpenGL program.
	glAttachShader(program, vs);
	glAttachShader(program, fs);

	glLinkProgram(program);

	// We'll specify that we want to use this program that we've attached the shaders to.
	glUseProgram(program);

	//// The Shader Program ends here
	///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

	GLuint textureIndex = glGetUniformLocation(program, "tex");

	glActiveTexture(GL_TEXTURE0);

	glBindTexture(GL_TEXTURE_2D, texture);

	glUniform1i(textureIndex, 0);

	// Set up our green background color
	static const GLfloat clear_color[] = { 0.6f, 0.7f, 1.0f, 1.0f };
	static const GLfloat clear_depth[] = { 1.0f, 1.0f, 1.0f, 1.0f };


	projectionMatrix = glm::perspective(1.1f, (float)800 / (float)600, 0.1f, 100.f);
	modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f)); // Create our model matrix which will halve the size of our model
	viewMatrix = glm::lookAt(glm::vec3(1.0f, 0.0f, 2.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	int projectionMatrixLocation = glGetUniformLocation(program, "projectionMatrix"); // Get the location of our projection matrix in the shader
	int viewMatrixLocation = glGetUniformLocation(program, "viewMatrix"); // Get the location of our view matrix in the shader
	int modelMatrixLocation = glGetUniformLocation(program, "modelMatrix"); // Get the location of our model matrix in the shader


	glUniformMatrix4fv(projectionMatrixLocation, 1, GL_FALSE, &projectionMatrix[0][0]); // Send our projection matrix to the shader
	glUniformMatrix4fv(viewMatrixLocation, 1, GL_FALSE, &viewMatrix[0][0]); // Send our view matrix to the shader
	glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]); // Send our model matrix to the shader

	glBindAttribLocation(program, 0, "in_Position");
	glBindAttribLocation(program, 1, "in_Tex");

	// this creates the scene
	setupScene();

	int i = 0;

	// Enable depth test
	// ignore this line, it allows us to keep the distance value after we proejct each object to a 2d canvas.
	glEnable(GL_DEPTH_TEST);

	// This is our render loop. As long as our window remains open (ESC is not pressed), we'll continue to render things.
	while (!glfwWindowShouldClose(window))
	{

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


		// This moves the model 
		modelMatrix = glm::translate(glm::mat4(1.0f), glm::vec3(0.0f, 0.0f, 0.0f));
		glUniformMatrix4fv(modelMatrixLocation, 1, GL_FALSE, &modelMatrix[0][0]); // Send our model matrix to the shader


		// This line renders your Ppolygon model
		renderSquare();


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

