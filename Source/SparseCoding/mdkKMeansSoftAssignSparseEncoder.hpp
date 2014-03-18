#ifndef __mdkKMeansSoftAssignSparseEncoder_hpp
#define __mdkKMeansSoftAssignSparseEncoder_hpp

namespace mdk
{

template<typename ElementType>
mdkKMeansSoftAssignSparseEncoder<ElementType>::mdkKMeansSoftAssignSparseEncoder()
{

}


template<typename ElementType>
mdkKMeansSoftAssignSparseEncoder<ElementType>::~mdkKMeansSoftAssignSparseEncoder()
{

}


template<typename ElementType>
mdkKMeansSoftAssignSparseEncoder<ElementType>::Reset()
{
    this->mdkFeatureSparseEncoder::Reset();

    m_MaxNumberOfNeighbours = 5;
}



template<typename ElementType>
mdkKMeansSoftAssignSparseEncoder<ElementType>::Clear()
{
    this->mdkFeatureSparseEncoder::Clear();

    m_MaxNumberOfNeighbours = 5;
}


template<typename ElementType>
bool mdkKMeansSoftAssignSparseEncoder<ElementType>::SetMaxNumberOfNeighbours(int64 MaxNumberOfNeighbours)
{
    if (MaxNumberOfNeighbours <= 0)
    {
        mdkError << "Invalid input @ mdkKMeansSoftAssignSparseEncoder::SetMaxNumberOfNeighbours(MaxNumberOfNeighbours)" << '\n';
        return false;
    }

    m_MaxNumberOfNeighbours = MaxNumberOfNeighbours;

    return true;
}


template<typename ElementType>
bool mdkKMeansSoftAssignSparseEncoder<ElementType>::GenerateCode()
{
    return true;
}


}// namespace mdk


#endif