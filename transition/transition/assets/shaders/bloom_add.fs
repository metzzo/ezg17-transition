#version 330 core
layout (location = 0) out vec4 fragColor;
in vec2 TexCoords;

uniform sampler2D tex1;
uniform sampler2D tex2;
uniform float addintensity;
uniform sampler2D end_tex;
uniform float end_tex_intensity;

void main()
{             
    vec3 originalColor = texture(tex1, TexCoords).rgb;      
    vec3 bloomColor = addintensity * texture(tex2, TexCoords).rgb;
	vec3 end_tex_color = end_tex_intensity * texture(end_tex, TexCoords).rgb;
	vec3 real_color = originalColor+bloomColor;
	fragColor = vec4(mix(real_color, end_tex_color, end_tex_intensity), 1);
}