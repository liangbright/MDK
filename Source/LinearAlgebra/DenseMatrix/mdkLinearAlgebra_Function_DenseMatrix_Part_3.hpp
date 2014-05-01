#ifndef __mdkLinearAlgebra_Function_DenseMatrix_Part_3_hpp
#define __mdkLinearAlgebra_Function_DenseMatrix_Part_3_hpp


namespace mdk
{

template<typename ElementType>
DenseMatrix<ElementType> ComputeInnerProductListFromSingleVectorToColVectorSet(const DenseMatrix<ElementType>& SingleVector,
                                                                               const DenseMatrix<ElementType>& ColVectorSet)
{
    DenseMatrix<ElementType> InnerProductList;

    auto Size = ColVectorSet.GetSize();

    if (Size.ColNumber == 0)
    {
        MDK_Error("Empty Dictionary @ mdkLinearAlgebra_DenseMatrix ComputeInnerProductListFromSingleVectorToColVectorSet(...)")
        return InnerProductList;
    }

    if (SingleVector.IsVector() == false)
    {
        MDK_Error("Input vector is not a vector @ mdkLinearAlgebra_DenseMatrix ComputeInnerProductListFromSingleVectorToColVectorSet(...)")
        return InnerProductList;
    }

    if (SingleVector.GetElementNumber() != Size.RowNumber)
    {
        MDK_Error("Size does not match @ mdkLinearAlgebra_DenseMatrix ComputeInnerProductListFromSingleVectorToColVectorSet(...)")
        return InnerProductList;
    }

    InnerProductList.FastResize(1, Size.ColNumber);

    auto PointToSingleVector = SingleVector.GetElementPointer();

    auto PointerToSet = ColVectorSet.GetElementPointer();

    for (int_max j = 0; j < Size.ColNumber; ++j)
    {
        auto Vector_j = PointerToSet + j*Size.RowNumber;

        InnerProductList[j] = ComputeInnerProductOfTwoVectors(PointToSingleVector, Vector_j, Size.RowNumber, false);
    }

    return InnerProductList;
}


template<typename ElementType>
inline
ElementType ComputeInnerProductOfTwoVectors(const DenseMatrix<ElementType>& VectorA, const DenseMatrix<ElementType>& VectorB)
{
    if (VectorA.IsVector() == false || VectorB.IsVector() == false)
    {
        MDK_Error("Input VectorA or VectorB is not a vector @ mdkLinearAlgebra_DenseMatrix ComputeInnerProductOfTwoVectors(...)")
        return ElementType(0);
    }

    auto LengthA = VectorA.GetElementNumber();
    auto LengthB = VectorB.GetElementNumber();

    if (LengthA != LengthB)
    {
        MDK_Error("Size does not match @ mdkLinearAlgebra_DenseMatrix ComputeInnerProductOfTwoVectors(...)")
        return ElementType(0);
    }

    return ComputeInnerProductOfTwoVectors(VectorA.GetElementPointer(), VectorB.GetElementPointer(), LengthA, false);
}


template<typename ElementType>
inline
ElementType ComputeInnerProductOfTwoVectors(const ElementType* VectorA, const ElementType* VectorB, int_max Length, bool CheckInput)
{
    if (CheckInput == true)
    {
        if (VectorA == nullptr || VectorB == nullptr || Length <= 0)
        {
            MDK_Error("Invalid input @ mdkLinearAlgebra_DenseMatrix ComputeInnerProductOfTwoVectors(pointer ...)")
            return ElementType(0);
        }
    }

    auto InnerProduct = ElementType(0);

    for (int_max i = 0; i < Length; ++i)
    {
        InnerProduct += VectorA[i] * VectorB[i];
    }

    return InnerProduct;
}


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

    L2DistanceList.FastResize(1, Size.ColNumber);

    auto PointToSingleVector = SingleVector.GetElementPointer();

    auto PointerToSet = ColVectorSet.GetElementPointer();

    for (int_max j = 0; j < Size.ColNumber; ++j)
    {
        auto Vector_j = PointerToSet + j*Size.RowNumber;

        L2DistanceList[j] = ComputeL2DistanceBetweenTwoVectors(PointToSingleVector, Vector_j, Size.RowNumber, false);
    }

    return L2DistanceList;
}


template<typename ElementType>
inline
ElementType ComputeL2DistanceBetweenTwoVectors(const DenseMatrix<ElementType>& VectorA, const DenseMatrix<ElementType>& VectorB)
{
    if (VectorA.IsVector() == false || VectorB.IsVector() == false)
    {
        MDK_Error("Input VectorA or VectorB is not a vector @ mdkLinearAlgebra_DenseMatrix ComputeL2DistanceBetweenTwoVectors(...)")
        return ElementType(-100);
    }

    auto LengthA = VectorA.GetElementNumber();
    auto LengthB = VectorB.GetElementNumber();

    if (LengthA != LengthB)
    {
        MDK_Error("Size does not match @ mdkLinearAlgebra_DenseMatrix ComputeL2DistanceBetweenTwoVectors(...)")
        return ElementType(-100);
    }

    return ComputeL2DistanceBetweenTwoVectors(VectorA.GetElementPointer(), VectorB.GetElementPointer(), LengthA, false);
}


template<typename ElementType>
inline
ElementType ComputeL2DistanceBetweenTwoVectors(const ElementType* VectorA, const ElementType* VectorB, int_max Length, bool CheckInput)
{
    if (CheckInput == true)
    {
        if (VectorA == nullptr || VectorB == nullptr || Length <= 0)
        {
            MDK_Error("Invalid input @ mdkLinearAlgebra_DenseMatrix ComputeL2DistanceBetweenTwoVectors(pointer ...)")
            return ElementType(-100);
        }
    }

    auto Distance = ElementType(0);

    for (int_max i = 0; i < Length; ++i)
    {
        auto temp = VectorA[i] - VectorB[i];
        Distance += temp*temp;
    }

    Distance = std::sqrt(Distance);

    return Distance;
}


// SingleVector is a column vector
// each column of VectorSet is a vector
// L2DistanceList[j] = distance between SingleVector and VectorSet(ALL, j)
template<typename ElementType>
DenseMatrix<ElementType> ComputeSquaredL2DistanceListFromSingleVectorToColVectorSet(const DenseMatrix<ElementType>& SingleVector,
                                                                                    const DenseMatrix<ElementType>& ColVectorSet)
{
    DenseMatrix<ElementType> SquaredL2DistanceList;

    auto Size = ColVectorSet.GetSize();

    if (Size.ColNumber == 0)
    {
        MDK_Error("Empty Dictionary @ mdkLinearAlgebra_DenseMatrix ComputeSquaredL2DistanceListFromSingleVectorToColVectorSet(...)")
        return SquaredL2DistanceList;
    }

    if (SingleVector.IsVector() == false)
    {
        MDK_Error("Input vector is not a vector @ mdkLinearAlgebra_DenseMatrix ComputeSquaredL2DistanceListFromSingleVectorToColVectorSet(...)")
        return SquaredL2DistanceList;
    }

    if (SingleVector.GetElementNumber() != Size.RowNumber)
    {
        MDK_Error("Size does not match @ mdkLinearAlgebra_DenseMatrix ComputeL2DistanceListFromSingleVectorToColVectorSet(...)")
        return SquaredL2DistanceList;
    }

    SquaredL2DistanceList.FastResize(1, Size.ColNumber);

    auto PointToSingleVector = SingleVector.GetElementPointer();

    auto PointerToSet = ColVectorSet.GetElementPointer();

    for (int_max j = 0; j < Size.ColNumber; ++j)
    {
        auto Vector_j = PointerToSet + j*Size.RowNumber;

        SquaredL2DistanceList[j] = ComputeSquaredL2DistanceBetweenTwoVectors(PointToSingleVector, Vector_j, Size.RowNumber, false);
    }

    return SquaredL2DistanceList;
}


template<typename ElementType>
inline
ElementType ComputeSquaredL2DistanceBetweenTwoVectors(const DenseMatrix<ElementType>& VectorA, const DenseMatrix<ElementType>& VectorB)
{
    if (VectorA.IsVector() == false || VectorB.IsVector() == false)
    {
        MDK_Error("Input VectorA or VectorB is not a vector @ mdkLinearAlgebra_DenseMatrix ComputeSquaredL2DistanceBetweenTwoVectors(...)")
        return ElementType(-100);
    }

    auto LengthA = VectorA.GetElementNumber();
    auto LengthB = VectorB.GetElementNumber();

    if (LengthA != LengthB)
    {
        MDK_Error("Size does not match @ mdkLinearAlgebra_DenseMatrix ComputeSquaredL2DistanceBetweenTwoVectors(...)")
        return ElementType(-100);
    }

    return ComputeSquaredL2DistanceBetweenTwoVectors(VectorA.GetElementPointer(), VectorB.GetElementPointer(), LengthA, false);
}


template<typename ElementType>
inline
ElementType ComputeSquaredL2DistanceBetweenTwoVectors(const ElementType* VectorA, const ElementType* VectorB, int_max Length, bool CheckInput)
{
    if (CheckInput == true)
    {
        if (VectorA == nullptr || VectorB == nullptr || Length <= 0)
        {
            MDK_Error("Invalid input @ mdkLinearAlgebra_DenseMatrix ComputeSquaredL2DistanceBetweenTwoVectors(pointer ...)")
            return ElementType(-100);
        }
    }

    auto SquaredDistance = ElementType(0);

    for (int_max i = 0; i < Length; ++i)
    {
        auto temp = VectorA[i] - VectorB[i];
        SquaredDistance += temp*temp;
    }

    return SquaredDistance;
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

    L1DistanceList.FastResize(1, Size.ColNumber);

    auto PointToSingleVector = SingleVector.GetElementPointer();

    auto PointerToSet = ColVectorSet.GetElementPointer();

    for (int_max j = 0; j < Size.ColNumber; ++j)
    {
        auto Vector_j = PointerToSet + j*Size.RowNumber;

        L1DistanceList[j] = ComputeL1DistanceBetweenTwoVectors(PointToSingleVector, Vector_j, Size.RowNumber, false);
    }

    return L1DistanceList;
}


template<typename ElementType>
inline
ElementType ComputeL1DistanceBetweenTwoVectors(const DenseMatrix<ElementType>& VectorA, const DenseMatrix<ElementType>& VectorB)
{
    if (VectorA.IsVector() == false || VectorB.IsVector() == false)
    {
        MDK_Error("Input VectorA or VectorB is not a vector @ mdkLinearAlgebra_DenseMatrix ComputeL1DistanceBetweenTwoVectors(...)")
        return ElementType(-100);
    }

    auto LengthA = VectorA.GetElementNumber();
    auto LengthB = VectorB.GetElementNumber();

    if (LengthA != LengthB)
    {
        MDK_Error("Size does not match @ mdkLinearAlgebra_DenseMatrix ComputeL1DistanceBetweenTwoVectors(...)")
        return ElementType(-100);
    }

    return ComputeL1DistanceBetweenTwoVectors(VectorA.GetElementPointer(), VectorB.GetElementPointer(), LengthA, false);
}


template<typename ElementType>
inline
ElementType ComputeL1DistanceBetweenTwoVectors(const ElementType* VectorA, const ElementType* VectorB, int_max Length, bool CheckInput)
{
    if (CheckInput == true)
    {
        if (VectorA == nullptr || VectorB == nullptr || Length <= 0)
        {
            MDK_Error("Invalid input @ mdkLinearAlgebra_DenseMatrix ComputeL1DistanceBetweenTwoVectors(pointer ...)")
            return ElementType(-100);
        }
    }

    auto Distance = ElementType(0);

    for (int_max i = 0; i < Length; ++i)
    {
        Distance += std::abs(VectorA[i] - VectorB[i]);
    }

    return Distance;
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

    CorrelationList.FastResize(1, Size.ColNumber);

    auto PointerToSingleVector = SingleVector.GetElementPointer();

    auto PointerToSet = ColVectorSet.GetElementPointer();

    for (int_max j = 0; j < Size.ColNumber; ++j)
    {
        auto Vector_j = PointerToSet + j*Size.RowNumber;

        CorrelationList[j] = ComputeCorrelationBetweenTwoVectors(PointerToSingleVector, Vector_j, Size.RowNumber, false);   
    }

    return CorrelationList;
}


template<typename ElementType>
inline
ElementType ComputeCorrelationBetweenTwoVectors(const DenseMatrix<ElementType>& VectorA, const DenseMatrix<ElementType>& VectorB)
{
    if (VectorA.IsVector() == false || VectorB.IsVector() == false)
    {
        MDK_Error("Input VectorA or VectorB is not a vector @ mdkLinearAlgebra_DenseMatrix ComputeCorrelationBetweenTwoVectors(...)")
        return ElementType(-100);
    }

    auto LengthA = VectorA.GetElementNumber();
    auto LengthB = VectorB.GetElementNumber();

    if (LengthA != LengthB)
    {
        MDK_Error("Size does not match @ mdkLinearAlgebra_DenseMatrix ComputeCorrelationBetweenTwoVectors(...)")
        return ElementType(-100);
    }

    return ComputeCorrelationBetweenTwoVectors(VectorA.GetElementPointer(), VectorB.GetElementPointer(), LengthA, false);
}


template<typename ElementType>
inline
ElementType ComputeCorrelationBetweenTwoVectors(const ElementType* VectorA, const ElementType* VectorB, int_max Length, bool CheckInput)
{
    if (CheckInput == true)
    {
        if (VectorA == nullptr || VectorB == nullptr || Length <= 0)
        {
            MDK_Error("Invalid input @ mdkLinearAlgebra_DenseMatrix ComputeCorrelationBetweenTwoVectors(pointer ...)")
            return ElementType(-100);
        }
    }

    auto Mean_A = ElementType(0);

    auto Mean_B = ElementType(0);

    for (int_max i = 0; i < Length; ++i)
    {
        Mean_A += VectorA[i];
        Mean_B += VectorB[i];
    }
    Mean_A /= ElementType(Length);
    Mean_B /= ElementType(Length);

    ElementType part_AB = ElementType(0);
    ElementType part_AA = ElementType(0);
    ElementType part_BB = ElementType(0);

    for (int_max i = 0; i < Length; ++i)
    {
        part_AB += (VectorA[i] - Mean_A)*(VectorB[i] - Mean_B);
        
        part_AA += (VectorA[i] - Mean_A)*(VectorA[i] - Mean_A);

        part_BB += (VectorB[i] - Mean_B)*(VectorB[i] - Mean_B);
    }

    auto Correlation = ElementType(0);

    auto eps_value = std::numeric_limits<ElementType>::epsilon();

    if (std::abs(part_AA) > eps_value && std::abs(part_BB) > eps_value)
    { 
        Correlation = part_AB / (std::sqrt(part_AA) * std::sqrt(part_BB));
    }
    else if (std::abs(part_AA) <= eps_value && std::abs(part_BB) <= eps_value)
    {
        Correlation = 1;
    }
    else // (std::abs(part_AA) <= eps_value && std::abs(part_BB) > eps_value) || (std::abs(part_AA) > eps_value && std::abs(part_BB) <= eps_value)
    {
        Correlation = 0;
    }
    
    return Correlation;
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

    CorrelationList.FastResize(1, Size.ColNumber);

    auto PointerToSingleVector = SingleVector.GetElementPointer();

    auto PointerToSet = ColVectorSet.GetElementPointer();

    for (int_max j = 0; j < Size.ColNumber; ++j)
    {
        auto Vector_j = PointerToSet + j*Size.RowNumber;

        CorrelationList[j] = ComputeUncenteredCorrelationBetweenTwoVectors(PointerToSingleVector, Vector_j, Size.RowNumber, false);
    }

    return CorrelationList;
}


template<typename ElementType>
inline
ElementType ComputeUncenteredCorrelationBetweenTwoVectors(const DenseMatrix<ElementType>& VectorA, const DenseMatrix<ElementType>& VectorB)
{
    if (VectorA.IsVector() == false || VectorB.IsVector() == false)
    {
        MDK_Error("Input VectorA or VectorB is not a vector @ mdkLinearAlgebra_DenseMatrix ComputeUncenteredCorrelationBetweenTwoVectors(...)")
        return ElementType(-100);
    }

    auto LengthA = VectorA.GetElementNumber();
    auto LengthB = VectorB.GetElementNumber();

    if (LengthA != LengthB)
    {
        MDK_Error("Size does not match @ mdkLinearAlgebra_DenseMatrix ComputeUncenteredCorrelationBetweenTwoVectors(...)")
        return ElementType(-100);
    }

    return ComputeUncenteredCorrelationBetweenTwoVectors(VectorA.GetElementPointer(), VectorB.GetElementPointer(), LengthA, false);
}


template<typename ElementType>
inline
ElementType ComputeUncenteredCorrelationBetweenTwoVectors(const ElementType* VectorA, const ElementType* VectorB, int_max Length, bool CheckInput)
{
    if (CheckInput == true)
    {
        if (VectorA == nullptr || VectorB == nullptr || Length <= 0)
        {
            MDK_Error("Invalid input @ mdkLinearAlgebra_DenseMatrix ComputeUncenteredCorrelationBetweenTwoVectors(pointer ...)")
            return ElementType(-100);
        }
    }

    auto part_AB = ElementType(0);
    auto part_AA = ElementType(0);
    auto part_BB = ElementType(0);

    for (int_max i = 0; i < Length; ++i)
    {
        part_AB += VectorA[i] * VectorB[i];

        part_AA += VectorA[i] * VectorA[i];
        
        part_BB += VectorB[i] * VectorB[i];
    }

    auto Correlation = ElementType(0);

    auto eps_value = std::numeric_limits<ElementType>::epsilon();

    if (std::abs(part_AA) > eps_value && std::abs(part_BB) > eps_value)
    {
        Correlation = part_AB / (std::sqrt(part_AA) * std::sqrt(part_BB));
    }
    else if (std::abs(part_AA) <= eps_value && std::abs(part_BB) <= eps_value)
    {
        Correlation = 1;
    }
    else // (std::abs(part_AA) <= eps_value && std::abs(part_BB) > eps_value) || (std::abs(part_AA) > eps_value && std::abs(part_BB) <= eps_value)
    {
        Correlation = 0;
    }

    return Correlation;
}


template<typename ElementType>
DenseMatrix<ElementType> ComputeUnnormalizedCorrelationListFromSingleVectorToColVectorSet(const DenseMatrix<ElementType>& SingleVector,
                                                                                          const DenseMatrix<ElementType>& ColVectorSet)
{
    DenseMatrix<ElementType> CorrelationList;

    auto Size = ColVectorSet.GetSize();

    if (Size.ColNumber == 0)
    {
        MDK_Error("Empty Dictionary @ mdkLinearAlgebra_DenseMatrix ComputeUnnormalizedCorrelationListFromSingleVectorToColVectorSet(...)")
        return CorrelationList;
    }

    if (SingleVector.IsVector() == false)
    {
        MDK_Error("Input vector is not a vector @ mdkLinearAlgebra_DenseMatrix ComputeUnnormalizedCorrelationListFromSingleVectorToColVectorSet(...)")
        return CorrelationList;
    }

    if (SingleVector.GetElementNumber() != Size.RowNumber)
    {
        MDK_Error("Size does not match @ mdkLinearAlgebra_DenseMatrix ComputeUnnormalizedCorrelationListFromSingleVectorToColVectorSet(...)")
        return CorrelationList;
    }

    CorrelationList.FastResize(1, Size.ColNumber);

    auto PointerToSingleVector = SingleVector.GetElementPointer();

    auto PointerToSet = ColVectorSet.GetElementPointer();

    for (int_max j = 0; j < Size.ColNumber; ++j)
    {
        auto Vector_j = PointerToSet + j*Size.RowNumber;

        CorrelationList[j] = ComputeUnnormalizedCorrelationBetweenTwoVectors(PointerToSingleVector, Vector_j, Size.RowNumber, false);
    }

    return CorrelationList;
}


template<typename ElementType>
inline
ElementType ComputeUnnormalizedCorrelationBetweenTwoVectors(const DenseMatrix<ElementType>& VectorA, const DenseMatrix<ElementType>& VectorB)
{
    if (VectorA.IsVector() == false || VectorB.IsVector() == false)
    {
        MDK_Error("Input VectorA or VectorB is not a vector @ mdkLinearAlgebra_DenseMatrix ComputeUnnormalizedCorrelationBetweenTwoVectors(...)")
        return ElementType(-100);
    }

    auto LengthA = VectorA.GetElementNumber();
    auto LengthB = VectorB.GetElementNumber();

    if (LengthA != LengthB)
    {
        MDK_Error("Size does not match @ mdkLinearAlgebra_DenseMatrix ComputeUnnormalizedCorrelationBetweenTwoVectors(...)")
        return ElementType(-100);
    }

    return ComputeUnnormalizedCorrelationBetweenTwoVectors(VectorA.GetElementPointer(), VectorB.GetElementPointer(), LengthA, false);
}


template<typename ElementType>
inline
ElementType ComputeUnnormalizedCorrelationBetweenTwoVectors(const ElementType* VectorA, const ElementType* VectorB, int_max Length, bool CheckInput)
{
    if (CheckInput == true)
    {
        if (VectorA == nullptr || VectorB == nullptr || Length <= 0)
        {
            MDK_Error("Invalid input @ mdkLinearAlgebra_DenseMatrix ComputeUnnormalizedCorrelationBetweenTwoVectors(pointer ...)")
            return ElementType(-100);
        }
    }

    auto Mean_A = ElementType(0);

    auto Mean_B = ElementType(0);

    for (int_max i = 0; i < Length; ++i)
    {
        Mean_A += VectorA[i];
        Mean_B += VectorB[i];
    }
    Mean_A /= ElementType(Length);
    Mean_B /= ElementType(Length);

    auto Correlation = ElementType(0);

    for (int_max i = 0; i < Length; ++i)
    {
        Correlation += (VectorA[i] - Mean_A)*(VectorB[i] - Mean_B);
    }

    return Correlation;
}


template<typename ElementType>
DenseMatrix<ElementType> ComputeKLDivergenceListOfSingleVectorFromColVectorSet(const DenseMatrix<ElementType>& SingleVector,
                                                                               const DenseMatrix<ElementType>& ColVectorSet)
{
    DenseMatrix<ElementType> KLDivergenceList;

    auto Size = ColVectorSet.GetSize();

    if (Size.ColNumber == 0)
    {
        MDK_Error("Empty Dictionary @ mdkLinearAlgebra_DenseMatrix ComputeKLDivergenceListOfSingleVectorFromColVectorSet(...)")
        return KLDivergenceList;
    }

    if (SingleVector.IsVector() == false)
    {
        MDK_Error("Input vector is not a vector @ mdkLinearAlgebra_DenseMatrix ComputeKLDivergenceListOfSingleVectorFromColVectorSet(...)")
        return KLDivergenceList;
    }

    if (SingleVector.GetElementNumber() != Size.RowNumber)
    {
        MDK_Error("Size does not match @ mdkLinearAlgebra_DenseMatrix ComputeKLDivergenceOfSingleVectorFromColVectorSet(...)")
        return KLDivergenceList;
    }

    KLDivergenceList.FastResize(1, Size.ColNumber);

    auto PointToSingleVector = SingleVector.GetElementPointer();

    auto PointerToSet = ColVectorSet.GetElementPointer();

    for (int_max j = 0; j < Size.ColNumber; ++j)
    {
        auto Vector_j = PointerToSet + j*Size.RowNumber;

        KLDivergenceList[j] = ComputeKLDivergenceOfVectorAFromVectorB(PointToSingleVector, Vector_j, Size.RowNumber, false);
    }

    return KLDivergenceList;
}


template<typename ElementType>
inline
ElementType ComputeKLDivergenceOfVectorAFromVectorB(const DenseMatrix<ElementType>& VectorA, const DenseMatrix<ElementType>& VectorB)
{
// VectorA and VectorB should >= 0

    if (VectorA.IsVector() == false || VectorB.IsVector() == false)
    {
        MDK_Error("Input VectorA or VectorB is not a vector @ mdkLinearAlgebra_DenseMatrix ComputeKLDivergenceOfVectorAFromVectorB(...)")
        return ElementType(-100); // Kullback�Leibler divergence is always non-negative
    }

    auto LengthA = VectorA.GetElementNumber();
    auto LengthB = VectorB.GetElementNumber();

    if (LengthA != LengthB)
    {
        MDK_Error("Size does not match @ mdkLinearAlgebra_DenseMatrix ComputeKLDivergenceOfVectorAFromVectorB(...)")
        return ElementType(-100);
    }

    return ComputeKLDivergenceOfVectorAFromVectorB(VectorA.GetElementPointer(), VectorB.GetElementPointer(), LengthA, false);
}


template<typename ElementType>
inline
ElementType ComputeKLDivergenceOfVectorAFromVectorB(const ElementType* VectorA, const ElementType* VectorB, int_max Length, bool CheckInput)
{
// VectorA and VectorB should >= 0

    if (CheckInput == true)
    {
        if (VectorA == nullptr || VectorB == nullptr || Length <= 0)
        {
            MDK_Error("Invalid input @ mdkLinearAlgebra_DenseMatrix ComputeKLDivergenceOfVectorAFromVectorB(pointer ...)")
            return ElementType(-100);
        }
    }

    auto KLDivergence = ElementType(0);

    auto eps_value = std::numeric_limits<ElementType>::epsilon();

    for (int_max i = 0; i < Length; ++i)
    {
        if (VectorA[i] > eps_value && VectorB[i] > eps_value)
        {
            KLDivergence += VectorB[i] * (std::log(VectorB[i] / VectorA[i]));
        }
        else if (VectorA[i] < ElementType(0) || VectorB[i] < ElementType(0))
        {
            MDK_Error("VectorA[i] < 0 || VectorB[i] < 0 @ mdkLinearAlgebra_DenseMatrix ComputeKLDivergenceOfVectorAFromVectorB(pointer ...)")
            return ElementType(-100);
        }
    }

    return KLDivergence;
}


}

#endif