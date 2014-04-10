#ifndef __mdkFeatureDictionaryBuilder_h
#define __mdkFeatureDictionaryBuilder_h

#include "mdkProcessObject.h"

namespace mdk
{

template<typename ElementType>
class FeatureDictionaryBuilder : public ProcessObject
{
public:

    FeatureDictionaryBuilder() {}
    virtual ~FeatureDictionaryBuilder() {}

    //---------------------------------------------------//
    // just for reminder
    void SetInputFeatureData() {MDK_Error("Empty Function @ FeatureDictionaryBuilder::SetInputFeatureData()")}

    //----------------------------------------------------//
    // just for reminder
    void SetOutputDictionary() {MDK_Error("Empty Function @ FeatureDictionaryBuilder::SetOutputDictionary()")}
 
    //----------------------------------------------------//

    virtual bool CheckInput() = 0;

    //----------------------------------------------------//

    virtual bool Update();

    //----------------------------------------------------//
    // just for reminder
    void GetOutputDictionary() {MDK_Error("Empty Function @ FeatureDictionaryBuilder::SetOutputDictionary()") } 

protected:
    virtual void GenerateDictionary() = 0;

    virtual void SetupDefaultPipelineOutput() = 0;

    virtual void UpdatePipelineOutput() = 0;

private:
    FeatureDictionaryBuilder(const FeatureDictionaryBuilder&) = delete;

    void operator=(const FeatureDictionaryBuilder&) = delete;

    FeatureDictionaryBuilder(FeatureDictionaryBuilder&&) = delete;

    void operator=(FeatureDictionaryBuilder&&) = delete;
};

}// namespace mdk

#endif