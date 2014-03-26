#ifndef __mdkKNNReconstructionSparseEncoder_h
#define __mdkKNNReconstructionSparseEncoder_h

#include <string>

#include "mdkFeatureDictionaryBasedSparseEncoder.hpp"


namespace mdk
{

template<typename ElementType>
class KNNReconstructionSparseEncoder : public FeatureDictionaryBasedSparseEncoder<ElementType>
{

private: 

    int64 m_NeighbourNumber;

public:

    KNNReconstructionSparseEncoder();

    ~KNNReconstructionSparseEncoder();

    //-----------------------------------------

    void Clear();

    bool SetNeighbourNumber(int64 NeighbourNumber);

    //-----------------------------------------

    static DenseMatrix<ElementType> Apply();

protected:

    bool CheckInputAndOutput();

    bool EncodingFunction(int64 IndexOfFeatureVector);

private:
//deleted:
    KNNReconstructionSparseEncoder(const KNNReconstructionSparseEncoder&) = delete;

    void operator=(const KNNReconstructionSparseEncoder&) = delete;

};

}

#include "mdkKNNReconstructionSparseEncoder.hpp"

#endif