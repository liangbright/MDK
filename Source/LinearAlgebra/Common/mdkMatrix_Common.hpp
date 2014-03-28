#ifndef __mdkMatrixCommon_hpp
#define __mdkMatrixCommon_hpp

//#include "mdkMatrixCommon.h"

#include "mdkDebugConfig.h"

namespace mdk
{

template<typename ElementType>
inline
MatrixElementTypeEnum FindMatrixElementType(ElementType Element)
{
    std::string TypeName(typeid(Element).name());

    if (TypeName == "double")
    {
        return MatrixElementTypeEnum::Scalar_DOUBLE64;
    }
    else if (TypeName == "float")
    {
        return MatrixElementTypeEnum::Scalar_FLOAT32;
    }
    else if (TypeName == "signed char")
    {
        return MatrixElementTypeEnum::Scalar_INT8;
    }
    else if (TypeName == "short")
    {
        return MatrixElementTypeEnum::Scalar_INT16;
    }
    else if (TypeName == "int")
    {
        return MatrixElementTypeEnum::Scalar_INT32;
    }
    else if (TypeName == "__int64") // OS is Windows
    {
        return MatrixElementTypeEnum::Scalar_INT64;
    }
    else if (TypeName == "unsigned char")
    {
        return MatrixElementTypeEnum::Scalar_UINT8;
    }
    else if (TypeName == "unsigned short")
    {
        return MatrixElementTypeEnum::Scalar_UINT16;
    }
    else if (TypeName == "unsigned int")
    {
        return MatrixElementTypeEnum::Scalar_UINT32;
    }
    else if (TypeName == "unsigned __int64") // OS is Windows
    {
        return MatrixElementTypeEnum::Scalar_UINT64;
    }

    return MatrixElementTypeEnum::UNKNOWN;
}


template<typename ElementType>
inline
ElementType GetMatrixNaNElement()
{
    ElementType Element;

    Element = Element - Element;

    return GetMatrixNaNElement(Element);
}


template<typename ElementType>
inline
ElementType GetMatrixNaNElement(ElementType ReferenceElement)
{
    auto TypeEnum = FindMatrixElementType(ReferenceElement);

    switch (TypeEnum)
    {
    case MatrixElementTypeEnum::Scalar_DOUBLE64:
        return ElementType(std::nan(nullptr));

    case MatrixElementTypeEnum::Scalar_FLOAT32:
        return ElementType(std::nanf(nullptr));

    case MatrixElementTypeEnum::StdVector_DOUBLE64:
        return ReferenceElement + ElementType(std::nan(nullptr));

    case MatrixElementTypeEnum::StdVector_FLOAT32:
        return ReferenceElement + ElementType(std::nanf(nullptr));

    case MatrixElementTypeEnum::StdArray_DOUBLE64:
        return ReferenceElement + ElementType(std::nan(nullptr));

    case MatrixElementTypeEnum::StdArray_FLOAT32:
        return ReferenceElement + ElementType(std::nanf(nullptr));

    default:
        //MDK_Warning << "ElementType is not float or double, so NaNElement is set to zero @ GetMatrixNaNElement()" << '\n';
        return ReferenceElement - ReferenceElement;
    }
}


}//end namespace mdk

#endif
