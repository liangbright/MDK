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


}