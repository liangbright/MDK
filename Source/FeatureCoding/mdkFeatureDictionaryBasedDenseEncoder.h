#ifndef __mdkFeatureDictionaryBasedDenseEncoder_h
#define __mdkFeatureDictionaryBasedDenseEncoder_h

#include <string>

#include "mdkFeatureDictionaryBasedEncoder.h"
#include "mdkFeatureDictionaryForDenseCoding.h"

namespace mdk
{

template<typename Scalar_Type>
class FeatureDictionaryBasedDenseEncoder : public FeatureDictionaryBasedEncoder<FeatureDictionaryForDenseCoding<Scalar_Type>, DenseMatrix<Scalar_Type>>
{
public:
	typedef Scalar_Type ScalarType;

protected:
    // input data
	const DenseMatrix<ScalarType>* m_FeatureData;
	// input dictionary
	const FeatureDictionaryForDenseCoding<ScalarType>* m_Dictionary;
    // output dense code
    DenseMatrix<ElementType> m_DenseCode;

private:
    //about thread:
    int_max m_MinNumberOfDataPerThread;
    int_max m_MaxNumberOfThread;

protected:
    FeatureDictionaryBasedDenseEncoder();
    virtual ~FeatureDictionaryBasedDenseEncoder();

public:
    bool IsDenseEncoder() const { return true; }

    bool IsSparseEncoder() const { return false; }

    virtual void Clear();
    
    void SetInputFeatureData(const DenseMatrix<ElementType>* InputFeatureData);

    void SetInputDictionary(const FeatureDictionaryForDenseCoding<ElementType>* Dictionary);

    void SetMaxNumberOfThread(int_max Number);

    void SetMinNumberOfDataPerThread(int_max Number);

    virtual bool CheckInput();

    virtual bool Update();

    DenseMatrix<ElementType>* GetOutputCode();

protected:
    int_max GetNumberOfThreadTobeCreated();

    int_max GetMinNumberOfDataPerThread();

    int_max GetTotalNumberOfInputFeatureDataVector();

	virtual bool Preprocess();

	virtual bool Postprocess();

    virtual void GenerateCode_in_a_Thread(int_max IndexOfFeatureVector_start, int_max IndexOfFeatureVector_end);
	virtual DenseMatrix<ScalarType> EncodeSingleFeatureVector(int_max DataIndex, const DenseMatrix<ScalarType>& FeatureVector, int_max ThreadIndex) {}

private:
//deleted:
    FeatureDictionaryBasedDenseEncoder(const FeatureDictionaryBasedDenseEncoder&) = delete;
    void operator=(const FeatureDictionaryBasedDenseEncoder&) = delete;
};

}

#include "mdkFeatureDictionaryBasedDenseEncoder.hpp"

#endif