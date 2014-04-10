#ifndef __mdkFeatureDictionaryBasedSparseEncoder_h
#define __mdkFeatureDictionaryBasedSparseEncoder_h

#include <string>
#include <atomic>
#include <thread>


#include "mdkSparseMatrix.h"
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

    DenseMatrix<SparseMatrix<ElementType>>* m_CodeInSparseColVectorList;

    //Input Parameter:

    int_max m_MaxNumberOfThreads; // the maximum number of threads for encoding

private:

    DenseMatrix<ElementType>  m_CodeInDenseMatrix_SharedCopy;

    SparseMatrix<ElementType>  m_CodeInSparseMatrix_SharedCopy;

    DenseMatrix<SparseMatrix<ElementType>> m_CodeInSparseColVectorList_SharedCopy;

    bool m_Flag_Output_CodeInDenseMatrix;

    bool m_Flag_Output_CodeInSparseMatrix;

    std::atomic<bool> m_Flag_CodeInDenseMatrix_Is_Updated;

    std::atomic<bool> m_Flag_CodeInSparseMatrix_Is_Updated;

protected:
    FeatureDictionaryBasedSparseEncoder();
    virtual ~FeatureDictionaryBasedSparseEncoder();

public:
    //-----------------------------------------

    virtual void Clear();

    //-----------------------------------------

    void SetInputFeatureData(const DenseMatrix<ElementType>* FeatureData);

    void SetInputDictionary(const FeatureDictionaryForSparseCoding<ElementType>* Dictionary);

    void SetOutputCode(DenseMatrix<ElementType>* Code); // output CodeInDenseMatrix

    void SetOutputCodeInDenseMatrix(DenseMatrix<ElementType>* Code);

    void SetOutputCodeInSparseMatrix(SparseMatrix<ElementType>* Code);

    void SetOutputCodeInSparseColVectorList(DenseMatrix<SparseMatrix<ElementType>>* Code);

    void SetMaxNumberOfThreads(int_max Number);

    //-----------------------------------------

    virtual bool CheckInput();

    //-----------------------------------------

    inline virtual void EncodingFunction(const DenseMatrix<ElementType>& DataColVector,
                                         DenseMatrix<ElementType>& CodeInDenseColVector);

    inline virtual void EncodingFunction(const DenseMatrix<ElementType>& DataColVector,
                                         SparseMatrix<ElementType>& CodeInSparseColVector) = 0;

    //----------------------------------------------------//

    DenseMatrix<ElementType>* GetOutputCode(); // output CodeInDenseMatrix

    DenseMatrix<ElementType>* GetOutputCodeInDenseMatrix();

    SparseMatrix<ElementType>* GetOutputCodeInSparseMatrix();

    DenseMatrix<SparseMatrix<ElementType>>* GetOutputCodeInSparseColVectorList();

    //---------------------------------------------------//

protected:

    int_max GetMaxNumberOfThreads();

    int_max GetTotalNumberOfInputFeatureVectors();

    virtual void GenerateCode_in_a_Thread(int_max IndexOfFeatureVector_start, int_max IndexOfFeatureVector_end);

    virtual void SetupDefaultPipelineOutput();

    virtual void UpdatePipelineOutput();

private:
    FeatureDictionaryBasedSparseEncoder(const FeatureDictionaryBasedSparseEncoder&) = delete;

    void operator=(const FeatureDictionaryBasedSparseEncoder&) = delete;

};

}

#include "mdkFeatureDictionaryBasedSparseEncoder.hpp"

#endif