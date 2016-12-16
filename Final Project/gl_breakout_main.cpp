/* 
 * gl_breakout_main.cpp
 * Author: Adam N. Morris
 * 
 * Several classes based on code created by Rafael Radkowski
 */

#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "gl_common/controls.h"
#include "gl_common/HCI557Common.h"
#include "gl_common/CoordSystem.h"
#include "Block.h"
#include "BlockColorSequencer.h"
#include "Paddle.h"
#include "Ball.h"
#include "Wall.h"

GLFWwindow* window;
GLuint program;

extern Trackball trackball;

extern int g_change_texture_blend;

FieldConfiguration configuration;
BlockColorSequencer color_sequencer;
vector<Block*> blocks = {};
vector<Wall*> walls = {};
Paddle* paddle;
Ball* ball;

void keyboard_handler(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
	{
		glfwSetWindowShouldClose(window, GL_TRUE);
	}

	if (action == GLFW_PRESS)
	{
		switch (key)
		{
		case GLFW_KEY_LEFT:
			cout << "PRESSING LEFT" << endl;
			paddle->start_move_left();
			break;
		case GLFW_KEY_RIGHT:
			cout << "PRESSING RIGHT" << endl;
			paddle->start_move_right();
			break;
		case GLFW_KEY_SPACE:
			cout << "SPACE PRESSED" << endl;
			ball->start_movement();
			break;
		default:
			break;
		}
	}

	if (action == GLFW_RELEASE)
	{
		switch (key)
		{
		case GLFW_KEY_LEFT:
		case GLFW_KEY_RIGHT:
			cout << "KEY RELEASED" << endl;
			paddle->stop_move();
			break;
		default:
			break;
		}
	}
}

int main(int argc, const char* argv[])
{
	window = initWindow();

	initGlew();

	CoordSystem* coordinate_system = new CoordSystem(40.0);

	// These values are used to configure the playing field.
	configuration.x_minimum = -12.0f;
	configuration.x_maximum = 12.0f;
	configuration.z_start = 12.0;
	configuration.num_rows = 5;

#pragma region Generate Walls
	Wall* left_wall = new Wall(LEFT_WALL);
	left_wall->init(configuration);
	walls.push_back(left_wall);

	Wall* right_wall = new Wall(RIGHT_WALL);
	right_wall->init(configuration);
	walls.push_back(right_wall);

	Wall* back_wall = new Wall(BACK_WALL);
	back_wall->init(configuration);
	walls.push_back(back_wall);
#pragma endregion 

#pragma region Generate Blocks
	for (float z = configuration.z_start; z <= configuration.z_start + configuration.num_rows; ++z)
	{
		for (float x = configuration.x_minimum; x < configuration.x_maximum; x += 3)
		{
			Block* block = new Block();
			block->init(x, z, color_sequencer.next_color());
			blocks.push_back(block);
		}
	}
#pragma endregion

#pragma region Generate Paddle
	paddle = new Paddle();
	paddle->init(configuration);
	paddle->draw();
#pragma endregion 

#pragma region Generate Ball
	ball = new Ball(1, 5, 1);
	ball->init(configuration);
#pragma endregion

	// Set our keyboard event handler.
	glfwSetKeyCallback(window, keyboard_handler);

#pragma region Render Loop
	static const GLfloat clear_color[] = {0.0f, 0.0f, 0.0f, 1.0f};
	static const GLfloat clear_depth[] = {1.0f, 1.0f, 1.0f, 1.0f};

	// the first parameter is the eye position, the second the center location, and the third the up vector.
	SetViewAsLookAt(glm::vec3(0.0f, 16.0f, -19.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));

	glEnable(GL_DEPTH_TEST);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	while (!glfwWindowShouldClose(window))
	{
		glClearBufferfv(GL_COLOR, 0, clear_color);
		glClearBufferfv(GL_DEPTH, 0, clear_depth);

		SetTrackballLocation(trackball.getRotationMatrix());

		// Draw our objects
		//coordinate_system->draw();

		bool toggle_block_light = false;

		for (Wall* wall : walls)
		{
			wall->draw();

			if (ball->check_collision(*wall))
			{
				cout << "Wall Collision!" << endl;

				if (wall->getType() == BACK_WALL)
				{
					ball->bounce_on_z_axis();
				}
				else
				{
					ball->bounce_on_x_axis();
					toggle_block_light = true;
				}

				switch (wall->getType())
				{
				case BACK_WALL:
					ball->decrease_z_velocity();
					break;
				case LEFT_WALL:
				case RIGHT_WALL:
				default:
					ball->increase_z_velocity();
				}
			}
		}

		paddle->draw();

		// MMMMM, nested if conditions. How nasty. I should clean these up.
		if (ball->is_ball_moving()) {
			if (ball->is_ball_out_of_bounds())
			{
				cout << "Ball is out of bounds!" << endl;
				ball->stop_movement();
				paddle->stop_move();
				ball->reset();
				paddle->reset();
				SetViewAsLookAt(glm::vec3(0.0f, 16.0f, -19.0f), glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 1.0f, 0.0f));
			}
			else if (ball->check_collision(*paddle))
			{
				float distance_to_paddle_center = paddle->measure_x_distance_from_paddle_center(ball->get_x_position());
				cout << "Impact distance: " << distance_to_paddle_center << endl;

				if (distance_to_paddle_center > 1.5f)
				{
					ball->increase_x_velocity();
				}
				else
				{
					ball->decrease_x_velocity();
				}

				ball->bounce_on_z_axis();
			}
		} else
		{
			ball->move_ball_to_x_position(paddle->get_x_position());

			switch (paddle->get_direction())
			{
			case PADDLE_LEFT:
				ball->set_start_direction(BALL_LEFT);
				break;
			case PADDLE_RIGHT:
				ball->set_start_direction(BALL_RIGHT);
				break;
			case PADDLE_STAY:
			default:
				// Do nothing.
				break;
			}
		}

		bool no_blocks_left = true;
		for (Block* block : blocks)
		{
			if (!block->is_block_visible())
			{
				continue;
			}

			no_blocks_left = false;

			if (toggle_block_light)
			{
				block->swap_light_sources();
			}

			block->draw();

			if (ball->check_collision(*block))
			{
				cout << "Block Collision!" << endl;
				block->toggle_block();
				ball->bounce_on_z_axis();
			}
		}

		if (no_blocks_left)
		{
			ball->stop_movement();
			cout << "Game over! You win!" << endl;
		}

		ball->draw();

		glfwSwapBuffers(window);
		glfwPollEvents();
	}

	delete coordinate_system;
#pragma endregion
}
