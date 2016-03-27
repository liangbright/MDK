#pragma once


namespace mdk
{
    
template<typename ElementType, int_max L>
inline 
DenseVector<int_max> FindUniqueElementInVector(const DenseVector<ElementType, L>& InputVector)
{
	auto Length = InputVector.GetLength();

    DenseVector<int_max> IndexList_unique;

    if (Length <= 0)
    {
        return IndexList_unique;
    }

	auto IndexList_sort = InputVector.Sort([](const ElementType& a, const ElementType& b){ return a < b; });

    if (IndexList_sort.IsEmpty() == true)
    {
        return IndexList_unique;
    }

    IndexList_unique.SetCapacity(IndexList_sort.GetLength());

	auto Element_prev = InputVector[IndexList_sort[0]];

    IndexList_unique.Append(IndexList_sort[0]);

    for (int_max k = 1; k < IndexList_sort.GetLength(); ++k)
    {
		auto Element = InputVector[IndexList_sort[k]];

        if (Element != Element_prev)
        {
            IndexList_unique.Append(IndexList_sort[k]);

            Element_prev = Element;
        }
    }

    return IndexList_unique;
}


template<typename ElementType, int_max L, typename SpecialCompareFunctionType>
inline 
DenseVector<int_max> FindUniqueElementInVector(const DenseVector<ElementType, L>& InputVector, SpecialCompareFunctionType SpecialCompareFunction)
{
    // Result = CompareFunction(const ElementType& a, const ElementType& b)
    // Result: integer type
    // Result: -1: a < b
    // Result: 0:  a = b 
    // Result: 1:  a > b
    //-------------------------------------------------------------------------

	auto Length = InputVector.GetLength();

    if (Length <= 0)
    {
        return IndexList_unique;
    }

    DenseVector<int_max> IndexList_unique;

	auto IndexList_sort = InputVector.Sort([&](const ElementType& a, const ElementType& b)
    {
        auto Result = SpecialCompareFunction(a, b);
        return (Result < 0);
    });

    if (IndexList_sort.IsEmpty() == true)
    {
        return IndexList_unique;
    }

    IndexList_unique.SetCapacity(IndexList_sort.GetLength());

	auto Element_prev = InputVector[IndexList_sort[0]];

    IndexList_unique.Append(IndexList_sort[0]);

    for (int_max k = 1; k < IndexList_sort.GetLength(); ++k)
    {
		auto Element = InputVector[IndexList_sort[k]];

        if (SpecialCompareFunction(Element, Element_prev) != 0)
        {
            IndexList_unique.Append(IndexList_sort[k]);

            Element_prev = Element;
        }
    }

    return IndexList_unique;
}


template<typename ElementType, int_max A, int_max B>
inline DenseVector<ElementType> Intersect(const DenseVector<ElementType, A>& SetA, const DenseVector<ElementType, B>& SetB)
{
	DenseVector<ElementType> SetC;

	if (SetA.IsEmpty() == true || SetB.IsEmpty() == true)
	{
		return SetC;
	}

	SetC.SetCapacity(std::min(SetA.GetLength(), SetB.GetLength()));

	for (int_max k = 0; k < SetA.GetLength(); ++k)
	{
		for (int_max n = 0; n < SetB.GetLength(); ++n)
		{
			if (SetA[k] == SetB[n])
			{
				SetC.Append(SetA[k]);
			}
		}
	}
	return SetC;
}


template<typename ElementType, int_max A, int_max B>
DenseVector<ElementType> SetDiff(const DenseVector<ElementType, A>& SetA, const DenseVector<ElementType, B>& SetB)
{// in A, NOT in B

	if (SetB.IsEmpty() == true)
	{
		return SetA;
	}

	DenseVector<ElementType> SetC;
	SetC.SetCapacity(SetA.GetLength());

	for (int_max k = 0; k < SetA.GetLength(); ++k)
	{
		bool Flag = false;
		for (int_max n = 0; n < SetB.GetLength(); ++n)
		{
			if (SetA[k] == SetB[n])
			{
				Flag = true;
				break;
			}
		}
		if (Flag == false)
		{
			SetC.Append(SetA[k]);
		}
	}
	return SetC;
}


}//namespace
