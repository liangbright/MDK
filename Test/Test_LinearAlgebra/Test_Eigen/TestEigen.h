#ifndef __TestEigen_h
#define __TestEigen_h

//================================================
#include <algorithm>
#include <chrono>
#include <ctime>
#include <iomanip>

#include <Eigen/Dense>

#include "mdkDenseMatrix.h"

namespace mdk
{

template<typename T>
void DisplayMatrix(const std::string& Name, const DenseMatrix<T>& Matrix, uint32 value_std_setw = 6, uint32 precision = 4)
{
    std::cout << Name << " = " << '\n';

    for (int64 i = 0; i < Matrix.GetRowNumber(); ++i)
    {
        for (int64 j = 0; j < Matrix.GetColNumber(); ++j)
        {
            //std::cout << std::fixed << std::setprecision(precision) << Matrix(i, j) << ' ';

            std::cout << std::setw(value_std_setw + precision) << std::setprecision(precision) << Matrix(i, j) << ' ';
        }
        std::cout << '\n';
    }
}




void Test_Constructor()
{
    DenseMatrix<double> A;

    A.Resize(5, 6);

    A = { { 1, 2, 3, 4, 5, 6 },
          { 10, 20, 30, 40, 50, 60 },
          { 100, 200, 300, 40, 500, 600 },
          { 1000, 2000, 3000, 4000, 5000, 6000 },
          { 10000, 20000, 30000, 40000, 50000, 60000 } };

    DisplayMatrix("A", A);


    Eigen::MatrixXd B(2, 2);

    B(0, 0) = 3;
    B(1, 0) = 2.5;
    B(0, 1) = -1;
    B(1, 1) = B(1, 0) + B(0, 1);

    std::cout << B << '\n';
  

    Eigen::Map<Eigen::MatrixXd> C(A.GetElementPointer(), 5, 6);

    std::cout << C << '\n';

}


void Test_LinearEquationSolver_LeastSquares()
{
    DenseMatrix<double> InputA(10, 2);

    InputA.FillCol(0, 1.0);

    InputA.FillCol(1, 1.0);

    DisplayMatrix("InputA", InputA);

    DenseMatrix<double> InputB(10, 1);

    InputB.Fill(2.0);

    DisplayMatrix("InputB", InputB);

    DenseMatrix<double> OutputX(2, 1);

    DenseMatrix<double> OutputResidual(10, 1);

    Eigen::Map<Eigen::MatrixXd> A(InputA.GetElementPointer(), 10, 2);

    Eigen::Map<Eigen::MatrixXd> B(InputB.GetElementPointer(), 10, 1);

    Eigen::Map<Eigen::MatrixXd> X(OutputX.GetElementPointer(), 2, 1);

    Eigen::Map<Eigen::MatrixXd> Residual(OutputResidual.GetElementPointer(), 10, 1);

    X = A.jacobiSvd(Eigen::ComputeThinU | Eigen::ComputeThinV).solve(B);

    Residual = A*X - B;

    std::cout << "The least-squares solution is:\n" << X << '\n';

    DisplayMatrix("OutputX", OutputX);

    auto Result = SolveLinearLeastSquaresProblem(InputA, InputB);

    OutputX = Result.X;

    DisplayMatrix("OutputX", OutputX);

}


}//namespace mdk

#endif