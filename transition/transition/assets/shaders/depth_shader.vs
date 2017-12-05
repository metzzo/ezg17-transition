#version 330 core
layout (location = 0) in vec3 aPos;

struct MVP {
	mat4 model;
	mat4 view;
	mat4 projection;
};

uniform MVP mvp;

void main()
{
	gl_Position = mvp.projection * mvp.view * mvp.model * vec4(aPos, 1.0);
}  