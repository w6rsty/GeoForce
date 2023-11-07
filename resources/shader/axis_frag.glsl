#version 460 core

in vec3 FragPos;

out vec4 FragColor;

void main(void)
{
    FragColor = vec4(abs(FragPos.x * 100), abs(FragPos.y * 100), abs(FragPos.z * 100), 1.0);
}
