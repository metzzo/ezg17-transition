#version 330 core

#define UPSAMPLE_DEPTH_THRESHOLD (0.5)

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in vec2 TexCoordsCenter;
in vec2 TexCoordsLeftTop;
in vec2 TexCoordsRightTop;
in vec2 TexCoordsLeftBottom;
in vec2 TexCoordsRightBottom;

float linear_eye_depth(float depth_value);

uniform sampler2D scene_tex;
uniform sampler2D volumetric_tex;

vec4 bilateral_blur(vec2 direction, vec2 texel_size, int kernel_radius);

void main() {
	vec4 scene_color = texture(scene_tex, TexCoordsCenter);
	
	vec4 volumetric_color;
	
	vec4 volumetric_tex_left_top = texture(volumetric_tex, TexCoordsLeftTop);
	vec4 volumetric_tex_right_top = texture(volumetric_tex, TexCoordsRightTop);
	vec4 volumetric_tex_left_bottom = texture(volumetric_tex, TexCoordsLeftBottom);
	vec4 volumetric_tex_right_bottom = texture(volumetric_tex, TexCoordsRightBottom);
	
	vec4 high_res_depth = vec4(linear_eye_depth(scene_color.w));
	vec4 low_res_depth = vec4(
		linear_eye_depth(volumetric_tex_left_top.w),
		linear_eye_depth(volumetric_tex_right_top.w),
		linear_eye_depth(volumetric_tex_left_bottom.w),
		linear_eye_depth(volumetric_tex_right_bottom.w)
	);
	
	vec4 depth_diff = abs(low_res_depth - high_res_depth);
	float accum_diff = dot(depth_diff, vec4(1));
	
	if (accum_diff < UPSAMPLE_DEPTH_THRESHOLD) {
		volumetric_color = texture(volumetric_tex, TexCoordsCenter);
	} else {
		float min_depth_diff = depth_diff[0];
		volumetric_color = volumetric_tex_left_top;
		
		if (depth_diff[1] < min_depth_diff) {
			min_depth_diff = depth_diff[1];
			volumetric_color = volumetric_tex_right_top;
		}
		
		if (depth_diff[2] < min_depth_diff) {
			min_depth_diff = depth_diff[2];
			volumetric_color = volumetric_tex_left_bottom;
		}
		
		if (depth_diff[3] < min_depth_diff) {
			min_depth_diff = depth_diff[3];
			volumetric_color = volumetric_tex_right_bottom;
		}
	}
	
	FragColor = volumetric_color + scene_color;
	
	float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
	if (brightness > 0.8) {
		BrightColor = FragColor;
	} else {
		BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
	}
}

float linear_eye_depth(float depth_value) {
	float near_plane = 0.1; // TODO as uniform
	float far_plane = 100.0; // TODO as uniform

	float z = depth_value * 2.0 - 1.0; // Back to NDC 
    return (2.0 * near_plane * far_plane) / (far_plane + near_plane - z * (far_plane - near_plane));	
}
