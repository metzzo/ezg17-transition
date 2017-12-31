#version 330 core

layout (location = 0) in vec3 aPos;

out VS_OUT {
    vec3 frag_pos;
} vs_out;

struct MVP {
	mat4 model;
	mat4 view;
	mat4 projection;
};

uniform MVP mvp;

void main()
{
	vs_out.frag_pos = vec3(mvp.model * vec4(aPos, 1.0));

	gl_Position = mvp.projection * mvp.view * vec4(vs_out.frag_pos, 1.0);
}