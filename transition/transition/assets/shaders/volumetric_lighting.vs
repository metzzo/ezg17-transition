#version 330 core

layout (location = 0) in vec3 position;
layout (location = 1) in vec2 TexCoords;

out VS_OUT {
	vec2 tex_coords;
} vs_out;

void main() {
	vs_out.tex_coords = TexCoords;
    gl_Position = vec4(position, 1.0f);
}

