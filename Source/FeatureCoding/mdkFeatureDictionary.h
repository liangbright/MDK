#ifndef __mdkFeatureDictionary_h
#define __mdkFeatureDictionary_h

#include "mdkDenseMatrix.h"

namespace mdk
{

template<typename ScalarType>
class FeatureDictionary : public Object
{
protected:
    FeatureDictionary() {}
    virtual ~FeatureDictionary() {}

public:
    virtual void Clear() = 0;

    virtual bool IsEmpty() const = 0;

	virtual MatrixSize GetSize() const = 0;

    virtual bool Load(const std::string& FilePathAndName) = 0;

    virtual bool Save(const std::string& FilePathAndName) const = 0;

    virtual const std::string& GetName() const = 0;

    virtual void SetName(const std::string& Name) = 0;

    virtual DenseMatrix<ScalarType>& BasisMatrix() = 0;

    virtual const DenseMatrix<ScalarType>& BasisMatrix() const = 0;

private:
    FeatureDictionary(const FeatureDictionary&) = delete;
    void operator=(const FeatureDictionary&) = delete;
};


}// namespace mdk


#endif