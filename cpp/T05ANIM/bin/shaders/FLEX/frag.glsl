/* */

// shader language version (4.2)
#version 420

in vec4 DrawColor;
in vec2 DrawTexCoord;
in vec3 DrawNormal;
in vec3 DrawPos;

layout(location = 0) out vec4 OutColor;

layout(binding = 0) uniform sampler2D Texture0;

void main( void )
{
  vec3 a = texture(Texture0, DrawTexCoord).rgb;
  //if (a.x > 0) OutColor = vec4(1);
  //else if (a.y > 0) OutColor = vec4(1);
  //else if (a.z > 0) OutColor = vec4(1);
  OutColor = vec4(a.bgr, 1);
  //OutColor = vec4(DrawNormal * 0.5 + 0.5, 1);
}
