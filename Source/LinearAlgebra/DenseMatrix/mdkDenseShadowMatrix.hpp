#pragma once


namespace mdk
{

template<typename ElementType>
inline
DenseShadowMatrix<ElementType>::DenseShadowMatrix()
{
    m_Flag_OutputVector = false;

    m_ElementCount = 0;

    m_RowCount = 0;

    m_ColCount = 0;

    m_Flag_All_Row = false;

    m_Flag_All_Col = false;
}


template<typename ElementType>
inline
DenseShadowMatrix<ElementType>::DenseShadowMatrix(const DenseMatrix<ElementType>& SourceMatrix, const std::initializer_list<int_max>& LinearIndexList)
: m_SourceMatrixSharedCopy(MDK_PURE_EMPTY_MATRIX)
{
    // bound check is performed in mdkDenseMatrix when calling the operator(), e.g., A({1, 2, 3}), A is a matrix    
    // the constructed ShadowMatrix is a col-vector

    m_SourceMatrixSharedCopy.ForceShare(SourceMatrix);

    m_LinearIndexList_source = LinearIndexList;

    m_Flag_OutputVector = true;

    //-----------------------------------------------

    m_ElementCount = m_LinearIndexList_source.size();

    m_RowCount = m_ElementCount;

    m_ColCount = 1;

    m_Flag_All_Row = false;

    m_Flag_All_Col = false;

    m_ErrorElement = m_SourceMatrixSharedCopy.GetErrorElement();
}


template<typename ElementType>
inline
DenseShadowMatrix<ElementType>::DenseShadowMatrix(const DenseMatrix<ElementType>& SourceMatrix,
                                                  const std::initializer_list<int_max>& RowIndexList,
                                                  const std::initializer_list<int_max>& ColIndexList)
: m_SourceMatrixSharedCopy(MDK_PURE_EMPTY_MATRIX)
{
    // all the indexes in RowIndexList and ColIndexList are within bound
    // bound check is performed in mdkDenseMatrix when calling the operator(), e.g., A({1, 2, 3}, {0, 1}), A is a matrix    

    m_SourceMatrixSharedCopy.ForceShare(SourceMatrix);

    auto RowCount_source = SourceMatrix.GetRowCount();

    auto ColCount_source = SourceMatrix.GetColCount();

    m_RowIndexList_source = RowIndexList;

    m_ColIndexList_source = ColIndexList;

    m_Flag_OutputVector = false;

    //-----------------------------------------------

    m_RowCount = m_RowIndexList_source.size();

    m_ColCount = m_ColIndexList_source.size();

    m_ElementCount = m_RowCount*m_ColCount;

    m_Flag_All_Row = false;

    m_Flag_All_Col = false;

    m_ErrorElement = m_SourceMatrixSharedCopy.GetErrorElement();
}


template<typename ElementType>
inline
DenseShadowMatrix<ElementType>::DenseShadowMatrix(const DenseMatrix<ElementType>& SourceMatrix,
                                                  const std::initializer_list<int_max>& RowIndexList,
                                                  const MDK_Symbol_ALL& ALL_Symbol)
: m_SourceMatrixSharedCopy(MDK_PURE_EMPTY_MATRIX)
{
    // all the indexes in RowIndexList and ColIndexList are within bound
    // bound check is performed in mdkDenseMatrix when calling the operator(), e.g., A({1, 2, 3}, ALL), A is a matrix    

    m_SourceMatrixSharedCopy.ForceShare(SourceMatrix);

    auto RowCount_source = SourceMatrix.GetRowCount();

    auto ColCount_source = SourceMatrix.GetColCount();

    m_RowIndexList_source = RowIndexList;

    m_ColIndexList_source.resize(ColCount_source);

    for (int_max i = 0; i < ColCount_source; ++i)
    {
        m_ColIndexList_source[i] = i;
    }

    m_Flag_OutputVector = false;

    //------------------------------------------

    m_RowCount = m_RowIndexList_source.size();

    m_ColCount = m_ColIndexList_source.size();

    m_ElementCount = m_RowCount*m_ColCount;

    m_Flag_All_Row = false;

    m_Flag_All_Col = true;

    m_ErrorElement = m_SourceMatrixSharedCopy.GetErrorElement();
}


template<typename ElementType>
inline
DenseShadowMatrix<ElementType>::DenseShadowMatrix(const DenseMatrix<ElementType>& SourceMatrix,
                                                  const MDK_Symbol_ALL& ALL_Symbol,
                                                  const std::initializer_list<int_max>& ColIndexList)
: m_SourceMatrixSharedCopy(MDK_PURE_EMPTY_MATRIX)
{
    // all the indexes in RowIndexList and ColIndexList are within bound
    // bound check is performed in mdkDenseMatrix when calling the operator(), e.g., A(ALL, {0, 1, 2}), A is a matrix    

    m_SourceMatrixSharedCopy.ForceShare(SourceMatrix);

    auto RowCount_source = SourceMatrix.GetRowCount();

    auto ColCount_source = SourceMatrix.GetColCount();

    m_RowIndexList_source.resize(RowCount_source);

    for (int_max i = 0; i < RowCount_source; ++i)
    {
        m_RowIndexList_source[i] = i;
    }

    m_ColIndexList_source = ColIndexList;

    m_Flag_OutputVector = false;

    //-------------------------------------------

    m_RowCount = m_RowIndexList_source.size();

    m_ColCount = m_ColIndexList_source.size();

    m_ElementCount = m_RowCount*m_ColCount;

    m_Flag_All_Row = true;

    m_Flag_All_Col = true;

    m_ErrorElement = m_SourceMatrixSharedCopy.GetErrorElement();
}


template<typename ElementType>
inline
DenseShadowMatrix<ElementType>::DenseShadowMatrix(const DenseMatrix<ElementType>& SourceMatrix, std::vector<int_max> LinearIndexList)
: m_SourceMatrixSharedCopy(MDK_PURE_EMPTY_MATRIX)
{
    // bound check is performed in mdkDenseMatrix when calling the operator(), e.g., A({1, 2, 3}), A is a matrix    
    // the constructed ShadowMatrix is a col-vector

    m_SourceMatrixSharedCopy.ForceShare(SourceMatrix);

    m_LinearIndexList_source = std::move(LinearIndexList);

    m_Flag_OutputVector = true;

    //-----------------------------------------------

    m_ElementCount = m_LinearIndexList_source.size();

    m_RowCount = m_ElementCount;

    m_ColCount = 1;

    m_Flag_All_Row = false;

    m_Flag_All_Col = false;

    m_ErrorElement = m_SourceMatrixSharedCopy.GetErrorElement();
}


template<typename ElementType>
inline
DenseShadowMatrix<ElementType>::DenseShadowMatrix(const DenseMatrix<ElementType>& SourceMatrix,
                                                  std::vector<int_max> RowIndexList,
                                                  std::vector<int_max> ColIndexList)
: m_SourceMatrixSharedCopy(MDK_PURE_EMPTY_MATRIX)
{
    // all the indexes in RowIndexList and ColIndexList are within bound
    // bound check is performed in mdkDenseMatrix when calling the operator(), e.g., A({1, 2, 3}, {0, 1}), A is a matrix    

    m_SourceMatrixSharedCopy.ForceShare(SourceMatrix);

    auto RowCount_source = SourceMatrix.GetRowCount();

    auto ColCount_source = SourceMatrix.GetColCount();

    m_RowIndexList_source = std::move(RowIndexList);

    m_ColIndexList_source = std::move(ColIndexList);

    m_Flag_OutputVector = false;

    //-----------------------------------------------

    m_RowCount = m_RowIndexList_source.size();

    m_ColCount = m_ColIndexList_source.size();

    m_ElementCount = m_RowCount*m_ColCount;

    m_Flag_All_Row = false;

    m_Flag_All_Col = false;

    m_ErrorElement = m_SourceMatrixSharedCopy.GetErrorElement();
}


template<typename ElementType>
inline
DenseShadowMatrix<ElementType>::DenseShadowMatrix(const DenseMatrix<ElementType>& SourceMatrix,
                                                  std::vector<int_max> RowIndexList,
                                                  const MDK_Symbol_ALL& ALL_Symbol)
: m_SourceMatrixSharedCopy(MDK_PURE_EMPTY_MATRIX)
{
    // all the indexes in RowIndexList and ColIndexList are within bound
    // bound check is performed in mdkDenseMatrix when calling the operator(), e.g., A({1, 2, 3}, ALL), A is a matrix    

    m_SourceMatrixSharedCopy.ForceShare(SourceMatrix);

    auto RowCount_source = SourceMatrix.GetRowCount();

    auto ColCount_source = SourceMatrix.GetColCount();

    m_RowIndexList_source = std::move(RowIndexList);

    m_ColIndexList_source.resize(ColCount_source);

    for (int_max i = 0; i < ColCount_source; ++i)
    {
        m_ColIndexList_source[i] = i;    
    }

    m_Flag_OutputVector = false;

    //------------------------------------------

    m_RowCount = m_RowIndexList_source.size();

    m_ColCount = m_ColIndexList_source.size();

    m_ElementCount = m_RowCount*m_ColCount;

    m_Flag_All_Row = false;

    m_Flag_All_Col = true;

    m_ErrorElement = m_SourceMatrixSharedCopy.GetErrorElement();
}


template<typename ElementType>
inline
DenseShadowMatrix<ElementType>::DenseShadowMatrix(const DenseMatrix<ElementType>& SourceMatrix,
                                                  const MDK_Symbol_ALL& ALL_Symbol,
                                                  std::vector<int_max> ColIndexList)
: m_SourceMatrixSharedCopy(MDK_PURE_EMPTY_MATRIX)
{
    // all the indexes in RowIndexList and ColIndexList are within bound
    // bound check is performed in mdkDenseMatrix when calling the operator(), e.g., A(ALL, {0, 1, 2}), A is a matrix    

    m_SourceMatrixSharedCopy.ForceShare(SourceMatrix);

    auto RowCount_source = SourceMatrix.GetRowCount();

    auto ColCount_source = SourceMatrix.GetColCount();

    m_ColIndexList_source = std::move(ColIndexList);

    m_RowIndexList_source.resize(RowCount_source);

    for (int_max i = 0; i < RowCount_source; ++i)
    {
        m_RowIndexList_source[i] = i;
    }

    m_Flag_OutputVector = false;

    //-------------------------------------------

    m_RowCount = m_RowIndexList_source.size();

    m_ColCount = m_ColIndexList_source.size();

    m_ElementCount = m_RowCount*m_ColCount;

    m_Flag_All_Row = true;

    m_Flag_All_Col = true;

    m_ErrorElement = m_SourceMatrixSharedCopy.GetErrorElement();
}


template<typename ElementType>
inline
DenseShadowMatrix<ElementType>::DenseShadowMatrix(const DenseMatrix<ElementType>& SourceMatrix, const MDK_Symbol_ALL& ALL_Symbol)
: m_SourceMatrixSharedCopy(MDK_PURE_EMPTY_MATRIX)
{
    m_SourceMatrixSharedCopy.ForceShare(SourceMatrix);

    m_Flag_OutputVector = true;

    auto RowCount_source = SourceMatrix.GetRowCount();

    auto ColCount_source = SourceMatrix.GetColCount();

    m_ColIndexList_source.resize(ColCount_source);

    for (int_max i = 0; i < ColCount_source; ++i)
    {
        m_ColIndexList_source[i] = i;
    }

    m_RowIndexList_source.resize(RowCount_source);

    for (int_max i = 0; i < RowCount_source; ++i)
    {
        m_RowIndexList_source[i] = i;
    }
    //-----------------------------------------------

    m_ElementCount = RowCount_source*ColCount_source;

    m_RowCount = RowCount_source;

    m_ColCount = ColCount_source;

    m_Flag_All_Row = true;

    m_Flag_All_Col = true;

    m_ErrorElement = m_SourceMatrixSharedCopy.GetErrorElement();
}

/*
// this function is not useful
template<typename ElementType>
inline 
DenseShadowMatrix<ElementType>::DenseShadowMatrix(const DenseShadowMatrix<ElementType>& InputShadowMatrix)
{
    // copy matrix data, not share
    m_SourceMatrixSharedCopy = InputShadowMatrix.CreateDenseMatrix();

    m_RowCount = m_SourceMatrixSharedCopy.GetRowCount();
    m_ColCount = m_SourceMatrixSharedCopy.GetColCount();
    m_ElementCount = m_RowCount*m_ColCount;

    m_Flag_All_Row = true;
    m_Flag_All_Col = true;

    m_ErrorElement = m_SourceMatrixSharedCopy.GetErrorElement();
}
*/

template<typename ElementType>
inline 
DenseShadowMatrix<ElementType>::DenseShadowMatrix(DenseShadowMatrix<ElementType>&& InputShadowMatrix)
: m_SourceMatrixSharedCopy(MDK_PURE_EMPTY_MATRIX)
{
    m_SourceMatrixSharedCopy.ForceShare(InputShadowMatrix.m_SourceMatrixSharedCopy); // std::move will move the data, i.e., std::move = Take

    m_RowIndexList_source = std::move(InputShadowMatrix.m_RowIndexList_source);

    m_ColIndexList_source = std::move(InputShadowMatrix.m_ColIndexList_source);

    m_LinearIndexList_source = std::move(InputShadowMatrix.m_LinearIndexList_source);

    m_Flag_OutputVector = InputShadowMatrix.m_Flag_OutputVector;

    //-----------------------------------------------------

    m_RowCount = InputShadowMatrix.m_RowCount;

    m_ColCount = InputShadowMatrix.m_ColCount;

    m_ElementCount = InputShadowMatrix.m_ElementCount;

    m_Flag_All_Row = InputShadowMatrix.m_Flag_All_Row;

    m_Flag_All_Col = InputShadowMatrix.m_Flag_All_Col;

    m_ErrorElement = m_SourceMatrixSharedCopy.GetErrorElement();

    // clear InputShadowMatrix :  not necessary
    // Do NOT Use InputShadowMatrix.m_SourceMatrixSharedCopy.clear() -> this will clear m_SourceMatrixSharedCopy of this ShadowMatrix
    //DenseMatrix<ElementType> EmptyMatrix(MDK_PURE_EMPTY_MATRIX);
    //InputShadowMatrix.m_SourceMatrixSharedCopy.SwapSmartPointer(EmptyMatrix);
}


template<typename ElementType>
inline
DenseShadowMatrix<ElementType>::~DenseShadowMatrix()
{
}


template<typename ElementType>
inline bool DenseShadowMatrix<ElementType>::IsLinearIndexListOnly() const
{
    return m_Flag_OutputVector;
}


template<typename ElementType>
inline
int_max DenseShadowMatrix<ElementType>::GetElementCount() const
{
    return m_ElementCount;
}


template<typename ElementType>
inline
int_max DenseShadowMatrix<ElementType>::GetColCount() const
{
    return m_ColCount;
}


template<typename ElementType>
inline
int_max DenseShadowMatrix<ElementType>::GetRowCount() const
{
    return m_RowCount;
}


template<typename ElementType>
inline
MatrixSize DenseShadowMatrix<ElementType>::GetSize() const
{
    MatrixSize Size;

    Size.RowCount = m_RowCount;
    Size.ColCount = m_ColCount;

    return Size;
}


template<typename ElementType>
inline
bool DenseShadowMatrix<ElementType>::IsEmpty() const
{
    return (m_ElementCount == 0);
}


template<typename ElementType>
inline 
const std::vector<int_max>& DenseShadowMatrix<ElementType>::GetRowIndexListOfSource() const
{
    return m_RowIndexList_source;
}


template<typename ElementType>
inline 
const std::vector<int_max>& DenseShadowMatrix<ElementType>::GetColIndexListOfSource() const
{
    return m_ColIndexList_source;
}


template<typename ElementType>
inline 
const std::vector<int_max>& DenseShadowMatrix<ElementType>::GetLinearIndexListOfSource() const
{
    return m_LinearIndexList_source;
}


template<typename ElementType>
inline 
const DenseMatrix<ElementType>& DenseShadowMatrix<ElementType>::GetSourceMatrixSharedCopy() const
{
    return m_SourceMatrixSharedCopy;
}


template<typename ElementType>
inline 
DenseMatrix<ElementType> DenseShadowMatrix<ElementType>::CreateDenseMatrix() const
{
    DenseMatrix<ElementType> tempMatrix;

    if (m_ElementCount > 0)
    {
        this->CreateDenseMatrix(tempMatrix);
    }
    
    return tempMatrix;
}


template<typename ElementType>
inline
bool DenseShadowMatrix<ElementType>::CreateDenseMatrix(DenseMatrix<ElementType>& OutputMatrix) const
{
    if (m_SourceMatrixSharedCopy.GetElementPointer() == OutputMatrix.GetElementPointer())
    {
        OutputMatrix = this->CreateDenseMatrix();

        return true;
    }

    if (m_RowCount != OutputMatrix.GetRowCount() || m_ColCount != OutputMatrix.GetColCount())
    {
        if (m_Flag_OutputVector == true)
        {
            OutputMatrix.FastResize(m_ElementCount, 1);
        }
        else
        {
            OutputMatrix.FastResize(m_RowCount, m_ColCount);
        }
    }

    if (m_ElementCount == 0)
    {
        MDK_Warning("Self is empty @ DenseShadowMatrix::CreateDenseMatrix(OutputMatrix)")
        return true;
    }

    auto ptrTemp = OutputMatrix.GetElementPointer();

    auto ptrSource = m_SourceMatrixSharedCopy.GetElementPointer();

    if (m_LinearIndexList_source.empty() == false)
    {
        for (int_max i = 0; i < m_ElementCount; ++i)
        {
            ptrTemp[i] = ptrSource[m_LinearIndexList_source[i]];
        }
    }
    else
    {
        auto RowCount_source = m_SourceMatrixSharedCopy.GetRowCount();

        for (int_max j = 0; j < m_ColCount; ++j)
        {
            auto Index = m_ColIndexList_source[j] * RowCount_source;

            for (int_max i = 0; i < m_RowCount; ++i)
            {
                auto LinearIndex_source = Index + m_RowIndexList_source[i];

                ptrTemp[0] = ptrSource[LinearIndex_source];
                ++ptrTemp;
            }
        }
    }

    return true;
}


template<typename ElementType>
inline
void DenseShadowMatrix<ElementType>::operator=(const DenseMatrix<ElementType>& InputMatrix)
{
    // MatrixA = MatrixA
    if (m_SourceMatrixSharedCopy.GetElementPointer() == InputMatrix.GetElementPointer())
    {
        return;
    }

    if (m_Flag_OutputVector == true)
    {
        if (m_ElementCount != InputMatrix.GetElementCount())
        {
            MDK_Error("m_ElementCount != InputMatrix.GetElementCount() @ DenseShadowMatrix::operator=(DenseMatrix)")
            return;
        }
    }
    else
    {
        if (InputMatrix.IsVector() == true)
        {
            if (m_RowCount != InputMatrix.GetElementCount() && m_ColCount != InputMatrix.GetElementCount())
            {
                MDK_Error("Size does not match @ DenseShadowMatrix::operator=(DenseMatrix)")
                return;
            }
        }
        else
        {
            if (m_RowCount != InputMatrix.GetRowCount() || m_ColCount != InputMatrix.GetColCount())
            {
                MDK_Error("Size does not match @ DenseShadowMatrix::operator=(DenseMatrix)")
                return;
            }
        }
    }

    //-------------------------------------------------

    if (m_LinearIndexList_source.empty() == true)
    {        
        if (m_RowIndexList_source.size() == 1 && m_Flag_All_Col == true)     // SourceMatrix(i,:) = InputMatrix
        {
            m_SourceMatrixSharedCopy.SetRow(m_RowIndexList_source[0], InputMatrix.GetElementPointer());
            return;
        }
        else if (m_ColIndexList_source.size() == 1 && m_Flag_All_Row == true) // SourceMatrix(:,j) = InputMatrix
        {
            m_SourceMatrixSharedCopy.SetCol(m_ColIndexList_source[0], InputMatrix.GetElementPointer());
            return;
        }
    }

    //-------------------------------------------------

    auto ptrMatrix = InputMatrix.GetElementPointer();

    for (int_max i = 0; i < m_ElementCount; ++i)
    {
        (*this)[i] = ptrMatrix[i];
    }
}


template<typename ElementType>
template<int_max Length>
inline
void DenseShadowMatrix<ElementType>::operator=(const DenseVector<ElementType, Length>& InputVector)
{
    // note: template parameter Length may not = InputVector.GetElementCount()

    // MatrixA = MatrixA
    if (m_SourceMatrixSharedCopy.GetElementPointer() == InputVector.GetElementPointer())
    {
        return;
    }

    if (m_Flag_OutputVector == true)
    {
        if (m_ElementCount != InputVector.GetElementCount())
        {
            MDK_Error("m_ElementCount != InputVector.GetElementCount() @ DenseShadowMatrix::operator=(DenseVector)")
            return;
        }
    }
    else
    {
        if (m_RowCount != InputVector.GetElementCount() && m_ColCount != InputVector.GetElementCount())
        {
            MDK_Error("Size does not match @ DenseShadowMatrix::operator=(DenseVector)")
            return;
        }
    }

    //-------------------------------------------------

    if (m_LinearIndexList_source.empty() == true)
    {
        if (m_RowIndexList_source.size() == 1 && m_Flag_All_Col == true)     // SourceMatrix(i,:) = InputVector
        {
            m_SourceMatrixSharedCopy.SetRow(m_RowIndexList_source[0], InputVector.GetElementPointer());
            return;
        }
        else if (m_ColIndexList_source.size() == 1 && m_Flag_All_Row == true) // SourceMatrix(:,j) = InputVector
        {
            m_SourceMatrixSharedCopy.SetCol(m_ColIndexList_source[0], InputVector.GetElementPointer());
            return;
        }
    }

    //-------------------------------------------------

	auto ptrVector = InputVector.GetElementPointer();

    for (int_max i = 0; i < m_ElementCount; ++i)
    {
		(*this)[i] = ptrVector[i];
    }
}


template<typename ElementType>
inline 
void DenseShadowMatrix<ElementType>::operator=(const std::initializer_list<ElementType>& InputVector)
{
	int_max InputElementCount = int_max(InputVector.size());

	if (m_Flag_OutputVector == true)
	{
		if (m_ElementCount != InputElementCount)
		{
			MDK_Error("m_ElementCount != InputElementCount @ DenseShadowMatrix::operator=(std::initializer_list)")
			return;
		}
	}
	else
	{
		if (m_RowCount != InputElementCount && m_ColCount != InputElementCount)
		{
			MDK_Error("Size does not match @ DenseShadowMatrix::operator=(std::initializer_list)")
			return;
		}
	}

	//-------------------------------------------------

	if (m_LinearIndexList_source.empty() == true)
	{
		if (m_RowIndexList_source.size() == 1 && m_Flag_All_Col == true)     // SourceMatrix(i,:) = InputVector
		{
			m_SourceMatrixSharedCopy.SetRow(m_RowIndexList_source[0], InputVector.begin());
			return;
		}
		else if (m_ColIndexList_source.size() == 1 && m_Flag_All_Row == true) // SourceMatrix(:,j) = InputVector
		{
			m_SourceMatrixSharedCopy.SetCol(m_ColIndexList_source[0], InputVector.begin());
			return;
		}
	}

	//-------------------------------------------------

	for (int_max i = 0; i < m_ElementCount; ++i)
	{
		(*this)[i] = InputVector.begin()[i];
	}
}


template<typename ElementType>
inline
void DenseShadowMatrix<ElementType>::operator=(const ElementType& Element)
{
    //------------------------------------------------------------

    if (m_LinearIndexList_source.empty() == true)
    {
        if (m_RowIndexList_source.size() == 1 && m_Flag_All_Col == true)     // SourceMatrix(i,:) = Element
        {
            m_SourceMatrixSharedCopy.FillRow(m_RowIndexList_source[0], Element);
            return;
        }
        else if (m_ColIndexList_source.size() == 1 && m_Flag_All_Row == true) // SourceMatrix(:,j) = Element
        {
            m_SourceMatrixSharedCopy.FillCol(m_ColIndexList_source[0], Element);
            return;
        }
    }

    //------------------------------------------------------------

    for (int_max i = 0; i < m_ElementCount; ++i)
    {
        (*this)[i] = Element;
    }
}


template<typename ElementType>
inline
void DenseShadowMatrix<ElementType>::operator=(const DenseShadowMatrix<ElementType>& InputShadowMatrix)
{
    // MatrixA = MatrixA
    if (this == &InputShadowMatrix)
    {
        return;
    }

    if (m_Flag_OutputVector == true)
    {
        if (m_ElementCount != InputShadowMatrix.GetElementCount())
        {
            MDK_Error("m_ElementCount != InputShadowMatrix.GetElementCount() @ DenseShadowMatrix::operator=(mdkDenseShadowMatrix)")
            return;
        }
    }
    else
    {
        if (m_RowCount != InputShadowMatrix.GetRowCount() || m_ColCount != InputShadowMatrix.GetColCount())
        {
            MDK_Error("Size does not match @ DenseShadowMatrix::operator=(mdkDenseShadowMatrix)")
            return;
        }
    }

    //--------------------------------------------

    if (m_SourceMatrixSharedCopy.GetElementPointer() == InputShadowMatrix.m_SourceMatrixSharedCopy.GetElementPointer())
    {
        (*this) = InputShadowMatrix.CreateDenseMatrix();
    }
    else
    {
        for (int_max i = 0; i < m_ElementCount; ++i)
        {
            (*this)[i] = InputShadowMatrix[i];
        }
    }
}


template<typename ElementType>
inline
void DenseShadowMatrix<ElementType>::operator=(const DenseGlueMatrixForLinearCombination<ElementType>& GlueMatrix)
{
    (*this) = GlueMatrix.CreateDenseMatrix();
}


template<typename ElementType>
inline
void DenseShadowMatrix<ElementType>::operator=(const DenseGlueMatrixForMultiplication<ElementType>& GlueMatrix)
{
    (*this) = GlueMatrix.CreateDenseMatrix();
}


template<typename ElementType>
inline
ElementType& DenseShadowMatrix<ElementType>::operator[](int_max LinearIndex)
{
#if defined(MDK_DEBUG_DenseShadowMatrix_Operator_CheckBound)

    if (LinearIndex >= m_ElementCount)
    {
        MDK_Error("Invalid input @ DenseShadowMatrix::operator[i]")
        return m_ErrorElement;
    }

#endif

    if (m_LinearIndexList_source.empty() == false)
    {
        return m_SourceMatrixSharedCopy[m_LinearIndexList_source[LinearIndex]];
    }

    int_max ColIndex = LinearIndex / m_RowCount;

    int_max RowIndex = LinearIndex % m_RowCount;

    auto RowCount_source = m_SourceMatrixSharedCopy.GetRowCount();

    auto LinearIndex_source = m_ColIndexList_source[ColIndex] * RowCount_source + m_RowIndexList_source[RowIndex];

    return m_SourceMatrixSharedCopy[LinearIndex_source];
}


template<typename ElementType>
inline
const ElementType& DenseShadowMatrix<ElementType>::operator[](int_max LinearIndex) const
{
#if defined(MDK_DEBUG_DenseShadowMatrix_Operator_CheckBound)

    if (LinearIndex >= m_ElementCount)
    {
        MDK_Error("Invalid input @ DenseShadowMatrix::operator[i] const")
        return m_ErrorElement;
    }

#endif

    if (m_LinearIndexList_source.empty() == false)
    {
        return m_SourceMatrixSharedCopy[m_LinearIndexList_source[LinearIndex]];
    }

    int_max ColIndex = LinearIndex / m_RowCount;

    int_max RowIndex = LinearIndex % m_RowCount;

    auto RowCount_source = m_SourceMatrixSharedCopy.GetRowCount();

    auto LinearIndex_source = m_ColIndexList_source[ColIndex] * RowCount_source + m_RowIndexList_source[RowIndex];

    return m_SourceMatrixSharedCopy[LinearIndex_source];
}


template<typename ElementType>
inline
ElementType& DenseShadowMatrix<ElementType>::operator()(int_max LinearIndex)
{
#if defined(MDK_DEBUG_DenseShadowMatrix_Operator_CheckBound)

    if (LinearIndex >= m_ElementCount || LinearIndex < 0)
	{
		MDK_Error("Invalid input @ DenseShadowMatrix::operator(i)")
        return m_ErrorElement;
	}

#endif

	if (m_LinearIndexList_source.empty() == false)
    {
        return m_SourceMatrixSharedCopy[m_LinearIndexList_source[LinearIndex]];
   	}
	
    int_max ColIndex = LinearIndex / m_RowCount;

	int_max RowIndex = LinearIndex % m_RowCount;

    auto RowCount_source = m_SourceMatrixSharedCopy.GetRowCount();

	int_max LinearIndex_source = m_ColIndexList_source[ColIndex] * RowCount_source + m_RowIndexList_source[RowIndex];

    return m_SourceMatrixSharedCopy[LinearIndex_source];
}


template<typename ElementType>
inline
const ElementType& DenseShadowMatrix<ElementType>::operator()(int_max LinearIndex) const
{
#if defined(MDK_DEBUG_DenseShadowMatrix_Operator_CheckBound)

    if (LinearIndex >= m_ElementCount || LinearIndex < 0)
    {
        MDK_Error("Invalid input @ DenseShadowMatrix::operator(i) const")
        return m_ErrorElement;
    }

#endif

    if (m_LinearIndexList_source.empty() == false)
    {
        return m_SourceMatrixSharedCopy[m_LinearIndexList_source[LinearIndex]];
    }

    int_max ColIndex = LinearIndex / m_RowCount;

    int_max RowIndex = LinearIndex % m_RowCount;

    auto RowCount_source = m_SourceMatrixSharedCopy.GetRowCount();

    int_max LinearIndex_source = m_ColIndexList_source[ColIndex] * RowCount_source + m_RowIndexList_source[RowIndex];

    return m_SourceMatrixSharedCopy[LinearIndex_source];
}


template<typename ElementType>
inline
ElementType& DenseShadowMatrix<ElementType>::operator()(int_max RowIndex, int_max ColIndex)
{
#if defined(MDK_DEBUG_DenseShadowMatrix_Operator_CheckBound)

    if (RowIndex >= m_RowCount || RowIndex < 0 || ColIndex >= m_ColCount || ColIndex < 0)
	{
		MDK_Error("Invalid input @ DenseShadowMatrix::operator(i, j)")
        return m_ErrorElement;
	}

#endif

    if (m_LinearIndexList_source.empty() == false)
	{
        auto LinearIndex = ColIndex *m_RowCount + RowIndex;

        return m_SourceMatrixSharedCopy[m_LinearIndexList_source[LinearIndex]];
	}
	
    auto RowCount_source = m_SourceMatrixSharedCopy.GetRowCount();

	int_max LinearIndex_source = m_ColIndexList_source[ColIndex] * RowCount_source + m_RowIndexList_source[RowIndex];

    return m_SourceMatrixSharedCopy[LinearIndex_source];
}


template<typename ElementType>
inline
const ElementType& DenseShadowMatrix<ElementType>::operator()(int_max RowIndex, int_max ColIndex) const
{
#if defined(MDK_DEBUG_DenseShadowMatrix_Operator_CheckBound)

    if (RowIndex >= m_RowCount || RowIndex < 0 || ColIndex >= m_ColCount || ColIndex < 0)
	{
		MDK_Error("Invalid input @ DenseShadowMatrix::operator(i, j) const")
        return m_ErrorElement;
	}

#endif

    if (m_LinearIndexList_source.empty() == false)
	{
        auto LinearIndex = ColIndex *m_RowCount + RowIndex;

        return m_SourceMatrixSharedCopy[m_LinearIndexList_source[LinearIndex]];
	}
	
    auto RowCount_source = m_SourceMatrixSharedCopy.GetRowCount();

	int_max LinearIndex_source = m_ColIndexList_source[ColIndex] * RowCount_source + m_RowIndexList_source[RowIndex];

    return m_SourceMatrixSharedCopy[LinearIndex_source];
}

//-------------------------------------------- DenseShadowMatrix {+= -= *= /=} DenseMatrix ------------------------------------------------//

template<typename ElementType>
inline 
void DenseShadowMatrix<ElementType>::operator+=(const DenseMatrix<ElementType>& InputMatrix)
{
    auto Size = InputMatrix.GetSize();

    if (m_RowCount <= 0 || Size.RowCount <= 0)
    {
        MDK_Error("Self or Matrix is empty @ DenseShadowMatrix::operator+=(InputMatrix)")
        return;
    }

    if (Size.RowCount == 1 && Size.ColCount == 1)
    {
        (*this) += InputMatrix[0];

        return;
    }

    if (Size.RowCount != m_RowCount || Size.ColCount != m_ColCount)
    {
        MDK_Error("Size does not match @ DenseShadowMatrix::operator+=(Matrix)")
        return;
    }

    //-------------------------------------------------

    if (m_LinearIndexList_source.empty() == true)
    {
        if (m_RowIndexList_source.size() == 1 && m_Flag_All_Col == true)     // SourceMatrix(i,:) += InputMatrix
        {
            m_SourceMatrixSharedCopy.RowOperation(m_RowIndexList_source[0], '+', InputMatrix, false);
            return;
        }
        else if (m_ColIndexList_source.size() == 1 && m_Flag_All_Row == true) // SourceMatrix(:,j) += InputMatrix
        {
            m_SourceMatrixSharedCopy.ColOperation(m_ColIndexList_source[0], '+', InputMatrix, false);
            return;
        }
    }

    //----------------------------------------------------//

    auto ptrInput = InputMatrix.GetElementPointer();

    for (int_max i = 0; i < m_ElementCount; ++i)
    {
        (*this)[i] += ptrInput[i];
    }
}


template<typename ElementType>
inline 
void DenseShadowMatrix<ElementType>::operator-=(const DenseMatrix<ElementType>& InputMatrix)
{
    auto Size = InputMatrix.GetSize();

    if (m_RowCount <= 0 || Size.RowCount <= 0)
    {
        MDK_Error("Self or InputMatrix is empty @ DenseShadowMatrix::operator-=(InputMatrix)")
        return;
    }

    if (Size.RowCount == 1 && Size.ColCount == 1)
    {
        (*this) -= InputMatrix[0];

        return;
    }

    if (Size.RowCount != m_RowCount || Size.ColCount != m_ColCount)
    {
        MDK_Error("Size does not match @ DenseShadowMatrix::operator-=(InputMatrix)")
        return;
    }

    //-------------------------------------------------

    if (m_LinearIndexList_source.empty() == true)
    {
        if (m_RowIndexList_source.size() == 1 && m_Flag_All_Col == true)     // SourceMatrix(i,:) -= InputMatrix
        {
            m_SourceMatrixSharedCopy.RowOperation(m_RowIndexList_source[0], '-', InputMatrix, false); // false: bound check has been done
            return;
        }
        else if (m_ColIndexList_source.size() == 1 && m_Flag_All_Row == true) // SourceMatrix(:,j) -= Matrix
        {
            m_SourceMatrixSharedCopy.ColOperation(m_ColIndexList_source[0], '-', InputMatrix, false); // false: bound check has been done
            return;
        }
    }

    //----------------------------------------------------//

    auto ptrInput = InputMatrix.GetElementPointer();

    for (int_max i = 0; i < m_ElementCount; ++i)
    {
        (*this)[i] -= ptrInput[i];
    }
}


template<typename ElementType>
inline 
void DenseShadowMatrix<ElementType>::operator*=(const DenseMatrix<ElementType>& InputMatrix)
{
    auto Size = InputMatrix.GetSize();

    if (m_RowCount <= 0 || Size.RowCount <= 0)
    {
        MDK_Error("Self or Matrix is empty @ DenseShadowMatrix::operator*=(InputMatrix)")
        return;
    }

    if (Size.RowCount == 1 && Size.ColCount == 1)
    {
        (*this) *= InputMatrix[0];

        return;
    }

    if (m_ColCount != Size.RowCount || Size.RowCount != Size.ColCount)
    {
        MDK_Error("Size does not match @ DenseShadowMatrix::operator*=(Matrix)")
        return;
    }

    //----------------------------------------------------//

    (*this) = this->CreateDenseMatrix() * InputMatrix;
}


template<typename ElementType>
inline 
void DenseShadowMatrix<ElementType>::operator/=(const DenseMatrix<ElementType>& InputMatrix)
{
    auto Size = InputMatrix.GetSize();

    if (m_RowCount <= 0 || Size.RowCount <= 0)
    {
        MDK_Error("Self or Matrix is empty @ DenseShadowMatrix::operator/=(InputMatrix)")
        return;
    }

    if (Size.RowCount == 1 && Size.ColCount == 1)
    {
        (*this) /= InputMatrix[0];

        return;
    }

    if (Size.RowCount != m_RowCount || Size.ColCount != m_ColCount)
    {
        MDK_Error("Size does not match @ DenseShadowMatrix::operator/=(InputMatrix)")
        return;
    }

    //-------------------------------------------------

    if (m_LinearIndexList_source.empty() == true)
    {
        if (m_RowIndexList_source.size() == 1 && m_Flag_All_Col == true)     // SourceMatrix(i,:) /= InputMatrix
        {
            m_SourceMatrixSharedCopy.RowOperation(m_RowIndexList_source[0], '/', InputMatrix, false);
            return;
        }
        else if (m_ColIndexList_source.size() == 1 && m_Flag_All_Row == true) // SourceMatrix(:,j) /= InputMatrix
        {
            m_SourceMatrixSharedCopy.ColOperation(m_ColIndexList_source[0], '/', InputMatrix, false);
            return;
        }
    }

    //----------------------------------------------------//

    auto ptrInput = InputMatrix.GetElementPointer();

    for (int_max i = 0; i < m_ElementCount; ++i)
    {
        (*this)[i] /= ptrInput[i];
    }
}

//--------------------------------------------- DenseShadowMatrix {+= -= *= /=} Element ------------------------------------------------------------//

template<typename ElementType>
inline
void DenseShadowMatrix<ElementType>::operator+=(const ElementType& Element)
{
    if (m_RowCount <= 0)
    {
        MDK_Error("Self is empty @ DenseShadowMatrix::operator+=(Element)")
        return;
    }

    //-------------------------------------------------

    if (m_LinearIndexList_source.empty() == true)
    {
        if (m_RowIndexList_source.size() == 1 && m_Flag_All_Col == true)     // SourceMatrix(i,:) += Element
        {
            m_SourceMatrixSharedCopy.RowOperation(m_RowIndexList_source[0], '+', Element);
            return;
        }
        else if (m_ColIndexList_source.size() == 1 && m_Flag_All_Row == true) // SourceMatrix(:,j) += Element
        {
            m_SourceMatrixSharedCopy.ColOperation(m_RowIndexList_source[0], '+', Element);
            return;
        }
    }

    //----------------------------------------------------//

    for (int_max i = 0; i < m_ElementCount; ++i)
    {
        (*this)[i] += Element;
    }
}


template<typename ElementType>
inline
void DenseShadowMatrix<ElementType>::operator-=(const ElementType& Element)
{
    if (m_RowCount <= 0)
    {
        MDK_Error("Self is empty @ DenseShadowMatrix::operator-=(Element)")
        return;
    }

    //-------------------------------------------------

    if (m_LinearIndexList_source.empty() == true)
    {
        if (m_RowIndexList_source.size() == 1 && m_Flag_All_Col == true)     // SourceMatrix(i,:) -= Element
        {
            m_SourceMatrixSharedCopy.RowOperation(m_RowIndexList_source[0], '-', Element);
            return;
        }
        else if (m_ColIndexList_source.size() == 1 && m_Flag_All_Row == true) // SourceMatrix(:,j) -= Element
        {
            m_SourceMatrixSharedCopy.ColOperation(m_ColIndexList_source[0], '-', Element);
            return;
        }
    }

    //----------------------------------------------------//

    for (int_max i = 0; i < m_ElementCount; ++i)
    {
        (*this)[i] -= Element;
    }
}


template<typename ElementType>
inline
void DenseShadowMatrix<ElementType>::operator*=(const ElementType& Element)
{
    if (m_RowCount <= 0)
    {
        MDK_Error("Self is empty @ DenseShadowMatrix::operator*=(Element)")
        return;
    }

    //-------------------------------------------------

    if (m_LinearIndexList_source.empty() == true)
    {
        if (m_RowIndexList_source.size() == 1 && m_Flag_All_Col == true)     // SourceMatrix(i,:) *= Element
        {
            m_SourceMatrixSharedCopy.RowOperation(m_RowIndexList_source[0], '*', Element);
            return;
        }
        else if (m_ColIndexList_source.size() == 1 && m_Flag_All_Row == true) // SourceMatrix(:,j) *= Element
        {
            m_SourceMatrixSharedCopy.ColOperation(m_ColIndexList_source[0], '*', Element);
            return;
        }
    }

    //----------------------------------------------------//

    for (int_max i = 0; i < m_ElementCount; ++i)
    {
        (*this)[i] *= Element;
    }
}


template<typename ElementType>
inline
void DenseShadowMatrix<ElementType>::operator/=(const ElementType& Element)
{
    if (m_RowCount <= 0)
    {
        MDK_Error("Self is empty @ DenseShadowMatrix::operator/=(Element)")
        return;
    }

    //-------------------------------------------------

    if (m_LinearIndexList_source.empty() == true)
    {
        if (m_RowIndexList_source.size() == 1 && m_Flag_All_Col == true)     // SourceMatrix(i,:) /= Element
        {
            m_SourceMatrixSharedCopy.RowOperation(m_RowIndexList_source[0], '/', Element);
            return;
        }
        else if (m_ColIndexList_source.size() == 1 && m_Flag_All_Row == true) // SourceMatrix(:,j) /= Element
        {
            m_SourceMatrixSharedCopy.ColOperation(m_RowIndexList_source[0], '/', Element);
            return;
        }
    }

    //----------------------------------------------------//

    for (int_max i = 0; i < m_ElementCount; ++i)
    {
        (*this)[i] /= Element;
    }
}

//----------------------------------------------- ShadowMatrix {+= -= *= /=} ShadowMatrix ---------------------------------------------//

template<typename ElementType>
inline
void DenseShadowMatrix<ElementType>::operator+=(const DenseShadowMatrix<ElementType>& ShadowMatrix)
{
    auto Size = ShadowMatrix.GetSize();

    if (Size.RowCount == 1 && Size.ColCount == 1)
    {
        (*this) += ShadowMatrix[0];

        return;
    }

    if (Size.RowCount != m_RowCount || Size.ColCount != m_ColCount)
    {
        MDK_Error("Size does not match @ DenseShadowMatrix::operator+=(ShadowMatrix)")
        return;
    }

    if (m_RowCount <= 0 || Size.RowCount <= 0)
    {
        MDK_Error("Self or ShadowMatrix is empty @ DenseShadowMatrix::operator+=(ShadowMatrix)")
        return;
    }

    //-------------------------------------------

    if (m_SourceMatrixSharedCopy.GetElementPointer() == ShadowMatrix.m_SourceMatrixSharedCopy.GetElementPointer())
    {
        (*this) += ShadowMatrix.CreateDenseMatrix();
    }
    else
    {
        for (int_max i = 0; i < m_ElementCount; ++i)
        {
            (*this)[i] += ShadowMatrix[i];
        }
    }
}


template<typename ElementType>
inline
void DenseShadowMatrix<ElementType>::operator-=(const DenseShadowMatrix<ElementType>& ShadowMatrix)
{
    auto Size = ShadowMatrix.GetSize();

    if (Size.RowCount == 1 && Size.ColCount == 1)
    {
        (*this) -= ShadowMatrix[0];

        return;
    }

    if (Size.RowCount != m_RowCount || Size.ColCount != m_ColCount)
    {
        MDK_Error("Size does not match @ DenseShadowMatrix::operato-=(ShadowMatrix)")
        return;
    }

    if (m_RowCount <= 0 || Size.RowCount <= 0)
    {
        MDK_Error("Self or ShadowMatrix is empty @ DenseShadowMatrix::operator-=(ShadowMatrix)")
        return;
    }

    //-------------------------------------------

    if (m_SourceMatrixSharedCopy.GetElementPointer() == ShadowMatrix.m_SourceMatrixSharedCopy.GetElementPointer())
    {
        (*this) -= ShadowMatrix.CreateDenseMatrix();
    }
    else
    {
        for (int_max i = 0; i < m_ElementCount; ++i)
        {
            (*this)[i] -= ShadowMatrix[i];
        }
    }
}


template<typename ElementType>
inline
void DenseShadowMatrix<ElementType>::operator*=(const DenseShadowMatrix<ElementType>& ShadowMatrix)
{
    auto Size = ShadowMatrix.GetSize();

    if (Size.RowCount == 1 && Size.ColCount == 1)
    {
        (*this) *= ShadowMatrix[0];

        return;
    }

    if (m_ColCount != Size.RowCount || Size.RowCount != Size.ColCount)
    {
        MDK_Error("Size does not match @ DenseShadowMatrix::operator*=(ShadowMatrix)")
        return;
    }

    if (m_RowCount <= 0 || Size.RowCount <= 0)
    {
        MDK_Error("Self or ShadowMatrix is empty @ DenseShadowMatrix::operator*=(ShadowMatrix)")
        return;
    }

    //-----------------------------------------------------------

    (*this) = this->CreateDenseMatrix() * ShadowMatrix.CreateDenseMatrix();
}


template<typename ElementType>
inline
void DenseShadowMatrix<ElementType>::operator/=(const DenseShadowMatrix<ElementType>& ShadowMatrix)
{
    auto Size = ShadowMatrix.GetSize();

    if (Size.RowCount == 1 && Size.ColCount == 1)
    {
        (*this) /= ShadowMatrix[0];

        return;
    }

    if (Size.RowCount != m_RowCount || Size.ColCount != m_ColCount)
    {
        MDK_Error("Size does not match @ DenseShadowMatrix::operato/=(ShadowMatrix)")
        return;
    }

    if (m_RowCount <= 0 || Size.RowCount <= 0)
    {
        MDK_Error("Self or ShadowMatrix is empty @ DenseShadowMatrix::operator/=(ShadowMatrix)")
        return;
    }

    //-------------------------------------------

    if (m_SourceMatrixSharedCopy.GetElementPointer() == ShadowMatrix.m_SourceMatrixSharedCopy.GetElementPointer())
    {
        (*this) /= ShadowMatrix.CreateDenseMatrix();
    }
    else
    {
        for (int_max i = 0; i < m_ElementCount; ++i)
        {
            (*this)[i] /= ShadowMatrix[i];
        }
    }
}

//----------------------------------------------- ShadowMatrix {+= -= *= /=} GlueMatrixForLinearCombination ----------------------------------------//

template<typename ElementType>
inline
void DenseShadowMatrix<ElementType>::operator+=(const DenseGlueMatrixForLinearCombination<ElementType>& GlueMatrix)
{
    (*this) += GlueMatrix.CreateDenseMatrix()
}


template<typename ElementType>
inline
void DenseShadowMatrix<ElementType>::operator-=(const DenseGlueMatrixForLinearCombination<ElementType>& GlueMatrix)
{
    (*this) -= GlueMatrix.CreateDenseMatrix()
}


template<typename ElementType>
inline
void DenseShadowMatrix<ElementType>::operator*=(const DenseGlueMatrixForLinearCombination<ElementType>& GlueMatrix)
{
    (*this) *= GlueMatrix.CreateDenseMatrix()
}


template<typename ElementType>
inline
void DenseShadowMatrix<ElementType>::operator/=(const DenseGlueMatrixForLinearCombination<ElementType>& GlueMatrix)
{
    (*this) /= GlueMatrix.CreateDenseMatrix()
}

//------------------------------------------------ ShadowMatrix {+= -= *= /=} GlueMatrixForMultiplication --------------------------------------------//

template<typename ElementType>
inline
void DenseShadowMatrix<ElementType>::operator+=(const DenseGlueMatrixForMultiplication<ElementType>& GlueMatrix)
{
    (*this) += GlueMatrix.CreateDenseMatrix()
}


template<typename ElementType>
inline
void DenseShadowMatrix<ElementType>::operator-=(const DenseGlueMatrixForMultiplication<ElementType>& GlueMatrix)
{
    (*this) -= GlueMatrix.CreateDenseMatrix()
}


template<typename ElementType>
inline
void DenseShadowMatrix<ElementType>::operator*=(const DenseGlueMatrixForMultiplication<ElementType>& GlueMatrix)
{
    (*this) *= GlueMatrix.CreateDenseMatrix()
}

template<typename ElementType>
inline
void DenseShadowMatrix<ElementType>::operator/=(const DenseGlueMatrixForMultiplication<ElementType>& GlueMatrix)
{
    (*this) /= GlueMatrix.CreateDenseMatrix()
}


template<typename ElementType>
inline
DenseMatrix<ElementType> DenseShadowMatrix<ElementType>::ElementMultiply(const DenseMatrix<ElementType>& InputMatrix) const
{
    auto tempMatrix = this->CreateDenseMatrix();

    MatrixElementMultiply(tempMatrix, tempMatrix, InputMatrix);

    return tempMatrix;   
}


template<typename ElementType>
inline
DenseMatrix<ElementType> DenseShadowMatrix<ElementType>::ElementMultiply(const ElementType& Element) const
{
    auto tempMatrix = this->CreateDenseMatrix();

    MatrixElementMultiply(tempMatrix, tempMatrix, Element);

    return tempMatrix;
}


template<typename ElementType>
inline
DenseMatrix<ElementType> DenseShadowMatrix<ElementType>::ElementMultiply(const DenseShadowMatrix<ElementType>& ShadowMatrix) const
{
    auto Size = ShadowMatrix.GetSize();

    if (Size.RowCount == 1 && Size.ColCount == 1)
    {
        return this->ElementMultiply(ShadowMatrix(0));
    }

    DenseMatrix<ElementType> tempMatrix;

    if (Size.RowCount != m_RowCount || Size.ColCount != m_ColCount)
    {
        MDK_Error("Size does not match @ DenseShadowMatrix::ElementMultiply(ShadowMatrix)")
        return tempMatrix;
    }

    if (m_RowCount <= 0 || Size.RowCount <= 0)
    {
        MDK_Error("Self or ShadowMatrix is empty @ DenseShadowMatrix::ElementMultiply(ShadowMatrix)")
        return tempMatrix;
    }

    tempMatrix.FastResize(m_RowCount, m_ColCount);

    auto ptrTemp = tempMatrix.GetElementPointer();

    for (int_max i = 0; i < m_ElementCount; ++i)
    {
        ptrTemp[i] = (*this)[i] * ShadowMatrix[i];
    }

    return  tempMatrix;
}


template<typename ElementType>
inline
DenseMatrix<ElementType> DenseShadowMatrix<ElementType>::ElementMultiply(const DenseGlueMatrixForLinearCombination<ElementType>& GlueMatrix) const
{
    auto tempMatrixA = this->CreateDenseMatrix();

    auto tempMatrixB = GlueMatrix.CreateDenseMatrix();

    MatrixElementMultiply(tempMatrixA, tempMatrixA, tempMatrixB);

    return tempMatrixA;
}


template<typename ElementType>
inline
DenseMatrix<ElementType> DenseShadowMatrix<ElementType>::ElementMultiply(const DenseGlueMatrixForMultiplication<ElementType>& GlueMatrix) const
{
    auto tempMatrixA = this->CreateDenseMatrix();

    auto tempMatrixB = GlueMatrix.CreateDenseMatrix();

    MatrixElementMultiply(tempMatrixA, tempMatrixA, tempMatrixB);

    return tempMatrixA;
}


}//end namespace mdk

