#ifndef __TestMatrix_h
#define __TestMatrix_h

#include "mdkMatrix.h"

#define ElementLength_in 10

#define ElementNumber_in 10

namespace mdk
{

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

}//end of namespace

#endif