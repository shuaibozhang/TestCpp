
varying vec4 v_fragmentColor;
varying vec2 v_texCoord;

uniform sampler2D u_texture1;
uniform float time;

void main() {
    vec4 texel = texture2D(CC_Texture0, v_texCoord);
    vec4 mask = texture2D(u_texture1, v_texCoord);
    gl_FragColor = mix(texel, vec4(0.0, 0.0, 0.0, 0.0), step(mask.x, time));
}

