#pragma once

#include <GL/glew.h>
#include <GLFW/glfw3.h>
#define GLM_FORCE_INLINE
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include "gl_common/GLObjectObj.h"
#include "BoundingBox.h"
#include "CollisionObject.h"
#include "TogglableSpotLightSource.h"

class Block : public CollisionObject
{
	GLObjectObj* object;
	bool is_visible;

	GLAppearance* appearance_0;
	TogglableSpotLightSource left_spot_light_source;
	TogglableSpotLightSource right_spot_light_source;

public:
	Block();
	~Block();
	void draw() const;
	void init(double x_position, double z_position, glm::vec3 color);
	void toggle_block();
	bool is_block_visible() const;
	void swap_light_sources();
};
