#ifndef __TestMatrix_h
#define __TestMatrix_h

#include "mdkMatrix.h"
#include "mdkLinearAlgebra.h"

namespace mdk
{

void TestMatrix_ConstPointer_function(const double* p)
{
	//p[1] = 2;
}

void TestMatrix_ConstPointer()
{
	double a[3] = { 1, 2, 3 };
	TestMatrix_ConstPointer_function(a);
}

// use resize and operator[]
// do not use insert or emplace : meaningless
void TestMatrix_std_vector()
{
	std::vector<int> vec1(100);
	auto it1 = vec1.begin();
	vec1.insert(it1 + 10, 200);
	vec1[10] = 200;

	std::vector<int> vec2;

	vec2.reserve(100);

	auto it2 = vec2.begin();

	//crash
	//vec2.insert(it2 + 10, 200);

	//crash
	//vec2[10] = 200;

	std::vector<int> vec3;

	//vec3.reserve(100);

	vec3.resize(100);

	auto it3 = vec3.begin();

	// lead to crash vec3.insert(it3 + 10, 10);
	//vec3.insert(it3, 1);

	vec3[0] = 1;

	std::cout << vec3[0] << '\n';

	//crash
    vec3.insert(it3 + 10, 10);

	vec3[10] = 10;

	std::cout << vec3[10] << '\n';
	
	//crash
	//vec3.insert(it3 + 20, 20);

	//good
	//it3 = vec3.begin();
	//vec3.insert(it3 + 20, 20);

	vec3.emplace(it3 + 20, 20);

	std::cout << vec3[20] << '\n';
}

void TestMatrix_Mutiplication()
{

	mdkMatrix<double> DoubleMatrixA;

	DoubleMatrixA.SetSize(2, 4);

	DoubleMatrixA = { 1, 2, 3, 4, 
		              5, 6, 7, 8 };
	
	std::cout << "DoubleMatrixA" << '\n';

	for (uint64 i = 0; i < 2; ++i)
	{
		for (uint64 j = 0; j < 4; ++j)
		{
			std::cout << DoubleMatrixA(i, j) << ' ';
		}

		std::cout << '\n';
	}


	mdkMatrix<double> DoubleMatrixB;

	DoubleMatrixB.SetSize(4, 2);

	DoubleMatrixB = { 1, 2, 
		              3, 4, 
					  5, 6, 
					  7, 8 };

	std::cout << "DoubleMatrixB" << '\n';

	for (uint64 i = 0; i < 4; ++i)
	{
		for (uint64 j = 0; j < 2; ++j)
		{
			std::cout << DoubleMatrixB(i, j) << ' ';
		}

		std::cout << '\n';
	}

	auto DoubleMatrixC = DoubleMatrixA*DoubleMatrixB;

	std::cout << "DoubleMatrixC = DoubleMatrixA * DoubleMatrixB" << '\n';

	for (uint64 i = 0; i < 2; ++i)
	{
		for (uint64 j = 0; j < 2; ++j)
		{
			std::cout << DoubleMatrixC(i, j) << ' ';
		}

		std::cout << '\n';
	}

	DoubleMatrixA *= 1.25;

	std::cout << "DoubleMatrixA * 1.25 =" << '\n';

	for (uint64 i = 0; i < 2; ++i)
	{
		for (uint64 j = 0; j < 4; ++j)
		{
			std::cout << DoubleMatrixA(i, j) << ' ';
		}

		std::cout << '\n';
	}


	mdkMatrix<double> DoubleMatrixD(2, 10);
	DoubleMatrixD = {  1,  2,  3,  4,  5,  6,  7,  8,  9, 10,
		              11, 12, 13, 14, 15, 16, 17, 18, 19, 20 };

	std::cout << "DoubleMatrixD =" << '\n';
	for (uint64 i = 0; i < 2; ++i)
	{
		for (uint64 j = 0; j < 10; ++j)
		{
			std::cout << DoubleMatrixD(i, j) << ' ';
		}

		std::cout << '\n';
	}
}


void TestMatrix_CopyOnWrite()
{

	mdkMatrix<double> DoubleMatrixA;

	DoubleMatrixA.SetSize(2, 4);

	DoubleMatrixA = { 1, 2, 3, 4,
		5, 6, 7, 8 };

	std::cout << "DoubleMatrixA" << '\n';

	for (uint64 i = 0; i < 2; ++i)
	{
		for (uint64 j = 0; j < 4; ++j)
		{
			std::cout << DoubleMatrixA(i, j) << ' ';
		}

		std::cout << '\n';
	}


	mdkMatrix<double> DoubleMatrixB;

	DoubleMatrixB = DoubleMatrixA;

	std::cout << "DoubleMatrixB" << '\n';

	for (uint64 i = 0; i < 2; ++i)
	{
		for (uint64 j = 0; j < 4; ++j)
		{
			std::cout << DoubleMatrixB(i, j) << ' ';
		}

		std::cout << '\n';
	}

	DoubleMatrixB += 10;

	std::cout << "DoubleMatrixB +10 = " << '\n';

	for (uint64 i = 0; i < 2; ++i)
	{
		for (uint64 j = 0; j < 4; ++j)
		{
			std::cout << DoubleMatrixB(i, j) << ' ';
		}

		std::cout << '\n';
	}

	std::cout << "DoubleMatrixA = " << '\n';

	for (uint64 i = 0; i < 2; ++i)
	{
		for (uint64 j = 0; j < 4; ++j)
		{
			std::cout << DoubleMatrixA(i, j) << ' ';
		}

		std::cout << '\n';
	}
}


void TestMatrix_ElementOperation()
{
	mdkMatrix<double> DoubleMatrixA;

	DoubleMatrixA.SetSize(2, 4);

	DoubleMatrixA = { 1, 2, 3, 4,
		5, 6, 7, 8 };

	std::cout << "DoubleMatrixA" << '\n';

	for (uint64 i = 0; i < 2; ++i)
	{
		for (uint64 j = 0; j < 4; ++j)
		{
			std::cout << DoubleMatrixA(i, j) << ' ';
		}

		std::cout << '\n';
	}

	auto DoubleMatrixB = DoubleMatrixA.ElementOperation(std::string("^"), 10.0);

	std::cout << "DoubleMatrixB = DoubleMatrixA ^ 10 " << '\n';

	for (uint64 i = 0; i < 2; ++i)
	{
		for (uint64 j = 0; j < 4; ++j)
		{
			std::cout << DoubleMatrixB(i, j) << ' ';
		}

		std::cout << '\n';
	}


	auto DoubleMatrixC = DoubleMatrixA.ElementOperation(std::string("sqrt"));

	std::cout << "DoubleMatrixC = sqrt(DoubleMatrixA) " << '\n';

	for (uint64 i = 0; i < 2; ++i)
	{
		for (uint64 j = 0; j < 4; ++j)
		{
			std::cout << DoubleMatrixC(i, j) << ' ';
		}

		std::cout << '\n';
	}
}


void TestMatrix_Transpose()
{
	mdkMatrix<double> DoubleMatrixA;

	DoubleMatrixA.SetSize(2, 4);

	DoubleMatrixA = { 1, 2, 3, 4,
		              5, 6, 7, 8 };

	std::cout << "DoubleMatrixA" << '\n';

	for (uint64 i = 0; i < 2; ++i)
	{
		for (uint64 j = 0; j < 4; ++j)
		{
			std::cout << DoubleMatrixA(i, j) << ' ';
		}

		std::cout << '\n';
	}

	auto DoubleMatrixB = DoubleMatrixA.GetTranspose();

	std::cout << "DoubleMatrixB = transpose(DoubleMatrixA)" << '\n';

	for (uint64 i = 0; i < 4; ++i)
	{
		for (uint64 j = 0; j < 2; ++j)
		{
			std::cout << DoubleMatrixB(i, j) << ' ';
		}

		std::cout << '\n';
	}
}


void TestMatrix_Sum_Mean_Max_Min()
{
	mdkMatrix<double> DoubleMatrixA;

	DoubleMatrixA.SetSize(2, 4);

	DoubleMatrixA = { 1, 2, 3, 4,
		              5, 6, 7, 8 };

	std::cout << "DoubleMatrixA" << '\n';

	for (uint64 i = 0; i < 2; ++i)
	{
		for (uint64 j = 0; j < 4; ++j)
		{
			std::cout << DoubleMatrixA(i, j) << ' ';
		}

		std::cout << '\n';
	}

	auto value = DoubleMatrixA.Sum();
	std::cout << "value = sum(DoubleMatrixA) = " << value << '\n';

	value = DoubleMatrixA.Mean();
	std::cout << "value = mean(DoubleMatrixA) = " << value << '\n';

	value = DoubleMatrixA.Max();
	std::cout << "value = max(DoubleMatrixA) = " << value << '\n';

	value = DoubleMatrixA.Min();
	std::cout << "value = min(DoubleMatrixA) = " << value << '\n';

	auto RowMatrix = DoubleMatrixA.SumAlongRow();

	std::cout << "DoubleMatrixA.SumAlongRow()" << '\n';
	for (uint64 j = 0; j < 4; ++j)
	{
		std::cout << RowMatrix(0, j) << ' ';
	}
	std::cout << '\n';

	auto ColMatrix = DoubleMatrixA.SumAlongCol();

	std::cout << "DoubleMatrixA.SumAlongCol()" << '\n';
	for (uint64 i = 0; i < 2; ++i)
	{
		std::cout << ColMatrix(i, 0) << '\n';
	}


	RowMatrix = DoubleMatrixA.MeanAlongRow();

	std::cout << "DoubleMatrixA.MeanAlongRow()" << '\n';
	for (uint64 j = 0; j < 4; ++j)
	{
		std::cout << RowMatrix(0, j) << ' ';
	}
	std::cout << '\n';

	ColMatrix = DoubleMatrixA.MeanAlongCol();

	std::cout << "DoubleMatrixA.MeanAlongCol()" << '\n';
	for (uint64 i = 0; i < 2; ++i)
	{
		std::cout << ColMatrix(i, 0) << '\n';
	}


	RowMatrix = DoubleMatrixA.MaxAlongRow();

	std::cout << "DoubleMatrixA.MaxAlongRow()" << '\n';
	for (uint64 j = 0; j < 4; ++j)
	{
		std::cout << RowMatrix(0, j) << ' ';
	}
	std::cout << '\n';

	ColMatrix = DoubleMatrixA.MaxAlongCol();

	std::cout << "DoubleMatrixA.MaxAlongCol()" << '\n';
	for (uint64 i = 0; i < 2; ++i)
	{
		std::cout << ColMatrix(i, 0) << '\n';
	}


	RowMatrix = DoubleMatrixA.MinAlongRow();

	std::cout << "DoubleMatrixA.MinAlongRow()" << '\n';
	for (uint64 j = 0; j < 4; ++j)
	{
		std::cout << RowMatrix(0, j) << ' ';
	}
	std::cout << '\n';

	ColMatrix = DoubleMatrixA.MinAlongCol();

	std::cout << "DoubleMatrixA.MinAlongCol()" << '\n';
	for (uint64 i = 0; i < 2; ++i)
	{
		std::cout << ColMatrix(i, 0) << '\n';
	}
}


void TestMatrix_LinearCombine()
{
	std::cout << "TestMatrix_LinearCombine " << '\n';

	mdkMatrix<double> A(3, 4);
	A = {1,  2,  3,  4,
	     5,  6,  7,  8,
	     9, 10, 11, 12};

	std::cout << "A = " << '\n';

	for (uint64 i = 0; i < 3; ++i)
	{
		for (uint64 j = 0; j < 4; ++j)
		{
			std::cout << A(i, j) << ' ';
		}

		std::cout << '\n';
	}

	mdkMatrix<double> B(3, 4);
	B =  1.0 + A;

	std::cout << "B = A+1 " << '\n';

	for (uint64 i = 0; i < 3; ++i)
	{
		for (uint64 j = 0; j < 4; ++j)
		{
			std::cout << B(i, j) << ' ';
		}

		std::cout << '\n';
	}

	mdkMatrix<double> C(3, 4);
	C = B + 1.0;

	std::cout << "C = B+1 " << '\n';

	for (uint64 i = 0; i < 3; ++i)
	{
		for (uint64 j = 0; j < 4; ++j)
		{
			std::cout << C(i, j) << ' ';
		}

		std::cout << '\n';
	}

	mdkMatrix<double> D(3, 4);
	D = C + 1.0;

	std::cout << "D = C+1 " << '\n';

	for (uint64 i = 0; i < 3; ++i)
	{
		for (uint64 j = 0; j < 4; ++j)
		{
			std::cout << D(i, j) << ' ';
		}

		std::cout << '\n';
	}

	auto E = A + B * 2.0 + C * 3.0 + D * 4.0;

	std::cout << "E = A + B*2 + C*3 + D*4 = " << '\n';

	for (uint64 i = 0; i < 3; ++i)
	{
		for (uint64 j = 0; j < 4; ++j)
		{
			std::cout << E(i, j) << ' ';
		}

		std::cout << '\n';
	}

	std::vector<double> AlphaList = { 1, 2, 3, 4 };

	std::vector<mdkMatrix<double>*> MatrixList = { &A, &B, &C, &D };

	auto SumMatrix = mdk::LinearCombine(AlphaList, MatrixList);

	std::cout << "1A+2B+3C+4D = " << '\n';

	for (uint64 i = 0; i < 3; ++i)
	{
		for (uint64 j = 0; j < 4; ++j)
		{
			std::cout << SumMatrix(i, j) << ' ';
		}

		std::cout << '\n';
	}
}

void TestMatrix_Set_Get_Append_Col()
{
	std::cout << "TestMatrix_Set_Get_Append_Col " << '\n';

	mdkMatrix<double> A;

	A.SetSize(3, 0);

	std::vector<double> Col;

	Col = { 1, 2, 3 };
	A.AppendCol(Col);

	Col = { 4, 5, 6 };
	A.AppendCol(Col);

	Col = { 7, 8, 9 };
	A.AppendCol(Col);

	std::cout << "A = " << '\n';

	for (uint64 i = 0; i < 3; ++i)
	{
		for (uint64 j = 0; j < 3; ++j)
		{
			std::cout << A(i, j) << ' ';
		}

		std::cout << '\n';
	}


	Col = { 10, 20, 30 };
	A.SetCol(1, Col);

	Col = { 101, 201, 301 };
	A.SetCol(2, Col);

	std::cout << "A = " << '\n';

	for (uint64 i = 0; i < 3; ++i)
	{
		for (uint64 j = 0; j < 3; ++j)
		{
			std::cout << A(i, j) << ' ';
		}

		std::cout << '\n';
	}


	auto Col_output = A.Col(0);
	std::cout << "A.Col(0)" << '\n';
	for (uint64 i = 0; i < 3; ++i)
	{
		std::cout << Col_output(i) << '\n';
	}

	Col_output = A.Col(1);
	std::cout << "A.Col(1)" << '\n';
	for (uint64 i = 0; i < 3; ++i)
	{
		std::cout << Col_output(i) << '\n';
	}

	Col_output = A.Col(2);
	std::cout << "A.Col(2)" << '\n';
	for (uint64 i = 0; i < 3; ++i)
	{
		std::cout << Col_output(i) << '\n';
	}

	Col_output = A.Col(3);
}


void TestMatrix_Set_Get_Append_Row()
{
	std::cout << "TestMatrix_Set_Get_Append_Row " << '\n';

	mdkMatrix<double> A;

	A.SetSize(0, 3);

	std::vector<double> Row;

	Row = { 1, 2, 3 };
	A.AppendRow(Row);

	Row = { 4, 5, 6 };
	A.AppendRow(Row);

	Row = { 7, 8, 9 };
	A.AppendRow(Row);

	std::cout << "A = " << '\n';

	for (uint64 i = 0; i < 3; ++i)
	{
		for (uint64 j = 0; j < 3; ++j)
		{
			std::cout << A(i, j) << ' ';
		}

		std::cout << '\n';
	}


	Row = { 11, 21, 31 };
	A.SetRow(1, Row);

	Row = { 111, 211, 311 };
	A.SetRow(2, Row);

	std::cout << "A = " << '\n';

	for (uint64 i = 0; i < 3; ++i)
	{
		for (uint64 j = 0; j < 3; ++j)
		{
			std::cout << A(i, j) << ' ';
		}

		std::cout << '\n';
	}


	auto Row_output = A.Row(0);
	std::cout << "A.Row(0)" << '\n';
	for (uint64 i = 0; i < 3; ++i)
	{
		std::cout << Row_output(i) << ' ';
	}
	std::cout << '\n';


	Row_output = A.Row(1);
	std::cout << "A.Row(1)" << '\n';
	for (uint64 i = 0; i < 3; ++i)
	{
		std::cout << Row_output(i) << ' ';
	}
	std::cout << '\n';



	Row_output = A.Row(2);
	std::cout << "A.Row(2)" << '\n';
	for (uint64 i = 0; i < 3; ++i)
	{
		std::cout << Row_output(i) << ' ';
	}
	std::cout << '\n';
}


void TestMatrix_Arma()
{
	std::cout << "TestMatrix_LinearCombine " << '\n';

	mdkMatrix<double> A(3, 3);
	A = { 1, 2, 3,
		  0, 5, 0, 
		  0, 0, 9};

	std::cout << "A = " << '\n';

	for (uint64 i = 0; i < 3; ++i)
	{
		for (uint64 j = 0; j < 3; ++j)
		{
			std::cout << A(i, j) << ' ';
		}

		std::cout << '\n';
	}

	auto invA = A.Inv();
	std::cout << "A.Inv() = " << '\n';

	for (uint64 i = 0; i < 3; ++i)
	{
		for (uint64 j = 0; j < 3; ++j)
		{
			std::cout << invA(i, j) << ' ';
		}

		std::cout << '\n';
	}

	auto AinvA = A*invA;
	std::cout << "A*invA = " << '\n';

	for (uint64 i = 0; i < 3; ++i)
	{
		for (uint64 j = 0; j < 3; ++j)
		{
			std::cout << AinvA(i, j) << ' ';
		}

		std::cout << '\n';
	}

	auto ASVD = A.SVD();

	std::cout << "ASVD.U = " << '\n';

	for (uint64 i = 0; i < 3; ++i)
	{
		for (uint64 j = 0; j < 3; ++j)
		{
			std::cout << ASVD.U(i, j) << ' ';
		}

		std::cout << '\n';
	}

	std::cout << "ASVD.S = " << '\n';

	for (uint64 i = 0; i < 3; ++i)
	{
		for (uint64 j = 0; j < 3; ++j)
		{
			std::cout << ASVD.S(i, j) << ' ';
		}

		std::cout << '\n';
	}


	std::cout << "ASVD.V = " << '\n';

	for (uint64 i = 0; i < 3; ++i)
	{
		for (uint64 j = 0; j < 3; ++j)
		{
			std::cout << ASVD.V(i, j) << ' ';
		}

		std::cout << '\n';
	}

	auto tempA = ASVD.U*ASVD.S*ASVD.V.GetTranspose();

	std::cout << "ASVD.U*ASVD.S*ASVD.V' = " << '\n';

	for (uint64 i = 0; i < 3; ++i)
	{
		for (uint64 j = 0; j < 3; ++j)
		{
			std::cout << tempA(i, j) << ' ';
		}

		std::cout << '\n';
	}
}

void TestMatrix_CopyOnWrite_And_Operator()
{
	std::cout << "TestMatrix_LinearCombine " << '\n';

	mdkMatrix<double> A(3, 3);
	A = { 1, 2, 3,
		0, 5, 0,
		0, 0, 9 };

	std::cout << "A = " << '\n';

	for (uint64 i = 0; i < 3; ++i)
	{
		for (uint64 j = 0; j < 3; ++j)
		{
			std::cout << A(i, j) << ' ';
		}

		std::cout << '\n';
	}
}

}//end of namespace


#endif