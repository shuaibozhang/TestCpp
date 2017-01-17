
varying vec4 v_fragmentColor;
varying vec2 v_texCoord;
uniform sampler2D u_texture1;
uniform float time;
uniform float offy;
void main() {
  vec4 texel = texture2D(CC_Texture0, v_texCoord);
	float ux = v_texCoord.x;
	float uy =  1.0 - v_texCoord.y;
	uy += time * 0.5;
	vec4 colormask = texture2D(u_texture1, vec2(ux,uy));
	gl_FragColor = texel * (colormask.x)*v_fragmentColor.a; 
}

