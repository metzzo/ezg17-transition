#version 330 core
layout (location = 0) out vec4 fragColor;
in vec2 TexCoords;

uniform sampler2D tex;

void main()
{             
	fragColor = vec4(texture(tex, TexCoords).rgb, 1);
}