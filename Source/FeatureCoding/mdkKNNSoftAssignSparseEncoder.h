#ifndef __mdkKNNSoftAssignSparseEncoder_h
#define __mdkKNNSoftAssignSparseEncoder_h

#include <string>

#include "mdkFeatureDictionaryBasedSparseEncoder.h"


namespace mdk
{

template<typename ElementType>
class KNNSoftAssignSparseEncoder : public FeatureDictionaryBasedSparseEncoder<ElementType>
{

public:

    int_max m_NeighbourNumber;

public:

    KNNSoftAssignSparseEncoder();

    ~KNNSoftAssignSparseEncoder();

    //-----------------------------------------

    void Clear();

    bool CheckInput();
    //-----------------------------------------

    using FeatureDictionaryBasedSparseEncoder::EncodingFunction;

    inline void EncodingFunction(const DenseMatrix<ElementType>& DataColVector, SparseMatrix<ElementType>& CodeInSparseColVector);


private:
//deleted:
    KNNSoftAssignSparseEncoder(const KNNSoftAssignSparseEncoder&) = delete;

    void operator=(const KNNSoftAssignSparseEncoder&) = delete;

};

}

#include "mdkKNNSoftAssignSparseEncoder.hpp"

#endif