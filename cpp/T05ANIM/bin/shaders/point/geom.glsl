/* */
#version 430
 
layout (points) in;
layout (points, max_vertices = 4) out;

out vec2 DrawTexCoord;

void main( void )
{                                                       
  gl_Position = vec4(0, 0, 0, 1);
  EmitVertex();
  EndPrimitive();
}
