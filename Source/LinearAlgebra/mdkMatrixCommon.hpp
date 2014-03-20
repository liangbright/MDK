#ifndef __mdkMatrixCommon_hpp
#define __mdkMatrixCommon_hpp

//#include "mdkMatrixCommon.h"

#include "mdkDebugConfig.h"

namespace mdk
{

template<typename ElementType>
inline
mdkMatrixElementTypeEnum FindMatrixElementType(ElementType Element)
{
    std::string TypeName(typeid(Element).name());

    if (TypeName == "double")
    {
        return mdkMatrixElementTypeEnum::Scalar_DOUBLE64;
    }
    else if (TypeName == "float")
    {
        return mdkMatrixElementTypeEnum::Scalar_FLOAT32;
    }
    else if (TypeName == "signed char")
    {
        return mdkMatrixElementTypeEnum::Scalar_INT8;
    }
    else if (TypeName == "short")
    {
        return mdkMatrixElementTypeEnum::Scalar_INT16;
    }
    else if (TypeName == "int")
    {
        return mdkMatrixElementTypeEnum::Scalar_INT32;
    }
    else if (TypeName == "__int64") // OS is Windows
    {
        return mdkMatrixElementTypeEnum::Scalar_INT64;
    }
    else if (TypeName == "unsigned char")
    {
        return mdkMatrixElementTypeEnum::Scalar_UINT8;
    }
    else if (TypeName == "unsigned short")
    {
        return mdkMatrixElementTypeEnum::Scalar_UINT16;
    }
    else if (TypeName == "unsigned int")
    {
        return mdkMatrixElementTypeEnum::Scalar_UINT32;
    }
    else if (TypeName == "unsigned __int64") // OS is Windows
    {
        return mdkMatrixElementTypeEnum::Scalar_UINT64;
    }

    return mdkMatrixElementTypeEnum::UNKNOWN;
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
    case mdkMatrixElementTypeEnum::Scalar_DOUBLE64:
        return ElementType(std::nan(nullptr));

    case mdkMatrixElementTypeEnum::Scalar_FLOAT32:
        return std::nanf(nullptr);

    case mdkMatrixElementTypeEnum::StdVector_DOUBLE64:
        return ReferenceElement + ElementType(std::nan(nullptr));

    case mdkMatrixElementTypeEnum::StdVector_FLOAT32:
        return ReferenceElement + std::nanf(nullptr);

    case mdkMatrixElementTypeEnum::StdArray_DOUBLE64:
        return ReferenceElement + ElementType(std::nan(nullptr));

    case mdkMatrixElementTypeEnum::StdArray_FLOAT32:
        return ReferenceElement + std::nanf(nullptr);

    default:
        mdkWarning << "ElementType is not float or double, so NaNElement is set to zero @ GetMatrixNaNElement()" << '\n';
        return ReferenceElement - ReferenceElement;
    }
}


}//end namespace mdk

#endif
