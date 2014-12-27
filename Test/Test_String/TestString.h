#ifndef __TestImage_h
#define __TestImage_h

#include <ctime>
#include <cstdlib>
#include <array>
#include <chrono>
#include <ctime>
#include <locale.h>
#include <stdio.h> 
#include <sstream>

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

	std::basic_ostringstream<char16_t> out;

	out << FileNameA1.StdString() << '\n';
	out << FileNameB1.StdString() << '\n';
	out << FileNameC1.StdString() << '\n';

	//char16_t A = 'a';
}

}


#endif