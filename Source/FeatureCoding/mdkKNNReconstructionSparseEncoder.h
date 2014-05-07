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

    bool UpdateWithPreviousKNNSearchResult();

    //---------------------------------------------------------------------------------------------------

    static DenseMatrix<ElementType> ComputeKNNCode(const DenseMatrix<ElementType>& DataColVector,
                                                   const DenseMatrix<ElementType>& KNNBasisMatrix,
                                                   const DenseMatrix<int_max>&     KNNBasisIndexList,
                                                   const DenseMatrix<ElementType>& GramianMatrix_DtD,
                                                   bool CodeNonnegative,
                                                   bool CodeSumToOne);

    static DenseMatrix<ElementType> ComputeKNNCode(const DenseMatrix<ElementType>& DataColVector,
                                                   const DenseMatrix<ElementType>& KNNBasisMatrix,
                                                   bool CodeNonnegative,
                                                   bool CodeSumToOne);

    //----------------------------------------------------------------------------------------------------

    static void UpdateReconstructionCode(DataContainer<SparseVector<ElementType>>& ReconstructionCodeSet, 
                                         const DenseMatrix<ElementType>&  FeatureData,                                                                                
                                         const DenseMatrix<ElementType>&  BasisMatrix,
                                         bool CodeNonnegative,
                                         bool CodeSumToOne,
                                         int_max MaxNumberOfThreads = 1);

    static DataContainer<SparseVector<ElementType>> 
        ComputeReconstructionCodeFromSimilarityCode(const DenseMatrix<ElementType>&  FeatureData, 
                                                    const DataContainer<SparseVector<ElementType>>& SimilarityCodeSet,
                                                    const DenseMatrix<ElementType>&  BasisMatrix,
                                                    bool CodeNonnegative,
                                                    bool CodeSumToOne,
                                                    int_max MaxNumberOfThreads = 1);

    static DataContainer<SparseVector<ElementType>> 
        ComputeReconstructionCodeFromSoftAssignCode(const DenseMatrix<ElementType>&  FeatureData, 
                                                    const DataContainer<SparseVector<ElementType>>& SoftAssignCodeSet,
                                                    const DenseMatrix<ElementType>&  BasisMatrix,
                                                    bool CodeNonnegative,
                                                    bool CodeSumToOne,
                                                    int_max MaxNumberOfThreads = 1);

    //----------------------------------------------------------------------------------------------------

    static DenseMatrix<ElementType> ReconstructData(const SparseVector<ElementType>& ReconstructionCode,
                                                    const DenseMatrix<ElementType>&  BasisMatrix);

    static void ReconstructData(DenseMatrix<ElementType>& ReconstructedDataVector,
                                const SparseVector<ElementType>& ReconstructionCode,
                                const DenseMatrix<ElementType>&  BasisMatrix);


    static DenseMatrix<ElementType> ReconstructData(const DataContainer<SparseVector<ElementType>>& ReconstructionCodeSet,
                                                    const DenseMatrix<ElementType>&  BasisMatrix,
                                                    int_max MaxNumberOfThreads = 1);

    static void ReconstructData(DenseMatrix<ElementType>& ReconstructedDataSet,
                                const DataContainer<SparseVector<ElementType>>& ReconstructionCodeSet,
                                const DenseMatrix<ElementType>&  BasisMatrix,
                                int_max MaxNumberOfThreads = 1);

    static ElementType ComputeReconstructionErrorL2Norm(const DenseMatrix<ElementType>&  DataColVector, 
                                                        const SparseVector<ElementType>& ReconstructionCodeSet,
                                                        const DenseMatrix<ElementType>&  BasisMatrix);

    static DenseMatrix<ElementType> ComputeReconstructionErrorL2Norm(const DenseMatrix<ElementType>&  FeatureData,
                                                                     const DataContainer<SparseVector<ElementType>>& ReconstructionCodeSet,
                                                                     const DenseMatrix<ElementType>&  BasisMatrix,
                                                                     int_max MaxNumberOfThreads = 1);

    static void ComputeReconstructionErrorL2Norm(DenseMatrix<ElementType>& ErrorL2NormList,
                                                 const DenseMatrix<ElementType>&  FeatureData,
                                                 const DataContainer<SparseVector<ElementType>>& ReconstructionCodeSet,
                                                 const DenseMatrix<ElementType>&  BasisMatrix,
                                                 int_max MaxNumberOfThreads = 1);

    //----------------------------------------------------------------------------------------------------

    static void Apply(DenseMatrix<ElementType>& OutputCodeInDenseMatrix,
                      const DenseMatrix<ElementType>* FeatureData,
                      const FeatureDictionary<ElementType>* Dictionary,
                      const Parameter_Of_KNNReconstructionSparseEncoder<ElementType>& Parameter,
                      int_max MaxNumberOfThreads = 1);

    static void Apply(SparseMatrix<ElementType>& OutputCodeInSparseMatrix,
                      const DenseMatrix<ElementType>* FeatureData,
                      const FeatureDictionary<ElementType>* Dictionary,
                      const Parameter_Of_KNNReconstructionSparseEncoder<ElementType>& Parameter,
                      int_max MaxNumberOfThreads = 1);

    static void Apply(DenseMatrix<SparseMatrix<ElementType>>& OutputCodeInSparseColVectorSet,
                      const DenseMatrix<ElementType>* FeatureData,
                      const FeatureDictionary<ElementType>* Dictionary,
                      const Parameter_Of_KNNReconstructionSparseEncoder<ElementType>& Parameter,
                      int_max MaxNumberOfThreads = 1);

protected:
    bool Preprocess();

    bool Postprocess();

    bool ComputeGramianMatrix_DtD();

    inline void EncodingFunction(int_max DataIndex, int_max ThreadIndex);


private:
//deleted:
    KNNReconstructionSparseEncoder(const KNNReconstructionSparseEncoder&) = delete;

    void operator=(const KNNReconstructionSparseEncoder&) = delete;

};

}

#include "mdkKNNReconstructionSparseEncoder.hpp"

#endif