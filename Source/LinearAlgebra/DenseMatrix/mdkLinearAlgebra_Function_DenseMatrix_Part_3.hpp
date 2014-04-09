#ifndef __mdkLinearAlgebra_Function_DenseMatrix_Part_3_hpp
#define __mdkLinearAlgebra_Function_DenseMatrix_Part_3_hpp

//#include "mdkLinearAlgebra_Function_DenseMatrix_Part_3.h"

namespace mdk
{

// SingleVector is a column vector
// each column of VectorSet is a vector
// L2DistanceList[j] = distance between SingleVector and VectorSet(ALL, j)
template<typename ElementType>
DenseMatrix<ElementType> ComputeL2DistanceListFromSingleVectorToColVectorSet(const DenseMatrix<ElementType>& SingleVector,
                                                                             const DenseMatrix<ElementType>& ColVectorSet)
{
    DenseMatrix<ElementType> L2DistanceList;

    auto Size = ColVectorSet.GetSize();

    if (Size.ColNumber == 0)
    {
        MDK_Error("Empty Dictionary @ mdkLinearAlgebra_DenseMatrix ComputeL2DistanceListFromSingleVectorToColVectorSet(...)")
        return L2DistanceList;
    }

    if (SingleVector.IsVector() == false)
    {
        MDK_Error("Input vector is not a vector @ mdkLinearAlgebra_DenseMatrix ComputeL2DistanceListFromSingleVectorToColVectorSet(...)")
        return L2DistanceList;
    }

    if (SingleVector.GetElementNumber() != Size.RowNumber)
    {
        MDK_Error("Size does not match @ mdkLinearAlgebra_DenseMatrix ComputeL2DistanceListFromSingleVectorToColVectorSet(...)")
        return L2DistanceList;
    }

    L2DistanceList.FastResize(1, Size.RowNumber);

    auto ElementPointer = ColVectorSet.GetElementPointer();

    for (int_max j = 0; j < Size.ColNumber; ++j)
    {
        auto Vector_j = ElementPointer + j*Size.RowNumber;

        ElementType Distance_j = ElementType(0);

        for (int_max i = 0; i < Size.RowNumber; ++i)
        {
            auto temp = SingleVector[i] - Vector_j[i];
            Distance_j += temp*temp;
        }

        L2DistanceList[j] = std::sqrt(Distance_j);
    }

    return L2DistanceList;
}


template<typename ElementType>
DenseMatrix<ElementType> ComputeL1DistanceListFromSingleVectorToColVectorSet(const DenseMatrix<ElementType>& SingleVector,
    const DenseMatrix<ElementType>& ColVectorSet)
{
    DenseMatrix<ElementType> L1DistanceList;

    auto Size = ColVectorSet.GetSize();

    if (Size.ColNumber == 0)
    {
        MDK_Error("Empty Dictionary @ mdkLinearAlgebra_DenseMatrix ComputeL1DistanceListFromSingleVectorToColVectorSet(...)")
            return L1DistanceList;
    }

    if (SingleVector.IsVector() == false)
    {
        MDK_Error("Input vector is not a vector @ mdkLinearAlgebra_DenseMatrix ComputeL1DistanceListFromSingleVectorToColVectorSet(...)")
        return L1DistanceList;
    }

    if (SingleVector.GetElementNumber() != Size.RowNumber)
    {
        MDK_Error("Size does not match @ mdkLinearAlgebra_DenseMatrix ComputeL1DistanceListFromSingleVectorToColVectorSet(...)")
        return L1DistanceList;
    }

    L1DistanceList.FastResize(1, Size.RowNumber);

    auto ElementPointer = ColVectorSet.GetElementPointer();

    for (int_max j = 0; j < Size.ColNumber; ++j)
    {
        auto Vector_j = ElementPointer + j*Size.RowNumber;

        ElementType Distance_j = ElementType(0);

        for (int_max i = 0; i < Size.RowNumber; ++i)
        {
            Distance_j = std::abs(SingleVector[i] - Vector_j[i]);
        }

        L1DistanceList[j] = Distance_j;
    }

    return L1DistanceList;
}


template<typename ElementType>
DenseMatrix<ElementType> ComputeCorrelationListFromSingleVectorToColVectorSet(const DenseMatrix<ElementType>& SingleVector,
                                                                              const DenseMatrix<ElementType>& ColVectorSet)
{
    DenseMatrix<ElementType> CorrelationList;

    auto Size = ColVectorSet.GetSize();

    if (Size.ColNumber == 0)
    {
        MDK_Error("Empty Dictionary @ mdkLinearAlgebra_DenseMatrix ComputeCorrelationListFromSingleVectorToColVectorSet(...)")
        return CorrelationList;
    }

    if (SingleVector.IsVector() == false)
    {
        MDK_Error("Input vector is not a vector @ mdkLinearAlgebra_DenseMatrix ComputeCorrelationListFromSingleVectorToColVectorSet(...)")
        return CorrelationList;
    }

    if (SingleVector.GetElementNumber() != Size.RowNumber)
    {
        MDK_Error("Size does not match @ mdkLinearAlgebra_DenseMatrix ComputeCorrelationListFromSingleVectorToColVectorSet(...)")
        return CorrelationList;
    }

    CorrelationList.FastResize(1, Size.RowNumber);

    auto ElementPointer = ColVectorSet.GetElementPointer();

    for (int_max j = 0; j < Size.ColNumber; ++j)
    {
        auto Vector_j = ElementPointer + j*Size.RowNumber;

        ElementType Mean_s = ElementType(0);

        ElementType Mean_j = ElementType(0);

        for (int_max i = 0; i < Size.RowNumber; ++i)
        {
            Mean_s += SingleVector[i];
            Mean_j += Vector_j[i];
        }
        Mean_s /= ElementType(Size.RowNumber);
        Mean_j /= ElementType(Size.RowNumber);

        ElementType part_a = ElementType(0);
        ElementType part_b = ElementType(0);
        ElementType part_c = ElementType(0);

        for (int_max i = 0; i < Size.RowNumber; ++i)
        {
            part_a += (SingleVector[i] - Mean_s)*(Vector_j[i] - Mean_j);
            part_b += (SingleVector[i] - Mean_s)*(SingleVector[i] - Mean_s);
            part_c += (Vector_j[i] - Mean_j)*(Vector_j[i] - Mean_j);
        }

        CorrelationList[j] = part_a / (std::sqrt(part_b) * std::sqrt(part_c));
    }

    return CorrelationList;
}


template<typename ElementType>
DenseMatrix<ElementType> ComputeUncenteredCorrelationListFromSingleVectorToColVectorSet(const DenseMatrix<ElementType>& SingleVector,
    const DenseMatrix<ElementType>& ColVectorSet)
{
    DenseMatrix<ElementType> CorrelationList;

    auto Size = ColVectorSet.GetSize();

    if (Size.ColNumber == 0)
    {
        MDK_Error("Empty Dictionary @ mdkLinearAlgebra_DenseMatrix ComputeUncenteredCorrelationListFromSingleVectorToColVectorSet(...)")
            return CorrelationList;
    }

    if (SingleVector.IsVector() == false)
    {
        MDK_Error("Input vector is not a vector @ mdkLinearAlgebra_DenseMatrix ComputeUncenteredCorrelationListFromSingleVectorToColVectorSet(...)")
            return CorrelationList;
    }

    if (SingleVector.GetElementNumber() != Size.RowNumber)
    {
        MDK_Error("Size does not match @ mdkLinearAlgebra_DenseMatrix ComputeUncenteredCorrelationListFromSingleVectorToColVectorSet(...)")
            return CorrelationList;
    }

    CorrelationList.FastResize(1, Size.RowNumber);

    auto ElementPointer = ColVectorSet.GetElementPointer();

    for (int_max j = 0; j < Size.ColNumber; ++j)
    {
        auto Vector_j = ElementPointer + j*Size.RowNumber;

        ElementType part_a = ElementType(0);
        ElementType part_b = ElementType(0);
        ElementType part_c = ElementType(0);

        for (int_max i = 0; i < Size.RowNumber; ++i)
        {
            part_a += SingleVector[i] * Vector_j[i];
            part_b += SingleVector[i] * SingleVector[i];
            part_c += Vector_j[i] * Vector_j[i];
        }

        CorrelationList[j] = part_a / (std::sqrt(part_b) * std::sqrt(part_c));
    }

    return CorrelationList;
}


}

#endif