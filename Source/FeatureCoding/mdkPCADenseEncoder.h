#ifndef __mdkPCADenseEncoder_h
#define __mdkPCADenseEncoder_h

#include <string>

#include "mdkFeatureDictionaryBasedDenseEncoder.h"


namespace mdk
{

template<typename ElementType>
class PCADenseEncoder : public FeatureDictionaryBasedDenseEncoder<ElementType>
{
protected:



protected:
    PCADenseEncoder();
    ~PCADenseEncoder();

public:
    //-----------------------------------------

    virtual void Clear();
    
    //-----------------------------------------

    bool SetInputFeatureData(const DenseMatrix<ElementType>* InputFeatureData);

    bool SetInputDictionary(const FeatureDictionary<ElementType>* Dictionary);

    bool SetOutputFeatureCode(DenseMatrix<ElementType>* FeatureCode);

    //-----------------------------------------

    virtual bool Update();

    //----------------------------------------------------//

    DenseMatrix<ElementType>* GetOutputFeatureCode();


protected:

    int_max GetMaximunNumberOfThreads();

    int_max GetFeatureVectorNumber();

    virtual void GenerateCode_in_a_Thread(int_max IndexOfFeatureVector_start, int_max IndexOfFeatureVector_end);

private:
//deleted:
    FeatureDictionaryBasedDenseEncoder(const FeatureDictionaryBasedDenseEncoder&) = delete;

    void operator=(const FeatureDictionaryBasedDenseEncoder&) = delete;

};

}

#include "mdkFeatureDictionaryBasedDenseEncoder.hpp"

#endif