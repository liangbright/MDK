#include <algorithm>
#include <chrono>
#include <ctime>
#include <iomanip>

#include "mdkDenseMatrix.h"
#include "mdkDenseVector.h"

void Test_Constructor()
{
    using namespace mdk;

	DenseVector<double, 3> A = { 1, 2, 3 };

    DenseVector<double> B;

    B.FastResize(10);
    B.Fill(1);

    DisplayVector("A", A);

    DisplayVector("B", B);
}

void Test_GetSubSet()
{

    using namespace mdk;

	DenseVector<double, 10> A = { 0, 1, 2, 3, 4, 5, 6, 7, 8, 9 };

    auto subA1 = A.GetSubSet(0, 1);
	auto subA2 = A.GetSubSet(2, 3);
	auto subA3 = A.GetSubSet(4, 9);

	DisplayVector("A", A);
	DisplayVector("subA1", subA1);
	DisplayVector("subA2", subA2);
	DisplayVector("subA3", subA3);

	DenseVector<double> B = A;

    auto subB = B.GetSubSet(0, 1);

	DenseVector<double, 10> C = A;

    auto subC = C.GetSubSet(0, 1);

    DisplayVector("subB", subB);

    DisplayVector("subC", subC);

}

void Test_Operator()
{
    using namespace mdk;

    DenseVector<double, 10> A;
    A.Fill(1);

    DenseVector<double> B;

    B = A + 1.0;

    auto C = A*B;
    auto A1 = A * 2.0;
    auto B1 = B * 2.0;

	DisplayVector("A1", A1);
	DisplayVector("B1", B1);
	DisplayVector("C", C);

}
