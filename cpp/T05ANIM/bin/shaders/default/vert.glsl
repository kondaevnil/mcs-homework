/* */

// shader language version (3.3)
#version 460

layout(location = 0) in vec3 InPosition;
layout(location = 1) in vec2 InTexCoord;
layout(location = 2) in vec3 InNormal;
layout(location = 3) in vec4 InColor;
 
layout(std140, binding = 3) uniform jjj
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
  //gl_Position = MatrWVP * vec4(InPosition * 300 / dot(InPosition, InPosition), 1);
  DrawColor.rgb = InNormal.xyz;

  DrawPos = InPosition;
  DrawTexCoord = InTexCoord;
  DrawNormal = InNormal;  
  //DrawNormal = InPosition.xyz;
   //vec4(sin(TInColor;   ime) * InPosition.x, cos(Time) * InPosition.y, InPosition.z, 1);
}
