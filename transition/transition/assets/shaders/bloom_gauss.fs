#version 330 core
//Source: http://learnopengl.com/#!Advanced-Lighting/Bloom
out vec4 FragColor;
in vec2 TexCoords;

uniform sampler2D image;
uniform bool horizontal;

#define SIZE 8

//uniform float weight[SIZE] = float[] (0.2270270270, 0.1945945946, 0.1216216216, 0.0540540541, 0.0162162162);
uniform float weight[SIZE] = float[] (0.0779, 0.0772, 0.0752, 0.0719, 0.0676, 0.0624, 0.0565, 0.0504);

void main()
{             
     vec2 tex_offset = 1.0 / textureSize(image, 0); // gets size of single texel
	 tex_offset *= textureSize(image, 0).x / 1024;	// Auflösungskorrektur
     vec3 result = texture(image, TexCoords).rgb * weight[0];
     if(horizontal)
     {
         for(int i = 1; i < SIZE; ++i)
         {
            result += texture(image, TexCoords + vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
            result += texture(image, TexCoords - vec2(tex_offset.x * i, 0.0)).rgb * weight[i];
         }
     }
     else
     {
         for(int i = 1; i < SIZE; ++i)
         {
             result += texture(image, TexCoords + vec2(0.0, tex_offset.y * i)).rgb * weight[i];
             result += texture(image, TexCoords - vec2(0.0, tex_offset.y * i)).rgb * weight[i];
         }
     }
     FragColor = vec4(result, 1.0);
}