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

    SRM(int w, int h);
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

    void InitializeSegmentation(int* input);
    void Segmentation(int* input);
    bool MergePredicate(int reg1, int reg2);
    void MergeRegions(int C1, int C2);
    void OutputSegmentation(int* output);
    void DrawBorder(int* output);

    /**
    * Sort the edges according to the maximum color channel difference
    */
    void BucketSortOrder();

    int width, height, size;
    double complexity = 0.0;        ///< Complexity of regions generated
    int levels = 256;               ///< Number of levels in a color channel
    double logdelta = 0.0;
    int edges = 0;

    std::unique_ptr<UnionFind> UF;
    std::vector<int> N;
    std::vector<double> Ravg;
    std::vector<double> Gavg;
    std::vector<double> Bavg;
    std::vector<int> nbe;
    std::vector<int> cnbe;
    std::vector<Rmpair> order;
    std::vector<Rmpair> order2;
};