#version 330 core
layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;
layout (location = 2) in vec3 aNormal;

out VS_OUT {
    vec3 frag_pos;
    vec3 normal;
    vec2 tex_coords;
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
	vs_out.normal = mat3(transpose(inverse(mvp.model)))*aNormal;
	vs_out.tex_coords = aTex;
	
	gl_Position = mvp.projection * mvp.view * vec4(vs_out.frag_pos, 1.0);
}