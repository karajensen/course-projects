////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - SRM.cpp
////////////////////////////////////////////////////////////////////////////////////////

#include "SRM.h"
#include <math.h>
#include <vector>
#include <algorithm>
#include <numeric>

namespace
{
    double Max(double a, double b, double c)
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

    int ToColour(int r, int g, int b)
    {
        return 0xff000000 | b << 16 | g << 8 | r;
    }
}

SRM::~SRM() = default;

SRM::SRM(int w, int h) :
    m_width(w),
    m_height(h),
    m_size(w * h)
{
    m_logdelta = 2.0 * log(6.0 * m_size);
    m_levelsSqr = m_levels * m_levels;

    m_average.resize(m_size);
    m_count.resize(m_size);
    m_parent.resize(m_size);
    m_rank.resize(m_size);

    const int edges = 2 * (w - 1) * (h - 1) + (h - 1) + (w - 1);
    m_order.resize(edges);
}

void SRM::Execute(int* input, int* output, float vectorization)
{
    // complexity is inverse to vectorization range [0,1]
    m_complexity = 30.0 + ((1.0 - vectorization) * 300.0);

    // Initialise all containers
    std::iota(m_parent.begin(), m_parent.end(), 0);
    m_count.assign(m_count.size(), 1);
    m_rank.assign(m_rank.size(), 0);

    DisjointSet set(&m_rank[0], &m_parent[0]);

    for (size_t i = 0; i < m_average.size(); ++i)
    {
        m_average[i].r = Red(input[i]);
        m_average[i].g = Green(input[i]);
        m_average[i].b = Blue(input[i]);
    }
    
    // Main segmentation algorithm
    Segmentation(input, set);
    
    // Copy to output buffer
    for (size_t i = 0; i < m_average.size(); ++i)
    {
        const auto& c = m_average[set.find_set(i)];
        output[i] = ToColour(c.r, c.g, c.b);
    }
}

void SRM::Segmentation(int* input, DisjointSet& set)
{
    // Algorithm from: http://www.lix.polytechnique.fr/~nielsen/Srmjava.java

    int pair = 0;
    for (int y = 0; y < m_height - 1; y++)
    {
        for (int x = 0; x < m_width - 1; x++)
        {
            int index = y * m_width + x;

            m_order[pair].r1 = index;
            m_order[pair].r2 = index + 1;

            int r1 = Red(input[index]);
            int g1 = Green(input[index]);
            int b1 = Blue(input[index]);

            int r2 = Red(input[index + 1]);
            int g2 = Green(input[index + 1]);
            int b2 = Blue(input[index + 1]);

            m_order[pair].diff = (int)Max(std::abs(r2 - r1), std::abs(g2 - g1), std::abs(b2 - b1));
            pair++;

            m_order[pair].r1 = index;
            m_order[pair].r2 = index + m_width;

            r2 = Red(input[index + m_width]);
            g2 = Green(input[index + m_width]);
            b2 = Blue(input[index + m_width]);

            m_order[pair].diff = (int)Max(std::abs(r2 - r1), std::abs(g2 - g1), std::abs(b2 - b1));
            pair++;
        }
    }

    std::sort(m_order.begin(), m_order.end(),
        [](const SRM::Edge& e1, const SRM::Edge& e2)
    {
        return e1.diff < e2.diff;
    });

    for (size_t i = 0; i < m_order.size(); i++)
    {
        int r1 = m_order[i].r1;
        int C1 = set.find_set(r1);
        int r2 = m_order[i].r2;
        int C2 = set.find_set(r2);

        if (C1 != C2 && MergePredicate(C1, C2))
        {
            MergeRegions(C1, C2, set);
        }
    }
}

void SRM::MergeRegions(int C1, int C2, DisjointSet& set)
{
    // Algorithm from: http://www.lix.polytechnique.fr/~nielsen/Srmjava.java

    set.union_set(C1, C2);
    
    const int region = m_rank[C1] > m_rank[C2] ? C1 : C2;
    const int nreg = m_count[C1] + m_count[C2];

    m_average[region].r = (m_count[C1] * m_average[C1].r + m_count[C2] * m_average[C2].r) / (double)nreg;
    m_average[region].g = (m_count[C1] * m_average[C1].g + m_count[C2] * m_average[C2].g) / (double)nreg;
    m_average[region].b = (m_count[C1] * m_average[C1].b + m_count[C2] * m_average[C2].b) / (double)nreg;
    m_count[region] = nreg;
}

bool SRM::MergePredicate(int reg1, int reg2)
{
    // Algorithm from: http://www.lix.polytechnique.fr/~nielsen/Srmjava.java

    const double r = pow(m_average[reg1].r - m_average[reg2].r, 2);
    const double g = pow(m_average[reg1].g - m_average[reg2].g, 2);
    const double b = pow(m_average[reg1].b - m_average[reg2].b, 2);

    const double logreg1 = std::min(double(m_levels), (double)(m_count[reg1]) * log(1.0 + m_count[reg1]));
    const double logreg2 = std::min(double(m_levels), (double)(m_count[reg2]) * log(1.0 + m_count[reg2]));

    const double dev1 = (m_levelsSqr / (2.0 * m_complexity * m_count[reg1]))*(logreg1 + m_logdelta);
    const double dev2 = (m_levelsSqr / (2.0 * m_complexity * m_count[reg2]))*(logreg2 + m_logdelta);
    const double dev = dev1 + dev2;

    return r < dev && g < dev && b < dev;
}