////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - flat_glsl_frag.fx
////////////////////////////////////////////////////////////////////////////////////////

#version 150

out vec4 out_Color[SCENE_TEXTURES];

in float ex_Depth;
in vec2 ex_UVs;
in vec3 ex_Normal;
uniform float meshAmbience;

uniform sampler2D DiffuseSampler;

void main(void)
{
    out_Color[ID_COLOUR].rgb = texture(DiffuseSampler, ex_UVs).rgb;
    out_Color[ID_COLOUR].rgb *= meshAmbience;
    out_Color[ID_COLOUR].a = 1.0;
    out_Color[ID_DEPTH] = vec4(ex_Depth, ex_Depth, ex_Depth, 1.0);
}
