#include "mdkLinearAlgebra.h"

namespace mdk
{

template<typename ScalarType>
inline 
mdkMatrix<ScalarType> LinearCombine(const std::vector<double>& AlphaList, const std::vector<mdkMatrix<ScalarType>*>& MatrixList)
{
	mdkMatrix<ScalarType> tempMatrix;

	tempMatrix.SetTobeTemporaryMatrix();

	auto MatrixNumber = MatrixList.size();

	auto Size = MatrixList[0]->GetSize();

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


template<typename ScalarType, uint64 Length>
inline
std::array<ScalarType, Length> LinearCombine(const std::vector<double>& CoefList, const std::vector<std::array<ScalarType, Length>*>& ArrayList)
{
	std::array<ScalarType, Length> tempArray;

	uint64 ArrayNumber = ArrayList.size();

	for (uint64 i = 0; i < ArrayList[0]->size(); ++i)
	{
		tempArray[i] = 0;

		for (uint64 k = 0; k < ArrayNumber; ++k)
		{
			tempArray[i] += CoefList[i] * ArrayList[k]->operator[](i);
		}
	}

	return tempArray;
}


template<typename ScalarType>
inline
std::vector<ScalarType> LinearCombine(const std::vector<double>& CoefList, const std::vector<std::vector<ScalarType>*>& ArrayList)
{
	std::vector<ScalarType> tempArray;

	uint64 ArrayNumber = ArrayList.size();

	for (uint64 i = 0; i < ArrayList[0]->size(); ++i)
	{
		tempArray[i] = 0;

		for (uint64 k = 0; k < ArrayNumber; ++k)
		{
			tempArray[i] += CoefList[i] * ArrayList[k]->operator[](i);
		}
	}

	return tempArray;
}


}//end namespace mdk