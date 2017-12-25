#version 430 core

in vec2 gf_uv;
flat in float gf_ttl;
out vec4 FragColor;

void main() {
	float radius = 2*length(gf_uv - vec2(0.5, 0.5));
	if (radius > 1) {
		discard;
	} else {
		FragColor = vec4(1, 0, 0, 1-radius);
	}
}