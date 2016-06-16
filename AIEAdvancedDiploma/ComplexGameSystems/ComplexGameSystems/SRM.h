////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - SRM.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <vector>
#include <memory>
#include "boost/pending/disjoint_sets.hpp"

/**
* Statistical Region Merging(SRM) Image Segmentation Reference
* www1.univ-ag.fr/~rnock/Articles/Drafts/tpami04-nn.pdf
*/
class SRM
{
public:

    SRM(int w, int h);
    ~SRM();
    void Execute(int* input, int* output, float vectorization);

private:

    /**
    * Union Find Data Structure to keep track of a set of elements
    * partitioned into a number of disjoint (nonoverlapping) subsets
    */
    typedef boost::disjoint_sets<int*, int*> DisjointSet;

    /**
    * An edge: two indices and a difference value
    */
    struct Edge
    {
        int r1 = 0;
        int r2 = 0;
        int diff = 0;
    };

    /**
    * 3 Component colour
    */
    struct Colour
    {
        float r, g, b;
    };

    /**
    * Main algorithm technique 
    * Reference: http://www.lix.polytechnique.fr/~nielsen/Srmjava.java
    */
    void Segmentation(int* input, DisjointSet& set);
    bool MergePredicate(int reg1, int reg2);
    void MergeRegions(int C1, int C2, DisjointSet& set);

    int m_width = 0;                  ///< Width of the image 
    int m_height = 0;                 ///< Height of the image 
    int m_size = 0;                   ///< width x height of the image 
    double m_complexity = 0.0;        ///< Complexity of regions generated
    int m_levels = 256;               ///< Number of levels in a color channel
    int m_levelsSqr = 0;              ///< Levels value squared
    double m_logdelta = 0.0;          ///< Constant for algorithm

    std::vector<int> m_count;
    std::vector<Colour> m_average;
    std::vector<Edge> m_order;
    std::vector<int> m_rank;
    std::vector<int> m_parent;
};