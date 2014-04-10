#ifndef __mdkFeatureDictionary_h
#define __mdkFeatureDictionary_h

#include <string>

#include "mdkObject.h"

namespace mdk
{

template<typename ElementType>
class DenseMatrix;

struct MatrixSize;

template<typename ElementType>
class FeatureDictionary : Object
{

public:

    FeatureDictionary() {}

    FeatureDictionary(const FeatureDictionary& InputDictionary) {}

    FeatureDictionary(FeatureDictionary&& InputDictionary) {}

    virtual ~FeatureDictionary() {}

    virtual void Clear() = 0;

    virtual bool IsEmpty() const = 0;

    virtual MatrixSize GetSize() const = 0;

    virtual bool Load(const std::string& FilePathAndName) = 0;

    virtual bool Save(const std::string& FilePathAndName) const = 0;

    //------------------------------------------------------------//

    virtual const std::string& GetName() const = 0;

    virtual DenseMatrix<ElementType>& BasisMatrix() = 0;

    virtual const DenseMatrix<ElementType>& BasisMatrix() const = 0;
};


}// namespace mdk


#endif