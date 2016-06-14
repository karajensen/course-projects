////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - SRM.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "SRM.h"
#include <math.h>
#include <vector>
#include <algorithm>
#include <numeric>

/**
* Union Find Data Structure of Tarjan for Disjoint Sets
*/
class UnionFind
{
public:

    /**
    * Create a UF for n elements
    */
    UnionFind(int size)
    {
        parent.resize(size);
        rank.resize(size);
    }

    /**
    * Initialises
    */
    void Initialise()
    {
        std::iota(parent.begin(), parent.end(), 0);
        rank.assign(rank.size(), 0);
    }

    /**
    * Find procedures
    */
    int Find(int k)
    {
        while (parent[k] != k)
        {
            k = parent[k];
        }
        return k;
    }

    /**
    * Assume x and y being roots
    */
    int UnionRoot(int x, int y)
    {
        if (x == y)
        {
            return -1;
        }

        if (rank[x] > rank[y])
        {
            parent[y] = x; 
            return x;
        }
        else
        {
            parent[x] = y; 
            if (rank[x] == rank[y])
            {
                rank[y]++;
            }
            return y;
        }
    }

private:

    std::vector<int> rank;
    std::vector<int> parent;
};

double Max3(double a, double b, double c)
{
    return std::max(a, std::max(b, c));
}

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

int Colour(int r, int g, int b)
{
    return 0xff000000 | b << 16 | g << 8 | r;
}

SRM::~SRM() = default;

SRM::SRM(int w, int h) :
    width(w),
    height(h),
    size(w * h)
{
    logdelta = 2.0 * log(6.0 * size);
    edges = 2 * (w - 1)*(h - 1) + (h - 1) + (w - 1);
    UF = std::make_unique<UnionFind>(size);

    Ravg.resize(size);
    Gavg.resize(size);
    Bavg.resize(size);
    N.resize(size);
    order.resize(edges);
    order2.resize(edges);
    nbe.resize(levels);
    cnbe.resize(levels);
}

void SRM::Execute(int* input, int* output, float vectorization, bool border)
{
    // Complexity is inverse to vectorization range [0,1]
    complexity = 30.0 + ((1.0 - vectorization) * 300.0);

    InitializeSegmentation(input);
    Segmentation(input);
    OutputSegmentation(output);

    if (border)
    {
        DrawBorder(output);
    }
}

void SRM::InitializeSegmentation(int* input)
{
    UF->Initialise();
    N.assign(N.size(), 1);

    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int index = y*width + x;

            int red = Red(input[index]);
            int green = Green(input[index]);
            int blue = Blue(input[index]);

            Ravg[index] = red;
            Gavg[index] = green;
            Bavg[index] = blue;
        }
    }
}

void SRM::OutputSegmentation(int* output)
{
    for (int y = 0; y < height; y++)
    {
        for (int x = 0; x < width; x++)
        {
            int index = y*width + x;
            int indexb = UF->Find(index); // Get the root index 

            int r = (int)Ravg[indexb];
            int g = (int)Gavg[indexb];
            int b = (int)Bavg[indexb];

            output[index] = Colour(r, g, b);
        }
    }
}

void SRM::DrawBorder(int* output)
{
    const int borderthickness = 1;

    for (int i = 1; i < height; i++)
    {
        for (int j = 1; j < width; j++)
        {
            int index = i*width + j;

            int C1 = UF->Find(index);
            int C2 = UF->Find(index - 1 - width);

            if (C2 != C1)
            {
                for (int k = -borderthickness; k <= borderthickness; k++)
                {
                    for (int l = -borderthickness; l <= borderthickness; l++)
                    {
                        index = (i + k) * width + (j + l);
                        if ((index >= 0) && (index < size))
                        {
                            int red = std::max(Red(output[index]) - 10, 0);
                            int green = std::max(Green(output[index]) - 10, 0);
                            int blue = std::max(Blue(output[index]) - 10, 0);
                            output[index] = Colour(red, green, blue);
                        }
                    }
                }
            }
        }
    }
}

void SRM::Segmentation(int* input)
{
    int cpair = 0;

    for (int y = 0; y < height - 1; y++)
    {
        for (int x = 0; x < width - 1; x++)
        {
            int index = y*width + x;

            order[cpair].r1 = index;
            order[cpair].r2 = index + 1;

            int r1 = Red(input[index]);
            int g1 = Green(input[index]);
            int b1 = Blue(input[index]);

            int r2 = Red(input[index + 1]);
            int g2 = Green(input[index + 1]);
            int b2 = Blue(input[index + 1]);

            order[cpair].diff = (int)Max3(std::abs(r2 - r1), std::abs(g2 - g1), std::abs(b2 - b1));
            cpair++;

            order[cpair].r1 = index;
            order[cpair].r2 = index + width;

            r2 = Red(input[index + width]);
            g2 = Green(input[index + width]);
            b2 = Blue(input[index + width]);

            order[cpair].diff = (int)Max3(std::abs(r2 - r1), std::abs(g2 - g1), std::abs(b2 - b1));
            cpair++;
        }
    }

    // The two border lines
    for (int i = 0; i < height - 1; i++)
    {
        int index = i * width + width - 1;
        order[cpair].r1 = index;
        order[cpair].r2 = index + width;

        int r1 = Red(input[index]);
        int g1 = Green(input[index]);
        int b1 = Blue(input[index]);
        int r2 = Red(input[index + width]);
        int g2 = Green(input[index + width]);
        int b2 = Blue(input[index + width]);

        order[cpair].diff = (int)Max3(std::abs(r2 - r1), std::abs(g2 - g1), std::abs(b2 - b1));
        cpair++;
    }

    for (int j = 0; j < width - 1; j++)
    {
        int index = (height - 1)*width + j;

        order[cpair].r1 = index;
        order[cpair].r2 = index + 1;

        int r1 = Red(input[index]);
        int g1 = Green(input[index]);
        int b1 = Blue(input[index]);
        int r2 = Red(input[index + 1]);
        int g2 = Green(input[index + 1]);
        int b2 = Blue(input[index + 1]);
        order[cpair].diff = (int)Max3(std::abs(r2 - r1), std::abs(g2 - g1), std::abs(b2 - b1));

        cpair++;
    }

    BucketSortOrder();

    for (int i = 0; i < edges; i++)
    {
        int reg1 = order[i].r1;
        int C1 = UF->Find(reg1);
        int reg2 = order[i].r2;
        int C2 = UF->Find(reg2);

        if ((C1 != C2) && (MergePredicate(C1, C2)))
        {
            MergeRegions(C1, C2);
        }
    }
}

void SRM::MergeRegions(int C1, int C2)
{
    int reg = UF->UnionRoot(C1, C2);
    int nreg = N[C1] + N[C2];
    double ravg = (N[C1] * Ravg[C1] + N[C2] * Ravg[C2]) / nreg;
    double gavg = (N[C1] * Gavg[C1] + N[C2] * Gavg[C2]) / nreg;
    double bavg = (N[C1] * Bavg[C1] + N[C2] * Bavg[C2]) / nreg;

    N[reg] = nreg;
    Ravg[reg] = ravg;
    Gavg[reg] = gavg;
    Bavg[reg] = bavg;
}

bool SRM::MergePredicate(int reg1, int reg2)
{
    double dR = (Ravg[reg1] - Ravg[reg2]);
    dR *= dR;

    double dG = (Gavg[reg1] - Gavg[reg2]);
    dG *= dG;

    double dB = (Bavg[reg1] - Bavg[reg2]);
    dB *= dB;

    double logreg1 = std::min(double(levels), (double)(N[reg1]) * log(1.0 + N[reg1]));
    double logreg2 = std::min(double(levels), (double)(N[reg2]) * log(1.0 + N[reg2]));

    double dev1 = ((levels*levels) / (2.0 * complexity * N[reg1]))*(logreg1 + logdelta);
    double dev2 = ((levels*levels) / (2.0 * complexity * N[reg2]))*(logreg2 + logdelta);

    double dev = dev1 + dev2;

    return ((dR<dev) && (dG<dev) && (dB<dev));
}

void SRM::BucketSortOrder()
{
    for (int i = 0; i<levels; i++)
        nbe[i] = 0;

    // class all elements according to their family
    for (int i = 0; i<edges; i++)
        nbe[order[i].diff]++;

    // cumulative histogram
    cnbe[0] = 0;
    for (int i = 1; i<levels; i++)
        cnbe[i] = cnbe[i - 1] + nbe[i - 1]; // index of first element of category i

    // allocation
    for (int i = 0; i<edges; i++)
        order2[cnbe[order[i].diff]++] = order[i];

    std::swap(order2, order);
}