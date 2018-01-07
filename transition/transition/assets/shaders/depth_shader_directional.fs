#version 330 core

in vec2 f_uv;

uniform bool has_alpha_tex;
uniform sampler2D alpha_tex;

void main()
{             
    //gl_FragDepth = gl_FragCoord.z;
	if (has_alpha_tex) {
		float alpha = texture(alpha_tex, f_uv).r;
		if (alpha < 0.01) discard;
	}
} 