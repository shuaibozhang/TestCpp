
varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

uniform vec3 colorglow;
void main() {  
    vec4 texel = texture2D(CC_Texture0, v_texCoord);
	float mask = CC_CosTime[3];
	mask = (mask + 1.0) * 0.5 * 0.9;
	float luminance = dot(texel, vec4(0.299, 0.587, 0.114, 0.0)); 
	mask = luminance * luminance * mask;  
	gl_FragColor = texel + vec4(colorglow.x * mask, colorglow.y * mask, colorglow.z * mask, 0);
}

