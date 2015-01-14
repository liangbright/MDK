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

	DenseVector<double, 1> OneElementVectorA;
	OneElementVectorA[0] = 12;

	DenseVector<double> OneElementVectorB;
	OneElementVectorB.Resize(1);
	OneElementVectorB[0] = 12;

    DenseVector<double> B;

    B = A + 1.0;

	// these should not pass compiler because Vector * Vector is not defined in mdk
	//auto AA = A*A;
	//auto BB = B*B;
	//auto AB = A*B;
	//auto BA = B*A;
	//-----------------------------

	auto tempA1 = A + OneElementVectorA;
	auto tempA2 = OneElementVectorA + A;
	auto tempA3 = A - OneElementVectorA;
	auto tempA4 = OneElementVectorA - A;
	auto tempA5 = A / OneElementVectorA;
	auto tempA6 = OneElementVectorA / A;

	auto tempA7  = A + OneElementVectorB;
	auto tempA8  = OneElementVectorB + A;
	auto tempA9  = A - OneElementVectorB;
	auto tempA10 = OneElementVectorB - A;
	auto tempA11 = A / OneElementVectorB;
	auto tempA12 = OneElementVectorB / A;

	auto tempB1 = B + OneElementVectorA;
	auto tempB2 = OneElementVectorA + B;
	auto tempB3 = B - OneElementVectorA;
	auto tempB4 = OneElementVectorA - B;
	auto tempB5 = B / OneElementVectorA;
	auto tempB6 = OneElementVectorA / B;

	auto tempB7  = B + OneElementVectorB;
	auto tempB8  = OneElementVectorB + B;
	auto tempB9  = B - OneElementVectorB;
	auto tempB10 = OneElementVectorB - B;
	auto tempB11 = B / OneElementVectorB;
	auto tempB12 = OneElementVectorB / B;

	auto A1 = A * 2.0;
	auto B1 = B * 2.0;

	auto ApA = A + A;
	auto AmA = A - A;

	auto BpB = B + B;
	auto BmB = B - B;

    auto ApB = A + B;
	auto BpA = B + A;

	auto AmB = A - B;
	auto BmA = B - A;
}
