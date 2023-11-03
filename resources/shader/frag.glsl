#version 460 core

in vec2 TexCoord;
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform sampler2D samp;
uniform vec3 lightColor;
uniform vec3 lightPos;
uniform vec3 viewPos;

void main(void) {
    vec4 texColor = texture(samp, TexCoord);
    float ambientStrength = 0.1;
    vec3 ambeint = ambientStrength * lightColor;

    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);

    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * lightColor;

    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);

    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 16);
    vec3 specular = specularStrength * spec * lightColor;
    
    vec3 result = (ambeint + diffuse + specular) * vec3(texColor.xyz);
    FragColor = vec4(result, 1.0);
}