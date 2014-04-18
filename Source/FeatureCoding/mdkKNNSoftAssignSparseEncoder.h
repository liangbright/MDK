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
    // L1Distance
    // L2Distance
    // Correlation

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

    inline void EncodingFunction(const DenseMatrix<ElementType>& DataColVector, SparseVector<ElementType>& CodeInSparseColVector);


private:
//deleted:
    KNNSoftAssignSparseEncoder(const KNNSoftAssignSparseEncoder&) = delete;

    void operator=(const KNNSoftAssignSparseEncoder&) = delete;

};

}

#include "mdkKNNSoftAssignSparseEncoder.hpp"

#endif