#include <iostream>

#include "Test_TriangleMesh.h"

using namespace mdk;

int main()
{
    Test_MeshConstruction();
	Test_PointDataSet();
	Test_ShrinkEdge();
	//Test_ShrinkFace();
	Test_GlobalAttribute();
    std::system("pause");

    return 1;
}