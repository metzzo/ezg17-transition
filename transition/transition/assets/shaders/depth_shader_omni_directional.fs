#version 330 core
in vec4 FragPos;

uniform vec3 light_pos;
uniform float far_plane;

void main()
{
    float light_distance = length(FragPos.xyz - light_pos);
    
    // map to [0;1] range by dividing by far_plane
    light_distance = light_distance / far_plane;
    
    // write this as modified depth
    gl_FragDepth = light_distance;
}