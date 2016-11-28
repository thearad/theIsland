#version 330 core

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;
in vec4 clipSpace;

out vec4 color;

uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;

uniform int quad_width;
uniform int quad_height;

void main()
{    
    vec2 ndc = (clipSpace.xy/clipSpace.w)/2.0 + 0.5;
    float x = ndc.x;
    float y = ndc.y;
    vec2 refractionTexCoords = vec2(x, y);
    vec2 reflectionTexCoords = vec2(x, -y);
    
    vec4 reflectColor = texture(reflectionTexture, reflectionTexCoords);
    vec4 refractColor = texture(refractionTexture, refractionTexCoords);
    
    color = mix(reflectColor, refractColor, 0.5f);
    // color = reflectColor;
    // color = refractColor;
}
