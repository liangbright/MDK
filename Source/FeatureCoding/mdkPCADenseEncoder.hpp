#ifndef __mdkPCADenseEncoder_hpp
#define __mdkPCADenseEncoder_hpp

namespace mdk
{

template<typename ElementType>
PCADenseEncoder<ElementType>::PCADenseEncoder()
{
}


template<typename ElementType>
PCADenseEncoder<ElementType>::~PCADenseEncoder()
{
}


template<typename ElementType>
void PCADenseEncoder<ElementType>::EncodingFunction(const DenseMatrix<ElementType>& DataColVector, DenseMatrix<ElementType>& CodeInDenseColVector)
{
    auto D = m_Dictionary->BasisMatrix();

    auto CodeLength = D.GetColNumber();

    auto DataDimension = DataColVector.GetElementNumber();

    CodeInDenseColVector.FastResize(CodeLength, 1);

    auto tempData = MatrixSubtract(DataColVector, m_Dictionary->m_MeanVector);

    for (int_max j = 0; j < CodeLength; ++i)
    {
        auto temp = ElementType(0);

        auto dj = D->GetElementPointerOfCol(j);

        for (int_max i = 0; i < DataDimension; ++i)
        {
            temp += tempData[i] * dj[i];
        }

        CodeInDenseColVector[j] = temp;
    }
}



}// namespace mdk


#endif