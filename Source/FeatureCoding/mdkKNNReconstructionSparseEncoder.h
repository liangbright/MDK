#ifndef __mdkKNNReconstructionSparseEncoder_h
#define __mdkKNNReconstructionSparseEncoder_h

#include <string>
#include <limits>

#include "mdkFeatureCoding_Common_Type.h"
#include "mdkFeatureCoding_Common_Function.h"
#include "mdkFeatureDictionaryBasedSparseEncoder.h"
#include "mdkLinearLeastSquaresProblemSolver.h"
#include "mdkKNNSoftAssignSparseEncoder.h"

// find K Nearest Neighbor [d_1, d_2, ..., d_k] from D by using KNNDistanceType
// find Alpha that minimizes||X - [d_1, d_2, d_k ]*Alpha||, by using Linear least suqares (Lsqlin) method
// constraints on Alpha can be added to Lsqlin, such as nonnegative, sum to 1, etc, ...
// X : m_FeatureData
// D : m_Dictionary->m_Record
// Alpha: m_CodeInSparseVectorList

namespace mdk
{

struct Constraint_on_Code_Of_KNNReconstruction_For_FeatureCoding
{
    bool CodeNonnegative;

    bool CodeSumToOne;
};


template<typename ElementType>
struct Parameter_Of_KNNReconstructionSparseEncoder
{
    int_max NeighbourNumber;

    VectorSimilarityTypeEnum SimilarityType; // to find KNN
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
        NeighbourNumber = -1;

        SimilarityType = VectorSimilarityTypeEnum::Unknown;

        CodeNonnegative = false;
        CodeSumToOne    = false;
    }

    void operator=(const Parameter_Of_KNNReconstructionSparseEncoder& InputParameter)
    {
        NeighbourNumber = InputParameter.NeighbourNumber;

        SimilarityType  = InputParameter.SimilarityType;

        CodeNonnegative = InputParameter.CodeNonnegative;
        CodeSumToOne    = InputParameter.CodeSumToOne;
    }

private:
    Parameter_Of_KNNReconstructionSparseEncoder(const Parameter_Of_KNNReconstructionSparseEncoder&) = delete;
};


template<typename ElementType>
class KNNReconstructionSparseEncoder : public FeatureDictionaryBasedSparseEncoder<ElementType>
{
public:

    Parameter_Of_KNNReconstructionSparseEncoder<ElementType> m_Parameter;

protected:

    // D'*D
    DenseMatrix<ElementType> m_GramianMatrix_DtD; 

public:

    KNNReconstructionSparseEncoder();

    ~KNNReconstructionSparseEncoder();

    //--------------------------------------------------------------------------------

    void Clear();

    bool CheckInput();

    bool ComputeGramianMatrix_DtD();

    bool Preprocess();

    bool Postprocess();

    //-------------------------------------------------------------------------------------------------

    static ElementType ComputeSimilarityBetweenTwoVectors(VectorSimilarityTypeEnum SimilarityType,
                                                          const DenseMatrix<ElementType>& VectorA, 
                                                          const DenseMatrix<ElementType>& VectorB,
                                                          ElementType Variance);

    static ElementType ComputeSimilarityBetweenTwoVectors(VectorSimilarityTypeEnum SimilarityType, 
                                                          const ElementType* VectorA, const ElementType* VectorB, int_max Length, 
                                                          ElementType Variance, bool CheckInput = true);

    //---------------------------------------------------------------------------------------------------

    static DenseMatrix<ElementType> ComputeCodeVector(const DenseMatrix<ElementType>& DataColVector,
                                                      const DenseMatrix<ElementType>& KNNBasisMatrix,
                                                      const DenseMatrix<int_max>&     KNNBasisIndexList,
                                                      const DenseMatrix<ElementType>& GramianMatrix_DtD,
                                                      bool CodeNonnegative,
                                                      bool CodeSumToOne);

    //----------------------------------------------------------------------------------------------------

    static bool Apply(DenseMatrix<ElementType>& OutputCodeInDenseMatrix,
                      const DenseMatrix<ElementType>* FeatureData,
                      const FeatureDictionary<ElementType>* Dictionary,
                      const Parameter_Of_KNNReconstructionSparseEncoder<ElementType>& Parameter,
                      int_max MaxNumberOfThreads = 1);

    static bool Apply(SparseMatrix<ElementType>& OutputCodeInSparseMatrix,
                      const DenseMatrix<ElementType>* FeatureData,
                      const FeatureDictionary<ElementType>* Dictionary,
                      const Parameter_Of_KNNReconstructionSparseEncoder<ElementType>& Parameter,
                      int_max MaxNumberOfThreads = 1);

    static bool Apply(DenseMatrix<SparseMatrix<ElementType>>& OutputCodeInSparseColVectorList,
                      const DenseMatrix<ElementType>* FeatureData,
                      const FeatureDictionary<ElementType>* Dictionary,
                      const Parameter_Of_KNNReconstructionSparseEncoder<ElementType>& Parameter,
                      int_max MaxNumberOfThreads = 1);

protected:
    inline void EncodingFunction(int_max DataIndex, int_max ThreadIndex);

private:
//deleted:
    KNNReconstructionSparseEncoder(const KNNReconstructionSparseEncoder&) = delete;

    void operator=(const KNNReconstructionSparseEncoder&) = delete;

};

}

#include "mdkKNNReconstructionSparseEncoder.hpp"

#endif