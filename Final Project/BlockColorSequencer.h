#pragma once
#include <glm/detail/type_vec3.hpp>
#include <vector>

/**
 * Picks colors in a sequence for blocks.
 */
class BlockColorSequencer
{
public:
	BlockColorSequencer();
	~BlockColorSequencer();

	/**
	 * \brief Returns the next color in the sequence.
	 * \return A glm::vec3 with the color.
	 */
	glm::vec3 next_color();

private:
	unsigned int current_index = 0;

	std::vector<glm::vec3> colors = {
		glm::vec3(1.0f, 0.0f, 0.0f), // Red
		glm::vec3(0.0f, 1.0f, 0.0f), // Green
		glm::vec3(1.0f, 1.0f, 0.0f), // Yellow
		glm::vec3(0.0f, 0.0f, 1.0f), // Blue
		glm::vec3(1.0f, 0.0f, 1.0f), // Magenta
		glm::vec3(0.0f, 1.0f, 1.0f)  // Cyan
	};
};