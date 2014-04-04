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

    virtual bool SetOutputCode(DenseMatrix<ElementType>* Code) = 0;

    virtual bool SetMaxNumberOfThreads(int_max Number) = 0;

    //-----------------------------------------------------//

    virtual bool Update();

    //-----------------------------------------------------//

    virtual bool CheckInputAndOutput();

    virtual bool Preprocess();

    virtual bool Postprocess();

    //----------------------------------------------------//

    virtual inline void EncodingFunction(const DenseMatrix<ElementType>& SingleFeatureDataVector, 
                                         DenseMatrix<ElementType>& CodeInDenseVector) = 0;

    //----------------------------------------------------//

    virtual DenseMatrix<ElementType>* GetOutputCode() = 0;

    //----------------------------------------------------//
    // just put it here for reference
    // Clone self using new and return unique_ptr
    // only copy the parameter of the Encoder
    // then a copy is independent with the others, and can be used in a thread
    // But, this will cause huge problem
    // a virtual destructor chain is needed to delete the real object instead of the base class
    // so, avoid virtual destructor
    //
    // virtual std::unique_ptr<FeatureDictionaryBasedEncoder<ElementType>> Clone();

    //----------------------------------------------------//

protected:

    virtual int_max GetTotalNumberOfInputFeatureVectors() = 0;

    virtual int_max GetMaxNumberOfThreads() = 0;

    //output: IndexList_start and IndexList_end
    void DivideData(int_max Index_min, int_max Index_max, std::vector<int_max>& IndexList_start, std::vector<int_max>& IndexList_end);

    virtual void GenerateCode_in_a_Thread(int_max IndexOfFeatureDataVector_start, int_max IndexOfFeatureDataVector_end) = 0;

private:
//deleted:
    FeatureDictionaryBasedEncoder(const FeatureDictionaryBasedEncoder&) = delete;

    void operator=(const FeatureDictionaryBasedEncoder&) = delete;

};

}

#include "mdkFeatureDictionaryBasedEncoder.hpp"

#endif