#version 330 core
#define MAX_NR_LIGHTS 10

layout (location = 0) in vec3 aPos;
layout (location = 1) in vec2 aTex;
layout (location = 2) in vec3 aNormal;

out VS_OUT {
    vec3 frag_pos;
    vec3 normal;
    vec2 tex_coords;
    vec4 frag_pos_lightspace[MAX_NR_LIGHTS];
} vs_out;


uniform mat4 light_space_matrices[MAX_NR_LIGHTS];
uniform int num_lights;

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
	
	for (int i = 0; i < num_lights; i++) {
		vs_out.frag_pos_lightspace[i] = light_space_matrices[i] * vec4(vs_out.frag_pos, 1.0);
	}
	
	gl_Position = mvp.projection * mvp.view * vec4(vs_out.frag_pos, 1.0);
}