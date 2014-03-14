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

    m_SharedSourceMatrixList = std::move(GlueMatrix.m_SharedSourceMatrixList);

    m_Element_Coef = std::move(GlueMatrix.m_Element_Coef);

    m_Is_m_Element_Coef_Equal_to_One = GlueMatrix.m_Is_m_Element_Coef_Equal_to_One;
    
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

    m_SharedSourceMatrixList.resize(0);

    m_SharedSourceMatrixList.reserve(MDK_GlueMatrixForMultiplication_ReservedCapacity);

    m_Element_Coef = ElementType(1);

    m_Is_m_Element_Coef_Equal_to_One = true;
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
uint64 mdkGlueMatrixForMultiplication<ElementType>::GetMatrixNumber() const
{
    return m_SharedSourceMatrixList.size();
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

    this->CreateMatrix(tempMatrix);

    return tempMatrix;
}


template<typename ElementType>
inline 
bool mdkGlueMatrixForMultiplication<ElementType>::CreateMatrix(mdkMatrix<ElementType>& OutputMatrix) const
{
    if (m_RowNumber != OutputMatrix.GetRowNumber() || m_ColNumber != OutputMatrix.GetColNumber())
    {
        if (OutputMatrix.IsSizeFixed() == false)
        {
            OutputMatrix.Clear();

            OutputMatrix.Resize(m_RowNumber, m_ColNumber);
        }
        else
        {
            mdkError << "Size does not match @ mdkGlueMatrixForMultiplication::CreateMatrix(OutputMatrix)" << '\n';
            return false;
        }
    }

    auto MatrixNumber = m_SharedSourceMatrixList.size();

    if (MatrixNumber == 0)
    {
        mdkError << "MatrixNumber is zero @ mdkGlueMatrixForMultiplication::CreateMatrix(OutputMatrix)" << '\n';
        return false;
    }

    if (MatrixNumber == 1)
    {
        if (m_Is_m_Element_Coef_Equal_to_One == true)
        {
            mdkError << "MatrixNumber is 1 and  m_Is_m_Element_Coef_Equal_to_One = true @ mdkGlueMatrixForMultiplication::CreateMatrix(OutputMatrix)" << '\n';
            OutputMatrix.Copy(m_SharedSourceMatrixList[0]);
            return false;
        }
        
        MatrixElementMultiply(OutputMatrix, m_SharedSourceMatrixList[0], m_Element_Coef);        
        
        return true;
    }

    if (MatrixNumber == 2)
    {
         MatrixMultiply(OutputMatrix, m_SharedSourceMatrixList[0], m_SharedSourceMatrixList[1]);

         if (m_Is_m_Element_Coef_Equal_to_One == false)
         {
             MatrixElementMultiply(OutputMatrix, OutputMatrix, m_Element_Coef);
         }
         
        return true;
    }

    // MatrixNumber >= 3

    // output is a vector or scalar (in matrix form) ------------------------------------------
    if (m_RowNumber == 1)
    {
        MatrixMultiply(OutputMatrix, m_SharedSourceMatrixList[0], m_SharedSourceMatrixList[1]);
        
        for (uint64 i = 2; i < MatrixNumber; ++i)
        {
            OutputMatrix = MatrixMultiply(OutputMatrix, m_SharedSourceMatrixList[i]);
        }

        if (m_Is_m_Element_Coef_Equal_to_One == false)
        {
            MatrixElementMultiply(OutputMatrix, OutputMatrix, m_Element_Coef);
        }

        return true;
    }

    // output is a vector or scalar (in matrix form) ------------------------------------------
    if (m_ColNumber == 1)
    {
        MatrixMultiply(OutputMatrix, m_SharedSourceMatrixList[MatrixNumber - 2], m_SharedSourceMatrixList[MatrixNumber - 1]);

        for (uint64 i = MatrixNumber-3; i >= 0; --i)
        {
            OutputMatrix = MatrixMultiply(m_SharedSourceMatrixList[i], OutputMatrix);
        }

        if (m_Is_m_Element_Coef_Equal_to_One == false)
        {
            MatrixElementMultiply(OutputMatrix, OutputMatrix, m_Element_Coef);
        }

        return true;
    }

    //---------------------------------------------------------------------------------


    //General : find the best combination to get the result -------------------------------------------------------------

    auto MatrixPointerList = std::vector<const mdkMatrix<ElementType>*>(MatrixNumber);

    for (uint64 i = 0; i < MatrixNumber - 1; ++i)
    {
        MatrixPointerList[i] = &m_SharedSourceMatrixList[i];
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

        for (uint64 i = 0; i < CurMatrixNumber - 1; ++i)
        {
            auto tempRowNumber_a = MatrixPointerList[i]->GetRowNumber();
            auto tempColNumber_a = MatrixPointerList[i]->GetRowNumber();

            auto tempRowNumber_b = MatrixPointerList[i + 1]->GetRowNumber();
            auto tempColNumber_b = MatrixPointerList[i + 1]->GetRowNumber();

            auto ElementNumber_Diff = (tempRowNumber_a*tempColNumber_a + tempRowNumber_b*tempColNumber_b) - 2 * tempRowNumber_a * tempColNumber_b;

            if (ElementNumber_Diff > Max_ElementNumber_Diff)
            {
                Max_ElementNumber_Diff = ElementNumber_Diff;

                RelativeIndex_BestMatrixPair = i;
            }
        }

        auto tempMatrix = MatrixMultiply(*MatrixPointerList[RelativeIndex_BestMatrixPair], *MatrixPointerList[RelativeIndex_BestMatrixPair + 1]);

        // change MatrixPointerList and ResultMatrixList

        ResultMatrixList.erase(ResultMatrixList.begin() + RelativeIndex_BestMatrixPair);

        ResultMatrixList[RelativeIndex_BestMatrixPair].Take(tempMatrix);

        MatrixPointerList.erase(MatrixPointerList.begin() + RelativeIndex_BestMatrixPair);

        MatrixPointerList[RelativeIndex_BestMatrixPair] = &ResultMatrixList[RelativeIndex_BestMatrixPair];

    }// while
    
    // take the final result
    OutputMatrix.Take(ResultMatrixList[0]);

    if (m_Is_m_Element_Coef_Equal_to_One == false)
    {
        MatrixElementMultiply(OutputMatrix, OutputMatrix, m_Element_Coef);
    }

    return true;
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> mdkGlueMatrixForMultiplication<ElementType>::ElementMultiply(const mdkMatrix<ElementType>& targetMatrix)
{
    auto tempMatrix = this->CreateMatrix();

    MatrixElementMultiply(tempMatrix, tempMatrix, targetMatrix);

    return tempMatrix;
}


template<typename ElementType>
inline
mdkMatrix<ElementType> mdkGlueMatrixForMultiplication<ElementType>::ElementMultiply(const ElementType& Element)
{
    auto tempMatrix = this->CreateMatrix();

    MatrixElementMultiply(tempMatrix, tempMatrix, Element);

    return tempMatrix;
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> mdkGlueMatrixForMultiplication<ElementType>::ElementMultiply(const mdkShadowMatrix<ElementType>& ShadowMatrix)
{
    auto tempMatrix = this->CreateMatrix();

    MatrixElementMultiply(tempMatrix, tempMatrix, ShadowMatrix.CreateMatrix());

    return tempMatrix;
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> mdkGlueMatrixForMultiplication<ElementType>::ElementMultiply(const mdkLinearCombineGlueMatrix<ElementType>& GlueMatrix)
{
    auto tempMatrix = this->CreateMatrix();

    MatrixElementMultiply(tempMatrix, tempMatrix, GlueMatrix.CreateMatrix());

    return tempMatrix;
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> mdkGlueMatrixForMultiplication<ElementType>::ElementMultiply(const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrix)
{
    auto tempMatrix = this->CreateMatrix();

    MatrixElementMultiply(tempMatrix, tempMatrix, GlueMatrix.CreateMatrix());

    return tempMatrix;
}


}// end namespace mdk


#endif
