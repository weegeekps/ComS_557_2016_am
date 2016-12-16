
#include "Block.h"
#include <glm/detail/type_mat.hpp>

Block::Block()
{
	object = nullptr;
	is_visible = true;
}

Block::~Block()
{
	if (object != nullptr)
	{
		delete object;
	}
}

void Block::draw() const
{
	if (object != nullptr)
	{
		object->draw();
	} else
	{
		assert("Call Block::init() prior to calling Block::draw()");
	}
}

void Block::init(double x_position, double z_position, glm::vec3 color)
{
	appearance_0 = new GLAppearance("shaders/multi_vertex_lights.vert", "shaders/multi_vertex_lights.frag");

	left_spot_light_source._lightPos = glm::vec4(20.0, 20.0, 0.0, 1.0);
	left_spot_light_source._ambient_intensity = 0.0f;
	left_spot_light_source._specular_intensity = 4.0f;
	left_spot_light_source._diffuse_intensity = 2.0f;
	left_spot_light_source._attenuation_coeff = 0.00005f;
	left_spot_light_source._cone_angle = 90.0f; 
	left_spot_light_source._cone_direction = glm::vec3(0.0, -3.0, -3.0);
	left_spot_light_source.turn_light_off();

	appearance_0->addLightSource(left_spot_light_source);

	right_spot_light_source._lightPos = glm::vec4(-20.0, 20.0, 0.0, 1.0);
	right_spot_light_source._ambient_intensity = 0.0f;
	right_spot_light_source._specular_intensity = 4.0f;
	right_spot_light_source._diffuse_intensity = 2.0f;
	right_spot_light_source._attenuation_coeff = 0.00005f;
	right_spot_light_source._cone_angle = 90.0f;
	right_spot_light_source._cone_direction = glm::vec3(0.0, -3.0, -3.0);

	appearance_0->addLightSource(right_spot_light_source);

	GLMaterial material_0;
	material_0._diffuse_material = color;
	material_0._ambient_material = color;
	material_0._specular_material = glm::vec3(1.0, 1.0, 1.0);
	material_0._shininess = 8.0f;
	material_0._transparency = 1.0f;

	appearance_0->setMaterial(material_0);
	appearance_0->finalize();

	object = new GLObjectObj("models/Normal_Block.obj");
	object->setApperance(*appearance_0);
	object->init();

	// Correct based on the center point of model.
	// This is a quality of life thing so I can have a consistent point at the bottom left corner.
	x_position += 1.5;
	z_position += 0.5;

	glm::vec3 initial_position = glm::vec3(x_position, 0.0, z_position);
	glm::mat4 transform = glm::translate(initial_position);
	object->setMatrix(transform);

	bounding_box = new BoundingBox(*object);
	bounding_box->recalculate(initial_position); // This is the only time we have to recalcuate the AABB for the block.
}

void Block::toggle_block()
{
	is_visible = !is_visible;
}

bool Block::is_block_visible() const
{
	return is_visible;
}

void Block::swap_light_sources()
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
