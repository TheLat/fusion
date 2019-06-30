#version 120
uniform float invert;
uniform float contrast;
uniform sampler2D Mtexture;
void main(){
  vec4 color = texture2D(Mtexture, gl_TexCoord[0].st);
  color = vec4(((1.0 - invert) * color.r) + (invert*(1.0 - color.r)), ((1.0 - invert) * color.g) +  (invert*(1.0 - color.g)), ((1.0 - invert) * color.b) +  (invert*(1.0 - color.b)), 1.0);
  gl_FragColor = vec4(color.r, color.g, color.b, 1.0);
}
