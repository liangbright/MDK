#ifndef __mdkKNNSoftAssignSparseEncoder_h
#define __mdkKNNSoftAssignSparseEncoder_h

#include "mdkString.h"
#include "mdkFeatureDictionaryBasedSparseEncoder.h"


namespace mdk
{
template<typename ElementType>
struct Parameter_Of_KNNSoftAssignSparseEncoder
{
    int_max NeighbourNumber;

    std::string SimilarityType;
    //
    // If a Feature vector is treated as ordinary signal
    // L1Distance
    // L2Distance
    // Correlation
    //
    // If a Feature vector is normalized histogram or probability mass function (PMF), i.e., discrete probability distribution
    // KLDivergence

    ElementType SimilarityThreshold; // find KNN with Similarity >= SimilarityThreshold
    // K in KNN can be < MaxNumberOfNeighbours

    // Sigma_L1, Sigma_L2, and Sigma_KL may be carried by Dictionary 

    ElementType Sigma_L1; // standard deviation to convert L1Distance to Similarity

    ElementType Sigma_L2; // standard deviation to convert L2Distance to Similarity

    ElementType Sigma_KL; // standard deviation to convert KLDivergence to Similarity

    bool IgnoreSign_Correlation; // if it is true, Similarity = abs(Correlation)
    // else, Similarity = (Correlation +1)/2

    Parameter_Of_KNNSoftAssignSparseEncoder() { this->Clear(); }
    ~Parameter_Of_KNNSoftAssignSparseEncoder() {}

    void Clear()
    {
        MaxNumberOfNeighbours = 0;

        SimilarityType.clear();

        SimilarityThreshold = 0;

        Sigma_L1 = 0;

        Sigma_L2 = 0;

        Sigma_KL = 0;

        IgnoreSign_Correlation = false;
    }
};


template<typename ElementType>
class KNNSoftAssignSparseEncoder : public FeatureDictionaryBasedSparseEncoder<ElementType>
{

public:

    Parameter_Of_KNNSoftAssignSparseEncoder<ElementType> m_Paramter;

public:

    KNNSoftAssignSparseEncoder();

    ~KNNSoftAssignSparseEncoder();

    //-----------------------------------------

    void Clear();

    bool CheckInput();
    //-----------------------------------------

protected:
    inline void EncodingFunction(int_max DataIndex, int_max ThreadIndex);

private:
//deleted:
    KNNSoftAssignSparseEncoder(const KNNSoftAssignSparseEncoder&) = delete;

    void operator=(const KNNSoftAssignSparseEncoder&) = delete;

};

}

#include "mdkKNNSoftAssignSparseEncoder.hpp"

#endif