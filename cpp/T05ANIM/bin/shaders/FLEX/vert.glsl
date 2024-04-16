/* */

// shader language version (3.3)
#version 330

layout(location = 0) in vec3 InPosition;
layout(location = 1) in vec2 InTexCoord;
layout(location = 2) in vec3 InNormal;
layout(location = 3) in vec4 InColor;

uniform mat4 MatrWVP;
uniform vec3 Ka;
uniform vec3 Kd;
uniform vec3 Ks;
uniform float Trans;
uniform float Ph;

// out params (varying)
out vec4 DrawColor;
out vec2 DrawTexCoord;
out vec3 DrawNormal; 
out vec3 DrawPos;
 
 
void main( void )
{
    gl_Position = MatrWVP * vec4(InPosition, 1);
    DrawColor = InColor;
    DrawTexCoord = InTexCoord;
    DrawNormal = InNormal;
}
