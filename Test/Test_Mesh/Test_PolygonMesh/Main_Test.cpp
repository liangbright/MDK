#include <iostream>

#include "Test_PolygonMesh.h"

using namespace mdk;

int main()
{
    Test_MeshConstruction();
	Test_ShrinkEdge();
	Test_SplitEdge();
	Test_ShrinkFace();
	Test_ReversePointOrder();
	Test_AddtionalInfo();
    std::system("pause");

    return 1;
}