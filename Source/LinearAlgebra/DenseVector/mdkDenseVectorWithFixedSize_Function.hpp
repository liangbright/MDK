#ifndef __mdkDenseVectorWithFixedSize_Function_hpp
#define __mdkDenseVectorWithFixedSize_Function_hpp


namespace mdk
{

template<typename ElementType, int_max Length>
inline
DenseVectorWithFixedSize<ElementType, Length> 
VectorLinearCombine(const std::vector<ElementType>& CoefList, const std::vector<const DenseVectorWithFixedSize<ElementType, Length>*>& VectorSet)
{
    typedef DenseVectorWithFixedSize<ElementType, Length> VectorType;

    VectorType tempVector; // all elements have been set to zero

    auto Number = int_max(VectorSet.size());

    for (int_max k = 0; k < Number; ++k)
    {
        auto Coef_k = CoefList[k];

        VectorType& Vector_k = *VectorSet[k];

        for (int_max i = 0; i < Length; ++i)
        {            
            tempVector[i] + = Coef_k * Vector_k[i];
        }
    }

    return tempVector;
}


}// namespace mdk

#endif