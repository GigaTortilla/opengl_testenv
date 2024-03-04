#version 330 core
struct Material {
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    float shininess;
};
struct Light {
    vec3 position;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 normalVec;
in vec3 fragPos;

out vec4 FragColor;

uniform Light light;
uniform Material material;
uniform vec3 viewPos;
  
void main()
{
    // ambient lighting
    vec3 ambient = light.ambient * material.ambient;

    // diffuse lighting 
    vec3 normalNormal = normalize(normalVec);
    vec3 lightDirection = normalize(light.position - fragPos);
    float diffImpact = max(dot(normalNormal, lightDirection), 0.0);
    vec3 diffuse = light.diffuse * (diffImpact * material.diffuse);

    // specular lighting 
    vec3 viewDirection = normalize(viewPos - fragPos);
    vec3 reflectDirection = reflect(-lightDirection, normalNormal);
    float specImpact = pow(max(dot(reflectDirection, viewDirection), 0.0), material.shininess);
    vec3 specular = light.specular * (specImpact * material.specular);

    vec3 result = ambient + diffuse + specular;
    FragColor = vec4(result, 1.0);
}