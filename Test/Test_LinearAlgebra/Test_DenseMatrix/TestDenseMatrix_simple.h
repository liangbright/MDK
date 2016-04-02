#pragma once

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

void print_vec(const std::vector<int>& vec)
{
    for (auto x : vec) {
        std::cout << ' ' << x;
    }
    std::cout << '\n';
}

void test_vec_insert()
{
    std::vector<int> vec;
    print_vec(vec);

    //auto it = vec.begin();
    //it = vec.insert(it, { 1, 2 });
    //print_vec(vec);

    std::vector<int> vec2 = {1, 2};

    auto it = vec.begin();
    it = vec.insert(it, vec2.begin(), vec2.end());
    print_vec(vec);
}


void Test_SubMatrix_simple()
{
    DenseMatrix<double> A;
	 A = { 1,     2,     3,     4,     5,     6,
           10,    20,    30,    40,    50,    60,
           100,   200,   300,   40,    500,   600,
           1000,  2000,  3000,  4000,  5000,  6000,
           10000, 20000, 30000, 40000, 50000, 60000, };
    A.Reshape(6, 5);
	A.TransposeInPlace();

    DisplayMatrix("A", A);

    // aa is scalar in vs2013	
	// can not be compiled in vs2015
    //auto aa = A({  });

    //this can not be compiled
    //auto bb = A({0});

    DenseMatrix<double> testA1 = {0};

	DenseMatrix<double> testA2 = A.GetSubMatrix(span(0, 3), span(2, 5));

    DenseMatrix<double> subA1;    
    subA1 = A({ 1, 2 });
    DisplayMatrix("subA1", subA1);

    DenseMatrix<double> subA2 = A.at({ 1, 2 });
    DisplayMatrix("subA2", subA2);
    
    DenseMatrix<double> subA3 = A.at({ 1 }, { 0, 1 });
	DisplayMatrix("subA3", subA3);

    subA3 = A({ 1, 2, 3 }, { 0, 1, 2 });
	DisplayMatrix("subA3", subA3);

    subA3 = A.at({ 1, 2, 3 }, { 0, 1, 2 });
    DisplayMatrix("subA3", subA3);

    DenseMatrix<double> subA4 = A.at(ALL, { 0, 1, 2, 1, 0 });
    DisplayMatrix("subA4", subA4);

    DenseMatrix<double> subA5 = A.at({ 0, 1, 2, 1, 0 }, ALL);
    DisplayMatrix("subA5", subA5);
    
}

}//namespace mdk
