#include <iostream>

#include "TestDenseMatrix.h"
#include "TestDenseMatrix_simple.h"

using namespace mdk;

struct TempStruct
{
    int t1;
    int t2;
};

int Test_A(int_max a, int_max b)
{
    std::cout << "a = " << a << '\n';

    std::cout << " int_max a, int_max b" << '\n';

    return 1;
}

TempStruct Test_A(const std::initializer_list<int_max>& a, std::initializer_list<int_max>& b)
{
    std::cout << "initializer_list" << '\n';

    TempStruct Temp;

    Temp.t1 = 1;

    Temp.t2 = 2;

    return Temp;
}


TempStruct Test_A(const std::initializer_list<int_max>& a, const Object& b)
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

    int_max a2 = int_max(a1);


    auto a = Test_A(0, 0);

    auto b = Test_A({0}, {0});

   // auto c = Test_A({ 3 }, ALL);

    std::cout << "ab" << '\n';

    return 1;
}

int main()
{
	//Test_SubMatrix_simple();

    //Test_Operator_With_ElementPointer();

    //Test_MatrixMutiply();

   // auto a = Test_A(1, 1);

   // auto b = Test_A({ 1 }, {});

   // test_vec_insert();
    
   // Test_Constructor_a();

    // Test_Constructor_External_Data();

    //Test_Constructor();

    //Test_Destructor();

  //  Test_MoveConstructor();

  //  Test_ShareConstuctor();

   // Test_ElementMultiply();

    //Test_ElementOperation();

  //  Test_ColOperation();

   // Test_Matrix_Operator();

    // Test_Matrix_ChangeSize();

   // Test_Set_Get_Append_Delete_Insert_Col();

   // Test_Set_Get_Append_Delete_Insert_Row();

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

    //Test_FindCol();

    //Test_Matrix_Operator_with_DenseVector();

    //Test_unique();

 //   Test_SortCol();

	//Test_DenseMatrix_DenseVector_ObjectArray();

	test_save_load();

    std::system("pause");

    return 1;
}