////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - water_low_glsl_vert.fx
// Based on nvidia Water Shader: http://nvidia.com/shaderlibrary
////////////////////////////////////////////////////////////////////////////////////////

#version 150

in vec4 in_Position;
in vec2 in_UVs;

out float ex_Depth;
out vec2 ex_UVs;
out vec3 ex_PositionWorld;

uniform float depthNear;
uniform float depthFar;
uniform float timer;
uniform float speed;
uniform mat4 world;
uniform mat4 viewProjection;
uniform vec2 uvScale;

uniform float waveFrequency[MAX_WAVES];
uniform float waveAmplitude[MAX_WAVES];
uniform float wavePhase[MAX_WAVES];
uniform float waveDirectionX[MAX_WAVES];
uniform float waveDirectionZ[MAX_WAVES];
 
void main(void)
{
    // Sum the waves together
    float time = timer * speed;
    vec4 wavePosition = in_Position;
    for (int i = 0; i < MAX_WAVES; i++)
    {
        // Wave equation: y = a * sin(kx-wt+phase)
        vec2 direction = vec2(waveDirectionX[i], waveDirectionZ[i]);
        float component = dot(direction, in_Position.xz) - (waveFrequency[i] * time) + wavePhase[i];
        wavePosition.y += waveAmplitude[i] * sin(component);
    }

    gl_Position = viewProjection * world * wavePosition;
    ex_PositionWorld = (world * wavePosition).xyz;
    ex_UVs = in_UVs * uvScale;

    vec2 depthBounds = vec2(0.0, 1.0);
    ex_Depth = ((gl_Position.z - depthNear) *
        ((depthBounds.x - depthBounds.y) / (depthFar - depthNear))) + depthBounds.y;
}