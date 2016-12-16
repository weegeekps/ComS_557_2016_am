#include "TogglableSpotLightSource.h"
#include "gl_common/Shaders.h"

#ifdef WIN32
string TogglableSpotLightSource::_glsl_names[1] = { "light_state" };
#endif

TogglableSpotLightSource::TogglableSpotLightSource()
{
	light_state = 1;
	light_state_idx = -1;
}


TogglableSpotLightSource::~TogglableSpotLightSource()
{
}

bool TogglableSpotLightSource::is_light_on() const
{
	return light_state == 1;
}

void TogglableSpotLightSource::turn_light_off()
{
	light_state = 0;
}

void TogglableSpotLightSource::turn_light_on()
{
	light_state = 1;
}

bool TogglableSpotLightSource::addVariablesToProgram(GLuint program, int variable_index)
{
	if (program == -1)
	{
		return false;
	}

	GLint params;
	glGetProgramiv(program, GL_LINK_STATUS, &params);
	if (params == GL_FALSE)
	{
		cerr << "[ToggleableSpotlightSource] Program " << program << " has not been linked. Light source cannot be added." << endl;
		return false;
	}

	glUseProgram(program);

	light_state_idx = glGetUniformLocation(program, GetVariableName(_glsl_object, _glsl_names[0], variable_index).c_str());
	checkUniform(light_state_idx, GetVariableName(_glsl_object, _glsl_names[0], variable_index));

	GLSpotLightSource::addVariablesToProgram(program, variable_index);

	glUniform1i(light_state_idx, light_state);

	return true;
}

bool TogglableSpotLightSource::dirty(GLuint program)
{
	glUseProgram(program);

	glUniform1i(light_state_idx, light_state);

	GLSpotLightSource::dirty(program);

	glUseProgram(0);

	return true;
}
