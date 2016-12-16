#version 410 core                                                 

uniform sampler2D tex;

in vec2 pass_TexCoord;
in vec4 pass_Color;
out vec4 color;

uniform int texture_blend;

void main(void)                                                   
{                                                                 
    vec4 tex_color = texture(tex, pass_TexCoord);
    
    color = pass_Color * 0.5 + tex_color * 0.5;
}                           