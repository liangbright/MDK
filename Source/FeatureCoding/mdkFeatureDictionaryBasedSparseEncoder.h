#ifndef mdk_FeatureDictionaryBasedSparseEncoder_h
#define mdk_FeatureDictionaryBasedSparseEncoder_h

#include "mdkParallelForLoop.h"
#include "mdkObjectArray.h"
#include "mdkSparseMatrix.h"
#include "mdkSparseVector.h"
#include "mdkFeatureDictionary.h"
#include "mdkFeatureDictionaryBasedEncoder.h"
#include "mdkFeatureDictionaryForSparseCoding.h"

namespace mdk
{
template<typename Scalar_Type>
class FeatureDictionaryBasedSparseEncoder : public FeatureDictionaryBasedEncoder<FeatureDictionaryForSparseCoding<Scalar_Type>, ObjectArray<SparseVector<Scalar_Type>>>
{
public:
	typedef Scalar_Type ScalarType;
	typedef FeatureDictionaryForSparseCoding<ScalarType> DictionaryType;

protected:
    // input data
	const DenseMatrix<ScalarType>* m_FeatureData;

    // input dictionary
	const DictionaryType* m_Dictionary;

	// output sparse code
	ObjectArray<SparseVector<ScalarType>> m_SparseCode;

	//-------------- about thread ------------//
    int_max m_MinCountOfDataPerThread;
    int_max m_MaxThreadCount;
    DenseMatrix<int_max> m_ThreadStatus;

protected:
    FeatureDictionaryBasedSparseEncoder();
    virtual ~FeatureDictionaryBasedSparseEncoder();

public:
    bool IsDenseEncoder() const { return false; }
    bool IsSparseEncoder() const { return true; }

    virtual void Clear();

	void SetInputFeatureData(const DenseMatrix<ScalarType>* FeatureData);

	void SetInputDictionary(const DictionaryType* Dictionary);

    void SetMaxThreadCount(int_max Count);

    void SetMinCountOfDataPerThread(int_max Count);

    virtual bool CheckInput();

    virtual bool Update();

	inline virtual SparseVector<ScalarType> EncodeSingleDataVector(const DenseMatrix<ScalarType>& DataColVector) = 0;

	ObjectArray<SparseVector<ScalarType>>* GetOutputCode();

	DenseMatrix<ScalarType> ConvertOutputCodeToDenseMatrix();

	SparseMatrix<ScalarType> ConvertOutputCodeToSparseMatrix();

protected:
    virtual bool Preprocess();
    virtual bool Postprocess();

    virtual void GenerateCode_in_a_thread(int_max IndexOfDataVector_start, int_max IndexOfDataVector_end, int_max ThreadIndex);

	inline virtual SparseVector<ScalarType> EncodeSingleDataVector(int_max DataIndex, const DenseMatrix<ScalarType>& DataColVector, int_max ThreadIndex) = 0;

    void CheckThreadStatus();

	int_max GetCountOfThreadTobeCreated();
	int_max GetMinCountOfDataPerThread();

	int_max GetTotalCountOfInputFeatureDataVector();

private:
    FeatureDictionaryBasedSparseEncoder(const FeatureDictionaryBasedSparseEncoder&) = delete;
    void operator=(const FeatureDictionaryBasedSparseEncoder&) = delete;
};

}

#include "mdkFeatureDictionaryBasedSparseEncoder.hpp"

#endif