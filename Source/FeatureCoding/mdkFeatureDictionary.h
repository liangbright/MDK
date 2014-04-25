#ifndef __mdkFeatureDictionary_h
#define __mdkFeatureDictionary_h


#include "mdkObject.h"
#include "mdkString.h"

namespace mdk
{

template<typename ElementType>
class DenseMatrix;

struct MatrixSize;

template<typename ElementType>
class FeatureDictionary : Object
{

protected:
    FeatureDictionary() {}

    virtual ~FeatureDictionary() {}

public:
    virtual void Clear() = 0;

    virtual bool IsEmpty() const = 0;

    virtual MatrixSize GetSize() const = 0;

    virtual bool Load(const CharString& FilePathAndName) = 0;

    virtual bool Save(const CharString& FilePathAndName) const = 0;

    //------------------------------------------------------------//

    virtual CharString& Name() = 0;

    virtual const CharString& Name() const = 0;

    virtual DenseMatrix<ElementType>& BasisMatrix() = 0;

    virtual const DenseMatrix<ElementType>& BasisMatrix() const = 0;

private:
    FeatureDictionary(const FeatureDictionary&) = delete;

    void operator=(const FeatureDictionary&) = delete;
};


}// namespace mdk


#endif