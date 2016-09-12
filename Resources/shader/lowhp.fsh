
varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

uniform vec3 colorglow;
uniform sampler2D u_texture1;

void main() {  
    vec2 outUV = v_texCoord;
    vec4 basetexel = texture2D(CC_Texture0,outUV);

    vec2 blurUV = outUV-0.5;
    vec2 blurUV0 = blurUV*0.99+0.5;
    vec2 blurUV1 = blurUV*0.98+0.5;
    vec2 blurUV2 = blurUV*0.97+0.5;
    vec2 blurUV3 = blurUV*0.96+0.5;
    vec2 blurUV4 = blurUV*0.95+0.5;

    float nums = 4.0;

    //vec4 texel2 = texture2D(CC_Texture0, blurUV0)*(1.0/nums);
   // texel2 += texture2D(CC_Texture0, blurUV1)*(1.0/nums);
   // texel2 += texture2D(CC_Texture0, blurUV2)*(1.0/nums);
   // texel2 += texture2D(CC_Texture0, blurUV3)*(1.0/nums);
   // texel2 += texture2D(CC_Texture0, blurUV4)*(1.0/nums);

   vec4 texel2 = texture2D(CC_Texture0, blurUV0)*(0.25);
   texel2 += texture2D(CC_Texture0, blurUV1)*(0.25);
   texel2 += texture2D(CC_Texture0, blurUV2)*(0.25);
   texel2 += texture2D(CC_Texture0, blurUV3)*(0.25);
   texel2 += texture2D(CC_Texture0, blurUV4)*(0.25);

	//float mask = abs(CC_CosTime[3]);
	//float RATIO = mask;
	
		float RATIO = (CC_CosTime[3] + 1.0)* 0.5;

   float mask = (texture2D(u_texture1, outUV).r+0.3)*RATIO;
   gl_FragColor = mix(basetexel, texel2, mask)+vec4(0.5, 0.0, 0.0, 1.0)*mask*RATIO;
}

