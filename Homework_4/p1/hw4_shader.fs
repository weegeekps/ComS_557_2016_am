#version 410 core

uniform sampler2D tex;

in vec2 pass_TexCoord;

out vec4 color;

void main(void)
{
    color = texture(tex, pass_TexCoord);
}