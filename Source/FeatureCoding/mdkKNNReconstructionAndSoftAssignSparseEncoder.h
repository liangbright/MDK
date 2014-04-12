#ifndef __mdkKNNReconstructionAndSoftAssignSparseEncoder_h
#define __mdkKNNReconstructionAndSoftAssignSparseEncoder_h


#include "mdkKNNReconstructionSparseEncoder.h"


namespace mdk
{

struct Parameter_Of_KNNReconstructionAndSoftAssignSparseEncoder
{
    int_max NeighbourNumber;

    // parameter for KNN Reconstruction ----------------------------

    bool Nonnegative;

    bool SumToOne;
    
    std::string DistanceTypeForKNNSearch;
    // L1Distance
    // L2Distance
    // Correlation

    // parameter for KNN SoftAssign ----------------------------

    std::string DistanceTypeForSoftAssign;
    // L1Distance
    // L2Distance
    // Correlation

    Parameter_Of_KNNReconstructionAndSoftAssignSparseEncoder() { this->Clear(); }
    ~Parameter_Of_KNNReconstructionAndSoftAssignSparseEncoder() {}

    void Clear()
    {
        NeighbourNumber  = -1;
        DistanceTypeForKNNSearch.clear();
        Nonnegative      = false;
        SumToOne         = false;

        DistanceTypeForSoftAssign.clear();
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

public:

    KNNReconstructionAndSoftAssignSparseEncoder();

    ~KNNReconstructionAndSoftAssignSparseEncoder();

    //--------------------------------------------------------------------------------

    void Clear();

    void SetInputFeatureData(const DenseMatrix<ElementType>* FeatureData);

    void SetInputDictionary(const FeatureDictionaryForSparseCoding<ElementType>* Dictionary);

    bool CheckInput();

    //--------------------------------------------------------------------------------

    using FeatureDictionaryBasedSparseEncoder::EncodingFunction;

    inline void EncodingFunction(const DenseMatrix<ElementType>& DataColVector, SparseMatrix<ElementType>& CodeInSparseColVector);

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

    void bool Preprocess();

    void bool UpdateParameterForKNNReconstruction();

private:
//deleted:
    KNNReconstructionAndSoftAssignSparseEncoder(const KNNReconstructionAndSoftAssignSparseEncoder&) = delete;

    void operator=(const KNNReconstructionAndSoftAssignSparseEncoder&) = delete;

};

}

#include "mdkKNNReconstructionAndSoftAssignSparseEncoder.hpp"

#endif