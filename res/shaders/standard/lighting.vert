#version 330 core
/*
    -   =   Layout Locations    =   -
    
    0 -> Vertex position  ( MESH_3D / MESH_2D )
    1 -> Normal position  ( MESH_3D_NORMAL )
    2 -> Texture position ( MESH_3D_TEXTURED / MESH_3D_TEXTURED_NORMAL )
*/

layout (location = 0) in vec3 pos;
layout (location = 1) in vec3 normal;
layout (location = 2) in vec2 texcoords; 

// Uniforms parsed by the renderer
uniform mat4 model;
uniform mat4 view;
uniform mat4 projection;

out vec3 Normal;
out vec3 FragPos;
out vec2 TexCoords;

void main() {
    FragPos = vec3(model * vec4(pos, 1.0));
    gl_Position =  projection * view * model * vec4(pos, 1.0);
    Normal = normal;
    TexCoords = texcoords;
}
