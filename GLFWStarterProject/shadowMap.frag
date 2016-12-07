#version 330 core

in vec3 Normal;
in vec3 FragPos;
in vec2 TexCoord;
in vec3 normal_cam;
in vec3 eyeDir;
in vec3 lightDir;
in vec4 shadowCoord;

in float Visibility;

layout(location = 0) out vec4 color;

uniform sampler2DShadow shadowMap;
uniform sampler2D texSampler0;
uniform sampler2D texSampler1;
uniform sampler2D texSampler2;
uniform sampler2D texSampler3;
uniform sampler2D texSampler4;
uniform sampler2D texSampler5;

	vec2 poissonDisk[16] = vec2[]( 
   vec2( -0.94201624, -0.39906216 ), 
   vec2( 0.94558609, -0.76890725 ), 
   vec2( -0.094184101, -0.92938870 ), 
   vec2( 0.34495938, 0.29387760 ), 
   vec2( -0.91588581, 0.45771432 ), 
   vec2( -0.81544232, -0.87912464 ), 
   vec2( -0.38277543, 0.27676845 ), 
   vec2( 0.97484398, 0.75648379 ), 
   vec2( 0.44323325, -0.97511554 ), 
   vec2( 0.53742981, -0.47373420 ), 
   vec2( -0.26496911, -0.41893023 ), 
   vec2( 0.79197514, 0.19090188 ), 
   vec2( -0.24188840, 0.99706507 ), 
   vec2( -0.81409955, 0.91437590 ), 
   vec2( 0.19984126, 0.78641367 ), 
   vec2( 0.14383161, -0.14100790 ) 
);

void main()
{    
    float range_cutoffs[] = {-.5f,-0.1f, 1.f, 2.5f, 5.f, 20.f};
    
	// Shadows------------------------------------------------------
	vec3 diffuse, ambient;
	vec3 specular = vec3(0.4,0.4,0.4);
	vec3 lightColor = vec3(1,1,1);
	float lightPower = 1.0f;
	float visibility = 1.0f;
    
	//vec3 n = normalize(normal_cam);
	vec3 n = Normal;
	vec3 l = normalize(lightDir);
	//vec3 l = vec3(1,1,1);
	float cosTheta = clamp( dot( n,l ), 0,1 );
	vec3 e = normalize(eyeDir);
	vec3 r = reflect(-l, n);
	float cosAlpha = clamp( dot( e,r ), 0,1 );

	// Fixed bias, or...
	float bias = 0.005;

	// ...variable bias
	// float bias = 0.005*tan(acos(cosTheta));
	// bias = clamp(bias, 0,0.01);

	for (int i=0;i<4;i++){
		visibility -= 0.2*(1.0-texture( shadowMap, vec3(shadowCoord.xy + poissonDisk[i]/700.0,  (shadowCoord.z-bias)/shadowCoord.w) ));
	}

    // Textures------------------------------------------------------
    vec4 texColor;

    if (FragPos.y <= range_cutoffs[1]) {
        float range = range_cutoffs[1] - range_cutoffs[0];
        float fraction = (FragPos.y - range_cutoffs[0])/range;
        texColor = vec4(texture(texSampler0, TexCoord).xyz, 0.f)*(1-fraction) + vec4(texture(texSampler1, TexCoord).xyz, 1.f)*fraction;
		ambient = vec3(0.1,0.1,0.1) * vec3(texColor);
        color = vec4(texture(texSampler1, TexCoord).xyz, 1.0f);
    }
    else if (FragPos.y <= range_cutoffs[2]) {
        float range = range_cutoffs[2] - range_cutoffs[1];
        float fraction = (FragPos.y - range_cutoffs[1])/range;
        texColor = texture(texSampler1, TexCoord)*(1-fraction) + texture(texSampler2, TexCoord)*fraction;
		ambient = vec3(0.1,0.1,0.1) * vec3(texColor);
         color = vec4(texColor.xyz, 1.0f);
    }
    else if (FragPos.y <= range_cutoffs[3]) {
        float range = range_cutoffs[3] - range_cutoffs[2];
        float fraction = (FragPos.y - range_cutoffs[2])/range;
        texColor = texture(texSampler2, TexCoord)*(1-fraction) + texture(texSampler3, TexCoord)*fraction;
		ambient = vec3(0.1,0.1,0.1) * texColor.xyz;
        color = vec4(texColor.xyz, 1.0f);
    }
    else if (FragPos.y <= range_cutoffs[4]) {
        float range = range_cutoffs[4] - range_cutoffs[3];
        float fraction = (FragPos.y - range_cutoffs[3])/range;
        texColor = texture(texSampler3, TexCoord)*(1-fraction) + texture(texSampler4, TexCoord)*fraction;
		ambient = vec3(0.1,0.1,0.1) * texColor.xyz;
        color = vec4(texColor.xyz, 1.0f);

    }
        else if (FragPos.y <= range_cutoffs[5]) {
        float range = range_cutoffs[5] - range_cutoffs[4];
        float fraction = (FragPos.y - range_cutoffs[4])/range;
        texColor = texture(texSampler4, TexCoord)*(1-fraction) + texture(texSampler5, TexCoord)*fraction;
		ambient = vec3(0.1,0.1,0.1) * texColor.xyz;
        color = vec4(texColor.xyz, 1.0f);
    }
    else {
        texColor = texture(texSampler5, TexCoord);
		ambient = vec3(0.1,0.1,0.1) * texColor.xyz;
        color = vec4(texColor.xyz, 1.0f);
    }//(Visibility+visibility) * 
	//* cosTheta
    color = vec4((ambient + ((Visibility+visibility)/2)*texColor.xyz* lightColor* lightPower* cosTheta), 1.0f); //+  ((Visibility+visibility)/2)*specular* lightColor* lightPower* pow(cosAlpha, 5)), 1.0f);
	//color = mix(vec4(0.4, 0.4, 0.4, 0.0), color, Visibility);
	//mix(vec4(0.4, 0.4, 0.4, 0.0), color, Visibility);
}
