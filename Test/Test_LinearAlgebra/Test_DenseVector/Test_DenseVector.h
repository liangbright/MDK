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

void Test_GetSubSet()
{

    using namespace mdk;

    DenseVector<double, 10> A;
    A.Fill(1);

    auto subA = A.GetSubSet(0, 1);


    DenseVector<double> B;

    B = A;

    auto subB = B.GetSubSet(0, 1);


    DenseVector<double, 10> C;

    C = B;

    auto subC = C.GetSubSet(0, 1);

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

void Test_DataArray_Vector()
{
    using namespace mdk;

    DataArray<DenseVector<int_max, 2>> List;

    List.FastResize(10);
}