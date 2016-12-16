#pragma once

#include <glm/glm.hpp>
#include "gl_common/GLObjectObj.h"

enum COLLISION_TYPE { COLLIDING, OVERLAP, NONE };

class BoundingBox
{
	GLObjectObj object;

	glm::vec3 max;
	glm::vec3 min;

public:
	BoundingBox();

	BoundingBox(GLObjectObj object);

	~BoundingBox();

	void recalculate(glm::vec3 translation_matrix);

	COLLISION_TYPE is_colliding_with(BoundingBox &other_bb) const;
};

