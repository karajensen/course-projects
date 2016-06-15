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
    m_edges = 2 * (w - 1)*(h - 1) + (h - 1) + (w - 1);
    m_levelsSqr = m_levels * m_levels;

    m_average.resize(m_size);
    m_N.resize(m_size);
    m_order.resize(m_edges);
    m_parent.resize(m_size);
    m_rank.resize(m_size);
}

void SRM::Execute(int* input, int* output, float vectorization)
{
    // complexity is inverse to vectorization range [0,1]
    m_complexity = 30.0 + ((1.0 - vectorization) * 300.0);

    std::iota(m_parent.begin(), m_parent.end(), 0);
    m_N.assign(m_N.size(), 1);
    m_rank.assign(m_rank.size(), 0);
    DisjointSet set(&m_rank[0], &m_parent[0]);

    for (size_t i = 0; i < m_average.size(); ++i)
    {
        m_average[i].r = Red(input[i]);
        m_average[i].g = Green(input[i]);
        m_average[i].b = Blue(input[i]);
    }
    
    Segmentation(input, set);
    
    for (size_t i = 0; i < m_average.size(); ++i)
    {
        const auto& c = m_average[set.find_set(i)];
        output[i] = ToColour(c.r, c.g, c.b);
    }
}

void SRM::Segmentation(int* input, DisjointSet& set)
{
    int cpair = 0;

    // Segmentation to move to GPU
    for (int y = 0; y < m_height - 1; y++)
    {
        for (int x = 0; x < m_width - 1; x++)
        {
            int index = y*m_width + x;

            m_order[cpair].r1 = index;
            m_order[cpair].r2 = index + 1;

            int r1 = Red(input[index]);
            int g1 = Green(input[index]);
            int b1 = Blue(input[index]);

            int r2 = Red(input[index + 1]);
            int g2 = Green(input[index + 1]);
            int b2 = Blue(input[index + 1]);

            m_order[cpair].diff = (int)Max(std::abs(r2 - r1), std::abs(g2 - g1), std::abs(b2 - b1));
            cpair++;

            m_order[cpair].r1 = index;
            m_order[cpair].r2 = index + m_width;

            r2 = Red(input[index + m_width]);
            g2 = Green(input[index + m_width]);
            b2 = Blue(input[index + m_width]);

            m_order[cpair].diff = (int)Max(std::abs(r2 - r1), std::abs(g2 - g1), std::abs(b2 - b1));
            cpair++;
        }
    }

    std::sort(m_order.begin(), m_order.end(),
        [](const SRM::Edge& e1, const SRM::Edge& e2)
    {
        return e1.diff < e2.diff;
    });

    for (int i = 0; i < m_edges; i++)
    {
        int reg1 = m_order[i].r1;
        int C1 = set.find_set(reg1);
        int reg2 = m_order[i].r2;
        int C2 = set.find_set(reg2);

        if (C1 != C2 && MergePredicate(C1, C2))
        {
            MergeRegions(C1, C2, set);
        }
    }
}

void SRM::MergeRegions(int C1, int C2, DisjointSet& set)
{
    set.union_set(C1, C2);
    
    const int reg = m_rank[C1] > m_rank[C2] ? C1 : C2;
    const int nreg = m_N[C1] + m_N[C2];

    m_average[reg].r = (m_N[C1] * m_average[C1].r + m_N[C2] * m_average[C2].r) / (double)nreg;
    m_average[reg].g = (m_N[C1] * m_average[C1].g + m_N[C2] * m_average[C2].g) / (double)nreg;
    m_average[reg].b = (m_N[C1] * m_average[C1].b + m_N[C2] * m_average[C2].b) / (double)nreg;
    m_N[reg] = nreg;
}

bool SRM::MergePredicate(int reg1, int reg2)
{
    const double dR = pow(m_average[reg1].r - m_average[reg2].r, 2);
    const double dG = pow(m_average[reg1].g - m_average[reg2].g, 2);
    const double dB = pow(m_average[reg1].b - m_average[reg2].b, 2);

    const double logreg1 = std::min(double(m_levels), (double)(m_N[reg1]) * log(1.0 + m_N[reg1]));
    const double logreg2 = std::min(double(m_levels), (double)(m_N[reg2]) * log(1.0 + m_N[reg2]));

    const double dev1 = (m_levelsSqr / (2.0 * m_complexity * m_N[reg1]))*(logreg1 + m_logdelta);
    const double dev2 = (m_levelsSqr / (2.0 * m_complexity * m_N[reg2]))*(logreg2 + m_logdelta);

    const double dev = dev1 + dev2;

    return dR < dev && dG < dev && dB < dev;
}