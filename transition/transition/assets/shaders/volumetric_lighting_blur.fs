#version 330 core

// bilateral shader from https://github.com/SableRaf/Filters4Processing/blob/master/sketches/Bilateral/data/bilateral.glsl

#define SIGMA 10.0
#define BSIGMA 0.1
#define MSIZE 15
#define kSize ((MSIZE-1)/2)

const float kernel[MSIZE] = float[MSIZE](
0.031225216, 0.033322271, 0.035206333, 0.036826804, 0.038138565, 0.039104044, 0.039695028, 0.039894000, 0.039695028, 0.039104044, 0.038138565, 0.036826804, 0.035206333, 0.033322271, 0.031225216);


float normpdf(in float x, in float sigma) {
	return 0.39894*exp(-0.5*x*x/(sigma*sigma))/sigma;
}

float normpdf3(in vec3 v, in float sigma) {
	return 0.39894*exp(-0.5*dot(v,v)/(sigma*sigma))/sigma;
}

layout (location = 0) out vec4 FragColor;
layout (location = 1) out vec4 BrightColor;

in vec2 TexCoords;

uniform sampler2D scene_tex;
uniform sampler2D volumetric_tex;

void main() {
	vec3 c = texture(scene_tex, TexCoords).xyz + texture(volumetric_tex, TexCoords).xyz;
	FragColor = vec4(c, 1.0);
	
	//declare stuff
	/*vec2 screen_size = vec2(1600, 900); // TODO: put in uniform
	
	float Z = 0.0;
	vec3 final_color = vec3(0.0);
	vec3 cc;
	float factor;
	float bZ = 1.0/normpdf(0.0, BSIGMA);
	//read out the texels
	for (int i=-kSize; i <= kSize; ++i)
	{
		for (int j=-kSize; j <= kSize; ++j)
		{
			cc = texture2D(volumetric_tex, vec2(0.0, 0.0) + ( gl_FragCoord.xy + vec2(float(i),float(j)) ) / screen_size.xy ).rgb;
			factor = normpdf3(cc-c, BSIGMA)*bZ*kernel[kSize+j]*kernel[kSize+i];
			Z += factor;
			final_color += factor*cc;

		}
	}
	
	FragColor = vec4(final_color/Z, 1.0) + texture(scene_tex, TexCoords);*/
	
	
	float brightness = dot(FragColor.rgb, vec3(0.2126, 0.7152, 0.0722));
	if (brightness > 0.8) {
		BrightColor = FragColor;
	} else {
		BrightColor = vec4(0.0, 0.0, 0.0, 1.0);
	}
}