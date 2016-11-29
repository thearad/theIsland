#version 330 core

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoords;
in vec4 clipSpace;
in vec3 toCameraVec;

in float Visibility;

out vec4 color;

uniform sampler2D reflectionTexture;
uniform sampler2D refractionTexture;
uniform sampler2D dudv;

uniform float moveFactor;

const float waveStr = 0.02;

void main()
{    
    vec2 ndc = (clipSpace.xy/clipSpace.w)/2.0 + 0.5;
    float x = ndc.x;
    float y = ndc.y;
    vec2 refractionTexCoords = vec2(x, y);
    vec2 reflectionTexCoords = vec2(x, -y);
    
    vec2 distortion1 = texture(dudv, vec2(TexCoords.x + moveFactor, TexCoords.y)).rg * 2.0 - 1.0;
    vec2 distortion2 = texture(dudv, vec2(-TexCoords.x + moveFactor, TexCoords.y + moveFactor)).rg * 2.0 - 1.0;
    vec2 totalDistortion = distortion1 + distortion2;
    totalDistortion *= waveStr;
    reflectionTexCoords+=totalDistortion;
    refractionTexCoords+=totalDistortion;
    
    refractionTexCoords = clamp(refractionTexCoords, 0.001, 0.999);
    reflectionTexCoords.x = clamp(reflectionTexCoords.x, 0.001, 0.999);
    reflectionTexCoords.y = clamp(reflectionTexCoords.y, -0.999, -0.001);

    vec4 reflectColor = texture(reflectionTexture, reflectionTexCoords);
    vec4 refractColor = texture(refractionTexture, refractionTexCoords);
    
    
    vec3 viewVector = normalize(toCameraVec);
    float refractiveFactor = dot(viewVector, vec3(0.0, 1.0, 0.0));
    refractiveFactor = pow(refractiveFactor, 0.8);
    
    color = refractColor;
    // color = reflectColor;
    color = mix(reflectColor, refractColor, 0.3);
    color = mix(color, vec4(0.0, 0.3, 0.5, 1.0), 0.2);
    color = mix(vec4(0.4, 0.4, 0.4, 0.0), color, Visibility);
}
