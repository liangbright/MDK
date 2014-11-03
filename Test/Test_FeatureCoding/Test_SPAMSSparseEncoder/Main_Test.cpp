#include <iostream>

#include "Test_SPAMSSparseEncoder.h"

using namespace mdk;

int main()
{

    //Test_FindKNNByDistanceList();

    Test_OMP();

    Test_Lasso();

    std::system("pause");

    return 1;
}