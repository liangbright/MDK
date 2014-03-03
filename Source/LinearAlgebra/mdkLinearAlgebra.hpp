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
void MatrixLinearCombine(mdkMatrix<ElementType>& OutputMatrix, const std::vector<ElementType>& CoefList, const std::vector<mdkMatrix<ElementType>*>& MatrixList)
{
	auto MatrixNumber = MatrixList.size();

    auto CoefNumber = CoefList.size();

    if (MatrixNumber != CoefNumber)
    {
        mdkError << "MatrixNumber != AlphaNumber @ MatrixLinearCombine(OutputMatrix, AlphaList, MatrixList)" << '\n';
        return;
    }

    if (MatrixNumber == 0)
    {
        mdkWarning << "Input is empty @ MatrixLinearCombine(OutputMatrix, AlphaList, MatrixList)" << '\n';
        return;
    }

	auto Size = MatrixList[0]->GetSize();

    auto OutputRawPointer = OutputMatrix.GetElementDataRawPointer();

    std::vector<ElementType*> MatrixElementDataRawPtrList(MatrixNumber);

    for (uint64 k = 0; k < MatrixNumber; ++k)
    {
        MatrixElementDataRawPtrList[k] = MatrixList[k]->GetElementDataRawPointer();
    }

    auto CoefRawPtr = CoefList.data();

    auto ElementNumber = Size.ColNumber*Size.RowNumber;

    switch (MatrixNumber)
    {
    case 1:
        MatrixLinearCombine_UnrollForLoop_1(OutputRawPointer, ElementNumber, CoefRawPtr, MatrixElementDataRawPtrList);
        break;

    case 2:
        MatrixLinearCombine_UnrollForLoop_2(OutputRawPointer, ElementNumber, CoefRawPtr, MatrixElementDataRawPtrList);
        break;

    case 3:
        MatrixLinearCombine_UnrollForLoop_3(OutputRawPointer, ElementNumber, CoefRawPtr, MatrixElementDataRawPtrList);
        break;

    case 4:
        MatrixLinearCombine_UnrollForLoop_4(OutputRawPointer, ElementNumber, CoefRawPtr, MatrixElementDataRawPtrList);
        break;

    case 5:
        MatrixLinearCombine_UnrollForLoop_5(OutputRawPointer, ElementNumber, CoefRawPtr, MatrixElementDataRawPtrList);
        break;

    case 6:       
        MatrixLinearCombine_UnrollForLoop_6(OutputRawPointer, ElementNumber, CoefRawPtr, MatrixElementDataRawPtrList);        
        break;

    default:
            
        ElementType tempElement = CoefRawPtr[0];

        for (uint64 LinearIndex = 0; LinearIndex < ElementNumber; ++LinearIndex)
        {
            tempElement -= tempElement;

            for (uint64 k = 0; k < MatrixNumber; ++k)
            {
                tempElement += CoefRawPtr[k] * MatrixElementDataRawPtrList[k][LinearIndex];
            }

            OutputRawPointer[LinearIndex] = tempElement;
        }
    }
}


template<typename ElementType>
inline
mdkMatrix<ElementType> MatrixLinearCombine(const std::vector<ElementType>& CoefList, const std::vector<mdkMatrix<ElementType>*>& MatrixList)
{
    mdkMatrix<ElementType> tempMatrix;

    tempMatrix.SetTobeTemporary();

    auto MatrixNumber = MatrixList.size();

    auto CoefNumber = CoefList.size();

    if (MatrixNumber != CoefNumber)
    {
        mdkError << "MatrixNumber != AlphaNumber @ MatrixLinearCombine(AlphaList, MatrixList)" << '\n';
        return tempMatrix;
    }

    if (MatrixNumber == 0)
    {
        mdkWarning << "Input is empty @ MatrixLinearCombine(AlphaList, MatrixList)" << '\n';
        return tempMatrix;
    }

    auto Size = MatrixList[0]->GetSize();

    tempMatrix.SetSize(Size.RowNumber, Size.ColNumber);

    MatrixLinearCombine(tempMatrix, CoefList, MatrixList);

    return tempMatrix;
}


template<typename ElementType>
inline
void MatrixLinearCombine_UnrollForLoop_1(ElementType* Output, uint64 ElementNumber,
                                         const ElementType* Coef, const std::vector<ElementType*>& MatrixElementDataRawPtrList)
{
    for (uint64 LinearIndex = 0; LinearIndex < ElementNumber; ++LinearIndex)
    {
        Output[LinearIndex] = Coef[0] * MatrixElementDataRawPtrList[0][LinearIndex];
    }
}


template<typename ElementType>
inline
void MatrixLinearCombine_UnrollForLoop_2(ElementType* Output, uint64 ElementNumber,
                                         const ElementType* Coef, const std::vector<ElementType*>& MatrixElementDataRawPtrList)
{
    for (uint64 LinearIndex = 0; LinearIndex < ElementNumber; ++LinearIndex)
    {
        Output[LinearIndex] = Coef[0] * MatrixElementDataRawPtrList[0][LinearIndex]
                            + Coef[1] * MatrixElementDataRawPtrList[1][LinearIndex];
    }
}


template<typename ElementType>
inline
void MatrixLinearCombine_UnrollForLoop_3(ElementType* Output, uint64 ElementNumber,
                                         const ElementType* Coef, const std::vector<ElementType*>& MatrixElementDataRawPtrList)
{
    for (uint64 LinearIndex = 0; LinearIndex < ElementNumber; ++LinearIndex)
    {
        Output[LinearIndex] =   Coef[0] * MatrixElementDataRawPtrList[0][LinearIndex]
                              + Coef[1] * MatrixElementDataRawPtrList[1][LinearIndex]
                              + Coef[2] * MatrixElementDataRawPtrList[2][LinearIndex];
    }
}



template<typename ElementType>
inline
void MatrixLinearCombine_UnrollForLoop_4(ElementType* Output, uint64 ElementNumber,
                                         const ElementType* Coef, const std::vector<ElementType*>& MatrixElementDataRawPtrList)
{
    for (uint64 LinearIndex = 0; LinearIndex < ElementNumber; ++LinearIndex)
    {
        Output[LinearIndex] =  Coef[0] * MatrixElementDataRawPtrList[0][LinearIndex]
                             + Coef[1] * MatrixElementDataRawPtrList[1][LinearIndex]
                             + Coef[2] * MatrixElementDataRawPtrList[2][LinearIndex]
                             + Coef[3] * MatrixElementDataRawPtrList[3][LinearIndex];
    }
}


template<typename ElementType>
inline
void MatrixLinearCombine_UnrollForLoop_5(ElementType* Output, uint64 ElementNumber,
                                         const ElementType* Coef, const std::vector<ElementType*>& MatrixElementDataRawPtrList)
{
    for (uint64 LinearIndex = 0; LinearIndex < ElementNumber; ++LinearIndex)
    {
        Output[LinearIndex] =  Coef[0] * MatrixElementDataRawPtrList[0][LinearIndex]
                             + Coef[1] * MatrixElementDataRawPtrList[1][LinearIndex]
                             + Coef[2] * MatrixElementDataRawPtrList[2][LinearIndex]
                             + Coef[3] * MatrixElementDataRawPtrList[3][LinearIndex]
                             + Coef[4] * MatrixElementDataRawPtrList[4][LinearIndex];
    }
}

template<typename ElementType>
inline
void MatrixLinearCombine_UnrollForLoop_6(ElementType* Output, uint64 ElementNumber,
                                         const ElementType* Coef, const std::vector<ElementType*>& MatrixElementDataRawPtrList)
{
    for (uint64 LinearIndex = 0; LinearIndex < ElementNumber; ++LinearIndex)
    {
        Output[LinearIndex] = Coef[0] * MatrixElementDataRawPtrList[0][LinearIndex]
                            + Coef[1] * MatrixElementDataRawPtrList[1][LinearIndex]
                            + Coef[2] * MatrixElementDataRawPtrList[2][LinearIndex]
                            + Coef[3] * MatrixElementDataRawPtrList[3][LinearIndex]
                            + Coef[4] * MatrixElementDataRawPtrList[4][LinearIndex]
                            + Coef[5] * MatrixElementDataRawPtrList[5][LinearIndex];
    }
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