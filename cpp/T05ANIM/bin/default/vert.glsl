/* */

// shader language version (3.3)
#version 460

layout(location = 0) in vec3 InPosition;
layout(location = 1) in vec2 InTexCoord;
layout(location = 2) in vec3 InNormal;
layout(location = 3) in vec4 InColor;
 
// global var (World * View * Proj)
//uniform mat4 MatrWVP;
layout(std140, binding = 4) uniform jjj
{
  mat4 MatrWVP;
};

// out params (varying)
out vec4 DrawColor;
out vec2 DrawTexCoord;
out vec3 DrawNormal; 
out vec3 DrawPos;

void main( void )
{
  gl_Position = MatrWVP * vec4(InPosition, 1);
  DrawColor = InColor;

  DrawPos = InPosition;
  DrawTexCoord = InTexCoord;
  DrawNormal = InNormal;  
}
