#extension GL_ARB_texture_rectangle : enable
uniform sampler2DRect tex0;
void main() {
	vec2 pos = gl_TexCoord[0].st;
	vec4 color = texture2DRect(tex0, pos);
    gl_FragColor = vec4(color.rgb, 1.0);
}