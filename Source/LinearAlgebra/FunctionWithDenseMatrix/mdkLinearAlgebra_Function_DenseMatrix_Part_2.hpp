#ifndef mdk_LinearAlgebra_Function_DenseMatrix_Part_2_hpp
#define mdk_LinearAlgebra_Function_DenseMatrix_Part_2_hpp


namespace mdk
{

//================================================================================================================================//

template<typename ElementType, typename MatchFunctionType>
DenseMatrix<int_max> FindElementInMatrix(const DenseMatrix<ElementType>& InputMatrix, MatchFunctionType MatchFunction)
{
    return FindElementInMatrix(InputMatrix, InputMatrix.GetElementCount(), 0, InputMatrix.GetElementCount()-1, MatchFunction);
}


template<typename ElementType, typename MatchFunctionType>
DenseMatrix<int_max> FindElementInMatrix(const DenseMatrix<ElementType>& InputMatrix, int_max MaxOutputNumber, MatchFunctionType MatchFunction)
{
    return FindElementInMatrix(InputMatrix, MaxOutputNumber, 0, InputMatrix.GetElementCount()-1, MatchFunction);
}


template<typename ElementType, typename MatchFunctionType>
DenseMatrix<int_max> FindElementInMatrix(const DenseMatrix<ElementType>& InputMatrix, int_max MaxOutputNumber, 
                                         int_max LinearIndex_start, int_max LinearIndex_end, MatchFunctionType MatchFunction)
{
    DenseMatrix<int_max> LinearIndexList;

    if (MaxOutputNumber == 0)
    {
        return LinearIndexList;
    }

    auto InputElementNumber = InputMatrix.GetElementCount();

    if (MaxOutputNumber < 0 || MaxOutputNumber > InputElementNumber)
    {
        MDK_Error("MaxOutputNumber is invalid @ mdkLinearAlgebra_DenseMatrix FindElementInMatrix(...)")
        return LinearIndexList;
    }

    if (LinearIndex_start < 0 || LinearIndex_start >= InputElementNumber || LinearIndex_start > LinearIndex_end)
    {
        MDK_Error("LinearIndex_start is invalid @ mdkLinearAlgebra_DenseMatrix FindElementInMatrix(...)")
        return LinearIndexList;
    }

    if (LinearIndex_end < 0 || LinearIndex_end >= InputElementNumber)
    {
        MDK_Error("LinearIndex_end is invalid @ mdkLinearAlgebra_DenseMatrix FindElementInMatrix(...)")
        return LinearIndexList;
    }

    if (InputElementNumber == 0)
    {
        return LinearIndexList;
    }

    if (LinearIndex_start == LinearIndex_end)
    {
        LinearIndexList.AppendCol({ LinearIndex_start });
        return LinearIndexList;
    }

    LinearIndexList.ReserveCapacity(MaxOutputNumber);

    for (int_max i = LinearIndex_start; i <= LinearIndex_end; ++i)
    {
        if (MatchFunction(InputMatrix[i]) == true)
        {
            LinearIndexList.AppendCol({ i });

            auto CurrentNumber = LinearIndexList.GetElementCount();

            if (CurrentNumber == MaxOutputNumber)
            {
                break;
            }
        }
    }

    return LinearIndexList;
}


template<typename ElementType, typename MatchFunctionType>
int_max FindElementInMatrix(const DenseMatrix<ElementType>& InputMatrix, const char* first_or_last, MatchFunctionType MatchFunction)
{
    int_max Index_output = -1;

	if (first_or_last[0] == "f")//first
	{
		for (int_max i = 0; i < InputMatrix.GetElementCount(); ++i)
		{
			if (MatchFunction(InputMatrix[i]) == true)
			{
				Index_output = i;
				break;
			}
		}
	}
	else if (first_or_last[0] == "l")//last
	{
		for (int_max i = InputMatrix.GetElementCount()-1; i >= 0; --i)
		{
			if (MatchFunction(InputMatrix[i]) == true)
			{
				Index_output = i;
				break;
			}
		}
	}
	else
	{
		MDK_Error("Wrong option @ mdkLinearAlgebra_DenseMatrix FindElementInMatrix(...)")
	}

    return Index_output;
}


template<typename ElementType>
inline
DenseMatrix<int_max> ExactMatchElementInMatrix(const DenseMatrix<ElementType>& InputMatrix, const ElementType& InputElement)
{
	return FindElementInMatrix(*this, InputElement, [&](const ElementType& Element){return Element == InputElement; });
}


template<typename ElementType>
inline
int_max ExactMatchElementInMatrix(const DenseMatrix<ElementType>& InputMatrix, const char* first_or_last, const ElementType& InputElement)
{
	return FindElementInMatrix(*this, first_or_last, InputElement, [&](const ElementType& Element){return Element == InputElement; });
}


template<typename ElementType, typename MatchFunctionType>
DenseMatrix<int_max> FindColInMatrix(const DenseMatrix<ElementType>& InputMatrix, MatchFunctionType MatchFunction)
{
    return FindColInMatrix(InputMatrix, InputMatrix.GetColCount(), 0, InputMatrix.GetColCount()-1, MatchFunction);
}


template<typename ElementType, typename MatchFunctionType>
DenseMatrix<int_max> FindColInMatrix(const DenseMatrix<ElementType>& InputMatrix, int_max MaxOutputColCount, MatchFunctionType MatchFunction)
{
    return FindColInMatrix(InputMatrix, MaxOutputColCount, 0, InputMatrix.GetColCount()-1, MatchFunction);
}


template<typename ElementType, typename MatchFunctionType>
DenseMatrix<int_max> FindColInMatrix(const DenseMatrix<ElementType>& InputMatrix, int_max MaxOutputColCount, 
                                     int_max ColIndex_start, int_max ColIndex_end, MatchFunctionType MatchFunction)
{
    DenseMatrix<int_max> ColIndexList;

    if (MaxOutputColCount == 0)
    {
        return ColIndexList;
    }

    auto InputSize = InputMatrix.GetSize();

    if (MaxOutputColCount < 0 || MaxOutputColCount > InputSize.ColCount)
    {
        MDK_Error("MaxOutputColCount is invalid @ mdkLinearAlgebra_DenseMatrix FindColInMatrix(...)")
        return ColIndexList;
    }

    if (ColIndex_start < 0 || ColIndex_start >= InputSize.ColCount || ColIndex_start > ColIndex_end)
    {
        MDK_Error("ColIndex_start is invalid @ mdkLinearAlgebra_DenseMatrix FindColInMatrix(...)")
        return ColIndexList;
    }

    if (ColIndex_end < 0 || ColIndex_end >= InputSize.ColCount)
    {
        MDK_Error("ColIndex_end is invalid @ mdkLinearAlgebra_DenseMatrix FindColInMatrix(...)")
        return ColIndexList;
    }

    if (InputSize.ColCount == 0)
    {
        return ColIndexList;
    }

    if (ColIndex_start == ColIndex_end)
    {
        ColIndexList.Append(ColIndex_start);
        return ColIndexList;
    }

    ColIndexList.ReserveCapacity(MaxOutputColCount);

    DenseMatrix<ElementType> ColVector;

	if (ColIndex_start < ColIndex_end)
	{
		for (int_max i = ColIndex_start; i <= ColIndex_end; ++i)
		{
			DenseMatrix<ElementType> ColVector;
			ColVector.ForceShare(InputMatrix.GetElementPointerOfCol(i), InputSize.RowCount, 1);

			if (MatchFunction(ColVector) == true)
			{
				ColIndexList.Append(i);

				auto CurrentNumber = ColIndexList.GetElementCount();

				if (CurrentNumber == MaxOutputColCount)
				{
					break;
				}
			}
		}
	}
	else // if (ColIndex_start > ColIndex_end)
	{
		for (int_max i = ColIndex_start; i >= ColIndex_end; --i)
		{
			DenseMatrix<ElementType> ColVector;
			ColVector.ForceShare(InputMatrix.GetElementPointerOfCol(i), InputSize.RowCount, 1);

			if (MatchFunction(ColVector) == true)
			{
				ColIndexList.Append(i);

				auto CurrentNumber = ColIndexList.GetElementCount();

				if (CurrentNumber == MaxOutputColCount)
				{
					break;
				}
			}
		}
	}

    return ColIndexList;
}


template<typename ElementType, typename MatchFunctionType>
DenseMatrix<int_max> FindColInMatrix(const DenseMatrix<ElementType>& InputMatrix, const std::string& first_or_last, MatchFunctionType MatchFunction)
{
	int_max ColIndex_output = -1;

	auto InputSize = InputMatrix.GetSize();

	if (first_or_last == "first")
	{
		for (int_max i = 0; i <= InputSize.ColCount - 1; ++i)
		{
			DenseMatrix<ElementType> ColVector;
			ColVector.ForceShare(InputMatrix.GetElementPointerOfCol(i), InputSize.RowCount, 1);

			if (MatchFunction(ColVector) == true)
			{
				ColIndex_output = i;
				break;
			}
		}
	}
	else if (first_or_last == "last")
	{
		for (int_max i = InputSize.ColCount - 1; i >= 0;  --i)
		{
			DenseMatrix<ElementType> ColVector;
			ColVector.ForceShare(InputMatrix.GetElementPointerOfCol(i), InputSize.RowCount, 1);

			if (MatchFunction(ColVector) == true)
			{
				ColIndex_output = i;
				break;
			}
		}
	}
	else
	{
		MDK_Error("Wrong option @ mdkLinearAlgebra_DenseMatrix FindElementInMatrix(...)")
	}	

	return ColIndex_output;
}


template<typename ElementType, typename CompareFunctionType>
DenseMatrix<int_max> SortColInMatrix(const DenseMatrix<ElementType>& InputMatrix, CompareFunctionType CompareFunction)
{
    return SortColInMatrix(InputMatrix, 0, InputMatrix.GetColCount()-1, CompareFunction);
}


template<typename ElementType, typename CompareFunctionType>
DenseMatrix<int_max> SortColInMatrix(const DenseMatrix<ElementType>& InputMatrix,
                                     int_max ColIndex_start, int_max ColIndex_end, CompareFunctionType CompareFunction)
{
    DenseMatrix<int_max> ColIndexList;

    auto InputSize = InputMatrix.GetSize();

    if (ColIndex_start < 0 || ColIndex_start >= InputSize.ColCount || ColIndex_start > ColIndex_end)
    {
        MDK_Error("ColIndex_start is invalid @ mdkLinearAlgebra_DenseMatrix SortColInMatrix(...)")
        return ColIndexList;
    }

    if (ColIndex_end < 0 || ColIndex_end >= InputSize.ColCount)
    {
        MDK_Error("ColIndex_end is invalid @ mdkLinearAlgebra_DenseMatrix SortColInMatrix(...)")
        return ColIndexList;
    }

    if (InputSize.ColCount == 0)
    {
        return ColIndexList;
    }

    if (ColIndex_start == ColIndex_end)
    {
        ColIndexList.AppendCol({ ColIndex_start });
        return ColIndexList;
    }

    ColIndexList.FastResize(1, InputSize.ColCount);

    for (int_max i = ColIndex_start; i <= ColIndex_end; ++i)
    {
        ColIndexList[i] = i;
    }

    DenseMatrix<ElementType> ColVector_a, ColVector_b;

    std::sort(ColIndexList.begin(), ColIndexList.end(), [&](int_max a, int_max b)
    {
        ColVector_a.ForceShare(InputMatrix.GetElementPointerOfCol(a), InputSize.RowCount, 1);
        ColVector_b.ForceShare(InputMatrix.GetElementPointerOfCol(b), InputSize.RowCount, 1);

        return CompareFunction(ColVector_a, ColVector_b);
    });

    return ColIndexList;
}


template<typename ElementType>
DenseMatrix<int_max> FindUniqueElementInMatrix(const DenseMatrix<ElementType>& InputMatrix)
{
    DenseMatrix<int_max> LinearIndexList_unique;

    if (InputMatrix.IsEmpty() == true)
    {
        return LinearIndexList_unique;
    }

    auto LinearIndexList_sort = InputMatrix.Sort([](const ElementType& a, const ElementType& b){ return a < b; });

    LinearIndexList_unique.ReserveCapacity(LinearIndexList_sort.GetElementCount());

    LinearIndexList_unique.Append(LinearIndexList_sort[0]);

    auto Element_prev = InputMatrix[LinearIndexList_sort[0]];

    for (int_max k = 1; k < LinearIndexList_sort.GetElementCount(); ++k)
    {
        auto Element = InputMatrix[LinearIndexList_sort[k]];

        if (Element != Element_prev)
        {
            LinearIndexList_unique.Append(LinearIndexList_sort[k]);

            Element_prev = Element;
        }
    }

    return LinearIndexList_unique;
}


template<typename ElementType, typename SpecialCompareFunctionType>
DenseMatrix<int_max> FindUniqueElementInMatrix(const DenseMatrix<ElementType>& InputMatrix, SpecialCompareFunctionType SpecialCompareFunction)
{
    // Result = SpecialCompareFunction(const ElementType& a, const ElementType& b)
    // Result: integer type
    // Result: -1: a < b
    // Result: 0:  a = b 
    // Result: 1:  a > b
    //

    DenseMatrix<int_max> LinearIndexList_unique;

    if (InputMatrix.IsEmpty() == true)
    {
        return LinearIndexList_unique;
    }

    auto LinearIndexList_sort = InputMatrix.Sort([&](const ElementType& a, const ElementType& b)
    { 
        auto Result = SpecialCompareFunction(a, b);
        return (Result < 0);
    });

    LinearIndexList_unique.ReserveCapacity(LinearIndexList_sort.GetElementCount());

    LinearIndexList_unique.Append(LinearIndexList_sort[0]);

    auto Element_prev = InputMatrix[LinearIndexList_sort[0]];

    for (int_max k = 1; k < LinearIndexList_sort.GetElementCount(); ++k)
    {
        auto Element = InputMatrix[LinearIndexList_sort[k]];

        if (SpecialCompareFunction(Element, Element_prev) == 0)
        {
            LinearIndexList_unique.Append(LinearIndexList_sort[k]);

            Element_prev = Element;
        }
    }

    return LinearIndexList_unique;
}


template<typename ElementType>
DenseMatrix<int_max> FindUniqueColInMatrix(const DenseMatrix<ElementType>& InputMatrix)
{
    DenseMatrix<int_max> ColIndexList_unique;

    if (InputMatrix.IsEmpty() == true)
    {
        return ColIndexList_unique;
    }

    auto TempFunction_CompareCol = [](const DenseMatrix<ElementType>& VectorA, const DenseMatrix<ElementType>& VectorB)
    {
        auto RowCount = VectorA.GetElementCount();

        for (int_max k = 0; k < RowCount; ++k)
        {
            if (VectorA[k] < VectorB[k])
            {
                return true;
            }
            else if (VectorA[k] > VectorB[k])
            {
                return false;
            }
        }

        // default value must be false, true will cause assert failure
        return false;
    };

    auto ColIndexList_sort = SortColInMatrix(InputMatrix, TempFunction_CompareCol);

    ColIndexList_unique.ReserveCapacity(ColIndexList_sort.GetElementCount());

    ColIndexList_unique.Append(ColIndexList_sort[0]);

    DenseMatrix<ElementType> Vector_prev, Vector_k;

    Vector_prev.ForceShare(InputMatrix.GetElementPointerOfCol(ColIndexList_sort[0]), InputMatrix.GetRowCount(), 1);

    for (int_max k = 1; k < ColIndexList_sort.GetElementCount(); ++k)
    {
        Vector_k.ForceShare(InputMatrix.GetElementPointerOfCol(ColIndexList_sort[k]), InputMatrix.GetRowCount(), 1);

        if (TempFunction_CompareCol(Vector_prev, Vector_k) == true)
        {
            ColIndexList_unique.Append(ColIndexList_sort[k]);

            Vector_prev.ForceShare(InputMatrix.GetElementPointerOfCol(ColIndexList_sort[k]), InputMatrix.GetRowCount(), 1);
        }
    }

    return ColIndexList_unique;
}


template<typename ElementType, typename SpecialCompareFunctionType>
DenseMatrix<int_max> FindUniqueColInMatrix(const DenseMatrix<ElementType>& InputMatrix, SpecialCompareFunctionType SpecialCompareFunction)
{
    // Result = SpecialCompareFunction(const DenseMatrix<ElementType>& a, const const DenseMatrix<ElementType>& b)
    // Result: integer type
    // Result: -1: a < b
    // Result: 0:  a = b 
    // Result: 1:  a > b
    //

    DenseMatrix<int_max> ColIndexList_unique;

    if (InputMatrix.IsEmpty() == true)
    {
        return ColIndexList_unique;
    }

    auto ColIndexList_sort = SortColInMatrix(InputMatrix, [&](const DenseMatrix<ElementType>& VectorA, const DenseMatrix<ElementType>& VectorB)
    {
        auto Result = SpecialCompareFunction(VectorA, VectorB);
        return (Result < 0);
    });

    ColIndexList_unique.ReserveCapacity(ColIndexList_sort.GetElementCount());

    ColIndexList_unique.Append(ColIndexList_sort[0]);

    DenseMatrix<ElementType> Vector_prev, Vector_k;

    Vector_prev.ForceShare(InputMatrix.GetElementPointerOfCol(ColIndexList_sort[0]), InputMatrix.GetRowCount(), 1);

    for (int_max k = 1; k < ColIndexList_sort.GetElementCount(); ++k)
    {
        Vector_k.ForceShare(InputMatrix.GetElementPointerOfCol(ColIndexList_sort[k]), InputMatrix.GetRowCount(), 1);

        if (SpecialCompareFunction(Vector_prev, Vector_k) != 0)
        {
            ColIndexList_unique.Append(ColIndexList_sort[k]);

            Vector_prev.ForceShare(InputMatrix.GetElementPointerOfCol(ColIndexList_sort[k]), InputMatrix.GetRowCount(), 1);
        }
    }

    return ColIndexList_unique;
}

//================================================================================================================================//

template<typename ElementType>
inline
ElementType MatrixMean(const DenseMatrix<ElementType>& InputMatrix)
{
    auto ElementNumber = InputMatrix.GetElementCount();

    if (ElementNumber <= 0)
    {
        MDK_Error("Input is an empty Matrix @ mdkLinearAlgebra_DenseMatrix MatrixMean(InputMatrix)")
        return InputMatrix.GetErrorElement();
    }

    auto RawPointer = InputMatrix.GetElementPointer();

    ElementType value = RawPointer[0];

    for (auto Ptr = RawPointer + 1; Ptr < RawPointer + ElementNumber; ++Ptr)
    {
        value += Ptr[0];
    }

    value /= ElementType(ElementNumber);

    return value;
}


template<typename ElementType>
inline
DenseMatrix<ElementType> MatrixMeanOfEachCol(const DenseMatrix<ElementType>& InputMatrix)
{
    DenseMatrix<ElementType> tempMatrix;

    auto InputSize = InputMatrix.GetSize();

    if (InputSize.RowCount <= 0)
    {
        MDK_Error("Input is empty Matrix @ mdkLinearAlgebra_DenseMatrix MatrixMeanOfEachCol(InputMatrix)")

        return tempMatrix;
    }

    tempMatrix.Resize(1, InputSize.ColCount);

    auto tempRawPointer = tempMatrix.GetElementPointer();

    auto RawPointer = InputMatrix.GetElementPointer();

    for (int_max j = 0; j < InputSize.ColCount; ++j)
    {
        auto value = RawPointer[0];
        for (int_max i = 1; i < InputSize.RowCount; ++i)
        {
            value += RawPointer[i];
        }

        tempRawPointer[j] = value / ElementType(InputSize.RowCount);

        RawPointer += InputSize.RowCount;
    }

    return tempMatrix;
}


template<typename ElementType>
inline
DenseMatrix<ElementType> MatrixMeanOfEachRow(const DenseMatrix<ElementType>& InputMatrix)
{
    DenseMatrix<ElementType> tempMatrix;

    auto InputSize = InputMatrix.GetSize();

    if (InputSize.RowCount <= 0)
    {
        MDK_Error("Input is empty Matrix @ mdkLinearAlgebra_DenseMatrix MatrixMeanOfEachRow(InputMatrix)")

        return tempMatrix;
    }

    tempMatrix.Resize(InputSize.RowCount, 1);

    auto tempRawPointer = tempMatrix.GetElementPointer();

    auto RawPointer = InputMatrix.GetElementPointer();

    for (int_max i = 0; i < InputSize.RowCount; ++i)
    {
        auto value = RawPointer[i];

        int_max Index = InputSize.RowCount;

        for (int_max j = 1; j < InputSize.ColCount; ++j)
        {
            value += RawPointer[Index + i];

            Index += InputSize.RowCount;
        }

        tempRawPointer[i] = value / ElementType(InputSize.ColCount);
    }

    return tempMatrix;
}


template<typename ElementType>
inline
ElementType MatrixStd(const DenseMatrix<ElementType>& InputMatrix) // standard deviation
{
	auto ElementNumber = InputMatrix.GetElementCount();

	if (ElementNumber <= 0)
	{
		MDK_Error("Input is an empty Matrix @ mdkLinearAlgebra_DenseMatrix MatrixStd(InputMatrix)")
		return InputMatrix.GetErrorElement();
	}

	auto MeanValue = MatrixMean(InputMatrix);

	auto RawPointer = InputMatrix.GetElementPointer();

	auto Value = ElementType(0);
	for (int_max i = 0; i < ElementNumber; ++i)
	{
		auto temp = RawPointer[i] - MeanValue;
		Value += temp*temp;
	}
	Value /= ElementType(ElementNumber);
	Value = std::sqrt(Value);

	return Value;
}

template<typename ElementType>
inline
int_max FindLinearIndexOfMaxInMatrix(const DenseMatrix<ElementType>& InputMatrix)
{
    auto Input_ElementNumber = InputMatrix.GetElementCount();

    if (Input_ElementNumber <= 0)
    {
        MDK_Error("Input is empty Matrix @ mdkLinearAlgebra_DenseMatrix FindLinearIndexOfMaxInMatrix(InputMatrix)")

        return -1;
    }

    auto InputPtr = InputMatrix.GetElementPointer();

    ElementType MaxValue = InputPtr[0];

    int_max LinearIndex = 0;

    for (int_max k = 1; k < Input_ElementNumber; ++k)
    {
        if (InputPtr[k] > MaxValue)
        {
            MaxValue = InputPtr[k];
            LinearIndex = k;
        }
    }

    return LinearIndex;
}


template<typename ElementType>
inline
ElementType MatrixMax(const DenseMatrix<ElementType>& InputMatrix)
{   
    if (InputMatrix.IsEmpty() == true)
    { 
        MDK_Error("Input is empty Matrix @ mdkLinearAlgebra_DenseMatrix MatrixMax(InputMatrix)")
        return InputMatrix.GetErrorElement();
    }

    int_max LinearIndex = FindLinearIndexOfMaxInMatrix(InputMatrix);

    return InputMatrix[LinearIndex];
}


template<typename ElementType>
inline
DenseMatrix<ElementType> MatrixMaxOfEachCol(const DenseMatrix<ElementType>& InputMatrix)
{
    DenseMatrix<ElementType> tempMatrix;

    auto InputSize = InputMatrix.GetSize();

    if (InputSize.RowCount <= 0)
    {
        MDK_Error("Input is empty Matrix @ mdkLinearAlgebra_DenseMatrix MatrixMaxOfEachCol(InputMatrix)")

        return tempMatrix;
    }

    tempMatrix.Resize(1, InputSize.ColCount);

    auto tempRawPointer = tempMatrix.GetElementPointer();

    auto RawPointer = InputMatrix.GetElementPointer();

    for (int_max j = 0; j < InputSize.ColCount; ++j)
    {
        auto value = RawPointer[0];

        ++RawPointer;

        for (int_max i = 1; i < InputSize.RowCount; ++i)
        {
            value = std::max(value, RawPointer[0]);

            ++RawPointer;
        }

        tempRawPointer[j] = value;
    }

    return tempMatrix;
}


template<typename ElementType>
inline
DenseMatrix<ElementType> MatrixMaxOfEachRow(const DenseMatrix<ElementType>& InputMatrix)
{
    DenseMatrix<ElementType> tempMatrix;

    auto InputSize = InputMatrix.GetSize();

    if (InputSize.RowCount <= 0)
    {
        MDK_Error("Input is empty Matrix @ mdkLinearAlgebra_DenseMatrix MatrixMaxOfEachRow(InputMatrix)")

        return tempMatrix;
    }

    tempMatrix.Resize(InputSize.ColCount, 1);

    auto tempRawPointer = tempMatrix.GetElementPointer();

    auto RawPointer = InputMatrix.GetElementPointer();

    for (int_max i = 0; i < InputSize.RowCount; ++i)
    {
        auto value = RawPointer[i];

        auto Index = InputSize.RowCount;

        for (int_max j = 1; j < InputSize.ColCount; ++j)
        {
            value = std::max(value, RawPointer[Index + i]);

            Index += InputSize.RowCount;
        }

        tempRawPointer[i] = value;
    }

    return tempMatrix;
}


template<typename ElementType>
int_max FindLinearIndexOfMinInMatrix(const DenseMatrix<ElementType>& InputMatrix)
{
    auto Input_ElementNumber = InputMatrix.GetElementCount();

    if (Input_ElementNumber <= 0)
    {
        MDK_Error("Input is empty Matrix @ mdkLinearAlgebra_DenseMatrix FindLinearIndexOfMinInMatrix(InputMatrix)")
        return -1;
    }

    auto InputPtr = InputMatrix.GetElementPointer();

    ElementType MinValue = InputPtr[0];

    int_max LinearIndex = 0;

    for (int_max k = 1; k < Input_ElementNumber; ++k)
    {
        if (InputPtr[k] < MinValue)
        {
            MinValue = InputPtr[k];
            LinearIndex = k;
        }
    }

    return LinearIndex;
}


template<typename ElementType>
inline
ElementType MatrixMin(const DenseMatrix<ElementType>& InputMatrix)
{
    if (InputMatrix.IsEmpty() == true)
    {
        MDK_Error("Input is empty Matrix @ mdkLinearAlgebra_DenseMatrix MatrixMin(InputMatrix)")
        return InputMatrix.GetErrorElement();
    }

    auto LinearIndex = FindLinearIndexOfMinInMatrix(InputMatrix);

    return InputMatrix[LinearIndex];
}


template<typename ElementType>
inline
DenseMatrix<ElementType> MatrixMinOfEachCol(const DenseMatrix<ElementType>& InputMatrix)
{
    DenseMatrix<ElementType> tempMatrix;

    auto InputSize = InputMatrix.GetSize();

    if (InputSize.RowCount <= 0)
    {
        MDK_Error("Input is empty Matrix @ mdkLinearAlgebra_DenseMatrix MatrixMinOfEachCol(InputMatrix)")

        return tempMatrix;
    }

    tempMatrix.Resize(1, InputSize.ColCount);

    auto tempRawPointer = tempMatrix.GetElementPointer();

    auto RawPointer = InputMatrix.GetElementPointer();

    for (int_max j = 0; j < InputSize.ColCount; ++j)
    {
        auto value = RawPointer[0];

        ++RawPointer;

        for (int_max i = 1; i < InputSize.RowCount; ++i)
        {
            value = std::min(value, RawPointer[0]);

            ++RawPointer;
        }

        tempRawPointer[j] = value;
    }

    return tempMatrix;
}


template<typename ElementType>
inline
DenseMatrix<ElementType> MatrixMinOfEachRow(const DenseMatrix<ElementType>& InputMatrix)
{
    DenseMatrix<ElementType> tempMatrix;

    auto InputSize = InputMatrix.GetSize();

    if (InputSize.RowCount <= 0)
    {
        MDK_Error("Input is empty Matrix @ mdkLinearAlgebra_DenseMatrix MatrixMinOfEachRow(InputMatrix)")

        return tempMatrix;
    }

    tempMatrix.Resize(InputSize.ColCount, 1);

    auto tempRawPointer = tempMatrix.GetElementPointer();

    auto RawPointer = InputMatrix.GetElementPointer();

    for (int_max i = 0; i < InputSize.RowCount; ++i)
    {
        auto value = RawPointer[i];

        auto Index = InputSize.RowCount;

        for (int_max j = 1; j < InputSize.ColCount; ++j)
        {
            value = std::min(value, RawPointer[Index + i]);

            Index += InputSize.RowCount;
        }

        tempRawPointer[i] = value;
    }

    return tempMatrix;
}


template<typename ElementType>
inline
ElementType MatrixSum(const DenseMatrix<ElementType>& InputMatrix)
{
    auto Input_ElementNumber = InputMatrix.GetElementCount();

    if (Input_ElementNumber <= 0)
    {
        MDK_Error("Input is empty Matrix @ mdkLinearAlgebra_DenseMatrix MatrixSum(InputMatrix)")
        return InputMatrix.GetErrorElement();
    }

    auto RawPointer = InputMatrix.GetElementPointer();

    ElementType value = RawPointer[0];

    for (auto Ptr = RawPointer + 1; Ptr < RawPointer + Input_ElementNumber; ++Ptr)
    {
        value += Ptr[0];
    }

    return value;
}


template<typename ElementType>
inline
DenseMatrix<ElementType> MatrixSumOfEachCol(const DenseMatrix<ElementType>& InputMatrix)
{
    DenseMatrix<ElementType> tempMatrix;

    auto InputSize = InputMatrix.GetSize();

    if (InputSize.RowCount <= 0)
    {
        MDK_Error("InputMatrix is empty Matrix @ mdkLinearAlgebra_DenseMatrix MatrixSumOfEachCol(InputMatrix)")

        return tempMatrix;
    }

    tempMatrix.Resize(1, InputSize.ColCount);

    auto tempRawPointer = tempMatrix.GetElementPointer();

    auto RawPointer = InputMatrix.GetElementPointer();

    for (int_max j = 0; j < InputSize.ColCount; ++j)
    {
        auto value = RawPointer[0];

        ++RawPointer;

        for (int_max i = 1; i < InputSize.RowCount; ++i)
        {
            value += RawPointer[0];

            ++RawPointer;
        }

        tempRawPointer[j] = value;
    }

    return tempMatrix;
}


template<typename ElementType>
inline
DenseMatrix<ElementType> MatrixSumOfEachRow(const DenseMatrix<ElementType>& InputMatrix)
{
    DenseMatrix<ElementType> tempMatrix;

    auto InputSize = InputMatrix.GetSize();

    if (InputSize.RowCount <= 0)
    {
        MDK_Error("Input is empty Matrix @ mdkLinearAlgebra_DenseMatrix MatrixSumOfEachRow(InputMatrix)")

        return tempMatrix;
    }

    tempMatrix.Resize(InputSize.RowCount, 1);

    auto tempRawPointer = tempMatrix.GetElementPointer();

    auto RawPointer = InputMatrix.GetElementPointer();

    for (int_max i = 0; i < InputSize.RowCount; ++i)
    {
        auto value = RawPointer[i];

        int_max Index = InputSize.RowCount;

        for (int_max j = 1; j < InputSize.ColCount; ++j)
        {
            value += RawPointer[Index + i];

            Index += InputSize.RowCount;
        }

        tempRawPointer[i] = value;
    }

    return tempMatrix;
}


//===================================================================================================================================//

template<typename ElementType>
inline
ElementType MatrixNorm_L1(const DenseMatrix<ElementType>& InputMatrix)
{
    auto ElementNumber = InputMatrix.GetElementCount();

    if (ElementNumber == 0)
    {
        MDK_Error("empty input matrix @ mdkLinearAlgebra_DenseMatrix MatrixNorm_L1(InputMatrix)")
        return InputMatrix.GetErrorElement();
    }

    ElementType Value = ElementType(0);

    auto BeginPointer = InputMatrix.GetElementPointer();

    for (auto Ptr = BeginPointer; Ptr < BeginPointer + ElementNumber; ++Ptr)
    {
        Value += std::abs(Ptr[0]);
    }

    return Value;
}


template<typename ElementType>
inline
ElementType MatrixNorm_L2(const DenseMatrix<ElementType>& InputMatrix)
{
    auto ElementNumber = InputMatrix.GetElementCount();

    if (ElementNumber == 0)
    {
        MDK_Error("empty input matrix @ mdkLinearAlgebra_DenseMatrix MatrixNorm_L2(InputMatrix)")
        return InputMatrix.GetErrorElement();
    }

    auto BeginPointer = InputMatrix.GetElementPointer();

    auto Value = BeginPointer[0] * BeginPointer[0];

    for (auto Ptr = BeginPointer + 1; Ptr < BeginPointer + ElementNumber; ++Ptr)
    {
        Value += Ptr[0] * Ptr[0];
    }

    Value = std::sqrt(Value);

    return Value;
}


template<typename ElementType>
inline 
DenseMatrix<ElementType> MatrixTranspose(const DenseMatrix<ElementType>& InputMatrix)
{
    DenseMatrix<ElementType> tempMatrix;

    auto Size = InputMatrix.GetSize();

    if (Size.RowCount == 0)
    {
        MDK_Error("InputMatrix is empty @ mdkLinearAlgebra_DenseMatrix MatrixTranspose(InputMatrix)")

        return tempMatrix;
    }

    if (Size.ColCount == 1)
    {
        tempMatrix.Copy(InputMatrix.GetElementPointer(), 1, Size.RowCount);

        return tempMatrix;
    }

    if (Size.RowCount == 1)
    {
        tempMatrix.Copy(InputMatrix.GetElementPointer(), Size.ColCount, 1);

        return tempMatrix;
    }

    tempMatrix.FastResize(Size.ColCount, Size.RowCount);

    auto tempRawPointer = tempMatrix.GetElementPointer();

    auto RawPointer = InputMatrix.GetElementPointer();

    for (int_max i = 0; i < Size.RowCount; ++i)
    {
        int_max Index = 0;

        for (int_max j = 0; j < Size.ColCount; ++j)
        {
            tempRawPointer[0] = RawPointer[Index + i];

            Index += Size.RowCount;

            ++tempRawPointer;
        }
    }

    return tempMatrix;
}


template<typename ElementType>
inline 
void MatrixTransposeInPlace(DenseMatrix<ElementType>& InputMatrix)
{
    auto Size = InputMatrix.GetSize();

    if (Size.RowCount == 0)
    {
        return;
    }

    if (Size.ColCount == 1 || Size.RowCount == 1)
    {
        InputMatrix.Reshape(Size.ColCount, Size.RowCount);
        return;
    }

    //--------------------- call armadillo

    arma::Mat<ElementType> A(InputMatrix.GetElementPointer(), arma::uword(InputMatrix.GetRowCount()), arma::uword(InputMatrix.GetColCount()), false);

    arma::inplace_trans(A);

    InputMatrix.Reshape(Size.ColCount, Size.RowCount);
}


template<typename ElementType>
inline 
int_max MatrixRank(const DenseMatrix<ElementType>& InputMatrix)
{
    auto Size = InputMatrix.GetSize();

    if (Size.RowCount == 0)
    {
        MDK_Error("InputMatrix is empty  @ mdkLinearAlgebra_DenseMatrix MatrixRank(InputMatrix)")
        return 0;
    }

    auto ptrData = const_cast<ElementType*>(InputMatrix.GetElementPointer());

    // call Armadillo 

    arma::Mat<ElementType> tempMat(ptrData, arma::uword(Size.RowCount), arma::uword(Size.ColCount), false);

    int_max value = arma::rank(tempMat);

    return value;
}


template<typename ElementType>
inline
ElementType MatrixDeterminant(const DenseMatrix<ElementType>& InputMatrix)
{
	if (InputMatrix.IsEmpty() == true)
	{
		return ElementType(0);
	}

	if (InputMatrix.IsVector() == true)
	{
		auto Det = ElementType(0);
		for (auto Ptr = InputMatrix.begin(); Ptr != InputMatrix.end(); ++Ptr)
		{
			Det *= Ptr[0];
		}
		return Det;
	}

	// call Armadillo 
	auto Size = InputMatrix.GetSize();
	auto ptrData = const_cast<ElementType*>(InputMatrix.GetElementPointer());
	arma::Mat<ElementType> tempMat(ptrData, arma::uword(Size.RowCount), arma::uword(Size.ColCount), false);
	auto Det = arma::det(tempMat);

	return Det;
}



template<typename ElementType>
inline
DenseMatrix<ElementType> MatrixInverse(const DenseMatrix<ElementType>& InputMatrix)
{
    DenseMatrix<ElementType> OutputMatrix;

    auto Size = InputMatrix.GetSize();

    if (Size.RowCount == 0)
    {
		MDK_Warning("InputMatrix is empty matrix @ mdkLinearAlgebra_DenseMatrix MatrixInverse(InputMatrix)")
        return OutputMatrix;
    }

    if (Size.RowCount != Size.ColCount)
    {
        MDK_Error("InputMatrix is not square @ mdkLinearAlgebra_DenseMatrix MatrixInverse(InputMatrix)")
        return OutputMatrix;
    }

	OutputMatrix.FastResize(Size.RowCount, Size.ColCount);

    auto ptrData = const_cast<ElementType*>(InputMatrix.GetElementPointer());

    // call Armadillo 
    arma::Mat<ElementType> tempMat(ptrData, arma::uword(Size.RowCount), arma::uword(Size.ColCount), false);
	arma::Mat<ElementType> tempInv(OutputMatrix.GetElementPointer(), arma::uword(Size.RowCount), arma::uword(Size.ColCount), false);
    tempInv = arma::inv(tempMat);

	return OutputMatrix;
}


template<typename ElementType>
inline
DenseMatrix<ElementType> MatrixPseudoInverse(const DenseMatrix<ElementType>& InputMatrix)
{
	DenseMatrix<ElementType> OutputMatrix;

	auto Size = InputMatrix.GetSize();

	if (Size.RowCount == 0)
	{
		MDK_Warning("InputMatrix is empty matrix @ mdkLinearAlgebra_DenseMatrix MatrixPseudoInverse(InputMatrix)")
	    return OutputMatrix;
	}

	OutputMatrix.FastResize(Size.ColCount, Size.RowCount);// switch size

	auto ptrData = const_cast<ElementType*>(InputMatrix.GetElementPointer());

	// call Armadillo 
	auto tolerance = ElementType(std::max(Size.RowCount, Size.ColCount))*InputMatrix.L1Norm()*std::numeric_limits<ElementType>::epsilon();
	arma::Mat<ElementType> tempMat(ptrData, arma::uword(Size.RowCount), arma::uword(Size.ColCount), false);
	arma::Mat<ElementType> tempInv(OutputMatrix.GetElementPointer(), arma::uword(Size.RowCount), arma::uword(Size.ColCount), false);	
	bool Flag = arma::pinv(tempInv, tempMat, tolerance, "std"); // do not use "dc", it has problem
	if (Flag == false)
	{
		MDK_Error("Armadillo pinv failed @ @ mdkLinearAlgebra_DenseMatrix MatrixPseudoInverse(InputMatrix)")
	}
	return OutputMatrix;
}


template<typename ElementType>
inline 
DenseMatrix<ElementType> SolveMatrixLinearEquation(const DenseMatrix<ElementType>& MatrixA, const DenseMatrix<ElementType>& MatrixB)
{	// A*X=B
	DenseMatrix<ElementType> MatrixX;

	auto SizeA = MatrixA.GetSize();
	auto SizeB = MatrixB.GetSize();

	if (SizeA.RowCount != SizeB.RowCount)
	{
		MDK_Error("A RowCount != B RowCount @ mdkLinearAlgebra_DenseMatrix SolveMatrixLinearEquation(MatrixA, MatrixB)")
 	    return MatrixX;
	}

	if (SizeA.RowCount == 0)
	{
		MDK_Warning("MatrixA is empty @ mdkLinearAlgebra_DenseMatrix SolveMatrixLinearEquation(MatrixA, MatrixB)")
	    return MatrixX;
	}

	MatrixX.Resize(SizeA.ColCount, SizeB.ColCount);

	auto ptrX = const_cast<ElementType*>(MatrixX.GetElementPointer());
	auto ptrA = const_cast<ElementType*>(MatrixA.GetElementPointer());
	auto ptrB = const_cast<ElementType*>(MatrixB.GetElementPointer());

	// call Armadillo 
	//arma::Mat<ElementType> tempX(ptrX, arma::uword(SizeA.ColCount), arma::uword(SizeB.ColCount), false);
	//arma::Mat<ElementType> tempA(ptrA, arma::uword(SizeA.RowCount), arma::uword(SizeA.ColCount), false);
	//arma::Mat<ElementType> tempB(ptrB, arma::uword(SizeB.RowCount), arma::uword(SizeB.ColCount), false);
	//auto Flag = arma::solve(tempX, tempA, tempB);
	//if (Flag == false)
	//{
	//	MDK_Error("No solution @ mdkLinearAlgebra_DenseMatrix SolveMatrixLinearEquation(MatrixA, MatrixB)")
	//}

	// call eigen: SVD
	Eigen::Map<Eigen::Matrix<ElementType, Eigen::Dynamic, Eigen::Dynamic>> A(ptrA, SizeA.RowCount, SizeA.ColCount);
	Eigen::Map<Eigen::Matrix<ElementType, Eigen::Dynamic, Eigen::Dynamic>> B(ptrB, SizeB.RowCount, SizeB.ColCount);
	Eigen::Map<Eigen::Matrix<ElementType, Eigen::Dynamic, Eigen::Dynamic>> X(ptrX, SizeA.ColCount, SizeB.ColCount);
	try
	{
		X = A.jacobiSvd(Eigen::ComputeThinU | Eigen::ComputeThinV).solve(B);
	}
	catch (...)
	{
		MDK_Error("Eigen-Lib error @ mdkLinearAlgebra_DenseMatrix SolveMatrixLinearEquation(MatrixA, MatrixB)")
	}
	
	return MatrixX;
}


template<typename ElementType>
inline 
DenseMatrixEigenResult<std::complex<ElementType>> NonSymmetricRealMatrixEigen(const DenseMatrix<ElementType>& InputMatrix)
{
    DenseMatrixEigenResult<std::complex<ElementType>> Result;

    auto Size = InputMatrix.GetSize();

    if (Size.RowCount == 0)
    {
        MDK_Error("InputMatrix is empty matrix @ mdkLinearAlgebra_DenseMatrix NonSymmetricRealMatrixEigen(InputMatrix)")

        return Result;
    }

    if (Size.RowCount != Size.ColCount)
    {
        MDK_Error("InputMatrix is not square @ mdkLinearAlgebra_DenseMatrix NonSymmetricRealMatrixEigen(InputMatrix)")

        return Result;
    }

    Result.EigenVector.FastResize(Size.RowCount, Size.RowCount);
    Result.EigenValue.FastResize(Size.RowCount, 1);
    auto ptrData = const_cast<ElementType*>(InputMatrix.GetElementPointer());

    // call Armadillo 
    arma::Mat<ElementType> tempMat(ptrData, arma::uword(Size.RowCount), arma::uword(Size.RowCount), false);
    arma::Mat<std::complex<ElementType>> tempEigenVector(Result.EigenVector.GetElementPointer(), arma::uword(Size.RowCount), arma::uword(Size.RowCount), false);
    arma::Col<std::complex<ElementType>> tempEigenValue(Result.EigenValue.GetElementPointer(), arma::uword(Size.RowCount), false);
    arma::eig_gen(tempEigenValue, tempEigenVector, tempMat);

    return Result;
}


template<typename ElementType>
inline 
DenseMatrixEigenResult<ElementType> RealSymmetricMatrixEigen(const DenseMatrix<ElementType>& InputMatrix, bool CheckIfSymmetric = false)
{
    DenseMatrixEigenResult<ElementType> Result;

    auto Size = InputMatrix.GetSize();

    if (Size.RowCount == 0)
    {
        MDK_Error("Matrix is empty matrix @ mdkLinearAlgebra_DenseMatrix MatrixEigen(InputMatrix)")

        return Result;
    }

    if (Size.RowCount != Size.ColCount)
    {
        MDK_Error("Matrix is not square @ mdkLinearAlgebra_DenseMatrix MatrixEigen(InputMatrix)")

        return Result;
    }

    if (CheckIfSymmetric == true)
    {
        DenseMatrix<ElementType> tempMatrix_2 = InputMatrix - InputMatrix.Transpose();

        tempMatrix_2.ElementOperationInPlace("abs");

        double tempsum = double(tempMatrix_2.Sum());

        if (std::abs(tempsum) > EPS(tempsum))
        {
            MDK_Error("Matrix is not Symmetric, try to generate result @ mdkLinearAlgebra_DenseMatrix MatrixEigen(InputMatrix)")
        }
    }
  
    Result.EigenVector.FastResize(Size.RowCount, Size.RowCount);

    Result.EigenValue.FastResize(Size.RowCount, 1);

    auto ptrData = const_cast<ElementType*>(InputMatrix.GetElementPointer());

    // call Armadillo 

    arma::Mat<ElementType> tempMat(ptrData, arma::uword(Size.RowCount), arma::uword(Size.RowCount), false);

    arma::Mat<ElementType> tempEigenVector(Result.EigenVector.GetElementPointer(), arma::uword(Size.RowCount), arma::uword(Size.RowCount), false);

    arma::Col<ElementType> tempEigenValue(Result.EigenValue.GetElementPointer(), arma::uword(Size.RowCount), false);

    arma::eig_sym(tempEigenValue, tempEigenVector, tempMat);

    return Result;
}


template<typename ElementType>
inline
DenseMatrixPCAResult<ElementType> MatrixPCA(const DenseMatrix<ElementType>& InputMatrix)
{
    DenseMatrixPCAResult<ElementType> PCAResult;

    auto Size = InputMatrix.GetSize();

    if (Size.ColCount <= 1)
    {
        MDK_Error("ColCount <= 1, return empty PCAResult @ mdkLinearAlgebra_DenseMatrix MatrixPCA(...)")
        return PCAResult;
    }

    auto MeanCol = InputMatrix.SumToCol();
    MeanCol /= Size.ColCount;

    DenseMatrix<ElementType> CovarianceMatrix(Size.RowCount, Size.RowCount);

    CovarianceMatrix.Fill(0);

    // reference only
    //for (int_max i = 0; i < Size.ColCount; ++i)
    //{
    //    DenseMatrix<ElementType> tempCol = InputMatrix(ALL, { i }) - MeanCol;
       
    //    CovarianceMatrix += tempCol * tempCol.Transpose();
    //}

    DenseMatrix<ElementType> tempMatrix(Size.RowCount, Size.RowCount);

    DenseMatrix<ElementType> tempCol(Size.RowCount, 1);

    DenseMatrix<ElementType> tempColTranspose(1, Size.RowCount);

    for (int_max i = 0; i < Size.ColCount; ++i)
    {
        InputMatrix.GetCol(i, tempCol);

        MatrixSubtract(tempCol, tempCol, MeanCol);

        tempColTranspose = tempCol.Transpose();

        MatrixMultiply(tempMatrix, tempCol, tempColTranspose);

        CovarianceMatrix += tempMatrix;
    }

    CovarianceMatrix /= Size.ColCount;

    auto EigenResult = SymmetricRealMatrixEigen(CovarianceMatrix);

    //--------------------------------------------------------------//

    PCAResult.Mean = std::move(MeanCol);

    PCAResult.EigenVector = std::move(EigenResult.EigenVector);

    PCAResult.EigenValue = std::move(EigenResult.EigenValue);

    return PCAResult;
}


template<typename ElementType>
inline
DenseMatrixSVDResult<ElementType> MatrixSVD(const DenseMatrix<ElementType>& InputMatrix)
{
    DenseMatrixSVDResult<ElementType> Result;

    auto Size = InputMatrix.GetSize();

    if (Size.RowCount == 0)
    {
        MDK_Error("Matrix is empty  @ mdkLinearAlgebra_DenseMatrix MatrixSVD(InputMatrix)")
        return Result;
    }

    Result.U.FastResize(Size.RowCount, Size.ColCount);
    Result.S.FastResize(Size.RowCount, Size.ColCount);
    Result.V.FastResize(Size.RowCount, Size.ColCount);

    auto ptrData = const_cast<ElementType*>(InputMatrix.GetElementPointer());

    // call Armadillo 

    arma::Mat<ElementType> X(ptrData, arma::uword(Size.RowCount), arma::uword(Size.ColCount), false);

    arma::Mat<ElementType> U(Result.U.GetElementPointer(), arma::uword(Size.RowCount), arma::uword(Size.ColCount), false);
    arma::Col<ElementType> S;
    arma::Mat<ElementType> V(Result.V.GetElementPointer(), arma::uword(Size.RowCount), arma::uword(Size.ColCount), false);

    arma::svd(U, S, V, X);

    Result.S.SetDiagonal(S.memptr());

    return Result;
}

}//end namespace mdk

#endif
