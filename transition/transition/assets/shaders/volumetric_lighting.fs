#version 330 core

#define MAX_NR_LIGHTS (10)
#define MAX_NR_DIRECTIONAL_SHADOWS (5)
#define MAX_NR_OMNI_DIRECTIONAL_SHADOWS (5)
#define SHADOW_BIAS_MAX (0.001)
#define SHADOW_BIAS_MIN (0.0001)
#define PCF_TOTAL_SAMPLES (25)
#define PCF_COUNT (2)
#define PCF_OMNI_DIRECTIONAL_SAMPLES (20)

in VS_OUT {
    vec3 frag_pos;
} fs_in;

layout (location = 0) out vec4 VolumetricColor;

struct Light {
	// light_type=1: directional light
	// light_type=2: point light
	// light_type=3: spot light
	int light_type;
	bool volumetric;
	
	vec3 position;
    vec3 direction;
  
	float constant;
    float linear;
    float quadratic;
  
    vec3 diffuse;
    vec3 specular;
	
	bool shadow_casting;
	int shadow_map_index;
	
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

uniform vec3 view_pos;
uniform sampler2D blue_noise_texture;
uniform int seed;

float volumetric_lighting(Light light, float bias);


void main() {
	vec3 vol_color = vec3(0);
	
	for (int i = 0; i < num_lights; i++) {
		if (lights[i].volumetric) {
			float bias = 0.0; // TODO
			
			// TODO: different implementations for point light
			vol_color += volumetric_lighting(lights[i], bias)*lights[i].diffuse;
		}
	}
	
	VolumetricColor = vec4(vol_color, gl_FragDepth);
}

float dither_pattern[16] = float[16] (
	0.0f, 0.5f, 0.125f, 0.625f,
	0.75f, 0.22f, 0.875f, 0.375f,
	0.1875f, 0.6875f, 0.0625f, 0.5625,
	0.9375f, 0.4375f, 0.8125f, 0.3125
);

#define TAU (0.000001)
#define PHI (50000000.0)
#define PI_RCP (0.31830988618379067153776752674503)
#define NUM_STEPS (12)

float volumetric_lighting(Light light, float bias) {
	//vec2 screen_pos = vec2(mod(gl_FragCoord.x, 512), mod(gl_FragCoord.y, 512)) / 512.0;
	//float dither_value = 0.000001*texture(blue_noise_texture, screen_pos).r;
	
	float dither_value = dither_pattern[ (int(gl_FragCoord.x) % 4)* 4 + (int(gl_FragCoord.y) % 4) ];
	
	vec4 end_pos_worldspace  = vec4(view_pos, 1.0);
	vec4 start_pos_worldspace = vec4(fs_in.frag_pos, 1.0);
	vec4 delta_worldspace = normalize(end_pos_worldspace - start_pos_worldspace);
	
	vec4 end_pos_lightview = light_view_matrices[light.shadow_map_index] * end_pos_worldspace;
	vec4 start_pos_lightview = light_view_matrices[light.shadow_map_index] * start_pos_worldspace;
	vec4 delta_lightview = normalize(end_pos_lightview - start_pos_lightview);
	
	float raymarch_distance_lightview = length(end_pos_lightview - start_pos_lightview);
	float step_size_lightview = raymarch_distance_lightview / NUM_STEPS;
	
	float raymarch_distance_worldspace = length(end_pos_worldspace - start_pos_worldspace);
	float step_size_worldspace = raymarch_distance_worldspace / NUM_STEPS;
	
	vec4 ray_position_lightview = start_pos_lightview + dither_value*step_size_lightview * delta_lightview;
	vec4 ray_position_worldspace = start_pos_worldspace + dither_value*step_size_worldspace * delta_worldspace;
	
	float light_contribution = 0.0;
	float epsilon = (light.cutoff - light.outer_cutoff);
	for (float l = raymarch_distance_lightview; l > step_size_lightview; l -= step_size_lightview) {
		
		vec4 ray_position_lightspace = light_projection_matrices[light.shadow_map_index] * vec4(ray_position_lightview.xyz, 1);
		// perform perspective divide            
		vec3 proj_coords = ray_position_lightspace.xyz / ray_position_lightspace.w;
		
		// transform to [0,1] range
		proj_coords = proj_coords * 0.5 + 0.5;		
		
		// get closest depth value from light's perspective (using [0,1] range fragPosLight as coords)
		vec4 closest_depth;
		DIRECTIONAL_SHADOW_MAP(texture, light.shadow_map_index, proj_coords.xy, closest_depth)
		
		float shadow_term = 1.0;
		
		if (proj_coords.z - bias > closest_depth.r) {
			shadow_term = 0.0;
		}
		
		float d = length(ray_position_lightview.xyz);
		float d_rcp = 1.0/d;
		
		vec3 light_delta = light.position - ray_position_worldspace.xyz;
		float distance    = length(light_delta);
		float intensity = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    
		
		vec3 light_dir = normalize(light_delta);
		// spotlight (soft edges)
		float theta = dot(light_dir, normalize(-light.direction)); 
		intensity *= clamp((theta - light.outer_cutoff) / epsilon, 0.0, 1.0);
		
		light_contribution += intensity * TAU * (shadow_term * (PHI * 0.25 * PI_RCP) * d_rcp * d_rcp ) * exp(-d*TAU)*exp(-l*TAU) * step_size_lightview;
	
		ray_position_lightview += step_size_lightview * delta_lightview;
		ray_position_worldspace += step_size_worldspace * delta_worldspace;
	}
	
	return light_contribution;
}