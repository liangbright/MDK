#include <iostream>

#include "mdkType.h"

#include "TestEigen.h"

using namespace mdk;


int main()
{
    Test_Constructor();

    Test_LinearEquationSolver_LeastSquares();

    std::system("pause");

    return 1;
}