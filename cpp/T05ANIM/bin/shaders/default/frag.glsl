/* */

// shader language version (4.2)
#version 420

in vec4 DrawColor;   
in vec2 DrawTexCoord;
in vec3 DrawNormal;  
in vec3 DrawPos;  
   
//painting color
layout(location = 0) out vec4 OutColor;

layout(std140, binding = 1) uniform in_fr
{
  vec4 Ka4;
  vec4 KdTrans;
  vec4 KsPh;
};

layout(std140, binding = 2) uniform in_jr
{
  vec4 CamLocFrameW;
  mat4 VP;
  vec4 CamRightFrameH;
  vec4 CamUp;
  vec4 CamDir;
};
 
layout(binding = 0) uniform sampler2D Texture0;

vec3 Shade( vec3 P, vec3 N, vec3 L, vec3 LColor )
{
  vec3 Ka = Ka4.xyz, Kd = KdTrans.xyz, Ks = KsPh.xyz;
  float Trans = KdTrans.w, Ph = KsPh.w;
  vec3 V = normalize(P - CamLocFrameW.xyz);
  vec3 color = vec3(0);

  // Reverse normal if need
  //N = faceforward(N, V, N);

  // Ambient
  color += Ka;

  // Duffuse
  float nl = dot(N, L);
  nl = max(nl, 0);
  vec3 diff = LColor * Kd * nl;
  vec3 tex = texture(Texture0, DrawTexCoord).rgb;
  if (tex != 0) diff *= tex;
  color += diff;
 
  // Specular
  vec3 R = reflect(V, N);
  float rl = max(dot(R, L), 0);
  color += rl * LColor * Ks * pow(rl, Ph);

  return color;
}

void main( void )
{
  vec3 N = normalize(DrawNormal);
  vec3 L = normalize(vec3(100, 100, 100));  // light source
  OutColor = vec4(Shade(DrawPos, N, L, vec3(1)), 1);
}