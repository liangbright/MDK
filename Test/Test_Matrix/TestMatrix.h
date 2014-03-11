#ifndef __Test_h
#define __Test_h

#include <chrono>
#include <ctime>
#include <iomanip>
#include <initializer_list>
#include <functional>

#include "mdkMatrix.h"
#include "mdkShadowMatrix.h"
#include "mdkLinearAlgebra.h"

using namespace mdk;

template<typename T>
void DisplayMatrix(const mdkMatrix<T>& Matrix, int precision = 0)
{
    for (uint64 i = 0; i < Matrix.GetRowNumber(); ++i)
    {
        for (uint64 j = 0; j < Matrix.GetColNumber(); ++j)
        {
            //std::cout << std::fixed << std::setprecision(precision) << Matrix(i, j) << ' ';

            std::cout << std::setw(8) << std::fixed << std::setprecision(precision) << Matrix(i, j);
        }
        std::cout << '\n';
    }
}


void Test_ConstPointer_function(const double* p)
{
	//p[1] = 2;
}

void Test_ConstPointer()
{
	double a[3] = { 1, 2, 3 };
	Test_ConstPointer_function(a);
}

// use resize and operator[]
// do not use insert or emplace : meaningless
void Test_std_vector()
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


void Test_Constructor()
{
    mdkMatrix<double> A(2, 2);

    A = { 2, 0, 
          0, 4};

    DisplayMatrix(A);

 
    mdkMatrix<double> A1(A);

    mdkMatrix<double> B(A.GetElementDataRawPointer(), 2, 2);
    
    DisplayMatrix(B);

    mdkMatrix<double> C;

    C.ForceShallowCopy(A);

    A.Copy(A);

    A.Copy(C);

    C.Copy(A);
    //-----------------

    // move constructor is used 2 times
    // one for temp (A*A), the other for D(temp)
    mdkMatrix<double> D(A*A);


    // copy constructor, D will be temporary ?
    mdkMatrix<double> D1 = A*A;

    mdkMatrix<double> D2;

    // move constructor is used
    // move "=" is used
    //D2 is not temporary
    D2 = (A+A).ElementMultiply(A);

    //D3 is temporary
    auto D3 = D2;

    mdkMatrix<double> D4;

    // D4 is not temporary
    D4 = D2;

}

void Test_MoveConstructor()
{
    mdkMatrix<double> A(2, 2);

    auto ptrA = A.GetElementDataRawPointer();

    A = { 1, 2,
        3, 4 };

    std::cout << "A" << '\n';
    DisplayMatrix(A);

    auto Result = A.SVD();

    mdkMatrix<double> B;

    B = std::move(A);

    auto ptrB = B.GetElementDataRawPointer();

    auto ptrA_B = A.GetElementDataRawPointer();

    std::cout << "B" << '\n';
    DisplayMatrix(B);
   
    mdkMatrix<double> C = std::move(A);

    auto ptrC = C.GetElementDataRawPointer();

    auto ptrA_C = A.GetElementDataRawPointer();

    std::cout << "C" << '\n';
    DisplayMatrix(C);

    mdkMatrix<double> D1;

    // (B*B) is created from move constructor
    // D is assigned by move "="
    D1 = std::move(B*B);

    mdkMatrix<double> D2;

    // move "=" is used
    D2 = B*B;
    //-------------
}


void Test_Matrix_Operator()
{

    double temp = {};

    mdkMatrix<double> A(2, 4);

    A = { 1, 2, 3, 4,
          5, 6, 7, 8 };

    std::cout << "A" << '\n';

    DisplayMatrix(A);

    auto a = A(0, 0);

    mdkMatrix<double> b = A({ 0 }, ALL);

    // good: this can not be compiled
    //auto b1 = A(0, ALL);

    mdkMatrix<double> B;
    
    B = A({ 0 }, ALL);

    // attention !!!: this is compiled: {} is {0}
    mdkMatrix<double> B1;
    
    B1 = A({ 1 }, {});

    // this is compiled as (uint64, uint64)
    mdkMatrix<double> B2;
    
    B2 = A({ 0 }, { 0 });

    A({ 0 }, { 0 }) = 0;

    mdkMatrix<double> C1;
        
    C1 = A({ 0 });

    mdkMatrix<double> C2 = A({ 0, 1 });

    mdkMatrix<double> C3 = A({ 0, 1 }, { 0 });

    mdkMatrix<double> C4 = A({ 0 }, { 0 })*A({ 0, 1 }, { 0 });

    // good: this can not be compiled
    //auto C5 = A({ 0, 1 }, {});

    std::cout << "A" << '\n';

    DisplayMatrix(A);

    std::cout << "C4" << '\n';

    DisplayMatrix(C4);

    // operator []

    mdkMatrix<double> D;
        
    D = A[0];

    mdkMatrix<double> D1;
        
    D1 = A[{ 0 }];

    mdkMatrix<double> D2 = A({ 0, 1 });

    A({ 0, 1 }) = 1;

    std::cout << "A" << '\n';

    DisplayMatrix(A);
}


void Test_Mutiplication()
{

	mdkMatrix<double> A;

	A.Resize(2, 4);

	A = { 1, 2, 3, 4, 
		              5, 6, 7, 8 };
	
	std::cout << "A" << '\n';

    DisplayMatrix(A);

	mdkMatrix<double> B;

    B.Resize(4, 2);

	B = { 1, 2, 
		  3, 4, 
		  5, 6, 
		  7, 8 };

	std::cout << "B" << '\n';

    DisplayMatrix(B);

    mdkMatrix<double> C = A*B;

	std::cout << "C = A * B" << '\n';

    DisplayMatrix(C);

	A *= 1.25;

	std::cout << "A * 1.25 =" << '\n';

    DisplayMatrix(A);


	mdkMatrix<double> D(2, 10);
	D = {  1,  2,  3,  4,  5,  6,  7,  8,  9, 10,
		  11, 12, 13, 14, 15, 16, 17, 18, 19, 20 };

	std::cout << "D =" << '\n';
    DisplayMatrix(D);
}


void Test_Share()
{
    mdkMatrix<double> A(2, 2);

    A = { 1, 2,
          3, 4 };

    std::cout << "A =" << '\n';
    DisplayMatrix(A);

    mdkMatrix<double> B;

    B.ForceShallowCopy(A);

    B(1, 1) = 0;

    std::cout << "B =" << '\n';
    DisplayMatrix(B);
}


void Test_Mutiplication_Speed()
{
    mdkMatrix<double> A(512, 512);

    mdkMatrix<double> B(512, 512);

    mdkMatrix<double> C(512, 512);

    auto t0 = std::chrono::system_clock::now();

    for (uint64 i = 0; i < 1000; ++i)
    {
       // C = 1.0*(A*C) + 2.0*(A*C) + 3.0*(A*C) + 4.0*(A*C) + 5.0*(A*C) + 6.0*(A*C);
        //C = 1.0*A*C + 2.0*A*C + 3.0*A*C + 4.0*A*C + 5.0*A*C + 6.0*A*C;

        C = (A*C) + (A*C) + (A*C) + (A*C) + (A*C) + (A*C);
    }

    auto t1 = std::chrono::system_clock::now();
    std::chrono::duration<double> Temp_time = t1 - t0;
    std::cout << "C = A*C time = " << Temp_time.count() << '\n';


    arma::Mat<double> Am(A.GetElementDataRawPointer(), arma::uword(A.GetRowNumber()), arma::uword(A.GetColNumber()), false);
    arma::Mat<double> Bm(B.GetElementDataRawPointer(), arma::uword(B.GetRowNumber()), arma::uword(B.GetColNumber()), false);
    arma::Mat<double> Cm(C.GetElementDataRawPointer(), arma::uword(C.GetRowNumber()), arma::uword(C.GetColNumber()), false);

    t0 = std::chrono::system_clock::now();

    for (uint64 i = 0; i < 1000; ++i)
    {
        //Cm = 1.0*(Am*Cm) + 2.0*(Am*Cm) + 3.0*(Am*Cm) + 4.0*(Am*Cm) + 5.0*(Am*Cm) + 6.0*(Am*Cm);
       // Cm = 1.0*Am*Cm + 2.0*Am*Cm + 3.0*Am*Cm + 4.0*Am*Cm + 5.0*Am*Cm + 6.0*Am*Cm;

        Cm = Am*Cm + Am*Cm + Am*Cm + Am*Cm + Am*Cm + Am*Cm;
    }

    t1 = std::chrono::system_clock::now();
    std::chrono::duration<double> Temp_time_arma = t1 - t0;
    std::cout << "Cm = Am*Cm time = " << Temp_time_arma.count() << '\n';


    std::system("pause");
}


void Test_ElementOperation()
{
	mdkMatrix<double> A;

    A.Resize(2, 4);

	A = { 1, 2, 3, 4,
		5, 6, 7, 8 };

	std::cout << "A" << '\n';

	for (uint64 i = 0; i < 2; ++i)
	{
		for (uint64 j = 0; j < 4; ++j)
		{
			std::cout << A(i, j) << ' ';
		}

		std::cout << '\n';
	}

    auto B = A.ElementOperation("sqrt"); 

	std::cout << "B = A ^ 10 " << '\n';

	for (uint64 i = 0; i < 2; ++i)
	{
		for (uint64 j = 0; j < 4; ++j)
		{
			std::cout << B(i, j) << ' ';
		}

		std::cout << '\n';
	}


    auto C = A.ElementOperation("^", 10.0);

	//td::cout << "C = sqrt(A) " << '\n';

}


void Test_Transpose()
{
	mdkMatrix<double> A;

    A.Resize(2, 4);

	A = { 1, 2, 3, 4,
		  5, 6, 7, 8 };

	std::cout << "A" << '\n';

	for (uint64 i = 0; i < 2; ++i)
	{
		for (uint64 j = 0; j < 4; ++j)
		{
			std::cout << A(i, j) << ' ';
		}

		std::cout << '\n';
	}

	auto B = A.Transpose();

	std::cout << "B = transpose(A)" << '\n';

	for (uint64 i = 0; i < 4; ++i)
	{
		for (uint64 j = 0; j < 2; ++j)
		{
			std::cout << B(i, j) << ' ';
		}

		std::cout << '\n';
	}
}


void Test_Sum_Mean_Max_Min()
{
	mdkMatrix<double> A;

    A.Resize(2, 4);

	A = { 1, 2, 3, 4,
		              5, 6, 7, 8 };

	std::cout << "A" << '\n';

	for (uint64 i = 0; i < 2; ++i)
	{
		for (uint64 j = 0; j < 4; ++j)
		{
			std::cout << A(i, j) << ' ';
		}

		std::cout << '\n';
	}

	auto value = A.Sum();
	std::cout << "value = sum(A) = " << value << '\n';

	value = A.Mean();
	std::cout << "value = mean(A) = " << value << '\n';

	value = A.Max();
	std::cout << "value = max(A) = " << value << '\n';

	value = A.Min();
	std::cout << "value = min(A) = " << value << '\n';

	auto RowMatrix = A.SumToRow();

	std::cout << "A.SumToRow()" << '\n';
	for (uint64 j = 0; j < 4; ++j)
	{
		std::cout << RowMatrix(0, j) << ' ';
	}
	std::cout << '\n';

	auto ColMatrix = A.SumToCol();

	std::cout << "A.SumToCol()" << '\n';
	for (uint64 i = 0; i < 2; ++i)
	{
		std::cout << ColMatrix(i, 0) << '\n';
	}


	RowMatrix = A.MeanToRow();

	std::cout << "A.MeanToRow()" << '\n';
	for (uint64 j = 0; j < 4; ++j)
	{
		std::cout << RowMatrix(0, j) << ' ';
	}
	std::cout << '\n';

	ColMatrix = A.MeanToCol();

	std::cout << "A.MeanToCol()" << '\n';
	for (uint64 i = 0; i < 2; ++i)
	{
		std::cout << ColMatrix(i, 0) << '\n';
	}


	RowMatrix = A.MaxToRow();

	std::cout << "A.MaxToRow()" << '\n';
	for (uint64 j = 0; j < 4; ++j)
	{
		std::cout << RowMatrix(0, j) << ' ';
	}
	std::cout << '\n';

	ColMatrix = A.MaxToCol();

	std::cout << "A.MaxToCol()" << '\n';
	for (uint64 i = 0; i < 2; ++i)
	{
		std::cout << ColMatrix(i, 0) << '\n';
	}


	RowMatrix = A.MinToRow();

	std::cout << "A.MinToRow()" << '\n';
	for (uint64 j = 0; j < 4; ++j)
	{
		std::cout << RowMatrix(0, j) << ' ';
	}
	std::cout << '\n';

	ColMatrix = A.MinToCol();

	std::cout << "A.MinToCol()" << '\n';
	for (uint64 i = 0; i < 2; ++i)
	{
		std::cout << ColMatrix(i, 0) << '\n';
	}
}


void Test_LinearCombine()
{
    /*
	std::cout << "Test_LinearCombine " << '\n';

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
    */
}

void Test_Set_Get_Append_Delete_Insert_Col()
{
	std::cout << "Test_Set_Get_Append_Delete_Insert_Col " << '\n';

	mdkMatrix<double> A;

	std::vector<double> Col;

	Col = { 1, 2, 3 };
	A.AppendCol(Col);

    std::cout << "A = " << '\n';
    DisplayMatrix(A);

	Col = { 4, 5, 6 };
	A.AppendCol(Col);

    std::cout << "A = " << '\n';
    DisplayMatrix(A);

	Col = { 7, 8, 9 };
	A.AppendCol(Col);

	std::cout << "A = " << '\n';
    DisplayMatrix(A);


	Col = { 10, 20, 30 };
	A.SetCol(1, Col);

    std::cout << "A = " << '\n';
    DisplayMatrix(A);

	Col = { 101, 201, 301 };
	A.SetCol(2, Col);

	std::cout << "A = " << '\n';
    DisplayMatrix(A);


    Col = { 11, 21, 31 };
    A.InsertCol(2, Col);

    std::cout << "A = " << '\n';
    DisplayMatrix(A);


    Col = { 110, 210, 310 };
    A.InsertCol(2, Col);

    std::cout << "A = " << '\n';
    DisplayMatrix(A);

    Col = { 4, 5, 6 };
    A.InsertCol(0, Col);

    std::cout << "A = " << '\n';
    DisplayMatrix(A);

    A.DeleteCol(0);
    std::cout << "A = " << '\n';
    DisplayMatrix(A);

    A.DeleteCol(1);
    std::cout << "A = " << '\n';
    DisplayMatrix(A);

    A.DeleteCol({0, 1});
    std::cout << "A = " << '\n';
    DisplayMatrix(A);

    A.DeleteCol(1);
    std::cout << "A = " << '\n';
    DisplayMatrix(A);
}


void Test_Set_Get_Append_Delete_Insert_Row()
{
    std::cout << "Test_Set_Get_Append_Delete_Insert_Row " << '\n';

    mdkMatrix<double> A;

    std::vector<double> Row;

    Row = { 1, 2, 3 };
    A.AppendRow(Row);

    std::cout << "A = " << '\n';
    DisplayMatrix(A);

    Row = { 4, 5, 6 };
    A.AppendRow(Row);

    std::cout << "A = " << '\n';
    DisplayMatrix(A);

    Row = { 7, 8, 9 };
    A.AppendRow(Row);

    std::cout << "A = " << '\n';
    DisplayMatrix(A);


    Row = { 10, 20, 30 };
    A.SetRow(1, Row);

    std::cout << "A = " << '\n';
    DisplayMatrix(A);

    Row = { 101, 201, 301 };
    A.SetRow(2, Row);

    std::cout << "A = " << '\n';
    DisplayMatrix(A);


    Row = { 11, 21, 31 };
    A.InsertRow(2, Row);

    std::cout << "A = " << '\n';
    DisplayMatrix(A);


    Row = { 110, 210, 310 };
    A.InsertRow(2, Row);

    std::cout << "A = " << '\n';
    DisplayMatrix(A);

    Row = { 4, 5, 6 };
    A.InsertRow(0, Row);

    std::cout << "A = " << '\n';
    DisplayMatrix(A);

    A.DeleteRow(0);
    std::cout << "A = " << '\n';
    DisplayMatrix(A);

    A.DeleteRow(1);
    std::cout << "A = " << '\n';
    DisplayMatrix(A);

    A.DeleteRow({ 0, 1 });
    std::cout << "A = " << '\n';
    DisplayMatrix(A);

    A.DeleteRow(1);
    std::cout << "A = " << '\n';
    DisplayMatrix(A);
}


void Test_GetSubMatrix()
{
    mdkMatrix<double> A(5, 6);

    A = { 1,     2,     3,     4,     5,     6,
          10,    20,    30,    40,    50,    60, 
          100,   200,   300,   40,    500,   600, 
          1000,  2000,  3000,  4000,  5000,  6000,
          10000, 20000, 30000, 40000, 50000, 60000, };

    std::cout << "A = " << '\n';
    DisplayMatrix(A);

    auto subA1 = A.GetSubMatrix(0, 3, 2, 5);
    std::cout << "subA1 = " << '\n';
    DisplayMatrix(subA1);

    auto subA2 = A.GetSubMatrix({ 1 }, {0, 1});
    std::cout << "subA2 = " << '\n';
    DisplayMatrix(subA2);

    auto subA3 = A.GetSubMatrix({ 1, 2, 3}, { 0, 1 , 2});
    std::cout << "subA3 = " << '\n';
    DisplayMatrix(subA3);

    auto subAa = A.GetSubMatrix(ALL, { 0, 1, 2 });
    std::cout << "subAa = " << '\n';
    DisplayMatrix(subAa);

    auto subAb = A.GetSubMatrix({ 0, 1, 2 }, ALL);
    std::cout << "subAb = " << '\n';
    DisplayMatrix(subAb);

    mdkMatrix<double> B1(3, 5);

    A.GetSubMatrix(B1, { 0, 1, 2 }, { 0, 1, 2, 3, 4 });

    std::cout << "B1 = " << '\n';
    DisplayMatrix(B1);

    mdkMatrix<double> B2(5, 3);

    A.GetSubMatrix(B2, {0, 1, 2, 3, 4}, { 0, 1, 2 });

    std::cout << "B2 = " << '\n';
    DisplayMatrix(B2);
}


void Test_SubMatrix()
{
    mdkMatrix<double> A(5, 6);

    A = { 1,     2,     3,     4,     5,     6,
          10,    20,    30,    40,    50,    60, 
          100,   200,   300,   40,    500,   600, 
          1000,  2000,  3000,  4000,  5000,  6000,
          10000, 20000, 30000, 40000, 50000, 60000, };

    std::cout << "A = " << '\n';
    DisplayMatrix(A);

    mdkMatrix<double> subA1 = A.GetSubMatrix(0, 3, 2, 5);
    std::cout << "subA1 = " << '\n';
    DisplayMatrix(subA1);

    mdkMatrix<double> subA2 = A.at({ 1 }, { 0, 1 });
    std::cout << "subA2 = " << '\n';
    DisplayMatrix(subA2);

    mdkMatrix<double> subA3 = A.at({ 1, 2, 3 }, { 0, 1, 2 });
    std::cout << "subA3 = " << '\n';
    DisplayMatrix(subA3);

    mdkMatrix<double> subAa = A.at(ALL, { 0, 1, 2, 1, 0});
    std::cout << "subAa = " << '\n';
    DisplayMatrix(subAa);

    mdkMatrix<double> subAb = A.at({ 0, 1, 2, 1, 0 }, ALL);
    std::cout << "subAb = " << '\n';
    DisplayMatrix(subAb);

 }


void Test_ShadowMatrix()
{

    mdkMatrix<double> A(3, 3);

    A = { 1, 2, 3,
          4, 5, 6,
          7, 8, 9 };

    mdkMatrix<double> B(4, 4);

    B = { 1, 2, 3, 0,
          4, 5, 6, 0,
          7, 8, 9, 0,
          0, 0, 0, 0};

    mdkMatrix<double> C;

    C = 1.0 + A*A + A * (10.0 + A*B({0,1,2}, {0,1,2}));

    std::cout << "C = " << '\n';
    DisplayMatrix(C);

    mdkMatrix<double> D = A(ALL);

    std::system("pause");
}


void Test_Arma()
{
	std::cout << "Test_Arma " << '\n';

	mdkMatrix<double> A(3, 3);
	A = { 1, 2, 3,
		  0, 5, 0, 
		  0, 0, 9};

	std::cout << "A = " << '\n';
    DisplayMatrix(A);

    mdkMatrix<double> invA = A.Inv();
	std::cout << "A.Inv() = " << '\n';
    DisplayMatrix(invA, 3);

    mdkMatrix<double> AinvA = A*invA;
	std::cout << "A*invA = " << '\n';
    DisplayMatrix(AinvA, 3);

	auto ASVD = A.SVD();

	std::cout << "ASVD.U = " << '\n';
    DisplayMatrix(ASVD.U, 3);

	std::cout << "ASVD.S = " << '\n';
    DisplayMatrix(ASVD.S, 3);

	std::cout << "ASVD.V = " << '\n';
    DisplayMatrix(ASVD.V, 3);

    mdkMatrix<double> tempA = ASVD.U * ASVD.S * ASVD.V.Transpose();

	std::cout << "ASVD.U*ASVD.S*ASVD.V' = " << '\n';
    DisplayMatrix(tempA, 3);


    std::system("pause");
}


void Test_Matrix_ChangeSize()
{
    mdkMatrix<double> A(6, 9);

    A = { 1, 2, 3, 4, 5, 6, 7, 8, 9,
          2, 5, 0, 2, 1, 2, 1, 2, 1,
          3, 0, 9, 3, 5, 3, 5, 3, 5,
          4, 0, 8, 4, 5, 4, 5, 4, 5,
          5, 0, 7, 5, 5, 5, 5, 5, 5,
          6, 0, 6, 6, 5, 6, 5, 6, 5 };

    std::cout << "A = " << '\n';

    DisplayMatrix(A);

    A.DeleteCol({0});

    std::cout << "A.DeleteCol({0}) = " << '\n';

    DisplayMatrix(A);


    A.DeleteCol({1, 7});

    std::cout << "A.DeleteCol({1, 7}) = " << '\n';

    DisplayMatrix(A);


    A.DeleteRow({1, 5});

    std::cout << "A.DeleteRow({1, 5}) = " << '\n';

    DisplayMatrix(A);
}


void Test_ShadowMatrix_SubMatrix()
{
    mdkMatrix<double> A(6, 9);

    A = { 1, 2, 3, 4, 5, 6, 7, 8, 9,
        2, 5, 0, 2, 1, 2, 1, 2, 1,
        3, 0, 9, 3, 5, 3, 5, 3, 5,
        4, 0, 8, 4, 5, 4, 5, 4, 5,
        5, 0, 7, 5, 5, 5, 5, 5, 5,
        6, 0, 6, 6, 5, 6, 5, 6, 5 };

    std::cout << "A = " << '\n';

    DisplayMatrix(A);

    auto A1 = A({ 0 }, { 1 });

    mdkMatrix<double> subA1;
    
    subA1 = A.at({ 0 }, { 1 });

    std::cout << "subA1 = A.SubMatrix({ 0 }, { 1 }) " << '\n';

    DisplayMatrix(subA1);

    mdkMatrix<double> subA2 = A.at({ 1, 0 }, { 2, 1 });

    std::cout << "subA2 = A.SubMatrix({ 1, 0 }, { 2, 1 }) " << '\n';

    DisplayMatrix(subA2);

    // good this can not be compiled
    mdkMatrix<double> subA3;    
    //subA3 = A.SubMatrix({ 1, 0 }, {});
    //subA3 = A.SubMatrix({}, { 1, 0 });

    mdkMatrix<double> subA4 = A.at({ 1, 0 }, ALL);

    std::cout << "subA4 = A.SubMatrix({ 1, 0 }, ALL) " << '\n';

    DisplayMatrix(subA4);
}


void Test_ShadowMatrix_Col_Row()
{
    mdkMatrix<double> A(6, 9);

    A = { 1, 2, 3, 4, 5, 6, 7, 8.001, 9.00000001,
        2, 5, 0, 2, 1, 2, 1, 2, 1,
        3, 0, 9, 3, 5, 3, 5, 3, 5,
        4, 0, 8, 4, 5, 4, 5, 4, 5,
        5, 0, 7, 5, 5, 5, 5, 5, 5,
        6, 0, 6, 6, 5, 6, 5, 6, 5 };

    std::cout << "A = " << '\n';

    DisplayMatrix(A);

    mdkMatrix<double> subA1 = A.Col({0, 1});

    std::cout << "subA1 = A.Col({0, 1}) " << '\n';

    DisplayMatrix(subA1);

    mdkMatrix<double> subA2 = A.Row({ 0, 1 });

    std::cout << "subA2 = A.Row({0, 1}) " << '\n';

    DisplayMatrix(subA2);
}

void Test_ShadowMatrix_Operator()
{
    mdkMatrix<double> A(5, 5);
    A = { 1, 2, 3, 4, 5,
          0, 5, 0, 2, 1,
          0, 0, 9, 1, 0,
          0, 1, 0, 1, 0,
          1, 0, 1, 0, 1};

    std::cout << "A = " << '\n';
    DisplayMatrix(A);

    mdkMatrix<double> subA1 = A({ 0, 1 }, ALL);

    mdkMatrix<double> subA2 = subA1;

    std::cout << "subA2 = " << '\n';
    DisplayMatrix(subA2);

    mdkMatrix<double> subA3 = A({ 0, 1 }, ALL) * 10.0;

    std::cout << "subA3 = " << '\n';
    DisplayMatrix(subA3);

    mdkMatrix<double> subA4 = A(ALL, { 0, 1 });

    std::cout << "sub4 = " << '\n';
    DisplayMatrix(subA4);

    std::vector<double> v = {};

    mdkMatrix<double> subA5 = A({0, 1}, {0, 1, 2}) * A({0, 1, 2}, {0, 1});

    std::cout << "subA5 = " << '\n';
    DisplayMatrix(subA5);

    // good:
    // before use ALL symbol
    // compiler internal error  C1001
    // mdkMatrix<double> subA5a = A({ 0, 1 }, { 0, 1, 2 }) * A({}, { 0, 1 });
    //
    // after use ALL, then still error 

    mdkMatrix<double> subA6 = A.Row({0, 1}) * A.Col({0, 1});

    std::cout << "subA6 = " << '\n';
    DisplayMatrix(subA6);


    typedef double  aaa;

    typedef int  bbb;

    if (std::is_same<aaa, bbb>::value == true)
    {

    }
}


void Test_GlueMatrix()
{
    mdkMatrix<double> A(3, 3);
    A = { 1, 2, 0,
          1, 2, 0,
          3, 3, 3};


    mdkMatrix<double> B(2, 3);
    B = { 1, 2, 3,
          1, 2, 3};


    mdkMatrix<double> C(2, 3);
    C = { 0, 2, 1,
          0, 2, 1};

    mdkMatrix<double> C1(2, 3);
    C1 = { 1, 0, 1,
           1, 0, 1 };

    mdkMatrix<double> D = 1.0*A({ 0, 1 }, ALL) + 2.0*B - 3.0*C + 4.0*C1;

    std::cout << "D= " << '\n';

    DisplayMatrix(D);

    auto E = MatrixAdd(mdkMatrix<double>(A.Row({ 0, 1 })), C);


    mdkMatrix<double> All = A(ALL, { 0, 1 })*(B + 1.0 + C - 3.0*C + 4.0*C1);

    std::cout << "All= " << '\n';

    DisplayMatrix(All);

    (B + C).~mdkGlueMatrixForLinearCombination();

    //auto a = (A*A).Mean();

    A({ 0, 1 }) += A;
}

class TestClass
{
public:
    TestClass()
    {
        std::cout << "hello ~" << '\n';

        std::cout << this << '\n';
    }

    ~TestClass()
    {
        std::cout << "bye ~" << '\n';
        std::cout << this << '\n';
    }

    TestClass(const TestClass&) = delete;
    void operator=(const TestClass&) = delete;
};

void Test_Destructor()
{
//    auto Data = std::shared_ptr<std::vector<TestClass>>(new std::vector<TestClass>(1));

    auto Data = std::shared_ptr<TestClass>(new TestClass);


    auto Ptr = Data.get();

    {
      //  mdkMatrix<TestClass> A(Data);
    }

    std::cout << "All= " << '\n';
}


void Test_GlueMatrix_Speed1()
{

    mdkMatrix<double> A(512, 512);

    mdkMatrix<double> B(512, 512);

    mdkMatrix<double> C(512, 512);

    auto t0 = std::time(0);

    mdkMatrix<double> D(512, 512);
    
    for (uint64 i = 0; i < 10000; ++i)
    {
        D += 1.0*A + 2.0*B - 3.0 * C;
    }

    std::cout << "D(0,0) " << D(0,0) << '\n';

    auto t1 = std::time(0);

    std::cout << "time " << t1 - t0 << '\n';

    std::system("pause");

    // time : 23s GlueMatrix
    // time : 45s No GlueMatrix
}

void Test_GlueMatrix_Speed2()
{

    mdkMatrix<double> A(512, 512);

    A.Fill(1.0);

    mdkMatrix<double> B(512, 512);

    B.Fill(2.0);

    mdkMatrix<double> C(512, 512);

    C.Fill(3.0);

    mdkMatrix<double> C2(512, 512);

    C2.Fill(3.0);
   
    mdkMatrix<double> D(512, 512);

    D.Fill(0.0);

    //--------------

    D += A + B;

    //----------

    D = D + 1.0*A + 2.0*B + 3.0*C + 4.0*C2 + 5.0*D + 6.0*D;

    //-------------------------------------------------------------------------------------------
    double ElementList_Coef[] = { 1, 2, 3, 4, 5, 6 };

    double* MatrixElementDataRawPtrList[] = { A.GetElementDataRawPointer(),
                                              B.GetElementDataRawPointer(),
                                              C.GetElementDataRawPointer(),
                                              C2.GetElementDataRawPointer(),
                                              D.GetElementDataRawPointer(),
                                              D.GetElementDataRawPointer() };

    auto ElementNumber = 512 * 512;

    auto tempRawPointer = D.GetElementDataRawPointer();

    auto t0 = std::chrono::system_clock::now();

   // #pragma loop(hint_parallel(8))

    for (uint64 i = 0; i < 10000; ++i)
    {
        for (uint64 LinearIndex = 0; LinearIndex < ElementNumber; ++LinearIndex)
        {
            tempRawPointer[LinearIndex] += 0.0
                + ElementList_Coef[0] * MatrixElementDataRawPtrList[0][LinearIndex]
                + ElementList_Coef[1] * MatrixElementDataRawPtrList[1][LinearIndex]
                + ElementList_Coef[2] * MatrixElementDataRawPtrList[2][LinearIndex]
                + ElementList_Coef[3] * MatrixElementDataRawPtrList[3][LinearIndex]
                + ElementList_Coef[4] * MatrixElementDataRawPtrList[4][LinearIndex]
                + ElementList_Coef[5] * MatrixElementDataRawPtrList[5][LinearIndex];
        }
    }
    
    auto t1 = std::chrono::system_clock::now();

    std::chrono::duration<double> raw_time = t1 - t0;
    std::cout << "raw_time = " << raw_time.count() << '\n';

    //---------------------------------------------------------------------------------------------

    t0 = std::chrono::system_clock::now();

    for (uint64 i = 0; i < 10000; ++i)
    {
        MatrixLinearCombine<double>(D, { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0 }, { &A, &B, &C, &C2, &D, &D }, 0.0);
    }

    t1 = std::chrono::system_clock::now();

    std::chrono::duration<double> MatrixLinearCombine_time = t1 - t0;

    std::cout << "MatrixLinearCombine(OutputMatrix, AlphaList, MatrixList) time = " << MatrixLinearCombine_time.count() << '\n';

    //---------------------------------------------------------------------------------------------

    t0 = std::chrono::system_clock::now();

    for (uint64 i = 0; i < 10000; ++i)
    {
        MatrixLinearCombine<double>({ 1.0, 2.0, 3.0, 4.0, 5.0, 6.0 }, { &A, &B, &C, &C2, &D, &D }, 0.0);
    }

    t1 = std::chrono::system_clock::now();

    std::chrono::duration<double> MatrixLinearCombine_time_a = t1 - t0;

    std::cout << "MatrixLinearCombine(AlphaList, MatrixList) time = " << MatrixLinearCombine_time_a.count() << '\n';

    //---------------------------------------------------------------------------------------------

    t0 = std::chrono::system_clock::now();

    for (uint64 i = 0; i < 10000; ++i)
    {
        D+= MatrixLinearCombine<double>({ 1.0, 2.0, 3.0, 4.0, 5.0, 6.0 }, { &A, &B, &C, &C2, &D, &D }, 0.0);
    }

    t1 = std::chrono::system_clock::now();

    std::chrono::duration<double> MatrixLinearCombine_time_b = t1 - t0;

    std::cout << "D+=MatrixLinearCombine(AlphaList, MatrixList) time = " << MatrixLinearCombine_time_b.count() << '\n';
    
    //---------------------------------------------------------------------------------------------

    t0 = std::chrono::system_clock::now();

    for (uint64 i = 0; i < 10000; ++i)
    {
        D += 1.0*A + 2.0*B + 3.0*C + 4.0*C2 + 5.0*D + 6.0*D;
    }

    t1 = std::chrono::system_clock::now();

    std::chrono::duration<double> MatrixLinearCombine_time_c = t1 - t0;

    std::cout << "D += 1.0*A + 2.0*B + 3.0*C + 4.0*C2 + 5.0*D + 6.0*D : time = " << MatrixLinearCombine_time_c.count() << '\n';

    //---------------------------------------------------------------------------------------------

    t0 = std::chrono::system_clock::now();

    for (uint64 i = 0; i < 10000; ++i)
    {
        //D = D + 1.0*A + 2.0*B + 3.0*C + 4.0*C2 + 5.0*D + 6.0*D;

        D = D + 1.0*A + 2.0*B + 3.0*C + 4.0*C2 + 5.0*D + 6.0*D;
    }

    t1 = std::chrono::system_clock::now();

    std::chrono::duration<double> MatrixLinearCombine_time_d = t1 - t0;

    std::cout << "D = D + 1.0*A + 2.0*B + 3.0*C + 4.0*C2 + 5.0*D + 6.0*D : time= " << MatrixLinearCombine_time_d.count() << '\n';


    std::system("pause");

    //  D += A + B;
    // time : 19s GlueMatrix
    // time : 11s No GlueMatrix

    //  D += A + B + C;
    // time : 23s GlueMatrix
    // time : 21s No GlueMatrix

    //  D += A + B + C + C2;
    // time : 27s GlueMatrix
    // time : 30s No GlueMatrix


    arma::Mat<double> Am(A.GetElementDataRawPointer(), arma::uword(A.GetRowNumber()), arma::uword(A.GetColNumber()), false);
    arma::Mat<double> Bm(A.GetElementDataRawPointer(), arma::uword(B.GetRowNumber()), arma::uword(B.GetColNumber()), false);
    arma::Mat<double> Cm(A.GetElementDataRawPointer(), arma::uword(C.GetRowNumber()), arma::uword(C.GetColNumber()), false);
    arma::Mat<double> C2m(A.GetElementDataRawPointer(), arma::uword(C2.GetRowNumber()), arma::uword(C2.GetColNumber()), false);
    arma::Mat<double> Dm(A.GetElementDataRawPointer(), arma::uword(D.GetRowNumber()), arma::uword(D.GetColNumber()), false);

    t0 = std::chrono::system_clock::now();

    for (uint64 i = 0; i < 10000; ++i)
    {
        Dm += 1.0*Am + 2.0*Bm + 3.0*Cm + 4.0*C2m +5.0*Dm + 6.0*Dm;
    }

    t1 = std::chrono::system_clock::now();

    std::chrono::duration<double> MatrixLinearCombine_time_arma_1 = t1 - t0;

    std::cout << "arma Dm += 1.0*Am + 2.0*Bm + 3.0*Cm + 4.0*C2m +5.0*Dm + 6.0*Dm time = " << MatrixLinearCombine_time_arma_1.count() << '\n';

    //-----------------------------------------------------------------------------------
    t0 = std::chrono::system_clock::now();

    for (uint64 i = 0; i < 10000; ++i)
    {
        Dm = Dm + 1.0*Am + 2.0*Bm + 3.0*Cm + 4.0*C2m + 5.0*Dm + 6.0*Dm;
    }

    t1 = std::chrono::system_clock::now();

    std::chrono::duration<double> MatrixLinearCombine_time_arma_2 = t1 - t0;

    std::cout << "arma Dm = Dm + 1.0*Am + 2.0*Bm + 3.0*Cm + 4.0*C2m +5.0*Dm + 6.0*Dm time = " << MatrixLinearCombine_time_arma_2.count() << '\n';

    //--------------------------------------------------------------------------------------

    std::system("pause");

    /*
    raw time 7
    MatrixLinearCombine(OutputMatrix, AlphaList, MatrixList) time 8
    MatrixLinearCombine(AlphaList, MatrixList) time 14
    D+=MatrixLinearCombine(AlphaList, MatrixList) time 17
    D(0,0) -1.#QNAN
    D += 1.0*A + 2.0*B + 3.0*C + 4.0*C2 + 5.0*D + 6.0*D : time  18
    D(0,0) -1.#QNAN
    D = D + 1.0*A + 2.0*B + 3.0*C + 4.0*C2 + 5.0*D + 6.0*D : time  9
    Press any key to continue . . .
    arma Dm += 1.0*Am + 2.0*Bm + 3.0*Cm + 4.0*C2m +5.0*Dm + 6.0*Dm time 21
    arma Dm = Dm + 1.0*Am + 2.0*Bm + 3.0*Cm + 4.0*C2m +5.0*Dm + 6.0*Dm time 23
    */
}


void Test_GlueMatrix_Create()
{

    mdkMatrix<double> A(512, 512);

    A.Fill(1.0);

    mdkMatrix<double> B(512, 512);

    B.Fill(2.0);

    mdkMatrix<double> C(512, 512);

    C.Fill(3.0);

    //--------------

    // use operator =, not copy constructor
    C = A;

    // use operator =, not copy constructor
    C = A+B;




    C.Fill(3.0);
}

#endif