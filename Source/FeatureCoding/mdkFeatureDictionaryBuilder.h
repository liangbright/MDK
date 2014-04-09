#ifndef __mdkFeatureDictionaryBuilder_h
#define __mdkFeatureDictionaryBuilder_h

#include "mdkObject.h"

namespace mdk
{

template<typename ElementType>
class FeatureDictionaryBuilder : public Object
{
public:

    FeatureDictionaryBuilder() {}
    virtual ~FeatureDictionaryBuilder() {}

    //---------------------------------------------------//

    virtual void Clear() = 0;

    //---------------------------------------------------//

    void SetInputFeatureData() {} // just for reminder

    //----------------------------------------------------//

    void SetOutputDictionary() {} // just for reminder
 
    //----------------------------------------------------//

    virtual bool CheckInput() = 0;

    //----------------------------------------------------//

    virtual bool Update();

    //----------------------------------------------------//

    void GetOutputDictionary() {} // just for reminder

protected:
    virtual void GenerateDictionary() = 0;

private:
    FeatureDictionaryBuilder(const FeatureDictionaryBuilder&) = delete;

    void operator=(const FeatureDictionaryBuilder&) = delete;

    FeatureDictionaryBuilder(FeatureDictionaryBuilder&&) = delete;

    void operator=(FeatureDictionaryBuilder&&) = delete;
};

}// namespace mdk

#endif