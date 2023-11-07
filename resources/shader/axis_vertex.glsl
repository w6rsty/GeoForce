#version 460 core

layout (location = 0) in vec3 position;

uniform mat4 proj_matrix;
uniform mat4 view_matrix;

out vec3 FragPos;

void main(void)
{
    gl_Position = proj_matrix * view_matrix * vec4(position, 1.0);
    FragPos = position;
}
