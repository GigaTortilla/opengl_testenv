#version 330 core
struct Material 
{
    sampler2D diffuse;
    sampler2D specular;
    sampler2D emission;
    float shininess;
};
struct DirLight 
{
    vec3 direction;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
struct PointLight 
{
    vec3 position;

    float constant;
    float linear;
    float quadratic;
    
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};
struct SpotLight 
{
    vec3 position;
    vec3 direction;
    float cutOff;
    float outerCutOff;

    float constant;
    float linear;
    float quadratic;

    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
};

in vec3 normalVec;
in vec3 fragPos;
in vec2 texCoords;

out vec4 FragColor;

uniform DirLight direcionalLight;
uniform PointLight pointLight;
uniform SpotLight spotlight;
uniform Material material;
uniform vec3 viewPos;

// time-dependent fun stuff
uniform float time;

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir);
vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragmentPos, vec3 viewDir);
vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragmentPos, vec3 viewDir);
  
void main()
{
    // emission map
    vec3 emission = vec3(0.0);
    if(texture(material.specular, texCoords).r == 0.0)
    {
        emission = texture(material.emission, texCoords).rgb;

        // time-dependent fun stuff
        // emission = texture(material.emission, texCoords + vec2(0.5 * time, 0.0)).rgb;
        emission = emission * vec3(0.1, 0.2 * sin(time) + 0.2, 0.0);
    }
    
    vec3 normalNormal = normalize(normalVec);
    vec3 viewDirection = normalize(viewPos - fragPos);
    
    vec3 result = calcDirLight(direcionalLight, normalNormal, viewDirection);
    result += calcPointLight(pointLight, normalNormal, fragPos, viewDirection);
    result += calcSpotLight(spotlight, normalNormal, fragPos, viewDirection);

    FragColor = vec4(result, 1.0);
}

vec3 calcDirLight(DirLight light, vec3 normal, vec3 viewDir)
{
    vec3 lightDir = normalize(-light.direction);

    float diffuseShading = max(dot(lightDir, normal), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float specularShading = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);

    vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoords));
    vec3 diffuse = light.diffuse * diffuseShading * vec3(texture(material.diffuse, texCoords));
    vec3 specular = light.specular * specularShading * vec3(texture(material.specular, texCoords));
    return vec3(ambient + diffuse + specular);
}

vec3 calcPointLight(PointLight light, vec3 normal, vec3 fragmentPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.position - fragmentPos);
    // diffuse shading
    float diffFactor = max(dot(normal, lightDir), 0.0);
    // specular shading
    vec3 reflectDir = reflect(-lightDir, normal);
    float specFactor = pow(max(dot(viewDir, reflectDir), 0.0), material.shininess);

    // attenuation
    float dist = length(light.position - fragmentPos);
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));   
    
    vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoords));
    vec3 diffuse = light.diffuse * diffFactor * vec3(texture(material.diffuse, texCoords));
    vec3 specular = light.specular * specFactor * vec3(texture(material.specular, texCoords));
    ambient *= attenuation;
    diffuse *= attenuation;
    specular *= attenuation;
    return (ambient + diffuse + specular);
}

vec3 calcSpotLight(SpotLight light, vec3 normal, vec3 fragmentPos, vec3 viewDir)
{
    vec3 lightDir = normalize(light.direction - fragmentPos);

    float diffuseShading = max(dot(lightDir, normal), 0.0);

    vec3 reflectDir = reflect(-lightDir, normal);
    float specularShading = pow(max(dot(reflectDir, viewDir), 0.0), material.shininess);
    
    // attenuation calculation for distant light sources
    float dist = length(light.position - fragmentPos);
    float attenuation = 1.0 / (light.constant + light.linear * dist + light.quadratic * (dist * dist));
    // Light intensity
    float cosTheta = dot(lightDir, normalize(-light.direction));
    float epsilon = light.cutOff - light.outerCutOff;
    float intensity = clamp((cosTheta - light.outerCutOff) / epsilon, 0.0, 1.0);

    vec3 ambient = light.ambient * vec3(texture(material.diffuse, texCoords));
    vec3 diffuse = light.diffuse * diffuseShading * vec3(texture(material.diffuse, texCoords));
    vec3 specular = light.specular * specularShading * vec3(texture(material.specular, texCoords));
    // attenuating light components
    ambient *= attenuation * intensity;
    diffuse *= attenuation * intensity;
    specular *= attenuation * intensity;
    return vec3(ambient + diffuse + specular);
}