
varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

void main() {  
    vec4 texel = texture2D(CC_Texture0, v_texCoord);

    float luminance = dot(texel, vec4(0.299, 0.587, 0.114, 0.0))+0.3;
	float mask = CC_SinTime[3];
    gl_FragColor = vec4(mix(vec3(texel), vec3(luminance), mask), texel.a);
}

