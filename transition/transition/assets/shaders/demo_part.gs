#version 430 core

layout (points) in;
layout (triangle_strip, max_vertices = 4) out;

in float vg_ttl[];

out vec2 gf_uv;
flat out float gf_ttl;

struct MVP {
	mat4 model;
	mat4 view;
	mat4 projection;
};

uniform MVP mvp;

void main () {
	const vec2 size = vec2(0.5, 0.5);
	vec4 P = gl_in[0].gl_Position;
	//First Vertex
	vec2 va = P.xy + vec2(-0.5, -0.5) * size;
	gl_Position = mvp.projection * vec4(va, P.zw);
	gf_uv = vec2(0.0, 0.0);
	gf_ttl = vg_ttl[0];
	EmitVertex();
	//Second Vertex
	va = P.xy + vec2(+0.5, -0.5) * size;
	gl_Position = mvp.projection * vec4(va, P.zw);
	gf_uv = vec2(1.0, 0.0);
	gf_ttl = vg_ttl[0];
	EmitVertex();
	//Third Vertex
	va = P.xy + vec2(-0.5, +0.5) * size;
	gl_Position = mvp.projection * vec4(va, P.zw);
	gf_uv = vec2(0.0, 1.0);
	gf_ttl = vg_ttl[0];
	EmitVertex();
	//Fourth Vertex
	va = P.xy + vec2(+0.5, +0.5) * size;
	gl_Position = mvp.projection * vec4(va, P.zw);
	gf_uv = vec2(1.0, 1.0);
	gf_ttl = vg_ttl[0];
	EmitVertex();

	EndPrimitive();
}