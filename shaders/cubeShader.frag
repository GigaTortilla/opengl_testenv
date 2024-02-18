#version 330 core
in vec2 textureCoords;

out vec4 FragColor;  

uniform sampler2D texture1;
uniform sampler2D texture2;
uniform float blend;
  
void main()
{
    FragColor = mix(texture(texture1, vec2(textureCoords.s, textureCoords.t)), texture(texture2, vec2(1.0 - 2.0 * textureCoords.s, 2.0 * textureCoords.t)), blend);
}