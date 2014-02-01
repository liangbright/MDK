#include "mdkLinearAlgebra.h"

namespace mdk
{

template<typename ScalarType>
inline 
mdkMatrix<ScalarType> LinearCombine(std::vector<double> AlphaList, std::vector<mdkMatrix<ScalarType>*> MatrixList)
{
	mdkMatrix<ScalarType> tempMatrix;

	auto MatrixNumber = MatrixList.size();

	MatrixSize Size = MatrixList[0]->GetSize();

	tempMatrix.SetSize(Size.RowNumber, Size.ColNumber);

	auto tempRawPointer = tempMatrix.GetElementDataRawPointer();

	for (uint64 i = 0; i < Size.ColNumber*Size.RowNumber; ++i)
	{
		tempRawPointer[i] = 0;

		for (uint64 k = 0; k < MatrixNumber; ++k)
		{
			tempRawPointer[i] += AlphaList[k] * MatrixList[k]->Element(i);
		}
	}

	return tempMatrix;
}


}//end namespace mdk