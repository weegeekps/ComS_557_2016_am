/* 
 * hw4_part2_main.cpp
 * HCI 557 Homework 2 Part 4 Assignment
 * for Adam N. Morris
 * 
 * Several classes based on code created by Rafael Radkowski
 */

#include <GL/glew.h>
#include "controls.h"
#include "HCI557Common.h"
#include "CoordSystem.h"
#include "Plane3D.h"
#include "TripleTexture.h"

GLFWwindow* window;
GLuint program;

extern Trackball trackball;

extern int g_change_texture_blend;

int main(int argc, const char * argv[])
{
	window = initWindow();

	initGlew();

	CoordSystem* coordinate_system = new CoordSystem(40.0);

#pragma region Create & Configure Plane
	GLAppearance* appearance = new GLAppearance("hw4_part2_vertex_shader.glsl", "hw4_part2_fragment_shader.glsl");

	// Using the values in the example since they seem to work well.
	GLDirectLightSource  light_source;
	light_source._lightPos = glm::vec4(00.0, 20.0, 20.0, 0.0);
	light_source._ambient_intensity = 0.2;
	light_source._specular_intensity = 4.5;
	light_source._diffuse_intensity = 1.0;
	light_source._attenuation_coeff = 0.0;
	appearance->addLightSource(light_source);

	GLSpotLightSource spotlight_source;
	spotlight_source._lightPos = glm::vec4(0.0, 00.0, 50.0, 1.0);
	spotlight_source._ambient_intensity = 0.2;
	spotlight_source._specular_intensity = 30.5;
	spotlight_source._diffuse_intensity = 8.0;
	spotlight_source._attenuation_coeff = 0.0002;
	spotlight_source._cone_direction = glm::vec3(-1.0, -1.0, -1.0);
	spotlight_source._cone_angle = 20.0;
	appearance->addLightSource(spotlight_source);

	GLMaterial material;
	material._diffuse_material = glm::vec3(0.8, 0.8, 0.0);
	material._ambient_material = glm::vec3(0.8, 0.8, 0.0);
	material._specular_material = glm::vec3(1.0, 1.0, 1.0);
	material._shininess = 12.0;
	material._transparency = 1.0;
	appearance->setMaterial(material);

	// TripleTexture is my own class based of the pattern in Dr. Radkowski's texture classes.
	TripleTexture* texture = new TripleTexture();
	int texture_id = texture->loadAndCreateTextures("black_and_white_gradient.bmp", "./shenandoah.bmp", "my_portrait.bmp");
	assert(texture_id > 5 && "Textures might not have gotten loaded properly.");
	appearance->setTexture(texture);

	appearance->finalize();

	GLPlane3D* plane = new GLPlane3D(0.0, 0.0, 0.0, 50.0, 50.0);
	plane->setApperance(*appearance);
	plane->init();

	appearance->updateLightSources();
#pragma endregion

#pragma region Render Loop
	static const GLfloat clear_color[] = { 0.6f, 0.7f, 0.0f, 1.0f };
	static const GLfloat clear_depth[] = { 1.0f, 1.0f, 1.0f, 1.0f };

	// the first parameter is the eye position, the second the center location, and the third the up vector.
	SetViewAsLookAt(glm::vec3(12.0f, 12.0f, 65.5f), glm::vec3(1.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	while(!glfwWindowShouldClose(window))
	{
		glClearBufferfv(GL_COLOR, 0, clear_color);
		glClearBufferfv(GL_DEPTH, 0, clear_depth);

		SetTrackballLocation(trackball.getRotationMatrix());

		// Draw our objects
		coordinate_system->draw();
		plane->draw();

		// This is neat, press t to change the texture blend mode.
		bool isValidCall = texture->setTextureBlendMode(g_change_texture_blend);
		if (isValidCall) appearance->updateTextures();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	delete coordinate_system;
#pragma endregion
}
