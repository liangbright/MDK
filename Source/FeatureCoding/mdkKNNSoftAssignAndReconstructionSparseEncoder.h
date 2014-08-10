#ifndef __mdkKNNSoftAssignAndReconstructionSparseEncoder_h
#define __mdkKNNSoftAssignAndReconstructionSparseEncoder_h

#include  <algorithm>

#include "mdkKNNReconstructionSparseEncoder.h"


namespace mdk
{
template<typename ElementType>
struct Parameter_Of_KNNSoftAssignAndReconstructionSparseEncoder
{
    int_max NeighbourNumber;

    VectorSimilarityTypeEnum SimilarityType;
    // L1Distance
    // L2Distance
    // Correlation
    // KLDivergence

    // parameter for SoftAssign ----------------------------

    ElementType SimilarityThreshold; // find KNN with Similarity >= SimilarityThreshold
                                     // K in KNN can be < NeighbourNumber

    // parameter for Reconstruction ----------------------------

    bool CodePositive;

    bool CodeSumToOne;

//--------------------------------------------------------------------------------------------
    Parameter_Of_KNNSoftAssignAndReconstructionSparseEncoder() { this->Clear(); }
    ~Parameter_Of_KNNSoftAssignAndReconstructionSparseEncoder() {}

    void Clear()
    {
        NeighbourNumber  = 0;

        SimilarityType = VectorSimilarityTypeEnum::Unknown;

        CodePositive = false;
        CodeSumToOne = false;
    }

private:
    Parameter_Of_KNNSoftAssignAndReconstructionSparseEncoder(const Parameter_Of_KNNSoftAssignAndReconstructionSparseEncoder&) = delete;
    void operator=(const Parameter_Of_KNNSoftAssignAndReconstructionSparseEncoder&) = delete;
};


template<typename Element_Type>
class KNNSoftAssignAndReconstructionSparseEncoder : public FeatureDictionaryBasedSparseEncoder<Element_Type>
{
public:
	typedef Element_Type ElementType;

public:
    Parameter_Of_KNNSoftAssignAndReconstructionSparseEncoder<ElementType> m_Parameter;

private:
    DenseMatrix<ElementType> m_GramianMatrix_DtD;

    KNNSoftAssignSparseEncoder<ElementType> m_KNNSoftAssignSparseEncoder;

public:

    KNNSoftAssignAndReconstructionSparseEncoder();

    ~KNNSoftAssignAndReconstructionSparseEncoder();

    //--------------------------------------------------------------------------------

    void Clear();

    bool CheckInput();

    bool ComputeGramianMatrix_DtD();

    bool Preprocess();

    bool Postprocess();

    //---------------------------------------------------------------------------------

    static bool Apply(DenseMatrix<ElementType>& OutputCodeInDenseMatrix,
                      const DenseMatrix<ElementType>* FeatureData,
                      const FeatureDictionary<ElementType>* Dictionary,
                      int_max NeighbourNumber,
                      int_max MaxNumberOfThreads = 1);

    static bool Apply(SparseMatrix<ElementType>& OutputCodeInSparseMatrix,
                      const DenseMatrix<ElementType>* FeatureData,
                      const FeatureDictionary<ElementType>* Dictionary,
                      int_max NeighbourNumber,
                      int_max MaxNumberOfThreads = 1);

    static bool Apply(DataContainer<SparseVector<ElementType>>& OutputCodeInSparseColVectorSet,
                      const DenseMatrix<ElementType>* FeatureData,
                      const FeatureDictionary<ElementType>* Dictionary,
                      int_max NeighbourNumber,
                      int_max MaxNumberOfThreads = 1);

private:

    inline void EncodingFunction(int_max DataIndex, int_max ThreadIndex);

    inline DenseMatrix<ElementType> ReconstructDataColVector(const DenseMatrix<ElementType>& KNNBasisMatrix,
                                                             const DenseMatrix<ElementType>& CodeVector);

    inline DenseMatrix<ElementType> ComputeMembershipUsingReconstructedDataColVector(const DenseMatrix<ElementType>& ReconstructedDataColVector,
                                                                                     const DenseMatrix<ElementType>& KNNBasisMatrix);

private:
//deleted:
    KNNSoftAssignAndReconstructionSparseEncoder(const KNNSoftAssignAndReconstructionSparseEncoder&) = delete;

    void operator=(const KNNSoftAssignAndReconstructionSparseEncoder&) = delete;

};

}

#include "mdkKNNSoftAssignAndReconstructionSparseEncoder.hpp"

#endif