#version 330 core
in vec2 uv;
out vec4 FragColor;

uniform sampler2D diffusetexture;

void main()
{
    FragColor = vec4(0.0, 1.0, 0.0, 1.0);
	FragColor = vec4(texture(diffusetexture, uv).rgb, 1.0f);
	//FragColor.yz = uv;
}