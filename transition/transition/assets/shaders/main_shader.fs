#version 330 core
in vec2 uv;
out vec4 FragColor;

struct Light {
	int light_type;
	
	vec3 position;
    vec3 direction;
  
    float constant;
    float linear;
    float quadratic;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
#define MAX_NR_LIGHTS 10
uniform Light lights[MAX_NR_LIGHTS];
uniform int num_lights;

uniform sampler2D diffusetexture;

void main()
{
    FragColor = vec4(0.0, 1.0, 0.0, 1.0);
	FragColor = vec4(texture(diffusetexture, uv).rgb, 1.0f);
	//FragColor.yz = uv;
}