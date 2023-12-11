//  Per Pixel Lighting shader with texture
// CREDIT: ex 25: pixtex.vert
#version 120

varying float YPosition;
varying vec3 View;
varying vec3 Light0;
varying vec3 Normal;

void main()
{
   //  Vertex location in modelview coordinates
   // gl_ProjectionMatrix: https://moddb.fandom.com/wiki/OpenGL:Tutorials:Loading_and_using_GLSL_shaders
   vec4 P = gl_ModelViewMatrix * gl_Vertex;

   // ADDED: the y position to decrease red with depth
   YPosition = P.y;

   //  Light position
   Light0  = gl_LightSource[0].position.xyz - P.xyz;
   //  Normal
   Normal = gl_NormalMatrix * gl_Normal;
   //  Eye position
   View  = -P.xyz;
   //  Texture
   gl_TexCoord[0] = gl_MultiTexCoord0;
   //  Set vertex position
   gl_Position = gl_ModelViewProjectionMatrix * gl_Vertex;
}
