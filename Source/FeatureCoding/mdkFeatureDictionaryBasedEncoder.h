#ifndef __mdkFeatureDictionaryBasedEncoder_h
#define __mdkFeatureDictionaryBasedEncoder_h


#include "mdkFeatureDictionary.h"


namespace mdk
{

template<typename ElementType>
class FeatureDictionaryBasedEncoder : public Object
{
protected:

    bool m_IsDenseEncoder;
    
public:

    FeatureDictionaryBasedEncoder();

    ~FeatureDictionaryBasedEncoder();

    //-----------------------------------------

    bool IsDenseEncoder();

    bool IsSparseEncoder();

    //-----------------------------------------

    virtual void Clear() = 0;
    
    //-----------------------------------------

    virtual bool SetInputFeatureData(const DenseMatrix<ElementType>* InputFeatureData) = 0;
    
    virtual bool SetInputDictionary(const FeatureDictionary<ElementType>* Dictionary) = 0;

    virtual bool SetOutputFeatureCode(DenseMatrix<ElementType>* FeatureCode) = 0;

    virtual bool SetMaxNumberOfThreads(int64 Number) = 0;

    //-----------------------------------------

    virtual bool Update();

    //----------------------------------------------------//

    virtual DenseMatrix<ElementType>* GetOutputFeatureCode() = 0;

protected:

    virtual int64 GetTotalNumberOfInputFeatureVectors() = 0;

    virtual int64 GetMaxNumberOfThreads() = 0;

    //output: IndexList_start and IndexList_end
    void DivideData(int64 Index_min, int64 Index_max, std::vector<int64>& IndexList_start, std::vector<int64>& IndexList_end);

    void GenerateCode_in_a_Thread(int64 IndexOfFeatureVector_start, int64 IndexOfFeatureVector_end);

    virtual void EncodingFunction(int64 IndexOfFeatureVector) = 0;

private:
//deleted:
    FeatureDictionaryBasedEncoder(const FeatureDictionaryBasedEncoder&) = delete;

    void operator=(const FeatureDictionaryBasedEncoder&) = delete;

};

}

#include "mdkFeatureDictionaryBasedEncoder.hpp"

#endif