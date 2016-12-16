
#include "BlockColorSequencer.h"

BlockColorSequencer::BlockColorSequencer()
{
}

BlockColorSequencer::~BlockColorSequencer()
{
}

glm::vec3 BlockColorSequencer::next_color()
{
	unsigned int color_picker_value = current_index % colors.size();

	current_index++;

	return colors[color_picker_value];
}
