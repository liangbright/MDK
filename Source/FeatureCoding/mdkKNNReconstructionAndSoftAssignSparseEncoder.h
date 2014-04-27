#ifndef __mdkKNNReconstructionAndSoftAssignSparseEncoder_h
#define __mdkKNNReconstructionAndSoftAssignSparseEncoder_h

#include  <algorithm>

#include "mdkKNNReconstructionSparseEncoder.h"


namespace mdk
{

struct Parameter_Of_KNNReconstructionAndSoftAssignSparseEncoder
{
    int_max NeighbourNumber;

    // parameter only for Reconstruction ----------------------------

    bool Nonnegative;

    bool SumToOne;
    
    std::string DistanceTypeForKNNSearch;
    // L1Distance
    // L2Distance  : default
    // Correlation
    // KLDivergence

    // parameter only for Soft Assign ----------------------------

    std::string DistanceTypeForSoftAssign;
    // L1Distance
    // L2Distance
    // Correlation  : default
    // KLDivergence

    Parameter_Of_KNNReconstructionAndSoftAssignSparseEncoder() { this->Clear(); }
    ~Parameter_Of_KNNReconstructionAndSoftAssignSparseEncoder() {}

    void Clear()
    {
        NeighbourNumber  = -1;
        DistanceTypeForKNNSearch = "L2Distance";
        Nonnegative      = false;
        SumToOne         = false;

        DistanceTypeForSoftAssign = "Correlation";
    }

private:
    Parameter_Of_KNNReconstructionAndSoftAssignSparseEncoder(const Parameter_Of_KNNReconstructionAndSoftAssignSparseEncoder&) = delete;
    void operator=(const Parameter_Of_KNNReconstructionAndSoftAssignSparseEncoder&) = delete;
};


template<typename ElementType>
class KNNReconstructionAndSoftAssignSparseEncoder : public FeatureDictionaryBasedSparseEncoder<ElementType>
{
public:

    Parameter_Of_KNNReconstructionAndSoftAssignSparseEncoder m_Parameter;

private:

    KNNReconstructionSparseEncoder<ElementType> m_ReconstructionEncoder;

    // output reconstruction error
    DenseMatrix<ElementType>* m_ReconstructionErrorNorm;

    DenseMatrix<ElementType> m_ReconstructionErrorNorm_SharedCopy;

public:

    KNNReconstructionAndSoftAssignSparseEncoder();

    ~KNNReconstructionAndSoftAssignSparseEncoder();

    //--------------------------------------------------------------------------------

    void Clear();

    void SetInputFeatureData(const DenseMatrix<ElementType>* FeatureData);

    void SetInputDictionary(const FeatureDictionaryForSparseCoding<ElementType>* Dictionary);

    void SetOutputReconstructionErrorNorm(DenseMatrix<ElementType>* ErrorNorm);

    DenseMatrix<ElementType>* GetOutputReconstructionErrorNorm();

    bool CheckInput();

    //---------------------------------------------------------------------------------

    void PreprocessBeforeUsing_EncodeSingleDataVector();

    void PostprocessAfterUsing_EncodeSingleDataVector();

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
    KNNReconstructionAndSoftAssignSparseEncoder(const KNNReconstructionAndSoftAssignSparseEncoder&) = delete;

    void operator=(const KNNReconstructionAndSoftAssignSparseEncoder&) = delete;

};

}

#include "mdkKNNReconstructionAndSoftAssignSparseEncoder.hpp"

#endif