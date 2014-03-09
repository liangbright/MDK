#ifndef __mdkGlueMatrixForMultiplication_hpp
#define __mdkGlueMatrixForMultiplication_hpp

//#include "mdkLinearAlgebra.h"

namespace mdk
{

template<typename ElementType>
mdkGlueMatrixForMultiplication<ElementType>::mdkGlueMatrixForMultiplication()
{
    this->Reset();
}


template<typename ElementType>
mdkGlueMatrixForMultiplication<ElementType>::~mdkGlueMatrixForMultiplication()
{

}


template<typename ElementType>
mdkGlueMatrixForMultiplication<ElementType>::mdkGlueMatrixForMultiplication(mdkGlueMatrixForMultiplication<ElementType>&& GlueMatrix)
{
    m_RowNumber = GlueMatrix.m_RowNumber;

    m_ColNumber = GlueMatrix.m_ColNumber;

    m_SourceMatrixSharedCopyList = std::move(GlueMatrix.m_SourceMatrixSharedCopyList);

    m_Element_Coef = std::move(GlueMatrix.m_Element_Coef);

    // clear counter 
    GlueMatrix.m_RowNumber = 0;
    GlueMatrix.m_ColNumber = 0;
}


template<typename ElementType>
inline
void mdkGlueMatrixForMultiplication<ElementType>::Reset()
{
    m_RowNumber = 0;

    m_ColNumber = 0;

    m_SourceMatrixSharedCopyList.resize(0);

    m_SourceMatrixSharedCopyList.reserve(MDK_GlueMatrixForMultiplication_ReservedCapacity);

    m_Element_Coef = ElementType(1);
}


template<typename ElementType>
inline
uint64 mdkGlueMatrixForMultiplication<ElementType>::GetRowNumber() const
{
    return m_RowNumber;
}


template<typename ElementType>
inline
uint64 mdkGlueMatrixForMultiplication<ElementType>::GetColNumber() const
{
    return m_ColNumber;
}


template<typename ElementType>
inline
uint64 mdkGlueMatrixForMultiplication<ElementType>::GetElementNumber() const
{
    return m_RowNumber*m_ColNumber;
}


template<typename ElementType>
inline
mdkMatrixSize mdkGlueMatrixForMultiplication<ElementType>::GetSize() const
{
    mdkMatrixSize Size;

    Size.RowNumber = m_RowNumber;

    Size.ColNumber = m_ColNumber;

    return Size;
}


template<typename ElementType>
inline
bool mdkGlueMatrixForMultiplication<ElementType>::IsEmpty() const
{
    if (m_RowNumber == 0)
    {
        return true;
    }

    return false;
}


template<typename ElementType>
inline
mdkMatrix<ElementType> mdkGlueMatrixForMultiplication<ElementType>::CreateMatrix() const
{
    mdkMatrix<ElementType> tempMatrix;

    if (m_RowNumber == 0)
    {
        mdkWarning << "return empty matrix @ mdkGlueMatrixForMultiplication::CreateMatrix()" << '\n';

        return tempMatrix;
    }

    tempMatrix.Resize(m_RowNumber, m_ColNumber);

    this->CreateMatrix(tempMatrix);

    return tempMatrix;
}


template<typename ElementType>
inline 
void mdkGlueMatrixForMultiplication<ElementType>::CreateMatrix(mdkMatrix<ElementType>& OutputMatrix) const
{
    if (m_RowNumber != OutputMatrix.GetRowNumber() || m_ColNumber != OutputMatrix.GetColNumber())
    {
        if (OutputMatrix.IsSizeFixed() == false)
        {
            OutputMatrix.Clear();
        }
        else
        {
            mdkError << "Size does not match @ mdkGlueMatrixForMultiplication::CreateMatrix(OutputMatrix)" << '\n';
            return;
        }
    }

    auto MatrixNumber = m_SourceMatrixSharedCopyList.size();

    if (MatrixNumber == 0)
    {
        mdkError << "MatrixNumber is zero @ mdkGlueMatrixForMultiplication::CreateMatrix(OutputMatrix)" << '\n';
        return;
    }

    if (MatrixNumber == 1)
    {
        OutputMatrix.Take(m_SourceMatrixSharedCopyList[0].ElementOperation("*", m_Element_Coef));
        
        return;
    }

    if (MatrixNumber == 2)
    {
        OutputMatrix = MatrixMultiply(m_SourceMatrixSharedCopyList[0], m_SourceMatrixSharedCopyList[1]);   
        OutputMatrix.ElementOperationInPlace("*", m_Element_Coef);

        return;
    }

    // MatrixNumber >= 3

    // output is a scalar (in matrix form) ------------------------------------------
    if (m_RowNumber == 1 && m_ColNumber == 1)
    {
        OutputMatrix = MatrixMultiply(m_SourceMatrixSharedCopyList[0], m_SourceMatrixSharedCopyList[1]);
        
        for (uint64 i = 2; i < MatrixNumber; i++)
        {
            OutputMatrix = MatrixMultiply(OutputMatrix, m_SourceMatrixSharedCopyList[i]);
        }

        return;
    }
    //---------------------------------------------------------------------------------


    //General : find the best combination to get the result -------------------------------------------------------------

    auto MatrixPointerList = std::vector<const mdkMatrix<ElementType>*>(MatrixNumber);

    for (uint64 i = 0; i < MatrixNumber - 1; i++)
    {
        MatrixPointerList[i] = &m_SourceMatrixSharedCopyList[i];
    }

    auto ResultMatrixList = std::vector<mdkMatrix<ElementType>>(MatrixNumber);


    while (true)
    {
        auto CurMatrixNumber = ResultMatrixList.size();

        if (CurMatrixNumber == 1)
        {
            break;
        }

        uint64 Max_ElementNumber_Diff = 0;

        uint64 RelativeIndex_BestMatrixPair = 0;

        for (uint64 i = 0; i < CurMatrixNumber - 1; i++)
        {
            auto tempRowNumber_a = MatrixPointerList[i]->GetRowNumber();
            auto tempColNumber_a = MatrixPointerList[i]->GetRowNumber();

            auto tempRowNumber_b = MatrixPointerList[i + 1]->GetRowNumber();
            auto tempColNumber_b = MatrixPointerList[i + 1]->GetRowNumber();

            auto ElementNumber_Diff = (tempRowNumber_a*tempRowNumber_a + tempRowNumber_b*tempRowNumber_b) - 2 * tempRowNumber_a * tempColNumber_b;

            if (ElementNumber_Diff > Max_ElementNumber_Diff)
            {
                Max_ElementNumber_Diff = ElementNumber_Diff;

                RelativeIndex_BestMatrixPair = i;
            }

            // Row_vector * Matrix  or Matrix * Col_vector
            // then prefer this
            if (tempRowNumber_a == 1 || tempColNumber_b == 1)
            {
                RelativeIndex_BestMatrixPair = i;
            }
        }

        mdkMatrix<ElementType> tempMatrix = MatrixMultiply(*MatrixPointerList[RelativeIndex_BestMatrixPair],
                                                           *MatrixPointerList[RelativeIndex_BestMatrixPair + 1]);

        // change MatrixPointerList and ResultMatrixList

        ResultMatrixList.erase(ResultMatrixList.begin() + RelativeIndex_BestMatrixPair);

        ResultMatrixList[RelativeIndex_BestMatrixPair].Take(tempMatrix);

        MatrixPointerList.erase(MatrixPointerList.begin() + RelativeIndex_BestMatrixPair);

        MatrixPointerList[RelativeIndex_BestMatrixPair] = ResultMatrixList.data() + RelativeIndex_BestMatrixPair;

    }
    
    OutputMatrix.Take(ResultMatrixList[0]);

}


template<typename ElementType>
inline 
mdkMatrix<ElementType> mdkGlueMatrixForMultiplication<ElementType>::ElementMultiply(const mdkMatrix<ElementType>& targetMatrix)
{
    auto tempMatrix = this->CreateMatrix();

    return tempMatrix.ElementMultiply(targetMatrix);
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> mdkGlueMatrixForMultiplication<ElementType>::ElementMultiply(const mdkShadowMatrix<ElementType>& ShadowMatrix)
{
    auto tempMatrix = this->CreateMatrix();

    return tempMatrix.ElementMultiply(ShadowMatrix);
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> mdkGlueMatrixForMultiplication<ElementType>::ElementMultiply(const mdkLinearCombineGlueMatrix<ElementType>& GlueMatrix)
{
    auto tempMatrix = this->CreateMatrix();

    return tempMatrix.ElementMultiply(GlueMatrix);
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> mdkGlueMatrixForMultiplication<ElementType>::ElementMultiply(const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrix)
{
    auto tempMatrix = this->CreateMatrix();

    return tempMatrix.ElementMultiply(GlueMatrix);
}

}// end namespace mdk


#endif
