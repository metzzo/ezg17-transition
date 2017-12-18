#version 330 core
layout (triangles) in;
layout (triangle_strip, max_vertices=18) out;

uniform mat4 shadow_transform[6];

out vec4 FragPos;

void main() {
    for(int face = 0; face < 6; ++face) {
        gl_Layer = face;
        for(int i = 0; i < 3; ++i) {// for each triangle's vertices
            FragPos = gl_in[i].gl_Position;
            gl_Position = shadow_transform[face] * FragPos;
            EmitVertex();
        }    
        EndPrimitive();
    }
} 