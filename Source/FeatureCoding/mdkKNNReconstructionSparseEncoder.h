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


template<typename ScalarType>
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
        NeighbourNumber = 0;

        SimilarityType  = VectorSimilarityTypeEnum::Unknown;

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


template<typename Scalar_Type>
class KNNReconstructionSparseEncoder : public FeatureDictionaryBasedSparseEncoder<Scalar_Type>
{
public:
	typedef Scalar_Type ScalarType;

public:
    Parameter_Of_KNNReconstructionSparseEncoder<ScalarType> m_Parameter;

protected:
    // D'*D
    DenseMatrix<ScalarType> m_GramianMatrix_DtD; 

public:
    KNNReconstructionSparseEncoder();
    ~KNNReconstructionSparseEncoder();

    //--------------------------------------------------------------------------------

    void Clear();

    bool CheckInput();

    bool UpdateWithPreviousKNNSearchResult();

    void GetReconstructedData(DenseMatrix<ScalarType>& ReconstructedDataSet);

	inline SparseVector<ScalarType> EncodeSingleDataVector(const DenseMatrix<ScalarType>& DataColVector);
    //---------------------------------------------------------------------------------------------------

    static inline bool CheckIfSimilarityTypeSupported(VectorSimilarityTypeEnum SimilarityType);

    //---------------------------------------------------------------------------------------------------

    static DenseMatrix<ScalarType> ComputeKNNCode(const DenseMatrix<ScalarType>& DataColVector,
                                                   const DenseMatrix<ScalarType>& KNNBasisMatrix,
                                                   const DenseMatrix<int_max>&     KNNBasisIndexList,
                                                   const DenseMatrix<ScalarType>& GramianMatrix_DtD,
                                                   bool CodeNonnegative,
                                                   bool CodeSumToOne);

    static DenseMatrix<ScalarType> ComputeKNNCode(const DenseMatrix<ScalarType>& DataColVector,
                                                   const DenseMatrix<ScalarType>& KNNBasisMatrix,
                                                   bool CodeNonnegative,
                                                   bool CodeSumToOne);

    //----------------------------------------------------------------------------------------------------

	static void UpdateReconstructionCodeWithKNNSearchResult(ObjectArray<SparseVector<ScalarType>>& ReconstructionCodeSet,
                                                            const DenseMatrix<ScalarType>&  FeatureData,                                                                                
														    const DenseMatrix<ScalarType>&  BasisMatrix,
														    bool CodeNonnegative,
															bool CodeSumToOne,
															int_max MaxNumberOfThread = 1);

    static ObjectArray<SparseVector<ScalarType>> 
        ComputeReconstructionCodeFromSimilarityCode(const DenseMatrix<ScalarType>&  FeatureData, 
                                                    const ObjectArray<SparseVector<ScalarType>>& SimilarityCodeSet,
                                                    const DenseMatrix<ScalarType>&  BasisMatrix,
                                                    bool CodeNonnegative,
                                                    bool CodeSumToOne,
                                                    int_max MaxNumberOfThread = 1);

    static ObjectArray<SparseVector<ScalarType>> 
        ComputeReconstructionCodeFromSoftAssignCode(const DenseMatrix<ScalarType>&  FeatureData, 
                                                    const ObjectArray<SparseVector<ScalarType>>& SoftAssignCodeSet,
                                                    const DenseMatrix<ScalarType>&  BasisMatrix,
                                                    bool CodeNonnegative,
                                                    bool CodeSumToOne,
                                                    int_max MaxNumberOfThread = 1);

    //----------------------------------------------------------------------------------------------------

    static DenseMatrix<ScalarType> ReconstructData(const SparseVector<ScalarType>& ReconstructionCode,
                                                    const DenseMatrix<ScalarType>&  BasisMatrix);

    static void ReconstructData(DenseMatrix<ScalarType>& ReconstructedDataVector,
                                const SparseVector<ScalarType>& ReconstructionCode,
                                const DenseMatrix<ScalarType>&  BasisMatrix);


    static DenseMatrix<ScalarType> ReconstructData(const ObjectArray<SparseVector<ScalarType>>& ReconstructionCodeSet,
                                                    const DenseMatrix<ScalarType>&  BasisMatrix,
                                                    int_max MaxNumberOfThread = 1);

    static void ReconstructData(DenseMatrix<ScalarType>& ReconstructedDataSet,
                                const ObjectArray<SparseVector<ScalarType>>& ReconstructionCodeSet,
                                const DenseMatrix<ScalarType>&  BasisMatrix,
								int_max MaxNumberOfThread = 1);

    static ScalarType ComputeReconstructionErrorL2Norm(const DenseMatrix<ScalarType>&  DataColVector, 
                                                        const SparseVector<ScalarType>& ReconstructionCode,
                                                        const DenseMatrix<ScalarType>&  BasisMatrix);

    static DenseMatrix<ScalarType> ComputeReconstructionErrorL2Norm(const DenseMatrix<ScalarType>&  FeatureData,
                                                                     const ObjectArray<SparseVector<ScalarType>>& ReconstructionCodeSet,
                                                                     const DenseMatrix<ScalarType>&  BasisMatrix,
																	 int_max MaxNumberOfThread = 1);

    static void ComputeReconstructionErrorL2Norm(DenseMatrix<ScalarType>& ErrorL2NormList,
                                                 const DenseMatrix<ScalarType>&  FeatureData,
                                                 const ObjectArray<SparseVector<ScalarType>>& ReconstructionCodeSet,
                                                 const DenseMatrix<ScalarType>&  BasisMatrix,
												 int_max MaxNumberOfThread = 1);

    //----------------------------------------------------------------------------------------------------

	static ObjectArray<SparseVector<ScalarType>> Apply(const DenseMatrix<ScalarType>* FeatureData,
													  const FeatureDictionary<ScalarType>* Dictionary,
													  const Parameter_Of_KNNReconstructionSparseEncoder<ScalarType>& Parameter,
													  int_max MaxNumberOfThread = 1);

protected:
    bool Preprocess();

    bool Postprocess();

    bool ComputeGramianMatrix_DtD();

	inline SparseVector<ScalarType> EncodeSingleDataVector(int_max DataIndex, const DenseMatrix<ScalarType>& DataColVector, int_max ThreadIndex);

private:
//deleted:
    KNNReconstructionSparseEncoder(const KNNReconstructionSparseEncoder&) = delete;

    void operator=(const KNNReconstructionSparseEncoder&) = delete;

};

}

#include "mdkKNNReconstructionSparseEncoder.hpp"

#endif