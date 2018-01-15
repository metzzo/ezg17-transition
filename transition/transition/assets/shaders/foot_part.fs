#version 430 core

in vec2 gf_uv;
flat in float gf_ttl;
in vec3 gf_color;
out vec4 FragColor;
uniform float max_ttl;


void main() {
	float radius = 2*length(gf_uv - vec2(0.5, 0.5));
	float intensity = min(gf_ttl,max_ttl)/max_ttl;
	if (radius > 1) {
		discard;
	} else {
		FragColor = vec4(gf_color, intensity*(1-radius));
	}
}