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
    return GetMatrixNaNElement(ElementType(0));
}


template<typename ElementType>
inline
ElementType GetMatrixNaNElement(const ElementType& ReferenceElement)
{
    return ReferenceElement;
}


inline double GetMatrixNaNElement(const double&)
{
    return std::nan(nullptr);
}


inline float GetMatrixNaNElement(const float&)
{
    return std::nanf(nullptr);
}


inline char GetMatrixNaNElement(const char&)
{
    return char(0);
}


inline short GetMatrixNaNElement(const short&)
{
    return short(0);
}


inline int GetMatrixNaNElement(const int&)
{
    return int(0);
}


inline long GetMatrixNaNElement(const long&)
{
    return long(0);
}


inline long long GetMatrixNaNElement(const long long&)
{
    return long long(0);
}


inline unsigned char GetMatrixNaNElement(const unsigned char&)
{
    return unsigned char(0);
}


inline unsigned short GetMatrixNaNElement(const unsigned short&)
{
    return unsigned short(0);
}


inline unsigned int GetMatrixNaNElement(const unsigned int&)
{
    return unsigned int(0);
}


inline unsigned long GetMatrixNaNElement(const unsigned long&)
{
    return unsigned long(0);
}


inline unsigned long long GetMatrixNaNElement(const unsigned long long&)
{
    return unsigned long long(0);
}

//------------------------------------------------------------------------------//

inline std::vector<int_max> span(int_max Index_A, int_max Index_B)
{
    std::vector<int_max> IndexList;

    if (Index_A == Index_B)
    {
        IndexList.push_back(Index_A);
    }
    else if (Index_A < Index_B)
    {
        IndexList.reserve(Index_B - Index_A + 1);

        for (int_max i = Index_A; i <= Index_B; ++i)
        {
            IndexList.push_back(i);
        }
    }
    else //if (Index_A > Index_B)
    {
        IndexList.reserve(Index_A - Index_B + 1);

        for (int_max i = Index_A; i >= Index_B; --i)
        {
            IndexList.push_back(i);
        }
    }

    return IndexList;
}


inline std::vector<int_max> span(int_max Index_A, int_max Step, int_max Index_B)
{
    std::vector<int_max> IndexList;

    if (Index_A == Index_B && Step == 0)
    {
        IndexList.push_back(Index_A);
    }
    else if (Index_A < Index_B && Step > 0)
    {
        IndexList.reserve(Index_B - Index_A + 1);

        for (int_max i = Index_A; i <= Index_B; i += Step)
        {
            IndexList.push_back(i);
        }
    }
    else if (Index_A > Index_B && Step < 0)
    {
        IndexList.reserve(Index_A - Index_B + 1);

        for (int_max i = Index_A; i >= Index_B; i += Step)
        {
            IndexList.push_back(i);
        }
    }

    return IndexList;
}

}//end namespace mdk

#endif
