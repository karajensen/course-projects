////////////////////////////////////////////////////////////////////////////////////////
// Kara Jensen - mail@karajensen.com - SRM.h
////////////////////////////////////////////////////////////////////////////////////////

#pragma once

#include <vector>
#include <memory>

class UnionFind;

/**
* Statistical Region Merging(SRM) Image Segmentation
* Reference: www1.univ-ag.fr/~rnock/Articles/Drafts/tpami04-nn.pdf
* Reference: http://www.lix.polytechnique.fr/~nielsen/Srmjava.java
*/
class SRM
{
public:

    SRM(int width, int height);
    ~SRM();
    void Execute(int* input, int* output, float vectorization, bool border);

private:

    /**
    * An edge: two indices and a difference value
    */
    struct Rmpair
    {
        int r1 = 0;
        int r2 = 0;
        int diff = 0;
    };

    void FullSegmentation(int* intput, int* output, bool border);
    void InitializeSegmentation(int* input);
    void Segmentation(int* input);
    bool MergePredicate(int reg1, int reg2);
    void MergeRegions(int C1, int C2);
    void MergeSmallRegion();
    void OutputSegmentation(int* output);
    void DrawBorder(int* output);

    /**
    * Sort the edges according to the maximum color channel difference
    */
    void BucketSortOrder();

    int w, h, n;
    double aspectratio = 0.0;
    double complexity = 0.0;        ///< Complexity of regions generated
    int levels = 256;               ///< Number of levels in a color channel
    int smallregion = 0;            ///< Number of pixels that define a "small region" to be collapsed
    double logdelta = 0.0;
    int edges = 0;

    std::unique_ptr<UnionFind> UF;
    std::vector<int> N;
    std::vector<double> Ravg;
    std::vector<double> Gavg;
    std::vector<double> Bavg;
    std::vector<int> C; // the class number
    std::vector<Rmpair> order;
    std::vector<int> nbe;
    std::vector<int> cnbe;
    std::vector<Rmpair> order2;
};