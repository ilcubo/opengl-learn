#version 410 core

in vec3 colour;
in vec2 texCoord;

out vec4 frag_colour;

uniform sampler2D texture_0;
uniform sampler2D texture_1;

void main() { frag_colour = mix(texture(texture_0, texCoord), texture(texture_1, texCoord), 0.5); };
