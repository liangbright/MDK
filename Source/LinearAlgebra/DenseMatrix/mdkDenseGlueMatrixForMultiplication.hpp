#ifndef mdk_DenseGlueMatrixForMultiplication_hpp
#define mdk_DenseGlueMatrixForMultiplication_hpp


namespace mdk
{

template<typename ElementType>
DenseGlueMatrixForMultiplication<ElementType>::DenseGlueMatrixForMultiplication()
{
    m_RowCount = 0;

    m_ColCount = 0;

    m_SourceMatrixSharedCopyList.resize(0);

    m_SourceMatrixSharedCopyList.reserve(MDK_DenseGlueMatrixForMultiplication_ReservedCapacity);

    m_Element_Coef = ElementType(1);

    m_Is_m_Element_Coef_Equal_to_One = true;
}


template<typename ElementType>
DenseGlueMatrixForMultiplication<ElementType>::~DenseGlueMatrixForMultiplication()
{

}


template<typename ElementType>
DenseGlueMatrixForMultiplication<ElementType>::DenseGlueMatrixForMultiplication(DenseGlueMatrixForMultiplication<ElementType>&& GlueMatrix)
{
    m_RowCount = GlueMatrix.m_RowCount;

    m_ColCount = GlueMatrix.m_ColCount;

    m_SourceMatrixSharedCopyList = std::move(GlueMatrix.m_SourceMatrixSharedCopyList);

    m_Element_Coef = std::move(GlueMatrix.m_Element_Coef);

    m_Is_m_Element_Coef_Equal_to_One = GlueMatrix.m_Is_m_Element_Coef_Equal_to_One;
    
    // clear counter 
    GlueMatrix.m_RowCount = 0;
    GlueMatrix.m_ColCount = 0;
}


template<typename ElementType>
inline
int_max DenseGlueMatrixForMultiplication<ElementType>::GetRowCount() const
{
    return m_RowCount;
}


template<typename ElementType>
inline
int_max DenseGlueMatrixForMultiplication<ElementType>::GetColCount() const
{
    return m_ColCount;
}


template<typename ElementType>
inline
int_max DenseGlueMatrixForMultiplication<ElementType>::GetElementNumber() const
{
    return m_RowCount*m_ColCount;
}


template<typename ElementType>
inline
MatrixSize DenseGlueMatrixForMultiplication<ElementType>::GetSize() const
{
    MatrixSize Size;

    Size.RowCount = m_RowCount;

    Size.ColCount = m_ColCount;

    return Size;
}


template<typename ElementType>
inline
int_max DenseGlueMatrixForMultiplication<ElementType>::GetMatrixNumber() const
{
    return m_SourceMatrixSharedCopyList.size();
}


template<typename ElementType>
inline
bool DenseGlueMatrixForMultiplication<ElementType>::IsEmpty() const
{
    return (m_RowCount <= 0);
}


template<typename ElementType>
inline
DenseMatrix<ElementType> DenseGlueMatrixForMultiplication<ElementType>::CreateDenseMatrix() const
{
    DenseMatrix<ElementType> tempMatrix;

    if (this->IsEmpty() == false)
    {
        this->CreateDenseMatrix(tempMatrix);
    }

    return tempMatrix;
}


template<typename ElementType>
inline 
bool DenseGlueMatrixForMultiplication<ElementType>::CreateDenseMatrix(DenseMatrix<ElementType>& OutputMatrix) const
{
	OutputMatrix.FastResize(m_RowCount, m_ColCount);

    auto MatrixNumber = int_max(m_SourceMatrixSharedCopyList.size());

    if (MatrixNumber == 0)
    {
        MDK_Error("MatrixNumber is zero @ mdkDenseGlueMatrixForMultiplication::CreateDenseMatrix(OutputMatrix)")
        return false;
    }

    if (MatrixNumber == 1)
    {
        if (m_Is_m_Element_Coef_Equal_to_One == true)
        {
            MDK_Error("MatrixNumber is 1 and  m_Is_m_Element_Coef_Equal_to_One = true @ mdkDenseGlueMatrixForMultiplication::CreateDenseMatrix(OutputMatrix)")
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
    if (m_RowCount == 1)
    {
        MatrixMultiply(OutputMatrix, m_SourceMatrixSharedCopyList[0], m_SourceMatrixSharedCopyList[1]);
        
        for (int_max i = 2; i < MatrixNumber; ++i)
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
    if (m_ColCount == 1)
    {
        MatrixMultiply(OutputMatrix, m_SourceMatrixSharedCopyList[MatrixNumber - 2], m_SourceMatrixSharedCopyList[MatrixNumber - 1]);

        for (int_max i = MatrixNumber-3; i >= 0; --i)
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

    auto MatrixPointerList = std::vector<const DenseMatrix<ElementType>*>(MatrixNumber);

    for (int_max i = 0; i < MatrixNumber; ++i)
    {
        MatrixPointerList[i] = &m_SourceMatrixSharedCopyList[i];
    }

    auto ResultMatrixList = std::vector<DenseMatrix<ElementType>>(MatrixNumber);
   
    while (true)
    {
        auto CurMatrixNumber = int_max(ResultMatrixList.size());

        if (CurMatrixNumber == 1)
        {
            break;
        }

        int_max Max_ElementNumber_Diff = 0;

        int_max RelativeIndex_BestMatrixPair = 0;

        for (int_max i = 0; i < CurMatrixNumber - 1; ++i)
        {
            auto tempRowCount_a = MatrixPointerList[i]->GetRowCount();
            auto tempColCount_a = MatrixPointerList[i]->GetRowCount();

            auto tempRowCount_b = MatrixPointerList[i + 1]->GetRowCount();
            auto tempColCount_b = MatrixPointerList[i + 1]->GetRowCount();

            auto ElementNumber_Diff = (tempRowCount_a*tempColCount_a + tempRowCount_b*tempColCount_b) - 2 * tempRowCount_a * tempColCount_b;

            if (ElementNumber_Diff > Max_ElementNumber_Diff)
            {
                Max_ElementNumber_Diff = ElementNumber_Diff;

                RelativeIndex_BestMatrixPair = i;
            }
        }

        auto tempMatrix = MatrixMultiply(*MatrixPointerList[RelativeIndex_BestMatrixPair], *MatrixPointerList[RelativeIndex_BestMatrixPair + 1]);

        // change MatrixPointerList and ResultMatrixList

        ResultMatrixList.erase(ResultMatrixList.begin() + RelativeIndex_BestMatrixPair);

        ResultMatrixList[RelativeIndex_BestMatrixPair].ForceShare(tempMatrix); 

        MatrixPointerList.erase(MatrixPointerList.begin() + RelativeIndex_BestMatrixPair);

        MatrixPointerList[RelativeIndex_BestMatrixPair] = &ResultMatrixList[RelativeIndex_BestMatrixPair];

    }// while
    
    // take the final result
    OutputMatrix.Copy(std::move(ResultMatrixList[0]));

    if (m_Is_m_Element_Coef_Equal_to_One == false)
    {
        MatrixElementMultiply(OutputMatrix, OutputMatrix, m_Element_Coef);
    }

    return true;
}


template<typename ElementType>
inline 
DenseMatrix<ElementType> DenseGlueMatrixForMultiplication<ElementType>::ElementMultiply(const DenseMatrix<ElementType>& targetMatrix)
{
    auto tempMatrix = this->CreateDenseMatrix();

    MatrixElementMultiply(tempMatrix, tempMatrix, targetMatrix);

    return tempMatrix;
}


template<typename ElementType>
inline
DenseMatrix<ElementType> DenseGlueMatrixForMultiplication<ElementType>::ElementMultiply(const ElementType& Element)
{
    auto tempMatrix = this->CreateDenseMatrix();

    MatrixElementMultiply(tempMatrix, tempMatrix, Element);

    return tempMatrix;
}


template<typename ElementType>
inline 
DenseMatrix<ElementType> DenseGlueMatrixForMultiplication<ElementType>::ElementMultiply(const DenseShadowMatrix<ElementType>& ShadowMatrix)
{
    auto tempMatrix = this->CreateDenseMatrix();

    MatrixElementMultiply(tempMatrix, tempMatrix, ShadowMatrix.CreateDenseMatrix());

    return tempMatrix;
}


template<typename ElementType>
inline 
DenseMatrix<ElementType> DenseGlueMatrixForMultiplication<ElementType>::ElementMultiply(const DenseGlueMatrixForLinearCombination<ElementType>& GlueMatrix)
{
    auto tempMatrix = this->CreateDenseMatrix();

    MatrixElementMultiply(tempMatrix, tempMatrix, GlueMatrix.CreateDenseMatrix());

    return tempMatrix;
}


template<typename ElementType>
inline 
DenseMatrix<ElementType> DenseGlueMatrixForMultiplication<ElementType>::ElementMultiply(const DenseGlueMatrixForMultiplication<ElementType>& GlueMatrix)
{
    auto tempMatrix = this->CreateDenseMatrix();

    MatrixElementMultiply(tempMatrix, tempMatrix, GlueMatrix.CreateDenseMatrix());

    return tempMatrix;
}


}// end namespace mdk


#endif
