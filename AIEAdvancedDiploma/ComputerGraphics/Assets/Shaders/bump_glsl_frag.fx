
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

uniform float meshAmbience;
uniform float meshBump;

uniform sampler2D DiffuseSampler;
uniform sampler2D NormalSampler;

void main(void)
{
    vec4 diffuseTex = texture(DiffuseSampler, ex_UVs);
    vec4 diffuse = vec4(0.0, 0.0, 0.0, 0.0);
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
        lightColour *= ((dot(vertToLight, normal) + 1.0) * 0.5);

        diffuse.rgb += lightColour * attenuation * lightActive[i];
    }

    out_Color[ID_COLOUR].rgb = diffuseTex.rgb * diffuse;
    out_Color[ID_COLOUR].rgb *= meshAmbience;
    out_Color[ID_COLOUR].a = 1.0;

    out_Color[ID_NORMAL].rgb = normal;
    out_Color[ID_NORMAL].a = ex_Depth;
}
