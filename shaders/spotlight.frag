#version 330 core
struct Material {
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};
struct Light {
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;

    float constant;
    float linear;
    float quadratic;
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
    // attenuation calculation for distant light sources
    float dist = length(light.position - fragPos);
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));

    // ambient lighting
    vec3 ambient = light.ambient * texture(material.diffuse, texCoords).rgb;

    // emission map
    vec3 emission = vec3(0.0);
    if(texture(material.specular, texCoords).r == 0.0)
    {
        emission = texture(material.emission, texCoords).rgb;

        // time-dependent fun stuff
        // emission = texture(material.emission, texCoords + vec2(0.5 * time, 0.0)).rgb;
        emission = emission * vec3(0.1, 0.2 * sin(time) + 0.2, 0.0);
    }
    
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
    
    // Light intensity
    float cosTheta = dot(lightDirection, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((cosTheta - light.outerCutOff) / epsilon, 0.0, 1.0);
    diffuse *= intensity;
    specular *= intensity;

    // attenuating light components
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;

    vec3 result = ambient + diffuse + specular + emission;
    
    FragColor = vec4(result, 1.0);
}