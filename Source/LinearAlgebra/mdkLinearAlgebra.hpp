#include "mdkLinearAlgebra.h"

namespace mdk
{

template<typename ElementType>
inline 
mdkMatrix<ElementType> LinearCombine(const std::vector<double>& AlphaList, const std::vector<mdkMatrix<ElementType>*>& MatrixList)
{
	mdkMatrix<ElementType> tempMatrix;

	tempMatrix.SetTobeTemporary();

	auto MatrixNumber = MatrixList.size();

	auto Size = MatrixList[0]->GetSize();

	tempMatrix.SetSize(Size.RowNumber, Size.ColNumber);

	auto tempRawPointer = tempMatrix.GetElementDataRawPointer();

	for (uint64 i = 0; i < Size.ColNumber*Size.RowNumber; ++i)
	{
		tempRawPointer[i] = 0;

		for (uint64 k = 0; k < MatrixNumber; ++k)
		{
			tempRawPointer[i] += AlphaList[k] * MatrixList[k]->at(i);
		}
	}

	return tempMatrix;
}


template<typename ElementType, uint64 Length>
inline
std::array<ElementType, Length> LinearCombine(const std::vector<double>& CoefList, const std::vector<std::array<ElementType, Length>*>& ArrayList)
{
	std::array<ElementType, Length> tempArray;

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


template<typename ElementType>
inline
std::vector<ElementType> LinearCombine(const std::vector<double>& CoefList, const std::vector<std::vector<ElementType>*>& ArrayList)
{
	std::vector<ElementType> tempArray;

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