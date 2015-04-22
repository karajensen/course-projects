////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - particle_glsl_vert.fx
////////////////////////////////////////////////////////////////////////////////////////

#version 150

in vec4 in_Position;
in vec2 in_UVs;

out vec2 ex_UVs;
out float ex_Depth;

uniform float depthNear;
uniform float depthFar;
uniform mat4 worldViewProjection;
 
void main(void)
{
    gl_Position = worldViewProjection * in_Position;
    ex_UVs = in_UVs;

    vec2 depthBounds = vec2(0.0, 1.0);
    ex_Depth = ((gl_Position.z - depthNear) *
        ((depthBounds.x - depthBounds.y) / (depthFar - depthNear))) + depthBounds.y;
}