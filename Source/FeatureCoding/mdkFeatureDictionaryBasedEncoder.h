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
    

protected:
    FeatureDictionaryBasedEncoder();
    ~FeatureDictionaryBasedEncoder();

public:
    //-----------------------------------------

    bool IsDenseEncoder();

    bool IsSparseEncoder();

    //-----------------------------------------

    virtual void Clear() = 0;
    
    //-----------------------------------------

    virtual bool SetInputFeatureData(const DenseMatrix<ElementType>* InputFeatureData) = 0;
    
    virtual bool SetInputDictionary(const FeatureDictionary<ElementType>* Dictionary) = 0;

    virtual bool SetOutputFeatureCode(DenseMatrix<ElementType>* FeatureCode) = 0;

    virtual bool SetMaxNumberOfThreads(int_max Number) = 0;

    //-----------------------------------------

    virtual bool Update();

    //----------------------------------------------------//

    virtual DenseMatrix<ElementType>* GetOutputFeatureCode() = 0;

    //----------------------------------------------------//

protected:

    virtual int_max GetTotalNumberOfInputFeatureVectors() = 0;

    virtual int_max GetMaxNumberOfThreads() = 0;

    //output: IndexList_start and IndexList_end
    void DivideData(int_max Index_min, int_max Index_max, std::vector<int_max>& IndexList_start, std::vector<int_max>& IndexList_end);

    void GenerateCode_in_a_Thread(int_max IndexOfFeatureVector_start, int_max IndexOfFeatureVector_end);

    virtual void EncodingFunction(int_max IndexOfFeatureVector) = 0;

private:
//deleted:
    FeatureDictionaryBasedEncoder(const FeatureDictionaryBasedEncoder&) = delete;

    void operator=(const FeatureDictionaryBasedEncoder&) = delete;

};

}

#include "mdkFeatureDictionaryBasedEncoder.hpp"

#endif