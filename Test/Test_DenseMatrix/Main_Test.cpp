﻿#include <iostream>

#include "mdkType.h"

#include "TestDenseMatrix.h"

using namespace mdk;

struct TempStruct
{
    int t1;
    int t2;
};

int Test_A(int64 a, int64 b)
{
    std::cout << "a = " << a << '\n';

    std::cout << " int64 a, int64 b" << '\n';

    return 1;
}

TempStruct Test_A(const std::initializer_list<int64>& a, std::initializer_list<int64>& b)
{
    std::cout << "initializer_list" << '\n';

    TempStruct Temp;

    Temp.t1 = 1;

    Temp.t2 = 2;

    return Temp;
}


TempStruct Test_A(const std::initializer_list<int64>& a, const Object& b)
{
    std::cout << "initializer_list" << '\n';

    TempStruct Temp;

    Temp.t1 = 1;

    Temp.t2 = 2;

    return Temp;
}

int aaa_main()
{
    double a1 = 1234567890;

    int64 a2 = int64(a1);


    auto a = Test_A(0, 0);

    auto b = Test_A({0}, {0});

   // auto c = Test_A({ 3 }, ALL);

    std::cout << "ab" << '\n';

    return 1;
}

int main()
{
   // auto a = Test_A(1, 1);

   // auto b = Test_A({ 1 }, {});

   // test_vec_insert();
    
    //Test_Constructor();

    //Test_Destructor();

  //  Test_MoveConstructor();

  //  Test_ShareConstuctor();

//    Test_ElementOperation();

  //  Test_ColOperation();

   // Test_Matrix_Operator();

    // Test_Matrix_ChangeSize();

    Test_Set_Get_Append_Delete_Insert_Col();

    Test_Set_Get_Append_Delete_Insert_Row();

   // Test_GetSubMatrix();

    // Test_Share();

    //Test_SubMatrix();

    //Test_ShadowMatrix();
   
   // Test_ShadowMatrix_Const();

    //Test_ShadowMatrix_Operator();
    
   // Test_ShadowMatrix_SubMatrix();

  //  Test_ShadowMatrix_Col_Row();

    // Test_GlueMatrix();

    //Test_GlueMatrix_2();

  //  Test_GlueMatrix_Speed1();

    //Test_GlueMatrix_Speed2();

    //Test_GlueMatrix_Create();

    //Test_Mutiplication_Speed();

    //Test_Arma();

    std::system("pause");

    return 1;
}