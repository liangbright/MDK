#ifndef __mdkKNNSoftAssignSparseEncoder_h
#define __mdkKNNSoftAssignSparseEncoder_h

#include "mdkString.h"
#include "mdkFeatureCoding_Common_Type.h"
#include "mdkFeatureDictionaryBasedSparseEncoder.h"
#include "mdkKNNSimilaritySparseEncoder.h"

namespace mdk
{

template<typename ElementType>
struct Parameter_Of_KNNSoftAssignSparseEncoder
{
    int_max NeighbourNumber;

    VectorSimilarityTypeEnum SimilarityType;
    //
    // If a Feature vector is treated as ordinary signal
    // L1Distance
    // L2Distance
    // many kinds of Correlation
    //
    // If a Feature vector is normalized histogram or probability mass function (PMF), i.e., discrete probability distribution
    // KLDivergence

    ElementType SimilarityThreshold; // set Similarity to 0 if it is < SimilarityThreshold

//-------------------------------------------------------------------------------
    Parameter_Of_KNNSoftAssignSparseEncoder() { this->Clear(); }
    ~Parameter_Of_KNNSoftAssignSparseEncoder() {}

    void Clear()
    {
        NeighbourNumber = 0;

        SimilarityType = VectorSimilarityTypeEnum::Unknown;

        SimilarityThreshold = 0;
    }
};


template<typename ElementType>
class KNNSoftAssignSparseEncoder : public FeatureDictionaryBasedSparseEncoder<ElementType>
{

public:

    Parameter_Of_KNNSoftAssignSparseEncoder<ElementType> m_Parameter;

private:
    KNNSimilaritySparseEncoder<ElementType> m_KNNSimilaritySparseEncoder;

public:

    KNNSoftAssignSparseEncoder();

    ~KNNSoftAssignSparseEncoder();

    //-----------------------------------------

    void Clear();

    bool CheckInput();

    inline void EncodeSingleDataVector(SparseVector<ElementType>& CodeInSparseColVector,
                                       const DenseMatrix<ElementType>& DataColVector);

    //--------------------------------------------------------------------------------------------------

    static inline bool CheckIfSimilarityTypeSupported(VectorSimilarityTypeEnum SimilarityType);

    //--------------------------------------------------------------------------------------------------

    static DenseMatrix<ElementType> ComputeKNNCode(const DenseMatrix<ElementType>& DataColVector,
                                                   const DenseMatrix<ElementType>& KNNBasisMatrix,
                                                   const VectorSimilarityTypeEnum  SimilarityType,
                                                   const ElementType SimilarityThreshold,
                                                   const DenseMatrix<ElementType>& VarianceList);

    //--------------------------------------------------------------------------------------------------
        
    static ElementType ComputeSimilarityBetweenTwoVectors(const DenseMatrix<ElementType>& VectorA, 
                                                          const DenseMatrix<ElementType>& VectorB,
                                                          VectorSimilarityTypeEnum SimilarityType, 
                                                          ElementType Variance);

    static ElementType ComputeSimilarityBetweenTwoVectors(const ElementType* VectorA, const ElementType* VectorB, int_max Length, 
                                                          VectorSimilarityTypeEnum SimilarityType, ElementType Variance, bool CheckInput = true);

    //----------------------------------------------------------------------------------------------------

protected:

    inline void EncodingFunction(int_max DataIndex, int_max ThreadIndex);

private:
//deleted:
    KNNSoftAssignSparseEncoder(const KNNSoftAssignSparseEncoder&) = delete;

    void operator=(const KNNSoftAssignSparseEncoder&) = delete;

};

}

#include "mdkKNNSoftAssignSparseEncoder.hpp"

#endif