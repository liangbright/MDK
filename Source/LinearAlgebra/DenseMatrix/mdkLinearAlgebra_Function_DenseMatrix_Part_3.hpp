#ifndef __mdkLinearAlgebra_Function_DenseMatrix_Part_3_hpp
#define __mdkLinearAlgebra_Function_DenseMatrix_Part_3_hpp

//#include "mdkLinearAlgebra_Function_DenseMatrix_Part_3.h"

namespace mdk
{

// SingleVector is a column vector
// each column of VectorSet is a vector
// L2DistanceList[j] = distance between SingleVector and VectorSet(ALL, j)
template<typename ElementType>
DenseMatrix<ElementType> ComputeL2DistanceListFromSingleVectorToVectorSet(const DenseMatrix<ElementType>& SingleVector,
                                                                          const DenseMatrix<ElementType>& VectorSet)
{
    DenseMatrix<ElementType> L2DistanceList;

    auto Size = VectorSet.GetSize();

    if (Size.ColNumber == 0)
    {
        MDK_Error("Empty Dictionary @ mdkLinearAlgebra_DenseMatrix ComputeL2DistanceListFromSingleVectorToVectorSet(...)")
        return L2DistanceList;
    }

    if (SingleVector.GetElementNumber() != Size.RowNumber)
    {
        MDK_Error("Size does not match @ mdkLinearAlgebra_DenseMatrix ComputeL2DistanceListFromSingleVectorToVectorSet(...)")
        return L2DistanceList;
    }

    L2DistanceList.Resize(1, Size.ColNumber);

    auto ElementPointer = VectorSet.GetElementPointer();

    for (int_max j = 0; j < Size.ColNumber; ++j)
    {
        auto BeginPointer_j = ElementPointer + j*Size.RowNumber;

        ElementType Distance_j = ElementType(0);

        for (int_max i = 0; i < Size.RowNumber; ++i)
        {
            auto temp = SingleVector[i] - BeginPointer_j[i];
            Distance_j += temp*temp;
        }

        L2DistanceList[j] = std::sqrt(Distance_j);
    }

    return L2DistanceList;
}

}

#endif