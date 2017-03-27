#version 330 core
in vec2 out_texCoords;

out vec4 color;

uniform sampler2D texture_diffuse0;

void main()
{
    color = vec4(texture(texture_diffuse0, out_texCoords));
}