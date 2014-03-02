#include <iostream>

#include "mdkType.h"
#include "mdkMatrix.h"

#include "TestMatrix.h"

using namespace mdk;

int main()
{
    //Test_ShadowMatrix();
    
   // Test_GlueMatrix();

    Test_ShadowMatrix_Operator();

    Test_Matrix_ChangeSize();

    Test_ShadowMatrix_SubMatrix();

    Test_ShadowMatrix_Col_Row();
}