#version 330 core
#define MAX_NR_LIGHTS (10)
#define MAX_NR_DIRECTIONAL_SHADOWS (5)
#define MAX_NR_OMNI_DIRECTIONAL_SHADOWS (5)
#define PCF_TOTAL_SAMPLES (25)
#define PCF_COUNT (2)
#define PCF_OMNI_DIRECTIONAL_SAMPLES (20)

in VS_OUT {
    vec3 frag_pos;
    vec3 normal;
    vec2 tex_coords;
    vec4 frag_pos_lightspace[MAX_NR_DIRECTIONAL_SHADOWS];
} fs_in;

layout (location = 0) out vec4 FragColor;

struct MVP {
	mat4 model;
	mat4 view;
	mat4 projection;	
	
	mat3 model_normal;
};
uniform MVP mvp;

struct Light {
	// light_type=1: directional light
	// light_type=2: point light
	// light_type=3: spot light
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
	float min_bias;
	float max_bias;
	
	// Spotlight
	float cutoff;
	float outer_cutoff;
	
	// point light
	float far_plane;
	float near_plane;
};
uniform Light lights[MAX_NR_LIGHTS];
uniform sampler2D directional_shadow_maps[MAX_NR_DIRECTIONAL_SHADOWS];
uniform samplerCube omni_directional_shadow_maps[MAX_NR_OMNI_DIRECTIONAL_SHADOWS];
uniform int num_lights;
uniform mat4 light_space_matrices[MAX_NR_DIRECTIONAL_SHADOWS];
uniform mat4 light_view_matrices[MAX_NR_DIRECTIONAL_SHADOWS];
uniform mat4 light_projection_matrices[MAX_NR_DIRECTIONAL_SHADOWS];

#define DIRECTIONAL_SHADOW_MAP(A,B,C,X) \
	if (B == 0) { \
		X = A(directional_shadow_maps[0], C); \
	} else if (B == 1) { \
		X = A(directional_shadow_maps[1], C); \
	} else if (B == 2) { \
		X = A(directional_shadow_maps[2], C); \
	} else if (B == 3) { \
		X = A(directional_shadow_maps[3], C); \
	} else if (B == 4) { \
		X = A(directional_shadow_maps[4], C); \
	} 

#define OMNI_DIRECTIONAL_SHADOW_MAP(A,B,C,X) \
	if (B == 0) { \
		X = A(omni_directional_shadow_maps[0], C); \
	} else if (B == 1) { \
		X = A(omni_directional_shadow_maps[1], C); \
	} else if (B == 2) { \
		X = A(omni_directional_shadow_maps[2], C); \
	} else if (B == 3) { \
		X = A(omni_directional_shadow_maps[3], C); \
	} else if (B == 4) { \
		X = A(omni_directional_shadow_maps[4], C); \
	} 

struct Material {
	bool has_diffuse_tex;
	sampler2D diffuse_tex;
	sampler2D specular_tex;
	bool has_alpha_tex;
	sampler2D alpha_tex;
	float shininess;
	float opacity;
	
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
	vec3 view_dir,
	out float bias
);

vec3 calc_point_light(
	Light light, 
	vec3 diffuse_material, 
	vec3 normal, 
	vec3 view_dir,
	out float bias
);

vec3 calc_spot_light(
	Light light, 
	vec3 diffuse_tex, 
	vec3 normal, 
	vec3 view_dir,
	out float bias
);

vec3 render_type_debug_depth(vec3 diffuse_tex);

float shadow_calculation_directional(Light light, float bias);
float shadow_calculation_omni_directional(Light light, float bias, vec3 view_delta);

vec3 sample_offset_directions[20] = vec3[]
(
   vec3( 1,  1,  1), vec3( 1, -1,  1), vec3(-1, -1,  1), vec3(-1,  1,  1), 
   vec3( 1,  1, -1), vec3( 1, -1, -1), vec3(-1, -1, -1), vec3(-1,  1, -1),
   vec3( 1,  1,  0), vec3( 1, -1,  0), vec3(-1, -1,  0), vec3(-1,  1,  0),
   vec3( 1,  0,  1), vec3(-1,  0,  1), vec3( 1,  0, -1), vec3(-1,  0, -1),
   vec3( 0,  1,  1), vec3( 0, -1,  1), vec3( 0, -1, -1), vec3( 0,  1, -1)
);

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
	vec3 view_delta = view_pos - fs_in.frag_pos;
    vec3 view_dir = normalize(view_delta);
	
	for (int i = 0; i < num_lights; i++) {
		float shadow = 0.0, bias = 0.0;
		
		vec3 add_color = vec3(0.0, 0.0, 0.0);
		switch (lights[i].light_type) {
		case 1:
			add_color = calc_dir_light(
				lights[i], 
				diffuse_tex, 
				normal, 
				view_dir,
				bias
			);
			break;
		case 2:
			add_color = calc_point_light(
				lights[i], 
				diffuse_tex, 
				normal, 
				view_dir,
				bias
			);
			break;
		case 3:
			add_color = calc_spot_light(
				lights[i], 
				diffuse_tex, 
				normal, 
				view_dir,
				bias
			);
			break;
		default:
			color = vec3(0.0,1.0,0.0);
		}
		
		if (lights[i].shadow_casting) {
			switch (lights[i].light_type) {
			case 1:
			case 3:
				shadow = shadow_calculation_directional(lights[i], bias);
				break;
			case 2:
				shadow = shadow_calculation_omni_directional(lights[i], bias, view_delta);
				break;
			}
		}
		
		color += (1.0 - shadow)*add_color;
	}
	float alpha = material.opacity;
	if (material.has_alpha_tex) {
		alpha = alpha * texture(material.alpha_tex, fs_in.tex_coords).r;
	}
	if (alpha < 0.01) {
		alpha = 0;
		gl_FragDepth = 100;
	} else {
		gl_FragDepth = gl_FragCoord.z;
	}
	FragColor = vec4(color, alpha);
}

#define DEBUG_PERSPECTIVE_DEPTH

vec3 render_type_debug_depth(vec3 diffuse_tex) {
	float depth_value = diffuse_tex.r;
	float near_plane = 1.0;
	float far_plane = 100.0;

#ifdef DEBUG_PERSPECTIVE_DEPTH
	float z = depth_value * 2.0 - 1.0; // Back to NDC 
    return vec3((2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane)));	
#else
	float z = depth_value * 2.0 - 1.0;
	return vec3(((2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane))) / far_plane);
#endif
}

vec3 calc_dir_light(
	Light light, 
	vec3 diffuse_tex, 
	vec3 normal, 
	vec3 view_dir,
	out float bias) {
	
	vec3 light_dir = normalize(-light.direction);
	bias = max(light.max_bias * (1.0 - dot(normal, light_dir)), light.min_bias);  
	
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
	vec3 view_dir,
	out float bias) {
	
	vec3 light_delta = light.position - fs_in.frag_pos;
	
	// diffuse 
    vec3 light_dir = normalize(light_delta);
	bias = max(light.max_bias * (1.0 - dot(normal, light_dir)), light.min_bias);  
	
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

vec3 calc_spot_light(
	Light light, 
	vec3 diffuse_tex, 
	vec3 normal,
	vec3 view_dir,
	out float bias) {
	
	vec3 light_delta = light.position - fs_in.frag_pos;
	
	// diffuse 
    vec3 light_dir = normalize(light_delta);
	bias = max(light.max_bias * (1.0 - dot(normal, light_dir)), light.min_bias);  
	
    float diff = max(dot(normal, light_dir), 0.0);
    vec3 diffuse = diff * (light.diffuse * material.diffuse_color);
    
    // specular
	vec3 specular = vec3(0.0f);
	if (diff > 0) {
		vec3 reflect_dir = reflect(-light_dir, normal);  
		float spec = pow(max(dot(view_dir, reflect_dir), 0.0), material.shininess);
		specular = spec * (light.specular * material.specular_color);      
	}
    
	// spotlight (soft edges)
    float theta = dot(light_dir, normalize(-light.direction)); 
    float epsilon = (light.cutoff - light.outer_cutoff);
    float intensity = clamp((theta - light.outer_cutoff) / epsilon, 0.0, 1.0);
	
    // attenuation
    float distance    = length(light_delta);
    float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

    diffuse  *= attenuation*intensity;
    specular *= attenuation*intensity;
	
    return (diffuse + specular)*diffuse_tex;
}

float shadow_calculation_directional(Light light, float bias) {
	vec4 frag_pos_lightspace = fs_in.frag_pos_lightspace[light.shadow_map_index];
	
    // perform perspective divide
    vec3 proj_coords = frag_pos_lightspace.xyz / frag_pos_lightspace.w;
	if(proj_coords.z > 1.0) {
		return 0.0;
	}
    
	// transform to [0,1] range
    proj_coords = proj_coords * 0.5 + 0.5;
    
	float current_depth = proj_coords.z - bias;
	
	float shadow = 0.0;
	vec2 texel_size;
	DIRECTIONAL_SHADOW_MAP(textureSize, light.shadow_map_index, 0, texel_size)
	texel_size = 1.0 / texel_size;
	
	for(int x = -PCF_COUNT; x <= PCF_COUNT; ++x) {
		for(int y = -PCF_COUNT; y <= PCF_COUNT; ++y) {
			// get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
			vec4 closest_depth;
			DIRECTIONAL_SHADOW_MAP(texture, light.shadow_map_index, (proj_coords.xy + vec2(x, y) * texel_size), closest_depth)
			
			shadow += current_depth - bias > closest_depth.r ? 1.0 : 0.0;        
		}    
	}
	return shadow / PCF_TOTAL_SAMPLES;
	
}

float shadow_calculation_omni_directional(Light light, float bias, vec3 view_delta) {
	// get vector between fragment position and light position
    vec3 frag_to_light = fs_in.frag_pos - light.position;
	
    // now get current linear depth as the length between the fragment and light position
    float current_depth = length(frag_to_light);
	
    // test for shadows
	float shadow = 0.0;
	float view_distance = length(view_pos - fs_in.frag_pos);
	float disk_radius = (1.0 + (view_distance / light.far_plane)) / 20.0;
	for(int i = 0; i < PCF_OMNI_DIRECTIONAL_SAMPLES; ++i) {
		 // fragment to light vector to sample from the depth map    
		vec4 closest_depth;
		OMNI_DIRECTIONAL_SHADOW_MAP(texture, light.shadow_map_index, (frag_to_light + sample_offset_directions[i] * disk_radius), closest_depth)

		// it is currently in linear range between [0,1], let's re-transform it back to original depth value
		closest_depth.r *= light.far_plane;
		
		if(current_depth - bias	> closest_depth.r) {
			shadow += 1.0;
		}
	}
	return shadow / float(PCF_OMNI_DIRECTIONAL_SAMPLES);
}
