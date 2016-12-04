#version 330 core

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;

in float Visibility;

layout(location = 0) out vec4 color;

uniform sampler2D texSampler0;
uniform sampler2D texSampler1;
uniform sampler2D texSampler2;
uniform sampler2D texSampler3;
uniform sampler2D texSampler4;
uniform sampler2D texSampler5;

void main()
{    
    float range_cutoffs[] = {-.5f,-0.1f, 1.f, 2.5f, 5.f, 20.f};
    
    // Textures------------------------------------------------------
    vec4 texColor;
    
    // if (FragPos.y <= range_cutoffs[0]){
        // texColor = texture(texSampler0, TexCoord);
        // color = vec4(texColor.xyz, 0.0f);
    // }
    if (FragPos.y <= range_cutoffs[1]) {
        float range = range_cutoffs[1] - range_cutoffs[0];
        float fraction = (FragPos.y - range_cutoffs[0])/range;
        texColor = vec4(texture(texSampler0, TexCoord).xyz, 0.f)*(1-fraction) + vec4(texture(texSampler1, TexCoord).xyz, 1.f)*fraction;
        color = vec4(texture(texSampler1, TexCoord).xyz, 1.f);
    }
    else if (FragPos.y <= range_cutoffs[2]) {
        float range = range_cutoffs[2] - range_cutoffs[1];
        float fraction = (FragPos.y - range_cutoffs[1])/range;
        texColor = texture(texSampler1, TexCoord)*(1-fraction) + texture(texSampler2, TexCoord)*fraction;
         color = vec4(texColor.xyz, 1.0f);
    }
    else if (FragPos.y <= range_cutoffs[3]) {
        float range = range_cutoffs[3] - range_cutoffs[2];
        float fraction = (FragPos.y - range_cutoffs[2])/range;
        texColor = texture(texSampler2, TexCoord)*(1-fraction) + texture(texSampler3, TexCoord)*fraction;
        color = vec4(texColor.xyz, 1.0f);
    }
    else if (FragPos.y <= range_cutoffs[4]) {
        float range = range_cutoffs[4] - range_cutoffs[3];
        float fraction = (FragPos.y - range_cutoffs[3])/range;
        texColor = texture(texSampler3, TexCoord)*(1-fraction) + texture(texSampler4, TexCoord)*fraction;
        color = vec4(texColor.xyz, 1.0f);

    }
        else if (FragPos.y <= range_cutoffs[5]) {
        float range = range_cutoffs[5] - range_cutoffs[4];
        float fraction = (FragPos.y - range_cutoffs[4])/range;
        texColor = texture(texSampler4, TexCoord)*(1-fraction) + texture(texSampler5, TexCoord)*fraction;
        color = vec4(texColor.xyz, 1.0f);
    }
    else {
        texColor = texture(texSampler5, TexCoord);
        color = vec4(texColor.xyz, 1.0f);
    }
    color = mix(vec4(0.4, 0.4, 0.4, 0.0), color, Visibility);
    
}
