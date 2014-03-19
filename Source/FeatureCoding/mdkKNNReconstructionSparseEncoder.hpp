#ifndef __mdkKNNSoftAssignSparseEncoder_hpp
#define __mdkKNNSoftAssignSparseEncoder_hpp

namespace mdk
{

template<typename ElementType>
mdkKNNSoftAssignSparseEncoder<ElementType>::mdkKNNSoftAssignSparseEncoder()
{

}


template<typename ElementType>
mdkKNNSoftAssignSparseEncoder<ElementType>::~mdkKNNSoftAssignSparseEncoder()
{

}


template<typename ElementType>
mdkKNNSoftAssignSparseEncoder<ElementType>::Reset()
{
    this->mdkFeatureSparseEncoder::Reset();

    m_MaxNumberOfNeighbours = 5;
}



template<typename ElementType>
mdkKNNSoftAssignSparseEncoder<ElementType>::Clear()
{
    this->mdkFeatureSparseEncoder::Clear();

    m_MaxNumberOfNeighbours = 5;
}


template<typename ElementType>
bool mdkKNNSoftAssignSparseEncoder<ElementType>::SetMaxNumberOfNeighbours(int64 MaxNumberOfNeighbours)
{
    if (MaxNumberOfNeighbours <= 0)
    {
        mdkError << "Invalid input @ mdkKNNSoftAssignSparseEncoder::SetMaxNumberOfNeighbours(MaxNumberOfNeighbours)" << '\n';
        return false;
    }

    m_MaxNumberOfNeighbours = MaxNumberOfNeighbours;

    return true;
}


template<typename ElementType>
bool mdkKNNSoftAssignSparseEncoder<ElementType>::GenerateCode()
{
    return true;
}


}// namespace mdk


#endif