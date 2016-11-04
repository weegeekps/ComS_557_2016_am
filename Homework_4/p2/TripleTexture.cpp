/*
* TripleTexture.h
* Adam N. Morris
*
* HCI 557
*/
#include "TripleTexture.h"

#ifdef WIN32
string TripleTexture::_glsl_names[4] = { "gradient_texture", "landscape_texture", "person_texture", "texture_blend" };
#endif

TripleTexture::TripleTexture()
{
	gradient_texture = 0;
	landscape_texture = 0;
	person_texture = 0;
	texture_blend_mode = 0;

	gradient_texture_index = -1;
	landscape_texture_index = -1;
	person_texture_index = -1;
	texture_blend_mode_index = -1;

	_dirty = false;
}

TripleTexture::~TripleTexture()
{
}

int TripleTexture::loadAndCreateTextures(string gradient_texture_path, string landscape_texture_path, string person_texture_path)
{
	TextureMetaData gradient_texture_meta_data, landscape_texture_meta_data, person_texture_meta_data;

	unsigned char* gradient_texture_data = loadBitmapFile(gradient_texture_path, gradient_texture_meta_data.channels, gradient_texture_meta_data.width, gradient_texture_meta_data.height);
	unsigned char* landscape_texture_data = loadBitmapFile(landscape_texture_path, landscape_texture_meta_data.channels, landscape_texture_meta_data.width, landscape_texture_meta_data.height);
	unsigned char* person_texture_data = loadBitmapFile(person_texture_path, person_texture_meta_data.channels, person_texture_meta_data.width, person_texture_meta_data.height);

	if (gradient_texture_data == nullptr ||
		landscape_texture_data == nullptr ||
		person_texture_data == nullptr)
	{
		return -1;
	}

#pragma region Gradient Texture Creation
	gradient_texture = generateAndBindTexture(gradient_texture_data, gradient_texture_meta_data);
	
	free(gradient_texture_data);
#pragma endregion 

#pragma region Landscape Texture Creation
	landscape_texture = generateAndBindTexture(landscape_texture_data, landscape_texture_meta_data);

	free(landscape_texture_data);
#pragma endregion

#pragma region Person Texture Creation
	person_texture = generateAndBindTexture(person_texture_data, person_texture_meta_data);

	free(person_texture_data);
#pragma endregion

	return gradient_texture + landscape_texture + person_texture;
}

bool TripleTexture::setTextureBlendMode(int mode)
{
	if (mode < 0 || mode > 3) return false;
	if (texture_blend_mode == mode) return false;

	texture_blend_mode = mode;

	_dirty = true;

	return true;
}

bool TripleTexture::addVariablesToProgram(GLuint program, int variable_index)
{
	// We can't add variables if the program hasn't been bound yet.
	if (program == -1) return false;

	GLint params;

	glGetProgramiv(program, GL_LINK_STATUS, &params);
	if (params == GL_FALSE)
	{
		std::cerr << "[TripleTexture] Program " << program << " has not been linked. Textures cannot be added." << std::endl;

		return false;
	}

	glUseProgram(program);

	gradient_texture_index = glGetUniformLocation(program, _glsl_names[0].c_str());
	checkUniform(gradient_texture_index, _glsl_names[0]);

	landscape_texture_index = glGetUniformLocation(program, _glsl_names[1].c_str());
	checkUniform(landscape_texture_index, _glsl_names[1]);

	person_texture_index = glGetUniformLocation(program, _glsl_names[2].c_str());
	checkUniform(person_texture_index, _glsl_names[2]);

	texture_blend_mode_index = glGetUniformLocation(program, _glsl_names[3].c_str());
	checkUniform(texture_blend_mode_index, _glsl_names[3]);

	glActiveTexture(GL_TEXTURE0);
	glBindTexture(GL_TEXTURE_2D, gradient_texture);
	glUniform1i(gradient_texture_index, 0);

	glActiveTexture(GL_TEXTURE1);
	glBindTexture(GL_TEXTURE_2D, landscape_texture);
	glUniform1i(landscape_texture_index, 1);

	glActiveTexture(GL_TEXTURE2);
	glBindTexture(GL_TEXTURE_2D, person_texture);
	glUniform1i(person_texture_index, 2);

	dirty(program);

	return true;
}

bool TripleTexture::dirty(GLuint program)
{
	glUseProgram(program);

	glUniform1i(texture_blend_mode_index, texture_blend_mode);

	glUseProgram(0);

	_dirty = false;

	return true;
}

GLuint generateAndBindTexture(unsigned char* data, TextureMetaData meta_data)
{
	GLuint texture;
	
	glActiveTexture(GL_TEXTURE0);

	glGenTextures(1, &texture);

	glBindTexture(GL_TEXTURE_2D, texture);

	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);

	if (meta_data.channels == 3)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, meta_data.width, meta_data.height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
	}
	else if (meta_data.channels == 4)
	{
		glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, meta_data.width, meta_data.height, 0, GL_BGR, GL_UNSIGNED_BYTE, data);
	}

	return texture;
}
