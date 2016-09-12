
varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

uniform vec3 colorglow;
uniform vec3 colorout;
uniform float time;
uniform float time2;
void main() {  
   vec4 texel = texture2D(CC_Texture0, v_texCoord);
   vec3 colorout1 = vec3(colorout.x * time2, colorout.y * time2, colorout.z * time2);
   vec3 colorglow1 = vec3(colorglow.x * time, colorglow.y * time, colorglow.z * time); 
   gl_FragColor = vec4(vec3(texel) * colorout1 + colorglow1, texel.a);
}

