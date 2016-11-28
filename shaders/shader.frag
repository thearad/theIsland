#version 330 core

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

out vec3 color;

uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;

void main()
{    
    vec4 reflectColor = texture(reflectionTexture, TexCoord);
    vec4 refractColor = texture(refractionTexture, TexCoord);
    
    // color = vec4(0.f, 0.f, 1.f, 0.5f);
    // color = mix(reflectColor, refractColor, 0.5f);
    color = reflectColor;
}
