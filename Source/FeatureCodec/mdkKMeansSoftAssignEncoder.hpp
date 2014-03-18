#ifndef __mdkKMeansSoftAssignEncoder_hpp
#define __mdkKMeansSoftAssignEncoder_hpp

namespace mdk
{

template<typename ElementType>
mdkKMeansSoftAssignEncoder<ElementType>::mdkKMeansSoftAssignEncoder()
{

}


template<typename ElementType>
mdkKMeansSoftAssignEncoder<ElementType>::~mdkKMeansSoftAssignEncoder()
{

}


template<typename ElementType>
mdkKMeansSoftAssignEncoder<ElementType>::Reset()
{
    this->mdkFeatureEncoder::Reset();

    m_MaxNumberOfNeighbours = 5;
}



template<typename ElementType>
mdkKMeansSoftAssignEncoder<ElementType>::Clear()
{
    this->mdkFeatureEncoder::Clear();

    m_MaxNumberOfNeighbours = 5;
}


template<typename ElementType>
bool mdkKMeansSoftAssignEncoder<ElementType>::SetMaxNumberOfNeighbours(int64 MaxNumberOfNeighbours)
{
    if (MaxNumberOfNeighbours <= 0)
    {
        mdkError << "Invalid input @ mdkFeatureEncoder::SetMaxNumberOfNeighbours(MaxNumberOfNeighbours)" << '\n';
        return false;
    }

    m_MaxNumberOfNeighbours = MaxNumberOfNeighbours;

    return true;
}


template<typename ElementType>
bool mdkKMeansSoftAssignEncoder<ElementType>::GenerateCode()
{
    return true;
}


}// namespace mdk


#endif