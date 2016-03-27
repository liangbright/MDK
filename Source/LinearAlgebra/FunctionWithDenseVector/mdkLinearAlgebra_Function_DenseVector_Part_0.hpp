#pragma once


namespace mdk
{
template<typename ElementType, int_max Length>
DenseVector<ElementType, Length> 
VectorLinearCombine(const std::vector<ElementType>& CoefList, const std::vector<const DenseVector<ElementType, Length>*>& VectorSet)
{
    typedef DenseVector<ElementType, Length> VectorType;

	VectorType OutputVector; // all elements have been set to zero

    auto Number = int_max(VectorSet.size());

    for (int_max k = 0; k < Number; ++k)
    {
        auto Coef_k = CoefList[k];

        const VectorType& Vector_k = *VectorSet[k];

        for (int_max i = 0; i < Length; ++i)
        {            
			OutputVector[i] + = Coef_k * Vector_k[i];
        }
    }

	return OutputVector;
}


template<typename ElementType>
inline
DenseVector<ElementType> 
VectorLinearCombine(const std::vector<ElementType>& CoefList, const std::vector<const DenseVector<ElementType>*>& VectorSet)
{
    typedef DenseVector<ElementType> VectorType;

    VectorType EmptyVector;

    if (CoefList.size() != VectorSet.size())
    {
        MDK_Error("CoefList.size() != VectorSet.size() @ mdkLinearAlgebra VectorLinearCombine(...)")

        return EmptyVector;
    } 

    int_max VectorNumber = int_max(VectorSet.size());

    auto Length = VectorSet[0]->GetLength();

    for (int_max k = 1; k < VectorNumber; ++k)
    {
        if (VectorSet[k]->GetLength() != Length)
        {
            MDK_Error("Size is not the same in VectorSet @ mdkLinearAlgebra VectorLinearCombine(...)")

            return EmptyVector;
        }
    }

    VectorType OutputVector;

    OutputVector.Resize(Length);

    OutputVector.Fill(ElementType(0));

    for (int_max k = 0; k < VectorNumber; ++k)
    {
        auto Coef_k = CoefList[k];

        const VectorType& Vector_k = *VectorSet[k];

        for (int_max i = 0; i < Length; ++i)
        {
            OutputVector[i] + = Coef_k * Vector_k[i];
        }
    }

    return OutputVector;
}

}// namespace mdk
