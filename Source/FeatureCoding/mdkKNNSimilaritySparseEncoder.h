#ifndef __mdkKNNSimilaritySparseEncoder_h
#define __mdkKNNSimilaritySparseEncoder_h

#include "mdkString.h"
#include "mdkFeatureCoding_Common_Type.h"
#include "mdkFeatureDictionaryBasedSparseEncoder.h"

namespace mdk
{

template<typename ElementType>
struct Parameter_Of_KNNSimilaritySparseEncoder
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

//-------------------------------------------------------------------------------
    Parameter_Of_KNNSimilaritySparseEncoder() { this->Clear(); }
    ~Parameter_Of_KNNSimilaritySparseEncoder() {}

    void Clear()
    {
        NeighbourNumber = 0;

        SimilarityType = VectorSimilarityTypeEnum::Unknown;
    }
};


template<typename Element_Type>
class KNNSimilaritySparseEncoder : public FeatureDictionaryBasedSparseEncoder<Element_Type>
{
public:
	typedef Element_Type ElementType;

public:
    Parameter_Of_KNNSimilaritySparseEncoder<ElementType> m_Parameter;

public:

    KNNSimilaritySparseEncoder();

    ~KNNSimilaritySparseEncoder();

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
    KNNSimilaritySparseEncoder(const KNNSimilaritySparseEncoder&) = delete;

    void operator=(const KNNSimilaritySparseEncoder&) = delete;

};

}

#include "mdkKNNSimilaritySparseEncoder.hpp"

#endif