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

protected:

    int_max m_NeighbourNumber;

public:

    KNNReconstructionSparseEncoder();

    ~KNNReconstructionSparseEncoder();

    //--------------------------------------------------------------------------------

    void Clear();

    bool SetNeighbourNumber(int_max NeighbourNumber); // the number of nearest neighbors, i.e., K

    bool CheckInputAndOutput();

    //--------------------------------------------------------------------------------

    using FeatureDictionaryBasedSparseEncoder::EncodingFunction;

    inline void EncodingFunction(const DenseMatrix<ElementType>& FeatureData,
                                 SparseMatrix<ElementType>& Code);

    //---------------------------------------------------------------------------------

    static bool Apply(DenseMatrix<ElementType>& OutputCode, 
                      const DenseMatrix<ElementType>* FeatureData,
                      const FeatureDictionary<ElementType>* Dictionary,
                      int_max NeighbourNumber = 3,
                      int_max MaxNumberOfThreads = 1);

    static bool Apply(SparseMatrix<ElementType>& OutputCode,
                      const DenseMatrix<ElementType>* FeatureData,
                      const FeatureDictionary<ElementType>* Dictionary,
                      int_max NeighbourNumber = 3,
                      int_max MaxNumberOfThreads = 1);

    static bool Apply(std::vector<SparseMatrix<ElementType>>& OutputCode,
                      const DenseMatrix<ElementType>* FeatureData,
                      const FeatureDictionary<ElementType>* Dictionary,
                      int_max NeighbourNumber = 3,
                      int_max MaxNumberOfThreads = 1);

private:
//deleted:
    KNNReconstructionSparseEncoder(const KNNReconstructionSparseEncoder&) = delete;

    void operator=(const KNNReconstructionSparseEncoder&) = delete;

};

}

#include "mdkKNNReconstructionSparseEncoder.hpp"

#endif