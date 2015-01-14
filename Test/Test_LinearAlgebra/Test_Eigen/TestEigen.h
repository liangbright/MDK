#ifndef Test_Eigen_h
#define Test_Eigen_h

//================================================
#include <algorithm>
#include <chrono>
#include <ctime>
#include <iomanip>

#include <Eigen/Dense>

#include "mdkDenseMatrix.h"

void Test_Constructor()
{
	using namespace mdk;

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
	using namespace mdk;

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

	auto OutputY = SolveMatrixLinearEquation(InputA, InputB);

	DisplayMatrix("OutputY", OutputY);

}

#endif