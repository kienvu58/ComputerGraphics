#version 330 core
in vec2 outTexCoords;

out vec4 color;

uniform sampler2D texture0;

void main()
{
   color = texture(texture0, outTexCoords); 
}