#ifndef __mdkDenseGlueMatrixForMultiplication_hpp
#define __mdkDenseGlueMatrixForMultiplication_hpp

//#include "mdkLinearAlgebra.h"

namespace mdk
{

template<typename ElementType>
mdkDenseGlueMatrixForMultiplication<ElementType>::mdkDenseGlueMatrixForMultiplication()
{
    this->Reset();
}


template<typename ElementType>
mdkDenseGlueMatrixForMultiplication<ElementType>::~mdkDenseGlueMatrixForMultiplication()
{

}


template<typename ElementType>
mdkDenseGlueMatrixForMultiplication<ElementType>::mdkDenseGlueMatrixForMultiplication(mdkDenseGlueMatrixForMultiplication<ElementType>&& GlueMatrix)
{
    m_RowNumber = GlueMatrix.m_RowNumber;

    m_ColNumber = GlueMatrix.m_ColNumber;

    m_SourceMatrixSharedCopyList = std::move(GlueMatrix.m_SourceMatrixSharedCopyList);

    m_Element_Coef = std::move(GlueMatrix.m_Element_Coef);

    m_Is_m_Element_Coef_Equal_to_One = GlueMatrix.m_Is_m_Element_Coef_Equal_to_One;
    
    // clear counter 
    GlueMatrix.m_RowNumber = 0;
    GlueMatrix.m_ColNumber = 0;
}


template<typename ElementType>
inline
void mdkDenseGlueMatrixForMultiplication<ElementType>::Reset()
{
    m_RowNumber = 0;

    m_ColNumber = 0;

    m_SourceMatrixSharedCopyList.resize(0);

    m_SourceMatrixSharedCopyList.reserve(MDK_DenseGlueMatrixForMultiplication_ReservedCapacity);

    m_Element_Coef = ElementType(1);

    m_Is_m_Element_Coef_Equal_to_One = true;
}


template<typename ElementType>
inline
int64 mdkDenseGlueMatrixForMultiplication<ElementType>::GetRowNumber() const
{
    return m_RowNumber;
}


template<typename ElementType>
inline
int64 mdkDenseGlueMatrixForMultiplication<ElementType>::GetColNumber() const
{
    return m_ColNumber;
}


template<typename ElementType>
inline
int64 mdkDenseGlueMatrixForMultiplication<ElementType>::GetElementNumber() const
{
    return m_RowNumber*m_ColNumber;
}


template<typename ElementType>
inline
mdkMatrixSize mdkDenseGlueMatrixForMultiplication<ElementType>::GetSize() const
{
    mdkMatrixSize Size;

    Size.RowNumber = m_RowNumber;

    Size.ColNumber = m_ColNumber;

    return Size;
}


template<typename ElementType>
inline
int64 mdkDenseGlueMatrixForMultiplication<ElementType>::GetMatrixNumber() const
{
    return m_SourceMatrixSharedCopyList.size();
}


template<typename ElementType>
inline
bool mdkDenseGlueMatrixForMultiplication<ElementType>::IsEmpty() const
{
    if (m_RowNumber <= 0)
    {
        return true;
    }

    return false;
}


template<typename ElementType>
inline
mdkDenseMatrix<ElementType> mdkDenseGlueMatrixForMultiplication<ElementType>::CreateDenseMatrix() const
{
    mdkDenseMatrix<ElementType> tempMatrix;

    this->CreateDenseMatrix(tempMatrix);

    return tempMatrix;
}


template<typename ElementType>
inline 
bool mdkDenseGlueMatrixForMultiplication<ElementType>::CreateDenseMatrix(mdkDenseMatrix<ElementType>& OutputMatrix) const
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
            mdkError << "Size does not match @ mdkDenseGlueMatrixForMultiplication::CreateDenseMatrix(OutputMatrix)" << '\n';
            return false;
        }
    }

    auto MatrixNumber = int64(m_SourceMatrixSharedCopyList.size());

    if (MatrixNumber == 0)
    {
        mdkError << "MatrixNumber is zero @ mdkDenseGlueMatrixForMultiplication::CreateDenseMatrix(OutputMatrix)" << '\n';
        return false;
    }

    if (MatrixNumber == 1)
    {
        if (m_Is_m_Element_Coef_Equal_to_One == true)
        {
            mdkError << "MatrixNumber is 1 and  m_Is_m_Element_Coef_Equal_to_One = true @ mdkDenseGlueMatrixForMultiplication::CreateDenseMatrix(OutputMatrix)" << '\n';
            OutputMatrix.Copy(m_SourceMatrixSharedCopyList[0]);
            return false;
        }
        
        MatrixElementMultiply(OutputMatrix, m_SourceMatrixSharedCopyList[0], m_Element_Coef);        
        
        return true;
    }

    if (MatrixNumber == 2)
    {
         MatrixMultiply(OutputMatrix, m_SourceMatrixSharedCopyList[0], m_SourceMatrixSharedCopyList[1]);

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
        MatrixMultiply(OutputMatrix, m_SourceMatrixSharedCopyList[0], m_SourceMatrixSharedCopyList[1]);
        
        for (int64 i = 2; i < MatrixNumber; ++i)
        {
            OutputMatrix = MatrixMultiply(OutputMatrix, m_SourceMatrixSharedCopyList[i]);
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
        MatrixMultiply(OutputMatrix, m_SourceMatrixSharedCopyList[MatrixNumber - 2], m_SourceMatrixSharedCopyList[MatrixNumber - 1]);

        for (int64 i = MatrixNumber-3; i >= 0; --i)
        {
            OutputMatrix = MatrixMultiply(m_SourceMatrixSharedCopyList[i], OutputMatrix);
        }

        if (m_Is_m_Element_Coef_Equal_to_One == false)
        {
            MatrixElementMultiply(OutputMatrix, OutputMatrix, m_Element_Coef);
        }

        return true;
    }

    //---------------------------------------------------------------------------------


    //General : find the best combination to get the result -------------------------------------------------------------

    auto MatrixPointerList = std::vector<const mdkDenseMatrix<ElementType>*>(MatrixNumber);

    for (int64 i = 0; i < MatrixNumber - 1; ++i)
    {
        MatrixPointerList[i] = &m_SourceMatrixSharedCopyList[i];
    }

    auto ResultMatrixList = std::vector<mdkDenseMatrix<ElementType>>(MatrixNumber);

   
    while (true)
    {
        auto CurMatrixNumber = int64(ResultMatrixList.size());

        if (CurMatrixNumber == 1)
        {
            break;
        }

        int64 Max_ElementNumber_Diff = 0;

        int64 RelativeIndex_BestMatrixPair = 0;

        for (int64 i = 0; i < CurMatrixNumber - 1; ++i)
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
mdkDenseMatrix<ElementType> mdkDenseGlueMatrixForMultiplication<ElementType>::ElementMultiply(const mdkDenseMatrix<ElementType>& targetMatrix)
{
    auto tempMatrix = this->CreateDenseMatrix();

    MatrixElementMultiply(tempMatrix, tempMatrix, targetMatrix);

    return tempMatrix;
}


template<typename ElementType>
inline
mdkDenseMatrix<ElementType> mdkDenseGlueMatrixForMultiplication<ElementType>::ElementMultiply(const ElementType& Element)
{
    auto tempMatrix = this->CreateDenseMatrix();

    MatrixElementMultiply(tempMatrix, tempMatrix, Element);

    return tempMatrix;
}


template<typename ElementType>
inline 
mdkDenseMatrix<ElementType> mdkDenseGlueMatrixForMultiplication<ElementType>::ElementMultiply(const mdkDenseShadowMatrix<ElementType>& ShadowMatrix)
{
    auto tempMatrix = this->CreateDenseMatrix();

    MatrixElementMultiply(tempMatrix, tempMatrix, ShadowMatrix.CreateDenseMatrix());

    return tempMatrix;
}


template<typename ElementType>
inline 
mdkDenseMatrix<ElementType> mdkDenseGlueMatrixForMultiplication<ElementType>::ElementMultiply(const mdkDenseGlueMatrixForLinearCombination<ElementType>& GlueMatrix)
{
    auto tempMatrix = this->CreateDenseMatrix();

    MatrixElementMultiply(tempMatrix, tempMatrix, GlueMatrix.CreateDenseMatrix());

    return tempMatrix;
}


template<typename ElementType>
inline 
mdkDenseMatrix<ElementType> mdkDenseGlueMatrixForMultiplication<ElementType>::ElementMultiply(const mdkDenseGlueMatrixForMultiplication<ElementType>& GlueMatrix)
{
    auto tempMatrix = this->CreateDenseMatrix();

    MatrixElementMultiply(tempMatrix, tempMatrix, GlueMatrix.CreateDenseMatrix());

    return tempMatrix;
}


}// end namespace mdk


#endif
