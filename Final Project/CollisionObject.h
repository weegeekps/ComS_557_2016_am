#pragma once
#include "BoundingBox.h"

class CollisionObject
{
public:
	CollisionObject();
	~CollisionObject();

	BoundingBox* get_bounding_box() const;
	bool check_collision(CollisionObject& object) const;
protected:
	BoundingBox* bounding_box;
};
