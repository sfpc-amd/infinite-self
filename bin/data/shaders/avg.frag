// fragment shader
#version 120

//https://www.opengl.org/registry/specs/EXT/texture_array.txt
//http://www.gamedev.net/topic/508125-glsl-array-of-textures/

// this is how we receive the texture
uniform sampler2DRect tex0;
uniform sampler2DRect tex1;
uniform sampler2DRect tex2;
uniform sampler2DRect tex3;
uniform sampler2DRect tex4;
uniform sampler2DRect tex5;
uniform sampler2DRect tex6;
uniform sampler2DRect tex7;
uniform sampler2DRect tex8;
uniform sampler2DRect tex9;

varying vec2 texCoordVarying;

void main()
{

    int n = 10;
    vec4 avg = vec4(0.0);
    
//    avg = mix(texture2DRect(tex0, texCoordVarying), avg, 1.0/n);
//    avg = mix(texture2DRect(tex1, texCoordVarying), avg, 1.0/n);
//    avg = mix(texture2DRect(tex2, texCoordVarying), avg, 1.0/n);
//    avg = mix(texture2DRect(tex3, texCoordVarying), avg, 1.0/n);
//    avg = mix(texture2DRect(tex4, texCoordVarying), avg, 1.0/n);
//    avg = mix(texture2DRect(tex5, texCoordVarying), avg, 1.0/n);
//    avg = mix(texture2DRect(tex6, texCoordVarying), avg, 1.0/n);
//    avg = mix(texture2DRect(tex7, texCoordVarying), avg, 1.0/n);
//    avg = mix(texture2DRect(tex8, texCoordVarying), avg, 1.0/n);
//    avg = mix(texture2DRect(tex9, texCoordVarying), avg, 1.0/n);
    
    avg = texture2DRect(tex0, texCoordVarying);
    avg += texture2DRect(tex1, texCoordVarying);
    avg += texture2DRect(tex2, texCoordVarying);
    avg += texture2DRect(tex3, texCoordVarying);
    avg += texture2DRect(tex4, texCoordVarying);
    avg += texture2DRect(tex5, texCoordVarying);
    avg += texture2DRect(tex6, texCoordVarying);
    avg += texture2DRect(tex7, texCoordVarying);
    avg += texture2DRect(tex8, texCoordVarying);
    avg += texture2DRect(tex9, texCoordVarying);
    avg = avg / n;

    
    gl_FragColor = vec4(avg.rgb, 1.0);
}