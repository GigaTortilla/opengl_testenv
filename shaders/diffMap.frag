#version 330 core
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
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
in vec2 texCoords;

out vec4 FragColor;

uniform Light light;
uniform Material material;
uniform vec3 viewPos;

// time-dependent fun stuff
uniform float time;
  
void main()
{
    // ambient lighting
    vec3 ambient = light.ambient * texture(material.diffuse, texCoords).rgb;

    // diffuse lighting 
    vec3 normalNormal = normalize(normalVec);
    vec3 lightDirection = normalize(light.position - fragPos);
    float diffImpact = max(dot(normalNormal, lightDirection), 0.0);
    vec3 diffuse = light.diffuse * diffImpact * texture(material.diffuse, texCoords).rgb;

    // specular lighting 
    vec3 viewDirection = normalize(viewPos - fragPos);
    vec3 reflectDirection = reflect(-lightDirection, normalNormal);
    float specImpact = pow(max(dot(reflectDirection, viewDirection), 0.0), material.shininess);
    vec3 specular = light.specular * specImpact * texture(material.specular, texCoords).rgb;

    // emission map
    vec3 emission = vec3(0.0);
    if(texture(material.specular, texCoords).r == 0.0)
    {
        emission = texture(material.emission, texCoords).rgb;

        // time-dependent fun stuff
        emission = texture(material.emission, texCoords + vec2(time, 0.0)).rgb;
        emission = emission * (sin(2.5 * time) + 1.0);
    }

    vec3 result = ambient + diffuse + specular + emission;
    FragColor = vec4(result, 1.0);
}