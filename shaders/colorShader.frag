#version 330 core
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};

in vec3 normalVec;
in vec3 fragPos;
in vec3 viewLightPos;

out vec4 FragColor;

uniform vec3 lightColor;
uniform Material material;
  
void main()
{
    // ambient lighting
    vec3 ambient = lightColor * material.ambient;

    // diffuse lighting 
    vec3 normalNormal = normalize(normalVec);
    vec3 lightDirection = normalize(viewLightPos - fragPos);
    float diffImpact = max(dot(normalNormal, lightDirection), 0.0);
    vec3 diffuse = lightColor * (diffImpact * material.diffuse);

    // specular lighting 
    vec3 viewDirection = normalize(-fragPos);
    vec3 reflectDirection = reflect(-lightDirection, normalNormal);
    float specImpact = pow(max(dot(reflectDirection, viewDirection), 0.0), 32);
    vec3 specular = lightColor * (specImpact * material.specular);

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}