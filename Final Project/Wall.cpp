#include "Wall.h"


string Wall::model_for_type() const
{
	switch(type)
	{
	case LEFT_WALL:
	case RIGHT_WALL:
		return "models/Side_Wall.obj";
	case BACK_WALL:
		return "models/Back_Wall.obj";
	default:
		assert("Invalid type.");
		return nullptr;
	}
}

Wall::Wall(WALL_TYPE type): type(type)
{
	wall = nullptr;
}


Wall::~Wall()
{
	if (wall != nullptr)
	{
		delete wall;
	}
}

void Wall::init(FieldConfiguration configuration)
{
	GLAppearance* appearance_0 = new GLAppearance("shaders/displacement_texture.vert", "shaders/displacement_texture.frag");

	GLDirectLightSource light_source;
	light_source._lightPos = glm::vec4(20.0, 20.0, 0.0, 0.0);
	light_source._ambient_intensity = 0.2f;
	light_source._specular_intensity = 4.0f;
	light_source._diffuse_intensity = 2.0f;
	light_source._attenuation_coeff = 0.0f;

	appearance_0->addLightSource(light_source);

	GLMaterial material_0;
	material_0._diffuse_material = glm::vec3(0.9f, 0.9f, 0.9f);
	material_0._ambient_material = glm::vec3(0.9f, 0.9f, 0.9f);
	material_0._specular_material = glm::vec3(1.0, 1.0, 1.0);
	material_0._shininess = 8.0f;
	material_0._transparency = 1.0f;

	appearance_0->setMaterial(material_0);

	texture = new GLMultiTexture();
	texture->loadAndCreateTextures("models/walls.bmp", "models/walls_norm.bmp");
	appearance_0->setTexture(texture);

	appearance_0->finalize();

	wall = new GLObjectObj(model_for_type());
	wall->setApperance(*appearance_0);
	wall->init();

	glm::mat4 wall_rotation;
	
	if (type == LEFT_WALL || type == RIGHT_WALL) 
	{
		//wall_rotation = glm::rotate(1.5708f, glm::vec3(0.0f, 1.0f, 0.0f));
	}

	glm::vec3 initial_position;
	glm::mat4 wall_transform;

	switch (type)
	{
	case RIGHT_WALL:
		initial_position = glm::vec3(configuration.x_minimum, 0.0f, 0.0f);
		wall_transform = glm::translate(initial_position)/* * wall_rotation*/;
		break;
	case LEFT_WALL:
		initial_position = glm::vec3(configuration.x_maximum, 0.0f, 0.0f);
		wall_transform = glm::translate(initial_position)/* * wall_rotation*/;
		break;
	case BACK_WALL:
	default:
		initial_position = glm::vec3(0.0f, 0.0f, configuration.z_start + configuration.num_rows + 2.0f); // The extra 2.0f is for a little space in the back.
		wall_transform = glm::translate(initial_position);
	}

	wall->setMatrix(wall_transform);

	CollisionObject::bounding_box = new BoundingBox(*wall);
	CollisionObject::bounding_box->recalculate(initial_position);
}

void Wall::draw() const
{
	if (wall == nullptr)
	{
		assert("You must call Walls::init(FieldConfiguration) before calling Walls::draw()");
	}

	texture->activate_texture(wall->getProgram());

	wall->draw();
}

WALL_TYPE Wall::getType() const
{
	return type;
}
