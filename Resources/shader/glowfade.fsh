
varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

uniform vec3 colorglow;
void main() {  
    vec4 texel = texture2D(CC_Texture0, v_texCoord);
	float mask = CC_CosTime[3];
	mask = (mask + 1.0) * 0.5 * 0.9;
	//vec4 colorglow2 = vec4(v_texCoord,0.5+0.5*CC_SinTime[3],1.0);
    gl_FragColor = texel + vec4(colorglow.x * mask, colorglow.y * mask, colorglow.z * mask, 0);
}

