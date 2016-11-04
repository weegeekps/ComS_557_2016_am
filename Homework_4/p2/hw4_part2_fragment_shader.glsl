#version 410 core

in vec2 pass_TexCoord;
in vec4 pass_Color;

uniform sampler2D gradient_texture;
uniform sampler2D landscape_texture;
uniform sampler2D person_texture;
uniform int texture_blend;

out vec4 color;

void main(void)
{
	vec4 gradient_texture_color = texture(gradient_texture, pass_TexCoord);
	vec4 landscape_texture_color = texture(landscape_texture, pass_TexCoord);
	vec4 person_texture_color = texture(person_texture, pass_TexCoord);

	if (texture_blend == 0) 
	{
		// This looks like a horrible Facebook profile picture.
		color = (0.25 * pass_Color + landscape_texture_color + person_texture_color) * gradient_texture_color;
	} 
	else if (texture_blend == 1) 
	{
		color = 0.1 * pass_Color * gradient_texture_color + landscape_texture_color + 0.5 * person_texture_color;
	}
	else if (texture_blend == 2)
	{
		color = 0.1 * pass_Color + 2 * (landscape_texture_color * gradient_texture_color * person_texture_color);
	} 
	else if (texture_blend == 3) // I'm having fun with creating all these different texture blends, so here's a 4th.
	{
		// This one is actually pretty cool. I do an addition on the landscape and my portrait, then multiply them and add lighting
		//   It makes for a much better balanced combination than if I had just did landscape + person.
		color = 0.1 * pass_Color + (landscape_texture_color + person_texture_color) * landscape_texture_color;
	}
	else 
	{
		color = (0.25 * pass_Color + landscape_texture_color + person_texture_color) * gradient_texture_color;
	}
}