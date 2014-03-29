#ifndef __TestMatrix_h
#define __TestMatrix_h

//================================================
#include <algorithm>
#include <chrono>
#include <ctime>
#include <iomanip>
#include <initializer_list>
#include <functional>

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
    std::cout << "Test_Constructor()" << '\n';

    std::vector<DenseMatrix<double>> AList;

    AList.reserve(10);

    auto t0 = std::chrono::system_clock::now();

    for (int64 i = 0; i < 1000; ++i)
    {
        //DenseMatrix<double> AA;
        std::vector<DenseMatrix<double>> TempList(1);
    }

    auto t1 = std::chrono::system_clock::now();

    std::chrono::duration<double> raw_time = t1 - t0;
    std::cout << "construction  time " << raw_time.count() << '\n';

    DenseMatrix<int64> testRowVector;

    testRowVector = {101, 102, 103, 104};

    DisplayMatrix("testRowVector", testRowVector);

    DenseMatrix<int64> testColVector = { 101, 102, 103, 104 };

    testColVector.Reshape(4, 1);

    DisplayMatrix("testColVector", testColVector);


    DenseMatrix<int64> testA = { { 101, 102 },
                                 { 103, 104 } };

    DisplayMatrix("testA", testA);

    DenseMatrix<double> A(2, 2);

    A(span(1, 0), span(1, 0)) = 1;

    DisplayMatrix("A", A);

    A = { { 101, 102 },
          { 103, 104 } };

    DisplayMatrix("A", A);

    A[1] = 11;

    DisplayMatrix("A", A);

    t0 = std::chrono::system_clock::now();

    for (int64 i = 0; i < 60000; ++i)
    {
        A[1] *= i;
    }

    t1 = std::chrono::system_clock::now();

    raw_time = t1 - t0;
    std::cout << "A[1] *= i  time " << raw_time.count() << '\n';


    A(1) = 12;

    DisplayMatrix("A", A);

    auto ptrA = A.GetElementPointer();

    std::cout << "ptrA = A.GetElementPointer();" << '\n';

    ptrA[1] = 1;

    DisplayMatrix("A", A);

    ptrA[2] = 2;
    
    DisplayMatrix("A", A);

    A(1,1) = 3;

    DisplayMatrix("A", A);

    t0 = std::chrono::system_clock::now();

    for (int64 i = 0; i < 60000; ++i)
    {
        A(1, 1) *= i;
    }

    t1 = std::chrono::system_clock::now();

    raw_time = t1 - t0;
    std::cout << "A(1,1) +=1  time " << raw_time.count() << '\n';

    A.at(1,1) = 4;

    DisplayMatrix("A", A);

    A = { { 5, 6 },
          { 7, 8 } };

    DisplayMatrix("A", A);
 
    DenseMatrix<double> A1(A);

    DenseMatrix<double> B(A.GetElementPointer(), 2, 2);
    
    DisplayMatrix("B", B);

    DenseMatrix<double> C;

    C.ForceShare(A);

    A.Copy(A);

    A.Copy(C);

    C.Copy(A);
    //-----------------

    // move constructor is used 2 times
    // one for temp (A*A), the other for D(temp)
    DenseMatrix<double> D(A*A);


    // copy constructor, D will be temporary ?
    DenseMatrix<double> D1 = A*A;

    DenseMatrix<double> D2;

    // move constructor is used
    // move "=" is used
    //D2 is not temporary
    D2 = (A+A).ElementMultiply(A);

    //D3 is temporary
    auto D3 = D2;

    DenseMatrix<double> D4;

    // D4 is not temporary
    D4 = D2;

}

void Test_MoveConstructor()
{
    DenseMatrix<double> A(2, 2);

    auto ptrA = A.GetElementPointer();

    A = { { 1, 2 },
          { 3, 4 } };

    DisplayMatrix("A", A);

    auto Result = A.SVD();

    DenseMatrix<double> B;

    B = std::move(A);

    auto ptrB = B.GetElementPointer();

    auto ptrA_B = A.GetElementPointer();

    DisplayMatrix("B", B);
   
    DenseMatrix<double> C = std::move(A);

    auto ptrC = C.GetElementPointer();

    auto ptrA_C = A.GetElementPointer();

    DisplayMatrix("C", C);

    DenseMatrix<double> D1;

    // (B*B) is created from move constructor
    // D is assigned by move "="
    D1 = std::move(B*B);

    DenseMatrix<double> D2;

    // move "=" is used
    D2 = B*B;
    //-------------
}

void Test_ShareConstuctor()
{

    DenseMatrix<double> A(2, 4);

    A = { { 1, 2, 3, 4 },
          { 5, 6, 7, 8 }};


    DisplayMatrix("A", A);

    auto B = DenseMatrix<double>(A, ObjectConstructionTypeEnum::Share);

    B(0) = 10;

    DisplayMatrix("B", B);

    DisplayMatrix("A", A);

    auto C = DenseMatrix<double>(A);

    C(1) = 10;

    DisplayMatrix("C", C);

    DisplayMatrix("A", A);

    const DenseMatrix<double> D = DenseMatrix<double>(A, ObjectConstructionTypeEnum::Share);

    A(2) = 10;

    DisplayMatrix("D", D);

    DisplayMatrix("A", A);

    std::vector<const DenseMatrix<double>> MatrixList;

    MatrixList.emplace_back(A, ObjectConstructionTypeEnum::Share);

    DisplayMatrix("MatrixList[0]", MatrixList[0]);

    // can be compiled
    MatrixList[0](1,1) = 100;

    DisplayMatrix("MatrixList[0]", MatrixList[0]);

    DisplayMatrix("A", A);


    std::vector<const DenseMatrix<double>*> MatrixPtrList = {&A};

    // can not be compiled
   // (*MatrixPtrList[0])(1, 1) = 1000;

}

void Test_Matrix_Operator()
{
    std::cout << "Test_Matrix_Operator()" << '\n';

    double temp = {};

    DenseMatrix<double> tempMatrix = {};

    DenseMatrix<double> A(2, 4);

    A = { { 1, 2, 3, 4 },
          { 5, 6, 7, 8 } };

    DisplayMatrix("A", A);

    auto a = A(0, 0);

    DenseMatrix<double> b = A({ 0 }, ALL);

    //error message:
    auto b1 = A(0, 100);

    // Good:
    //this can not be compiled
    //auto b2 = A(0, ALL);

    // Good:
    //this can not be compiled, b3 is ShadowMatix
    //auto b3 = A({ 0 }, ALL);

    DenseMatrix<double> B = A({ 0 }, ALL);

    // attention !!!:
    DenseMatrix<double> B1;
    //  this is compiled: A({ 1 }, {}) is A({1}, {0})
    B1 = A({ 1 }, {});

    // this can NOT be compiled
    //auto B2 = A({ 0 }, { 0 });
    
    // this can NOT be compiled
    // A({ 0 }, { 0 }) = 10;

    DenseMatrix<double> C1;
        
    // this can NOT be compiled
    //C1 = A({ 0 });

    // this can NOT be compiled
    //DenseMatrix<double> C1a = A({ 0 }, {0});

    DenseMatrix<double> C2 = A({ 0, 1 });

    DenseMatrix<double> C3 = A({ 0, 1 }, { 1, 1 });

    DenseMatrix<double> C3a = A({ 0, 1 }, {0});

    DenseMatrix<double> C3b = A({0}, { 0, 1});

    DenseMatrix<double> C4 = A(0, 0)*A({ 0, 1 }, { 0 });

    // good: this can not be compiled
    //auto C5 = A({ 0, 1 }, {});

    // good: this can not be compiled
    //DenseMatrix<double> C5 = A({ 0, 1 }, {1}).~mdkShadowMatrix();

    DisplayMatrix("A", A);

  //  DisplayMatrix("C4", C4);

    // operator []

    DenseMatrix<double> D;
        
    D = A[0];

    DenseMatrix<double> D1;
        
    D1 = A[{0}];

    // good: this can not be compiled
    //D1a = A[{ 0, 2 }];

   // DenseMatrix<double> D2 = A({ 0, 1 });

   // A({ 0, 1 }) = 1;

    DisplayMatrix("A", A);
}


void Test_Mutiplication()
{

	DenseMatrix<double> A;

	A.Resize(2, 4);

    A = { { 1, 2, 3, 4 },
          { 5, 6, 7, 8 } };
	

    DisplayMatrix("A", A);

	DenseMatrix<double> B;

    B.Resize(4, 2);

    B = { { 1, 2 },
          { 3, 4 },
          { 5, 6 },
          { 7, 8 } };

    DisplayMatrix("B", B);

    DenseMatrix<double> C = A*B;

    DisplayMatrix("C = A * B", C);

	A *= 1.25;

    DisplayMatrix("A * 1.25", A);


	DenseMatrix<double> D(2, 10);
    D = { { 1,  2,  3,  4,  5,  6,  7,  8,  9,  10 },
          { 11, 12, 13, 14, 15, 16, 17, 18, 19, 20 } };

    DisplayMatrix("D", D);
}


void Test_Share()
{
    DenseMatrix<double> A(2,2);

    A = { { 1, 2 },
          { 3, 4 } };

    DisplayMatrix("A", A);

    DenseMatrix<double> B;

    B.Share(A);

    B(1, 1) = 0;

    DisplayMatrix("A", A);

    DisplayMatrix("B", B);
}


void Test_Mutiplication_Speed()
{
    int64 Lx = 100;

    int64 Ly = 100;

    DenseMatrix<double> A(Lx, Ly);

    DenseMatrix<double> B(Lx, Ly);

    DenseMatrix<double> C(Lx, Ly);

    auto t0 = std::chrono::system_clock::now();

    for (int64 i = 0; i < 1000; ++i)
    {
       // C = 1.0*(A*C) + 2.0*(A*C) + 3.0*(A*C) + 4.0*(A*C) + 5.0*(A*C) + 6.0*(A*C);
        //C = 1.0*A*C + 2.0*A*C + 3.0*A*C + 4.0*A*C + 5.0*A*C + 6.0*A*C;

        C = (A*C) + (A*C) + (A*C) + (A*C) + (A*C) + (A*C);
    }

    auto t1 = std::chrono::system_clock::now();
    std::chrono::duration<double> Temp_time = t1 - t0;
    std::cout << "C = A*C time = " << Temp_time.count() << '\n';


    arma::Mat<double> Am(A.GetElementPointer(), arma::uword(A.GetRowNumber()), arma::uword(A.GetColNumber()), false);
    arma::Mat<double> Bm(B.GetElementPointer(), arma::uword(B.GetRowNumber()), arma::uword(B.GetColNumber()), false);
    arma::Mat<double> Cm(C.GetElementPointer(), arma::uword(C.GetRowNumber()), arma::uword(C.GetColNumber()), false);

    t0 = std::chrono::system_clock::now();

    for (int64 i = 0; i < 1000; ++i)
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
	DenseMatrix<double> A;

    A.Resize(2, 4);

    A = { { 1, 2, 3, 4 },
          { 5, 6, 7, 8 } };

    DisplayMatrix("A", A);

    auto B = A.ElementNamedOperation("sqrt");

    DisplayMatrix("B = A.ElementNamedOperation(\"sqrt\")", B);

    B = A.ElementNamedOperation('+', 1);

    DisplayMatrix("B = A.ElementNamedOperation('+', 1)", B);

    auto C = A^10.0;

    DisplayMatrix("C=A^10.0", C);


    A.ElementNamedOperationInPlace('+', 1);

    DisplayMatrix(" A.ElementNamedOperationInPlace('+', 1)", A);

}

void Test_ColOperation()
{
    DenseMatrix<double> A;

    A.Resize(2, 4);

    A = { { 1, 2, 3, 4 },
          { 5, 6, 7, 8 } };

    DisplayMatrix("A", A);

    auto B = A.ColNamedOperation(0, "sqrt");

    DisplayMatrix("B = A.ColNamedOperation(0, \"sqrt\")", B);

    B = A.ColNamedOperation(0, '+', 1);

    DisplayMatrix("B = A.ColNamedOperation(0, '+', 1)", B);


    A.ColNamedOperationInPlace(0, '+', 1);

    DisplayMatrix(" A.ColNamedOperationInPlace(0, '+', 1", A);

}


void Test_Transpose()
{
	DenseMatrix<double> A;

    A.Resize(2, 4);

    A = { { 1, 2, 3, 4 },
		  { 5, 6, 7, 8 }};

    DisplayMatrix("A", A);

	auto B = A.Transpose();

    DisplayMatrix("B = A.Transpose()", B);

}


void Test_Sum_Mean_Max_Min()
{
	DenseMatrix<double> A;

    A.Resize(2, 4);

    A = { { 1, 2, 3, 4 },
          { 5, 6, 7, 8 } };

    DisplayMatrix("A", A);

	auto value = A.Sum();
	std::cout << "value = sum(A) = " << value << '\n';

	value = A.Mean();
	std::cout << "value = mean(A) = " << value << '\n';

	value = A.Max();
	std::cout << "value = max(A) = " << value << '\n';

	value = A.Min();
	std::cout << "value = min(A) = " << value << '\n';

	auto RowMatrix = A.SumToRow();

    DisplayMatrix("A.SumToRow()", RowMatrix);

	auto ColMatrix = A.SumToCol();

    DisplayMatrix("A.SumToCol()", ColMatrix);

	RowMatrix = A.MeanToRow();

    DisplayMatrix("A.MeanToRow()", RowMatrix);

	ColMatrix = A.MeanToCol();

    DisplayMatrix("A.MeanToCol()", ColMatrix);

	RowMatrix = A.MaxToRow();

    DisplayMatrix("A.MaxToRow()", RowMatrix);

	ColMatrix = A.MaxToCol();

    DisplayMatrix("A.MaxToCol()", ColMatrix);

	RowMatrix = A.MinToRow();

    DisplayMatrix("A.MinToRow()", RowMatrix);

	ColMatrix = A.MinToCol();

    DisplayMatrix("A.MinToCol()", ColMatrix);

}


void Test_LinearCombine()
{
    /*
	std::cout << "Test_LinearCombine " << '\n';

	DenseMatrix<double> A(3, 4);
	A = {1,  2,  3,  4,
	     5,  6,  7,  8,
	     9, 10, 11, 12};

	std::cout << "A = " << '\n';

	for (int64 i = 0; i < 3; ++i)
	{
		for (int64 j = 0; j < 4; ++j)
		{
			std::cout << A(i, j) << ' ';
		}

		std::cout << '\n';
	}

	DenseMatrix<double> B(3, 4);
	B =  1.0 + A;

	std::cout << "B = A+1 " << '\n';

	for (int64 i = 0; i < 3; ++i)
	{
		for (int64 j = 0; j < 4; ++j)
		{
			std::cout << B(i, j) << ' ';
		}

		std::cout << '\n';
	}

	DenseMatrix<double> C(3, 4);
	C = B + 1.0;

	std::cout << "C = B+1 " << '\n';

	for (int64 i = 0; i < 3; ++i)
	{
		for (int64 j = 0; j < 4; ++j)
		{
			std::cout << C(i, j) << ' ';
		}

		std::cout << '\n';
	}

	DenseMatrix<double> D(3, 4);
	D = C + 1.0;

	std::cout << "D = C+1 " << '\n';

	for (int64 i = 0; i < 3; ++i)
	{
		for (int64 j = 0; j < 4; ++j)
		{
			std::cout << D(i, j) << ' ';
		}

		std::cout << '\n';
	}

	auto E = A + B * 2.0 + C * 3.0 + D * 4.0;

	std::cout << "E = A + B*2 + C*3 + D*4 = " << '\n';

	for (int64 i = 0; i < 3; ++i)
	{
		for (int64 j = 0; j < 4; ++j)
		{
			std::cout << E(i, j) << ' ';
		}

		std::cout << '\n';
	}

	std::vector<double> AlphaList = { 1, 2, 3, 4 };

	std::vector<DenseMatrix<double>*> MatrixList = { &A, &B, &C, &D };

	auto SumMatrix = mdk::LinearCombine(AlphaList, MatrixList);

	std::cout << "1A+2B+3C+4D = " << '\n';

	for (int64 i = 0; i < 3; ++i)
	{
		for (int64 j = 0; j < 4; ++j)
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

	DenseMatrix<double> A;

    DenseMatrix<double> Col(1, 3);
	//std::vector<double> Col;

	Col = { 1, 2, 3 };

	A.AppendCol(Col);

    std::cout << "A = " << '\n';
    DisplayMatrix("A.AppendCol(Col)", A);

	Col = { 4, 5, 6 };
	A.AppendCol(Col);

    DisplayMatrix("A.AppendCol(Col)", A);

	Col = { 7, 8, 9 };
	A.AppendCol(Col);

    DisplayMatrix("A.AppendCol(Col)", A);


	Col = { 10, 20, 30 };
	A.SetCol(1, Col);

    DisplayMatrix("A.SetCol(1, Col)", A);

	Col = { 101, 201, 301 };
	A.SetCol(2, Col);

    DisplayMatrix("A.SetCol(2, Col)", A);


    Col = { 11, 21, 31 };
    A.InsertCol(2, Col);

    DisplayMatrix("A.InsertCol(1, Col)", A);


    Col = { 110, 210, 310 };
    A.InsertCol(2, Col);

    DisplayMatrix("A.InsertCol(2, Col)", A);

    Col = { 4, 5, 6 };
    A.InsertCol(0, Col);

    DisplayMatrix("A.InsertCol(0, Col)", A);

    A.DeleteCol(0);

    DisplayMatrix("A.DeleteCol(0)", A);

    A.DeleteCol(1);

    DisplayMatrix("A.DeleteCol(1)", A);

    A.DeleteCol({0, 1});
    
    DisplayMatrix("A.DeleteCol({0, 1})", A);

    A.DeleteCol(1);

    DisplayMatrix("A.DeleteCol(1)", A);

}


void Test_Set_Get_Append_Delete_Insert_Row()
{
    std::cout << "Test_Set_Get_Append_Delete_Insert_Row " << '\n';

    DenseMatrix<double> A;

    DenseMatrix<double> Row(1, 3);
    //std::vector<double> Row;

    Row = { 1, 2, 3 };
    A.AppendRow(Row);

    DisplayMatrix("A.AppendRow(Row)", A);

    Row = { 4, 5, 6 };
    A.AppendRow(Row);

    DisplayMatrix("A.AppendRow(Row)", A);

    Row = { 7, 8, 9 };
    A.AppendRow(Row);

    DisplayMatrix("A.AppendRow(Row)", A);


    Row = { 10, 20, 30 };
    A.SetRow(1, Row);

    DisplayMatrix("A.SetRow(1, Row)", A);

    Row = { 101, 201, 301 };
    A.SetRow(2, Row);

    DisplayMatrix("A.SetRow(2, Row)", A);


    Row = { 11, 21, 31 };
    A.InsertRow(2, Row);

    DisplayMatrix("A.InsertRow(2, Row)", A);


    Row = { 110, 210, 310 };
    A.InsertRow(2, Row);

    DisplayMatrix("A.InsertRow(2, Row)", A);

    Row = { 4, 5, 6 };
    A.InsertRow(0, Row);

    DisplayMatrix("A.InsertRow(0, Row)", A);

    A.DeleteRow(0);

    DisplayMatrix("A.DeleteRow(0)", A);

    A.DeleteRow(1);

    DisplayMatrix("A.DeleteRow(1)", A);

    A.DeleteRow({ 0, 1 });

    DisplayMatrix("A.DeleteRow({ 0, 1 })", A);

    A.DeleteRow(1);

    DisplayMatrix("A.DeleteRow(1)", A);
}


void Test_GetSubMatrix()
{
    DenseMatrix<double> A(5, 6);

    A = { 1,     2,     3,     4,     5,     6,
          10,    20,    30,    40,    50,    60, 
          100,   200,   300,   40,    500,   600, 
          1000,  2000,  3000,  4000,  5000,  6000,
          10000, 20000, 30000, 40000, 50000, 60000, };

    DisplayMatrix("A", A);

   // auto subA1 = A.GetSubMatrix(span(0, 3), span(2, 5));

   // DisplayMatrix("subA1", subA1);

    auto subA2 = A.GetSubMatrix({ 1 }, {0, 1});

    DisplayMatrix("subA2", subA2);

    auto subA3 = A.GetSubMatrix({ 1, 2, 3}, { 0, 1 , 2});

    DisplayMatrix("subA3", subA3);

    auto subAa = A.GetSubMatrix(ALL, { 0, 1, 2 });

    DisplayMatrix("subAa", subAa);

    auto subAb = A.GetSubMatrix({ 0, 1, 2 }, ALL);

    DisplayMatrix("subAb", subAb);

    DenseMatrix<double> B1;

    A.GetSubMatrix(B1, { 0, 1, 2 }, { 0, 1, 2, 3, 4 });

    DisplayMatrix("B1", B1);

    DenseMatrix<double> B2;

    A.GetSubMatrix(B2, {0, 1, 2, 3, 4}, { 0, 1, 2 });

    DisplayMatrix("B2", B2);
}


void Test_SubMatrix()
{
    DenseMatrix<double> A(5, 6);

    A = { 1,     2,     3,     4,     5,     6,
          10,    20,    30,    40,    50,    60, 
          100,   200,   300,   40,    500,   600, 
          1000,  2000,  3000,  4000,  5000,  6000,
          10000, 20000, 30000, 40000, 50000, 60000, };

    DisplayMatrix("A", A);

   // DenseMatrix<double> subA1 = A.GetSubMatrix(span(0, 3), span(2, 5));

    DisplayMatrix("A", A);

    DenseMatrix<double> subA2 = A({1, 2});

   // DenseMatrix<double> subA2 = A.at({ 1 }, { 0, 1 });

    DisplayMatrix("subA2", subA2);

    DenseMatrix<double> subA3;
    
   // subA3 = A({ 1, 2, 3 }, { 0, 1, 2 });

   // DenseMatrix<double> subA3 = A.at({ 1, 2, 3 }, { 0, 1, 2 });

   // DisplayMatrix("subA3", subA3);

    DenseMatrix<double> subAa = A.at(ALL, { 0, 1, 2, 1, 0});

    DisplayMatrix("subAa", subAa);

    DenseMatrix<double> subAb = A.at({ 0, 1, 2, 1, 0 }, ALL);

    DisplayMatrix("subAb", subAb);

 }


void Test_ShadowMatrix()
{

    DenseMatrix<double> A(3, 3);

    A = { { 1, 2, 3 },
          { 4, 5, 6 },
          { 7, 8, 9 } };

    DenseMatrix<double> B(4, 4);

    B = { { 1, 2, 3, 0 },
          { 4, 5, 6, 0 },
          { 7, 8, 9, 0 },
          { 0, 0, 0, 0 } };

    DenseMatrix<double> C;

    C = 1.0 + A*A + A * (10.0 + A*B({0,1,2}, {0,1,2}));

    DisplayMatrix("C", C);

    DenseMatrix<double> D = A(ALL);

    std::system("pause");
}


void Test_Arma()
{
	std::cout << "Test_Arma " << '\n';

	DenseMatrix<double> A(3, 3);
    A = { { 1, 2, 3 },
          { 0, 5, 0 },
          { 0, 0, 9 } };

    DisplayMatrix("A", A);

    DenseMatrix<double> invA = A.Inv();

    DisplayMatrix("A.Inv()", invA, 3);

    DenseMatrix<double> AinvA = A*invA;

    DisplayMatrix("A*invA", AinvA, 3);

	auto ASVD = A.SVD();

    DisplayMatrix("ASVD.U", ASVD.U, 3);

    DisplayMatrix("ASVD.S", ASVD.S, 3);

    DisplayMatrix("ASVD.V", ASVD.V, 3);

    DenseMatrix<double> tempA = ASVD.U * ASVD.S * MatrixTranspose(ASVD.V);

    DisplayMatrix("ASVD.U*ASVD.S*ASVD.V'", tempA, 3);


    std::system("pause");
}


void Test_Matrix_ChangeSize()
{
    std::cout << "Test_Matrix_ChangeSize()" << '\n';

    DenseMatrix<double> A(6, 9);

    A = { { 1, 2, 3, 4, 5, 6, 7, 8, 9 },
          { 2, 5, 0, 2, 1, 2, 1, 2, 1 },
          { 3, 0, 9, 3, 5, 3, 5, 3, 5 },
          { 4, 0, 8, 4, 5, 4, 5, 4, 5 },
          { 5, 0, 7, 5, 5, 5, 5, 5, 5 },
          { 6, 0, 6, 6, 5, 6, 5, 6, 5 } };

    DisplayMatrix("A", A);

    // this can NOT be compiled
    //A.DeleteCol({0});

    A.DeleteCol(0);

    DisplayMatrix("A.DeleteCol(0)", A);


    A.DeleteCol({1, 7});

    DisplayMatrix("A.DeleteCol({1, 7})", A);


    A.DeleteRow({1, 5});


    DisplayMatrix("A.DeleteRow({1, 5})", A);
}


void Test_ShadowMatrix_SubMatrix()
{
    std::cout << "Test_ShadowMatrix_SubMatrix()" << '\n';

    DenseMatrix<double> A(6, 9);

    A = { { 1, 2, 3, 4, 5, 6, 7, 8, 9 },
          { 2, 5, 0, 2, 1, 2, 1, 2, 1 },
          { 3, 0, 9, 3, 5, 3, 5, 3, 5 },
          { 4, 0, 8, 4, 5, 4, 5, 4, 5 },
          { 5, 0, 7, 5, 5, 5, 5, 5, 5 },
          { 6, 0, 6, 6, 5, 6, 5, 6, 5 } };

    

    DisplayMatrix("A", A);

    auto A1 = A(0, 1);

    DenseMatrix<double> subA1;
    
    subA1 = A.at(0, 1);

    DisplayMatrix("subA1 = A.SubMatrix({ 0 }, { 1 })", subA1);

    DenseMatrix<double> subA2 = A.at({ 1, 0 }, { 2, 1 });

    DisplayMatrix("subA2 = A.SubMatrix({ 1, 0 }, { 2, 1 })", subA2);

    // good this can not be compiled
    DenseMatrix<double> subA3;    
    //subA3 = A.SubMatrix({ 1, 0 }, {});
    //subA3 = A.SubMatrix({}, { 1, 0 });

    DenseMatrix<double> subA4 = A.at({ 1, 0 }, ALL);

    DisplayMatrix("subA4 = A.SubMatrix({ 1, 0 }, ALL)", subA4);
}



void Function_for_Test_ShadowMatrix_Const(const DenseMatrix<double>& Matrix)
{
    DenseMatrix<double> A = Matrix(ALL);

    // Good:can not be compiled
    //Matrix(ALL) = 1;

}

void Function_for_Test_ShadowMatrix_Non_Const(DenseMatrix<double>& Matrix)
{
    DenseMatrix<double> A = Matrix(ALL);

    // can be compiled
    Matrix(ALL) = 1;

    A = Matrix(ALL) * A;
}

void Test_ShadowMatrix_Const()
{
    DenseMatrix<double> A(3, 3);

    A = { 1, 2, 3,
          4, 5, 6,
          7, 8, 9 };

    Function_for_Test_ShadowMatrix_Const(A);

    Function_for_Test_ShadowMatrix_Non_Const(A);
}


void Test_ShadowMatrix_Col_Row()
{
    DenseMatrix<double> A(6, 9);

    A = { { 1, 2, 3, 4, 5, 6, 7, 8, 9 },
          { 2, 5, 0, 2, 1, 2, 1, 2, 1 },
          { 3, 0, 9, 3, 5, 3, 5, 3, 5 },
          { 4, 0, 8, 4, 5, 4, 5, 4, 5 },
          { 5, 0, 7, 5, 5, 5, 5, 5, 5 },
          { 6, 0, 6, 6, 5, 6, 5, 6, 5 } };

    DisplayMatrix("A", A);

    DenseMatrix<double> subA1 = A.Col({0, 1});

    DisplayMatrix("subA1 = A.Col({0, 1}) ", subA1);

    DenseMatrix<double> subA2 = A.Row({ 0, 1 });

    DisplayMatrix("subA2 = A.Row({0, 1}) ", subA2);
}

void Test_ShadowMatrix_Operator()
{
    DenseMatrix<double> A(5, 5);
    A = { { 1, 2, 3, 4, 5 },
          { 0, 5, 0, 2, 1 },
          { 0, 0, 9, 1, 0 },
          { 0, 1, 0, 1, 0 },
          { 1, 0, 1, 0, 1 } };

    DisplayMatrix("A", A);

    DenseMatrix<double> subA1 = A({ 0, 1 }, ALL);

    DenseMatrix<double> subA2 = subA1;

    DisplayMatrix("subA2", subA2);

    DenseMatrix<double> subA3 = A({ 0, 1 }, ALL) * 10.0;

    DisplayMatrix("subA3", subA3);

    DenseMatrix<double> subA4 = A(ALL, { 0, 1 });

    DisplayMatrix("sub4", subA4);

    std::vector<double> v = {};

    DenseMatrix<double> subA5 = A({0, 1}, {0, 1, 2}) * A({0, 1, 2}, {0, 1});

    DisplayMatrix("subA5", subA5);

    // good:
    // before use ALL symbol
    // compiler internal error  C1001
    // DenseMatrix<double> subA5a = A({ 0, 1 }, { 0, 1, 2 }) * A({}, { 0, 1 });
    //
    // after use ALL, then still error 

    DenseMatrix<double> subA6 = A.Row({0, 1}) * A.Col({0, 1});

    DisplayMatrix("subA6", subA6);


    typedef double  aaa;

    typedef int  bbb;

    if (std::is_same<aaa, bbb>::value == true)
    {

    }
}


void Test_GlueMatrix()
{
    std::cout << "Test_GlueMatrix()" << '\n';


    DenseMatrix<double> A(3, 3);
    A = { { 1, 2, 0 },
          { 1, 2, 0 },
          { 3, 3, 3 } };


    DenseMatrix<double> B(2, 3);
    B = { { 1, 2, 3 },
          { 1, 2, 3 } };


    DenseMatrix<double> C(2, 3);
    C = { { 0, 2, 1 },
          { 0, 2, 1 } };

    DenseMatrix<double> C1(2, 3);
    C1 = { { 1, 0, 1 },
           { 1, 0, 1 } };

    DenseMatrix<double> D = 1.0*A({ 0, 1 }, ALL) + 2.0*B - 3.0*C + 4.0*C1;

    DisplayMatrix("D", D);

   // auto E = MatrixAdd(DenseMatrix<double>(A.Row({ 0, 1 })), C);


    DenseMatrix<double> All = A(ALL, { 0, 1 })*(B + 1.0 + C - 3.0*C + 4.0*C1);


    DisplayMatrix("All", All);

    (B + C).~DenseGlueMatrixForLinearCombination();

    //auto a = (A*A).Mean();

    //A({ 0, 1 }) += A(0);
}


void Test_GlueMatrix_2()
{
    std::cout << "Test_GlueMatrix_2() " << '\n';

    DenseMatrix<double> Scalar(0);

    DenseMatrix<double> A(3, 3);
    A = { { 1, 1, 1 },
          { 1, 1, 1 },
          { 1, 1, 1 } };

    DenseMatrix<double> AA(3, 4);

    AA = { { 1, 1, 1, 1 },
           { 1, 1, 1, 1 },
           { 1, 1, 1, 1 } };


    DenseMatrix<double> B(2, 3);
    B = { { 1, 2, 3 },
          { 1, 2, 3 } };


    DenseMatrix<double> C(2, 3);
    C = { { 0, 2, 1 },
          { 0, 2, 1 } };

    DenseMatrix<double> C1(2, 3);
    C1 = { { 1, 0, 1 },
           { 1, 0, 1 } };

    DenseMatrix<double> D = 1.0*A({ 0, 1 }, ALL) + 2.0*B - 3.0*C + 4.0*C1;

    DisplayMatrix("D", D);

   // auto E = MatrixAdd(DenseMatrix<double>(A.Row({ 0, 1 })), C);


    DenseMatrix<double> All = A(ALL, { 0, 1 })*(B + 1.0 + C - 3.0*C + 4.0*C1);

    DisplayMatrix("All", All);

    //--------------------------

    DenseMatrix<double> A1 = A;

    DisplayMatrix("A1", A1);

    DenseMatrix<double> A2 = A;

    DisplayMatrix("A2", A2);

    DenseMatrix<double> A3 = A;

    DisplayMatrix("A3", A3);

    DenseMatrix<double> temp = A1 * AA({ 0, 1, 2 }, { 0, 1, 2 });

    DisplayMatrix("temp", temp);

    //DenseMatrix<double> A4 = A1 + A1 * AA({ 0, 1, 2 }, { 0, 1, 2 });

    DenseMatrix<double> A4 = 1.0 + A1*(A1+ A1*A2 + 1.0 + 10.0*(A1 + A2*A3 + 1.0 + 10.0*A1)) + A1*AA({0, 1, 2}, {0, 1, 2}) + 1.0;


    DisplayMatrix("A4", A4, 3);
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
      //  DenseMatrix<TestClass> A(Data);
    }

    std::cout << "All= " << '\n';
}


void Test_GlueMatrix_Speed1()
{
    int64 Lx = 100;

    int64 Ly = 100;

    DenseMatrix<double> A(Lx, Ly);

    DenseMatrix<double> B(Lx, Ly);

    DenseMatrix<double> C(Lx, Ly);

    auto t0 = std::time(0);

    DenseMatrix<double> D(Lx, Ly);
    
    for (int64 i = 0; i < 10000; ++i)
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
    int64 Lx = 10000;

    int64 Ly = 10;

    auto ElementNumber = Lx * Ly;

    int64 TotalLoopNumber = std::max<int64>(30000, ElementNumber / 10);

    //-------------------------------------------------------------------------------------------

    DenseMatrix<double> A(Lx, Ly);

    A.Fill(1.0);

    DenseMatrix<double> B(Lx, Ly);

    B.Fill(2.0);

    DenseMatrix<double> C(Lx, Ly);

    C.Fill(3.0);

    DenseMatrix<double> C2;

    C2.Fill(3.0);
   
    DenseMatrix<double> D(Lx, Ly);

    D.Fill(0.0);

    //--------------

    D += A + B;

    //----------

    D = D + 1.0*A + 2.0*B + 3.0*C + 4.0*C2 + 5.0*D + 6.0*D;

    //-------------------------------------------------------------------------------------------

    double ElementList_Coef[] = { 1, 2, 3, 4, 5, 6 };

    double* MatrixElementDataRawPtrList[] = { A.GetElementPointer(),
                                              B.GetElementPointer(),
                                              C.GetElementPointer(),
                                              C2.GetElementPointer(),
                                              D.GetElementPointer(),
                                              D.GetElementPointer() };

    
    auto tempRawPointer = D.GetElementPointer();

    auto t0 = std::chrono::system_clock::now();

   // #pragma loop(hint_parallel(8))

    for (int64 i = 0; i < TotalLoopNumber; ++i)
    {
        for (int64 LinearIndex = 0; LinearIndex < ElementNumber; ++LinearIndex)
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

    for (int64 i = 0; i < TotalLoopNumber; ++i)
    {
        MatrixLinearCombine<double>(D, { 1.0, 2.0, 3.0, 4.0, 5.0, 6.0 }, { &A, &B, &C, &C2, &D, &D }, 0.0);
    }

    t1 = std::chrono::system_clock::now();

    std::chrono::duration<double> MatrixLinearCombine_time = t1 - t0;

    std::cout << "MatrixLinearCombine(OutputMatrix, AlphaList, MatrixList) time = " << MatrixLinearCombine_time.count() << '\n';

    //---------------------------------------------------------------------------------------------

    t0 = std::chrono::system_clock::now();

    for (int64 i = 0; i < TotalLoopNumber; ++i)
    {
        MatrixLinearCombine<double>({ 1.0, 2.0, 3.0, 4.0, 5.0, 6.0 }, { &A, &B, &C, &C2, &D, &D }, 0.0);
    }

    t1 = std::chrono::system_clock::now();

    std::chrono::duration<double> MatrixLinearCombine_time_a = t1 - t0;

    std::cout << "MatrixLinearCombine(AlphaList, MatrixList) time = " << MatrixLinearCombine_time_a.count() << '\n';

    //---------------------------------------------------------------------------------------------

    t0 = std::chrono::system_clock::now();

    for (int64 i = 0; i < TotalLoopNumber; ++i)
    {
        D+= MatrixLinearCombine<double>({ 1.0, 2.0, 3.0, 4.0, 5.0, 6.0 }, { &A, &B, &C, &C2, &D, &D }, 0.0);
    }

    t1 = std::chrono::system_clock::now();

    std::chrono::duration<double> MatrixLinearCombine_time_b = t1 - t0;

    std::cout << "D+=MatrixLinearCombine(AlphaList, MatrixList) time = " << MatrixLinearCombine_time_b.count() << '\n';
    
    //---------------------------------------------------------------------------------------------

    t0 = std::chrono::system_clock::now();

    for (int64 i = 0; i < TotalLoopNumber; ++i)
    {
        D += 1.0*A + 2.0*B + 3.0*C + 4.0*C2 + 5.0*D + 6.0*D;
    }

    t1 = std::chrono::system_clock::now();

    std::chrono::duration<double> MatrixLinearCombine_time_c = t1 - t0;

    std::cout << "D += 1.0*A + 2.0*B + 3.0*C + 4.0*C2 + 5.0*D + 6.0*D : time = " << MatrixLinearCombine_time_c.count() << '\n';

    //---------------------------------------------------------------------------------------------

    t0 = std::chrono::system_clock::now();

    for (int64 i = 0; i < TotalLoopNumber; ++i)
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


    arma::Mat<double> Am(A.GetElementPointer(), arma::uword(A.GetRowNumber()), arma::uword(A.GetColNumber()), false);
    arma::Mat<double> Bm(A.GetElementPointer(), arma::uword(B.GetRowNumber()), arma::uword(B.GetColNumber()), false);
    arma::Mat<double> Cm(A.GetElementPointer(), arma::uword(C.GetRowNumber()), arma::uword(C.GetColNumber()), false);
    arma::Mat<double> C2m(A.GetElementPointer(), arma::uword(C2.GetRowNumber()), arma::uword(C2.GetColNumber()), false);
    arma::Mat<double> Dm(A.GetElementPointer(), arma::uword(D.GetRowNumber()), arma::uword(D.GetColNumber()), false);

    t0 = std::chrono::system_clock::now();

    for (int64 i = 0; i < TotalLoopNumber; ++i)
    {
        Dm += 1.0*Am + 2.0*Bm + 3.0*Cm + 4.0*C2m +5.0*Dm + 6.0*Dm;
    }

    t1 = std::chrono::system_clock::now();

    std::chrono::duration<double> MatrixLinearCombine_time_arma_1 = t1 - t0;

    std::cout << "arma Dm += 1.0*Am + 2.0*Bm + 3.0*Cm + 4.0*C2m +5.0*Dm + 6.0*Dm time = " << MatrixLinearCombine_time_arma_1.count() << '\n';

    //-----------------------------------------------------------------------------------
    t0 = std::chrono::system_clock::now();

    for (int64 i = 0; i < TotalLoopNumber; ++i)
    {
        Dm = Dm + 1.0*Am + 2.0*Bm + 3.0*Cm + 4.0*C2m + 5.0*Dm + 6.0*Dm;
    }

    t1 = std::chrono::system_clock::now();

    std::chrono::duration<double> MatrixLinearCombine_time_arma_2 = t1 - t0;

    std::cout << "arma Dm = Dm + 1.0*Am + 2.0*Bm + 3.0*Cm + 4.0*C2m +5.0*Dm + 6.0*Dm time = " << MatrixLinearCombine_time_arma_2.count() << '\n';

    //--------------------------------------------------------------------------------------

    std::system("pause");

    /*
    Lx=512, Ly=512; TotalLoopNumber=10000;
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
    int64 Lx = 100;

    int64 Ly = 100;

    DenseMatrix<double> A(Lx, Ly);

    A.Fill(1.0);

    DenseMatrix<double> B(Lx, Ly);

    B.Fill(2.0);

    DenseMatrix<double> C(Lx, Ly);

    C.Fill(3.0);

    //--------------

    // use operator =, not copy constructor
    C = A;

    // use operator =, not copy constructor
    C = A+B;

    C.Fill(3.0);
}

}//namespace mdk

#endif