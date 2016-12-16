#pragma once
#include "gl_common\GLAppearance.h"

class TogglableSpotLightSource :
	public GLSpotLightSource
{

#ifdef WIN32
	static string _glsl_names[];
#else
	const string _glsl_names[1] = { "light_state" };
#endif

	/**
	 * \brief The state of the light. 0 = off, 1 = on.
	 */
	int light_state;

	/**
	 * \brief The uniform index for the light state.
	 */
	int light_state_idx;

public:
	TogglableSpotLightSource();
	~TogglableSpotLightSource();

	/**
	 * \brief Returns a bool indicating whether or not the light is currently on.
	 */
	bool is_light_on() const;

	/**
	 * \brief Turns the light off.
	 */
	void turn_light_off();

	/**
	 * \brief Turns the light on.
	 */
	void turn_light_on();

	
	virtual bool addVariablesToProgram(GLuint program, int variable_index);

	virtual bool dirty(GLuint program);
};
