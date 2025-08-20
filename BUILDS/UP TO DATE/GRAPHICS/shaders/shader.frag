#version 330 core
in vec3 _Color;
in vec3 Normal;
in vec3 FragPos;

out vec4 FragColor;

uniform vec3 lightPos = vec3(2.0, 2.0, 2.0);
uniform vec3 viewPos;

void main()
{
    // Ambient
    float ambientStrength = 0.45;
    vec3 ambient = ambientStrength * _Color;
    
    // Diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(lightPos - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = diff * _Color;
    
    // Simple specular (optional)
    float specularStrength = 0.5;
    vec3 viewDir = normalize(viewPos - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);
    float spec = pow(max(dot(viewDir, reflectDir), 0.0), 32);
    vec3 specular = specularStrength * spec * vec3(1.0);
    
    FragColor = vec4(ambient + diffuse + specular, 1.0);
}