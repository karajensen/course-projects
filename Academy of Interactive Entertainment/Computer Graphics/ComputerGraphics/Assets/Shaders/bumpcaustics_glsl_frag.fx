////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - bumpcaustics_glsl_frag.fx
////////////////////////////////////////////////////////////////////////////////////////

#version 150

out vec4 out_Color[SCENE_TEXTURES];

in float ex_Depth;
in vec2 ex_UVs;
in vec3 ex_PositionWorld;
in vec3 ex_Normal;
in vec3 ex_Tangent;
in vec3 ex_Bitangent;

uniform float lightActive[MAX_LIGHTS];
uniform vec3 lightPosition[MAX_LIGHTS];
uniform vec3 lightDiffuse[MAX_LIGHTS];
uniform vec3 lightAttenuation[MAX_LIGHTS];

uniform float meshDiffuse;
uniform float meshAmbience;
uniform float meshBump;
uniform float meshCausticAmount;
uniform float meshCausticScale;

uniform sampler2D DiffuseSampler;
uniform sampler2D NormalSampler;
uniform sampler2D CausticsSampler;

void main(void)
{
    vec4 diffuseTex = texture(DiffuseSampler, ex_UVs);
    vec3 diffuse = vec3(0.0, 0.0, 0.0);
    vec3 normal = normalize(ex_Normal);

    vec4 normalTex = texture(NormalSampler, ex_UVs);
    vec2 bump = meshBump * (normalTex.rg - 0.5);
    normal = normalize(normal + bump.x * normalize(ex_Tangent) 
        + bump.y * normalize(ex_Bitangent));

    for (int i = 0; i < MAX_LIGHTS; ++i)
    {
        vec3 lightColour = lightDiffuse[i];
        vec3 vertToLight = lightPosition[i] - ex_PositionWorld;
        float lightLength = length(vertToLight);
        
        float attenuation = 1.0 / (lightAttenuation[i].x 
            + lightAttenuation[i].y * lightLength 
            + lightAttenuation[i].z * lightLength * lightLength);

        vertToLight /= lightLength;

        lightColour *= ((dot(vertToLight, normal) + 1.0) *
            ((1.0 - meshDiffuse) * 0.5)) + meshDiffuse;

        diffuse += lightColour * attenuation * lightActive[i];
    }

    vec3 caustics = texture(CausticsSampler, 
        ex_UVs * meshCausticScale).rgb * max(normal.y, 0.0);

    out_Color[ID_COLOUR].rgb = diffuseTex.rgb * diffuse;
    out_Color[ID_COLOUR].rgb += caustics * meshCausticAmount;
    out_Color[ID_COLOUR].rgb *= meshAmbience;
    out_Color[ID_COLOUR].a = 1.0;
    out_Color[ID_DEPTH] = vec4(ex_Depth, ex_Depth, ex_Depth, 1.0);
}
