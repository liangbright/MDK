#ifndef __mdkFeatureDictionaryBuilder_h
#define __mdkFeatureDictionaryBuilder_h

#include "mdkProcessObject.h"

namespace mdk
{

template<typename ElementType>
class FeatureDictionaryBuilder : public ProcessObject
{

protected:
    FeatureDictionaryBuilder();
    virtual ~FeatureDictionaryBuilder();

public:
    //---------------------------------------------------//
    // just for reminder, must be overloaded
    void SetInputFeatureData() {MDK_Error("Empty Function @ FeatureDictionaryBuilder::SetInputFeatureData()")}

    //----------------------------------------------------//
    // just for reminder, must be overloaded
    void SetOutputDictionary() {MDK_Error("Empty Function @ FeatureDictionaryBuilder::SetOutputDictionary()")}
 
    //----------------------------------------------------//

    virtual bool CheckInput() = 0;

    //----------------------------------------------------//

    virtual bool Update();
  
    //----------------------------------------------------//
    // just for reminder, must be overloaded
    void GetOutputDictionary() {MDK_Error("Empty Function @ FeatureDictionaryBuilder::SetOutputDictionary()") } 

    //---------------------------------------------------//
protected:
    virtual void GenerateDictionary() = 0;

    virtual void ClearPipelineOutput() = 0;

    virtual void UpdatePipelineOutput() = 0;

private:
    FeatureDictionaryBuilder(const FeatureDictionaryBuilder&) = delete;

    void operator=(const FeatureDictionaryBuilder&) = delete;

    FeatureDictionaryBuilder(FeatureDictionaryBuilder&&) = delete;

    void operator=(FeatureDictionaryBuilder&&) = delete;
};

}// namespace mdk

#include "mdkFeatureDictionaryBuilder.hpp"

#endif