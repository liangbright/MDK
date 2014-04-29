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

    std::string SimilarityType; // to find KNN
    // L1Distance
    // L2Distance
    // Correlation
    // KLDivergence

    // L1Distance/L2Distance/Correlation/KLDivergence is directly used to find KNN, i.e., not converted to similarity

    bool CodeNonnegative;

    bool CodeSumToOne;

//------------------------------

    Parameter_Of_KNNReconstructionSparseEncoder() { this->Clear(); }
    ~Parameter_Of_KNNReconstructionSparseEncoder() {}

    void Clear()
    {
        SimilarityType.clear();
        NeighbourNumber = -1;
        CodeNonnegative = false;
        CodeSumToOne    = false;
    }

    void operator=(const Parameter_Of_KNNReconstructionSparseEncoder& InputParameter)
    {
        SimilarityType = InputParameter.SimilarityType;
        NeighbourNumber = InputParameter.NeighbourNumber;
        CodeNonnegative = InputParameter.CodeNonnegative;
        CodeSumToOne = InputParameter.CodeSumToOne;
    }

private:
    Parameter_Of_KNNReconstructionSparseEncoder(const Parameter_Of_KNNReconstructionSparseEncoder&) = delete;
};


template<typename ElementType>
class KNNReconstructionSparseEncoder : public FeatureDictionaryBasedSparseEncoder<ElementType>
{
public:

    Parameter_Of_KNNReconstructionSparseEncoder m_Parameter;


protected:
    // output reconstruction error
    DenseMatrix<ElementType>* m_ReconstructionErrorNorm;

    DenseMatrix<ElementType> m_ReconstructionErrorNorm_SharedCopy;

    // D'*D
    DenseMatrix<ElementType> m_GramianMatrix_DtD; 

public:

    KNNReconstructionSparseEncoder();

    ~KNNReconstructionSparseEncoder();

    //--------------------------------------------------------------------------------

    void Clear();

    void SetOutputReconstructionErrorNorm(DenseMatrix<ElementType>* ErrorNorm);

    DenseMatrix<ElementType>* GetOutputReconstructionErrorNorm();

    bool CheckInput();

    bool ComputeGramianMatrix_DtD();

    bool Preprocess();

    bool Postprocess();

    //---------------------------------------------------------------------------------

    static bool Apply(DenseMatrix<ElementType>& OutputCodeInDenseMatrix,
                      const DenseMatrix<ElementType>* FeatureData,
                      const FeatureDictionary<ElementType>* Dictionary,
                      const Parameter_Of_KNNReconstructionSparseEncoder& Parameter,
                      int_max MaxNumberOfThreads = 1);

    static bool Apply(SparseMatrix<ElementType>& OutputCodenSparseMatrix,
                      const DenseMatrix<ElementType>* FeatureData,
                      const FeatureDictionary<ElementType>* Dictionary,
                      const Parameter_Of_KNNReconstructionSparseEncoder& Parameter,
                      int_max MaxNumberOfThreads = 1);

    static bool Apply(DenseMatrix<SparseMatrix<ElementType>>& OutputCodeInSparseColVectorList,
                      const DenseMatrix<ElementType>* FeatureData,
                      const FeatureDictionary<ElementType>* Dictionary,
                      const Parameter_Of_KNNReconstructionSparseEncoder& Parameter,
                      int_max MaxNumberOfThreads = 1);

protected:
    inline void EncodingFunction(int_max DataIndex, int_max ThreadIndex);

    virtual void SetupDefaultPipelineOutput();

    virtual void UpdatePipelineOutput();

private:
//deleted:
    KNNReconstructionSparseEncoder(const KNNReconstructionSparseEncoder&) = delete;

    void operator=(const KNNReconstructionSparseEncoder&) = delete;

};

}

#include "mdkKNNReconstructionSparseEncoder.hpp"

#endif