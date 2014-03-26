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

    // output code in compact format (the first choice, used in encoding) :

    int64 m_MaxNumberOfNonZeroElementsInEachCode;

    DenseMatrix<ElementType>* m_FeatureCodeInCompactFormat;

    DenseMatrix<ElementType>  m_FeatureCodeInCompactFormat_SharedCopy;

    // output code in dense format (converted from code in compact format, once compact code is computed):

    DenseMatrix<ElementType>* m_FeatureCodeInDenseFormat;

    DenseMatrix<ElementType> m_FeatureCodeInDenseFormat_SharedCopy;

    // output code in sparse matrix (converted from code in compact format, once compact code is computed):
    SparseMatrix<ElementType>* m_FeatureCodeInSparseFormat;

    SparseMatrix<ElementType> m_FeatureCodeInSparseFormat_SharedCopy;

    // Code Example --------------------------------------------------------------------
    //
    // MaxNumberOfNonZeroElementsInEachCode is 2
    //
    // Code in Dense Format (a column is a code vector)
    // 0 1 0 1 0
    // 1 0 1 0 0
    // 1 1 0 0 1
    // 0 0 0 1 0
    // 0 0 1 0 0
    //
    // Code in Compact Format
    // row index  1  0  1  0   2
    // value      1  1  1  1   1
    // row index  2  2  4  3  -1
    // value      1  1  1  1  -1
    //
    // Code in Sparse Format
    // convert Code in Compact Format to a sparse matrix (Matlab sparse matrix format)
    //------------------------------------------------------------------------------------

    //Input Parameter:

    int64 m_MaximunNumberOfThreads; // the maximum number of threads for encoding

private:

    std::atomic<bool> m_Flag_FeatureCodeInDenseFormat_Is_Updated;

    std::atomic<bool> m_Flag_FeatureCodeInSparseFormat_Is_Updated;

public:

    FeatureDictionaryBasedSparseEncoder();

    ~FeatureDictionaryBasedSparseEncoder();

    //-----------------------------------------

    virtual void Clear();

    //-----------------------------------------

    bool SetInputFeatureData(const DenseMatrix<ElementType>* InputFeatureData);

    bool SetInputDictionary(const FeatureDictionary<ElementType>* Dictionary);

    bool SetMaxNumberOfNonZeroElementsInEachCode(int64 Spasity);

    bool SetOutputFeatureCode(DenseMatrix<ElementType>* FeatureCode); // output FeatureCodeInDenseFormat

    bool SetOutputFeatureCodeInCompactFormat(DenseMatrix<ElementType>* FeatureCode);

    bool SetOutputFeatureCodeInSparseFormat(DenseMatrix<ElementType>* FeatureCode);

    bool SetMaximunNumberOfThreads(int64 Number);

    //-----------------------------------------

    virtual bool Update();

    //----------------------------------------------------//

    DenseMatrix<ElementType>* GetOutputFeatureCodeInCompactFormat();

    DenseMatrix<ElementType>* GetOutputFeatureCode(); // output FeatureCodeInDenseFormat

    DenseMatrix<ElementType>* GetOutputFeatureCodeInSparseFormat();


    //---------------------------------------------------//

    static GetFeatureCodeInDenseFormatFromCompactFormat(DenseMatrix<ElementType>& FeatureCodeInDenseFormat, const DenseMatrix<ElementType>& FeatureCodeInCompactFormat);

    static GetFeatureCodeInSparseFormatFromCompactFormat(SparseMatrix<ElementType>& FeatureCodeInSparseFormat, const DenseMatrix<ElementType>& FeatureCodeInCompactFormat);

protected:

    int64 GetMaximunNumberOfThreads();

    int64 GetFeatureVectorNumber();

    virtual bool CheckInputAndOutput();

private:
//deleted:
    FeatureDictionaryBasedSparseEncoder(const FeatureDictionaryBasedSparseEncoder&) = delete;

    void operator=(const FeatureDictionaryBasedSparseEncoder&) = delete;

};

}

#include "mdkFeatureDictionaryBasedSparseEncoder.hpp"

#endif