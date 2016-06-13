////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - vectorization.h
////////////////////////////////////////////////////////////////////////////////////////

#define SCREEN_WIDTH 640
#define SCREEN_HEIGHT 480
#define DIMENSIONS 307200

struct Values
{
    float vectorization; // [0,1]
};

struct Pixel
{
    int colour;
};

StructuredBuffer<Pixel> BufferIn : register(t0);
StructuredBuffer<Values> BufferValues : register(t1);
RWStructuredBuffer<Pixel> BufferOut : register(u0);

[numthreads(1, 1, 1)]
void CSMain(uint3 dispatchThreadID : SV_DispatchThreadID)
{
    // Statistical Region Merging(SRM) Image Segmentation
    // Reference: www1.univ-ag.fr/~rnock/Articles/Drafts/tpami04-nn.pdf
    // Reference: http://www.lix.polytechnique.fr/~nielsen/Srmjava.java

    for (int x = 0; x < SCREEN_WIDTH; ++x)
    {
        for (int y = 0; y < SCREEN_HEIGHT; ++y)
        {
            int index = x * SCREEN_HEIGHT + y;

            BufferOut[index].colour = BufferIn[index].colour;
        }
    }
}
