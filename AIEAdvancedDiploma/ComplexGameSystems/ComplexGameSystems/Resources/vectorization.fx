////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - vectorization.h
////////////////////////////////////////////////////////////////////////////////////////

struct Constants
{
    int start;
    int width;
    int height;
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

[numthreads(1, 1, 1)]
void CSMain(uint3 dispatchThreadID : SV_DispatchThreadID)
{
    // Statistical Region Merging(SRM) Image Segmentation
    // Reference: www1.univ-ag.fr/~rnock/Articles/Drafts/tpami04-nn.pdf
    // Reference: http://www.lix.polytechnique.fr/~nielsen/Srmjava.java

    int width = BufferConstants[0].width;
    int height = BufferConstants[0].height;
    int start = BufferConstants[0].start;
    int pair = 0;

    BufferOut[pair].region1 = width;
    BufferOut[pair].region2 = height;
    BufferOut[pair].difference = 200;

    for (int y = start; y < height - 1; y++)
    {
        for (int x = start; x < width - 1; x++)
        {
            //int index = y * width + x;
            //
            //BufferOut[pair].region1 = index;
            //BufferOut[pair].region2 = index + 1;
            //
            //int c1 = BufferIn[index].colour;
            //int r1 = Red(c1);
            //int g1 = Green(c1);
            //int b1 = Blue(c1);
            //
            //int c2 = BufferIn[index + 1].colour;
            //int r2 = Red(c2);
            //int g2 = Green(c2);
            //int b2 = Blue(c2);
            //
            //BufferOut[pair].difference = max(max(abs(r2 - r1), abs(g2 - g1)), abs(b2 - b1));
            //pair++;
            //
            //BufferOut[pair].region1 = index;
            //BufferOut[pair].region2 = index + width;
            //
            //int c3 = BufferIn[index + width].colour;
            //int r3 = Red(c3);
            //int g3 = Green(c3);
            //int b3 = Blue(c3);
            //
            //BufferOut[pair].difference = max(max(abs(r3 - r1), abs(g3 - g1)), abs(b3 - b1));
            //pair++;
        }
    }
}
