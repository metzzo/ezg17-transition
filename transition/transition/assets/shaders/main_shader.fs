#version 330 core
#define MAX_NR_LIGHTS 10

in VS_OUT {
    vec3 frag_pos;
    vec3 normal;
    vec2 tex_coords;
} fs_in;

out vec4 FragColor;

struct Light {
	// light_type=1: directional light
	// light_type=2: point light
	int light_type;
	
	vec3 position;
    vec3 direction;
  
	float constant;
    float linear;
    float quadratic;
  
    vec3 diffuse;
    vec3 specular;
};
uniform Light lights[MAX_NR_LIGHTS];
uniform int num_lights;

struct Material {
	bool has_diffuse_tex;
	sampler2D diffuse_tex;
	sampler2D specular_tex;
	float shininess;
	
	vec3 ambient_color;
	vec3 diffuse_color;
	vec3 specular_color;
};
uniform Material material;

uniform vec3 view_pos;

vec3 calc_dir_light(
	Light light, 
	vec3 diffuse_material, 
	vec3 normal, 
	vec3 view_dir
);

vec3 calc_point_light(
	Light light, 
	vec3 diffuse_material, 
	vec3 normal, 
	vec3 view_dir
);

void main() {
	vec3 diffuse_tex;
	if (material.has_diffuse_tex) {
		diffuse_tex = vec3(texture(material.diffuse_tex, fs_in.tex_coords));
	} else {
		diffuse_tex = vec3(1,1,1);
	}

	vec3 color = material.ambient_color * diffuse_tex;
	vec3 normal = normalize(fs_in.normal);
    vec3 view_dir = normalize(view_pos - fs_in.frag_pos);
	
	for (int i = 0; i < num_lights; i++) {
		switch (lights[i].light_type) {
		case 1:
			color += calc_dir_light(
				lights[i], 
				diffuse_tex, 
				normal, 
				view_dir
			);
			break;
		case 2:
			color += calc_point_light(
				lights[i], 
				diffuse_tex, 
				normal, 
				view_dir
			);
			break;
		default:
			color = vec3(0.0,1.0,0.0);
		}
	}
	
	FragColor = vec4(color, 1.0f);
}

vec3 calc_dir_light(
	Light light, 
	vec3 diffuse_tex, 
	vec3 normal, 
	vec3 view_dir) {
	
	vec3 light_dir = normalize(-light.direction);
	
	// diffuse
    float diff = max(dot(normal, light_dir), 0.0);
    vec3 diffuse = diff * (light.diffuse * material.diffuse_color);
    
	// specular
	vec3 specular = vec3(0.0f);
	if (diff > 0) {
		vec3 reflect_dir = reflect(-light_dir, normal);
		float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
		specular = spec * (light.specular * material.specular_color); 
	}
    
	return (diffuse + specular)*diffuse_tex;
}

vec3 calc_point_light(
	Light light, 
	vec3 diffuse_tex, 
	vec3 normal,
	vec3 view_dir) {
	
	vec3 light_delta = light.position - fs_in.frag_pos;
	
	// diffuse 
    vec3 light_dir = normalize(light_delta);
    float diff = max(dot(normal, light_dir), 0.0);
    vec3 diffuse = diff * (light.diffuse * material.diffuse_color);
    
    // specular
	vec3 specular = vec3(0.0f);
	if (diff > 0) {
		vec3 reflect_dir = reflect(-light_dir, normal);  
		float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
		specular = spec * (light.specular * material.specular_color);      
	}
    
    // attenuation
    float distance    = length(light_delta);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

    diffuse  *= attenuation;
    specular *= attenuation;   
    
    return (diffuse + specular)*diffuse_tex;
}
