#version 330 core
layout (location = 0) in vec3 position;
layout (location = 1) in vec2 texCoords;

uniform sampler2D depth_tex;

out vec2 TexCoordsLeftTop;
out vec2 TexCoordsRightTop;
out vec2 TexCoordsLeftBottom;
out vec2 TexCoordsRightBottom;

void main()
{
	vec2 texel_size = 1.0 / textureSize(depth_tex, 0);
	
    TexCoordsLeftTop = texCoords - 0.5 * texel_size;
    TexCoordsRightTop = TexCoordsLeftTop + vec2(texel_size.x, 0);
	TexCoordsLeftBottom = TexCoordsLeftTop + vec2(0, texel_size.y);
	TexCoordsRightBottom = TexCoordsLeftTop + texel_size;
	
    gl_Position = vec4(position, 1.0f);
}