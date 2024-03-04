#version 330 core

in vec3 normalVec;
in vec3 fragPos;
in vec3 viewLightPos;

out vec4 FragColor;

uniform vec3 objectColor;
uniform vec3 lightColor;
  
void main()
{
    float ambientStrength = 0.1;
    vec3 ambient = ambientStrength * lightColor;

    vec3 normalNormal = normalize(normalVec);
    vec3 lightDirection = normalize(viewLightPos - fragPos);
    float diffImpact = max(dot(normalNormal, lightDirection), 0.0);
    vec3 diffuse = diffImpact * lightColor;

    float specStrength = 0.5;
    vec3 viewDirection = normalize(-fragPos);
    vec3 reflectDirection = reflect(-lightDirection, normalNormal);
    float specImpact = pow(max(dot(reflectDirection, viewDirection), 0.0), 32);
    vec3 specular = specStrength * specImpact * lightColor;

    vec3 result = (ambient + diffuse + specular) * objectColor;
    FragColor = vec4(result, 1.0);
}