#pragma once

//================================================
#include <algorithm>
#include <chrono>
#include <ctime>
#include <iomanip>

#include <Eigen/Dense>

#include "mdkDenseMatrix.h"
#include "mdkDenseMatrix_FileIO.h"

using namespace mdk;

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

void Test_pinv()
{
	DenseMatrix<double> A;
	LoadDenseMatrixFromJsonDataFile(A, "C:/Research/MDK/MDK_Build/Test/Test_LinearAlgebra/Test_Eigen/TestData/A.json");
	DisplayMatrix("A", A);
	auto pinvA =A.PInv();
	DisplayMatrix("pinvA", pinvA);
	SaveDenseMatrixAsJsonDataFile(pinvA, "C:/Research/MDK/MDK_Build/Test/Test_LinearAlgebra/Test_Eigen/TestData/pinvA.json");
}

void Test_inv()
{
	DenseMatrix<double> A;
	LoadDenseMatrixFromJsonDataFile(A, "C:/Research/MDK/MDK_Build/Test/Test_LinearAlgebra/Test_Eigen/TestData/A.json");
	DisplayMatrix("A", A);
	auto invA = A.Inv();
	DisplayMatrix("invA", invA);
	SaveDenseMatrixAsJsonDataFile(invA, "C:/Research/MDK/MDK_Build/Test/Test_LinearAlgebra/Test_Eigen/TestData/invA.json");
}

void Test_rank()
{
	DenseMatrix<double> A;
	LoadDenseMatrixFromJsonDataFile(A, "C:/Research/MDK/MDK_Build/Test/Test_LinearAlgebra/Test_Eigen/TestData/A.json");	
	auto rankA = A.Rank();	
	std::cout << "rankA=" << rankA << '\n';
}

void Test_det()
{
	DenseMatrix<double> A;
	LoadDenseMatrixFromJsonDataFile(A, "C:/Research/MDK/MDK_Build/Test/Test_LinearAlgebra/Test_Eigen/TestData/A.json");
	auto detA = A.Det();
	std::cout << "detA=" << detA << '\n';
}

void Test_SymmetricRealMatrixEigen()
{
	DenseMatrix<double> A;
	LoadDenseMatrixFromJsonDataFile(A, "C:/Research/MDK/MDK_Build/Test/Test_LinearAlgebra/Test_Eigen/TestData/A.json");	
	auto Result = SymmetricRealMatrixEigen(A);	
	SaveDenseMatrixAsJsonDataFile(Result.EigenValue, "C:/Research/MDK/MDK_Build/Test/Test_LinearAlgebra/Test_Eigen/TestData/EigenA_S.json");
	SaveDenseMatrixAsJsonDataFile(Result.EigenVector, "C:/Research/MDK/MDK_Build/Test/Test_LinearAlgebra/Test_Eigen/TestData/EigenA_V.json");
}