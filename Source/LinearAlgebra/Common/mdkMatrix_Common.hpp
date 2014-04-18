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


template<typename ElementType>
void DisplayMatrix(const std::string& Name, const DenseMatrix<ElementType>& InputMatrix, int_max precision, bool Flag_scientific)
{
    std::cout << Name << " = " << '\n';

    if (Flag_scientific == true)
    {
        std::cout << std::scientific << std::setprecision(precision);
    }
    else
    {
        std::cout << std::setprecision(precision) << std::fixed;
    }

    for (int_max i = 0; i < InputMatrix.GetRowNumber(); ++i)
    {
        for (int_max j = 0; j < InputMatrix.GetColNumber(); ++j)
        {
            std::cout << std::setw(6 + precision) << InputMatrix(i, j) << ' ';
        }
        std::cout << '\n';
    }
}


template<typename ElementType>
void DisplayMatrix(const std::string& Name, const SparseMatrix<ElementType>& InputMatrix, int_max precision, bool Flag_scientific)
{
    std::cout << Name << " = " << '\n';

    if (Flag_scientific == true)
    {
        std::cout << std::scientific << std::setprecision(precision);
    }
    else
    {
        std::cout << std::setprecision(precision) << std::fixed;
    }

    for (int_max i = 0; i < InputMatrix.GetRowNumber(); ++i)
    {
        for (int_max j = 0; j < InputMatrix.GetColNumber(); ++j)
        {
            std::cout << std::setw(6 + precision) << InputMatrix.GetElement(i, j) << ' ';
        }
        std::cout << '\n';
    }
}


}//end namespace mdk

#endif
