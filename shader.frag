#version 330 core
// This is a sample fragment shader.

// Inputs to the fragment shader are the outputs of the same name from the vertex shader.
// Note that you do not have access to the vertex shader's default output, gl_Position.
in float sampleExtraOutput;
in vec3 Normal;
in vec3 FragPos;

// You can output many things. The first vec4 type output determines the color of the fragment
out vec4 color;

void main()
{
    //Material coloring
    vec3 mat_ambient = vec3(0.24725f, 0.1995f, 0.0745f);
    vec3 mat_diffuse = vec3(0.75164f, 0.60648f, 0.22648f);
    vec3 mat_specular = vec3(0.628281f, 0.555802f, 0.366065f);
    float mat_shininess = 55.f;
    
    //Directional Light-----------------------------------------------------------------------
    vec3 lightColor = vec3(1.0f, 1.0f, 1.0f);
    // Ambient
    vec3 ambient = lightColor * mat_ambient;
    
    // Diffuse 
    vec3 norm = normalize(Normal);
    vec3 lightDir = normalize(vec3(0.f, 0.f, -10.f) - FragPos);
    float diff = max(dot(norm, lightDir), 0.0);
    vec3 diffuse = lightColor * (diff * mat_diffuse);
    
    // Specular
    vec3 viewP = vec3(0.f, 0.f, 20.f);
    vec3 viewDir = normalize(viewP - FragPos);
    vec3 reflectDir = reflect(-lightDir, norm);  
    float spec = pow(max(dot(viewDir, reflectDir), 0.0),mat_shininess);
    vec3 specular = lightColor * (spec * mat_specular);  
        
    vec3 dir_result = ambient + diffuse + specular;
    
     color = vec4(dir_result, 1.0f);
}
