
varying vec4 v_fragmentColor;
varying vec2 v_texCoord;
uniform sampler2D u_texture1;
varying vec2 v_test;
uniform float time;
uniform float offy;
void main() {
  vec4 texel = texture2D(CC_Texture0, v_texCoord);
	float ux = clamp(v_test.x/ 640.0, 0.0, 1.0);
	float uy = clamp(1.0 - (v_test.y + offy)/ 440.0, 0.0, 1.0);
	uy += time;
	vec4 colormask = texture2D(u_texture1, vec2(ux, uy));
	gl_FragColor = texel * colormask.x *v_fragmentColor.a; 
}

