#version 330                                                                        
                                                                                    
layout (location = 0) in vec3 Position;                                                
layout (location = 2) in vec3 Normal;                                               
                                                                                    
uniform mat4 modelview; 
uniform mat4 projection;                                                                 
uniform mat4 gLightWVP;                                                             
uniform mat4 model;                                                                
                                                                                    
out vec4 LightSpacePos;                                                                     
out vec3 Normal0;                                                                   
out vec3 WorldPos0;                                                                 
                                                                                    
void main()                                                                         
{                                                                                   
    gl_Position   = projection * modelview * vec4(Position, 1.0);                                  
    LightSpacePos = gLightWVP * vec4(Position, 1.0);                                                 
    Normal0       = (model * vec4(Normal, 0.0)).xyz;                            
    WorldPos0     = (model * vec4(Position, 1.0)).xyz;                          
}