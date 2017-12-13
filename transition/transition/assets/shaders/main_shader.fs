#version 330 core
#define MAX_NR_LIGHTS (10)
#define MAX_NR_SHADOWS (5)

in VS_OUT {
    vec3 frag_pos;
    vec3 normal;
    vec2 tex_coords;
    vec4 frag_pos_lightspace[MAX_NR_SHADOWS];
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
	
	bool shadow_casting;
	int shadow_map_index;
};
uniform Light lights[MAX_NR_LIGHTS];
uniform sampler2D shadow_maps[MAX_NR_SHADOWS];
uniform int num_lights;

#define SHADOW_MAP(A,B,C,X) \
	if (B == 0) { \
		X = A(shadow_maps[0], C); \
	} else if (B == 1) { \
		X = A(shadow_maps[1], C); \
	} else if (B == 2) { \
		X = A(shadow_maps[2], C); \
	} else if (B == 3) { \
		X = A(shadow_maps[3], C); \
	} else if (B == 4) { \
		X = A(shadow_maps[4], C); \
	} 
	

struct Material {
	bool has_diffuse_tex;
	sampler2D diffuse_tex;
	sampler2D specular_tex;
	float shininess;
	
	vec3 ambient_color;
	vec3 diffuse_color;
	vec3 specular_color;
	
	// 0= regular
	// 1= debug depth render type
	int material_type;
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

vec3 render_type_debug_depth(vec3 diffuse_tex);

float shadow_calculation(Light light);

void main() {
	vec3 diffuse_tex;
	if (material.has_diffuse_tex) {
		diffuse_tex = vec3(texture(material.diffuse_tex, fs_in.tex_coords));
	} else {
		diffuse_tex = vec3(1,1,1);
	}
	
	switch (material.material_type) {
		case 0: break; // do not touch the color
		case 1:
			diffuse_tex = render_type_debug_depth(diffuse_tex);
			break;
		default:
			diffuse_tex = vec3(1.0, 0.0, 0.0);
	}

	vec3 color = material.ambient_color * diffuse_tex;
	vec3 normal = normalize(fs_in.normal);
    vec3 view_dir = normalize(view_pos - fs_in.frag_pos);
	
	for (int i = 0; i < num_lights; i++) {
		float shadow = 0.0;
		
		if (lights[i].shadow_casting) {
			shadow = shadow_calculation(lights[i]);
		}
		
		vec3 add_color = vec3(0.0, 0.0, 0.0);
		switch (lights[i].light_type) {
		case 1:
			add_color = calc_dir_light(
				lights[i], 
				diffuse_tex, 
				normal, 
				view_dir
			);
			break;
		case 2:
			add_color = calc_point_light(
				lights[i], 
				diffuse_tex, 
				normal, 
				view_dir
			);
			break;
		default:
			color = vec3(0.0,1.0,0.0);
		}
		color += (1.0 - shadow)*add_color;
	}
	
	FragColor = vec4(color, 1.0f);
}

vec3 render_type_debug_depth(vec3 diffuse_tex) {
	float depth_value = diffuse_tex.r;
	float near_plane = 1.0;
	float far_plane = 20.0;
	float z = depth_value * 2.0 - 1.0;
	return vec3(vec3(((2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane))) / far_plane));
}

vec3 calc_dir_light(
	Light light, 
	vec3 diffuse_tex, 
	vec3 normal, 
	vec3 view_dir) {
	
	vec3 light_dir = normalize(light.direction);
	
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

float shadow_calculation(Light light) {
	vec4 frag_pos_lightspace = fs_in.frag_pos_lightspace[light.shadow_map_index];
	
    // perform perspective divide
    vec3 proj_coords = frag_pos_lightspace.xyz / frag_pos_lightspace.w;
    
	// transform to [0,1] range
    proj_coords = proj_coords * 0.5 + 0.5;
    
	// get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
	vec4 closest_depth;
	SHADOW_MAP(texture, light.shadow_map_index, proj_coords.xy, closest_depth)
    
	// get depth of current fragment from light's perspective
    float current_depth = proj_coords.z;
    
	// check whether current frag pos is in shadow
    return current_depth > closest_depth.r ? 1.0 : 0.0;
}
