#ifndef mdk_KNNSimilaritySparseEncoder_h
#define mdk_KNNSimilaritySparseEncoder_h

#include "mdkString.h"
#include "mdkFeatureCoding_Common_Type.h"
#include "mdkFeatureDictionaryBasedSparseEncoder.h"

namespace mdk
{

template<typename ScalarType>
struct Parameter_Of_KNNSimilaritySparseEncoder
{
    int_max NeighbourCount;

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
        NeighbourCount = 0;
        SimilarityType = VectorSimilarityTypeEnum::Unknown;
    }

	static ScalarType GetExpFactor()
	{   // see EncodeSingleDataVector
		// exp(-ExpFactor * x^2)
		// 0.1 is a good number
		return ScalarType(0.1);
	}
};


template<typename Scalar_Type>
class KNNSimilaritySparseEncoder : public FeatureDictionaryBasedSparseEncoder<Scalar_Type>
{
public:
	typedef Scalar_Type ScalarType;

public:
    Parameter_Of_KNNSimilaritySparseEncoder<ScalarType> m_Parameter;

public:
    KNNSimilaritySparseEncoder();
    ~KNNSimilaritySparseEncoder();

    //-----------------------------------------

	void Clear();

    bool CheckInput();

	inline SparseVector<ScalarType> EncodeSingleDataVector(const DenseMatrix<ScalarType>& DataColVector);

    //--------------------------------------------------------------------------------------------------

    static inline bool CheckIfSimilarityTypeSupported(VectorSimilarityTypeEnum SimilarityType);

    //--------------------------------------------------------------------------------------------------
       
    static DenseMatrix<ScalarType> ComputeKNNCode(const DenseMatrix<ScalarType>& DataColVector,
                                                  const DenseMatrix<ScalarType>& KNNBasisMatrix,
                                                  const VectorSimilarityTypeEnum  SimilarityType,
                                                  const DenseMatrix<ScalarType>& VarianceList);

    //--------------------------------------------------------------------------------------------------

    static ScalarType ComputeSimilarityBetweenTwoVector(const DenseMatrix<ScalarType>& VectorA, 
                                                        const DenseMatrix<ScalarType>& VectorB,
                                                        VectorSimilarityTypeEnum SimilarityType,
                                                        ScalarType Variance);

    static ScalarType ComputeSimilarityBetweenTwoVector(const ScalarType* VectorA, const ScalarType* VectorB, int_max Length, 
                                                        VectorSimilarityTypeEnum SimilarityType, ScalarType Variance, bool Flag_CheckInput = true);

    //----------------------------------------------------------------------------------------------------
private:
	inline SparseVector<ScalarType> EncodeSingleDataVector(int_max DataIndex, const DenseMatrix<ScalarType>& DataColVector, int_max ThreadIndex);
	void ClearSelf();

private:
//deleted:
    KNNSimilaritySparseEncoder(const KNNSimilaritySparseEncoder&) = delete;

    void operator=(const KNNSimilaritySparseEncoder&) = delete;

};

}

#include "mdkKNNSimilaritySparseEncoder.hpp"

#endif