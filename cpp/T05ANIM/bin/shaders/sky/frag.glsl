/**/
#version 420

layout(location = 0) out vec4 OutColor;

layout(binding = 0) uniform sampler2D Texture0;

in vec2 DrawTexCoord;

layout(std140, binding = 2) uniform in_jr
{
  vec4 CamLocFrameW;
  vec4 CamRightFrameH;
  vec4 CamUp;
  vec4 CamDir;
  mat4 VP;
};

uniform int FrameW, FrameH;

void  main( void )
{
  float FrameW = CamLocFrameW.w;
  vec3 CamLoc = CamLocFrameW.xyz;
  vec3 CamRight = CamRightFrameH.xyz;
  float FrameH = CamRightFrameH.w;
  vec3 Dir = CamDir.xyz;
  vec3 Up = CamUp.xyz;

  vec2 s = (DrawTexCoord - 0.5) * 1;
  
  if (FrameW > FrameH) s.x *= FrameW / FrameH;  
  else s.y *= FrameH / FrameW;

  vec3 dir = normalize(Dir + CamRight * s.x + Up * s.y);
  float
    theta = acos(dir.y),
    phi = atan(dir.x, dir.z),
    pi = acos(-1);
  vec2 c = vec2(phi / 2 / pi, 1 - theta / pi);  

  vec4 tc = texture(Texture0, c);
  //discard;
  OutColor = vec4(tc.bgr, 1);    
}
