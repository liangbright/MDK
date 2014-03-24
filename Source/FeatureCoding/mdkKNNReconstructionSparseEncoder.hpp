#ifndef __mdkKNNSoftAssignSparseEncoder_hpp
#define __mdkKNNSoftAssignSparseEncoder_hpp

namespace mdk
{

template<typename ElementType>
KNNSoftAssignSparseEncoder<ElementType>::KNNSoftAssignSparseEncoder()
{

}


template<typename ElementType>
KNNSoftAssignSparseEncoder<ElementType>::~KNNSoftAssignSparseEncoder()
{

}


template<typename ElementType>
KNNSoftAssignSparseEncoder<ElementType>::Clear()
{
    this->FeatureSparseEncoder::Clear();

    m_MaxNumberOfNeighbours = 5;
}


template<typename ElementType>
bool KNNSoftAssignSparseEncoder<ElementType>::SetMaxNumberOfNeighbours(int64 MaxNumberOfNeighbours)
{
    if (MaxNumberOfNeighbours <= 0)
    {
        MDK_Error << "Invalid input @ KNNSoftAssignSparseEncoder::SetMaxNumberOfNeighbours(MaxNumberOfNeighbours)" << '\n';
        return false;
    }

    m_MaxNumberOfNeighbours = MaxNumberOfNeighbours;

    return true;
}


template<typename ElementType>
bool KNNSoftAssignSparseEncoder<ElementType>::GenerateCode()
{
    return true;
}


}// namespace mdk


#endif