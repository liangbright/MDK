#ifndef __mdkLinearAlgebra_Function_DenseMatrix_Part_2_hpp
#define __mdkLinearAlgebra_Function_DenseMatrix_Part_2_hpp


namespace mdk
{

//================================================================================================================================//

template<typename ElementType, typename MatchFunctionType>
DenseMatrix<int_max> FindElementInMatrix(const DenseMatrix<ElementType>& InputMatrix, MatchFunctionType MatchFunction)
{
    return FindElementInMatrix(InputMatrix, InputMatrix.GetElementNumber(), 0, InputMatrix.GetElementNumber()-1, MatchFunction);
}


template<typename ElementType, typename MatchFunctionType>
DenseMatrix<int_max> FindElementInMatrix(const DenseMatrix<ElementType>& InputMatrix, int_max MaxOutputNumber, MatchFunctionType MatchFunction)
{
    return FindElementInMatrix(InputMatrix, MaxOutputNumber, 0, InputMatrix.GetElementNumber()-1, MatchFunction);
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

    auto InputElementNumber = InputMatrix.GetElementNumber();

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

            auto CurrentNumber = LinearIndexList.GetElementNumber();

            if (CurrentNumber == MaxOutputNumber)
            {
                break;
            }
        }
    }

    return LinearIndexList;
}


template<typename ElementType, typename MatchFunctionType>
int_max FindElementInMatrix(const DenseMatrix<ElementType>& InputMatrix, const std::string& first_or_last, MatchFunctionType MatchFunction)
{
    int_max Index_output = -1;

	if (first_or_last == "first")
	{
		for (int_max i = 0; i < InputMatrix.GetElementNumber(); ++i)
		{
			if (MatchFunction(InputMatrix[i]) == true)
			{
				Index_output = i;
				break;
			}
		}
	}
	else if (first_or_last == "last")
	{
		for (int_max i = InputMatrix.GetElementNumber()-1; i >= 0; --i)
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
DenseMatrix<int_max> ExactMatchElementInMatrix(const DenseMatrix<ElementType>& InputMatrix, const std::string& first_or_last, const ElementType& InputElement)
{
	return FindElementInMatrix(*this, first_or_last, InputElement, [&](const ElementType& Element){return Element == InputElement; });
}


template<typename ElementType, typename MatchFunctionType>
DenseMatrix<int_max> FindColInMatrix(const DenseMatrix<ElementType>& InputMatrix, MatchFunctionType MatchFunction)
{
    return FindColInMatrix(InputMatrix, InputMatrix.GetColNumber(), 0, InputMatrix.GetColNumber()-1, MatchFunction);
}


template<typename ElementType, typename MatchFunctionType>
DenseMatrix<int_max> FindColInMatrix(const DenseMatrix<ElementType>& InputMatrix, int_max MaxOutputColNumber, MatchFunctionType MatchFunction)
{
    return FindColInMatrix(InputMatrix, MaxOutputColNumber, 0, InputMatrix.GetColNumber()-1, MatchFunction);
}


template<typename ElementType, typename MatchFunctionType>
DenseMatrix<int_max> FindColInMatrix(const DenseMatrix<ElementType>& InputMatrix, int_max MaxOutputColNumber, 
                                     int_max ColIndex_start, int_max ColIndex_end, MatchFunctionType MatchFunction)
{
    DenseMatrix<int_max> ColIndexList;

    if (MaxOutputColNumber == 0)
    {
        return ColIndexList;
    }

    auto InputSize = InputMatrix.GetSize();

    if (MaxOutputColNumber < 0 || MaxOutputColNumber > InputSize.ColNumber)
    {
        MDK_Error("MaxOutputColNumber is invalid @ mdkLinearAlgebra_DenseMatrix FindColInMatrix(...)")
        return ColIndexList;
    }

    if (ColIndex_start < 0 || ColIndex_start >= InputSize.ColNumber || ColIndex_start > ColIndex_end)
    {
        MDK_Error("ColIndex_start is invalid @ mdkLinearAlgebra_DenseMatrix FindColInMatrix(...)")
        return ColIndexList;
    }

    if (ColIndex_end < 0 || ColIndex_end >= InputSize.ColNumber)
    {
        MDK_Error("ColIndex_end is invalid @ mdkLinearAlgebra_DenseMatrix FindColInMatrix(...)")
        return ColIndexList;
    }

    if (InputSize.ColNumber == 0)
    {
        return ColIndexList;
    }

    if (ColIndex_start == ColIndex_end)
    {
        ColIndexList.Append(ColIndex_start);
        return ColIndexList;
    }

    ColIndexList.ReserveCapacity(MaxOutputColNumber);

    DenseMatrix<ElementType> ColVector;

	if (ColIndex_start < ColIndex_end)
	{
		for (int_max i = ColIndex_start; i <= ColIndex_end; ++i)
		{
			DenseMatrix<ElementType> ColVector;
			ColVector.ForceShare(InputMatrix.GetElementPointerOfCol(i), InputSize.RowNumber, 1);

			if (MatchFunction(ColVector) == true)
			{
				ColIndexList.Append(i);

				auto CurrentNumber = ColIndexList.GetElementNumber();

				if (CurrentNumber == MaxOutputColNumber)
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
			ColVector.ForceShare(InputMatrix.GetElementPointerOfCol(i), InputSize.RowNumber, 1);

			if (MatchFunction(ColVector) == true)
			{
				ColIndexList.Append(i);

				auto CurrentNumber = ColIndexList.GetElementNumber();

				if (CurrentNumber == MaxOutputColNumber)
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
		for (int_max i = 0; i <= InputSize.ColNumber - 1; ++i)
		{
			DenseMatrix<ElementType> ColVector;
			ColVector.ForceShare(InputMatrix.GetElementPointerOfCol(i), InputSize.RowNumber, 1);

			if (MatchFunction(ColVector) == true)
			{
				ColIndex_output = i;
				break;
			}
		}
	}
	else if (first_or_last == "last")
	{
		for (int_max i = InputSize.ColNumber - 1; i >= 0;  --i)
		{
			DenseMatrix<ElementType> ColVector;
			ColVector.ForceShare(InputMatrix.GetElementPointerOfCol(i), InputSize.RowNumber, 1);

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
    return SortColInMatrix(InputMatrix, 0, InputMatrix.GetColNumber()-1, CompareFunction);
}


template<typename ElementType, typename CompareFunctionType>
DenseMatrix<int_max> SortColInMatrix(const DenseMatrix<ElementType>& InputMatrix,
                                     int_max ColIndex_start, int_max ColIndex_end, CompareFunctionType CompareFunction)
{
    DenseMatrix<int_max> ColIndexList;

    auto InputSize = InputMatrix.GetSize();

    if (ColIndex_start < 0 || ColIndex_start >= InputSize.ColNumber || ColIndex_start > ColIndex_end)
    {
        MDK_Error("ColIndex_start is invalid @ mdkLinearAlgebra_DenseMatrix SortColInMatrix(...)")
        return ColIndexList;
    }

    if (ColIndex_end < 0 || ColIndex_end >= InputSize.ColNumber)
    {
        MDK_Error("ColIndex_end is invalid @ mdkLinearAlgebra_DenseMatrix SortColInMatrix(...)")
        return ColIndexList;
    }

    if (InputSize.ColNumber == 0)
    {
        return ColIndexList;
    }

    if (ColIndex_start == ColIndex_end)
    {
        ColIndexList.AppendCol({ ColIndex_start });
        return ColIndexList;
    }

    ColIndexList.FastResize(1, InputSize.ColNumber);

    for (int_max i = ColIndex_start; i <= ColIndex_end; ++i)
    {
        ColIndexList[i] = i;
    }

    DenseMatrix<ElementType> ColVector_a, ColVector_b;

    std::sort(ColIndexList.begin(), ColIndexList.end(), [&](int_max a, int_max b)
    {
        ColVector_a.ForceShare(InputMatrix.GetElementPointerOfCol(a), InputSize.RowNumber, 1);
        ColVector_b.ForceShare(InputMatrix.GetElementPointerOfCol(b), InputSize.RowNumber, 1);

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

    LinearIndexList_unique.ReserveCapacity(LinearIndexList_sort.GetElementNumber());

    LinearIndexList_unique.Append(LinearIndexList_sort[0]);

    auto Element_prev = InputMatrix[LinearIndexList_sort[0]];

    for (int_max k = 1; k < LinearIndexList_sort.GetElementNumber(); ++k)
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

    LinearIndexList_unique.ReserveCapacity(LinearIndexList_sort.GetElementNumber());

    LinearIndexList_unique.Append(LinearIndexList_sort[0]);

    auto Element_prev = InputMatrix[LinearIndexList_sort[0]];

    for (int_max k = 1; k < LinearIndexList_sort.GetElementNumber(); ++k)
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
        auto RowNumber = VectorA.GetElementNumber();

        for (int_max k = 0; k < RowNumber; ++k)
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

    ColIndexList_unique.ReserveCapacity(ColIndexList_sort.GetElementNumber());

    ColIndexList_unique.Append(ColIndexList_sort[0]);

    DenseMatrix<ElementType> Vector_prev, Vector_k;

    Vector_prev.ForceShare(InputMatrix.GetElementPointerOfCol(ColIndexList_sort[0]), InputMatrix.GetRowNumber(), 1);

    for (int_max k = 1; k < ColIndexList_sort.GetElementNumber(); ++k)
    {
        Vector_k.ForceShare(InputMatrix.GetElementPointerOfCol(ColIndexList_sort[k]), InputMatrix.GetRowNumber(), 1);

        if (TempFunction_CompareCol(Vector_prev, Vector_k) == true)
        {
            ColIndexList_unique.Append(ColIndexList_sort[k]);

            Vector_prev.ForceShare(InputMatrix.GetElementPointerOfCol(ColIndexList_sort[k]), InputMatrix.GetRowNumber(), 1);
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

    ColIndexList_unique.ReserveCapacity(ColIndexList_sort.GetElementNumber());

    ColIndexList_unique.Append(ColIndexList_sort[0]);

    DenseMatrix<ElementType> Vector_prev, Vector_k;

    Vector_prev.ForceShare(InputMatrix.GetElementPointerOfCol(ColIndexList_sort[0]), InputMatrix.GetRowNumber(), 1);

    for (int_max k = 1; k < ColIndexList_sort.GetElementNumber(); ++k)
    {
        Vector_k.ForceShare(InputMatrix.GetElementPointerOfCol(ColIndexList_sort[k]), InputMatrix.GetRowNumber(), 1);

        if (SpecialCompareFunction(Vector_prev, Vector_k) != 0)
        {
            ColIndexList_unique.Append(ColIndexList_sort[k]);

            Vector_prev.ForceShare(InputMatrix.GetElementPointerOfCol(ColIndexList_sort[k]), InputMatrix.GetRowNumber(), 1);
        }
    }

    return ColIndexList_unique;
}

//================================================================================================================================//

template<typename ElementType>
inline
ElementType MatrixMean(const DenseMatrix<ElementType>& InputMatrix)
{
    auto ElementNumber = InputMatrix.GetElementNumber();

    if (ElementNumber <= 0)
    {
        MDK_Error("Input is an empty Matrix @ mdkLinearAlgebra_DenseMatrix MatrixMean(InputMatrix)")
        return InputMatrix.GetNaNElement();
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
DenseMatrix<ElementType> MatrixMeanToRow(const DenseMatrix<ElementType>& InputMatrix)
{
    DenseMatrix<ElementType> tempMatrix;

    auto InputSize = InputMatrix.GetSize();

    if (InputSize.RowNumber <= 0)
    {
        MDK_Error("Input is empty Matrix @ mdkLinearAlgebra_DenseMatrix MatrixMeanToRow(InputMatrix)")

        return tempMatrix;
    }

    tempMatrix.Resize(1, InputSize.ColNumber);

    auto tempRawPointer = tempMatrix.GetElementPointer();

    auto RawPointer = InputMatrix.GetElementPointer();

    for (int_max j = 0; j < InputSize.ColNumber; ++j)
    {
        auto value = RawPointer[0];
        for (int_max i = 1; i < InputSize.RowNumber; ++i)
        {
            value += RawPointer[i];
        }

        tempRawPointer[j] = value / ElementType(InputSize.RowNumber);

        RawPointer += InputSize.RowNumber;
    }

    return tempMatrix;
}


template<typename ElementType>
inline
DenseMatrix<ElementType> MatrixMeanToCol(const DenseMatrix<ElementType>& InputMatrix)
{
    DenseMatrix<ElementType> tempMatrix;

    auto InputSize = InputMatrix.GetSize();

    if (InputSize.RowNumber <= 0)
    {
        MDK_Error("Input is empty Matrix @ mdkLinearAlgebra_DenseMatrix MatrixMeanToCol(InputMatrix)")

        return tempMatrix;
    }

    tempMatrix.Resize(InputSize.RowNumber, 1);

    auto tempRawPointer = tempMatrix.GetElementPointer();

    auto RawPointer = InputMatrix.GetElementPointer();

    for (int_max i = 0; i < InputSize.RowNumber; ++i)
    {
        auto value = RawPointer[i];

        int_max Index = InputSize.RowNumber;

        for (int_max j = 1; j < InputSize.ColNumber; ++j)
        {
            value += RawPointer[Index + i];

            Index += InputSize.RowNumber;
        }

        tempRawPointer[i] = value / ElementType(InputSize.ColNumber);
    }

    return tempMatrix;
}


template<typename ElementType>
inline
int_max FindLinearIndexOfMaxInMatrix(const DenseMatrix<ElementType>& InputMatrix)
{
    auto Input_ElementNumber = InputMatrix.GetElementNumber();

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
        return InputMatrix.GetNaNElement();
    }

    int_max LinearIndex = FindLinearIndexOfMaxInMatrix(InputMatrix);

    return InputMatrix[LinearIndex];
}


template<typename ElementType>
inline
DenseMatrix<ElementType> MatrixMaxToRow(const DenseMatrix<ElementType>& InputMatrix)
{
    DenseMatrix<ElementType> tempMatrix;

    auto InputSize = InputMatrix.GetSize();

    if (InputSize.RowNumber <= 0)
    {
        MDK_Error("Input is empty Matrix @ mdkLinearAlgebra_DenseMatrix MatrixMaxToRow(InputMatrix)")

        return tempMatrix;
    }

    tempMatrix.Resize(1, InputSize.ColNumber);

    auto tempRawPointer = tempMatrix.GetElementPointer();

    auto RawPointer = InputMatrix.GetElementPointer();

    for (int_max j = 0; j < InputSize.ColNumber; ++j)
    {
        auto value = RawPointer[0];

        ++RawPointer;

        for (int_max i = 1; i < InputSize.RowNumber; ++i)
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
DenseMatrix<ElementType> MatrixMaxToCol(const DenseMatrix<ElementType>& InputMatrix)
{
    DenseMatrix<ElementType> tempMatrix;

    auto InputSize = InputMatrix.GetSize();

    if (InputSize.RowNumber <= 0)
    {
        MDK_Error("Input is empty Matrix @ mdkLinearAlgebra_DenseMatrix MatrixMaxToCol(InputMatrix)")

        return tempMatrix;
    }

    tempMatrix.Resize(InputSize.ColNumber, 1);

    auto tempRawPointer = tempMatrix.GetElementPointer();

    auto RawPointer = InputMatrix.GetElementPointer();

    for (int_max i = 0; i < InputSize.RowNumber; ++i)
    {
        auto value = RawPointer[i];

        auto Index = InputSize.RowNumber;

        for (int_max j = 1; j < InputSize.ColNumber; ++j)
        {
            value = std::max(value, RawPointer[Index + i]);

            Index += InputSize.RowNumber;
        }

        tempRawPointer[i] = value;
    }

    return tempMatrix;
}


template<typename ElementType>
int_max FindLinearIndexOfMinInMatrix(const DenseMatrix<ElementType>& InputMatrix)
{
    auto Input_ElementNumber = InputMatrix.GetElementNumber();

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
        return InputMatrix.GetNaNElement();
    }

    auto LinearIndex = FindLinearIndexOfMinInMatrix(InputMatrix);

    return InputMatrix[LinearIndex];
}


template<typename ElementType>
inline
DenseMatrix<ElementType> MatrixMinToRow(const DenseMatrix<ElementType>& InputMatrix)
{
    DenseMatrix<ElementType> tempMatrix;

    auto InputSize = InputMatrix.GetSize();

    if (InputSize.RowNumber <= 0)
    {
        MDK_Error("Input is empty Matrix @ mdkLinearAlgebra_DenseMatrix MatrixMinToRow(InputMatrix)")

        return tempMatrix;
    }

    tempMatrix.Resize(1, InputSize.ColNumber);

    auto tempRawPointer = tempMatrix.GetElementPointer();

    auto RawPointer = InputMatrix.GetElementPointer();

    for (int_max j = 0; j < InputSize.ColNumber; ++j)
    {
        auto value = RawPointer[0];

        ++RawPointer;

        for (int_max i = 1; i < InputSize.RowNumber; ++i)
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
DenseMatrix<ElementType> MatrixMinToCol(const DenseMatrix<ElementType>& InputMatrix)
{
    DenseMatrix<ElementType> tempMatrix;

    auto InputSize = InputMatrix.GetSize();

    if (InputSize.RowNumber <= 0)
    {
        MDK_Error("Input is empty Matrix @ mdkLinearAlgebra_DenseMatrix MatrixMinToCol(InputMatrix)")

        return tempMatrix;
    }

    tempMatrix.Resize(InputSize.ColNumber, 1);

    auto tempRawPointer = tempMatrix.GetElementPointer();

    auto RawPointer = InputMatrix.GetElementPointer();

    for (int_max i = 0; i < InputSize.RowNumber; ++i)
    {
        auto value = RawPointer[i];

        auto Index = InputSize.RowNumber;

        for (int_max j = 1; j < InputSize.ColNumber; ++j)
        {
            value = std::min(value, RawPointer[Index + i]);

            Index += InputSize.RowNumber;
        }

        tempRawPointer[i] = value;
    }

    return tempMatrix;
}


template<typename ElementType>
inline
ElementType MatrixSum(const DenseMatrix<ElementType>& InputMatrix)
{
    auto Input_ElementNumber = InputMatrix.GetElementNumber();

    if (Input_ElementNumber <= 0)
    {
        MDK_Error("Input is empty Matrix @ mdkLinearAlgebra_DenseMatrix MatrixSum(InputMatrix)")
        return InputMatrix.GetNaNElement();
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
DenseMatrix<ElementType> MatrixSumToRow(const DenseMatrix<ElementType>& InputMatrix)
{
    DenseMatrix<ElementType> tempMatrix;

    auto InputSize = InputMatrix.GetSize();

    if (InputSize.RowNumber <= 0)
    {
        MDK_Error("InputMatrix is empty Matrix @ mdkLinearAlgebra_DenseMatrix MatrixSumToRow(InputMatrix)")

        return tempMatrix;
    }

    tempMatrix.Resize(1, InputSize.ColNumber);

    auto tempRawPointer = tempMatrix.GetElementPointer();

    auto RawPointer = InputMatrix.GetElementPointer();

    for (int_max j = 0; j < InputSize.ColNumber; ++j)
    {
        auto value = RawPointer[0];

        ++RawPointer;

        for (int_max i = 1; i < InputSize.RowNumber; ++i)
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
DenseMatrix<ElementType> MatrixSumToCol(const DenseMatrix<ElementType>& InputMatrix)
{
    DenseMatrix<ElementType> tempMatrix;

    auto InputSize = InputMatrix.GetSize();

    if (InputSize.RowNumber <= 0)
    {
        MDK_Error("Input is empty Matrix @ mdkLinearAlgebra_DenseMatrix MatrixSumToCol(InputMatrix)")

        return tempMatrix;
    }

    tempMatrix.Resize(InputSize.RowNumber, 1);

    auto tempRawPointer = tempMatrix.GetElementPointer();

    auto RawPointer = InputMatrix.GetElementPointer();

    for (int_max i = 0; i < InputSize.RowNumber; ++i)
    {
        auto value = RawPointer[i];

        int_max Index = InputSize.RowNumber;

        for (int_max j = 1; j < InputSize.ColNumber; ++j)
        {
            value += RawPointer[Index + i];

            Index += InputSize.RowNumber;
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
    auto ElementNumber = InputMatrix.GetElementNumber();

    if (ElementNumber == 0)
    {
        MDK_Error("empty input matrix @ mdkLinearAlgebra_DenseMatrix MatrixNorm_L1(InputMatrix)")
        return InputMatrix.GetNaNElement();
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
    auto ElementNumber = InputMatrix.GetElementNumber();

    if (ElementNumber == 0)
    {
        MDK_Error("empty input matrix @ mdkLinearAlgebra_DenseMatrix MatrixNorm_L2(InputMatrix)")
        return InputMatrix.GetNaNElement();
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

    if (Size.RowNumber == 0)
    {
        MDK_Error("InputMatrix is empty @ mdkLinearAlgebra_DenseMatrix MatrixTranspose(InputMatrix)")

        return tempMatrix;
    }

    if (Size.ColNumber == 1)
    {
        tempMatrix.Copy(InputMatrix.GetElementPointer(), 1, Size.RowNumber);

        return tempMatrix;
    }

    if (Size.RowNumber == 1)
    {
        tempMatrix.Copy(InputMatrix.GetElementPointer(), Size.ColNumber, 1);

        return tempMatrix;
    }

    tempMatrix.FastResize(Size.ColNumber, Size.RowNumber);

    auto tempRawPointer = tempMatrix.GetElementPointer();

    auto RawPointer = InputMatrix.GetElementPointer();

    for (int_max i = 0; i < Size.RowNumber; ++i)
    {
        int_max Index = 0;

        for (int_max j = 0; j < Size.ColNumber; ++j)
        {
            tempRawPointer[0] = RawPointer[Index + i];

            Index += Size.RowNumber;

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

    if (Size.RowNumber == 0)
    {
        return;
    }

    if (Size.ColNumber == 1 || Size.RowNumber == 1)
    {
        InputMatrix.Reshape(Size.ColNumber, Size.RowNumber);
        return;
    }

    //--------------------- call armadillo

    arma::Mat<ElementType> A(InputMatrix.GetElementPointer(), arma::uword(InputMatrix.GetRowNumber()), arma::uword(InputMatrix.GetColNumber()), false);

    arma::inplace_trans(A);

    InputMatrix.Reshape(Size.ColNumber, Size.RowNumber);
}


template<typename ElementType>
inline 
int_max MatrixRank(const DenseMatrix<ElementType>& InputMatrix)
{
    auto Size = InputMatrix.GetSize();

    if (Size.RowNumber == 0)
    {
        MDK_Error("InputMatrix is empty  @ mdkLinearAlgebra_DenseMatrix MatrixRank(InputMatrix)")
        return 0;
    }

    auto ptrData = const_cast<ElementType*>(InputMatrix.GetElementPointer());

    // call Armadillo 

    arma::Mat<ElementType> tempMat(ptrData, arma::uword(Size.RowNumber), arma::uword(Size.ColNumber), false);

    int_max value = arma::rank(tempMat);

    return value;
}


template<typename ElementType>
inline
DenseMatrix<ElementType> MatrixInverse(const DenseMatrix<ElementType>& InputMatrix)
{
    DenseMatrix<ElementType> OutputMatrix;

    auto Size = InputMatrix.GetSize();

    if (Size.RowNumber == 0)
    {
		MDK_Warning("InputMatrix is empty matrix @ mdkLinearAlgebra_DenseMatrix MatrixInverse(InputMatrix)")
        return OutputMatrix;
    }

    if (Size.RowNumber != Size.ColNumber)
    {
        MDK_Error("InputMatrix is not square @ mdkLinearAlgebra_DenseMatrix MatrixInverse(InputMatrix)")
        return OutputMatrix;
    }

	OutputMatrix.FastResize(Size.RowNumber, Size.ColNumber);

    auto ptrData = const_cast<ElementType*>(InputMatrix.GetElementPointer());

    // call Armadillo 
    arma::Mat<ElementType> tempMat(ptrData, arma::uword(Size.RowNumber), arma::uword(Size.ColNumber), false);
	arma::Mat<ElementType> tempInv(OutputMatrix.GetElementPointer(), arma::uword(Size.RowNumber), arma::uword(Size.ColNumber), false);
    tempInv = arma::inv(tempMat);

	return OutputMatrix;
}


template<typename ElementType>
inline
DenseMatrix<ElementType> MatrixPseudoInverse(const DenseMatrix<ElementType>& InputMatrix)
{
	DenseMatrix<ElementType> OutputMatrix;

	auto Size = InputMatrix.GetSize();

	if (Size.RowNumber == 0)
	{
		MDK_Warning("InputMatrix is empty matrix @ mdkLinearAlgebra_DenseMatrix MatrixPseudoInverse(InputMatrix)")
	    return OutputMatrix;
	}

	OutputMatrix.FastResize(Size.RowNumber, Size.ColNumber);

	auto ptrData = const_cast<ElementType*>(InputMatrix.GetElementPointer());

	// call Armadillo 
	arma::Mat<ElementType> tempMat(ptrData, arma::uword(Size.RowNumber), arma::uword(Size.ColNumber), false);
	arma::Mat<ElementType> tempInv(OutputMatrix.GetElementPointer(), arma::uword(Size.RowNumber), arma::uword(Size.ColNumber), false);
	tempInv = arma::pinv(tempMat);

	return OutputMatrix;
}


template<typename ElementType>
inline 
DenseMatrix<ElementType> SolveMatrixLinearEquation(const DenseMatrix<ElementType>& MatrixA, const DenseMatrix<ElementType>& MatrixB)
{
	// A*X=B
	DenseMatrix<ElementType> MatrixX;

	auto SizeA = MatrixA.GetSize();
	auto SizeB = MatrixB.GetSize();

	if (SizeA.RowNumber != SizeB.RowNumber)
	{
		MDK_Error("A RowNumber != B RowNumber @ mdkLinearAlgebra_DenseMatrix SolveMatrixLinearEquation(MatrixA, MatrixB)")
 	    return MatrixX;
	}

	if (SizeA.RowNumber == 0)
	{
		MDK_Warning("MatrixA is empty @ mdkLinearAlgebra_DenseMatrix SolveMatrixLinearEquation(MatrixA, MatrixB)")
	    return MatrixX;
	}

	MatrixX.Resize(SizeA.ColNumber, SizeB.ColNumber);

	auto ptrX = const_cast<ElementType*>(MatrixX.GetElementPointer());
	auto ptrA = const_cast<ElementType*>(MatrixA.GetElementPointer());
	auto ptrB = const_cast<ElementType*>(MatrixB.GetElementPointer());

	// call Armadillo 
	arma::Mat<ElementType> tempX(ptrX, arma::uword(SizeA.ColNumber), arma::uword(SizeB.ColNumber), false);
	arma::Mat<ElementType> tempA(ptrA, arma::uword(SizeA.RowNumber), arma::uword(SizeA.ColNumber), false);
	arma::Mat<ElementType> tempB(ptrB, arma::uword(SizeB.RowNumber), arma::uword(SizeB.ColNumber), false);
	auto Flag = arma::solve(tempX, tempA, tempB);
	if (Flag == false)
	{
		MDK_Error("No solution @ mdkLinearAlgebra_DenseMatrix SolveMatrixLinearEquation(MatrixA, MatrixB)")
	}

	return MatrixX;
}

template<typename ElementType>
inline 
DenseMatrixEigenResult<std::complex<ElementType>> NonSymmetricRealMatrixEigen(const DenseMatrix<ElementType>& InputMatrix)
{
    DenseMatrixEigenResult<std::complex<ElementType>> Result;

    auto Size = InputMatrix.GetSize();

    if (Size.RowNumber == 0)
    {
        MDK_Error("InputMatrix is empty matrix @ mdkLinearAlgebra_DenseMatrix NonSymmetricRealMatrixEigen(InputMatrix)")

        return Result;
    }

    if (Size.RowNumber != Size.ColNumber)
    {
        MDK_Error("InputMatrix is not square @ mdkLinearAlgebra_DenseMatrix NonSymmetricRealMatrixEigen(InputMatrix)")

        return Result;
    }

    Result.EigenVector.FastResize(Size.RowNumber, Size.RowNumber);

    Result.EigenValue.FastResize(Size.RowNumber, 1);

    auto ptrData = const_cast<ElementType*>(InputMatrix.GetElementPointer());

    // call Armadillo 

    arma::Mat<ElementType> tempMat(ptrData, arma::uword(Size.RowNumber), arma::uword(Size.RowNumber), false);

    arma::Mat<std::complex<ElementType>> tempEigenVector(Result.EigenVector.GetElementPointer(), arma::uword(Size.RowNumber), arma::uword(Size.RowNumber), false);

    arma::Col<std::complex<ElementType>> tempEigenValue(Result.EigenValue.GetElementPointer(), arma::uword(Size.RowNumber), false);

    arma::eig_gen(tempEigenValue, tempEigenVector, tempMat);

    return Result;
}


template<typename ElementType>
inline 
DenseMatrixEigenResult<ElementType> RealSymmetricMatrixEigen(const DenseMatrix<ElementType>& InputMatrix, bool CheckIfSymmetric = false)
{
    DenseMatrixEigenResult<ElementType> Result;

    auto Size = InputMatrix.GetSize();

    if (Size.RowNumber == 0)
    {
        MDK_Error("Matrix is empty matrix @ mdkLinearAlgebra_DenseMatrix MatrixEigen(InputMatrix)")

        return Result;
    }

    if (Size.RowNumber != Size.ColNumber)
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
  
    Result.EigenVector.FastResize(Size.RowNumber, Size.RowNumber);

    Result.EigenValue.FastResize(Size.RowNumber, 1);

    auto ptrData = const_cast<ElementType*>(InputMatrix.GetElementPointer());

    // call Armadillo 

    arma::Mat<ElementType> tempMat(ptrData, arma::uword(Size.RowNumber), arma::uword(Size.RowNumber), false);

    arma::Mat<ElementType> tempEigenVector(Result.EigenVector.GetElementPointer(), arma::uword(Size.RowNumber), arma::uword(Size.RowNumber), false);

    arma::Col<ElementType> tempEigenValue(Result.EigenValue.GetElementPointer(), arma::uword(Size.RowNumber), false);

    arma::eig_sym(tempEigenValue, tempEigenVector, tempMat);

    return Result;
}


template<typename ElementType>
inline
DenseMatrixPCAResult<ElementType> MatrixPCA(const DenseMatrix<ElementType>& InputMatrix)
{
    DenseMatrixPCAResult<ElementType> PCAResult;

    auto Size = InputMatrix.GetSize();

    if (Size.ColNumber <= 1)
    {
        MDK_Error("ColNumber <= 1, return empty PCAResult @ mdkLinearAlgebra_DenseMatrix MatrixPCA(...)")
        return PCAResult;
    }

    auto MeanCol = InputMatrix.SumToCol();
    MeanCol /= Size.ColNumber;

    DenseMatrix<ElementType> CovarianceMatrix(Size.RowNumber, Size.RowNumber);

    CovarianceMatrix.Fill(0);

    // reference only
    //for (int_max i = 0; i < Size.ColNumber; ++i)
    //{
    //    DenseMatrix<ElementType> tempCol = InputMatrix(ALL, { i }) - MeanCol;
       
    //    CovarianceMatrix += tempCol * tempCol.Transpose();
    //}

    DenseMatrix<ElementType> tempMatrix(Size.RowNumber, Size.RowNumber);

    DenseMatrix<ElementType> tempCol(Size.RowNumber, 1);

    DenseMatrix<ElementType> tempColTranspose(1, Size.RowNumber);

    for (int_max i = 0; i < Size.ColNumber; ++i)
    {
        InputMatrix.GetCol(i, tempCol);

        MatrixSubtract(tempCol, tempCol, MeanCol);

        tempColTranspose = tempCol.Transpose();

        MatrixMultiply(tempMatrix, tempCol, tempColTranspose);

        CovarianceMatrix += tempMatrix;
    }

    CovarianceMatrix /= Size.ColNumber;

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

    if (Size.RowNumber == 0)
    {
        MDK_Error("Matrix is empty  @ mdkLinearAlgebra_DenseMatrix MatrixSVD(InputMatrix)")
        return Result;
    }

    Result.U.FastResize(Size.RowNumber, Size.ColNumber);
    Result.S.FastResize(Size.RowNumber, Size.ColNumber);
    Result.V.FastResize(Size.RowNumber, Size.ColNumber);

    auto ptrData = const_cast<ElementType*>(InputMatrix.GetElementPointer());

    // call Armadillo 

    arma::Mat<ElementType> X(ptrData, arma::uword(Size.RowNumber), arma::uword(Size.ColNumber), false);

    arma::Mat<ElementType> U(Result.U.GetElementPointer(), arma::uword(Size.RowNumber), arma::uword(Size.ColNumber), false);
    arma::Col<ElementType> S;
    arma::Mat<ElementType> V(Result.V.GetElementPointer(), arma::uword(Size.RowNumber), arma::uword(Size.ColNumber), false);

    arma::svd(U, S, V, X);

    Result.S.SetDiagonal(S.memptr());

    return Result;
}

}//end namespace mdk

#endif
