#version 120
uniform float invert;
uniform float contrast;
uniform float brightness;
uniform float offset;
uniform float wobble;
uniform sampler2D Mtexture;
void main(){
  vec2 texcoord = gl_TexCoord[0].st;
  texcoord[0] = texcoord[0] + (wobble*(sin((texcoord[1]*100.0) + offset)));
  vec4 color = texture2D(Mtexture, texcoord);
  color = vec4(((1.0 - invert) * color.r) + (invert*(1.0 - color.r)), ((1.0 - invert) * color.g) +  (invert*(1.0 - color.g)), ((1.0 - invert) * color.b) +  (invert*(1.0 - color.b)), 1.0);
  gl_FragColor = vec4(pow(color.r + brightness,contrast), pow(color.g + brightness,contrast), pow(color.b + brightness,contrast), 1.0);
}
