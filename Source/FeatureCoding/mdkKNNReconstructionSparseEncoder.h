#ifndef __mdkKNNReconstructionSparseEncoder_h
#define __mdkKNNReconstructionSparseEncoder_h

#include <string>

#include "mdkFeatureDictionaryBasedSparseEncoder.h"
#include "mdkFeatureCoding_Common_Function.h"


namespace mdk
{

template<typename ElementType>
class KNNReconstructionSparseEncoder : public FeatureDictionaryBasedSparseEncoder<ElementType>
{

public:

    KNNReconstructionSparseEncoder();

    ~KNNReconstructionSparseEncoder();

    //-----------------------------------------

    void Clear();

    bool SetNeighbourNumber(int64 NeighbourNumber); // the number of nearest neighbors, i.e., K

    //-----------------------------------------

    static DenseMatrix<ElementType> Apply(const DenseMatrix<ElementType>* FeatureData, 
                                          const FeatureDictionary<ElementType>* Dictionary,
                                          int64 NeighbourNumber = 3,
                                          bool  Flag_OutputCodeInCompactFormat = true, // CompactFormat in default
                                          int64 MaxNumberOfThreads = 1);

    static bool Apply(DenseMatrix<ElementType>& OutputFeatureCode, 
                      const DenseMatrix<ElementType>* FeatureData,
                      const FeatureDictionary<ElementType>* Dictionary,
                      int64 NeighbourNumber = 3,
                      bool  Flag_OutputCodeInCompactFormat = true, // CompactFormat in default
                      int64 MaxNumberOfThreads = 1);

    static bool Apply(SparseMatrix<ElementType>& OutputFeatureCode,
                      const DenseMatrix<ElementType>* FeatureData,
                      const FeatureDictionary<ElementType>* Dictionary,
                      int64 NeighbourNumber = 3,
                      int64 MaxNumberOfThreads = 1);

protected:

    bool CheckInputAndOutput();

    void EncodingFunction(int64 IndexOfFeatureVector);

private:
//deleted:
    KNNReconstructionSparseEncoder(const KNNReconstructionSparseEncoder&) = delete;

    void operator=(const KNNReconstructionSparseEncoder&) = delete;

};

}

#include "mdkKNNReconstructionSparseEncoder.hpp"

#endif