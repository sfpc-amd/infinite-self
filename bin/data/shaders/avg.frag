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
varying vec2 texCoordVarying;

void main()
{


    vec4 avg;
    
    avg += texture2DRect(tex0, texCoordVarying);
    avg += texture2DRect(tex1, texCoordVarying);
    avg += texture2DRect(tex2, texCoordVarying);
    avg += texture2DRect(tex3, texCoordVarying);
    avg += texture2DRect(tex4, texCoordVarying);
    avg += texture2DRect(tex5, texCoordVarying);
    
    gl_FragColor = avg/6;
}