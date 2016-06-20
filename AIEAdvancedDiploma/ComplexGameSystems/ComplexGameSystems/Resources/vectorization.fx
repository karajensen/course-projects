////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - vectorization.h
////////////////////////////////////////////////////////////////////////////////////////

struct Constants
{
    int width;
    int height;
    int edges;
};

struct Edge
{
    int region1;
    int region2;
    int difference;
};

struct Pixel
{
    int colour;
};

StructuredBuffer<Pixel> BufferIn : register(t0);
StructuredBuffer<Constants> BufferConstants : register(t1);
RWStructuredBuffer<Edge> BufferOut : register(u0);

int Red(int colour)
{
    return colour & 0xFF;
}

int Green(int colour)
{
    return (colour & 0xFF00) >> 8;
}

int Blue(int colour)
{
    return (colour & 0xFF0000) >> 16;
}

[numthreads(32, 32, 1)]
void CSMain(uint3 dispatchThreadID : SV_DispatchThreadID)
{
    // Statistical Region Merging(SRM) Image Segmentation
    // Reference: www1.univ-ag.fr/~rnock/Articles/Drafts/tpami04-nn.pdf
    // Reference: http://www.lix.polytechnique.fr/~nielsen/Srmjava.java

    int width = BufferConstants[0].width;
    int height = BufferConstants[0].height;
    int offset = BufferConstants[0].edges / 2;
    int endY = min(dispatchThreadID.y + (height / 32), height - 1);
    int endX = min(dispatchThreadID.x + (width / 32), width - 1);

    for (int y = dispatchThreadID.y; y < endY; y++)
    {
        for (int x = dispatchThreadID.x; x < endX; x++)
        {
            int index = y * width + x;
            
            int c1 = BufferIn[index].colour;
            int r1 = Red(c1);
            int g1 = Green(c1);
            int b1 = Blue(c1);
            
            int c2 = BufferIn[index + 1].colour;
            int r2 = Red(c2);
            int g2 = Green(c2);
            int b2 = Blue(c2);

            int c3 = BufferIn[index + width].colour;
            int r3 = Red(c3);
            int g3 = Green(c3);
            int b3 = Blue(c3);

            int outIndex = index;
            BufferOut[outIndex].region1 = index;
            BufferOut[outIndex].region2 = index + 1;
            BufferOut[outIndex].difference = max(max(abs(r2 - r1), abs(g2 - g1)), abs(b2 - b1));
            
            outIndex += offset;
            BufferOut[outIndex].region1 = index;
            BufferOut[outIndex].region2 = index + width;
            BufferOut[outIndex].difference = max(max(abs(r3 - r1), abs(g3 - g1)), abs(b3 - b1));
        }
    }
}
