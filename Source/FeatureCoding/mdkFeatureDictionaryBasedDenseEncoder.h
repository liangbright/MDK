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
	typedef FeatureDictionaryForDenseCoding<Scalar_Type>  DictionaryType;

protected:
    // input data
	const DenseMatrix<ScalarType>* m_FeatureData;
	// input dictionary
	const DictionaryType* m_Dictionary;
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

	void SetInputDictionary(const DictionaryType* Dictionary);

    void SetMaxNumberOfThread(int_max Number);

    void SetMinNumberOfDataPerThread(int_max Number);

    virtual bool CheckInput();

    virtual bool Update();

	inline virtual DenseMatrix<ScalarType> EncodeSingleDataVector(const DenseMatrix<ScalarType>& DataColVector) = 0;

    DenseMatrix<ElementType>* GetOutputCode();

protected:
    int_max GetNumberOfThreadTobeCreated();

    int_max GetMinNumberOfDataPerThread();

    int_max GetTotalNumberOfInputFeatureDataVector();

	virtual bool Preprocess();

	virtual bool Postprocess();

	virtual void GenerateCode_in_a_Thread(int_max IndexOfDataVector_start, int_max IndexOfDataVector_end);

	inline virtual DenseMatrix<ScalarType> EncodeSingleDataVector(int_max DataIndex, const DenseMatrix<ScalarType>& DataColVector, int_max ThreadIndex) = 0;

private:
//deleted:
    FeatureDictionaryBasedDenseEncoder(const FeatureDictionaryBasedDenseEncoder&) = delete;
    void operator=(const FeatureDictionaryBasedDenseEncoder&) = delete;
};

}

#include "mdkFeatureDictionaryBasedDenseEncoder.hpp"

#endif