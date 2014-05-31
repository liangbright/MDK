#ifndef __mdkDisplayMatrix_hpp
#define __mdkDisplayMatrix_hpp

namespace mdk
{

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


template<typename ElementType, int_max Length>
void DisplayVector(const std::string& Name, const DenseVector<ElementType, Length>& InputVector, int_max precision, bool Flag_scientific)
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

    for (int_max i = 0; i < InputVector.GetElementNumber(); ++i)
    {
        std::cout << std::setw(6 + precision) << InputVector[i] << ' ';
    }

    std::cout << '\n';
}


template<typename ElementType>
void DisplayVector(const std::string& Name, const SparseVector<ElementType>& InputVector, int_max precision, bool Flag_scientific)
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

    for (int_max i = 0; i < InputVector.GetElementNumber(); ++i)
    {
        std::cout << std::setw(6 + precision) << InputVector[i] << ' ';
    }

    std::cout << '\n';
}

}//end namespace mdk


#endif