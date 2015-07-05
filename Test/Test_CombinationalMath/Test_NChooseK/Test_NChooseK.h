#ifndef __TestFileIO_h
#define __TestFileIO_h

#include <iostream>
#include "mdkDenseMatrix.h"
#include "mdkDenseMatrix_FileIO.h"
#include "mdkCombinatorialMath.h"

namespace mdk
{


void Test_a()
{

	auto Coef = ComputeBinomialCoefficient(36, 3);

	auto SubSetList = NChooseK(36, 3);

	std::cout << "NChooseK=" << Coef << '\n';

	std::cout << "NChooseK=" << SubSetList.GetLength() << '\n';

	DenseMatrix<double> A(3, SubSetList.GetLength());
	for (int_max k = 0; k < SubSetList.GetLength(); ++k)
	{
		A(0, k) = SubSetList[k][0];
		A(1, k) = SubSetList[k][1];
		A(2, k) = SubSetList[k][2];
	}
	
	SaveDenseMatrixAsJsonDataFile(A, "A.json");
}


}//end of namespace


#endif