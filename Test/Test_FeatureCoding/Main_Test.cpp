#include <iostream>

#include "TestFeatureCoding.h"

using namespace mdk;



int main()
{
   
    Test_OpenCVMatrix();

    Test_mdkKMeansDictionaryBuilder_using_OpenCV();

    std::system("pause");

    return 1;
}