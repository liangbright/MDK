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

    // this is sparseness

    int_max m_MaxNumberOfNonzeroElementsInEachCode;

    // output code in compact format (the first choice, used in encoding) :

    DenseMatrix<ElementType>* m_FeatureCodeInCompactFormat;

    // output code in dense format (converted from code in compact format, once compact code is computed):

    DenseMatrix<ElementType>* m_FeatureCodeInDenseFormat;

    // output code in sparse matrix (converted from code in compact format, once compact code is computed):

    SparseMatrix<ElementType>* m_FeatureCodeInSparseFormat;

    // Code Example --------------------------------------------------------------------
    //
    // m_MaxNumberOfNonzeroElementsInEachCode is 2
    //
    // Code in Dense Format (a column is a code vector)
    // 0    0.1  0    0.2  0
    // 0.3  0.4  0    0.5  0 
    // 0.6  0    0    0    0.7
    // 0    0    0.8  0    0
    // 0    0    0.9  0    0
    //
    // Code in Compact Format
    // row index  1    0     3    0     2
    // value      0.3  0.1   0.8  0.2   0.7
    // row index  2    2     4    1    -1     (-1 means nothing is here and below)
    // value      0.6  0.4   0.9  0.5   ?     (? means this value does not matter)
    //
    // Code in Sparse Format
    // convert Code in Compact Format to a sparse matrix (Matlab sparse matrix format)
    //------------------------------------------------------------------------------------

    //Input Parameter:

    int_max m_MaxNumberOfThreads; // the maximum number of threads for encoding

private:

    DenseMatrix<ElementType>  m_FeatureCodeInCompactFormat_SharedCopy;

    DenseMatrix<ElementType>  m_FeatureCodeInDenseFormat_SharedCopy;

    SparseMatrix<ElementType> m_FeatureCodeInSparseFormat_SharedCopy;

    bool m_Flag_Output_FeatureCodeInDenseFormat;

    bool m_Flag_Output_FeatureCodeInSparseFormat;

    std::atomic<bool> m_Flag_FeatureCodeInDenseFormat_Is_Updated;

    std::atomic<bool> m_Flag_FeatureCodeInSparseFormat_Is_Updated;

protected:
    FeatureDictionaryBasedSparseEncoder();

public:
    virtual ~FeatureDictionaryBasedSparseEncoder();

    //-----------------------------------------

    virtual void Clear();

    //-----------------------------------------

    bool SetInputFeatureData(const DenseMatrix<ElementType>* InputFeatureData);

    bool SetInputDictionary(const FeatureDictionary<ElementType>* Dictionary);

    bool SetMaxNumberOfNonZeroElementsInEachCode(int_max Spasity);

    bool SetOutputFeatureCodeInCompactFormat(DenseMatrix<ElementType>* FeatureCode);

    bool SetOutputFeatureCodeInDenseFormat(DenseMatrix<ElementType>* FeatureCode);

    bool SetOutputFeatureCode(DenseMatrix<ElementType>* FeatureCode); // output FeatureCodeInDenseFormat

    bool SetOutputFeatureCodeInSparseFormat(SparseMatrix<ElementType>* FeatureCode);

    bool SetMaxNumberOfThreads(int_max Number);

    //-----------------------------------------

    virtual bool CheckInputAndOutput();

    //-----------------------------------------

    virtual bool Update();

    //----------------------------------------------------//

    DenseMatrix<ElementType>* GetOutputFeatureCodeInCompactFormat();

    DenseMatrix<ElementType>* GetOutputFeatureCodeInDenseFormat();

    DenseMatrix<ElementType>* GetOutputFeatureCode(); // output FeatureCodeInDenseFormat

    SparseMatrix<ElementType>* GetOutputFeatureCodeInSparseFormat();


    //---------------------------------------------------//

    static bool GetFeatureCodeInDenseFormatFromCompactFormat(DenseMatrix<ElementType>& FeatureCodeInDenseFormat, 
                                                             const DenseMatrix<ElementType>& FeatureCodeInCompactFormat,
                                                             int_max FeatureCodeDimension);

    static bool GetFeatureCodeInSparseFormatFromCompactFormat(SparseMatrix<ElementType>& FeatureCodeInSparseFormat, 
                                                              const DenseMatrix<ElementType>& FeatureCodeInCompactFormat,
                                                              int_max FeatureCodeDimension);

    //---------------------------------------------------//

    

protected:

    int_max GetMaxNumberOfThreads();

    int_max GetTotalNumberOfInputFeatureVectors();

private:
//deleted:
    FeatureDictionaryBasedSparseEncoder(const FeatureDictionaryBasedSparseEncoder&) = delete;

    void operator=(const FeatureDictionaryBasedSparseEncoder&) = delete;

};

}

#include "mdkFeatureDictionaryBasedSparseEncoder.hpp"

#endif