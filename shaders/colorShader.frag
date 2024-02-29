#version 330 core

out vec4 FragColor;  

uniform vec3 objectColor;
uniform vec3 lightColor;
  
void main()
{
    float ambientStrength = 0.15;

    vec3 result = ambientStrength * lightColor * objectColor;
    FragColor = vec4(result, 1.0);
}