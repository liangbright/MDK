#include <iostream>

#include "mdkCommonType.h"

#include "TestEigen.h"

int main()
{
    Test_Constructor();

    Test_LinearEquationSolver_LeastSquares();

    std::system("pause");

    return 1;
}