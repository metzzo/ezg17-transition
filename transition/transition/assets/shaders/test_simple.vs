#version 430 core

layout (location = 0) in vec4 in_position;

struct MVP {
	mat4 model;
	mat4 view;
	mat4 projection;
};
uniform MVP mvp;

void main() {
	gl_Position = mvp.projection * mvp.view * mvp.model * vec4(in_position.xyz, 1.0f);
}