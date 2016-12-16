#pragma once
#include "FieldConfiguration.h"
#include "gl_common/GLObjectObj.h"
#include "BoundingBox.h"
#include "Block.h"
#include "TogglableSpotLightSource.h"

enum BallXDirection
{
	BALL_LEFT = 1,
	BALL_RIGHT = -1
};

enum BallZDirection
{
	BALL_UP = 1,
	BALL_DOWN = -1
};

class Ball : public CollisionObject
{
	GLObjectObj* object = nullptr;
	BoundingBox* bounding_box = nullptr;
	GLTexture* texture = nullptr;

	float x_position = 0.0f;
	float z_position = 0.0f;

	const float base_velocity = 0.05f;

	const unsigned int min_velocity_factor;
	const unsigned int max_velocity_factor;
	const unsigned int velocity_factor_step;

	unsigned int x_velocity_factor = 1;
	unsigned int z_velocity_factor = 1;

	BallXDirection x_direction = BALL_LEFT;
	BallZDirection z_direction = BALL_UP;

	bool is_moving = false;
	int dont_switch_x_counter = 0;

	FieldConfiguration configuration;

	GLAppearance* appearance_0;
	TogglableSpotLightSource left_spot_light_source;
	TogglableSpotLightSource right_spot_light_source;

	void set_position() const;

public:
	
	Ball(unsigned int min_velocity_factor, unsigned int max_velocity_factor, unsigned int velocity_factor_step);
	~Ball();

	void init(FieldConfiguration configuration);
	void draw();

	void start_movement();
	void stop_movement();
	bool is_ball_moving() const;
	void move_ball_to_x_position(float paddle_x_position);
	void reset();

	bool is_ball_out_of_bounds() const;

	void bounce_on_x_axis();
	void bounce_on_z_axis();

	void increase_x_velocity();
	void decrease_x_velocity();

	void increase_z_velocity();
	void decrease_z_velocity();

	void swap_light_sources();

	float get_x_position() const;

	void set_start_direction(BallXDirection direction);
};

