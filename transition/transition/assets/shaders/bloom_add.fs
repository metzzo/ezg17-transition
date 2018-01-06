#version 330 core
layout (location = 0) out vec4 fragColor;
in vec2 TexCoords;

uniform sampler2D tex1;
uniform sampler2D tex2;
uniform float addintensity;

void main()
{             
    vec3 originalColor = texture(tex1, TexCoords).rgb;      
    vec3 bloomColor = addintensity * texture(tex2, TexCoords).rgb;
	fragColor = vec4(originalColor+bloomColor, 1);
}