#ifndef __mdkLinearAlgebra_Function_DenseVector_Part_1_hpp
#define __mdkLinearAlgebra_Function_DenseVector_Part_1_hpp


namespace mdk
{
    
template<typename ElementType, int_max L>
inline 
DenseVector<int_max, -1> FindUniqueElementInVector(const DenseVector<ElementType, L>& Vector)
{
    auto Length = Vector.GetLength();

    DenseVector<int_max, -1> IndexList_unique;

    if (Length <= 0)
    {
        return IndexList_unique;
    }

    auto IndexList_sort = Vector.Sort([](const ElementType& a, const ElementType& b){ return a < b; });

    if (IndexList_sort.IsEmpty() == true)
    {
        return IndexList_unique;
    }

    IndexList_unique.ReserveCapacity(IndexList_sort.GetLength());

    auto Element_prev = Vector[IndexList_sort[0]];

    IndexList_unique.Append(IndexList_sort[0]);

    for (int_max k = 1; k < IndexList_sort.GetLength(); ++k)
    {
        auto Element = Vector[IndexList_sort[k]];

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
DenseVector<int_max, -1> FindUniqueElementInVector(const DenseVector<ElementType, L>& Vector, SpecialCompareFunctionType SpecialCompareFunction)
{
    // Result = CompareFunction(const ElementType& a, const ElementType& b)
    // Result: integer type
    // Result: -1: a < b
    // Result: 0:  a = b 
    // Result: 1:  a > b
    //-------------------------------------------------------------------------

    auto Length = Vector.GetLength();

    if (Length <= 0)
    {
        return IndexList_unique;
    }

    DenseVector<int_max, -1> IndexList_unique;

    auto IndexList_sort = Vector.Sort([&](const ElementType& a, const ElementType& b)
    {
        auto Result = SpecialCompareFunction(a, b);
        return (Result < 0);
    });

    if (IndexList_sort.IsEmpty() == true)
    {
        return IndexList_unique;
    }

    IndexList_unique.ReserveCapacity(IndexList_sort.GetLength());

    auto Element_prev = Vector[IndexList_sort[0]];

    IndexList_unique.Append(IndexList_sort[0]);

    for (int_max k = 1; k < IndexList_sort.GetLength(); ++k)
    {
        auto Element = Vector[IndexList_sort[k]];

        if (SpecialCompareFunction(Element, Element_prev) != 0)
        {
            IndexList_unique.Append(IndexList_sort[k]);

            Element_prev = Element;
        }
    }

    return IndexList_unique;
}

}

#endif