#ifndef __mdkFeatureDictionaryBasedSparseEncoder_h
#define __mdkFeatureDictionaryBasedSparseEncoder_h

#include <string>
#include <atomic>
#include <thread>


#include "mdkSparseMatrix.h"
#include "mdkFeatureDictionaryBasedEncoder.h"

namespace mdk
{

template<typename ElementType>
class FeatureDictionaryBasedSparseEncoder : public FeatureDictionaryBasedEncoder<ElementType>
{
protected:

    // input data:

    const DenseMatrix<ElementType>* m_FeatureData;

    // input dictionary:

    const FeatureDictionary<ElementType>* m_Dictionary;

    // output code in dense Matrix:

    DenseMatrix<ElementType>* m_CodeInDenseMatrix; // converted from m_CodeInSparseVectorList

    // output code in one sparse matrix:

    SparseMatrix<ElementType>* m_CodeInSparseMatrix; // converted from m_CodeInSparseVectorList

    // output code as separated sparse vectors:

    DenseMatrix<SparseMatrix<ElementType>>* m_CodeInSparseVectorList;

    //Input Parameter:

    int_max m_MaxNumberOfThreads; // the maximum number of threads for encoding

private:

    DenseMatrix<ElementType>  m_CodeInDenseMatrix_SharedCopy;

    SparseMatrix<ElementType>  m_CodeInSparseMatrix_SharedCopy;

    DenseMatrix<SparseMatrix<ElementType>> m_CodeInSparseVectorList_SharedCopy;

    bool m_Flag_Output_CodeInDenseMatrix;

    bool m_Flag_Output_CodeInSparseMatrix;

    std::atomic<bool> m_Flag_CodeInDenseMatrix_Is_Updated;

    std::atomic<bool> m_Flag_CodeInSparseMatrix_Is_Updated;

protected:
    FeatureDictionaryBasedSparseEncoder();

public:
    virtual ~FeatureDictionaryBasedSparseEncoder();

    //-----------------------------------------

    virtual void Clear();

    //-----------------------------------------

    bool SetInputFeatureData(const DenseMatrix<ElementType>* FeatureData);

    bool SetInputDictionary(const FeatureDictionary<ElementType>* Dictionary);

    bool SetOutputCodeInDenseMatrix(DenseMatrix<ElementType>* Code);

    bool SetOutputCode(DenseMatrix<ElementType>* Code); // output CodeInDenseMatrix

    bool SetOutputCodeInSparseMatrix(SparseMatrix<ElementType>* Code);

    bool SetOutputCodeInSparseVectorList(DenseMatrix<SparseMatrix<ElementType>>* Code);

    bool SetMaxNumberOfThreads(int_max Number);

    //-----------------------------------------

    virtual bool CheckInputAndOutput();

    //-----------------------------------------

    virtual bool Update();

    inline virtual void EncodingFunction(const DenseMatrix<ElementType>& SingleFeatureDataVector,
                                         DenseMatrix<ElementType>& CodeInDenseVector);

    inline virtual void EncodingFunction(const DenseMatrix<ElementType>& SingleFeatureDataVector,
                                         SparseMatrix<ElementType>& CodeInSparseVector) = 0;

    //----------------------------------------------------//

    DenseMatrix<ElementType>* GetOutputCodeInDenseMatrix();

    DenseMatrix<ElementType>* GetOutputCode(); // output CodeInDenseMatrix

    SparseMatrix<ElementType>* GetOutputCodeInSparseMatrix();

    DenseMatrix<SparseMatrix<ElementType>>* GetOutputCodeInSparseVectorList();

    //---------------------------------------------------//

protected:

    int_max GetMaxNumberOfThreads();

    int_max GetTotalNumberOfInputFeatureVectors();

    virtual void GenerateCode_in_a_Thread(int_max IndexOfFeatureVector_start, int_max IndexOfFeatureVector_end);

private:
//deleted:
    FeatureDictionaryBasedSparseEncoder(const FeatureDictionaryBasedSparseEncoder&) = delete;

    void operator=(const FeatureDictionaryBasedSparseEncoder&) = delete;

};

}

#include "mdkFeatureDictionaryBasedSparseEncoder.hpp"

#endif