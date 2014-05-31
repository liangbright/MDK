#include <algorithm>
#include <chrono>
#include <ctime>
#include <iomanip>

#include "mdkDenseMatrix.h"
#include "mdkDenseVector.h"

void Test_Constructor()
{
    using namespace mdk;

    DenseVector<double, 3> A;

    DenseVector<double> B;

    B.FastResize(10);
    B.Fill(1);

    DisplayVector("A", A);

    DisplayVector("B", B);
}

void Test_GetSubVector()
{

    using namespace mdk;

    DenseVector<double, 10> A;
    A.Fill(1);

    auto subA = A.GetSubVector(0, 1);


    DenseVector<double> B;

    B = A;

    auto subB = B.GetSubVector(0, 1);


    DenseVector<double, 10> C;

    C = B;

    auto subC = C.GetSubVector(0, 1);

    DisplayVector("subA", subA);

    DisplayVector("subB", subB);

    DisplayVector("subC", subC);

}

void Test_Operator()
{
    using namespace mdk;

    DenseVector<double, 10> A;
    A.Fill(1);

    DenseVector<double> B;

    B = A;

    auto C = A*B;

    DisplayVector("C", C);

    auto A1 = A * 2.0;

    auto B1 = B * 2.0;


}