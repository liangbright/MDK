#ifndef __mdkFeatureDictionaryBasedSparseEncoder_h
#define __mdkFeatureDictionaryBasedSparseEncoder_h

#include <string>
#include <atomic>
#include <thread>


#include "mdkString.h"
#include "mdkDataArray.h"
#include "mdkSparseMatrix.h"
#include "mdkSparseVector.h"
#include "mdkFeatureDictionaryBasedEncoder.h"
#include "mdkFeatureDictionaryForSparseCoding.h"

namespace mdk
{

template<typename ElementType>
class FeatureDictionaryBasedSparseEncoder : public FeatureDictionaryBasedEncoder<ElementType>
{
protected:

    // input data:

    const DenseMatrix<ElementType>* m_FeatureData;

    // input dictionary:

    const FeatureDictionaryForSparseCoding<ElementType>* m_Dictionary;

    // output code in dense Matrix:

    DenseMatrix<ElementType>* m_CodeInDenseMatrix; // converted from m_CodeInSparseColVectorList

    // output code in one sparse matrix:

    SparseMatrix<ElementType>* m_CodeInSparseMatrix; // converted from m_CodeInSparseColVectorList

    // output code as separated sparse vectors:

    DataArray<SparseVector<ElementType>>* m_CodeInSparseColVectorSet; // computed in GenerateCode_in_a_Thread(...)

protected:

    DenseMatrix<ElementType>  m_CodeInDenseMatrix_SharedCopy;

    SparseMatrix<ElementType>  m_CodeInSparseMatrix_SharedCopy;

    DataArray<SparseVector<ElementType>> m_CodeInSparseColVectorSet_SharedCopy;

    bool m_Flag_Output_CodeInDenseMatrix;

    bool m_Flag_Output_CodeInSparseMatrix;

    std::atomic<bool> m_Flag_CodeInDenseMatrix_Is_Updated;

    std::atomic<bool> m_Flag_CodeInSparseMatrix_Is_Updated;

    //about multithreading:

    int_max m_MinNumberOfDataPerThread;

    int_max m_MaxNumberOfThreads;

    DenseMatrix<int_max> m_ThreadStatus;

protected:
    FeatureDictionaryBasedSparseEncoder();
    virtual ~FeatureDictionaryBasedSparseEncoder();

public:

    bool IsDenseEncoder() const { return false; }

    bool IsSparseEncoder() const { return true; }

    //-----------------------------------------

    virtual void Clear();

    //-----------------------------------------

    void SetInputFeatureData(const DenseMatrix<ElementType>* FeatureData);

    void SetInputDictionary(const FeatureDictionaryForSparseCoding<ElementType>* Dictionary);

    void SetOutputCode(DenseMatrix<ElementType>* Code);

    void SetOutputCode(SparseMatrix<ElementType>* Code);

    void SetOutputCode(DataArray<SparseVector<ElementType>>* Code); // CodeInSparseColVectorSet

    void SetMaxNumberOfThreads(int_max Number);

    void SetMinNumberOfDataPerThread(int_max Number);

    //-----------------------------------------

    virtual bool CheckInput();

    virtual bool Update();

    //----------------------------------------------------//

    DenseMatrix<ElementType>* GetOutputCode(); // output CodeInDenseMatrix

    DenseMatrix<ElementType>* GetOutputCodeInDenseMatrix();

    SparseMatrix<ElementType>* GetOutputCodeInSparseMatrix();

    DataArray<SparseVector<ElementType>>* GetOutputCodeInSparseColVectorSet();

protected:

    int_max GetNumberOfThreadsTobeCreated();

    int_max GetMinNumberOfDataPerThread();

    int_max GetTotalNumberOfInputFeatureDataVectors();

    virtual bool Preprocess();

    virtual bool Postprocess();

    virtual void EncodingFunction(int_max DataIndex, int_max ThreadIndex) {}

    virtual void GenerateCode_in_a_Thread(int_max IndexOfDataVector_start, int_max IndexOfDataVector_end, int_max ThreadIndex);

    virtual void ClearPipelineOutput();

    virtual void UpdatePipelineOutput();

    void CheckThreadStatus();

private:
    FeatureDictionaryBasedSparseEncoder(const FeatureDictionaryBasedSparseEncoder&) = delete;

    void operator=(const FeatureDictionaryBasedSparseEncoder&) = delete;

};

}

#include "mdkFeatureDictionaryBasedSparseEncoder.hpp"

#endif