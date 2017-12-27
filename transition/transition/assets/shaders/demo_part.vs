#version 430 core

layout (location = 0) in vec4 in_position;
layout (location = 1) in vec4 in_color;

struct MVP {
	mat4 model;
	mat4 view;
	mat4 projection;
};
uniform MVP mvp;

out float vg_ttl;
out vec3 vg_color;

void main() {
	gl_Position = mvp.view * mvp.model * vec4(in_position.xyz, 1.0f);
	vg_ttl = in_position.w;
	vg_color = in_color.rgb;
}