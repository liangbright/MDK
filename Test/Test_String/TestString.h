#include <iostream> 

#include "mdkString.h"

namespace mdk
{

void testA()
{
	std::string FileNameA = "C:/Research/MDK/MDK_Build/Test/Test_Image/Test_3DImage/TestData/CurveA.vtk";
	std::string FileNameB = "中华人民共和国";
	std::string FileNameC = "C:/Research中华人民共和国/MDK/MDK_Build/Test/Test_Image/Test_3DImage/TestData/CurveAandB.vtk";

	//std::wstring FileNameAA = FileNameA;//"C:/Research/MDK/MDK_Build/Test/Test_Image/Test_3DImage/TestData/CurveA.vtk";

	String FileNameA1 = "C:/Research/MDK/MDK_Build/Test/Test_Image/Test_3DImage/TestData/CurveA.vtk";	
	String FileNameB1 = "中华人民共和国";
	String FileNameC1 = "C:/Research中华人民共和国/MDK/MDK_Build/Test/Test_Image/Test_3DImage/TestData/CurveAandB.vtk";

	std::cout << FileNameA << '\n';
	std::cout << FileNameB << '\n';
	std::cout << FileNameC << '\n';

	std::cout << "String" << '\n';

	std::wcout.imbue(std::locale("chs"));
	std::wcout << L"中华人民共和国" << '\n';

	//char16_t A = 'a';
	
}

}
