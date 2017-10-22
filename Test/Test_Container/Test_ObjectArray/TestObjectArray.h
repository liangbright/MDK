#pragma once
//================================================
#include <iostream>

#include "mdkContainer.h"

using namespace mdk;

void Test_A()
{	
	ObjectArray<DenseMatrix<double>> A, B, A1, A2, A3;
	A.Resize(1);
	A[0].Resize(1, 2);

	A1.Resize(1);
	A1[0].Resize(2, 3);

	A2.Resize(2);
	A2[0].Resize(3, 4);
	A2[1].Resize(4, 5);

	A3.Resize(3);
	A3[0].Resize(5, 6);
	A3[1].Resize(6, 7);
	A3[2].Resize(7, 8);

	B = { &A, &A1, &A2, &A3 };
	A= {&A, &A1, &A2, &A3 };	

	A = { &A };

	A.Insert(0, { 0.1, 0.2 });
}

void Test_Append()
{
	ObjectArray<DenseVector<int_max>> A;
	A.Resize(1);

	A.Append({1, 2});
}