#ifndef __mdkKNNReconstructionSparseEncoder_h
#define __mdkKNNReconstructionSparseEncoder_h

#include <string>
#include <limits>


#include "mdkFeatureDictionaryBasedSparseEncoder.h"
#include "mdkFeatureCoding_Common_Function.h"
#include "mdkLinearLeastSquaresProblemSolver.h"

// find K Nearest Neighbor [d_1, d_2, ..., d_k] from D by using KNNDistanceType
// find Alpha that minimizes||X - [d_1, d_2, d_k ]*Alpha||, by using Linear least suqares (Lsqlin) method
// constraints on Alpha can be added to Lsqlin, such as nonnegative, sum to 1, etc, ...
// X : m_FeatureData
// D : m_Dictionary->m_Record
// Alpha: m_CodeInSparseVectorList

namespace mdk
{

struct Parameter_Of_KNNReconstructionSparseEncoder
{
    int_max NeighbourNumber;

    std::string DistanceTypeForKNNSearch;
    // L1Distance
    // L2Distance
    // Correlation
    // KLDivergence

    bool Nonnegative;

    bool SumToOne;

//------------------------------

    Parameter_Of_KNNReconstructionSparseEncoder() { this->Clear(); }
    ~Parameter_Of_KNNReconstructionSparseEncoder() {}

    void Clear()
    {
        NeighbourNumber  = -1;
        DistanceTypeForKNNSearch.clear();
        Nonnegative      = false;
        SumToOne         = false;
    }

private:
    Parameter_Of_KNNReconstructionSparseEncoder(const Parameter_Of_KNNReconstructionSparseEncoder&) = delete;
    void operator=(const Parameter_Of_KNNReconstructionSparseEncoder&) = delete;
};


template<typename ElementType>
class KNNReconstructionSparseEncoder : public FeatureDictionaryBasedSparseEncoder<ElementType>
{
public:

    Parameter_Of_KNNReconstructionSparseEncoder m_Parameter;

protected:
    DenseMatrix<ElementType> m_GramianMatrix_DtD; // D'*D

public:

    KNNReconstructionSparseEncoder();

    ~KNNReconstructionSparseEncoder();

    //--------------------------------------------------------------------------------

    void Clear();

    bool CheckInput();

    bool ComputeGramianMatrix_DtD();

    bool Preprocess();

    bool Postprocess();
    //--------------------------------------------------------------------------------

    using FeatureDictionaryBasedSparseEncoder::EncodingFunction;

    inline void EncodingFunction(SparseVector<ElementType>& CodeInSparseColVector, const DenseMatrix<ElementType>& DataColVector, int_max ThreadIndex);

    //---------------------------------------------------------------------------------

    static bool Apply(DenseMatrix<ElementType>& OutputCodeInDenseMatrix,
                      const DenseMatrix<ElementType>* FeatureData,
                      const FeatureDictionary<ElementType>* Dictionary,
                      int_max NeighbourNumber,
                      int_max MaxNumberOfThreads = 1);

    static bool Apply(SparseMatrix<ElementType>& OutputCodenSparseMatrix,
                      const DenseMatrix<ElementType>* FeatureData,
                      const FeatureDictionary<ElementType>* Dictionary,
                      int_max NeighbourNumber,
                      int_max MaxNumberOfThreads = 1);

    static bool Apply(DenseMatrix<SparseMatrix<ElementType>>& OutputCodeInSparseColVectorList,
                      const DenseMatrix<ElementType>* FeatureData,
                      const FeatureDictionary<ElementType>* Dictionary,
                      int_max NeighbourNumber,
                      int_max MaxNumberOfThreads = 1);

private:
//deleted:
    KNNReconstructionSparseEncoder(const KNNReconstructionSparseEncoder&) = delete;

    void operator=(const KNNReconstructionSparseEncoder&) = delete;

};

}

#include "mdkKNNReconstructionSparseEncoder.hpp"

#endif