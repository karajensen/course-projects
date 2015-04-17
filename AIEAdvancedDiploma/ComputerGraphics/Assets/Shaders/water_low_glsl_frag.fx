////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - water_lod_glsl_frag.fx
// Based on nvidia Water Shader: http://nvidia.com/shaderlibrary
////////////////////////////////////////////////////////////////////////////////////////

#version 150

out vec4 out_Color[SCENE_TEXTURES];

in float ex_Depth;
in vec2 ex_UVs;
in vec3 ex_PositionWorld;

uniform sampler2D DiffuseSampler;

uniform vec4 lowQualityTint;
uniform vec3 lightPosition[MAX_LIGHTS];
uniform vec3 lightDiffuse[MAX_LIGHTS];
uniform vec3 lightAttenuation[MAX_LIGHTS];
uniform float lightActive[MAX_LIGHTS];

float saturate(float value)
{
    return clamp(value, 0.0, 1.0);
}

void main(void)
{
    vec3 diffuseTex = texture(DiffuseSampler, ex_UVs).rgb;
    vec3 diffuse = vec3(0.0, 0.0, 0.0);
    vec3 normal = vec3(0,1,0);

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
        diffuse += lightColour * attenuation * lightActive[i];
    }

    out_Color[ID_COLOUR] = vec4(diffuseTex * diffuse, 1.0);
    out_Color[ID_COLOUR].rgb += lowQualityTint.rgb;
    out_Color[ID_COLOUR].a = lowQualityTint.a;

    out_Color[ID_NORMAL].rgb = normal;
    out_Color[ID_NORMAL].a = ex_Depth;
}