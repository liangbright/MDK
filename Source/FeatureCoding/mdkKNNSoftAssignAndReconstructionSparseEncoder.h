#ifndef __mdkKNNSoftAssignAndReconstructionSparseEncoder_h
#define __mdkKNNSoftAssignAndReconstructionSparseEncoder_h

#include  <algorithm>

#include "mdkKNNReconstructionSparseEncoder.h"


namespace mdk
{

struct Parameter_Of_KNNSoftAssignAndReconstructionSparseEncoder
{
    int_max NeighbourNumber;

    std::string SimilarityTypeForKNNSearch;
    // L1Distance
    // L2Distance
    // Correlation
    // KLDivergence

    bool ConvertDistanceToSimilarityForKNNSearch;
    // if it is false, then L1Distance/L2Distance/KLDivergence is directly used for KNN search
    // if it is true,  then ...... is converted to similarity for KNN search 

    std::string SimilarityTypeForEncoding;
    // L1Distance
    // L2Distance
    // Correlation
    // KLDivergence

    // parameter for SoftAssign ----------------------------

    ElementType SimilarityThreshold; // find KNN with Similarity >= SimilarityThreshold
                                     // K in KNN can be < NeighbourNumber

    // Sigma_L1, Sigma_L2, and Sigma_KL may be carried by Dictionary 

    ElementType Sigma_L1; // standard deviation to convert L1Distance to Similarity

    ElementType Sigma_L2; // standard deviation to convert L2Distance to Similarity

    ElementType Sigma_KL; // standard deviation to convert KLDivergence to Similarity

    bool IgnoreSign_Correlation; // if it is true, Similarity = abs(Correlation)
                                 // else, Similarity = (Correlation +1)/2

    // parameter for Reconstruction ----------------------------

    bool CodePositive;

    bool CodeSumToOne;

    Parameter_Of_KNNSoftAssignAndReconstructionSparseEncoder() { this->Clear(); }
    ~Parameter_Of_KNNSoftAssignAndReconstructionSparseEncoder() {}

    void Clear()
    {
        NeighbourNumber  = 0;

        SimilarityTypeForKNNSearch.clear();
        ConvertDistanceToSimilarityForKNNSearch = true;

        SimilarityTypeForEncoding.clear();

        Sigma_L1 = 0;
        Sigma_L2 = 0;
        Sigma_KL = 0;
        IgnoreSign_Correlation = false;

        CodePositive = false;
        CodeSumToOne = false;
    }

private:
    Parameter_Of_KNNSoftAssignAndReconstructionSparseEncoder(const Parameter_Of_KNNSoftAssignAndReconstructionSparseEncoder&) = delete;
    void operator=(const Parameter_Of_KNNSoftAssignAndReconstructionSparseEncoder&) = delete;
};


template<typename ElementType>
class KNNSoftAssignAndReconstructionSparseEncoder : public FeatureDictionaryBasedSparseEncoder<ElementType>
{
public:

    Parameter_Of_KNNSoftAssignAndReconstructionSparseEncoder m_Parameter;

public:

    KNNSoftAssignAndReconstructionSparseEncoder();

    ~KNNSoftAssignAndReconstructionSparseEncoder();

    //--------------------------------------------------------------------------------

    void Clear();

    bool CheckInput();

    //---------------------------------------------------------------------------------

    void PreprocessBeforeUsing_EncodeSingleDataVector();

    void PostprocessAfterUsing_EncodeSingleDataVector();

    using FeatureDictionaryBasedSparseEncoder::EncodeSingleDataVector;

    inline void EncodeSingleDataVector(SparseVector<ElementType>& CodeInSparseColVector,
                                       const DenseMatrix<ElementType>& DataColVector);

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

    static bool Apply(DenseMatrix<SparseVector<ElementType>>& OutputCodeInSparseColVectorSet,
                      const DenseMatrix<ElementType>* FeatureData,
                      const FeatureDictionary<ElementType>* Dictionary,
                      int_max NeighbourNumber,
                      int_max MaxNumberOfThreads = 1);

private:

    virtual void SetupDefaultPipelineOutput();

    virtual void UpdatePipelineOutput();

    virtual bool Preprocess();

    virtual bool Postprocess();

    bool UpdateInputOfReconstructionEncoder();

    inline void EncodingFunction(int_max DataIndex, int_max ThreadIndex);

private:
//deleted:
    KNNSoftAssignAndReconstructionSparseEncoder(const KNNSoftAssignAndReconstructionSparseEncoder&) = delete;

    void operator=(const KNNSoftAssignAndReconstructionSparseEncoder&) = delete;

};

}

#include "mdkKNNSoftAssignAndReconstructionSparseEncoder.hpp"

#endif