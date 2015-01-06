#include "TestFileIO.h"

int main()
{
	using namespace mdk;

	//Test_MatrixIO();

    //Test_ImageIO();

	//Test_PolygonMeshIO();

	//Test_TriangleMeshIO();

	Test_VTK_Json();

	Test_ITK_Json();

    std::system("pause");

    return 1;
}