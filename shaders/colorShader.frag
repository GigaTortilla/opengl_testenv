#version 330 core

in vec3 normalVec;
in vec3 fragPos;

out vec4 FragColor;  

uniform vec3 objectColor;
uniform vec3 lightColor;
uniform vec3 lightPos;
  
void main()
{
    float ambientStrength = 0.15;
    vec3 ambient = ambientStrength * lightColor;

    vec3 normalNormal = normalize(normalVec);
    vec3 lightDirection = normalize(lightPos - fragPos);
    float diffImpact = max(dot(normalNormal, lightDirection), 0.0);
    vec3 diffuse = diffImpact * lightColor;

    vec3 result = (ambient + diffuse) * objectColor;
    FragColor = vec4(result, 1.0);
}