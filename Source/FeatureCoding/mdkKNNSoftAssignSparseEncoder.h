#ifndef __mdkKNNSoftAssignSparseEncoder_h
#define __mdkKNNSoftAssignSparseEncoder_h

#include <string>

#include "mdkFeatureDictionaryBasedSparseEncoder.h"


namespace mdk
{

struct Parameter_Of_KNNSoftAssignSparseEncoder
{
    int_max NeighbourNumber;

    std::string DistanceType;
    //
    // If a Feature vector is treated as ordinary signal
    // L1Distance
    // L2Distance
    // Correlation
    //
    // If a Feature vector is normalized histogram or probability mass function (PMF), i.e., discrete probability distribution
    // KLDivergence

    Parameter_Of_KNNSoftAssignSparseEncoder() { this->Clear(); }
    ~Parameter_Of_KNNSoftAssignSparseEncoder() {}

    void Clear()
    {
        NeighbourNumber = -1;

        DistanceType.clear();
    }
};


template<typename ElementType>
class KNNSoftAssignSparseEncoder : public FeatureDictionaryBasedSparseEncoder<ElementType>
{

public:

    Parameter_Of_KNNSoftAssignSparseEncoder m_Paramter;

public:

    KNNSoftAssignSparseEncoder();

    ~KNNSoftAssignSparseEncoder();

    //-----------------------------------------

    void Clear();

    bool CheckInput();
    //-----------------------------------------

    using FeatureDictionaryBasedSparseEncoder::EncodingFunction;

    inline void EncodingFunction(SparseVector<ElementType>& CodeInSparseColVector, const DenseMatrix<ElementType>& DataColVector, int_max ThreadIndex);


private:
//deleted:
    KNNSoftAssignSparseEncoder(const KNNSoftAssignSparseEncoder&) = delete;

    void operator=(const KNNSoftAssignSparseEncoder&) = delete;

};

}

#include "mdkKNNSoftAssignSparseEncoder.hpp"

#endif