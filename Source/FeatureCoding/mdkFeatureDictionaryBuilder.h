#ifndef __mdkFeatureDictionaryBuilder_h
#define __mdkFeatureDictionaryBuilder_h

#include "mdkObject.h"

namespace mdk
{

template<typename DictionaryType>
class FeatureDictionaryBuilder : public Object
{
public:
	typedef typename DictionaryType::ScalarType ScalarType;

protected:
	FeatureDictionaryBuilder() {}
	virtual ~FeatureDictionaryBuilder() {}

public:
	virtual void SetInputFeatureData(const DenseMatrix<ScalarType>* FeatureData) = 0;

    virtual bool CheckInput() = 0;

	virtual bool Update() = 0;

	virtual DictionaryType* GetOutputDictionary() = 0;

private:
    FeatureDictionaryBuilder(const FeatureDictionaryBuilder&) = delete;
    void operator=(const FeatureDictionaryBuilder&) = delete;
};

}// namespace mdk

#endif