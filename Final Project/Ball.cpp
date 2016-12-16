#include "Ball.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtx/transform.hpp>
#include "gl_common/GLObjectObj.h"
#include "TogglableSpotLightSource.h"

void Ball::set_position() const
{
	glm::vec3 translation_vector = glm::vec3(x_position, 0.0f, z_position);
	glm::mat4 translation_matrix = glm::translate(translation_vector);
	object->setMatrix(translation_matrix);
	CollisionObject::bounding_box->recalculate(translation_vector);
}

Ball::Ball(unsigned int min_velocity_factor, unsigned int max_velocity_factor, unsigned int velocity_factor_step): min_velocity_factor(min_velocity_factor), max_velocity_factor(max_velocity_factor), velocity_factor_step(velocity_factor_step)
{
}


Ball::~Ball()
{
	if (object != nullptr)
	{
		delete object;
	}

	if (appearance_0 != nullptr)
	{
		delete appearance_0;
	}
}

void Ball::init(FieldConfiguration configuration)
{
	this->configuration = configuration;

	appearance_0 = new GLAppearance("shaders/spherical_mapping.vert", "shaders/spherical_mapping.frag");

	left_spot_light_source._lightPos = glm::vec4(20.0f, 10.0f, 0.0f, 0.0f);
	left_spot_light_source._ambient_intensity = 0.0f;
	left_spot_light_source._specular_intensity = 1.0f;
	left_spot_light_source._diffuse_intensity = 1.5f;
	left_spot_light_source._attenuation_coeff = 0.0f;

	appearance_0->addLightSource(left_spot_light_source);

	right_spot_light_source._lightPos = glm::vec4(-20.0f, 10.0f, 0.0f, 0.0f);
	right_spot_light_source._ambient_intensity = 0.0f;
	right_spot_light_source._specular_intensity = 1.0f;
	right_spot_light_source._diffuse_intensity = 1.5f;
	right_spot_light_source._attenuation_coeff = 0.0f;
	right_spot_light_source.turn_light_off();

	appearance_0->addLightSource(right_spot_light_source);

	GLMaterial material_0;
	material_0._diffuse_material = glm::vec3(0.9f, 0.9f, 0.9f);
	material_0._ambient_material = glm::vec3(0.9f, 0.9f, 0.9f);
	material_0._specular_material = glm::vec3(1.0, 1.0, 1.0);
	material_0._shininess = 8.0f;
	material_0._transparency = 1.0f;

	appearance_0->setMaterial(material_0);

	texture = new GLTexture();
	texture->loadAndCreateTexture("models/reflection_map.bmp");
	appearance_0->setTexture(texture);

	appearance_0->finalize();

	object = new GLObjectObj("models/Ball.obj");
	object->setApperance(*appearance_0);
	object->init();

	reset();
}

void Ball::reset()
{
	x_direction = BALL_LEFT;
	z_direction = BALL_UP;

	x_velocity_factor = 1;
	z_velocity_factor = 1;

	x_position = 0.0f;
	z_position = configuration.z_start * -1.0f + 1.2f;

	glm::vec3 initial_position = glm::vec3(x_position, 0.0f, z_position);
	glm::mat4 translation_matrix = glm::translate(initial_position);
	object->setMatrix(translation_matrix);

	CollisionObject::bounding_box = new BoundingBox(*object);
	CollisionObject::bounding_box->recalculate(initial_position);
}

void Ball::draw()
{
	if (is_moving)
	{
		// I'm a bit proud about this. I probably shouldn't be, but I'm happy I realized a few key things here.
		//   I use the base_velocity to calculate the basic speed. The factors then dictate how narrow or wide
		//   the angle is. Finally, the direction (handled by an enum with a 1 and -1 value) dictates the sign
		//   of the velocity indicating whether or not it moves left or right.
		//
		// A couple notes so I don't forget:
		//   If the x_velocity_factor > z_velocity_factor, the angle is >45 degrees. The inverse is also true.
		//   On the x axis, positive direction means we're going left, negative going right.
		//   On the z axis, positive = up, negative = down.
		//   Equally increasing the velocity factor means the ball moves faster at the basic 45 degree angle.
		x_position += base_velocity * x_velocity_factor * x_direction;
		z_position += base_velocity * z_velocity_factor * z_direction;

		set_position();
	}

	texture->activate_texture(object->getProgram());

	object->draw();
}

void Ball::start_movement()
{
	is_moving = true;
}

void Ball::stop_movement()
{
	is_moving = false;
}

bool Ball::is_ball_moving() const
{
	return is_moving;
}

void Ball::move_ball_to_x_position(float paddle_x_position)
{
	x_position = paddle_x_position;

	set_position();
}

bool Ball::is_ball_out_of_bounds() const
{
	if (z_position < configuration.z_start * -1.0f)
	{
		return true;
	}

	return false;
}

void Ball::bounce_on_x_axis()
{
	if (x_direction == BALL_LEFT)
	{
		x_direction = BALL_RIGHT;
	}
	else
	{
		x_direction = BALL_LEFT;
	}

	swap_light_sources();
}

void Ball::bounce_on_z_axis()
{
	if (z_direction == BALL_UP)
	{
		z_direction = BALL_DOWN;
	}
	else
	{
		z_direction = BALL_UP;
	}
}

void Ball::increase_x_velocity()
{
	if (x_velocity_factor < max_velocity_factor)
	{
		x_velocity_factor += velocity_factor_step;
	}
}

void Ball::decrease_x_velocity()
{
	if (x_velocity_factor > min_velocity_factor)
	{
		x_velocity_factor -= velocity_factor_step;
	}
}

void Ball::increase_z_velocity()
{
	if (z_velocity_factor < max_velocity_factor)
	{
		z_velocity_factor += velocity_factor_step;
	}
}

void Ball::decrease_z_velocity()
{
	if (z_velocity_factor > min_velocity_factor)
	{
		z_velocity_factor -= velocity_factor_step;
	}
}

void Ball::swap_light_sources()
{
	if (left_spot_light_source.is_light_on())
	{
		left_spot_light_source.turn_light_off();
		right_spot_light_source.turn_light_on();
	}
	else
	{
		left_spot_light_source.turn_light_on();
		right_spot_light_source.turn_light_off();
	}

	appearance_0->updateLightSources();
}

float Ball::get_x_position() const
{
	return x_position;
}

void Ball::set_start_direction(BallXDirection direction)
{
	if (!is_moving)
	{
		x_direction = direction;
	}
}
