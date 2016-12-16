#include "CollisionObject.h"


CollisionObject::CollisionObject()
{
	bounding_box = nullptr;
}

CollisionObject::~CollisionObject()
{
	if (bounding_box != nullptr)
	{
		delete bounding_box;
	}
}

BoundingBox* CollisionObject::get_bounding_box() const
{
	return bounding_box;
}

bool CollisionObject::check_collision(CollisionObject &object) const
{
	BoundingBox& other_bb = *object.get_bounding_box();
	COLLISION_TYPE collision = bounding_box->is_colliding_with(other_bb);

	switch (collision)
	{
	case COLLIDING:
		return true;
		break;
	case OVERLAP:
	case NONE:
	default:
		return false;
	}
}
