#include "mdkLinearAlgebra.h"

namespace mdk
{

template<typename ElementType>
inline 
mdkMatrix<ElementType> MatrixAdd(const mdkMatrix<ElementType>& MatrixA, const mdkMatrix<ElementType>& MatrixB)
{
    auto SizeA = MatrixA.GetSize();

    auto SizeB = MatrixB.GetSize();

    if (SizeA.ColNumber == 1 && SizeA.RowNumber == 1)
    {
        return MatrixAdd(MatrixA(0), MatrixB);
    }

    if (SizeB.ColNumber == 1 && SizeB.RowNumber == 1)
    {
        return MatrixAdd(MatrixA, MatrixB(0));
    }

    mdkMatrix<ElementType> tempMatrix;

    tempMatrix.SetTobeTemporary();


    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        mdkError << "Size does not match @ mdkLinearAlgebra Add(MatrixA, MatrixB)" << '\n';
        return  tempMatrix;
    }

    if (SizeA.RowNumber == 0 || SizeA.ColNumber == 0)
    {
        mdkWarning << "MatrixA or MatrixB is empty @ mdkLinearAlgebra Add(MatrixA, MatrixB)" << '\n';
        return  tempMatrix;
    }

    tempMatrix.SetSize(SizeA.RowNumber, SizeA.ColNumber);

    auto tempRawPointer = tempMatrix.GetElementDataRawPointer();

    auto RawPointerA = MatrixA.GetElementDataRawPointer();

    auto RawPointerB = MatrixB.GetElementDataRawPointer();

    for (uint64 i = 0; i < SizeA.RowNumber*SizeA.ColNumber; ++i)
    {
        tempRawPointer[i] = RawPointerA[i] + RawPointerB[i];
    }

    return  tempMatrix;
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> MatrixDiff(const mdkMatrix<ElementType>& MatrixA, const mdkMatrix<ElementType>& MatrixB)
{
    auto SizeA = MatrixA.GetSize();

    auto SizeB = MatrixB.GetSize();

    if (SizeA.ColNumber == 1 && SizeA.RowNumber == 1)
    {
        return Diff(MatrixA(0), MatrixB);
    }

    if (SizeB.ColNumber == 1 && SizeB.RowNumber == 1)
    {
        return Diff(MatrixA, MatrixB(0));
    }

    mdkMatrix<ElementType> tempMatrix;

    tempMatrix.SetTobeTemporary();


    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        mdkError << "Size does not match @ mdkLinearAlgebra Add(MatrixA, MatrixB)" << '\n';
        return  tempMatrix;
    }

    if (SizeA.RowNumber == 0 || SizeA.ColNumber == 0)
    {
        mdkWarning << "MatrixA or MatrixB is empty @ mdkLinearAlgebra Add(MatrixA, MatrixB)" << '\n';
        return  tempMatrix;
    }

    tempMatrix.SetSize(SizeA.RowNumber, SizeA.ColNumber);

    auto tempRawPointer = tempMatrix.GetElementDataRawPointer();

    auto RawPointerA = MatrixA.GetElementDataRawPointer();

    auto RawPointerB = MatrixB.GetElementDataRawPointer();

    for (uint64 i = 0; i < SizeA.RowNumber*SizeA.ColNumber; ++i)
    {
        tempRawPointer[i] = RawPointerA[i] - RawPointerB[i];
    }

    return  tempMatrix;
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> MatrixMultiply(const mdkMatrix<ElementType>& MatrixA, const mdkMatrix<ElementType>& MatrixB)
{
    return MatrixA*MatrixB;
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> MatrixAdd(const ElementType& Element, const mdkMatrix<ElementType>& Matrix)
{
    mdkMatrix<ElementType> tempMatrix;

    tempMatrix.SetTobeTemporary();

    auto Size = Matrix.GetSize();

    if (Size.RowNumber == 0 || Size.ColNumber == 0)
    {
        mdkError << "Matrix is empty @ mdkMatrix operator+(Element, Matrix)" << '\n';
        return  tempMatrix;
    }

    tempMatrix.SetSize(Size.RowNumber, Size.ColNumber);

    auto tempRawPointer = tempMatrix.GetElementDataRawPointer();

    auto RawPointer = Matrix.GetElementDataRawPointer();

    for (uint64 i = 0; i < Size.RowNumber*Size.ColNumber; ++i)
    {
        tempRawPointer[i] = Element + RawPointer[i];
    }

    return tempMatrix;
}


template<typename ElementType>
inline
mdkMatrix<ElementType> MatrixDiff(const ElementType& Element, const mdkMatrix<ElementType>& Matrix)
{
    mdkMatrix<ElementType> tempMatrix;

    tempMatrix.SetTobeTemporary();

    auto Size = Matrix.GetSize();

    if (Size.RowNumber == 0 || Size.ColNumber == 0)
    {
        mdkError << "Matrix is empty @ mdkMatrix operator+(Element, Matrix)" << '\n';
        return  tempMatrix;
    }

    tempMatrix.SetSize(Size.RowNumber, Size.ColNumber);

    auto tempRawPointer = tempMatrix.GetElementDataRawPointer();

    auto RawPointer = Matrix.GetElementDataRawPointer();

    for (uint64 i = 0; i < Size.RowNumber*Size.ColNumber; ++i)
    {
        tempRawPointer[i] = Element - RawPointer[i];
    }

    return tempMatrix;
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> MatrixMultiply(const ElementType& Element, const mdkMatrix<ElementType>& Matrix)
{
    mdkMatrix<ElementType> tempMatrix;

    tempMatrix.SetTobeTemporary();

    auto Size = Matrix.GetSize();

    if (Size.RowNumber == 0 || Size.ColNumber == 0)
    {
        mdkError << "Matrix is empty @ mdkMatrix operator+(Element, Matrix)" << '\n';
        return  tempMatrix;
    }

    tempMatrix.SetSize(Size.RowNumber, Size.ColNumber);

    auto tempRawPointer = tempMatrix.GetElementDataRawPointer();

    auto RawPointer = Matrix.GetElementDataRawPointer();

    for (uint64 i = 0; i < Size.RowNumber*Size.ColNumber; ++i)
    {
        tempRawPointer[i] = Element * RawPointer[i];
    }

    return tempMatrix;
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> MatrixAdd(const mdkMatrix<ElementType>& Matrix, const ElementType& Element)
{
    return MatrixAdd(Element, Matrix);
}


template<typename ElementType>
inline
mdkMatrix<ElementType> MatrixDiff(const mdkMatrix<ElementType>& Matrix, const ElementType& Element)
{
    mdkMatrix<ElementType> tempMatrix;

    tempMatrix.SetTobeTemporary();

    auto Size = Matrix.GetSize();

    if (Size.RowNumber == 0 || Size.ColNumber == 0)
    {
        mdkError << "Matrix is empty @ mdkMatrix operator+(Element, Matrix)" << '\n';
        return  tempMatrix;
    }

    tempMatrix.SetSize(Size.RowNumber, Size.ColNumber);

    auto tempRawPointer = tempMatrix.GetElementDataRawPointer();

    auto RawPointer = Matrix.GetElementDataRawPointer();

    for (uint64 i = 0; i < Size.RowNumber*Size.ColNumber; ++i)
    {
        tempRawPointer[i] = RawPointer[i] - Element;
    }

    return tempMatrix;
}


template<typename ElementType>
inline mdkMatrix<ElementType> MatrixMultiply(const mdkMatrix<ElementType>& Matrix, const ElementType& Element)
{
    return MatrixMultiply(Element, Matrix);
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> MatrixLinearCombine(const std::vector<double>& AlphaList, const std::vector<mdkMatrix<ElementType>*>& MatrixList)
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
std::array<ElementType, Length> MatrixLinearCombine(const std::vector<double>& CoefList, const std::vector<std::array<ElementType, Length>*>& ArrayList)
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
std::vector<ElementType> MatrixLinearCombine(const std::vector<double>& CoefList, const std::vector<std::vector<ElementType>*>& ArrayList)
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