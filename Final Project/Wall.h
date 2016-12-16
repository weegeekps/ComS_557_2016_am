#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define GLM_FORCE_INLINE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>

#include "gl_common/GLAppearance.h"
#include "gl_common/GLObjectObj.h"
#include "FieldConfiguration.h"
#include "CollisionObject.h"

enum WALL_TYPE { LEFT_WALL, RIGHT_WALL, BACK_WALL };

class Wall : public CollisionObject
{
	GLObjectObj* wall;
	GLMultiTexture* texture = nullptr;

	const WALL_TYPE type;

	string model_for_type() const;

public:
	Wall(WALL_TYPE type);
	~Wall();
	/**
	* \brief Initializes the Wall and configures it based on the specified FieldConfiguration object.
	* \param configuration The configuration object used throughout the program.
	*/
	void init(FieldConfiguration configuration);
	void draw() const;

	WALL_TYPE getType() const;
};

