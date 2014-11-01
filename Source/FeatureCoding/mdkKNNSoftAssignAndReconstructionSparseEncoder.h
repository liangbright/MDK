#ifndef __mdkKNNSoftAssignAndReconstructionSparseEncoder_h
#define __mdkKNNSoftAssignAndReconstructionSparseEncoder_h

#include  <algorithm>

#include "mdkKNNReconstructionSparseEncoder.h"


namespace mdk
{
template<typename ScalarType>
struct Parameter_Of_KNNSoftAssignAndReconstructionSparseEncoder
{
    int_max NeighbourNumber;

    VectorSimilarityTypeEnum SimilarityType;
    // L1Distance
    // L2Distance
    // Correlation
    // KLDivergence

    // parameter for SoftAssign ----------------------------

    ScalarType SimilarityThreshold; // find KNN with Similarity >= SimilarityThreshold
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


template<typename Scalar_Type>
class KNNSoftAssignAndReconstructionSparseEncoder : public FeatureDictionaryBasedSparseEncoder<Scalar_Type>
{
public:
	typedef Scalar_Type ScalarType;

public:
    Parameter_Of_KNNSoftAssignAndReconstructionSparseEncoder<ScalarType> m_Parameter;

private:
    DenseMatrix<ScalarType> m_GramianMatrix_DtD;

    KNNSoftAssignSparseEncoder<ScalarType> m_KNNSoftAssignSparseEncoder;

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

	static DataArray<SparseVector<ScalarType>> Apply(const DenseMatrix<ScalarType>* FeatureData,
													 const FeatureDictionary<ScalarType>* Dictionary,
													 int_max NeighbourNumber,
													 int_max MaxNumberOfThread = 1);

private:
	inline SparseVector<ScalarType> EncodeSingleDataVector(int_max DataIndex, const DenseMatrix<ScalarType>& DataColVector, int_max ThreadIndex);

    inline DenseMatrix<ScalarType> ReconstructDataColVector(const DenseMatrix<ScalarType>& KNNBasisMatrix,
                                                            const DenseMatrix<ScalarType>& CodeVector);

    inline DenseMatrix<ScalarType> ComputeMembershipUsingReconstructedDataColVector(const DenseMatrix<ScalarType>& ReconstructedDataColVector,
                                                                                    const DenseMatrix<ScalarType>& KNNBasisMatrix);

private:
//deleted:
    KNNSoftAssignAndReconstructionSparseEncoder(const KNNSoftAssignAndReconstructionSparseEncoder&) = delete;

    void operator=(const KNNSoftAssignAndReconstructionSparseEncoder&) = delete;

};

}// namespace mdk

#include "mdkKNNSoftAssignAndReconstructionSparseEncoder.hpp"

#endif