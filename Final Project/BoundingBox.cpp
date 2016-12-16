#include "BoundingBox.h"
#include "BlockColorSequencer.h"
#include <glm/detail/_vectorize.hpp>
#include <glm/detail/type_mat.hpp>


BoundingBox::BoundingBox()
{
}

BoundingBox::BoundingBox(GLObjectObj object)
{
	this->object = object;
}

BoundingBox::~BoundingBox()
{
	// This class is not the owner of the "object", so we don't want to clean it up.
}

void BoundingBox::recalculate(glm::vec3 translation_vector)
{
	auto vertices = object.getVertices();

	min = vertices[0];
	max = vertices[0];

	for (glm::vec3 vertex : vertices)
	{
		if (vertex.x < min.x)
		{
			min.x = vertex.x;
		}

		if (vertex.x > max.x)
		{
			max.x = vertex.x;
		}

		if (vertex.y < min.y)
		{
			min.y = vertex.y;
		}

		if (vertex.y > max.y)
		{
			max.y = vertex.y;
		}

		if (vertex.z < min.z)
		{
			min.z = vertex.z;
		}

		if (vertex.z > max.z)
		{
			max.z = vertex.z;
		}
	}

	min = min + translation_vector;
	max = max + translation_vector;
}

COLLISION_TYPE BoundingBox::is_colliding_with(BoundingBox &other_bb) const
{
	if (max.x < other_bb.min.x || min.x > other_bb.max.x ||
		max.y < other_bb.min.y || min.y > other_bb.max.y ||
		max.z < other_bb.min.z || min.z > other_bb.max.z)
	{
		return NONE;
	}

	if (min.x <= other_bb.min.x && max.x >= other_bb.max.x &&
		min.y <= other_bb.min.y && max.y >= other_bb.max.y &&
		min.z <= other_bb.min.z && max.z >= other_bb.max.z)
	{
		return OVERLAP; // Catching this case cause I may do something with it.
	}

	return COLLIDING;
}

