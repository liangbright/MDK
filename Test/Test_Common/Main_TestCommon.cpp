#include <iostream>
#include "mdkType.h"
#include "mdkMatrix.h"

#include "TestMatrix.h"

using namespace mdk;

int main(int argc, char** argv)
{
//	TestMatrix_std_vector();

	TestMatrix_Mutiplication();

	TestMatrix_CopyOnWrite();

	TestMatrix_ElementOperation();

	TestMatrix_Transpose();

	TestMatrix_Sum_Mean_Max_Min();

	TestMatrix_LinearCombine();

	TestMatrix_Arma();

	TestMatrix_Set_Get_Append_Col();

	TestMatrix_Set_Get_Append_Row();

}