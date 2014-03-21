#ifndef __TestSparseMatrix_h
#define __TestSparseMatrix_h

//================================================
#include <algorithm>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <initializer_list>
#include <functional>

#include "mdkDenseMatrix.h"

#include "mdkSparseMatrix.h"


using namespace mdk;

template<typename T>
void DisplayMatrix(const std::string& Name, const mdkSparseMatrix<T>& Matrix, uint32 value_std_setw = 6, uint32 precision = 0)
{
    std::cout << Name << " = " << '\n';

    for (int64 i = 0; i < Matrix.GetRowNumber(); ++i)
    {
        for (int64 j = 0; j < Matrix.GetColNumber(); ++j)
        {
            //std::cout << std::fixed << std::setprecision(precision) << Matrix(i, j) << ' ';

            std::cout << std::setw(value_std_setw + precision) << std::setprecision(precision) << Matrix.GetElement(i, j);
        }
        std::cout << '\n';
    }
}



void Test_Constructor()
{
    std::cout << "Test_Constructor()" << '\n';

    mdkSparseMatrix<double> A;

    A.Construct({ 0, 1 , 2, 3}, { 0, 1 , 2, 3}, { 1, 2, 3, 4}, 10, 10);


    DisplayMatrix("A", A);

    A(0, 1) = 101;

    DisplayMatrix("A", A);


    A(1, 2) = 12;

    DisplayMatrix("A", A);


    A(2, 5) = 25;

    DisplayMatrix("A", A);

    A(3, 4) = 34;

    DisplayMatrix("A", A);

    A(4, 3) = 43;

    DisplayMatrix("A", A);

    A(6, 6) = 66;

    DisplayMatrix("A", A);

    A(7, 7) = 77;

    DisplayMatrix("A", A);

    A(8, 8) = 88;

    DisplayMatrix("A", A);

    A(9, 9) = 99;

    DisplayMatrix("A", A);

    A(9, 8) = 98;

    DisplayMatrix("A", A);

    A(8, 9) = 89;

    DisplayMatrix("A", A);

}


#endif