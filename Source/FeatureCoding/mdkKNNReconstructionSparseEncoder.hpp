#ifndef __mdkKNNReconstructionSparseEncoder_hpp
#define __mdkKNNReconstructionSparseEncoder_hpp

//#include "mdkKNNReconstructionSparseEncoder.h"

namespace mdk
{

template<typename ElementType>
KNNReconstructionSparseEncoder<ElementType>::KNNReconstructionSparseEncoder()
{
    this->Clear();
}


template<typename ElementType>
KNNReconstructionSparseEncoder<ElementType>::~KNNReconstructionSparseEncoder()
{

}


template<typename ElementType>
void KNNReconstructionSparseEncoder<ElementType>::Clear()
{
    this->FeatureDictionaryBasedSparseEncoder::Clear();

    m_MaxNumberOfNeighbours = 5;
}


template<typename ElementType>
bool KNNReconstructionSparseEncoder<ElementType>::SetMaxNumberOfNeighbours(int64 MaxNumberOfNeighbours)
{
    if (MaxNumberOfNeighbours <= 0)
    {
        MDK_Error << "Invalid input @ KNNReconstructionSparseEncoder::SetMaxNumberOfNeighbours(MaxNumberOfNeighbours)" << '\n';
        return false;
    }

    m_MaxNumberOfNeighbours = MaxNumberOfNeighbours;

    return true;
}


template<typename ElementType>
bool KNNReconstructionSparseEncoder<ElementType>::GenerateCode()
{
    return true;
}


}// namespace mdk


#endif