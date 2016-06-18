#pragma once

namespace mdk
{
 
template<typename ElementType>
inline
DenseMatrix<ElementType>::DenseMatrix()
{
    this->Resize(0, 0);
}


template<typename ElementType>
inline
DenseMatrix<ElementType>::DenseMatrix(const MDK_Symbol_PureEmpty&)
{
}


template<typename ElementType>
inline
DenseMatrix<ElementType>::DenseMatrix(int_max RowCount, int_max ColCount)
{
    this->Resize(RowCount, ColCount);
}


template<typename ElementType>
inline
DenseMatrix<ElementType>::DenseMatrix(MatrixSize Size)
{
	this->Resize(Size);
}


template<typename ElementType>
inline
DenseMatrix<ElementType>::DenseMatrix(const ElementType& Element)
{
    this->Resize(1, 1);
    (*this)(0) = Element;
}


template<typename ElementType>
inline
DenseMatrix<ElementType>::DenseMatrix(const std::initializer_list<ElementType>& InputList)
{
    this->Resize(0, 0);
    if (InputList.size() > 0)
    {
        (*this) = InputList;
    }
}


template<typename ElementType>
inline
DenseMatrix<ElementType>::DenseMatrix(const std::initializer_list<std::initializer_list<ElementType>>& InputListInList)
{
    this->Resize(0, 0);
    if (InputListInList.size() > 0)
    {
        (*this) = InputListInList;
    }
}


template<typename ElementType>
inline DenseMatrix<ElementType>::DenseMatrix(const std::initializer_list<const DenseMatrix<ElementType>*>& InputList)
{
    this->Resize(0, 0);
    if (InputList.size() > 0)
    {
        (*this) = InputList;
    }
}


template<typename ElementType>
inline DenseMatrix<ElementType>::DenseMatrix(const std::initializer_list<std::initializer_list<const DenseMatrix<ElementType>*>>& InputListInList)
{
    this->Resize(0, 0);
    if (InputListInList.size() > 0)
    {
        (*this) = InputListInList;
    }
}


template<typename ElementType>
inline
DenseMatrix<ElementType>::DenseMatrix(std::vector<ElementType> InputColVector)
{
	this->Resize(0, 0);
	if (InputColVector.size() > 0)
	{
		this->Copy(std::move(InputColVector));
	}
}


template<typename ElementType>
inline
DenseMatrix<ElementType>::DenseMatrix(StdObjectVector<ElementType> InputColVector)
{
	this->Resize(0, 0);
	if (InputColVector.GetElementCount() > 0)
	{
		this->Copy(std::move(InputColVector));
	}
}


template<typename ElementType>
inline
DenseMatrix<ElementType>::DenseMatrix(const ObjectArray<ElementType>& InputColVector)
{
	this->Resize(0, 0);
	if (InputColVector.GetElementCount() > 0)
	{
		this->Copy(InputColVector);
	}
}


template<typename ElementType>
template<int_max VectorFixedLength>
inline
DenseMatrix<ElementType>::DenseMatrix(const DenseVector<ElementType, VectorFixedLength>& InputColVector)
{
    this->Resize(0, 0);
	if (InputColVector.GetElementCount() > 0)
    {
		this->Copy(InputColVector);
    }
}


template<typename ElementType>
inline 
DenseMatrix<ElementType>::DenseMatrix(DenseVector<ElementType> InputColVector)
{
	this->Resize(0, 0);
	if (InputColVector.GetElementCount() > 0)
	{
		this->Copy(std::move(InputColVector));
	}
}


template<typename ElementType>
inline
DenseMatrix<ElementType>::DenseMatrix(const DenseMatrix<ElementType>& InputMatrix)
{
	this->Resize(0, 0);
	this->Copy(InputMatrix);
}


// move constructor
template<typename ElementType>
inline
DenseMatrix<ElementType>::DenseMatrix(DenseMatrix<ElementType>&& InputMatrix)
{
	m_MatrixData = std::move(InputMatrix.m_MatrixData);
}


template<typename ElementType>
inline
DenseMatrix<ElementType>::DenseMatrix(const DenseShadowMatrix<ElementType>& ShadowMatrix)
{
    this->Resize(0, 0);
	this->Copy(ShadowMatrix.CreateDenseMatrix());
}


template<typename ElementType>
inline
DenseMatrix<ElementType>::DenseMatrix(const DenseGlueMatrixForLinearCombination<ElementType>& GlueMatrix)
{
    this->Resize(0, 0);
	this->Copy(GlueMatrix.CreateDenseMatrix());
}


template<typename ElementType>
inline
DenseMatrix<ElementType>::DenseMatrix(const DenseGlueMatrixForMultiplication<ElementType>& GlueMatrix)
{
    this->Resize(0, 0);
    this->Copy(GlueMatrix.CreateDenseMatrix());
}


template<typename ElementType>
inline
DenseMatrix<ElementType>::DenseMatrix(ElementType* InputElementPointer, int_max InputRowCount, int_max InputColCount, bool IsSizeFixed)
{
    this->Resize(0, 0);
    this->Share(InputElementPointer, InputRowCount, InputColCount);
    m_MatrixData->IsSizeFixed = IsSizeFixed;
}


template<typename ElementType>
inline
DenseMatrix<ElementType>::~DenseMatrix()
{
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::operator=(const DenseMatrix<ElementType>& InputMatrix)
{
    this->Copy(InputMatrix);
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::operator=(DenseMatrix<ElementType>&& InputMatrix)
{
	this->Copy(std::move(InputMatrix));
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::operator=(const DenseShadowMatrix<ElementType>& ShadowMatrix)
{
	this->Copy(ShadowMatrix);
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::operator=(const DenseGlueMatrixForLinearCombination<ElementType>& GlueMatrix)
{
	this->Copy(GlueMatrix);
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::operator=(const DenseGlueMatrixForMultiplication<ElementType>& GlueMatrix)
{
	this->Copy(GlueMatrix);
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::operator=(const ElementType& Element)
{
    auto ElementNumber = this->GetElementCount();

    if (this->IsSizeFixed() == true)
    {
        if (ElementNumber != 1)
        {
            MDK_Error("Can not change matrix size @ DenseMatrix::operator=(Element)")
            return;
        }
        else
        {
            (*this)[0] = Element;
        }
    }
    else
    {
        if (ElementNumber == 0)
        {
            this->Resize(1, 1);

            (*this)[0] = Element;
        }
        else if (ElementNumber == 1)
        {
            (*this)[0] = Element;
        }
        else
        {
            MDK_Warning("The size of the matrix changes to 1x1 @ DenseMatrix::operator=(Element)")

            this->Clear();
            this->Resize(1, 1);
            (*this)[0] = Element;
        }
    }
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::operator=(const std::initializer_list<ElementType>& InputList)
{
//InputList is treated as a row vector

    auto InputLength = int_max(InputList.size());

    if (InputLength <= 0)
    {        
        if (this->IsSizeFixed() == true)
        {
            if (this->IsEmpty() == false)
            {
                MDK_Error("Can not change matrix size @ DenseMatrix::operator=(initializer_list)")
            }
        }
        else
        {
			MDK_Warning("Input is empty, try to clear self @ DenseMatrix::operator=(initializer_list)")
            this->Clear();
        }
        return;
    }

    auto SelfSize = this->GetSize();

    if (SelfSize.RowCount == 1 && SelfSize.ColCount == InputLength)
    {
        this->SetRow(0, InputList);
        return;
    }
    else if (SelfSize.ColCount == 1 && SelfSize.RowCount == InputLength)
    {
        this->SetCol(0, InputList);
        return;
    }

    if (this->IsSizeFixed() == true)
    {
        MDK_Error("Can not change matrix size @ DenseMatrix::operator=(initializer_list)")
    }
    else
    {
        this->FastResize(1, InputLength);    
        this->SetRow(0, InputList);
    }
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::operator=(const std::initializer_list<std::initializer_list<ElementType>>& InputListInList)
{
//InputList is a matrix, each sub list is a row vector

    auto InputRowCount = int_max(InputListInList.size());

    if (InputRowCount <= 0)
    {
        MDK_Warning("Input is empty, try to clear self @ DenseMatrix::operator=(InputListInList)")
        
        if (this->IsSizeFixed() == true)
        {
            if (this->IsEmpty() == false)
            {
                MDK_Error("Can not change matrix size @ DenseMatrix::operator=(InputListInList)")
            }
        }
        else
        {
            this->Clear();
        }
        return;
    }

    auto InputColCount = (InputListInList.begin()[0]).size();

    //check each row-list 
    for (int_max i = 1; i < InputRowCount; ++i)
    {
        auto subListLength = int_max((InputListInList.begin()[i]).size());

        if (subListLength != InputColCount)
        {
            MDK_Error("The number of Elements in each row is not the same: #" << i << " @ DenseMatrix::operator=(InputListInList)")
            return;
        }
    }

    auto SelfSize = this->GetSize();

    if (this->IsSizeFixed() == true)
    {
        if (SelfSize.RowCount != InputRowCount || SelfSize.ColCount != InputColCount)
        {
            MDK_Error("Size can not change @ DenseMatrix::operator=(list in list)");
            return;
        }
    }
    else
    {
        this->FastResize(InputRowCount, InputColCount);

        SelfSize.RowCount = InputRowCount;
        SelfSize.ColCount = InputColCount;
    }

    auto RawPointer = this->GetElementPointer();

    for (int_max i = 0; i < SelfSize.RowCount; ++i)
    {
        auto subList = InputListInList.begin()[i];

        int_max Index = 0;

        for (int_max j = 0; j < SelfSize.ColCount; ++j)
        {
            RawPointer[Index + i] = subList.begin()[j];

            Index += SelfSize.RowCount;
        }
    }
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::operator=(const std::initializer_list<const DenseMatrix<ElementType>*>& InputList)
{
    //InputList is treated as a row vector, and each element in this vector is a matrix

    auto InputMatrixNumber = int_max(InputList.size());

    if (InputMatrixNumber <= 0)
    {
        MDK_Warning("Input is empty, try to clear self @ DenseMatrix::operator=(initializer_list of matrix pointer)")

        if (this->IsSizeFixed() == true)
        {
            if (this->IsEmpty() == false)
            {
                MDK_Error("Can not change matrix size @ DenseMatrix::operator=(initializer_list of matrix pointer)")
            }
        }
        else
        {
            this->Clear();
        }
        return;
    }

    bool IsSelfInInputList = false;

    std::vector<int_max> ColCountList;
    ColCountList.resize(InputMatrixNumber);

    int_max InputRowCount = 0;
    int_max TotalColCount = 0;

    for (int_max k = 0; k < InputMatrixNumber; k++)
    {
        ColCountList[k] = 0;

        auto InputMatrixPtr = InputList.begin()[k];
        if (InputMatrixPtr != nullptr)
        {
            if (InputRowCount == 0)
            {
                InputRowCount = InputMatrixPtr->GetRowCount();
            }

            ColCountList[k] = InputMatrixPtr->GetColCount();

            TotalColCount += InputMatrixPtr->GetColCount();

			if (InputMatrixPtr->IsEmpty() == false && InputRowCount != InputMatrixPtr->GetRowCount())
            {
                MDK_Error("RowCount is not the same in the list @ DenseMatrix::operator=(initializer_list of matrix pointer)")
                return;
            }

			if (this->GetElementPointer() != nullptr && this->GetElementPointer() == InputMatrixPtr->GetElementPointer())
            {
                IsSelfInInputList = true;
            }
        }
    }

    if (TotalColCount == 0)
    {
        MDK_Warning("TotalColCount is 0, try to clear self @ DenseMatrix::operator=(initializer_list of matrix pointer)")
        
        if (this->IsSizeFixed() == true)
        {
            if (this->IsEmpty() == false)
            {
                MDK_Error("Can not change matrix size @ DenseMatrix::operator=(initializer_list of matrix pointerr)")
            }
        }
        else
        {
            this->Clear();
        }
        return;
    }

    auto SelfSize = this->GetSize();

    if (IsSelfInInputList == false)
    {
        if (this->IsSizeFixed() == true)
        {
            if (SelfSize.RowCount != InputRowCount || SelfSize.ColCount != TotalColCount)
            {
                MDK_Error("Can not change matrix size @ DenseMatrix::operator=(initializer_list)")
                return;
            }
        }
        else
        {
            this->FastResize(InputRowCount, TotalColCount);
        }

        int_max temp_ColCount = 0;

        for (int_max k = 0; k < InputMatrixNumber; k++)
        {
            auto InputMatrixPtr = InputList.begin()[k];
            if (InputMatrixPtr != nullptr)
            {
                auto ColPtr = InputMatrixPtr->GetElementPointer();

                for (int_max j = temp_ColCount; j < temp_ColCount + ColCountList[k]; ++j, ColPtr += InputRowCount)
                {
                    this->SetCol(j, ColPtr);
                }

                temp_ColCount += ColCountList[k];
            }
        }
    }
    else // Self is in InputList 
    {
        if (TotalColCount == SelfSize.ColCount)
        {
            MDK_Warning("Self = {&Self} @  @ DenseMatrix::operator=(initializer_list)")
            return;
        }
        else
        {
            DenseMatrix<ElementType> tempMatrix = InputList;
            this->Copy(std::move(tempMatrix));
        }
    }
}


//template<typename ElementType>
//inline
//void DenseMatrix<ElementType>::operator=(const std::initializer_list<std::initializer_list<const DenseMatrix<ElementType>*>>& InputListInList)
//{
//   MDK_Error("Not implemented yet @ DenseMatrix::operator=(InputListInList of matrix pointer)")
//}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::operator=(const std::vector<ElementType>& InputColVector)
{
	this->Copy(InputColVector);
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::operator=(std::vector<ElementType>&& InputColVector)
{
	this->Copy(std::move(InputColVector));
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::operator=(const StdObjectVector<ElementType>& InputColVector)
{
	this->Copy(InputColVector);
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::operator=(StdObjectVector<ElementType>&& InputColVector)
{
	this->Copy(std::move(InputColVector));
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::operator=(const ObjectArray<ElementType>& InputColVector)
{
	this->Copy(InputColVector);
}


template<typename ElementType>
template<int_max TemplateLength>
inline
void DenseMatrix<ElementType>::operator=(const DenseVector<ElementType, TemplateLength>& InputColVector)
{
	this->Copy(InputColVector);
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::operator=(DenseVector<ElementType>&& InputColVector)
{
	this->Copy(std::move(InputColVector));
}


template<typename ElementType>
template<typename ElementType_Input>
inline
void DenseMatrix<ElementType>::Copy(const DenseMatrix<ElementType_Input>& InputMatrix)
{
    if (InputMatrix.IsEmpty() == true)
    {
        if (this->IsSizeFixed() == true)
        {
            if (this->IsEmpty() == false)
            {
                MDK_Error("Size can not change @ DenseMatrix::Copy(InputMatrix)")
			}
            return;
        }
        else
        {
            //MDK_Warning("InputMatrix is empty, and this matrix is set to be empty @ DenseMatrix::Copy(InputMatrix)")
            this->Clear();
            return;
        }       
    }

    // copy data
    this->Copy(InputMatrix.GetElementPointer(), InputMatrix.GetRowCount(), InputMatrix.GetColCount());
}


template<typename ElementType>
template<typename ElementType_Input>
inline
void DenseMatrix<ElementType>::Copy(const ElementType_Input* InputElementPointer, int_max InputRowCount, int_max InputColCount)
{
	if (InputRowCount < 0 || InputColCount < 0)
	{
		MDK_Error("Invalid input @ DenseMatrix::Copy(ElementType_Input*, RowCount, ColCount)")
		return;
	}

	if (this->IsPureEmpty() == true)
	{
		this->Resize(0, 0);
	}

    if (InputElementPointer == nullptr || InputRowCount == 0 || InputColCount == 0)
    {
		this->Copy(MDK_EMPTY);
		return;
    }

    auto tempElementType = FindMatrixElementType(InputElementPointer[0]);

    if (tempElementType == MatrixElementTypeEnum::UNKNOWN)
    {
        MDK_Error("Input type is unknown @ DenseMatrix::Copy(ElementType_Input*, RowCount, ColCount)")
        return;
    }

    // if this matrix is not empty, check if this and Input Share the same data
    if (this->IsEmpty() == false)
    {
        if (std::size_t(InputElementPointer) == std::size_t(this->GetElementPointer()))
        {
           // MDK_Warning("A Matrix tries to Copy itself @ DenseMatrix::Copy(ElementType_Input*, RowCount, ColCount)")
            return;
        }
    }

    //------------------------------------------------------------------

    auto SelfSize = this->GetSize();

    if (this->IsSizeFixed() == true)
    {
        if (InputRowCount != SelfSize.RowCount || InputColCount != SelfSize.ColCount)
        {
            MDK_Error("Can not change matrix size @ DenseMatrix::Copy(ElementType_Input*, InputRowCount, InputColCount)")
            return;
        }
    }
    else
    {
        this->FastResize(InputRowCount, InputColCount);

        SelfSize.RowCount = InputRowCount;
        SelfSize.ColCount = InputColCount;
    }

    //copy data ----------------------------------------------------------

    auto BeginPointer = this->GetElementPointer();

    auto tempPtr = InputElementPointer;

    auto Self_ElementNumber = SelfSize.RowCount * SelfSize.ColCount;

    for (auto Ptr = BeginPointer; Ptr < BeginPointer + Self_ElementNumber; ++Ptr)
    {
        Ptr[0] = ElementType(tempPtr[0]);

        ++tempPtr;
    }
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::Copy(DenseMatrix<ElementType>&& InputMatrix)
{
	if (this == &InputMatrix)
	{
		MDK_Warning("A Matrix try to Copy itself @ DenseMatrix::Copy(&& InputMatrix)")
		return;
	}

	if (this->IsPureEmpty() == true)
	{
		this->Resize(0, 0);
	}

	auto InputSize = InputMatrix.GetSize();

	auto SelfSize = this->GetSize();

	if (this->IsSizeFixed() == true)
	{
		if (InputSize.RowCount != SelfSize.RowCount || InputSize.ColCount != SelfSize.ColCount)
		{
			MDK_Error("Size does not match @ DenseMatrix::Copy(&& InputMatrix)")
			return;
		}
	}

	if (InputMatrix.IsEmpty() == true)
	{
		if (SelfSize.RowCount > 0)
		{
			//MDK_Warning("InputMatrix is empty, and this matrix is set to be empty @ DenseMatrix::Copy(&& InputMatrix)")
			this->Clear();
		}
		return;
	}

	// MatrixA = MatrixA
	if (this->GetElementPointer() == InputMatrix.GetElementPointer())
	{
		//MDK_Warning("A Matrix tries to take itself @ DenseMatrix::Take(InputMatrix)")
		return;
	}

	// now, InputMatrix is not empty, and is not self

	//note: m_MatrixData.swap(InputMatrix.m_MatrixData) will invalidate Share()

	m_MatrixData->RowCount = InputMatrix.m_MatrixData->RowCount;

	m_MatrixData->ColCount = InputMatrix.m_MatrixData->ColCount;

	m_MatrixData->StdVector = std::move(InputMatrix.m_MatrixData->StdVector);

	m_MatrixData->ElementPointer = InputMatrix.m_MatrixData->ElementPointer;

	m_MatrixData->ErrorElement = InputMatrix.m_MatrixData->ErrorElement;

	// Clear InputMatrix to be empty
	InputMatrix.Clear();
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::Copy(const DenseShadowMatrix<ElementType>& ShadowMatrix)
{
	if (this->IsPureEmpty() == true)
	{
		this->Resize(0, 0);
	}

	auto InputSize = ShadowMatrix.GetSize();

	auto SelfSize = this->GetSize();

	if (InputSize.RowCount == SelfSize.RowCount && InputSize.ColCount == SelfSize.ColCount)
	{
		ShadowMatrix.CreateDenseMatrix(*this);
	}
	else
	{
		if (this->IsSizeFixed() == true)
		{
			MDK_Error("Size can not change @ DenseMatrix::Copy(ShadowMatrix)")
			return;
		}

		this->Copy(ShadowMatrix.CreateDenseMatrix());
	}
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::Copy(const DenseGlueMatrixForLinearCombination<ElementType>& GlueMatrix)
{
	if (this->IsPureEmpty() == true)
	{
		this->Resize(0, 0);
	}

	auto InputSize = GlueMatrix.GetSize();

	auto SelfSize = this->GetSize();

	if (InputSize.RowCount == SelfSize.RowCount && InputSize.ColCount == SelfSize.ColCount)
	{
		GlueMatrix.CreateDenseMatrix(*this);
	}
	else
	{
		if (this->IsSizeFixed() == true)
		{
			MDK_Error("Size can not change @ DenseMatrix::Copy(DenseGlueMatrixForLinearCombination)")
			return;
		}

		this->Copy(GlueMatrix.CreateDenseMatrix());
	}
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::Copy(const DenseGlueMatrixForMultiplication<ElementType>& GlueMatrix)
{
	if (this->IsPureEmpty() == true)
	{
		this->Resize(0, 0);
	}

	auto InputSize = GlueMatrix.GetSize();

	auto SelfSize = this->GetSize();

	if (InputSize.RowCount == SelfSize.RowCount && InputSize.ColCount == SelfSize.ColCount)
	{
		GlueMatrix.CreateDenseMatrix(*this);
	}
	else
	{
		if (this->IsSizeFixed() == true)
		{
			MDK_Error("Size can not change @ DenseMatrix::Copy(DenseGlueMatrixForMultiplication)")
			return;
		}

		this->Copy(GlueMatrix.CreateDenseMatrix());
	}
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::Copy(const std::vector<ElementType>& InputColVector)
{
	this->Copy(InputColVector.data(), int_max(InputColVector.size()), 1);
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::Copy(std::vector<ElementType>&& InputColVector)
{
	auto InputLength = int_max(InputColVector.size());

	// MatrixA = MatrixA
	if (this->GetElementPointer() == InputColVector.data())
	{
		if (InputLength > 0 && this->IsEmpty() == false) // if(nullptr == nullptr)
		{
			MDK_Warning("A Matrix try to copy itself @ DenseMatrix::Copy(std::vector)")
		}
		return;
	}

	if (this->IsPureEmpty() == true)
	{
		this->Resize(0, 0);
	}

	auto SelfSize = this->GetSize();

	if (this->IsSizeFixed() == true)
	{
		if ((SelfSize.RowCount == 1 && SelfSize.ColCount == InputLength) || (SelfSize.ColCount == 1 && SelfSize.RowCount == InputLength))
		{
			m_MatrixData->StdVector = std::move(InputColVector);
			m_MatrixData->ElementPointer = m_MatrixData->StdVector.data();
		}
		else
		{
			MDK_Error("Size can not change @ DenseMatrix::Copy(std::vector)")
		}
	}
	else
	{
		if (InputLength == 0)
		{
			if (SelfSize.RowCount > 0)
			{
				//MDK_Warning("InputColVector is empty, and this matrix is set to be empty @ DenseMatrix::Copy(DenseVector)")
				this->Clear();
			}
			return;
		}

		// now, InputColVector is not empty

		if (SelfSize.RowCount == 1)
		{
			m_MatrixData->StdVector = std::move(InputColVector);
			m_MatrixData->ElementPointer = m_MatrixData->StdVector.data();
			m_MatrixData->ColCount = InputLength;
		}
		else if (SelfSize.ColCount == 1)
		{
			m_MatrixData->StdVector = std::move(InputColVector);
			m_MatrixData->ElementPointer = m_MatrixData->StdVector.data();
			m_MatrixData->RowCount = InputLength;
		}
		else
		{
			m_MatrixData->StdVector = std::move(InputColVector);
			m_MatrixData->ElementPointer = m_MatrixData->StdVector.data();
			m_MatrixData->RowCount = InputLength;
			m_MatrixData->ColCount = 1;
		}
	}
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::Copy(const StdObjectVector<ElementType>& InputColVector)
{
	this->Copy(InputColVector.GetElementPointer(), InputColVector.GetElementCount(), 1);
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::Copy(StdObjectVector<ElementType>&& InputColVector)
{
	this->Copy(InputColVector.StdVector());
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::Copy(const ObjectArray<ElementType>& InputColVector)
{
	this->Copy(InputColVector.GetElementPointer(), InputColVector.GetElementCount(), 1);
}


template<typename ElementType>
template<int_max TemplateLength>
inline
void DenseMatrix<ElementType>::Copy(const DenseVector<ElementType, TemplateLength>& InputColVector)
{
	auto InputVectorLength = InputColVector.GetLength();

	if (InputVectorLength <= 0)
	{
		this->Copy(MDK_EMPTY);
		return;
	}

	if (this->IsPureEmpty() == true)
	{
		this->Resize(0, 0);
	}

	auto SelfSize = this->GetSize();

	if (SelfSize.RowCount == 1 && SelfSize.ColCount == InputVectorLength)
	{// self is row vector, keep as row vector
		this->SetRow(0, InputColVector.GetElementPointer());
	}
	else if (SelfSize.ColCount == 1 && SelfSize.RowCount == InputVectorLength)
	{// self is col vector, keep as col vector
		this->SetCol(0, InputColVector.GetElementPointer());
	}
	else
	{// change to col vector
		if (this->IsSizeFixed() == true)
		{
			MDK_Error("Can not change matrix size @ DenseMatrix::operator=(DenseVector)")
			return;
		}
		else
		{
			this->FastResize(InputVectorLength, 1);
			this->SetCol(0, InputColVector.GetElementPointer());
		}
	}
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::Copy(DenseVector<ElementType>&& InputColVector)
{
	this->Copy(InputColVector.StdVector());
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::Copy(const MDK_Symbol_Empty&)
{
	if (this->IsPureEmpty() == true)
	{
		this->Resize(0, 0);
		return;
	}

	if (this->IsSizeFixed() == true)	
	{
		if (this->IsEmpty() == false)
		{			
			MDK_Error("Size can not change @ DenseMatrix::Copy(MDK_EMPTY)")
			return;	
		}
	}	

	this->Clear();
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::Fill(const ElementType& Element)
{
    auto Self_ElementNumber = this->GetElementCount();

    if (Self_ElementNumber <= 0)
    {
        MDK_Error("Self is empty @ DenseMatrix::Fill")
        return;
    }

    auto BeginPointer = this->GetElementPointer();

    for (auto Ptr = BeginPointer; Ptr < BeginPointer + Self_ElementNumber; ++Ptr)
    {
        Ptr[0] = Element;
    }
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::Share(DenseMatrix<ElementType>& InputMatrix)
{
    // Matrix = Matrix
    if (this == &InputMatrix)
    {
        MDK_Warning("A Matrix try to Share itself @ DenseMatrix::Share(InputMatrix)")
        return;
    }

    if (InputMatrix.IsPureEmpty() == true)
    {
        MDK_Error("InputMatrix is pure empty (m_MatrixData is empty) @ DenseMatrix::Share(InputMatrix)")
        return;
    }

    if (this->IsSizeFixed() == true)
    {
        auto InputSize = InputMatrix.GetSize();
        auto SelfSize = this->GetSize();
        if (InputSize.RowCount != SelfSize.RowCount || InputSize.ColCount != SelfSize.ColCount)
        {
            MDK_Error("Size can not change @ DenseMatrix::Share(InputMatrix)")
            return;
        }
    }

    //--------------------------------------------------------------------------------------------------------
    m_MatrixData = InputMatrix.m_MatrixData; // std::Shared_ptr, self assignment test is not necessary
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::ForceShare(const DenseMatrix<ElementType>& InputMatrix)
{
    // Matrix = Matrix
    if (this == &InputMatrix)
    {
        MDK_Warning("A Matrix try to ForceShare itself @ DenseMatrix::ForceShare(InputMatrix)")
        return;
    }

	if (InputMatrix.IsPureEmpty() == true)
	{
		MDK_Error("InputMatrix is pure empty (m_MatrixData is empty) @ DenseMatrix::ForceShare(InputMatrix)")
		return;
	}

    m_MatrixData = InputMatrix.m_MatrixData; // std::Shared_ptr, self assignment check is not necessary
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::Share(ElementType* InputElementPointer, int_max InputRowCount, int_max InputColCount, bool IsSizeFixed)
{
    if (InputElementPointer == nullptr || InputRowCount <= 0 || InputColCount <= 0)
    {
        MDK_Error("Invalid input @ DenseMatrix::Share(...)")
        return;
    }

	if (this->IsPureEmpty() == true)
	{
		this->Resize(0, 0);
	}

    if (this->IsSizeFixed() == true)
    {
        if (m_MatrixData->RowCount != InputRowCount || m_MatrixData->ColCount != InputColCount)
        {
            MDK_Error("Size can not change @ DenseMatrix::Share(...)")
            return;
        }
    }

    m_MatrixData->IsSizeFixed = IsSizeFixed;

    m_MatrixData->RowCount = InputRowCount;
    m_MatrixData->ColCount = InputColCount;

    m_MatrixData->StdVector.clear();

    m_MatrixData->ElementPointer = InputElementPointer;
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::ForceShare(const ElementType* InputElementPointer, int_max InputRowCount, int_max InputColCount, bool IsSizeFixed)
{
    this->Share(const_cast<ElementType*>(InputElementPointer), InputRowCount, InputColCount, IsSizeFixed);
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::Swap(DenseMatrix<ElementType>& InputMatrix)
{
	if (this == &InputMatrix)
	{
		MDK_Warning("A Matrix try to Swap with itself @ DenseMatrix::Swap(InputMatrix)")
		return;
	}

    m_MatrixData.swap(InputMatrix.m_MatrixData); // shared_ptr self swap check is not necessary
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::Recreate()
{
	DenseMatrix<ElementType> EmptyMatrix;
	this->Share(EmptyMatrix);
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::Clear()
{
    if (!m_MatrixData)
    {
        return;
    }

	m_MatrixData->Clear();
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::Reshape(int_max InputRowCount, int_max InputColCount)
{
    if (this->IsSizeFixed() == true)
    {
        MDK_Error("Size can not change @ DenseMatrix::Reshape(...)")
        return;
    }
	
	int_max InputElementNumber = InputRowCount*InputColCount;

	if (InputElementNumber != this->GetElementCount())
	{
		MDK_Error("Size NOT match @ DenseMatrix::Reshape(...)")
		return;
	}

	// InputRowCount or InputColCount may be 0
	// Reshape(1, 0) or Reshape(0, 1) is allowed

	if (InputElementNumber > 0) 
	{
		m_MatrixData->RowCount = InputRowCount;
		m_MatrixData->ColCount = InputColCount;
	}
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::Reshape(MatrixSize InputSize)
{
	this->Reshape(InputSize.RowCount, InputSize.ColCount);
}


template<typename ElementType>
inline 
void DenseMatrix<ElementType>::Resize(int_max InputRowCount, int_max InputColCount)
{
    if (InputRowCount < 0 || InputColCount < 0)
    {
        MDK_Error("Invalid Input: negtive @ DenseMatrix::Resize(int_max RowCount, int_max ColCount)")
        return;
    }

try
{
    //--------initialize the matrix data ----------------------------------------
	if (!m_MatrixData)//if (this->IsPureEmpty() == true)
    {
		m_MatrixData = std::make_shared<DenseMatrixData<ElementType>>();		
    }
    //-------------------------------------------------------------------------

    auto SelfSize = this->GetSize();

    if (InputRowCount == SelfSize.RowCount && InputColCount == SelfSize.ColCount)
    {
        return;
    }

    if (this->IsSizeFixed() == true)
    {
        MDK_Error("Size can not change @ DenseMatrix::Resize(int_max RowCount, int_max ColCount)")
        return;
    }

    if (InputRowCount == 0 || InputColCount == 0) // Resize(1, 0) or Resize(0, 1) is allowed
    {               
        m_MatrixData->RowCount = 0;
        m_MatrixData->ColCount = 0;
        m_MatrixData->StdVector.resize(0);
        m_MatrixData->ElementPointer = nullptr;        
        return;
    }

    // if self is empty
    if (this->IsEmpty() == true)
    {
        m_MatrixData->RowCount = InputRowCount;
        m_MatrixData->ColCount = InputColCount;
        m_MatrixData->StdVector.resize(InputRowCount*InputColCount);
        m_MatrixData->ElementPointer = m_MatrixData->StdVector.data();        
        return;
    }

    // if only Col changes and self is not empty

    if (InputRowCount == SelfSize.RowCount)
    {
        auto Self_ElementNumber = SelfSize.RowCount * SelfSize.ColCount;

        //m_MatrixData->CopyDataToInternalArrayIfNecessary();
        if (m_MatrixData->ElementPointer != m_MatrixData->StdVector.data())
        {
            auto ElementNumber_min = std::min(Self_ElementNumber, InputColCount*InputRowCount);

            m_MatrixData->StdVector.resize(ElementNumber_min);

            for (int_max i = 0; i < ElementNumber_min; ++i)
            {
                m_MatrixData->StdVector[i] = m_MatrixData->ElementPointer[i];
            }

            m_MatrixData->ElementPointer = m_MatrixData->StdVector.data();
        }

        m_MatrixData->RowCount = InputRowCount;
        m_MatrixData->ColCount = InputColCount;
        m_MatrixData->StdVector.resize(InputRowCount*InputColCount);
        m_MatrixData->ElementPointer = m_MatrixData->StdVector.data();        
        return;
    }

    // InputRowCount != SelfSize.RowCount and self is not empty -----------------------------------------------------------------------------

    auto tempDataArray = std::vector<ElementType>(InputRowCount*InputColCount);

    auto RawPointer = m_MatrixData->ElementPointer;

    auto ColCount_min = std::min(SelfSize.ColCount, InputColCount);

    auto RowCount_min = std::min(SelfSize.RowCount, InputRowCount);

    // try to keep old data as much as possible
    for (int_max j = 0; j < ColCount_min; ++j)
    {
        auto tempIndex = j*InputRowCount;

        auto Index = j*SelfSize.RowCount;

        for (int_max i = 0; i < RowCount_min; ++i)
        {
			tempDataArray[tempIndex + i] = RawPointer[Index + i];
        }
    }

    //-------------------------------------------------------------
    m_MatrixData->RowCount = InputRowCount;
    m_MatrixData->ColCount = InputColCount;
	m_MatrixData->StdVector = std::move(tempDataArray);
    m_MatrixData->ElementPointer = m_MatrixData->StdVector.data();

}
catch (...)
{
    MDK_Error("Out of Memory @ DenseMatrix::Resize(int_max InputRowCount, int_max InputColCount)")
    //this->Clear();
}
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::Resize(MatrixSize InputSize)
{
	this->Resize(InputSize.RowCount, InputSize.ColCount);
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::FastResize(int_max InputRowCount, int_max InputColCount)
{
    if (InputRowCount < 0 || InputColCount < 0)
    {
        MDK_Error("Invalid input @ DenseMatrix::FastResize(int_max InputRowCount, int_max InputColCount)")
        return;    
    }

	if (this->IsPureEmpty() == true)
	{
		this->Resize(0, 0);
	}

    auto Size = this->GetSize();

    if (Size.RowCount == InputRowCount && Size.ColCount == InputColCount)
    {
        return;
    }

    if (this->IsSizeFixed() == true)
    {
        MDK_Error("Size can not change @ DenseMatrix::FastResize(int_max InputRowCount, int_max InputColCount)")
        return;
    }

    if (InputRowCount == 0 || InputColCount == 0)// FastResize(1, 0) or FastResize(0, 1) is allowed
    {
        m_MatrixData->RowCount = 0;
        m_MatrixData->ColCount = 0;
        m_MatrixData->StdVector.clear(); // change size, but not release memory (shrink_to_fit)
        m_MatrixData->ElementPointer = nullptr;
        m_MatrixData->IsSizeFixed = false;        
        return;
    }

try
{
    int_max InputElementNumber = InputRowCount * InputColCount;

    if (InputElementNumber != Size.RowCount *  Size.ColCount)
    {
        if (InputElementNumber > int_max(m_MatrixData->StdVector.capacity()))
        {
            m_MatrixData->StdVector.clear();
        }

        m_MatrixData->StdVector.resize(InputElementNumber);
        m_MatrixData->ElementPointer = m_MatrixData->StdVector.data();        
    }
   
    m_MatrixData->RowCount = InputRowCount;
    m_MatrixData->ColCount = InputColCount;
}
catch (...)
{
    MDK_Error("Out of Memory @ DenseMatrix::FastResize(int_max InputRowCount, int_max InputColCount)")
    //this->Clear();
}
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::FastResize(MatrixSize InputSize)
{
	this->FastResize(InputSize.RowCount, InputSize.ColCount);
}


template<typename ElementType>
inline 
void DenseMatrix<ElementType>::Resize(int_max InputElementNumber) // try to keep the old data, can not use this to resize a m x n matrix (m>1 or n>1)
{
    if (InputElementNumber < 0)
    {
        MDK_Error("Invalid input @ DenseMatrix::Resize(int_max InputElementNumber)")
        return;
    }

	if (this->IsPureEmpty() == true)
	{
		this->Resize(0, 0);
	}

    auto Size = this->GetSize();

    if (Size.RowCount > 1 && Size.ColCount > 1)
    {
        MDK_Error("Self is not empty and not a vector @ DenseMatrix::Resize(int_max InputElementNumber)")
        return;
    }

    if (Size.RowCount*Size.ColCount == InputElementNumber)
    {
        return;
    }

    if (this->IsSizeFixed() == true)
    {
        MDK_Error("Size can not change @ DenseMatrix::Resize(int_max InputElementNumber)")
        return;
    }

    if (InputElementNumber == 0)
    {
        this->Clear();
        return;
    }

try
{
    m_MatrixData->StdVector.resize(InputElementNumber);
    m_MatrixData->ElementPointer = m_MatrixData->StdVector.data();

    if (Size.RowCount == 0) // empty -> row vector
    {
        m_MatrixData->RowCount = 1;
        m_MatrixData->ColCount = InputElementNumber;
    }
    else if (Size.RowCount == 1) // row vector -> row vector
    {
        m_MatrixData->RowCount = 1;
        m_MatrixData->ColCount = InputElementNumber;
    }
    else if (Size.ColCount == 1) // col vector -> col vector
    {
        m_MatrixData->RowCount = InputElementNumber;
        m_MatrixData->ColCount = 1;
    }
}
catch (...)
{
    MDK_Error("Out of Memory @ DenseMatrix::Resize(int_max InputElementNumber)")
    //this->Clear();
}
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::FastResize(int_max InputElementNumber) // do not care about old data, can not use this to resize a m x n matrix (m>1 or n>1)
{
    if (InputElementNumber < 0)
    {
        MDK_Error("Invalid input @ DenseMatrix::FastResize(int_max InputElementNumber)")
        return;
    }

	if (InputElementNumber == 0)
	{
		this->Resize(0, 0);
		return;
	}

	if (this->IsPureEmpty() == true)
	{
		this->Resize(0, 0);
	}

    auto Size = this->GetSize();

    if (Size.RowCount == 0) // empty -> row vector
    {
        this->FastResize(1, InputElementNumber);
    }
    else if (Size.RowCount == 1) // row vector -> row vector
    {
        this->FastResize(1, InputElementNumber);
    }
    else if (Size.ColCount == 1) // col vector -> col vector
    {
        this->FastResize(InputElementNumber, 1);
    }
    else
    {
        MDK_Error("Self is not empty or not a vector @ DenseMatrix::FastResize(int_max InputElementNumber)")
    }    
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::SetCapacity(int_max InputRowCount, int_max InputColCount)
{
	this->SetCapacity(InputRowCount*InputColCount);
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::SetCapacity(MatrixSize InputSize)
{
	this->SetCapacity(InputSize.RowCount*InputSize.ColCount);
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::SetCapacity(int_max InputElementNumber)
{
	if (this->IsPureEmpty() == true)
	{
		this->Resize(0, 0);
	}

	auto Self_ElementNumber = this->GetElementCount();

	if (this->IsSizeFixed() == true)
	{
		if (Self_ElementNumber != InputElementNumber)
		{
			MDK_Error("Size can not change @ DenseMatrix::SetCapacity(...)")
		}
		return;
	}

try
{
    if (InputElementNumber > Self_ElementNumber)
    {
        m_MatrixData->CopyDataToInternalArrayIfNecessary();
        m_MatrixData->StdVector.reserve(InputElementNumber);
        m_MatrixData->ElementPointer = m_MatrixData->StdVector.data();        
    }
}
catch (...)
{
    MDK_Error("Out of Memory @ DenseMatrix::SetCapacity(int_max InputElementNumber)")
}
}


template<typename ElementType>
inline 
int_max DenseMatrix<ElementType>::GetCapacity() const
{
	if (!m_MatrixData)
	{
		return 0;
	}

	return m_MatrixData->StdVector.capacity();
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::ReleaseUnusedCapacity()
{
    if (!m_MatrixData)
    {
        return;
    }

    if (m_MatrixData->StdVector.data() != nullptr)
    {
        m_MatrixData->StdVector.shrink_to_fit();
        m_MatrixData->ElementPointer = m_MatrixData->StdVector.data(); 
    }
}


template<typename ElementType>
inline 
void DenseMatrix<ElementType>::FixSize()
{
	if (this->IsPureEmpty() == true)
	{
		this->Resize(0, 0);
	}

    m_MatrixData->IsSizeFixed = true;
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::IsSizeFixed() const
{
    if (!m_MatrixData)
    {
        return false;
    }

    return m_MatrixData->IsSizeFixed;
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::IsEmpty() const
{
    if (m_MatrixData)
    {
        return (m_MatrixData->RowCount <= 0);
    }

    return true;
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::IsPureEmpty() const
{
    return (!m_MatrixData);
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::IsShared() const
{
    return (m_MatrixData.use_count() > 1);
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::IsSharedWith(const DenseMatrix& InputMatrix) const
{
	if (this->GetElementPointer() == nullptr)
	{
		return false;
	}
	return (this->GetElementPointer() == InputMatrix.GetElementPointer());
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::IsDataInInternalArray() const
{
	if (!m_MatrixData)
	{
		return false;
	}
	else
	{
		return (m_MatrixData->ElementPointer == m_MatrixData->StdVector.data());
	}
}


template<typename ElementType>
inline 
std::vector<ElementType>& DenseMatrix<ElementType>::InternalArray()
{
	return m_MatrixData->StdVector;
}


template<typename ElementType>
inline 
const std::vector<ElementType>& DenseMatrix<ElementType>::InternalArray() const
{
	return m_MatrixData->StdVector;
}


template<typename ElementType>
inline
MatrixSize DenseMatrix<ElementType>::GetSize() const
{
    MatrixSize Size;

    if (!m_MatrixData)
    {
        Size.RowCount = 0;
        Size.ColCount = 0;
    }
    else
    {
        Size.RowCount = m_MatrixData->RowCount;
        Size.ColCount = m_MatrixData->ColCount;
    }

    return Size;
}


template<typename ElementType>
inline
int_max DenseMatrix<ElementType>::GetElementCount() const
{
    if (!m_MatrixData)
    {
        return 0;
    }
    else
    {
        return m_MatrixData->RowCount * m_MatrixData->ColCount;
    }
}


template<typename ElementType>
inline 
int_max DenseMatrix<ElementType>::GetColCount() const
{
    if (!m_MatrixData)
    {
        return 0;
    }
    else
    {
        return m_MatrixData->ColCount;
    }
}


template<typename ElementType>
inline 
int_max DenseMatrix<ElementType>::GetRowCount() const
{
    if (!m_MatrixData)
    {
        return 0;
    }
    else
    {
        return m_MatrixData->RowCount;
    }
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::IsScalar() const
{
	if (!m_MatrixData)
	{
		return false;
	}
	else
	{
		return (m_MatrixData->RowCount == 1 && m_MatrixData->ColCount == 1);
	}
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::IsVector() const
{
    if (!m_MatrixData)
    {
        return false;
    }
    else
    {
        return (m_MatrixData->RowCount == 1 || m_MatrixData->ColCount == 1);
    }
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::IsRowVector() const
{
    if (!m_MatrixData)
    {
        return false;
    }
    else
    {
        return (m_MatrixData->RowCount == 1);
    }
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::IsColVector() const
{
    if (!m_MatrixData)
    {
        return false;
    }
    else
    {
        return (m_MatrixData->ColCount == 1);
    }
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::IsSquare() const
{
    if (!m_MatrixData)
    {
        return false;
    }
    else
    {
        return (m_MatrixData->RowCount > 0 && m_MatrixData->RowCount == m_MatrixData->ColCount);
    }
}


template<typename ElementType>
inline 
bool DenseMatrix<ElementType>::IsIdentityMatrix(ElementType Threshold) const
{
    if (this->IsSquare() == false)
    {
        return false;
    }

	auto SelfSize = this->GetSize();
	auto RawPointer = this->GetElementPointer();
	int_max LinearIndex = -1;
	for (int_max j = 0; j < SelfSize.ColCount; ++j)
	{
		for (int_max i = 0; j < SelfSize.RowCount; ++i)
		{
			LinearIndex += 1;
			if (i == j)
			{//compare DiagonalElement to 1
				auto Element = RawPointer[LinearIndex];
				if (std::abs(Element - ElementType(1)) > Threshold) // must use ">" becuause ElementType may be int and Threshold is 0
				{
					return false;
				}
			}
			else
			{//compare NonDiagonalElement to 0
				auto Element = RawPointer[LinearIndex];
				if (std::abs(Element) > Threshold) // must use ">" becuause ElementType may be int and Threshold is 0
				{
					return false;
				}
			}
		}
	}

    return true;
}


template<typename ElementType>
inline 
void DenseMatrix<ElementType>::SetErrorElement(const ElementType& Element)
{
	if (!m_MatrixData)
	{	
		this->Resize(0, 0);
	}
	m_MatrixData->ErrorElement = Element;
}


template<typename ElementType>
inline
ElementType DenseMatrix<ElementType>::GetErrorElement()  const
{
    if (!m_MatrixData)
    {
		return GetNaNElement<ElementType>();
    }

    return m_MatrixData->ErrorElement;
}


template<typename ElementType>
inline
ElementType* DenseMatrix<ElementType>::GetElementPointer()
{
    if (!m_MatrixData)
    {
        return nullptr;
    }
    else
    {
        return m_MatrixData->ElementPointer;
    }
}


template<typename ElementType>
inline
const ElementType* DenseMatrix<ElementType>::GetElementPointer() const
{
    if (!m_MatrixData)
    {
        return nullptr;
    }
    else
    {
        return m_MatrixData->ElementPointer;
    }
}


template<typename ElementType>
inline
ElementType* DenseMatrix<ElementType>::GetPointer()
{
    return this->GetElementPointer();
}


template<typename ElementType>
inline
const ElementType* DenseMatrix<ElementType>::GetPointer() const
{
    return this->GetElementPointer();
}


template<typename ElementType>
inline ElementType* DenseMatrix<ElementType>::begin()
{
    return this->GetElementPointer();
}


template<typename ElementType>
inline const ElementType* DenseMatrix<ElementType>::begin() const
{
    return this->GetElementPointer();
}


template<typename ElementType>
inline ElementType* DenseMatrix<ElementType>::end()
{
    auto endPtr = this->GetElementPointer();
    if (endPtr != nullptr)
    {
        endPtr += this->GetElementCount();
    }
    return endPtr;
}


template<typename ElementType>
inline const ElementType* DenseMatrix<ElementType>::end() const
{
    auto endPtr = this->GetElementPointer();
    if (endPtr != nullptr)
    {
        endPtr += this->GetElementCount();
    }
    return endPtr;
}


template<typename ElementType>
inline 
ElementType* DenseMatrix<ElementType>::GetElementPointerOfCol(int_max ColIndex) //  the position of the first element in Col # ColIndex
{
    if (!m_MatrixData)
    {
        return nullptr;
    }

    auto Size = this->GetSize();

    if (ColIndex < 0 || ColIndex >= Size.ColCount)
    {
        MDK_Error("Invalid ColIndex @ DenseMatrix::GetElementPointerOfCol(...)")
        return nullptr;
    }

    auto PointerOfCol = this->GetElementPointer() + ColIndex*Size.RowCount;

    return PointerOfCol;
}


template<typename ElementType>
inline
const ElementType* DenseMatrix<ElementType>::GetElementPointerOfCol(int_max ColIndex) const
{
    if (!m_MatrixData)
    {
        return nullptr;
    }

    auto Size = this->GetSize();

    if (ColIndex < 0 || ColIndex >= Size.ColCount)
    {
        MDK_Error("Invalid ColIndex @ DenseMatrix::GetElementPointerOfCol(...) const")
        return nullptr;
    }

    auto PointerOfCol = this->GetElementPointer() + ColIndex*Size.RowCount;

    return PointerOfCol;
}


template<typename ElementType>
inline
ElementType* DenseMatrix<ElementType>::GetPointerOfCol(int_max ColIndex) //  the position of the first element in Col # ColIndex
{
    return this->GetElementPointerOfCol(ColIndex);
}


template<typename ElementType>
inline
const ElementType* DenseMatrix<ElementType>::GetPointerOfCol(int_max ColIndex) const
{
    return this->GetElementPointerOfCol(ColIndex);
}

//----------- Get/Set Matrix(LinearIndex) -----------------------------------//

// operator[] (): no bound check in release mode

template<typename ElementType>
inline
ElementType& DenseMatrix<ElementType>::operator[](int_max LinearIndex)
{
#if defined(MDK_DEBUG_DenseMatrix_Operator_CheckBound)

    if (LinearIndex >= this->GetElementCount()|| LinearIndex < 0)
    {
        MDK_Error("Invalid Input @ DenseMatrix::operator[](i)")

        return m_MatrixData->ErrorElement;
    }

#endif //MDK_DEBUG_DenseMatrix_Operator_CheckBound

    return (*m_MatrixData)[LinearIndex];
}


template<typename ElementType>
inline
const ElementType& DenseMatrix<ElementType>::operator[](int_max LinearIndex) const
{
#if defined(MDK_DEBUG_DenseMatrix_Operator_CheckBound)

    if (LinearIndex >= this->GetElementCount() || LinearIndex < 0)
    {
        MDK_Error("Invalid Input @ DenseMatrix::operator[](i) const")

        return m_MatrixData->ErrorElement;
    }

#endif //MDK_DEBUG_DenseMatrix_Operator_CheckBound

	return (*m_MatrixData)[LinearIndex];
}


template<typename ElementType>
inline
ElementType& DenseMatrix<ElementType>::operator()(int_max LinearIndex)
{
#if defined(MDK_DEBUG_DenseMatrix_Operator_CheckBound)

    if (LinearIndex >= this->GetElementCount() || LinearIndex < 0)
    {
        MDK_Error("Invalid Input @ DenseMatrix::operator()(i)")

        return m_MatrixData->ErrorElement;
    }

#endif //MDK_DEBUG_DenseMatrix_Operator_CheckBound

	return (*m_MatrixData)[LinearIndex];
}


template<typename ElementType>
inline
const ElementType& DenseMatrix<ElementType>::operator()(int_max LinearIndex) const
{
#if defined(MDK_DEBUG_DenseMatrix_Operator_CheckBound)

    if (LinearIndex >= this->GetElementCount() || LinearIndex < 0)
    {
        MDK_Error("Invalid Input @ DenseMatrix::operator()(i) const")

        return m_MatrixData->ErrorElement;
    }

#endif //MDK_DEBUG_DenseMatrix_Operator_CheckBound

	return (*m_MatrixData)[LinearIndex];
}

// at(): bound check

template<typename ElementType>
inline
ElementType& DenseMatrix<ElementType>::at(int_max LinearIndex)
{
    if (LinearIndex >= this->GetElementCount() || LinearIndex < 0)
	{
		MDK_Error("Invalid Input @ DenseMatrix::at(i)")
        
        return m_MatrixData->ErrorElement;
	}

	return (*m_MatrixData)[LinearIndex];
}


template<typename ElementType>
inline
const ElementType& DenseMatrix<ElementType>::at(int_max LinearIndex) const
{
    if (LinearIndex >= this->GetElementCount() || LinearIndex < 0)
	{
		MDK_Error("Invalid Input @ DenseMatrix::at(i) const")
        
        return m_MatrixData->ErrorElement;
	}

	return (*m_MatrixData)[LinearIndex];
}

//----------- Get/Set Matrix(i,j)  ---------------------------------------------//

// operator(): no bound check in release mode

template<typename ElementType>
inline
ElementType& DenseMatrix<ElementType>::operator()(int_max RowIndex, int_max ColIndex)
{
#if defined(MDK_DEBUG_DenseMatrix_Operator_CheckBound)

    auto SelfSize = this->GetSize();

    if (RowIndex >= SelfSize.RowCount || RowIndex < 0 || ColIndex >= SelfSize.ColCount || ColIndex < 0)
    {
        MDK_Error("Invalid Input @ DenseMatrix::operator()(i,j)")

        return m_MatrixData->ErrorElement;
    }

#endif //MDK_DEBUG_DenseMatrix_Operator_CheckBound
    
    return (*m_MatrixData)(RowIndex, ColIndex);
}


template<typename ElementType>
inline
const ElementType& DenseMatrix<ElementType>::operator()(int_max RowIndex, int_max ColIndex) const
{
#if defined(MDK_DEBUG_DenseMatrix_Operator_CheckBound)

    auto SelfSize = this->GetSize();

    if (RowIndex >= SelfSize.RowCount || RowIndex < 0 || ColIndex >= SelfSize.ColCount || ColIndex < 0)
    {
        MDK_Error("Invalid Input @ DenseMatrix::operator()(i,j) const")

        return m_MatrixData->ErrorElement;
    }

#endif //MDK_DEBUG_DenseMatrix_Operator_CheckBound

    return (*m_MatrixData)(RowIndex, ColIndex);
}


template<typename ElementType>
inline
ElementType& DenseMatrix<ElementType>::at(int_max RowIndex, int_max ColIndex)
{
    auto SelfSize = this->GetSize();

    if (RowIndex >= SelfSize.RowCount || RowIndex < 0 || ColIndex >= SelfSize.ColCount || ColIndex < 0)
    {
        MDK_Error("Invalid Input @ DenseMatrix::at(i,j)")
        
        return m_MatrixData->ErrorElement;
    }
    
    return (*m_MatrixData)(RowIndex, ColIndex);
}


template<typename ElementType>
inline
const ElementType& DenseMatrix<ElementType>::at(int_max RowIndex, int_max ColIndex) const
{
    auto SelfSize = this->GetSize();

    if (RowIndex >= SelfSize.RowCount || RowIndex < 0 || ColIndex >= SelfSize.ColCount || ColIndex < 0)
    {
        MDK_Error("Invalid Input @ DenseMatrix::at(i,j) const")
        
        return m_MatrixData->ErrorElement;
    }

    return (*m_MatrixData)(RowIndex, ColIndex);
}

//----------- Get/Set a set of elements by Matrix({}), Matrix.at({}) -------------------------------------//

// operator(): no bound check in release mode
//
// note: operator[] is for single element access only, operator[{}] is not defined


template<typename ElementType>
inline
DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::operator()(const std::initializer_list<int_max>& LinearIndexList)
{
#if defined(MDK_DEBUG_DenseMatrix_Operator_CheckBound)

    if (LinearIndexList.size() <= 0)
    {
        MDK_Error("LinearIndexList is empty @ DenseMatrix::operator()(std::initializer_list {LinearIndexList})")
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto Self_ElementNumber = this->GetElementCount();

    for (auto it = LinearIndexList.begin(); it != LinearIndexList.end(); ++it)
    {
        if (*it >= Self_ElementNumber || *it < 0)
        {
            MDK_Error("Invalid LinearIndexList @ DenseMatrix::operator()(std::initializer_list {LinearIndexList})")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

#endif //MDK_DEBUG_DenseMatrix_Operator_CheckBound

    DenseShadowMatrix<ElementType> tempShadowMatrix(*this, LinearIndexList);

    return tempShadowMatrix;
}


template<typename ElementType>
inline
const DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::operator()(const std::initializer_list<int_max>& LinearIndexList) const
{
#if defined(MDK_DEBUG_DenseMatrix_Operator_CheckBound)

    if (LinearIndexList.size() <= 0)
    {
        MDK_Error("LinearIndexList is empty @ DenseMatrix::operator()(std::initializer_list {LinearIndexList}) const")
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto Self_ElementNumber = this->GetElementCount();

    for (auto it = LinearIndexList.begin(); it != LinearIndexList.end(); ++it)
    {
        if (*it >= Self_ElementNumber || *it < 0)
        {
            MDK_Error("Invalid LinearIndexList @ DenseMatrix::operator()(std::initializer_list {LinearIndexList}) const")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

#endif //MDK_DEBUG_DenseMatrix_Operator_CheckBound

    DenseShadowMatrix<ElementType> tempShadowMatrix(*this, LinearIndexList);

    return tempShadowMatrix;
}


template<typename ElementType>
inline
DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::operator()(DenseVector<int_max> LinearIndexList)
{
#if defined(MDK_DEBUG_DenseMatrix_Operator_CheckBound)

    if (LinearIndexList.GetLength() <= 0)
    {
        MDK_Error("LinearIndexList is empty @ DenseMatrix::operator()(DenseVector {LinearIndexList})")
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto Self_ElementNumber = this->GetElementCount();

    for (auto it = LinearIndexList.begin(); it != LinearIndexList.end(); ++it)
    {
        if (*it >= Self_ElementNumber || *it < 0)
        {
            MDK_Error("Invalid LinearIndexList @ DenseMatrix::operator()(DenseVector {LinearIndexList})")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

#endif //MDK_DEBUG_DenseMatrix_Operator_CheckBound

    DenseShadowMatrix<ElementType> tempShadowMatrix(*this, std::move(LinearIndexList.StdVector()));

    return tempShadowMatrix;
}


template<typename ElementType>
inline
const DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::operator()(DenseVector<int_max> LinearIndexList) const
{
#if defined(MDK_DEBUG_DenseMatrix_Operator_CheckBound)

    if (LinearIndexList.GetLength() <= 0)
    {
        MDK_Error("LinearIndexList is empty @ DenseMatrix::operator()(DenseVector {LinearIndexList}) const")
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto Self_ElementNumber = this->GetElementCount();

    for (auto it = LinearIndexList.begin(); it != LinearIndexList.end(); ++it)
    {
        if (*it >= Self_ElementNumber || *it < 0)
        {
            MDK_Error("Invalid LinearIndexList @ DenseMatrix::operator()(DenseVector {LinearIndexList}) const")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

#endif //MDK_DEBUG_DenseMatrix_Operator_CheckBound

    DenseShadowMatrix<ElementType> tempShadowMatrix(*this, std::move(LinearIndexList.StdVector()));

    return tempShadowMatrix;
}


template<typename ElementType>
template<int_max TemplateVectorLength>
inline
DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::operator()(const DenseVector<int_max, TemplateVectorLength>& LinearIndexList)
{
    return (*this)(LinearIndexList.CreateDenseVectorWithVariableLength());
}


template<typename ElementType>
template<int_max TemplateVectorLength>
inline
const DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::operator()(const DenseVector<int_max, TemplateVectorLength>& LinearIndexList) const
{
    return (*this)(LinearIndexList.CreateDenseVectorWithVariableLength());
}


template<typename ElementType>
inline
DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::operator()(const DenseMatrix<int_max>& LinearIndexList)
{
#if defined(MDK_DEBUG_DenseMatrix_Operator_CheckBound)

    if (LinearIndexList.IsVector() == false)
    {
        MDK_Error("LinearIndexList is not vector @ DenseMatrix::operator()(const DenseMatrix<int_max>& {LinearIndexList})")
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto Self_ElementNumber = this->GetElementCount();

    for (auto it = LinearIndexList.begin(); it != LinearIndexList.end(); ++it)
    {
        if (*it >= Self_ElementNumber || *it < 0)
        {
            MDK_Error("Invalid LinearIndexList @ DenseMatrix::operator()(const DenseMatrix<int_max>& {LinearIndexList})")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

#endif //MDK_DEBUG_DenseMatrix_Operator_CheckBound

    DenseShadowMatrix<ElementType> tempShadowMatrix(*this, LinearIndexList);
    return tempShadowMatrix;
}


template<typename ElementType>
inline
const DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::operator()(const DenseMatrix<int_max>& LinearIndexList) const
{
#if defined(MDK_DEBUG_DenseMatrix_Operator_CheckBound)

    if (LinearIndexList.IsVector() == false)
    {
        MDK_Error("LinearIndexList is not vector @ DenseMatrix::operator()(const DenseMatrix<int_max>& {LinearIndexList}) const")
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto Self_ElementNumber = this->GetElementCount();

    for (auto it = LinearIndexList.begin(); it != LinearIndexList.end(); ++it)
    {
        if (*it >= Self_ElementNumber || *it < 0)
        {
            MDK_Error("Invalid LinearIndexList @ DenseMatrix::operator()(const DenseMatrix<int_max>& {LinearIndexList}) const")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

#endif //MDK_DEBUG_DenseMatrix_Operator_CheckBound

    DenseShadowMatrix<ElementType> tempShadowMatrix(*this, LinearIndexList);
    return tempShadowMatrix;
}



template<typename ElementType>
inline
DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::operator()(const MDK_Symbol_ALL& ALL_Symbol)
{
    DenseShadowMatrix<ElementType> tempShadowMatrix(*this, ALL_Symbol);
    return tempShadowMatrix;
}


template<typename ElementType>
inline
const DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::operator()(const MDK_Symbol_ALL& ALL_Symbol) const
{
    DenseShadowMatrix<ElementType> tempShadowMatrix(*this, ALL_Symbol);
    return tempShadowMatrix;
}


template<typename ElementType>
inline
DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::at(const std::initializer_list<int_max>& LinearIndexList)
{
    if (LinearIndexList.size() <= 0)
    {
        MDK_Error("LinearIndexList is empty @ DenseMatrix::at(std::initializer_list {LinearIndexList})")
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto Self_ElementNumber = this->GetElementCount();

    for (auto it = LinearIndexList.begin(); it != LinearIndexList.end(); ++it)
    {
        if (*it >= Self_ElementNumber || *it < 0)
        {
            MDK_Error("Invalid LinearIndexList @ DenseMatrix::at(std::initializer_list {LinearIndexList})")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    DenseShadowMatrix<ElementType> tempShadowMatrix(*this, LinearIndexList);
    return tempShadowMatrix;
}


template<typename ElementType>
inline
const DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::at(const std::initializer_list<int_max>& LinearIndexList) const
{
    if (LinearIndexList.size() <= 0)
    {
        MDK_Error("LinearIndexList is empty @ DenseMatrix::at(std::initializer_list {LinearIndexList}) const")
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto Self_ElementNumber = this->GetElementCount();

    for (auto it = LinearIndexList.begin(); it != LinearIndexList.end(); ++it)
    {
        if (*it >= Self_ElementNumber || *it < 0)
        {
            MDK_Error("Invalid LinearIndexList @ DenseMatrix::at(std::initializer_list {LinearIndexList}) const")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    DenseShadowMatrix<ElementType> tempShadowMatrix(*this, LinearIndexList);
    return tempShadowMatrix;
}


template<typename ElementType>
inline
DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::at(DenseVector<int_max> LinearIndexList)
{
    if (LinearIndexList.GetLength() <= 0)
    {
        MDK_Error("LinearIndexList is empty @ DenseMatrix::at(DenseVector {LinearIndexList})")
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto Self_ElementNumber = this->GetElementCount();

    for (auto it = LinearIndexList.begin(); it != LinearIndexList.end(); ++it)
    {
        if (*it >= Self_ElementNumber || *it < 0)
        {
            MDK_Error("Invalid LinearIndexList @ DenseMatrix::at(DenseVector {LinearIndexList})")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    DenseShadowMatrix<ElementType> tempShadowMatrix(*this, std::move(LinearIndexList.StdVector()));
    return tempShadowMatrix;
}


template<typename ElementType>
inline
const DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::at(DenseVector<int_max> LinearIndexList) const
{
    if (LinearIndexList.GetLength() <= 0)
    {
        MDK_Error("LinearIndexList is empty @ DenseMatrix::at(DenseVector {LinearIndexList} const)")
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto Self_ElementNumber = this->GetElementCount();

    for (auto it = LinearIndexList.begin(); it != LinearIndexList.end(); ++it)
    {
        if (*it >= Self_ElementNumber || *it < 0)
        {
            MDK_Error("Invalid LinearIndexList @ DenseMatrix::at(DenseVector {LinearIndexList} const)")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    DenseShadowMatrix<ElementType> tempShadowMatrix(*this, std::move(LinearIndexList.StdVector()));
    return tempShadowMatrix;
}


template<typename ElementType>
template<int_max TemplateVectorLength>
inline
DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::at(const DenseVector<int_max, TemplateVectorLength>& LinearIndexList)
{
    return this->at(LinearIndexList.CreateDenseVectorWithVariableLength());
}


template<typename ElementType>
template<int_max TemplateVectorLength>
inline
const DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::at(const DenseVector<int_max, TemplateVectorLength>& LinearIndexList) const
{
    return this->at(LinearIndexList.CreateDenseVectorWithVariableLength());
}


template<typename ElementType>
inline
DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::at(const DenseMatrix<int_max>& LinearIndexList)
{
    if (LinearIndexList.IsVector() == false)
    {
        MDK_Error("Invalid input @ DenseMatrix::at(DenseMatrix: LinearIndexList)")

        DenseShadowMatrix<ElementType> EmptyShadowMatrix;

        return EmptyShadowMatrix;
    }

    auto Self_ElementNumber = this->GetElementCount();

    if (Self_ElementNumber <= 0)
    {
        MDK_Error("LinearIndexList is empty @ DenseMatrix::at(DenseMatrix {LinearIndexList})")
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    for (auto it = LinearIndexList.begin(); it != LinearIndexList.end(); ++it)
    {
        if (*it >= Self_ElementNumber || *it < 0)
        {
            MDK_Error("Invalid LinearIndexList @ DenseMatrix::at(DenseMatrix {LinearIndexList})")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    DenseShadowMatrix<ElementType> tempShadowMatrix(*this, LinearIndexList);
    return tempShadowMatrix;
}


template<typename ElementType>
inline
const DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::at(const DenseMatrix<int_max>& LinearIndexList) const
{
    if (LinearIndexList.IsVector() == false)
    {
        MDK_Error("Invalid input @ DenseMatrix::at(DenseMatrix: LinearIndexList) const")
        DenseShadowMatrix<ElementType> EmptyShadowMatrix;
        return EmptyShadowMatrix;
    }

    auto Self_ElementNumber = this->GetElementCount();

    if (Self_ElementNumber <= 0)
    {
        MDK_Error("LinearIndexList is empty @ DenseMatrix::at(DenseMatrix {LinearIndexList}) const")
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    for (auto it = LinearIndexList.begin(); it != LinearIndexList.end(); ++it)
    {
        if (*it >= Self_ElementNumber || *it < 0)
        {
            MDK_Error("Invalid LinearIndexList @ DenseMatrix::at(DenseMatrix {LinearIndexList}) const")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    DenseShadowMatrix<ElementType> tempShadowMatrix(*this, LinearIndexList);
    return tempShadowMatrix;
}


template<typename ElementType>
inline
DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::at(const MDK_Symbol_ALL& ALL_Symbol)
{
    DenseShadowMatrix<ElementType> tempShadowMatrix(*this, ALL_Symbol);
    return tempShadowMatrix;
}


template<typename ElementType>
inline
const DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::at(const MDK_Symbol_ALL& ALL_Symbol) const
{
    DenseShadowMatrix<ElementType> tempShadowMatrix(*this, ALL_Symbol);
    return tempShadowMatrix;
}

//---------- Get/Set SubMatrix by Matrix({...},{...}), Matrix.at({...}, {...}), Matrix.Col({...}), Matrix.Row({...}), Matrix.Diagnal() -------//

// operator(): no bound check

template<typename ElementType>
inline
DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::operator()(const std::initializer_list<int_max>& RowIndexList, const std::initializer_list<int_max>& ColIndexList)
{
#if defined(MDK_DEBUG_DenseMatrix_Operator_CheckBound)

    if (RowIndexList.size() <= 0 || ColIndexList.size() <= 0)
    {
        MDK_Error("RowIndexList or  ColIndexList is empty @ DenseMatrix::operator()(std::initializer_list {RowIndexList}, {ColIndexList})")
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = RowIndexList.begin(); it != RowIndexList.end(); ++it)
    {
        if (*it >= SelfSize.RowCount || *it < 0)
        {
            MDK_Error("Invalid RowIndexList @ DenseMatrix::operator()(std::initializer_list {RowIndexList}, {ColIndexList})")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= SelfSize.ColCount || *it < 0)
        {
            MDK_Error("Invalid ColIndexList @ DenseMatrix::operator()(std::initializer_list {RowIndexList}, {ColIndexList})")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

#endif //MDK_DEBUG_DenseMatrix_Operator_CheckBound

    DenseShadowMatrix<ElementType> tempShadowMatrix(*this, RowIndexList, ColIndexList);
    return tempShadowMatrix;
}


template<typename ElementType>
inline
const DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::operator()(const std::initializer_list<int_max>& RowIndexList, const std::initializer_list<int_max>& ColIndexList) const
{
#if defined(MDK_DEBUG_DenseMatrix_Operator_CheckBound)

    if (RowIndexList.size() <= 0 || ColIndexList.size() <= 0)
    {
        MDK_Error("RowIndexList or  ColIndexList is empty @ DenseMatrix::operator()(std::initializer_list {RowIndexList}, {ColIndexList}) const")
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = RowIndexList.begin(); it != RowIndexList.end(); ++it)
    {
        if (*it >= SelfSize.RowCount || *it < 0)
        {
            MDK_Error("Invalid RowIndexList @ DenseMatrix::operator()(std::initializer_list {RowIndexList}, {ColIndexList}) const")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= SelfSize.ColCount || *it < 0)
        {
            MDK_Error("Invalid ColIndexList @ DenseMatrix::operator()(std::initializer_list {RowIndexList}, {ColIndexList}) const")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

#endif //MDK_DEBUG_DenseMatrix_Operator_CheckBound

    DenseShadowMatrix<ElementType> tempShadowMatrix(*this, RowIndexList, ColIndexList);
    return tempShadowMatrix;
}


template<typename ElementType>
inline
DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::operator()(const std::initializer_list<int_max>& RowIndexList, const MDK_Symbol_ALL& ALL_Symbol)
{
#if defined(MDK_DEBUG_DenseMatrix_Operator_CheckBound)

    if (RowIndexList.size() <= 0)
    {
        MDK_Error("RowIndexList is empty @ DenseMatrix::operator()(std::initializer_list {RowIndexList}, ALL)")
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = RowIndexList.begin(); it != RowIndexList.end(); ++it)
    {
        if (*it >= SelfSize.RowCount || *it < 0)
        {
            MDK_Error("Invalid RowIndexList @ DenseMatrix::operator()(std::initializer_list {RowIndexList}, ALL)")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

#endif //MDK_DEBUG_DenseMatrix_Operator_CheckBound

    DenseShadowMatrix<ElementType> tempShadowMatrix(*this, RowIndexList, ALL_Symbol);
    return tempShadowMatrix;
}


template<typename ElementType>
inline
const DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::operator()(const std::initializer_list<int_max>& RowIndexList, const MDK_Symbol_ALL& ALL_Symbol) const
{
#if defined(MDK_DEBUG_DenseMatrix_Operator_CheckBound)

    if (RowIndexList.size() <= 0)
    {
        MDK_Error("RowIndexList is empty @ DenseMatrix::operator()(std::initializer_list {RowIndexList}, ALL) const")
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = RowIndexList.begin(); it != RowIndexList.end(); ++it)
    {
        if (*it >= SelfSize.RowCount || *it < 0)
        {
            MDK_Error("Invalid RowIndexList @ DenseMatrix::operator()(std::initializer_list {RowIndexList}, ALL) const")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

#endif //MDK_DEBUG_DenseMatrix_Operator_CheckBound

    DenseShadowMatrix<ElementType> tempShadowMatrix(*this, RowIndexList, ALL_Symbol);
    return tempShadowMatrix;
}


template<typename ElementType>
inline
DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::operator()(const MDK_Symbol_ALL& ALL_Symbol, const std::initializer_list<int_max>& ColIndexList)
{
#if defined(MDK_DEBUG_DenseMatrix_Operator_CheckBound)

    if (ColIndexList.size() <= 0)
    {
        MDK_Error("ColIndexList is empty @ DenseMatrix::operator()(ALL, std::initializer_list {ColIndexList})")
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= SelfSize.ColCount || *it < 0)
        {
            MDK_Error("Invalid ColIndexList @ DenseMatrix::operator()(ALL, std::initializer_list {ColIndexList})")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

#endif //MDK_DEBUG_DenseMatrix_Operator_CheckBound

    DenseShadowMatrix<ElementType> tempShadowMatrix(*this, ALL_Symbol, ColIndexList);
    return tempShadowMatrix;
}


template<typename ElementType>
inline
const DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::operator()(const MDK_Symbol_ALL& ALL_Symbol, const std::initializer_list<int_max>& ColIndexList) const
{
#if defined(MDK_DEBUG_DenseMatrix_Operator_CheckBound)

    if (ColIndexList.size() <= 0)
    {
        MDK_Error("ColIndexList is empty @ DenseMatrix::operator()(ALL, std::initializer_list {ColIndexList}) const")
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= SelfSize.ColCount || *it < 0)
        {
            MDK_Error("Invalid ColIndexList @ DenseMatrix::operator()(ALL, std::initializer_list {ColIndexList}) const")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

#endif //MDK_DEBUG_DenseMatrix_Operator_CheckBound

    DenseShadowMatrix<ElementType> tempShadowMatrix(*this, ALL_Symbol, ColIndexList);
    return tempShadowMatrix;
}


template<typename ElementType>
inline
DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::operator()(DenseVector<int_max> RowIndexList, DenseVector<int_max> ColIndexList)
{
#if defined(MDK_DEBUG_DenseMatrix_Operator_CheckBound)

    if (RowIndexList.GetLength() <= 0 || ColIndexList.GetLength() <= 0)
    {
        MDK_Error("RowIndexList or  ColIndexList is empty @ DenseMatrix::operator()(DenseVector {RowIndexList}, {ColIndexList})")
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = RowIndexList.begin(); it != RowIndexList.end(); ++it)
    {
        if (*it >= SelfSize.RowCount || *it < 0)
        {
            MDK_Error("Invalid RowIndexList @ DenseMatrix::operator()(DenseVector {RowIndexList}, {ColIndexList})")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= SelfSize.ColCount || *it < 0)
        {
            MDK_Error("Invalid ColIndexList @ DenseMatrix::operator()(DenseVector {RowIndexList}, {ColIndexList})")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

#endif //MDK_DEBUG_DenseMatrix_Operator_CheckBound

    DenseShadowMatrix<ElementType> tempShadowMatrix(*this, std::move(RowIndexList.StdVector()), std::move(ColIndexList.StdVector()));
    return tempShadowMatrix;
}


template<typename ElementType>
inline
const DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::operator()(DenseVector<int_max> RowIndexList, DenseVector<int_max> ColIndexList) const
{
#if defined(MDK_DEBUG_DenseMatrix_Operator_CheckBound)

    if (RowIndexList.GetLength() <= 0 || ColIndexList.GetLength() <= 0)
    {
        MDK_Error("RowIndexList or  ColIndexList is empty @ DenseMatrix::operator()(DenseVector {RowIndexList}, {ColIndexList}) const")
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = RowIndexList.begin(); it != RowIndexList.end(); ++it)
    {
        if (*it >= SelfSize.RowCount || *it < 0)
        {
            MDK_Error("Invalid RowIndexList @ DenseMatrix::operator()(DenseVector {RowIndexList}, {ColIndexList}) const")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= SelfSize.ColCount)
        {
            MDK_Error("Invalid ColIndexList @ DenseMatrix::operator()(DenseVector {RowIndexList}, {ColIndexList}) const")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

#endif //MDK_DEBUG_DenseMatrix_Operator_CheckBound

    DenseShadowMatrix<ElementType> tempShadowMatrix(*this, std::move(RowIndexList.StdVector), std::move(ColIndexList.StdVector()));
    return tempShadowMatrix;
}


template<typename ElementType>
inline
DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::operator()(DenseVector<int_max> RowIndexList, const MDK_Symbol_ALL& ALL_Symbol)
{
#if defined(MDK_DEBUG_DenseMatrix_Operator_CheckBound)

    if (RowIndexList.GetLength() <= 0)
    {
        MDK_Error("RowIndexList or  ColIndexList is empty @ DenseMatrix::operator()(DenseVector {RowIndexList}, ALL)")
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = RowIndexList.begin(); it != RowIndexList.end(); ++it)
    {
        if (*it >= SelfSize.RowCount || *it < 0)
        {
            MDK_Error("Invalid RowIndexList @ DenseMatrix::operator()(DenseVector {RowIndexList}, ALL)")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

#endif //MDK_DEBUG_DenseMatrix_Operator_CheckBound

    DenseShadowMatrix<ElementType> tempShadowMatrix(*this, std::move(RowIndexList.StdVector()), ALL_Symbol);
    return tempShadowMatrix;
}


template<typename ElementType>
inline
const DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::operator()(DenseVector<int_max> RowIndexList, const MDK_Symbol_ALL& ALL_Symbol) const
{
#if defined(MDK_DEBUG_DenseMatrix_Operator_CheckBound)

    if (RowIndexList.GetLength() <= 0)
    {
        MDK_Error("RowIndexList or  ColIndexList is empty @ DenseMatrix::operator()(DenseVector {RowIndexList}, ALL) const")
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = RowIndexList.begin(); it != RowIndexList.end(); ++it)
    {
        if (*it >= SelfSize.RowCount || *it < 0)
        {
            MDK_Error("Invalid RowIndexList @ DenseMatrix::operator()(DenseVector {RowIndexList}, ALL) const")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

#endif //MDK_DEBUG_DenseMatrix_Operator_CheckBound

    DenseShadowMatrix<ElementType> tempShadowMatrix(*this, std::move(RowIndexList.StdVector()), ALL_Symbol);
    return tempShadowMatrix;
}


template<typename ElementType>
inline
DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::operator()(const MDK_Symbol_ALL& ALL_Symbol, DenseVector<int_max> ColIndexList)
{
#if defined(MDK_DEBUG_DenseMatrix_Operator_CheckBound)

    if (ColIndexList.GetLength() <= 0)
    {
        MDK_Error("ColIndexList is empty @ DenseMatrix::operator()(ALL, DenseVector {ColIndexList})")
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= SelfSize.ColCount || *it < 0)
        {
            MDK_Error("Invalid ColIndexList @ DenseMatrix::operator()(ALL, DenseVector {ColIndexList})")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

#endif //MDK_DEBUG_DenseMatrix_Operator_CheckBound

    DenseShadowMatrix<ElementType> tempShadowMatrix(*this, ALL_Symbol, std::move(ColIndexList.StdVector()));
    return tempShadowMatrix;
}


template<typename ElementType>
inline
const DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::operator()(const MDK_Symbol_ALL& ALL_Symbol, DenseVector<int_max> ColIndexList) const
{
#if defined(MDK_DEBUG_DenseMatrix_Operator_CheckBound)

    if (ColIndexList.GetLength() <= 0)
    {
        MDK_Error("ColIndexList is empty @ DenseMatrix::operator()(ALL, DenseVector {ColIndexList}) const")
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= SelfSize.ColCount || *it < 0)
        {
            MDK_Error("Invalid ColIndexList @ DenseMatrix::operator()(ALL, DenseVector {ColIndexList}) const")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

#endif //MDK_DEBUG_DenseMatrix_Operator_CheckBound

    DenseShadowMatrix<ElementType> tempShadowMatrix(*this, ALL_Symbol, std::move(ColIndexList.StdVector()));
    return tempShadowMatrix;
}


template<typename ElementType>
template<int_max TemplateVectorLengthA, int_max TemplateVectorLengthB>
inline
DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::operator()(const DenseVector<int_max, TemplateVectorLengthA>& RowIndexList, 
                                     const DenseVector<int_max, TemplateVectorLengthB>& ColIndexList)
{
    return (*this)(RowIndexList.CreateDenseVectorWithVariableLength(), ColIndexList.CreateDenseVectorWithVariableLength());
}


template<typename ElementType>
template<int_max TemplateVectorLengthA, int_max TemplateVectorLengthB>
inline
const DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::operator()(const DenseVector<int_max, TemplateVectorLengthA>& RowIndexList,
                                     const DenseVector<int_max, TemplateVectorLengthB>& ColIndexList) const
{
    return (*this)(RowIndexList.CreateDenseVectorWithVariableLength(), ColIndexList.CreateDenseVectorWithVariableLength());
}


template<typename ElementType>
template<int_max TemplateVectorLength>
inline
DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::operator()(const DenseVector<int_max, TemplateVectorLength>& RowIndexList, const MDK_Symbol_ALL& ALL_Symbol)
{
    return (*this)(RowIndexList.CreateDenseVectorWithVariableLength(), ALL_Symbol);
}


template<typename ElementType>
template<int_max TemplateVectorLength>
inline
const DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::operator()(const DenseVector<int_max, TemplateVectorLength>& RowIndexList, const MDK_Symbol_ALL& ALL_Symbol) const
{
    return (*this)(RowIndexList.CreateDenseVectorWithVariableLength(), ALL_Symbol);
}


template<typename ElementType>
template<int_max TemplateVectorLength>
inline
DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::operator()(const MDK_Symbol_ALL& ALL_Symbol, const DenseVector<int_max, TemplateVectorLength>& ColIndexList)
{
    return (*this)(ALL_Symbol, ColIndexList.CreateDenseVectorWithVariableLength());
}


template<typename ElementType>
template<int_max TemplateVectorLength>
inline
const DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::operator()(const MDK_Symbol_ALL& ALL_Symbol, const DenseVector<int_max, TemplateVectorLength>& ColIndexList) const
{
    return (*this)(ALL_Symbol, ColIndexList.CreateDenseVectorWithVariableLength());
}


template<typename ElementType>
inline
DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::operator()(const DenseMatrix<int_max>& RowIndexList, const DenseMatrix<int_max>& ColIndexList)
{
#if defined(MDK_DEBUG_DenseMatrix_Operator_CheckBound)

    if (RowIndexList.IsVector() == false || ColIndexList.IsVector() == false)
    {
        MDK_Error("RowIndexList or ColIndexList is not vector @ DenseMatrix::operator()(DenseMatrix {RowIndexList}, {ColIndexList})")
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = RowIndexList.begin(); it != RowIndexList.end(); ++it)
    {
        if (*it >= SelfSize.RowCount || *it < 0)
        {
            MDK_Error("Invalid RowIndexList @ DenseMatrix::operator()(DenseMatrix {RowIndexList}, {ColIndexList})")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= SelfSize.ColCount)
        {
            MDK_Error("Invalid ColIndexList @ DenseMatrix::operator()(DenseMatrix {RowIndexList}, {ColIndexList})")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

#endif //MDK_DEBUG_DenseMatrix_Operator_CheckBound

	DenseVector<int_max> RowIndexList_temp, ColIndexList_temp;
	RowIndexList_temp = RowIndexList;
	ColIndexList_temp = ColIndexList;

	DenseShadowMatrix<ElementType> tempShadowMatrix(*this, RowIndexList_temp.StdVector(), ColIndexList_temp.StdVector());
    return tempShadowMatrix;
}


template<typename ElementType>
inline
const DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::operator()(const DenseMatrix<int_max>& RowIndexList, const DenseMatrix<int_max>& ColIndexList) const
{
#if defined(MDK_DEBUG_DenseMatrix_Operator_CheckBound)

    if (RowIndexList.IsVector() == false || ColIndexList.IsVector() == false)
    {
        MDK_Error("RowIndexList or ColIndexList is not vector @ DenseMatrix::operator()(DenseMatrix {RowIndexList}, {ColIndexList})")
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = RowIndexList.begin(); it != RowIndexList.end(); ++it)
    {
        if (*it >= SelfSize.RowCount || *it < 0)
        {
            MDK_Error("Invalid RowIndexList @ DenseMatrix::operator()(DenseMatrix {RowIndexList}, {ColIndexList}) const")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= SelfSize.ColCount)
        {
            MDK_Error("Invalid ColIndexList @ DenseMatrix::operator()(DenseMatrix {RowIndexList}, {ColIndexList}) const")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

#endif //MDK_DEBUG_DenseMatrix_Operator_CheckBound

	DenseVector<int_max> RowIndexList_temp, ColIndexList_temp;
	RowIndexList_temp = RowIndexList;
	ColIndexList_temp = ColIndexList;

	DenseShadowMatrix<ElementType> tempShadowMatrix(*this, RowIndexList_temp.StdVector(), ColIndexList_temp.StdVector());
    return tempShadowMatrix;
}


template<typename ElementType>
inline
DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::operator()(const DenseMatrix<int_max>& RowIndexList, const MDK_Symbol_ALL& ALL_Symbol)
{
#if defined(MDK_DEBUG_DenseMatrix_Operator_CheckBound)

    if (RowIndexList.IsVector() == false)
    {
        MDK_Error("Invalid input @ DenseMatrix::operator()(DenseMatrix: RowIndexList, ALL)")

        DenseShadowMatrix<ElementType> EmptyShadowMatrix;

        return EmptyShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = RowIndexList.begin(); it != RowIndexList.end(); ++it)
    {
        if (*it >= SelfSize.RowCount || *it < 0)
        {
            MDK_Error("Invalid RowIndexList @ DenseMatrix::operator()(DenseMatrix: {RowIndexList}, ALL)")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

#endif //MDK_DEBUG_DenseMatrix_Operator_CheckBound

	DenseVector<int_max> RowIndexList_temp = RowIndexList;

	DenseShadowMatrix<ElementType> tempShadowMatrix(*this, RowIndexList_temp.StdVector(), ALL_Symbol);
    return tempShadowMatrix;
}


template<typename ElementType>
inline
const DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::operator()(const DenseMatrix<int_max>& RowIndexList, const MDK_Symbol_ALL& ALL_Symbol) const
{
#if defined(MDK_DEBUG_DenseMatrix_Operator_CheckBound)

    if (RowIndexList.IsVector() == false)
    {
        MDK_Error("Invalid input @ DenseMatrix::operator()(DenseMatrix: RowIndexList, ALL) const")

        DenseShadowMatrix<ElementType> EmptyShadowMatrix;

        return EmptyShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = RowIndexList.begin(); it != RowIndexList.end(); ++it)
    {
        if (*it >= SelfSize.RowCount || *it < 0)
        {
            MDK_Error("Invalid RowIndexList @ DenseMatrix::operator()(DenseMatrix: {RowIndexList}, ALL) const")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

#endif //MDK_DEBUG_DenseMatrix_Operator_CheckBound

	DenseVector<int_max> RowIndexList_temp = RowIndexList;

	DenseShadowMatrix<ElementType> tempShadowMatrix(*this, RowIndexList_temp.StdVector(), ALL_Symbol);
    return tempShadowMatrix;
}


template<typename ElementType>
inline
DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::operator()(const MDK_Symbol_ALL& ALL_Symbol, const DenseMatrix<int_max>& ColIndexList)
{
#if defined(MDK_DEBUG_DenseMatrix_Operator_CheckBound)

    if (ColIndexList.IsVector() == false)
    {
        MDK_Error("Invalid input @ DenseMatrix::operator()(DenseMatrix: ALL, ColIndexList)")

        DenseShadowMatrix<ElementType> EmptyShadowMatrix;

        return EmptyShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= SelfSize.ColCount || *it < 0)
        {
            MDK_Error("Invalid ColIndexList @ DenseMatrix::operator()(DenseMatrix: ALL, ColIndexList)")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

#endif //MDK_DEBUG_DenseMatrix_Operator_CheckBound

	DenseVector<int_max> ColIndexList_temp = ColIndexList;

	DenseShadowMatrix<ElementType> tempShadowMatrix(*this, ALL_Symbol, ColIndexList_temp.StdVector());
    return tempShadowMatrix;
}


template<typename ElementType>
inline
const DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::operator()(const MDK_Symbol_ALL& ALL_Symbol, const DenseMatrix<int_max>& ColIndexList) const
{
#if defined(MDK_DEBUG_DenseMatrix_Operator_CheckBound)

    if (ColIndexList.IsVector() == false)
    {
        MDK_Error("Invalid input @ DenseMatrix::operator()(DenseMatrix: ALL, ColIndexList) const")

        DenseShadowMatrix<ElementType> EmptyShadowMatrix;

        return EmptyShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= SelfSize.ColCount || *it < 0)
        {
            MDK_Error("Invalid ColIndexList @ DenseMatrix::operator()(DenseMatrix: ALL, ColIndexList) const")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

#endif //MDK_DEBUG_DenseMatrix_Operator_CheckBound

	DenseVector<int_max> ColIndexList_temp = ColIndexList;

	DenseShadowMatrix<ElementType> tempShadowMatrix(*this, ALL_Symbol, ColIndexList_temp.StdVector());
    return tempShadowMatrix;
}

// at(): bound check

template<typename ElementType>
inline
DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::at(const std::initializer_list<int_max>& RowIndexList, const std::initializer_list<int_max>& ColIndexList)
{
    if (RowIndexList.size() <= 0 || ColIndexList.size() <= 0)
    {
        MDK_Error("RowIndexList or  ColIndexList is empty @ DenseMatrix::at(std::initializer_list {RowIndexList}, {ColIndexList})")
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = RowIndexList.begin(); it != RowIndexList.end(); ++it)
    {
        if (*it >= SelfSize.RowCount || *it < 0)
        {
            MDK_Error("Invalid RowIndexList @ DenseMatrix::at(std::initializer_list {RowIndexList}, {ColIndexList})")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= SelfSize.ColCount || *it < 0)
        {
            MDK_Error("Invalid ColIndexList @ DenseMatrix::at({RowIndexList}, {ColIndexList})")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

	DenseVector<int_max> RowIndexList_temp, ColIndexList_temp;
	RowIndexList_temp = RowIndexList;
	ColIndexList_temp = ColIndexList;

	DenseShadowMatrix<ElementType> tempShadowMatrix(*this, RowIndexList_temp.StdVector(), ColIndexList_temp.StdVector());
    return tempShadowMatrix;
}


template<typename ElementType>
inline
const DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::at(const std::initializer_list<int_max>& RowIndexList, const std::initializer_list<int_max>& ColIndexList) const
{
    if (RowIndexList.size() <= 0 || ColIndexList.size() <= 0)
    {
        MDK_Error("RowIndexList or  ColIndexList is empty @ DenseMatrix::at(std::initializer_list {RowIndexList}, {ColIndexList}) const")
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = RowIndexList.begin(); it != RowIndexList.end(); ++it)
    {
        if (*it >= SelfSize.RowCount || *it < 0)
        {
            MDK_Error("Invalid RowIndexList @ DenseMatrix::at(std::initializer_list {RowIndexList}, {ColIndexList}) const")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= SelfSize.ColCount || *it < 0)
        {
            MDK_Error("Invalid ColIndexList @ DenseMatrix::at({RowIndexList}, {ColIndexList})")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

	DenseVector<int_max> RowIndexList_temp, ColIndexList_temp;
	RowIndexList_temp = RowIndexList;
	ColIndexList_temp = ColIndexList;

	DenseShadowMatrix<ElementType> tempShadowMatrix(*this, RowIndexList_temp.StdVector(), ColIndexList_temp.StdVector());
    return tempShadowMatrix;
}


template<typename ElementType>
inline
DenseShadowMatrix<ElementType> 
DenseMatrix<ElementType>::at(const std::initializer_list<int_max>& RowIndexList, const MDK_Symbol_ALL& ALL_Symbol)
{
    if (RowIndexList.size() <= 0)
    {
        MDK_Error("RowIndexList is empty @ DenseMatrix::at(std::initializer_list {RowIndexList}, ALL)")
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = RowIndexList.begin(); it != RowIndexList.end(); ++it)
    {
        if (*it >= SelfSize.RowCount || *it < 0)
        {
            MDK_Error("Invalid RowIndexList @ DenseMatrix::at(std::initializer_list {RowIndexList}, ALL)")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

	DenseVector<int_max> RowIndexList_temp = RowIndexList;

	DenseShadowMatrix<ElementType> tempShadowMatrix(*this, RowIndexList_temp.StdVector(), ALL_Symbol);
    return tempShadowMatrix;
}


template<typename ElementType>
inline
const DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::at(const std::initializer_list<int_max>& RowIndexList, const MDK_Symbol_ALL& ALL_Symbol) const
{
    if (RowIndexList.size() <= 0)
    {
        MDK_Error("RowIndexList is empty @ DenseMatrix::at(std::initializer_list {RowIndexList}, ALL) const")
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = RowIndexList.begin(); it != RowIndexList.end(); ++it)
    {
        if (*it >= SelfSize.RowCount || *it < 0)
        {
            MDK_Error("Invalid RowIndexList @ DenseMatrix::at(std::initializer_list {RowIndexList}, ALL) const")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

	DenseVector<int_max> RowIndexList_temp = RowIndexList;

	DenseShadowMatrix<ElementType> tempShadowMatrix(*this, RowIndexList_temp.StdVector(), ALL_Symbol);
    return tempShadowMatrix;
}


template<typename ElementType>
inline
DenseShadowMatrix<ElementType> 
DenseMatrix<ElementType>::at(const MDK_Symbol_ALL& ALL_Symbol, const std::initializer_list<int_max>& ColIndexList)
{
    if (ColIndexList.size() <= 0)
    {
        MDK_Error("ColIndexList is empty @ DenseMatrix::at(ALL, std::initializer_list {ColIndexList})")
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= SelfSize.ColCount || *it < 0)
        {
            MDK_Error("Invalid ColIndexList @ DenseMatrix::at(ALL, std::initializer_list {ColIndexList})")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

	DenseVector<int_max> ColIndexList_temp = ColIndexList;

	DenseShadowMatrix<ElementType> tempShadowMatrix(*this, ALL_Symbol, ColIndexList_temp.StdVector());
    return tempShadowMatrix;
}


template<typename ElementType>
inline
const DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::at(const MDK_Symbol_ALL& ALL_Symbol, const std::initializer_list<int_max>& ColIndexList) const
{
    if (ColIndexList.size() <= 0)
    {
        MDK_Error("ColIndexList is empty @ DenseMatrix::at(ALL, std::initializer_list {ColIndexList}) const")
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= SelfSize.ColCount || *it < 0)
        {
            MDK_Error("Invalid ColIndexList @ DenseMatrix::at(ALL, std::initializer_list {ColIndexList}) const")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

	DenseVector<int_max> ColIndexList_temp = ColIndexList;

	DenseShadowMatrix<ElementType> tempShadowMatrix(*this, ALL_Symbol, ColIndexList_temp.StdVector());
    return tempShadowMatrix;
}


template<typename ElementType>
inline
DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::at(DenseVector<int_max> RowIndexList, DenseVector<int_max> ColIndexList)
{
    if (RowIndexList.GetLength() <= 0 || ColIndexList.GetLength() <= 0)
    {
        MDK_Error("RowIndexList or  ColIndexList is empty @ DenseMatrix::at(DenseVector {RowIndexList}, {ColIndexList})")
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = RowIndexList.begin(); it != RowIndexList.end(); ++it)
    {
        if (*it >= SelfSize.RowCount || *it < 0)
        {
            MDK_Error("Invalid RowIndexList @ DenseMatrix::at(DenseVector {RowIndexList}, {ColIndexList})")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= SelfSize.ColCount || *it < 0)
        {
            MDK_Error("Invalid ColIndexList @ DenseMatrix::at(DenseVector {RowIndexList}, {ColIndexList})")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    DenseShadowMatrix<ElementType> tempShadowMatrix(*this, RowIndexList.StdVector(), ColIndexList.StdVector());
    return tempShadowMatrix;
}


template<typename ElementType>
inline
const DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::at(DenseVector<int_max> RowIndexList, DenseVector<int_max> ColIndexList) const
{
    if (RowIndexList.GetLength() <= 0 || ColIndexList.GetLength() <= 0)
    {
        MDK_Error("RowIndexList or  ColIndexList is empty @ DenseMatrix::at(DenseVector {RowIndexList}, {ColIndexList}) const")
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = RowIndexList.begin(); it != RowIndexList.end(); ++it)
    {
        if (*it >= SelfSize.RowCount || *it < 0)
        {
            MDK_Error("Invalid RowIndexList @ DenseMatrix::at(DenseVector {RowIndexList}, {ColIndexList}) const")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= SelfSize.ColCount || *it < 0)
        {
            MDK_Error("Invalid ColIndexList @ DenseMatrix::at(DenseVector {RowIndexList}, {ColIndexList}) const")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    DenseShadowMatrix<ElementType> tempShadowMatrix(*this, RowIndexList.StdVector(), ColIndexList.StdVector());
    return tempShadowMatrix;
}


template<typename ElementType>
inline
DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::at(DenseVector<int_max> RowIndexList, const MDK_Symbol_ALL& ALL_Symbol)
{
    if (RowIndexList.GetLength() <= 0)
    {
        MDK_Error("RowIndexList is empty @ DenseMatrix::at(DenseVector {RowIndexList}, ALL)")
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = RowIndexList.begin(); it != RowIndexList.end(); ++it)
    {
        if (*it >= SelfSize.RowCount || *it < 0)
        {
            MDK_Error("Invalid RowIndexList @ DenseMatrix::at(DenseVector {RowIndexList}, ALL)")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    DenseShadowMatrix<ElementType> tempShadowMatrix(*this, RowIndexList.StdVector(), ALL_Symbol);
    return tempShadowMatrix;
}


template<typename ElementType>
inline
const DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::at(DenseVector<int_max> RowIndexList, const MDK_Symbol_ALL& ALL_Symbol) const
{
    if (RowIndexList.GetLength() <= 0)
    {
        MDK_Error("RowIndexList is empty @ DenseMatrix::at(DenseVector {RowIndexList}, ALL) const")
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = RowIndexList.begin(); it != RowIndexList.end(); ++it)
    {
        if (*it >= SelfSize.RowCount || *it < 0)
        {
            MDK_Error("Invalid RowIndexList @ DenseMatrix::at(DenseVector {RowIndexList}, ALL) const")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    DenseShadowMatrix<ElementType> tempShadowMatrix(*this, RowIndexList.StdVector(), ALL_Symbol);
    return tempShadowMatrix;
}


template<typename ElementType>
inline
DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::at(const MDK_Symbol_ALL& ALL_Symbol, DenseVector<int_max> ColIndexList)
{
    if (ColIndexList.GetLength() <= 0)
    {
        MDK_Error("ColIndexList is empty @ DenseMatrix::at(ALL, DenseVector {ColIndexList})")
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= SelfSize.ColCount || *it < 0)
        {
            MDK_Error("Invalid ColIndexList @ DenseMatrix::at(ALL, DenseVector {ColIndexList})")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    DenseShadowMatrix<ElementType> tempShadowMatrix(*this, ALL_Symbol, ColIndexList.StdVector());
    return tempShadowMatrix;
}


template<typename ElementType>
inline
const DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::at(const MDK_Symbol_ALL& ALL_Symbol, DenseVector<int_max> ColIndexList) const
{
    if (ColIndexList.size() <= 0)
    {
        MDK_Error("ColIndexList is empty @ DenseMatrix::at(ALL, DenseVector {ColIndexList}) const")
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= SelfSize.ColCount || *it < 0)
        {
            MDK_Error("Invalid ColIndexList @ DenseMatrix::at(ALL, DenseVector {ColIndexList}) const")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    DenseShadowMatrix<ElementType> tempShadowMatrix(*this, ALL_Symbol, ColIndexList.StdVector());
    return tempShadowMatrix;
}


template<typename ElementType>
template<int_max TemplateVectorLengthA, int_max TemplateVectorLengthB>
inline
DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::at(const DenseVector<int_max, TemplateVectorLengthA>& RowIndexList,
							 const DenseVector<int_max, TemplateVectorLengthB>& ColIndexList)
{
    return this->at(RowIndexList.CreateDenseVectorWithVariableLength(), ColIndexList.CreateDenseVectorWithVariableLength());
}


template<typename ElementType>
template<int_max TemplateVectorLengthA, int_max TemplateVectorLengthB>
inline
const DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::at(const DenseVector<int_max, TemplateVectorLengthA>& RowIndexList, 
						     const DenseVector<int_max, TemplateVectorLengthB>& ColIndexList) const
{
    return this->at(RowIndexList.CreateDenseVectorWithVariableLength(), ColIndexList.CreateDenseVectorWithVariableLength());
}


template<typename ElementType>
template<int_max TemplateVectorLength>
inline
DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::at(const DenseVector<int_max, TemplateVectorLength>& RowIndexList, const MDK_Symbol_ALL& ALL_Symbol)
{
    return this->at(RowIndexList.CreateDenseVectorWithVariableLength(), ALL_Symbol);
}


template<typename ElementType>
template<int_max TemplateVectorLength>
inline
const DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::at(const DenseVector<int_max, TemplateVectorLength>& RowIndexList, const MDK_Symbol_ALL& ALL_Symbol) const
{
    return this->at(RowIndexList.CreateDenseVectorWithVariableLength(), ColIndexList.CreateDenseVectorWithVariableLength());
}


template<typename ElementType>
template<int_max TemplateVectorLength>
inline
DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::at(const MDK_Symbol_ALL& ALL_Symbol, const DenseVector<int_max, TemplateVectorLength>& ColIndexList)
{
    return this->at(ALL_Symbol, ColIndexList.CreateDenseVectorWithVariableLength());
}


template<typename ElementType>
template<int_max TemplateVectorLength>
inline
const DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::at(const MDK_Symbol_ALL& ALL_Symbol, const DenseVector<int_max, TemplateVectorLength>& ColIndexList) const
{
    return this->at(ALL_Symbol, ColIndexList.CreateDenseVectorWithVariableLength());
}


template<typename ElementType>
inline
DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::at(const DenseMatrix<int_max>& RowIndexList, const DenseMatrix<int_max>& ColIndexList)
{
    if (RowIndexList.IsVector() == false || ColIndexList.IsVector() == false)
    {
        MDK_Error("Invalid input @ DenseMatrix::at(DenseMatrix: RowIndexList, ColIndexList)")
        DenseShadowMatrix<ElementType> EmptyShadowMatrix;
        return EmptyShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = RowIndexList.begin(); it != RowIndexList.end(); ++it)
    {
        if (*it >= SelfSize.RowCount || *it < 0)
        {
            MDK_Error("Invalid RowIndexList @ DenseMatrix::at(DenseMatrix: {RowIndexList}, {ColIndexList})")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= SelfSize.ColCount || *it < 0)
        {
            MDK_Error("Invalid ColIndexList @ DenseMatrix::at(DenseMatrix: {RowIndexList}, {ColIndexList})")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

	DenseVector<int_max> RowIndexList_temp, ColIndexList_temp;
	RowIndexList_temp = RowIndexList;
	ColIndexList_temp = ColIndexList;

	DenseShadowMatrix<ElementType> tempShadowMatrix(*this, RowIndexList_temp.StdVector(), ColIndexList_temp.StdVector());
    return tempShadowMatrix;
}


template<typename ElementType>
inline
const DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::at(const DenseMatrix<int_max>& RowIndexList, const DenseMatrix<int_max>& ColIndexList) const
{
     if (RowIndexList.IsVector() == false || ColIndexList.IsVector() == false)
    {
        MDK_Error("Invalid input @ DenseMatrix::at(DenseMatrix: RowIndexList, ColIndexList)")
        DenseShadowMatrix<ElementType> EmptyShadowMatrix;
        return EmptyShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = RowIndexList.begin(); it != RowIndexList.end(); ++it)
    {
        if (*it >= SelfSize.RowCount || *it < 0)
        {
            MDK_Error("Invalid RowIndexList @ DenseMatrix::at(DenseMatrix: {RowIndexList}, {ColIndexList}) const")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= SelfSize.ColCount || *it < 0)
        {
            MDK_Error("Invalid ColIndexList @ DenseMatrix::at(DenseMatrix: {RowIndexList}, {ColIndexList}) const")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

	DenseVector<int_max> RowIndexList_temp, ColIndexList_temp;
	RowIndexList_temp = RowIndexList;
	ColIndexList_temp = ColIndexList;

	DenseShadowMatrix<ElementType> tempShadowMatrix(*this, RowIndexList_temp.StdVector(), ColIndexList_temp.StdVector());
    return tempShadowMatrix;
}


template<typename ElementType>
inline
DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::at(const DenseMatrix<int_max>& RowIndexList, const MDK_Symbol_ALL& ALL_Symbol)
{
    if (RowIndexList.IsVector() == false)
    {
        MDK_Error("Invalid input @ DenseMatrix::at(DenseMatrix: RowIndexList, ALL)")
        DenseShadowMatrix<ElementType> EmptyShadowMatrix;
        return EmptyShadowMatrix;
    }
    
    auto SelfSize = this->GetSize();

    for (auto it = RowIndexList.begin(); it != RowIndexList.end(); ++it)
    {
        if (*it >= SelfSize.RowCount || *it < 0)
        {
            MDK_Error("Invalid RowIndexList @ DenseMatrix::at(DenseMatrix: {RowIndexList}, ALL)")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

	DenseVector<int_max> RowIndexList_temp = RowIndexList;

	DenseShadowMatrix<ElementType> tempShadowMatrix(*this, RowIndexList_temp.StdVector(), ALL_Symbol);
    return tempShadowMatrix;
}


template<typename ElementType>
inline
const DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::at(const DenseMatrix<int_max>& RowIndexList, const MDK_Symbol_ALL& ALL_Symbol) const
{
    if (RowIndexList.IsVector() == false)
    {
        MDK_Error("Invalid input @ DenseMatrix::at(DenseMatrix: RowIndexList, ALL) const")
        DenseShadowMatrix<ElementType> EmptyShadowMatrix;
        return EmptyShadowMatrix;
    }
    
    auto SelfSize = this->GetSize();

    for (auto it = RowIndexList.begin(); it != RowIndexList.end(); ++it)
    {
        if (*it >= SelfSize.RowCount || *it < 0)
        {
            MDK_Error("Invalid RowIndexList @ DenseMatrix::at(DenseMatrix: {RowIndexList}, ALL) const")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

	DenseVector<int_max> RowIndexList_temp = RowIndexList;

	DenseShadowMatrix<ElementType> tempShadowMatrix(*this, RowIndexList_temp.StdVector(), ALL_Symbol);
    return tempShadowMatrix;
}


template<typename ElementType>
inline
DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::at(const MDK_Symbol_ALL& ALL_Symbol, const DenseMatrix<int_max>& ColIndexList)
{
    if (ColIndexList.IsVector() == false)
    {
        MDK_Error("Invalid input @ DenseMatrix::at(DenseMatrix: ALL, ColIndexList)")
        DenseShadowMatrix<ElementType> EmptyShadowMatrix;
        return EmptyShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= SelfSize.ColCount || *it < 0)
        {
            MDK_Error("Invalid ColIndexList @ DenseMatrix::at(DenseMatrix: ALL, {ColIndexList})")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

	DenseVector<int_max> ColIndexList_temp = ColIndexList;

	DenseShadowMatrix<ElementType> tempShadowMatrix(*this, ALL_Symbol, ColIndexList_temp.StdVector());
    return tempShadowMatrix;
}


template<typename ElementType>
inline
const DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::at(const MDK_Symbol_ALL& ALL_Symbol, const DenseMatrix<int_max>& ColIndexList) const
{
    if (ColIndexList.IsVector() == false)
    {
        MDK_Error("Invalid input @ DenseMatrix::at(DenseMatrix: ALL, ColIndexList) const")
        DenseShadowMatrix<ElementType> EmptyShadowMatrix;
        return EmptyShadowMatrix;
    }
    
    auto SelfSize = this->GetSize();

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= SelfSize.ColCount || *it < 0)
        {
            MDK_Error("Invalid ColIndexList @ DenseMatrix::at(DenseMatrix: ALL, ColIndexList) const")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

	DenseVector<int_max> ColIndexList_temp = ColIndexList;

	DenseShadowMatrix<ElementType> tempShadowMatrix(*this, ALL_Symbol, ColIndexList_temp.StdVector());
    return tempShadowMatrix;
}


template<typename ElementType>
inline
DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::Col(int_max ColIndex)
{
    return (*this)(ALL, { ColIndex });
}


template<typename ElementType>
inline
const DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::Col(int_max ColIndex) const
{
    return (*this)(ALL, { ColIndex });
}


template<typename ElementType>
inline
DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::Col(const std::initializer_list<int_max>& ColIndexList)
{
    return (*this)(ALL, ColIndexList);
}


template<typename ElementType>
inline
const DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::Col(const std::initializer_list<int_max>& ColIndexList) const
{
    return (*this)(ALL, ColIndexList);
}


template<typename ElementType>
inline
DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::Col(DenseVector<int_max> ColIndexList)
{
    return (*this)(ALL, std::move(ColIndexList));
}


template<typename ElementType>
inline
const DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::Col(DenseVector<int_max> ColIndexList) const
{
    return const_cast<DenseShadowMatrix<ElementType>&&>((*this)(ALL, std::move(ColIndexList)));
}


template<typename ElementType>
template<int_max TemplateVectorLength>
inline
DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::Col(const DenseVector<int_max, TemplateVectorLength>& ColIndexList)
{
    return (*this)(ALL, ColIndexList.CreateDenseVectorWithVariableLength());
}


template<typename ElementType>
template<int_max TemplateVectorLength>
inline
const DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::Col(const DenseVector<int_max, TemplateVectorLength>& ColIndexList) const
{
    return (*this)(ALL, ColIndexList.CreateDenseVectorWithVariableLength());
}


template<typename ElementType>
inline
DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::Col(const DenseMatrix<int_max>& ColIndexList)
{
    return (*this)(ALL, ColIndexList);
}


template<typename ElementType>
inline
const DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::Col(const DenseMatrix<int_max>& ColIndexList) const
{
    return (*this)(ALL, ColIndexList);
}


template<typename ElementType>
inline
DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::Row(int_max RowIndex)
{
    return (*this)({ RowIndex }, ALL);
}


template<typename ElementType>
inline
const DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::Row(int_max RowIndex) const
{
    return (*this)({ RowIndex }, ALL);
}


template<typename ElementType>
inline
DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::Row(const std::initializer_list<int_max>& RowIndexList)
{
    return (*this)(RowIndexList, ALL);
}


template<typename ElementType>
inline
const DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::Row(const std::initializer_list<int_max>& RowIndexList) const
{
    return (*this)(RowIndexList, ALL);
}


template<typename ElementType>
inline
DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::Row(DenseVector<int_max> RowIndexList)
{
    return (*this)(std::move(RowIndexList), ALL);
}


template<typename ElementType>
inline
const DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::Row(DenseVector<int_max> RowIndexList) const
{
    return (*this)(std::move(RowIndexList), ALL);
}


template<typename ElementType>
template<int_max TemplateVectorLength>
inline
DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::Row(const DenseVector<int_max, TemplateVectorLength>& RowIndexList)
{
    return (*this)(RowIndexList.CreateDenseVectorWithVariableLength(), ALL);
}


template<typename ElementType>
template<int_max TemplateVectorLength>
inline
const DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::Row(const DenseVector<int_max, TemplateVectorLength>& RowIndexList) const
{
    return (*this)(RowIndexList.CreateDenseVectorWithVariableLength(), ALL);
}


template<typename ElementType>
inline
DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::Row(const DenseMatrix<int_max>& RowIndexList)
{
    return (*this)(RowIndexList, ALL);
}


template<typename ElementType>
inline
const DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::Row(const DenseMatrix<int_max>& RowIndexList) const
{
    return (*this)(RowIndexList, ALL);
}


template<typename ElementType>
inline
DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::Diagonal()
{
    auto SelfSize = this->GetSize();

    if (SelfSize.RowCount <= 0 || SelfSize.RowCount != SelfSize.ColCount)
    {
        MDK_Error(" Self is empty or not square @ DenseMatrix::Diagonal()")
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return  tempShadowMatrix;
    }

	std::vector<int_max> LinearIndexList;
	LinearIndexList.resize(SelfSize.ColCount);

    int_max Index = 0;

    for (int_max i = 0; i < SelfSize.RowCount; ++i)
    {
        LinearIndexList[j] = Index + i;

        Index += SelfSize.RowCount;
    }

    DenseShadowMatrix<ElementType> tempShadowMatrix(*this, LinearIndexList);
    return tempShadowMatrix;
}


template<typename ElementType>
inline
const DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::Diagonal() const
{
    auto SelfSize = this->GetSize();

    if (SelfSize.RowCount <= 0 || SelfSize.RowCount != SelfSize.ColCount)
    {
        MDK_Error(" Self is empty or not square @ DenseMatrix::Diagonal() const")
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return  tempShadowMatrix;
    }

	std::vector<int_max> LinearIndexList;
	LinearIndexList.resize(SelfSize.ColCount);

    int_max Index = 0;

    for (int_max i = 0; i < SelfSize.RowCount; ++i)
    {
        LinearIndexList[j] = Index + i;

        Index += SelfSize.RowCount;
    }

    DenseShadowMatrix<ElementType> tempShadowMatrix(*this, LinearIndexList);
    return tempShadowMatrix;
}


template<typename ElementType>
inline 
DenseMatrix<ElementType> DenseMatrix<ElementType>::RefCol(int_max ColIndex)
{
    DenseMatrix<ElementType> tempCol(this->GetElementPointerOfCol(ColIndex), this->GetRowCount(), 1, true);
    return tempCol;
}


template<typename ElementType>
inline 
const DenseMatrix<ElementType> DenseMatrix<ElementType>::RefCol(int_max ColIndex) const
{
    DenseMatrix<ElementType> tempCol(const_cast<ElementType*>(this->GetElementPointerOfCol(ColIndex)), this->GetRowCount(), 1, true);
    return tempCol;
}


// return SubMatrix as DenseMatrix -----------------------------------------------------------------------------------------------------------------

template<typename ElementType>
inline
DenseMatrix<ElementType> DenseMatrix<ElementType>::GetSubMatrix(const std::initializer_list<int_max>& LinearIndexList) const
{
    return this->GetSubMatrix(LinearIndexList.begin(), int_max(LinearIndexList.size()));
}


template<typename ElementType>
template<int_max TemplateVectorLength>
inline
DenseMatrix<ElementType> DenseMatrix<ElementType>::GetSubMatrix(const DenseVector<int_max, TemplateVectorLength>& LinearIndexList) const
{//TemplateVectorLength is -1 for variable length DenseVector
    return this->GetSubMatrix(LinearIndexList.GetElementPointer(), LinearIndexList.GetElementCount());
}


template<typename ElementType>
inline
DenseMatrix<ElementType> DenseMatrix<ElementType>::GetSubMatrix(const DenseMatrix<int_max>& LinearIndexList) const
{
    return this->GetSubMatrix(LinearIndexList.GetElementPointer(), LinearIndexList.GetElementCount());
}


template<typename ElementType>
inline
DenseMatrix<ElementType> DenseMatrix<ElementType>::GetSubMatrix(const int_max* LinearIndexList, int_max ListLength) const
{
    DenseMatrix<ElementType> OutputMatrix;

    auto Self_ElementNumber = this->GetElementCount();

    if (LinearIndexList == nullptr || ListLength <= 0 || ListLength >= Self_ElementNumber)
    {
        MDK_Warning("Input is empty @ DenseMatrix::GetSubMatrix(LinearIndexList)")
        return OutputMatrix;
    }

    auto Ptr = this->GetElementPointer();

    OutputMatrix.FastResize(1, ListLength);
    for (int_max i = 0; i < ListLength; ++i)
    {
        auto Index = LinearIndexList[i];
        if (Index < 0 || Index >= Self_ElementNumber)
        {
            OutputMatrix.Clear();
            MDK_Error("Invalid Index @ DenseMatrix::GetSubMatrix(LinearIndexList)")
            return OutputMatrix;
        }

        OutputMatrix[i] = Ptr[Index];
    }

    if (this->IsColVector() == true)
    {
        OutputMatrix.Reshape(ListLength, 1);
    }

    return OutputMatrix;
}

template<typename ElementType>
inline
DenseMatrix<ElementType> DenseMatrix<ElementType>::GetSubMatrix(const std::initializer_list<int_max>& RowIndexList, 
                                                                const std::initializer_list<int_max>& ColIndexList) const
{
    DenseMatrix<ElementType> tempMatrix;

    this->GetSubMatrix(tempMatrix,
                       RowIndexList.begin(), int_max(RowIndexList.size()),
                       ColIndexList.begin(), int_max(ColIndexList.size()));

    return tempMatrix;
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::GetSubMatrix(DenseMatrix<ElementType>& OutputMatrix,
                                            const std::initializer_list<int_max>& RowIndexList,
                                            const std::initializer_list<int_max>& ColIndexList) const
{
    this->GetSubMatrix(OutputMatrix,
                       RowIndexList.begin(), int_max(RowIndexList.size()),
                       ColIndexList.begin(), int_max(ColIndexList.size()));
}


template<typename ElementType>
inline
DenseMatrix<ElementType> DenseMatrix<ElementType>::GetSubMatrix(const std::initializer_list<int_max>& RowIndexList, 
                                                                const MDK_Symbol_ALL& ALL_Symbol) const
{
    DenseMatrix<ElementType> tempMatrix;
    this->GetSubMatrix(tempMatrix, RowIndexList.begin(), int_max(RowIndexList.size()), ALL_Symbol);
    return tempMatrix;
}


template<typename ElementType>
inline 
void DenseMatrix<ElementType>::GetSubMatrix(DenseMatrix<ElementType>& OutputMatrix,
                                            const std::initializer_list<int_max>& RowIndexList,
                                            const MDK_Symbol_ALL& ALL_Symbol) const
{
    this->GetSubMatrix(OutputMatrix, RowIndexList.begin(), int_max(RowIndexList.size()), ALL_Symbol);
}


template<typename ElementType>
inline
DenseMatrix<ElementType> DenseMatrix<ElementType>::GetSubMatrix(const MDK_Symbol_ALL& ALL_Symbol, 
                                                                const std::initializer_list<int_max>& ColIndexList) const
{
    DenseMatrix<ElementType> tempMatrix;
    this->GetSubMatrix(tempMatrix, ALL_Symbol, ColIndexList.begin(), int_max(ColIndexList.size()));
    return tempMatrix;
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::GetSubMatrix(DenseMatrix<ElementType>& OutputMatrix,
                                            const MDK_Symbol_ALL& ALL_Symbol,
                                            const std::initializer_list<int_max>& ColIndexList) const
{
    this->GetSubMatrix(OutputMatrix, ALL_Symbol, ColIndexList.begin(), int_max(ColIndexList.size()));
}


template<typename ElementType>
template<int_max TemplateVectorLengthA, int_max TemplateVectorLengthB>
inline
DenseMatrix<ElementType> DenseMatrix<ElementType>::GetSubMatrix(const DenseVector<int_max, TemplateVectorLengthA>& RowIndexList,
													            const DenseVector<int_max, TemplateVectorLengthB>& ColIndexList) const
{
    DenseMatrix<ElementType> tempMatrix;
    this->GetSubMatrix(tempMatrix, 
                       RowIndexList.GetElementPointer(), RowIndexList.GetElementCount(), 
                       ColIndexList.GetElementPointer(), ColIndexList.GetElementCount());
    return tempMatrix;
}


template<typename ElementType>
template<int_max TemplateVectorLengthA, int_max TemplateVectorLengthB>
inline
void DenseMatrix<ElementType>::GetSubMatrix(DenseMatrix<ElementType>& OutputMatrix,
                                            const DenseVector<int_max, TemplateVectorLengthA>& RowIndexList,
											const DenseVector<int_max, TemplateVectorLengthB>& ColIndexList) const
{
    this->GetSubMatrix(OutputMatrix,
                       RowIndexList.GetElementPointer(), RowIndexList.GetElementCount(), 
                       ColIndexList.GetElementPointer(), ColIndexList.GetElementCount());
}


template<typename ElementType>
template<int_max TemplateVectorLength>
inline
DenseMatrix<ElementType> DenseMatrix<ElementType>::GetSubMatrix(const DenseVector<int_max, TemplateVectorLength>& RowIndexList,
                                                                const MDK_Symbol_ALL& ALL_Symbol) const
{
    DenseMatrix<ElementType> tempMatrix;
    this->GetSubMatrix(tempMatrix, RowIndexList.GetElementPointer(), RowIndexList.GetElementCount(), ALL_Symbol);
    return tempMatrix;
}


template<typename ElementType>
template<int_max TemplateVectorLength>
inline
void DenseMatrix<ElementType>::GetSubMatrix(DenseMatrix<ElementType>& OutputMatrix, 
                                            const DenseVector<int_max, TemplateVectorLength>& RowIndexList,
                                            const MDK_Symbol_ALL& ALL_Symbol) const
{
    this->GetSubMatrix(OutputMatrix, RowIndexList.GetElementPointer(), RowIndexList.GetElementCount(), ALL_Symbol);
}


template<typename ElementType>
template<int_max TemplateVectorLength>
inline
DenseMatrix<ElementType> DenseMatrix<ElementType>::GetSubMatrix(const MDK_Symbol_ALL& ALL_Symbol, 
                                                                const DenseVector<int_max, TemplateVectorLength>& ColIndexList) const
{
    DenseMatrix<ElementType> tempMatrix;

    this->GetSubMatrix(tempMatrix, ALL_Symbol, ColIndexList.GetElementPointer(), ColIndexList.GetElementCount());

    return tempMatrix;
}


template<typename ElementType>
template<int_max TemplateVectorLength>
inline
void DenseMatrix<ElementType>::GetSubMatrix(DenseMatrix<ElementType>& OutputMatrix, 
                                            const MDK_Symbol_ALL& ALL_Symbol,
											const DenseVector<int_max, TemplateVectorLength>& ColIndexList) const
{
    this->GetSubMatrix(OutputMatrix, ALL_Symbol, ColIndexList.GetElementPointer(), ColIndexList.GetElementCount());
}


template<typename ElementType>
inline
DenseMatrix<ElementType> DenseMatrix<ElementType>::GetSubMatrix(const DenseMatrix<int_max>& RowIndexList, const DenseMatrix<int_max>& ColIndexList) const
{
    DenseMatrix<ElementType> tempMatrix;

    if (RowIndexList.IsVector() == false || ColIndexList.IsVector() == false)
    {
        MDK_Error("RowIndexList or ColIndexList is NOT a vector @ DenseMatrix::GetSubMatrix(DenseMatrix: RowIndexList, ColIndexList)")
        return tempMatrix;
    }

    this->GetSubMatrix(tempMatrix, 
                       RowIndexList.GetElementPointer(), RowIndexList.GetElementCount(), 
                       ColIndexList.GetElementPointer(), ColIndexList.GetElementCount());

    return tempMatrix;
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::GetSubMatrix(DenseMatrix<ElementType>& OutputMatrix,
                                            const DenseMatrix<int_max>& RowIndexList,
                                            const DenseMatrix<int_max>& ColIndexList) const
{
    if (RowIndexList.IsVector() == false || ColIndexList.IsVector() == false)
    {
        MDK_Error("RowIndexList or ColIndexList is NOT a vector @ DenseMatrix::GetSubMatrix(OutputMatrix, DenseMatrix: RowIndexList, ColIndexList)")
        return;
    }

    this->GetSubMatrix(OutputMatrix,
                       RowIndexList.GetElementPointer(), RowIndexList.GetElementCount(), 
                       ColIndexList.GetElementPointer(), ColIndexList.GetElementCount());
}


template<typename ElementType>
inline
DenseMatrix<ElementType> DenseMatrix<ElementType>::GetSubMatrix(const DenseMatrix<int_max>& RowIndexList, 
                                                                const MDK_Symbol_ALL& ALL_Symbol) const
{
    DenseMatrix<ElementType> tempMatrix;

    if (RowIndexList.IsVector() == false)
    {
        MDK_Error("RowIndexList is NOT a vector @ DenseMatrix::GetSubMatrix(DenseMatrix: RowIndexList, ALL)")
        return tempMatrix;
    }

    this->GetSubMatrix(tempMatrix, RowIndexList.GetElementPointer(), RowIndexList.GetElementCount(), ALL_Symbol);

    return tempMatrix;
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::GetSubMatrix(DenseMatrix<ElementType>& OutputMatrix, 
                                            const DenseMatrix<int_max>& RowIndexList,
                                            const MDK_Symbol_ALL& ALL_Symbol) const
{
    if (RowIndexList.IsVector() == false)
    {
        MDK_Error("RowIndexList is NOT a vector @ DenseMatrix::GetSubMatrix(DenseMatrix: RowIndexList, ALL)")
        return;
    }

    this->GetSubMatrix(OutputMatrix, RowIndexList.GetElementPointer(), RowIndexList.GetElementCount(), ALL_Symbol);
}


template<typename ElementType>
inline
DenseMatrix<ElementType> DenseMatrix<ElementType>::GetSubMatrix(const MDK_Symbol_ALL& ALL_Symbol, 
                                                                const DenseMatrix<int_max>& ColIndexList) const
{
    DenseMatrix<ElementType> tempMatrix;

    if (ColIndexList.IsVector() == false)
    {
        MDK_Error("ColIndexList is NOT a vector @ DenseMatrix::GetSubMatrix(OutputMatrix, ALL, DenseMatrix: ColIndexList)")
        return tempMatrix;
    }

    this->GetSubMatrix(tempMatrix, ALL_Symbol, ColIndexList.GetElementPointer(), ColIndexList.GetElementCount());

    return tempMatrix;
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::GetSubMatrix(DenseMatrix<ElementType>& OutputMatrix, 
                                            const MDK_Symbol_ALL& ALL_Symbol,
                                            const DenseMatrix<int_max>& ColIndexList) const
{
    if (ColIndexList.IsVector() == false)
    {
        MDK_Error("ColIndexList is NOT a vector @ DenseMatrix::GetSubMatrix(OutputMatrix, ALL, DenseMatrix: ColIndexList)")
        return;
    }
    this->GetSubMatrix(OutputMatrix, ALL_Symbol, ColIndexList.GetElementPointer(), ColIndexList.GetElementCount());
}


template<typename ElementType>
inline
DenseMatrix<ElementType> DenseMatrix<ElementType>::GetSubMatrix(const int_max* RowIndexList, int_max OutputRowCount,
                                                                const int_max* ColIndexList, int_max OutputColCount) const
{
    DenseMatrix<ElementType> OutputMatrix;
    this->GetSubMatrix(OutputMatrix, RowIndexList, OutputRowCount, ColIndexList, OutputColCount);
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::GetSubMatrix(DenseMatrix<ElementType>& OutputMatrix,
                                            const int_max* RowIndexList, int_max OutputRowCount,
                                            const int_max* ColIndexList, int_max OutputColCount) const
{
    auto SelfSize = this->GetSize();

    if (SelfSize.ColCount == 0)
    {
        MDK_Error("Self is empty @ DenseMatrix::GetSubMatrix(OutputMatrix, pointer...)")
        return;
    }

    if (this->GetElementPointer() == OutputMatrix.GetElementPointer())
    {
        DenseMatrix<ElementType> tempOutputMatrix;
        this->GetSubMatrix(tempOutputMatrix, RowIndexList, OutputRowCount, ColIndexList, OutputColCount);
		OutputMatrix.Copy(std::move(tempOutputMatrix));
        return;
    }

    if (RowIndexList == nullptr || OutputRowCount <= 0 || ColIndexList == nullptr || OutputColCount <= 0)
    {
        MDK_Error("RowIndexList or ColIndexList is empty @ DenseMatrix::GetSubMatrix(OutputMatrix, pointer,..., pointer,...)")
        return;
    }

    for (int_max i = 0; i < OutputRowCount; ++i)
    {
        auto RowIndex = RowIndexList[i];

        if (RowIndex < 0 || RowIndex >= SelfSize.RowCount)
        {
            MDK_Error("Invalid RowIndex @ DenseMatrix::GetSubMatrix(OutputMatrix, pointer,..., pointer,...)")
            return;
        }
    }

    for (int_max j = 0; j < OutputColCount; ++j)
    {
        auto ColIndex = ColIndexList[j];
        if (ColIndex < 0 || ColIndex >= SelfSize.ColCount)
        {
            MDK_Error("Invalid ColIndex @ DenseMatrix::GetSubMatrix(OutputMatrix, pointer,..., pointer,...)")
            return;
        }
    }

    OutputMatrix.FastResize(OutputRowCount, OutputColCount);

    auto OutputPointer = OutputMatrix.GetElementPointer();

    auto RawPointer = this->GetElementPointer();

	for (int_max j = 0; j < OutputMatrix.GetColCount(); ++j)
    {
        auto ColIndex = ColIndexList[j];
        auto Offset = ColIndex * SelfSize.RowCount;

		for (int_max i = 0; i < OutputMatrix.GetRowCount(); ++i)
        {
            auto RowIndex = RowIndexList[i];
            int_max LinearIndex = Offset + RowIndex;

            OutputPointer[0] = RawPointer[LinearIndex];
            ++OutputPointer;
        }
    }
}


template<typename ElementType>
inline
DenseMatrix<ElementType> DenseMatrix<ElementType>::GetSubMatrix(const MDK_Symbol_ALL& ALL_Symbol,
                                                                const int_max* ColIndexList, int_max OutputColCount) const
{
    DenseMatrix<ElementType> OutputMatrix;
    this->GetSubMatrix(OutputMatrix, ALL_Symbol, ColIndexList, OutputColCount);
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::GetSubMatrix(DenseMatrix<ElementType>& OutputMatrix,
                                            const MDK_Symbol_ALL& ALL_Symbol, 
                                            const int_max* ColIndexList, int_max OutputColCount) const
{
    auto SelfSize = this->GetSize();

    if (SelfSize.ColCount == 0)
    {
        MDK_Error("Self is empty @ DenseMatrix::GetSubMatrix(OutputMatrix, ALL, pointer...)")
        return;
    }

    if (this->GetElementPointer() == OutputMatrix.GetElementPointer())
    {
        DenseMatrix<ElementType> tempOutputMatrix;
        this->GetSubMatrix(tempOutputMatrix, ALL_Symbol, ColIndexList, OutputColCount);
        OutputMatrix.Copy(std::move(tempOutputMatrix));
        return;
    }

    if (ColIndexList == nullptr || OutputColCount <= 0)
    {
        MDK_Error("ColIndexList is empty @ DenseMatrix::GetSubMatrix(OutputMatrix, ALL, pointer...)")
        return;
    }

    for (int_max j = 0; j < OutputColCount; ++j)
    {
        auto ColIndex = ColIndexList[j];

        if (ColIndex < 0 || ColIndex >= SelfSize.ColCount)
        {
            MDK_Error("Invalid ColIndex @ DenseMatrix::GetSubMatrix(OutputMatrix, ALL, pointer...)")
            return;
        }
    }

    OutputMatrix.FastResize(SelfSize.RowCount, OutputColCount);

    auto OutputPointer = OutputMatrix.GetElementPointer();

    auto RawPointer = this->GetElementPointer();

	for (int_max j = 0; j < OutputMatrix.GetColCount(); ++j)
    {
        auto ColIndex = ColIndexList[j];
        auto Offset = ColIndex * SelfSize.RowCount;

		for (int_max i = 0; i < OutputMatrix.GetRowCount(); ++i)
        {
            int_max LinearIndex = Offset + i;

            OutputPointer[0] = RawPointer[LinearIndex];
            ++OutputPointer;
        }
    }
}


template<typename ElementType>
inline
DenseMatrix<ElementType> DenseMatrix<ElementType>::GetSubMatrix(const int_max* RowIndexList, int_max OutputRowCount,
                                                                const MDK_Symbol_ALL& ALL_Symbol) const
{
    DenseMatrix<ElementType> OutputMatrix;
    this->GetSubMatrix(OutputMatrix, RowIndexList, OutputRowCount, ALL_Symbol);
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::GetSubMatrix(DenseMatrix<ElementType>& OutputMatrix,
                                            const int_max* RowIndexList, int_max OutputRowCount,
                                            const MDK_Symbol_ALL& ALL_Symbol) const
{
    auto SelfSize = this->GetSize();

    if (SelfSize.RowCount == 0)
    {
        MDK_Error("Self is empty @ DenseMatrix::GetSubMatrix(OutputMatrix, pointer, ALL)")
        return;
    }

    if (this->GetElementPointer() == OutputMatrix.GetElementPointer())
    {
        DenseMatrix<ElementType> tempOutputMatrix;
        this->GetSubMatrix(tempOutputMatrix, RowIndexList, OutputRowCount, ALL_Symbol);
        OutputMatrix.Copy(std::move(tempOutputMatrix));
        return;
    }

    if (RowIndexList == nullptr || OutputRowCount <= 0)
    {
        MDK_Error("RowIndexList is empty @ DenseMatrix::GetSubMatrix(OutputMatrix, pointer, ALL)")
        return;
    }

    for (int_max i = 0; i < OutputRowCount; ++i)
    {
        auto RowIndex = RowIndexList[i];
        if (RowIndex < 0 || RowIndex >= SelfSize.RowCount)
        {
            MDK_Error("Invalid RowIndex @ DenseMatrix::GetSubMatrix(OutputMatrix, pointer, ALL)")
            return;
        }
    }

    OutputMatrix.FastResize(OutputRowCount, SelfSize.ColCount);
    
    auto OutputPointer = OutputMatrix.GetElementPointer();

    auto RawPointer = this->GetElementPointer();

	for (int_max j = 0; j < OutputMatrix.GetColCount(); ++j)
    {
        auto Offset = j * SelfSize.RowCount;

		for (int_max i = 0; i < OutputMatrix.GetRowCount(); ++i)
        {
            auto RowIndex = RowIndexList[i];
           
            int_max LinearIndex = Offset + RowIndex;

            OutputPointer[0] = RawPointer[LinearIndex];

            ++OutputPointer;
        }
    }
}


template<typename ElementType>
template<typename ElementType_Output>
inline 
DenseMatrix<ElementType_Output> DenseMatrix<ElementType>::GetCol(int_max ColIndex) const
{
	DenseMatrix<ElementType_Output> tempMatrix;
    tempMatrix.Resize(this->GetRowCount(), 1);
    this->GetCol(ColIndex, tempMatrix.GetElementPointer());
    return tempMatrix;
}


template<typename ElementType>
template<typename ElementType_Output, int_max VectorFixedLength>
inline
void DenseMatrix<ElementType>::GetCol(int_max ColIndex, DenseVector<ElementType_Output, VectorFixedLength>& ColVectorData) const
{ // this is for fixed length DenseVector
	if (VectorFixedLength != this->GetRowCount())
	{
		MDK_Error("Size not match @ DenseMatrix::GetCol(ColIndex, fixed length DenseVector)")
		return;
	}
    this->GetCol(ColIndex, ColVectorData.GetElementPointer());
}


template<typename ElementType>
template<typename ElementType_Output>
inline
void DenseMatrix<ElementType>::GetCol(int_max ColIndex, DenseVector<ElementType_Output>& ColVectorData) const
{ // this is for variable length DenseVector
	ColVectorData.FastResize(this->GetRowCount());
	this->GetCol(ColIndex, ColVectorData.GetElementPointer());
}


template<typename ElementType>
template<typename ElementType_Output>
inline
void DenseMatrix<ElementType>::GetCol(int_max ColIndex, DenseMatrix<ElementType_Output>& ColVectorData) const
{
    ColVectorData.FastResize(this->GetRowCount(), 1);
    this->GetCol(ColIndex, ColVectorData.GetElementPointer());
}


template<typename ElementType>
template<typename ElementType_Output>
inline 
void DenseMatrix<ElementType>::GetCol(int_max ColIndex, ElementType_Output* ColVectorData) const
{
    auto SelfSize = this->GetSize();
    if (SelfSize.ColCount == 0)
    {
        MDK_Error("Self is empty @ DenseMatrix::GetCol(ColIndex, ElementType* ColVectorData)")
        return;
    }

    if (ColIndex >= SelfSize.ColCount || ColIndex < 0 || ColVectorData == nullptr)
	{
		MDK_Error("Invalid Input @ DenseMatrix::GetCol(int_max ColIndex, ElementType* ColVectorData)")
		return;
	}

	auto RawPointer = this->GetElementPointer();

    RawPointer += ColIndex*SelfSize.RowCount;

	for (int_max i = 0; i < SelfSize.RowCount; ++i)
	{
		ColVectorData[i] = ElementType_Output(RawPointer[i]);
	}
}


template<typename ElementType>
template<typename ElementType_Input>
inline
void DenseMatrix<ElementType>::SetCol(int_max ColIndex, const std::initializer_list<ElementType_Input>& ColVectorData)
{
    if (int_max(ColVectorData.size()) != this->GetRowCount())
    {
        MDK_Error("Invalid Input size @ DenseMatrix::SetCol(ColIndex, std::initializer_list)")
        return;
    }

    this->SetCol(ColIndex, ColVectorData.begin());
}


template<typename ElementType>
template<typename ElementType_Input, int_max TemplateVectorLength>
inline
void DenseMatrix<ElementType>::SetCol(int_max ColIndex, const DenseVector<ElementType_Input, TemplateVectorLength>& ColVectorData)
{// TemplateVectorLength is -1 for variable length DenseVector
    if (ColVectorData.GetElementCount() != this->GetRowCount())
    {
        MDK_Error("Invalid Input size @ DenseMatrix::SetCol(ColIndex, DenseVector)")
        return;
    }
    this->SetCol(ColIndex, ColVectorData.GetElementPointer());
}


template<typename ElementType>
template<typename ElementType_Input>
inline
void DenseMatrix<ElementType>::SetCol(int_max ColIndex, const DenseMatrix<ElementType_Input>& ColVectorData)
{
    if (ColVectorData.IsVector() == false)
    {
        MDK_Error("Invalid Input : must be a vector @ DenseMatrix::SetCol(ColIndex, DenseMatrix)")
        return;
    }

    if (ColVectorData.GetElementCount() != this->GetRowCount())
    {
        MDK_Error("Invalid Input size @ DenseMatrix::SetCol(ColIndex, DenseMatrix)")
        return;
    }

    this->SetCol(ColIndex, ColVectorData.GetElementPointer());
}


template<typename ElementType>
template<typename ElementType_Input>
inline 
void DenseMatrix<ElementType>::SetCol(int_max ColIndex, const ElementType_Input* ColVectorData)
{
    auto SelfSize = this->GetSize();

    if (SelfSize.ColCount == 0)
    {
        MDK_Error("Self is empty @ DenseMatrix::SetCol(ColIndex, const ElementType_Input* ColVectorData)")
        return;
    }

    if (ColIndex >= SelfSize.ColCount || ColIndex < 0 || ColVectorData == nullptr)
	{
		MDK_Error("Invalid Input @ DenseMatrix::SetCol(ColIndex, const ElementType_Input* ColVectorData)")
		return;
	}

	if (this->IsPureEmpty() == true)
	{
		this->Resize(0, 0);
	}

    // just for reference:
    // int_max Index = ColIndex*SelfSize.RowCount;
    //for (int_max i = 0; i < SelfSize.RowCount; ++i)
    //{
    //	RawPointer[Index + i] = ElementType(ColVectorData[i]);
    //}

    auto tempColData = ColVectorData;

    auto BeginPointer = this->GetElementPointer();
    BeginPointer += ColIndex*SelfSize.RowCount;

    for (auto Ptr = BeginPointer; Ptr < BeginPointer + SelfSize.RowCount; ++Ptr)
    {
        Ptr[0] = ElementType(tempColData[0]);

        ++tempColData;
    }
}


template<typename ElementType>
inline 
void DenseMatrix<ElementType>::FillCol(int_max ColIndex, const ElementType& Element)
{
    auto SelfSize = this->GetSize();

    if (SelfSize.ColCount == 0)
    {
        MDK_Error("Self is empty @ DenseMatrix::FillCol(int_max ColIndex, const ElementType& Element)")
        return;
    }

    if (ColIndex >= SelfSize.ColCount || ColIndex < 0)
    {
        MDK_Error("Invalid Input @ DenseMatrix::FillCol(int_max ColIndex, const ElementType& Element)")
        return;
    }

    auto BeginPointer = this->GetElementPointer() + ColIndex*SelfSize.RowCount;

    for (auto Ptr = BeginPointer; Ptr < BeginPointer + SelfSize.RowCount; ++Ptr)
    {
        Ptr[0] = Element;
    }
}


template<typename ElementType>
template<typename ElementType_Input>
inline 
void DenseMatrix<ElementType>::AppendCol(const std::initializer_list<ElementType_Input>& ColVectorData)
{
    this->AppendCol(ColVectorData.begin(), int_max(ColVectorData.size()));
}


template<typename ElementType>
template<typename ElementType_Input, int_max TemplateVectorLength>
inline
void DenseMatrix<ElementType>::AppendCol(const DenseVector<ElementType_Input, TemplateVectorLength>& ColVectorData)
{// TemplateVectorLength is -1 for variable length DenseVector
    this->AppendCol(ColVectorData.GetElementPointer(), ColVectorData.GetElementCount());
}


template<typename ElementType>
template<typename ElementType_Input>
inline
void DenseMatrix<ElementType>::AppendCol(const DenseMatrix<ElementType_Input>& ColVectorData)
{
    if (ColVectorData.IsVector() == false)
    {
        MDK_Error("Input must be a vector @ DenseMatrix::AppendCol(const DenseMatrix<ElementType_Input>& ColVectorData)")
        return;
    }

    this->AppendCol(ColVectorData.GetElementPointer(), ColVectorData.GetElementCount());
}


template<typename ElementType>
template<typename ElementType_Input>
inline 
void DenseMatrix<ElementType>::AppendCol(const ElementType_Input* ColVectorData, int_max Length)
{
	if (ColVectorData == nullptr || Length <= 0)
	{
		MDK_Warning("Input is empty @ DenseMatrix::AppendCol(const ElementType_Input* ColVectorData, int_max Length)")
		return;
	}

    if (this->IsSizeFixed() == true)
    {
        MDK_Error("Size can not change @ DenseMatrix::AppendCol(const ElementType_Input* ColVectorData, int_max Length)")
        return;
    }

    auto SelfSize = this->GetSize();
	auto SelfElementCount = SelfSize.RowCount*SelfSize.ColCount;

    if (SelfSize.RowCount > 0 && Length != SelfSize.RowCount)
    {
        MDK_Error("Invalid Input: size not match @ DenseMatrix::AppendCol(const ElementType_Input* ColVectorData, int_max Length)")
        return;
    }

	if (this->GetCapacity() >= SelfElementCount + SelfSize.RowCount)
	{// no memory allocation
		this->Resize(Length, SelfSize.ColCount + 1);
		this->SetCol(SelfSize.ColCount, ColVectorData);
	}
	else
	{
		// must do in this way
		// ColVectorData may point to memory of this, and it will become invalid after memory allocation
		// example:
		// DenseMatrix<double> M(3, 10);
		// M.AppendCol(M.GetPointerOf(0));
		// then, the result is Wrong!!!

		DenseMatrix<ElementType> tempMatrix;
		tempMatrix.Resize(SelfSize.RowCount, SelfSize.ColCount + 1);
		auto PointerToTemp = tempMatrix.GetElementPointer();
		auto PointerToSelf = this->GetElementPointer();	
		for (int_max k = 0; k < SelfElementCount; ++k)
		{
			PointerToTemp[k] = std::move(PointerToSelf[k]);
		}
		for (int_max k = 0; k < SelfSize.RowCount; ++k)
		{
			PointerToTemp[k + SelfElementCount] = ColVectorData[k];
		}
		this->Clear();
		this->Copy(std::move(tempMatrix));
	}
}


template<typename ElementType>
template<typename ElementType_Input>
inline
void DenseMatrix<ElementType>::AppendCol(const ElementType_Input* ColVectorData)
{
    auto RowCount = this->GetRowCount();

    if (RowCount == 0)
    {
        MDK_Error("must input RowCount because Self is empty @ DenseMatrix::AppendCol(...)")
        return;
    }

    this->AppendCol(ColVectorData, RowCount);
}


template<typename ElementType>
inline 
void DenseMatrix<ElementType>::DeleteCol(int_max ColIndex)
{
    DeleteCol(&ColIndex, 1);
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::DeleteCol(const std::initializer_list<int_max>& ColIndexList)
{
    this->DeleteCol(ColIndexList.begin(), int_max(ColIndexList.size()));
}


template<typename ElementType>
template<int_max TemplateVectorLength>
inline
void DenseMatrix<ElementType>::DeleteCol(const DenseVector<int_max, TemplateVectorLength>& ColIndexList)
{// TemplateVectorLength is -1 for variable length DenseVector
    this->DeleteCol(ColIndexList.GetElementPointer(), ColIndexList.GetElementCount());
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::DeleteCol(const DenseMatrix<int_max>& ColIndexList)
{
    if (ColIndexList.IsVector() == false)
    {
        MDK_Error("Input is NOT a vector @ DenseMatrix::DeleteCol(const DenseMatrix<int_max>& ColIndexList)")
        return;
    }

    this->DeleteCol(ColIndexList.GetElementPointer(), ColIndexList.GetElementCount());
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::DeleteCol(const int_max* ColIndexList, int_max ListLength)
{
    if (ColIndexList == nullptr || ListLength <= 0)
    {
        //MDK_Warning("Empty Input @ DenseMatrix::DeleteCol(const int_max* ColIndexList, int_max ListLength)")
        return;
    }

    if (this->IsSizeFixed() == true)
    {
        MDK_Error("Size can not change @ DenseMatrix::DeleteCol(const int_max* ColIndexList, int_max ListLength)")
        return;
    }

    auto SelfSize = this->GetSize();

    if (SelfSize.ColCount == 0)
    {
        MDK_Error("Self is empty @ DenseMatrix::DeleteCol(const int_max* ColIndexList, int_max ListLength)")
        return;
    }

    for (auto it = ColIndexList; it != ColIndexList + ListLength; ++it)
    {
        if (*it >= SelfSize.ColCount || *it < 0)
        {
            MDK_Error("Out of bound ColIndex @ DenseMatrix::DeleteCol(const int_max* ColIndexList, int_max ListLength)")
            return;
        }
    }

    m_MatrixData->CopyDataToInternalArrayIfNecessary();

	//---------------------------------------------------------------
	//note: ColIndexList may pointer to memory of this
    std::vector<int_max> ColIndexList_max_to_min(ListLength);

    for (int_max i = 0; i < ListLength; ++i)
    {
        ColIndexList_max_to_min[i] = ColIndexList[i];
    }

    std::sort(ColIndexList_max_to_min.begin(), ColIndexList_max_to_min.end(), [](int_max a, int_max b) { return a > b; });
	//-----------------------------------------------------------------

    int_max Index_prev = -1;

    for (int_max i = 0; i < int_max(ColIndexList_max_to_min.size()); ++i)
    {
        auto Index_i = ColIndexList_max_to_min[i];

        if (Index_i == Index_prev)
        {
            MDK_Warning("duplicate Input @ DenseMatrix::DeleteCol(const int_max* ColIndexPtr, int_max ListLength)")
        }
        else
        {
            m_MatrixData->StdVector.erase(m_MatrixData->StdVector.begin() + Index_i * SelfSize.RowCount,
                                          m_MatrixData->StdVector.begin() + (Index_i + 1)* SelfSize.RowCount);

            Index_prev = Index_i;

            m_MatrixData->ColCount -= 1;
        }
    }

    m_MatrixData->ElementPointer = m_MatrixData->StdVector.data();
}


template<typename ElementType>
template<typename ElementType_Input>
inline
void DenseMatrix<ElementType>::InsertCol(int_max ColIndex, const std::initializer_list<ElementType_Input>& ColVectorData)
{
    this->InsertCol(ColIndex, ColVectorData.begin(), int_max(ColVectorData.size()));
}


template<typename ElementType>
template<typename ElementType_Input, int_max TemplateVectorLength>
inline
void DenseMatrix<ElementType>::InsertCol(int_max ColIndex, const DenseVector<ElementType_Input, TemplateVectorLength>& ColVectorData)
{// TemplateVectorLength is -1 for variable length DenseVector
    this->InsertCol(ColIndex, ColVectorData.GetElementPointer(), ColVectorData.GetElementCount());
}


template<typename ElementType>
template<typename ElementType_Input>
inline
void DenseMatrix<ElementType>::InsertCol(int_max ColIndex, const DenseMatrix<ElementType_Input>& ColVectorData)
{
    if (ColVectorData.IsVector() == false)
    {
        MDK_Error("Input is NOT a vector @ DenseMatrix::InsertCol(ColIndex, DenseMatrix)")
        return;
    }

    this->InsertCol(ColIndex, ColVectorData.GetElementPointer(), ColVectorData.GetElementCount());
}


template<typename ElementType>
template<typename ElementType_Input>
inline
void DenseMatrix<ElementType>::InsertCol(int_max ColIndex, const ElementType_Input* ColVectorData, int_max Length)
{
    if (this->IsSizeFixed() == true)
    {
        MDK_Error("Matrix Size can not change @ DenseMatrix::InsertCol(ColIndex, const ElementType_Input* ColVectorData, int_max Length)")
        return;
    }

    auto SelfSize = this->GetSize();

    if (SelfSize.ColCount == 0)
    {
        if (ColIndex != 0 || ColVectorData == nullptr || Length <= 0)
        {
            MDK_Error("Invalid Input @ DenseMatrix::InsertCol(ColIndex, const ElementType_Input* ColVectorData, int_max Length)")
            return;
        }
    }
    else
    {
        if (ColIndex >= SelfSize.ColCount || ColIndex < 0 || ColVectorData == nullptr || Length != SelfSize.RowCount || Length <= 0)
        {
            MDK_Error("Invalid Input @ DenseMatrix::InsertCol(ColIndex, const ElementType_Input* ColVectorData, int_max Length)")
            return;
        }
    }

	if (this->IsPureEmpty() == true)
	{
		this->Resize(0, 0);
	}

	m_MatrixData->CopyDataToInternalArrayIfNecessary();

	if (this->GetCapacity() >= this->GetElementCount() + SelfSize.RowCount)
	{// no new memory allocation
		m_MatrixData->StdVector.insert(m_MatrixData->StdVector.begin() + ColIndex*SelfSize.RowCount, ColVectorData, ColVectorData + Length);
	}
	else
	{
		// if ColVectorData point to memory of this, it will be invalid after memory allocation

		DenseVector<ElementType> tempColVector;
		tempColVector.Resize(SelfSize.RowCount);
		auto PointerToTemp = tempColVector.GetElementPointer();		
		for (int_max k = 0; k < SelfSize.RowCount; ++k)
		{
			PointerToTemp[k] = ColVectorData[k];
		}
		m_MatrixData->StdVector.insert(m_MatrixData->StdVector.begin() + ColIndex*SelfSize.RowCount, PointerToTemp, PointerToTemp + Length);
	}
	m_MatrixData->RowCount = Length;
	m_MatrixData->ColCount += 1;
	m_MatrixData->ElementPointer = m_MatrixData->StdVector.data();
}


template<typename ElementType>
template<typename ElementType_Input>
inline
void DenseMatrix<ElementType>::InsertCol(int_max ColIndex, const ElementType_Input* ColVectorData)
{
    auto RowCount = this->GetRowCount();
    if (RowCount == 0)
    {
        MDK_Error("must input RowCount because Self is empty @ DenseMatrix::InsertCol(...)")
        return;
    }
    this->InsertCol(ColIndex, ColVectorData, RowCount);
}


template<typename ElementType>
template<typename ElementType_Output>
inline
DenseMatrix<ElementType_Output> DenseMatrix<ElementType>::GetRow(int_max RowIndex) const
{
	DenseMatrix<ElementType_Output> tempMatrix;
    tempMatrix.Resize(1, this->GetColCount());
    this->GetRow(RowIndex, tempMatrix.GetElementPointer());
    return tempMatrix;
}


template<typename ElementType>
template<typename ElementType_Output, int_max VectorFixedLength>
inline
void DenseMatrix<ElementType>::GetRow(int_max RowIndex, DenseVector<ElementType_Output, VectorFixedLength>& RowVectorData) const
{// this is for fixed length DenseVector
	if (VectorFixedLength != this->GetColCount())
	{
		MDK_Error("Size not match @ DenseMatrix::GetRow(RowIndex, ixed length DenseVector)")
	}
    this->GetRow(RowIndex, RowVectorData.GetElementPointer());
}


template<typename ElementType>
template<typename ElementType_Output>
inline
void DenseMatrix<ElementType>::GetRow(int_max RowIndex, DenseVector<ElementType_Output>& RowVectorData) const
{// this is  for variable length DenseVector
	RowVectorData.FastResize(this->GetColCount());
	this->GetRow(RowIndex, RowVectorData.GetElementPointer());
}


template<typename ElementType>
template<typename ElementType_Output>
inline
void DenseMatrix<ElementType>::GetRow(int_max RowIndex, DenseMatrix<ElementType_Output>& RowVectorData) const
{
    RowVectorData.FastResize(1, this->GetColCount());
    this->GetRow(RowIndex, RowVectorData.GetElementPointer());
}


template<typename ElementType>
template<typename ElementType_Output>
inline
void DenseMatrix<ElementType>::GetRow(int_max RowIndex, ElementType_Output* RowVectorData) const
{
    auto SelfSize = this->GetSize();

    if (SelfSize.RowCount == 0)
    {
        MDK_Error("Self is empty @ DenseMatrix GetRow(RowIndex, ElementType* RowVectorData)")
        return;
    }

    if (RowIndex >= SelfSize.RowCount || RowIndex < 0 || RowVectorData == nullptr)
	{
		MDK_Error("Invalid Input @ DenseMatrix GetRow(RowIndex, ElementType* RowVectorData)")
		return;
	}

	auto RawPointer = this->GetElementPointer();

	int_max Index = 0;

	for (int_max j = 0; j < SelfSize.ColCount; ++j)
	{
		RowVectorData[j] = ElementType_Output(RawPointer[Index + RowIndex]);
		Index += SelfSize.RowCount;
	}
}


template<typename ElementType>
template<typename ElementType_Input>
inline
void DenseMatrix<ElementType>::SetRow(int_max RowIndex, const std::initializer_list<ElementType_Input>& RowVectorData)
{
    if (int_max(RowVectorData.size()) != this->GetColCount())
    {
        MDK_Error("Input size is wrong @ DenseMatrix::SetRow(int_max RowIndex, std::initializer_list)")
        return;
    }
    this->SetRow(RowIndex, RowVectorData.begin());
}


template<typename ElementType>
template<typename ElementType_Input, int_max TemplateVectorLength>
inline
void DenseMatrix<ElementType>::SetRow(int_max RowIndex, const DenseVector<ElementType_Input, TemplateVectorLength>& RowVectorData)
{//TemplateVectorLength is -1 for variable length DenseVector
    if (RowVectorData.GetElementCount() != this->GetColCount())
    {
        MDK_Error("Input size is wrong @ DenseMatrix::SetRow(int_max RowIndex, DenseVector)")
        return;
    }
    this->SetRow(RowIndex, RowVectorData.GetElementPointer());
}


template<typename ElementType>
template<typename ElementType_Input>
inline
void DenseMatrix<ElementType>::SetRow(int_max RowIndex, const DenseMatrix<ElementType_Input>& RowVectorData)
{
    if (RowVectorData.IsVector() == false)
    {
        MDK_Error("Input must be a vector @ DenseMatrix::SetRow(int_max RowIndex, DenseMatrix)")
        return;
    }

    if (RowVectorData.GetElementCount() != this->GetColCount())
    {
        MDK_Error("Input size is wrong @ DenseMatrix::SetRow(int_max RowIndex, DenseMatrix)")
        return;
    }

    this->SetRow(RowIndex, RowVectorData.GetElementPointer());
}


template<typename ElementType>
template<typename ElementType_Input>
inline
void DenseMatrix<ElementType>::SetRow(int_max RowIndex, const ElementType_Input* RowVectorData)
{
    auto SelfSize = this->GetSize();

    if (SelfSize.RowCount == 0)
    {
        MDK_Error("Self is empty @ DenseMatrix::SetRow(int_max RowIndex, const ElementType_Input* RowVectorData)")
        return;
    }

    if (RowIndex >= SelfSize.RowCount || RowIndex < 0 || RowVectorData == nullptr)
    {
        MDK_Error("Invalid Input @ DenseMatrix::SetRow(RowIndex, const ElementType_Input* RowVectorData)")
        return;
    }

    auto RawPointer = this->GetElementPointer();

    int_max Index = 0;

    for (int_max j = 0; j < SelfSize.ColCount; ++j)
    {
        RawPointer[Index + RowIndex] = ElementType(RowVectorData[j]);

        Index += SelfSize.RowCount;
     }
}


template<typename ElementType>
inline 
void DenseMatrix<ElementType>::FillRow(int_max RowIndex, const ElementType& Element)
{
    auto SelfSize = this->GetSize();

    if (SelfSize.RowCount == 0)
    {
        MDK_Error("Self is empty @ DenseMatrix::FillRow(int_max RowIndex, const ElementType& Element)")
        return;
    }

    if (RowIndex >= SelfSize.RowCount || RowIndex < 0)
    {
        MDK_Error("Invalid Input @ DenseMatrix::FillRow(int_max RowIndex, const ElementType& Element)")
        return;
    }

    auto RawPointer = this->GetElementPointer();

    int_max Index = 0;

    for (int_max j = 0; j < SelfSize.ColCount; ++j)
    {
        RawPointer[Index + RowIndex] = Element;

        Index += SelfSize.RowCount;
    }
}


template<typename ElementType>
template<typename ElementType_Input>
inline
void DenseMatrix<ElementType>::AppendRow(const std::initializer_list<ElementType_Input>& RowVectorData)
{
    this->AppendRow(RowVectorData.begin(), int_max(RowVectorData.size()));
}


template<typename ElementType>
template<typename ElementType_Input, int_max TemplateVectorLength>
inline
void DenseMatrix<ElementType>::AppendRow(const DenseVector<ElementType_Input, TemplateVectorLength>& RowVectorData)
{// TemplateVectorLength is -1 for variable length DenseVector
    this->AppendRow(RowVectorData.GetElementPointer(), RowVectorData.GetElementCount());
}


template<typename ElementType>
template<typename ElementType_Input>
inline
void DenseMatrix<ElementType>::AppendRow(const DenseMatrix<ElementType_Input>& RowVectorData)
{
    if (RowVectorData.IsVector() == false)
    {
        MDK_Error("Input must be a vector @ DenseMatrix::AppendRow(const DenseMatrix<ElementType_Input>& RowVectorData)")
        return;
    }

    this->AppendRow(RowVectorData.GetElementPointer(), RowVectorData.GetElementCount());
}


template<typename ElementType>
template<typename ElementType_Input>
inline
void DenseMatrix<ElementType>::AppendRow(const ElementType_Input* RowVectorData, int_max Length)
{
	if (RowVectorData == nullptr || Length <= 0)
	{
		MDK_Warning("Input is empty @ DenseMatrix::AppendRow(const ElementType_Input* RowVectorData, int_max Length)")
		return;
	}

    if (this->IsSizeFixed() == true)
    {
        MDK_Error("Size can not change @ DenseMatrix::AppendRow(const ElementType_Input* RowVectorData, int_max Length)")
        return;
    }

    auto SelfSize = this->GetSize();

    if (SelfSize.ColCount > 0 && Length != SelfSize.ColCount)
    {
        MDK_Error("Invalid Input: size does not match @ DenseMatrix::AppendRow(const ElementType_Input* RowVectorData, int_max Length)")
        return;
    }

	// RowVectorData may point to memory of this
	// even capacity is enough, resize will re-arrange the element then RowVectorData will be invalid

	DenseVector<ElementType> tempColVector;
	tempColVector.Resize(SelfSize.ColCount);
	auto PointerToTemp = tempColVector.GetElementPointer();
	for (int_max k = 0; k < SelfSize.ColCount; ++k)
	{
		PointerToTemp[k] = RowVectorData[k];
	}

	this->Resize(SelfSize.RowCount + 1, Length);
	this->SetRow(SelfSize.RowCount, PointerToTemp);
}


template<typename ElementType>
template<typename ElementType_Input>
inline
void DenseMatrix<ElementType>::AppendRow(const ElementType_Input* RowVectorData)
{
    auto ColCount = this->GetColCount();

    if (ColCount == 0)
    {
        MDK_Error("must input ColCount because Self is empty @ DenseMatrix::AppendRow(...)")
        return;
    }

    this->AppendRow(RowVectorData, ColCount);
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::DeleteRow(int_max RowIndex)
{
    this->DeleteRow(&RowIndex, 1);
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::DeleteRow(const std::initializer_list<int_max>& RowIndexList)
{
    this->DeleteRow(RowIndexList.begin(), int_max(RowIndexList.size()));
}


template<typename ElementType>
template<int_max TemplateVectorLength>
inline
void DenseMatrix<ElementType>::DeleteRow(const DenseVector<int_max, TemplateVectorLength>& RowIndexList)
{// TemplateVectorLength is -1 for variable length DenseVector
    this->DeleteRow(RowIndexList.GetElementPointer(), RowIndexList.GetElementCount());
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::DeleteRow(const DenseMatrix<int_max>& RowIndexList)
{
    if (RowIndexList.IsVector() == false)
    {
        MDK_Error("Input must be a vector @ DenseMatrix::DeleteRow(const DenseMatrix<int_max>& RowIndexList)")
        return;
    }

    this->DeleteRow(RowIndexList.GetElementPointer(), RowIndexList.GetElementCount());
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::DeleteRow(const int_max* RowIndexList, int_max ListLength)
{
    if (RowIndexList == nullptr || ListLength <= 0)
    {
        //MDK_Warning("Empty Input @ DenseMatrix::DeleteRow(const int_max* RowIndexList, int_max ListLength)")
        return;
    }

    if (this->IsSizeFixed() == true)
    {
        MDK_Error("Size can not change @ DenseMatrix::DeleteRow(const int_max* RowIndexList, int_max ListLength)")
        return;
    }

    auto SelfSize = this->GetSize();

    if (SelfSize.RowCount == 0)
    {
        MDK_Error("Self is empty @ DenseMatrix::DeleteRow(const int_max* RowIndexList, int_max ListLength)")
        return;
    }

    for (auto it = RowIndexList; it != RowIndexList + ListLength; ++it)
    {
        if (*it >= SelfSize.RowCount || *it < 0)
        {
            MDK_Error("Invalid Input @ DenseMatrix::DeleteRow(const int_max* RowIndexList, int_max ListLength)")
            return;
        }
    }

	// RowIndexList may pointer to memory of this

    DenseVector<int_max> CounterList(SelfSize.RowCount);
	CounterList.Fill(0);
    for (int_max i = 0; i < ListLength; ++i)
    {
        CounterList[RowIndexList[i]] = 1;
    }

	DenseVector<int_max> RowIndexList_output;
    for (int_max i = 0; i < SelfSize.RowCount; ++i)
    {
        if (CounterList[i] <= 0)
        {
            RowIndexList_output.Append(i);
        }
    }

    if (RowIndexList_output.GetElementCount() > 0)
    {
        this->Copy(this->GetSubMatrix(RowIndexList_output, ALL));
    }
    else
    {
        this->Clear();
    }
}


template<typename ElementType>
template<typename ElementType_Input>
inline
void DenseMatrix<ElementType>::InsertRow(int_max RowIndex, const std::initializer_list<ElementType_Input>& RowVectorData)
{
    this->InsertRow(RowIndex, RowVectorData.begin(), int_max(RowVectorData.size()));
}


template<typename ElementType>
template<typename ElementType_Input, int_max TemplateVectorLength>
inline
void DenseMatrix<ElementType>::InsertRow(int_max RowIndex, const DenseVector<ElementType_Input, TemplateVectorLength>& RowVectorData)
{// TemplateVectorLength is -1 for variable length DenseVector
    this->InsertRow(RowIndex, RowVectorData.GetElementPointer(), RowVectorData.GetElementCount());
}


template<typename ElementType>
template<typename ElementType_Input>
inline
void DenseMatrix<ElementType>::InsertRow(int_max RowIndex, const DenseMatrix<ElementType_Input>& RowVectorData)
{
    if (RowVectorData.IsVector() == false)
    {
        MDK_Error("Input is NOT a vector @ DenseMatrix::InsertRow(int_max RowIndex, const DenseMatrix<ElementType_Input>& RowVectorData)")
        return;
    }

    this->InsertRow(RowIndex, RowVectorData.GetElementPointer(), RowVectorData.GetElementCount());
}


template<typename ElementType>
template<typename ElementType_Input>
inline
void DenseMatrix<ElementType>::InsertRow(int_max RowIndex, const ElementType_Input* RowVectorData, int_max Length)
{
    if (this->IsSizeFixed() == true)
    {
        MDK_Error("Size can not change @ DenseMatrix::InsertRow(RowIndex, const ElementType_Input* RowVectorData, int_max Length)")
        return;
    }

    if (this->IsEmpty() == true)
    {
        if (RowIndex == 0 && RowVectorData != nullptr && Length > 0)
        {
            this->AppendRow(RowVectorData, Length);
        }
		else
		{
			MDK_Error("Invalid Input @ DenseMatrix::InsertRow(RowIndex, const ElementType_Input* RowVectorData, int_max Length)")
		}
        return;
    }

    // Self is not empty ----------------------------------------------------------------------------------------------------------

    auto SelfSize = this->GetSize();

    if (RowIndex >= SelfSize.RowCount || RowIndex < 0 || RowVectorData == nullptr || Length != SelfSize.ColCount)
    {
        MDK_Error("Invalid Input @ DenseMatrix::InsertRow(RowIndex, const ElementType_Input* RowVectorData, int_max Length)")
        return;
    }

    DenseMatrix<ElementType> tempMatrix(SelfSize.RowCount + 1, SelfSize.ColCount);

    auto tempRawPointer = tempMatrix.GetElementPointer();

    auto RawPointer = this->GetElementPointer();

    for (int_max j = 0; j < SelfSize.ColCount; ++j)
    {
        auto tempIndex = j*SelfSize.RowCount;

        for (int_max i = 0; i < RowIndex; ++i)
        {
            tempRawPointer[tempIndex + j + i] = RawPointer[tempIndex + i];
        }
    }

    for (int_max j = 0; j < SelfSize.ColCount; ++j)
    {
        tempRawPointer[j*(SelfSize.RowCount + 1) + RowIndex] = ElementType(RowVectorData[j]);
    }

    for (int_max j = 0; j < SelfSize.ColCount; ++j)
    {
        auto tempIndex = j*SelfSize.RowCount;

        for (int_max i = RowIndex + 1; i < SelfSize.RowCount + 1; ++i)
        {            
            tempRawPointer[tempIndex + j + i] = RawPointer[tempIndex + i - 1];
        }
    }
    //---------------------------------------------------
    this->Copy(std::move(tempMatrix));
}


template<typename ElementType>
template<typename ElementType_Input>
inline
void DenseMatrix<ElementType>::InsertRow(int_max RowIndex, const ElementType_Input* RowVectorData)
{
    auto ColCount = this->GetColCount();
    if (ColCount == 0)
    {
        MDK_Error("must input ColCount because Self is empty @ DenseMatrix::InsertRow(...)")
        return;
    }
    this->InsertRow(RowIndex, RowVectorData, ColCount);
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::Append(ElementType Element)
{
    if (this->IsEmpty() == true)
    {
        this->Resize(1, 1);
        (*this)[0] = std::move(Element);
        return;
    }

    auto SelfSize = this->GetSize();

	if (SelfSize.RowCount == 1)// RowVector
    {
        this->Resize(1, SelfSize.ColCount + 1);
        (*this)[SelfSize.ColCount] = std::move(Element);
    }
	else if (SelfSize.ColCount == 1)//ColVector
    {
        this->Resize(SelfSize.RowCount + 1, 1);
        (*this)[SelfSize.RowCount] = std::move(Element);
    }
	else
	{
		MDK_Error("Self is not empty and not a vector @ DenseMatrix::Append(Element)")
	}
}


template<typename ElementType>
template<typename ElementType_Input>
inline
void DenseMatrix<ElementType>::Append(const std::initializer_list<ElementType_Input>& ElementData)
{
    if (this->IsEmpty() == true)
    {
        this->AppendRow(ElementData);
		return;
    }

    if (this->IsRowVector() == true)
    {
        this->AppendCol(ElementData);
    }
    else if (this->IsColVector() == true)
    {
        this->AppendRow(ElementData);
    }
    else
    {
        MDK_Error("Self is not a vector @ DenseMatrix::Append(...)")
    }
}


template<typename ElementType>
template<typename ElementType_Input, int_max TemplateVectorLength>
inline
void DenseMatrix<ElementType>::Append(const DenseVector<ElementType_Input, TemplateVectorLength>& ElementData)
{// TemplateVectorLength is -1 for variable length DenseVector
    if (this->IsEmpty() == true)
    {
        this->AppendRow(ElementData);
		return;
    }

    if (this->IsRowVector() == true)
    {
        this->AppendCol(ElementData);
    }
    else if (this->IsColVector() == true)
    {
        this->AppendRow(ElementData);
    }
    else
    {
        MDK_Error("Self is not a vector @ DenseMatrix::Append(...)")
    }
}


template<typename ElementType>
template<typename ElementType_Input>
inline
void DenseMatrix<ElementType>::Append(const DenseMatrix<ElementType_Input>& ElementData)
{
    if (this->IsEmpty() == true)
    {
        this->AppendRow(ElementData);
		return;
    }

    if (this->IsRowVector() == true)
    {
        this->AppendCol(ElementData);
    }
    else if (this->IsColVector() == true)
    {
        this->AppendRow(ElementData);
    }
    else
    {
        MDK_Error("Self is not a vector @ DenseMatrix::Append(...)")
    }
}


template<typename ElementType>
template<typename ElementType_Input>
inline
void DenseMatrix<ElementType>::Append(const ElementType_Input* ElementData, int_max Length)
{
    if (this->IsEmpty() == true)
    {
        this->AppendRow(ElementData, Length);
		return;
    }

    if (this->IsRowVector() == true)
    {
        this->AppendCol(ElementData, Length);
    }
    else if (this->IsColVector() == true)
    {
        this->AppendRow(ElementData, Length);
    }
    else
    {
        MDK_Error("Self is not a vector @ DenseMatrix::Append(...)")
    }
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::Delete(int_max LinearIndex)
{
    if (this->IsRowVector() == true)
    {
        this->DeleteCol(LinearIndex);
    }
    else if (this->IsColVector() == true)
    {
        this->DeleteRow(LinearIndex);
    }
    else
    {
        MDK_Error("Self is not a vector @ DenseMatrix::Delete(...)")
    }
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::Delete(const std::initializer_list<int_max>& LinearIndexList)
{
    if (this->IsRowVector() == true)
    {
        this->DeleteCol(LinearIndexList);
    }
    else if (this->IsColVector() == true)
    {
        this->DeleteRow(LinearIndexList);
    }
    else
    {
        MDK_Error("Self is not a vector @ DenseMatrix::Delete(...)")
    }
}


template<typename ElementType>
template<int_max TemplateVectorLength>
inline
void DenseMatrix<ElementType>::Delete(const DenseVector<int_max, TemplateVectorLength>& LinearIndexList)
{// TemplateVectorLength is -1 for variable length DenseVector
    if (this->IsRowVector() == true)
    {
        this->DeleteCol(LinearIndexList);
    }
    else if (this->IsColVector() == true)
    {
        this->DeleteRow(LinearIndexList);
    }
    else
    {
		if (LinearIndexList.IsEmpty() == false)
		{
			MDK_Error("Self is not a vector @ DenseMatrix::Delete(...)")
		}
    }
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::Delete(const DenseMatrix<int_max>& LinearIndexList)
{
    if (this->IsRowVector() == true)
    {
        this->DeleteCol(LinearIndexList);
    }
    else if (this->IsColVector() == true)
    {
        this->DeleteRow(LinearIndexList);
    }
    else
    {
		if (LinearIndexList.IsEmpty() == false)
		{
			MDK_Error("Self is not a vector @ DenseMatrix::Delete(...)")
		}
    }
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::Delete(const int_max* LinearIndexList, int_max ListLength)
{
    if (LinearIndexList == nullptr || Length <= 0)
    {
        //MDK_Warning("Empty input @ DenseMatrix::Delete(...)")
        return;
    }

    if (this->IsRowVector() == true)
    {
        this->DeleteCol(LinearIndexList, int_max ListLength);
    }
    else if (this->IsColVector() == true)
    {
        this->DeleteRow(LinearIndexList, int_max ListLength);
    }
    else
    {
        MDK_Error("Self is not a vector @ DenseMatrix::Delete(...)")
    }
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::Insert(int_max LinearIndex, ElementType Element)
{
    if (this->IsRowVector() == true)
    {
        this->InsertCol(LinearIndex, { std::move(Element) });
    }
    else if (this->IsColVector() == true)
    {
		this->InsertRow(LinearIndex, { std::move(Element) });
    }
    else
    {
        MDK_Error("Self is not a vector @ DenseMatrix::Insert(...)")
    }
}


template<typename ElementType>
template<typename ElementType_Input>
inline
void DenseMatrix<ElementType>::Insert(int_max LinearIndex, const std::initializer_list<ElementType_Input>& ElementData)
{
    if (this->IsRowVector() == true)
    {
        this->InsertCol(LinearIndex, ElementData);
    }
    else if (this->IsColVector() == true)
    {
        this->InsertRow(LinearIndex, ElementData);
    }
    else
    {
        MDK_Error("Self is not a vector @ DenseMatrix::Insert(...)")
    }
}


template<typename ElementType>
template<typename ElementType_Input, int_max TemplateVectorLength>
inline
void DenseMatrix<ElementType>::Insert(int_max LinearIndex, const DenseVector<ElementType_Input, TemplateVectorLength>& ElementData)
{// TemplateVectorLength is -1 for variable length DenseVector
    if (this->IsRowVector() == true)
    {
        this->InsertCol(LinearIndex, ElementData);
    }
    else if (this->IsColVector() == true)
    {
        this->InsertRow(LinearIndex, ElementData);
    }
    else
    {
        MDK_Error("Self is not a vector @ DenseMatrix::Insert(...)")
    }
}


template<typename ElementType>
template<typename ElementType_Input>
inline
void DenseMatrix<ElementType>::Insert(int_max LinearIndex, const DenseMatrix<ElementType_Input>& ElementData)
{
    if (this->IsRowVector() == true)
    {
        this->InsertCol(LinearIndex, ElementData);
    }
    else if (this->IsColVector() == true)
    {
        this->InsertRow(LinearIndex, ElementData);
    }
    else
    {
        MDK_Error("Self is not a vector @ DenseMatrix::Insert(...)")
    }
}


template<typename ElementType>
template<typename ElementType_Input>
inline
void DenseMatrix<ElementType>::Insert(int_max LinearIndex, const ElementType_Input* ElementData, int_max Length)
{
    if (this->IsRowVector() == true)
    {
        this->InsertCol(LinearIndex, ElementData, Length);
    }
    else if (this->IsColVector() == true)
    {
        this->InsertRow(LinearIndex, ElementData, Length);
    }
    else
    {
        MDK_Error("Self is not a vector @ DenseMatrix::Insert(...)")
    }
}


template<typename ElementType>
template<typename ElementType_Output>
inline
DenseMatrix<ElementType_Output> DenseMatrix<ElementType>::GetDiagonal() const
{
	DenseMatrix<ElementType_Output> tempMatrix;

	auto SelfSize = this->GetSize();

	if (SelfSize.RowCount <= 0 || SelfSize.RowCount != SelfSize.ColCount)
	{
		MDK_Error(" Self is empty or not square @ DenseMatrix::GetDiagonal()")
		return  tempMatrix;
	}

	tempMatrix.Resize(SelfSize.RowCount, 1);

	auto tempRawPointer = tempMatrix.GetElementPointer();

	this->GetDiagonal(tempRawPointer);

	return tempMatrix;
}


template<typename ElementType>
template<typename ElementType_Output, int_max VectorFixedLength>
inline
void DenseMatrix<ElementType>::GetDiagonal(DenseVector<ElementType_Output, VectorFixedLength>& DiagonalData) const
{
	auto SelfSize = this->GetSize();

    if (SelfSize.RowCount <= 0 || SelfSize.RowCount != SelfSize.ColCount)
    {
        MDK_Error(" Self is empty or not square @ DenseMatrix::GetDiagonal(DenseVector)")
        return;
    }

	if (DiagonalData.GetElementCount() != SelfSize.RowCount)
	{
		MDK_Error(" Size does not match @ DenseMatrix::GetDiagonal(DenseVector)")
		return;
	}

	this->GetDiagonal(DiagonalData.GetElementPointer());
}


template<typename ElementType>
template<typename ElementType_Output>
inline
void DenseMatrix<ElementType>::GetDiagonal(DenseVector<ElementType_Output>& DiagonalData) const
{
    auto SelfSize = this->GetSize();

    if (SelfSize.RowCount <= 0 || SelfSize.RowCount != SelfSize.ColCount)
    {
        MDK_Error(" Self is empty or not square @ DenseMatrix::GetDiagonal(DenseVector)")
        return;
    }

    DiagonalData.FastResize(SelfSize.RowCount);

	this->GetDiagonal(DiagonalData.GetElementPointer());
}


template<typename ElementType>
template<typename ElementType_Output>
inline
void DenseMatrix<ElementType>::GetDiagonal(DenseMatrix<ElementType_Output>& DiagonalData) const
{
    auto SelfSize = this->GetSize();

    if (SelfSize.RowCount <= 0 || SelfSize.RowCount != SelfSize.ColCount)
    {
        MDK_Error(" Self is empty or not square @ DenseMatrix::GetDiagonal(DenseMatrix&)")
        return ;
    }

    DiagonalData.FastResize(1, SelfSize.RowCount);

    auto outputRawPointer = DiagonalData.GetElementPointer();

    this->GetDiagonal(outputRawPointer);
}


template<typename ElementType>
template<typename ElementType_Output>
inline
void DenseMatrix<ElementType>::GetDiagonal(ElementType_Output* DiagonalData) const
{
    if (DiagonalData == nullptr)
    {
        MDK_Error(" Input is nullptr @ DenseMatrix::GetDiagonal(ElementType* DiagonalData)")
        return;
    }

    auto SelfSize = this->GetSize();

    if (SelfSize.RowCount <= 0 || SelfSize.RowCount != SelfSize.ColCount)
    {
        MDK_Error(" Self is empty or not square @ DenseMatrix::GetDiagonal(ElementType* DiagonalData)")
        return;
    }

    auto RawPointer = this->GetElementPointer();

    int_max Index = 0;

    for (int_max i = 0; i < SelfSize.RowCount; ++i)
    {
		DiagonalData[i] = ElementType_Output(RawPointer[Index + i]);
        Index += SelfSize.RowCount;
    }
}


template<typename ElementType>
template<typename ElementType_Input>
inline
void DenseMatrix<ElementType>::SetDiagonal(const std::initializer_list<ElementType_Input>& DiagonalData)
{
    auto SelfSize = this->GetSize();

    if (SelfSize.RowCount <= 0 || SelfSize.RowCount != SelfSize.ColCount)
    {
        MDK_Error("Self is empty or not square @ DenseMatrix::SetDiagonal(std::initializer_list)")
        return;
    }

    if (DiagonalData.size() != SelfSize.RowCount)
    {
        MDK_Error("Invalid Input @ DenseMatrix::SetDiagonal(std::initializer_list)")
        return;
    }

    this->SetDiagonal(DiagonalData.begin());
}


template<typename ElementType>
template<typename ElementType_Input, int_max TemplateVectorLength>
inline
void DenseMatrix<ElementType>::SetDiagonal(const DenseVector<ElementType_Input, TemplateVectorLength>& DiagonalData)
{// TemplateVectorLength is -1 for variable length DenseVector
    auto InputLength = DiagonalData.GetLength();
    if (InputLength <= 0)
    {
        MDK_Error("Empty input @ DenseMatrix::SetDiagonal(DenseVector)")
		return;
    }
	else if (InputSize == 1)
    {
        MDK_Warning("Input matrix is 1x1 and treated as a scalar @ DenseMatrix::SetDiagonal(DenseVector)")
        this->FillDiagonal(ElementType(DiagonalData(0)));
    }
    else
    {
        this->SetDiagonal(DiagonalData.GetElementPointer());
    }
}


template<typename ElementType>
template<typename ElementType_Input>
inline
void DenseMatrix<ElementType>::SetDiagonal(const DenseMatrix<ElementType_Input>& DiagonalData)
{
    auto InputSize = DiagonalData.GetSize();

    if (InputSize.ColCount != 1 && InputSize.RowCount != 1)
    {
        MDK_Error("Invalid Input @ DenseMatrix::SetDiagonal(DenseMatrix)")
        return;
    }

    auto SelfSize = this->GetSize();

    if (InputSize.RowCount > 1 && InputSize.RowCount != SelfSize.ColCount)
    {
        MDK_Error("Invalid Input @ DenseMatrix::SetDiagonal(DenseMatrix)")
        return;
    }

    if (InputSize.ColCount > 1 && InputSize.ColCount != SelfSize.ColCount)
    {
        MDK_Error("Invalid Input @ DenseMatrix::SetDiagonal(Matrix)")
        return;
    }

    if (InputSize.RowCount == 1 && InputSize.ColCount == 1)
    {
        MDK_Warning("Input matrix is 1x1 and treated as a scalar @ DenseMatrix::SetDiagonal(Matrix)")
        this->FillDiagonal(ElementType(DiagonalData(0)));
    }
    else
    {
        this->SetDiagonal(DiagonalData.GetElementPointer());
    }
}


template<typename ElementType>
template<typename ElementType_Input>
inline
void DenseMatrix<ElementType>::SetDiagonal(const ElementType_Input* DiagonalData)
{
    if (DiagonalData == nullptr)
    {
        MDK_Error(" Input is nullptr @ DenseMatrix::SetDiagonal(const ElementType_Input* DiagonalData)")
        return;
    }

    auto SelfSize = this->GetSize();

    if (SelfSize.RowCount <= 0 || SelfSize.RowCount != SelfSize.ColCount)
    {
        MDK_Error(" Self is empty or not square @ DenseMatrix::SetDiagonal(const ElementType_Input* DiagonalData)")
        return;
    }

    auto RawPointer = this->GetElementPointer();

    int_max Index = 0;

    for (int_max i = 0; i < SelfSize.RowCount; ++i)
    {
        RawPointer[Index + i] = ElementType(DiagonalData[i]);

        Index += SelfSize.RowCount;
    }
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::FillDiagonal(const ElementType& Element)
{
    auto SelfSize = this->GetSize();

    if (SelfSize.RowCount <= 0 || SelfSize.RowCount != SelfSize.ColCount)
    {
        MDK_Error(" Self is empty or not square @ DenseMatrix::FillDiagonal")
        return;
    }

    auto RawPointer = this->GetElementPointer();

    int_max Index = 0;

    for (int_max i = 0; i < SelfSize.RowCount; ++i)
    {
        RawPointer[Index + i] = Element;

        Index += SelfSize.RowCount;
    }
}


//=========================================== For Reference ===============================================================//
/*

// --------------------------------------- Matrix {+ - * /}  Matrix ------------------------------------------------//

template<typename ElementType>
inline	
mdkDenseMatrix<ElementType> operator+(const mdkDenseMatrix<ElementType>& MatrixA, const mdkDenseMatrix<ElementType>& MatrixB)
{
    return MatrixAdd(MatrixA, MatrixB);
}


template<typename ElementType>
inline
mdkDenseMatrix<ElementType> operator-(const mdkDenseMatrix<ElementType>& MatrixA, const mdkDenseMatrix<ElementType>& MatrixB)
{
    return MatrixSubtract(MatrixA, MatrixB);
}


template<typename ElementType>
inline
mdkDenseMatrix<ElementType> operator*(const mdkDenseMatrix<ElementType>& MatrixA, const mdkDenseMatrix<ElementType>& MatrixB)
{
    return MatrixMultiply(MatrixA, MatrixB);
}


template<typename ElementType>
inline
mdkDenseMatrix<ElementType> operator/(const mdkDenseMatrix<ElementType>& MatrixA, const mdkDenseMatrix<ElementType>& MatrixB)
{
    return MatrixElementDivide(MatrixA, MatrixB);
}

// ----------------------- Matrix {+ - * /}  Element ------------------------------------------------//

template<typename ElementType>
inline mdkDenseMatrix<ElementType> operator+(const mdkDenseMatrix<ElementType>& MatrixA, const ElementType& ElementB)
{
    return MatrixAdd(MatrixA, ElementB);
}


template<typename ElementType>
inline mdkDenseMatrix<ElementType> operator-(const mdkDenseMatrix<ElementType>& MatrixA, const ElementType& ElementB)
{
    return MatrixSubtract(MatrixA, ElementB);
}


template<typename ElementType>
inline mdkDenseMatrix<ElementType> operator*(const mdkDenseMatrix<ElementType>& MatrixA, const ElementType& ElementB)
{
    return MatrixMultiply(MatrixA, ElementB);
}


template<typename ElementType>
inline mdkDenseMatrix<ElementType> operator/(const mdkDenseMatrix<ElementType>& MatrixA, const ElementType& ElementB)
{
    return MatrixElementDivide(MatrixA, ElementB);
}

// ----------------------- Element {+ - * /} Matrix ------------------------------------------------//

template<typename ElementType>
inline mdkDenseMatrix<ElementType> operator+(const ElementType& ElementA, const mdkDenseMatrix<ElementType>& MatrixB)
{
    return MatrixAdd(ElementA, MatrixB);
}


template<typename ElementType>
inline mdkDenseMatrix<ElementType> operator-(const ElementType& ElementA, const mdkDenseMatrix<ElementType>& MatrixB)
{
    return MatrixSubtract(ElementA, MatrixB);
}


template<typename ElementType>
inline mdkDenseMatrix<ElementType> operator*(const ElementType& ElementA, const mdkDenseMatrix<ElementType>& MatrixB)
{
    return MatrixMultiply(ElementA, MatrixB);
}


template<typename ElementType>
inline mdkDenseMatrix<ElementType> operator/(const ElementType& ElementA, const mdkDenseMatrix<ElementType>& MatrixB)
{
    return MatrixElementDivide(ElementA, MatrixB);
}

*/
//===========================================================================================================================//


//---------------------- Matrix {+= -= *= /=} Matrix ----------------------------------------//

template<typename ElementType>
inline
void DenseMatrix<ElementType>::operator+=(const DenseMatrix<ElementType>& InputMatrix)
{
    MatrixAdd(*this, *this, InputMatrix);
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::operator-=(const DenseMatrix<ElementType>& InputMatrix)
{
    MatrixSubtract(*this, *this, InputMatrix);
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::operator*=(const DenseMatrix<ElementType>& InputMatrix)
{
    MatrixMultiply(*this, *this, InputMatrix);
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::operator/=(const DenseMatrix<ElementType>& InputMatrix)
{
    MatrixElementDivide(*this, *this, InputMatrix);
}

//---------------------- Matrix {+= -= *= /=} mdkDenseShadowMatrix ----------------------------------------//

template<typename ElementType>
inline 
void DenseMatrix<ElementType>::operator+=(const DenseShadowMatrix<ElementType>& ShadowMatrix)
{
    auto InputSize = ShadowMatrix.GetSize();

    if (InputSize.RowCount <= 0)
    {
        MDK_Error("ShadowMatrix is empty @ DenseMatrix::operator+=(ShadowMatrix)")
        return;
    }

    if (InputSize.RowCount == 1 && InputSize.ColCount == 1)
    {
        (*this) += ShadowMatrix[0];

        return;
    }

    auto SelfSize = this->GetSize();

    auto Self_ElementNumber = SelfSize.RowCount * SelfSize.ColCount;

    if (InputSize.RowCount != SelfSize.RowCount || InputSize.ColCount != SelfSize.ColCount)
    {
        MDK_Error("Size does not match @ DenseMatrix::operator+=(ShadowMatrix)")
        return;
    }

    auto RawPointer = this->GetElementPointer();

    for (int_max i = 0; i < Self_ElementNumber; ++i)
    {
        RawPointer[i] += ShadowMatrix[i];
    }
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::operator-=(const DenseShadowMatrix<ElementType>& ShadowMatrix)
{
    auto InputSize = ShadowMatrix.GetSize();

    if (InputSize.RowCount <= 0)
    {
        MDK_Error("ShadowMatrix is empty @ DenseMatrix::operator-=(ShadowMatrix)")
        return;
    }

    if (InputSize.RowCount == 1 && InputSize.ColCount == 1)
    {
        (*this) -= ShadowMatrix[0];

        return;
    }

    auto SelfSize = this->GetSize();

    auto Self_ElementNumber = SelfSize.RowCount * SelfSize.ColCount;

    if (InputSize.RowCount != SelfSize.RowCount || InputSize.ColCount != SelfSize.ColCount)
    {
        MDK_Error("Size does not match @ DenseMatrix::operator-=(ShadowMatrix)")
        return;
    }

    auto RawPointer = this->GetElementPointer();

    for (int_max i = 0; i < Self_ElementNumber; ++i)
    {
        RawPointer[i] -= ShadowMatrix[i];
    }
}


template<typename ElementType>
inline 
void DenseMatrix<ElementType>::operator*=(const DenseShadowMatrix<ElementType>& ShadowMatrix)
{
    this->operator*=(DenseShadowMatrix.CreateDenseMatrix());
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::operator/=(const DenseShadowMatrix<ElementType>& ShadowMatrix)
{
    auto InputSize = ShadowMatrix.GetSize();

    if (InputSize.RowCount <= 0)
    {
        MDK_Error("ShadowMatrix is empty @ DenseMatrix::operator/=(ShadowMatrix)")
        return;
    }

    if (InputSize.RowCount == 1 && InputSize.ColCount == 1)
    {
        (*this) /= ShadowMatrix[0];

        return;
    }

    auto SelfSize = this->GetSize();

    auto Self_ElementNumber = SelfSize.RowCount * SelfSize.ColCount;

    if (InputSize.RowCount != SelfSize.RowCount || InputSize.ColCount != SelfSize.ColCount)
    {
        MDK_Error("Size does not match @ DenseMatrix::operator/=(ShadowMatrix)")
        return;
    }

    auto RawPointer = this->GetElementPointer();

    for (int_max i = 0; i < Self_ElementNumber; ++i)
    {
        RawPointer[i] /= ShadowMatrix[i];
    }
}

//---------------------- Matrix {+= -= *= /=} GlueMatrixForLinearCombination ----------------------------------------//

template<typename ElementType>
inline
void DenseMatrix<ElementType>::operator+=(DenseGlueMatrixForLinearCombination<ElementType> GlueMatrix)
{
    (*this) = (*this) + std::move(GlueMatrix);
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::operator-=(DenseGlueMatrixForLinearCombination<ElementType> GlueMatrix)
{
    (*this) = (*this) - std::move(GlueMatrix);
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::operator*=(const DenseGlueMatrixForLinearCombination<ElementType>& GlueMatrix)
{
    this->operator*=(GlueMatrix.CreateDenseMatrix());
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::operator/=(const DenseGlueMatrixForLinearCombination<ElementType>& GlueMatrix)
{
    this->operator/=(GlueMatrix.CreateDenseMatrix());
}

//---------------------- Matrix {+= -= *= /=} GlueMatrixForMultiplication ----------------------------------------//

template<typename ElementType>
inline
void DenseMatrix<ElementType>::operator+=(DenseGlueMatrixForMultiplication<ElementType> GlueMatrix)
{
    (*this) = (*this) + std::move(GlueMatrix);
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::operator-=(DenseGlueMatrixForMultiplication<ElementType> GlueMatrix)
{
    (*this) = (*this) - std::move(GlueMatrix);
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::operator*=(DenseGlueMatrixForMultiplication<ElementType> GlueMatrix)
{
    (*this) = (*this) * std::move(GlueMatrix);
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::operator/=(const DenseGlueMatrixForMultiplication<ElementType>& GlueMatrix)
{
    this->operator/=(GlueMatrix.CreateDenseMatrix());
}

//---------------------- Matrix {+= -= *= /=} Element ----------------------------------------//

template<typename ElementType>
inline	
void DenseMatrix<ElementType>::operator+=(const ElementType& Element)
{
    MatrixAdd(*this, *this, Element);
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::operator-=(const ElementType& Element)
{
    MatrixSubtract(*this, *this, Element);
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::operator*=(const ElementType& Element)
{
    MatrixElementMultiply(*this, *this, Element);
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::operator/=(const ElementType& Element)
{
    MatrixElementDivide(*this, *this, Element);
}

//-------------------- element operation {^} -----------------------------------------------------------//

template<typename ElementType>
inline 
DenseMatrix<ElementType> DenseMatrix<ElementType>::operator^(const ElementType& Element)
{
    return MatrixElementNamedOperation('^', *this, Element);
}

//-------------------- special element operation {^=} -----------------------------------------------------------//

template<typename ElementType>
inline void DenseMatrix<ElementType>::operator^=(const ElementType& Element)
{
    MatrixElementNamedOperation(*this, '^', *this, Element);
}

//-------------------- special element operation ( .* in matlab ) ------------------------------------------//

template<typename ElementType>
inline
DenseMatrix<ElementType> DenseMatrix<ElementType>::ElementMultiply(const ElementType& Element) const
{
    return MatrixElementMultiply(*this, Element);
}


template<typename ElementType>
inline
DenseMatrix<ElementType> DenseMatrix<ElementType>::ElementMultiply(const DenseMatrix<ElementType>& InputMatrix) const
{
    return MatrixElementMultiply(*this, InputMatrix);
}


template<typename ElementType>
inline
DenseMatrix<ElementType> DenseMatrix<ElementType>::ElementMultiply(DenseMatrix<ElementType>&& InputMatrix) const
{
    if (this->GetElementPointer() == InputMatrix.GetElementPointer())
    {
        return MatrixElementMultiply(*this, InputMatrix);
    }    
    MatrixElementMultiply(InputMatrix, InputMatrix, *this);
    return InputMatrix;
}



template<typename ElementType>
inline
DenseMatrix<ElementType> DenseMatrix<ElementType>::ElementMultiply(const DenseShadowMatrix<ElementType>& ShadowMatrix) const
{
    auto tempMatrix = ShadowMatrix.CreateDenseMatrix();
    MatrixElementMultiply(tempMatrix, tempMatrix, *this);
    return tempMatrix;
}


template<typename ElementType>
inline
DenseMatrix<ElementType> DenseMatrix<ElementType>::ElementMultiply(const DenseGlueMatrixForLinearCombination<ElementType>& GlueMatrix) const
{
    auto tempMatrix = GlueMatrix.CreateDenseMatrix();
    MatrixElementMultiply(tempMatrix, tempMatrix, *this);
    return tempMatrix;
}


template<typename ElementType>
inline
DenseMatrix<ElementType> DenseMatrix<ElementType>::ElementMultiply(const DenseGlueMatrixForMultiplication<ElementType>& GlueMatrix) const
{
    auto tempMatrix = GlueMatrix.CreateDenseMatrix();
    MatrixElementMultiply(tempMatrix, tempMatrix, *this);
    return tempMatrix;
}

//-------------------- element operation : Object.ElementOperation update the current data of the object ---------------//

template<typename ElementType>
template<typename OperationType>
inline
void DenseMatrix<ElementType>::ElementOperation(OperationType Operation)
{
	MatrixElementOperation(*this, Operation, *this);
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::ElementOperation(const char* OperationName)
{
    MatrixElementNamedOperation(*this, OperationName, *this);
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::ElementOperation(const std::string& OperationName)
{
    MatrixElementNamedOperation(*this, OperationName, *this);
}


template<typename ElementType>
template<typename OperationType>
inline
void DenseMatrix<ElementType>::ElementOperation(OperationType Operation, const DenseMatrix<ElementType>& InputMatrix)
{
	MatrixElementOperation(*this, Operation, *this, InputMatrix);
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::ElementOperation(const char OperationName, const DenseMatrix<ElementType>& InputMatrix)
{
    MatrixElementNamedOperation(*this, OperationName, *this, InputMatrix);
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::ElementOperation(const char* OperationName, const DenseMatrix<ElementType>& InputMatrix)
{
    MatrixElementNamedOperation(*this, OperationName, *this, InputMatrix);
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::ElementOperation(const std::string& OperationName, const DenseMatrix<ElementType>& InputMatrix)
{
    MatrixElementNamedOperation(*this, OperationName, *this, InputMatrix);
}


template<typename ElementType>
template<typename OperationType>
inline
void DenseMatrix<ElementType>::ElementOperation(OperationType Operation, const ElementType& Element)
{
	MatrixElementOperation(*this, Operation, *this, Element);
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::ElementOperation(const char OperationName, const ElementType& Element)
{
    MatrixElementNamedOperation(*this, OperationName, *this, Element);
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::ElementOperation(const char* OperationName, const ElementType& Element)
{
    MatrixElementNamedOperation(*this, OperationName, *this, Element);
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::ElementOperation(const std::string& OperationName, const ElementType& Element)
{
    MatrixElementNamedOperation(*this, OperationName, *this, Element);
}

//-------------------- general col operation : Object.ColOperation modify the object itself ---------------//

template<typename ElementType>
template<typename OperationType>
inline
void DenseMatrix<ElementType>::ColOperation(int_max ColIndex, OperationType Operation)
{
	MatrixColOperation(*this, ColIndex, Operation, *this, ColIndex, true);
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::ColOperation(int_max ColIndex, const char* OperationName)
{
    MatrixColNamedOperation(*this, ColIndex, OperationName, *this, ColIndex, true);
}


template<typename ElementType>
inline 
void DenseMatrix<ElementType>::ColOperation(int_max ColIndex, const std::string& OperationName)
{
    MatrixColNamedOperation(*this, ColIndex, OperationName, *this, ColIndex, true);
}


template<typename ElementType>
template<typename OperationType>
inline
void DenseMatrix<ElementType>::ColOperation(int_max ColIndex, OperationType Operation, const DenseMatrix<ElementType>& InputMatrix)
{
	MatrixColOperation(*this, ColIndex, Operation, *this, ColIndex, InputMatrix, true);
}


template<typename ElementType>
inline 
void DenseMatrix<ElementType>::ColOperation(int_max ColIndex, const char OperationName, const DenseMatrix<ElementType>& InputMatrix)
{
    MatrixColNamedOperation(*this, ColIndex, OperationName, *this, ColIndex, InputMatrix, true);
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::ColOperation(int_max ColIndex, const char* OperationName, const DenseMatrix<ElementType>& InputMatrix)
{
    MatrixColNamedOperation(*this, ColIndex, OperationName, *this, ColIndex, InputMatrix, true);
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::ColOperation(int_max ColIndex, const std::string& OperationName, const DenseMatrix<ElementType>& InputMatrix)
{
    MatrixColNamedOperation(*this, ColIndex, OperationName, *this, ColIndex, InputMatrix, true);
}


template<typename ElementType>
template<typename OperationType>
inline
void DenseMatrix<ElementType>::ColOperation(int_max ColIndex, OperationType Operation, const ElementType& Element)
{
	MatrixColOperation(*this, ColIndex, Operation, *this, ColIndex, Element, true);
}


template<typename ElementType>
inline 
void DenseMatrix<ElementType>::ColOperation(int_max ColIndex, const char OperationName, const ElementType& Element)
{
    MatrixColNamedOperation(*this, ColIndex, OperationName, *this, ColIndex, Element, true);
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::ColOperation(int_max ColIndex, const char* OperationName, const ElementType& Element)
{
    MatrixColNamedOperation(*this, ColIndex, OperationName, *this, ColIndex, Element, true);
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::ColOperation(int_max ColIndex, const std::string& OperationName, const ElementType& Element)
{
    MatrixColNamedOperation(*this, ColIndex, OperationName, *this, ColIndex, Element, true);
}

//-------------------- general row operation : Object.RowOperation modify the object itself ---------------//

template<typename ElementType>
template<typename OperationType>
inline
void DenseMatrix<ElementType>::RowOperation(int_max RowIndex, OperationType Operation)
{
	MatrixRowOperation(*this, RowIndex, Operation, *this, RowIndex, true);
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::RowOperation(int_max RowIndex, const char* OperationName)
{
    MatrixRowNamedOperation(*this, RowIndex, OperationName, *this, RowIndex, true);
}


template<typename ElementType>
inline 
void DenseMatrix<ElementType>::RowOperation(int_max RowIndex, const std::string& OperationName)
{
    MatrixRowNamedOperation(*this, RowIndex, OperationName, *this, RowIndex, true);
}


template<typename ElementType>
template<typename OperationType>
inline
void DenseMatrix<ElementType>::RowOperation(int_max RowIndex, OperationType Operation, const DenseMatrix<ElementType>& InputRowMatrix)
{
	MatrixRowOperation(*this, RowIndex, Operation, *this, RowIndex, InputRowMatrix, true);
}


template<typename ElementType>
inline 
void DenseMatrix<ElementType>::RowOperation(int_max RowIndex, const char OperationName, const DenseMatrix<ElementType>& InputRowMatrix)
{
	MatrixRowNamedOperation(*this, RowIndex, OperationName, *this, RowIndex, InputRowMatrix, true);
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::RowOperation(int_max RowIndex, const char* OperationName, const DenseMatrix<ElementType>& InputRowMatrix)
{
	MatrixRowNamedOperation(*this, RowIndex, OperationName, *this, RowIndex, InputRowMatrix, true);
}


template<typename ElementType>
inline 
void DenseMatrix<ElementType>::RowOperation(int_max RowIndex, const std::string& OperationName, const DenseMatrix<ElementType>& InputRowMatrix)
{
	MatrixRowNamedOperation(*this, RowIndex, OperationName, *this, RowIndex, InputRowMatrix, true);
}


template<typename ElementType>
template<typename OperationType>
inline
void
DenseMatrix<ElementType>::RowOperation(int_max RowIndex, OperationType Operation, const ElementType& Element)
{
	MatrixRowOperation(*this, RowIndex, Operation, *this, RowIndex, Element, true);
}


template<typename ElementType>
inline 
void DenseMatrix<ElementType>::RowOperation(int_max RowIndex, const char OperationName, const ElementType& Element)
{
    MatrixRowNamedOperation(*this, RowIndex, OperationName, *this, RowIndex, Element, true);
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::RowOperation(int_max RowIndex, const char* OperationName, const ElementType& Element)
{
    MatrixRowNamedOperation(*this, RowIndex, OperationName, *this, RowIndex, Element, true);
}


template<typename ElementType>
inline 
void
DenseMatrix<ElementType>::RowOperation(int_max RowIndex, const std::string& OperationName, const ElementType& Element)
{
    MatrixRowNamedOperation(*this, RowIndex, OperationName, *this, RowIndex, Element, true);
}

//--------------------------------------------------------------------------------------------//

template<typename ElementType>
template<typename MatchFunctionType>
inline
DenseMatrix<int_max>
DenseMatrix<ElementType>::Find(MatchFunctionType MatchFunction) const
{
    return FindElementInMatrix(*this, MatchFunction);
}


template<typename ElementType>
template<typename MatchFunctionType>
inline
DenseMatrix<int_max>
DenseMatrix<ElementType>::Find(int_max MaxOutputNumber, MatchFunctionType MatchFunction) const
{
    return FindElementInMatrix(*this, MaxOutputColCount, MatchFunction);
}


template<typename ElementType>
template<typename MatchFunctionType>
inline
DenseMatrix<int_max>
DenseMatrix<ElementType>::Find(int_max MaxOutputNumber, int_max LinearIndex_start, int_max LinearIndex_end, MatchFunctionType MatchFunction) const
{
    return FindElementInMatrix(*this, MaxOutputNumber, LinearIndex_start, LinearIndex_end, MatchFunction);
}


template<typename ElementType>
template<typename MatchFunctionType>
inline 
int_max DenseMatrix<ElementType>::Find(const char* first_or_last, MatchFunctionType MatchFunction) const
{
	return FindElementInMatrix(*this, first_or_last, MatchFunction);
}


template<typename ElementType>
template<typename MatchFunctionType>
inline
int_max DenseMatrix<ElementType>::Find(const std::string& first_or_last, MatchFunctionType MatchFunction) const
{
	return FindElementInMatrix(*this, first_or_last, MatchFunction);
}


template<typename ElementType>
inline
DenseMatrix<int_max> DenseMatrix<ElementType>::ExactMatch(const ElementType& InputElement) const
{
	return ExactMatchElementInMatrix(*this, InputElement);
}


template<typename ElementType>
inline
int_max DenseMatrix<ElementType>::ExactMatch(const char* first_or_last, const ElementType& InputElement) const
{
	return ExactMatchElementInMatrix(*this, first_or_last, InputElement);
}


template<typename ElementType>
inline
int_max DenseMatrix<ElementType>::ExactMatch(const std::string& first_or_last, const ElementType& InputElement) const
{
	return ExactMatchElementInMatrix(*this, first_or_last, InputElement);
}

//--------------------------------------------------------------------------------------------//

template<typename ElementType>
template<typename MatchFunctionType>
inline
DenseMatrix<int_max>
DenseMatrix<ElementType>::FindCol(MatchFunctionType MatchFunction) const
{
    return FindColInMatrix(*this, MatchFunction);
}


template<typename ElementType>
template<typename MatchFunctionType>
inline
DenseMatrix<int_max> 
DenseMatrix<ElementType>::FindCol(int_max MaxOutputColCount, MatchFunctionType MatchFunction) const
{
    return FindColInMatrix(*this, MaxOutputColCount, MatchFunction);
}


template<typename ElementType>
template<typename MatchFunctionType>
inline
DenseMatrix<int_max>
DenseMatrix<ElementType>::FindCol(int_max MaxOutputColCount, int_max ColIndex_start, int_max ColIndex_end, MatchFunctionType MatchFunction) const
{
    return FindColInMatrix(*this, MaxOutputColCount, ColIndex_start, ColIndex_end, MatchFunction);
}


template<typename ElementType>
template<typename MatchFunctionType>
inline
int_max DenseMatrix<ElementType>::FindCol(const char* first_or_last, MatchFunctionType MatchFunction) const
{
	return FindColInMatrix(*this, first_or_last, MatchFunction);
}


template<typename ElementType>
template<typename MatchFunctionType>
inline
int_max DenseMatrix<ElementType>::FindCol(const std::string& first_or_last, MatchFunctionType MatchFunction) const
{
	return FindColInMatrix(*this, first_or_last, MatchFunction);
}

//-------------------------------------------------------------------------------------------//

template<typename ElementType>
template<typename CompareFunctionType>
inline
DenseMatrix<int_max> DenseMatrix<ElementType>::Sort(CompareFunctionType CompareFunction) const
{
    return this->Sort(0, this->GetElementCount() - 1, CompareFunction);
}


template<typename ElementType>
template<typename CompareFunctionType>
inline 
DenseMatrix<int_max> DenseMatrix<ElementType>::Sort(int_max LinearIndex_start, int_max LinearIndex_end, CompareFunctionType CompareFunction) const
{
    DenseMatrix<int_max> LinearIndexList;

    auto ElementNumber = this->GetElementCount();

    if (LinearIndex_start < 0 || LinearIndex_start >= ElementNumber || LinearIndex_start > LinearIndex_end)
    {
        MDK_Error("LinearIndex_start is invalid @ DenseMatrix::Sort(...)")
        return LinearIndexList;
    }

    if (LinearIndex_end < 0 || LinearIndex_end >= ElementNumber)
    {
        MDK_Error("LinearIndex_end is invalid @ DenseMatrix::Sort(...)")
        return LinearIndexList;
    }

    if (ElementNumber == 0)
    {
        return LinearIndexList;
    }

    if (LinearIndex_start == LinearIndex_end)
    {
        LinearIndexList.Append(LinearIndex_start);
        return LinearIndexList;
    }

    LinearIndexList.FastResize(1, ElementNumber);

    for (int_max i = LinearIndex_start; i <= LinearIndex_end; ++i)
    {
        LinearIndexList[i] = i;
    }

	auto Ptr = this->GetElementPointer();
    std::sort(LinearIndexList.begin(), LinearIndexList.end(), [&](int_max a, int_max b) { return CompareFunction(Ptr[a], Ptr[b]); });

    return LinearIndexList;
}


template<typename ElementType>
inline
DenseMatrix<int_max> DenseMatrix<ElementType>::Sort(const char* ascend_or_descend) const
{
	if (ascend_or_descend[0] == 'a')
	{
		return this->Sort([](const ElementType& ElementA, const ElementType& ElementB){ return ElementA < ElementB; });
	}
	else if (ascend_or_descend[0] == 'd')
	{
		return this->Sort([](const ElementType& ElementA, const ElementType& ElementB){ return ElementA > ElementB; });
	}
	else
	{
		MDK_Error("Invalid order name @ DenseMatrix<ElementType>::Sort(...)")
		DenseMatrix<int_max> IndexList;
		return IndexList;
	}
}


template<typename ElementType>
inline
DenseMatrix<int_max> DenseMatrix<ElementType>::Sort(const std::string& ascend_or_descend) const
{
	if (ascend_or_descend == "ascend")
	{
		return this->Sort([](const ElementType& ElementA, const ElementType& ElementB){ return ElementA < ElementB; });
	}
	else if (ascend_or_descend == "descend")
	{
		return this->Sort([](const ElementType& ElementA, const ElementType& ElementB){ return ElementA > ElementB; });
	}
	else
	{
		MDK_Error("Invalid order name @ DenseMatrix<ElementType>::Sort(...)")
		DenseMatrix<int_max> IndexList;
		return IndexList;
	}
}


template<typename ElementType>
template<typename CompareFunctionType>
inline 
void DenseMatrix<ElementType>::SortInPlace(CompareFunctionType CompareFunction)
{
	if (this->IsEmpty() == true)
	{
		return;
	}
	std::sort(this->begin(), this->end(), CompareFunction);
}


template<typename ElementType>
template<typename CompareFunctionType>
inline 
void DenseMatrix<ElementType>::SortInPlace(int_max LinearIndex_start, int_max LinearIndex_end, CompareFunctionType CompareFunction)
{
	if (this->IsEmpty() == true)
	{
		return;
	}
	std::sort(this->begin() + Index_start, this->begin() + Index_end + 1, CompareFunction);
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::SortInPlace(const char* ascend_or_descend)
{
	if (ascend_or_descend[0] == 'a')
	{
		this->SortInPlace([](const ElementType& ElementA, const ElementType& ElementB){ return ElementA < ElementB; });
	}
	else if (ascend_or_descend[0] == 'd')
	{
		this->SortInPlace([](const ElementType& ElementA, const ElementType& ElementB){ return ElementA > ElementB; });
	}
	else
	{
		MDK_Error("Invalid order name @ DenseMatrix<ElementType>::SortInPlace(...)")
	}
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::SortInPlace(const std::string& ascend_or_descend)
{
	if (ascend_or_descend == "ascend")
	{
		this->SortInPlace([](const ElementType& ElementA, const ElementType& ElementB){ return ElementA < ElementB; });
	}
	else if (ascend_or_descend == "descend")
	{
		this->SortInPlace([](const ElementType& ElementA, const ElementType& ElementB){ return ElementA > ElementB; });
	}
	else
	{
		MDK_Error("Invalid order name @ DenseMatrix<ElementType>::SortInPlace(...)")
	}
}

//-------------------------------------------------------------------------------------------//

template<typename ElementType>
template<typename CompareFunctionType>
inline
DenseMatrix<int_max> 
DenseMatrix<ElementType>::SortCol(CompareFunctionType CompareFunction) const
{
    return SortColInMatrix(*this, CompareFunction);
}


template<typename ElementType>
template<typename CompareFunctionType>
inline
DenseMatrix<int_max>
DenseMatrix<ElementType>::SortCol(int_max ColIndex_start, int_max ColIndex_end, CompareFunctionType CompareFunction) const
{
    return SortColInMatrix(*this, ColIndex_start, ColIndex_end, CompareFunction);
}


template<typename ElementType>
template<typename CompareFunctionType>
inline 
void DenseMatrix<ElementType>::SortColInPlace(CompareFunctionType CompareFunction)
{
    this->SortColInPlace(0, this->GetElementCount()-1, CompareFunction);
}


template<typename ElementType>
template<typename CompareFunctionType>
inline
void DenseMatrix<ElementType>::SortColInPlace(int_max ColIndex_start, int_max ColIndex_end, CompareFunctionType CompareFunction)
{
    auto ColIndexList = SortColInMatrix(*this, ColIndex_start, ColIndex_end, CompareFunction);
    if (ColIndexList.IsEmpty() == true)
    {
        return;
    }
    (*this)(ALL, span(ColIndex_start, ColIndex_end)) = (*this)(ALL, ColIndexList);
}


//-----------------------------------------------------------------------------------------------------------//

template<typename ElementType>
inline
DenseMatrix<int_max> DenseMatrix<ElementType>::FindUnique() const
{
    return FindUniqueElementInMatrix(*this);
}


template<typename ElementType>
template<typename SpecialCompareFunctionType>
inline
DenseMatrix<int_max> DenseMatrix<ElementType>::FindUnique(SpecialCompareFunctionType SpecialCompareFunction) const
{
    return FindUniqueElementInMatrix(*this, SpecialCompareFunction);
}


template<typename ElementType>
inline DenseMatrix<int_max> DenseMatrix<ElementType>::FindUniqueCol() const
{
    if (this->IsVector() == true)
    {
        return FindUniqueElementInMatrix(*this);
    }

    return FindUniqueColInMatrix(*this);
}


template<typename ElementType>
template<typename SpecialCompareFunctionType>
inline DenseMatrix<int_max> DenseMatrix<ElementType>::FindUniqueCol(SpecialCompareFunctionType SpecialCompareFunction) const
{
    return FindUniqueColInMatrix(*this, SpecialCompareFunction);
}

//-----------------------------------------------------------------------------------------------------------//

template<typename ElementType>
inline
DenseVector<int_max, 2> DenseMatrix<ElementType>::TransformLinearIndexTo2DIndex(int_max LinearIndex) const
{// no input check
	DenseVector<int_max, 2> Index2D; // [RowIndex, ColIndex]
	auto SelfSize = this->GetSize();
	Index2D[0] = LinearIndex % SelfSize.RowCount;
	Index2D[1] = LinearIndex / SelfSize.RowCount;
	return Index2D;
}


template<typename ElementType>
inline
int_max DenseMatrix<ElementType>::Transform2DIndexToLinearIndex(const DenseVector<int_max, 2>& Index2D) const
{// no input check
	LinearIndex = Index2D[0] + Index2D[2]*this->GetRowCount();
	return LinearIndex;
}


template<typename ElementType>
inline
int_max DenseMatrix<ElementType>::Transform2DIndexToLinearIndex(int_max RowIndex, int_max ColIndex) const
{// not input check
	LinearIndex = RowIndex + ColIndex * this->GetRowCount();
	return LinearIndex;
}

//-----------------------------------------------------------------------------------------------------------//

template<typename ElementType>
inline ElementType DenseMatrix<ElementType>::Mean() const
{
    return MatrixMean(*this);
}


template<typename ElementType>
inline DenseMatrix<ElementType> DenseMatrix<ElementType>::MeanOfEachCol() const
{
	return MatrixMeanOfEachCol(*this);
}


template<typename ElementType>
inline DenseMatrix<ElementType> DenseMatrix<ElementType>::MeanOfEachRow() const
{
	return MatrixMeanOfEachRow(*this);
}


template<typename ElementType>
inline ElementType DenseMatrix<ElementType>::Std() const
{
	return MatrixStd(*this);
}


template<typename ElementType>
inline ElementType DenseMatrix<ElementType>::Sum() const
{
    return MatrixSum(*this);
}


template<typename ElementType>
inline DenseMatrix<ElementType> DenseMatrix<ElementType>::SumOfEachCol() const
{
	return MatrixSumOfEachCol(*this);
}


template<typename ElementType>
inline DenseMatrix<ElementType> DenseMatrix<ElementType>::SumOfEachRow() const
{
	return MatrixSumOfEachRow(*this);
}


template<typename ElementType>
inline ElementType DenseMatrix<ElementType>::Max() const
{
	return MatrixMax(*this);
}


template<typename ElementType>
inline int_max DenseMatrix<ElementType>::IndexOfMax() const
{
    return FindLinearIndexOfMaxInMatrix(*this);
}


template<typename ElementType>
inline DenseVector<int_max, 2> DenseMatrix<ElementType>::RowIndexAndColIndexOfMax() const
{
	auto LinearIndex = this->IndexOfMax();
	return this->TransformLinearIndexTo2DIndex(LinearIndex);
}


template<typename ElementType>
inline DenseMatrix<ElementType> DenseMatrix<ElementType>::MaxOfEachCol() const
{
	return MatrixMaxOfEachCol(*this);
}


template<typename ElementType>
inline DenseMatrix<ElementType> DenseMatrix<ElementType>::MaxOfEachRow() const
{
	return MatrixMaxOfEachRow(*this);
}


template<typename ElementType>
inline ElementType DenseMatrix<ElementType>::Min() const
{
	return MatrixMin(*this);
}


template<typename ElementType>
inline int_max DenseMatrix<ElementType>::IndexOfMin() const
{
    return FindLinearIndexOfMinInMatrix(*this);
}


template<typename ElementType>
inline DenseVector<int_max, 2> DenseMatrix<ElementType>::RowIndexAndColIndexOfMin() const
{
	auto LinearIndex = this->IndexOfMin();
	return this->TransformLinearIndexTo2DIndex(LinearIndex);
}


template<typename ElementType>
inline DenseMatrix<ElementType> DenseMatrix<ElementType>::MinOfEachCol() const
{
	return MatrixMinOfEachCol(*this);
}


template<typename ElementType>
inline DenseMatrix<ElementType> DenseMatrix<ElementType>::MinOfEachRow() const
{
	return MatrixMinOfEachRow(*this);
}


template<typename ElementType>
inline 
ElementType DenseMatrix<ElementType>::L1Norm() const
{
    return MatrixNorm_L1(*this);
}


template<typename ElementType>
inline 
ElementType DenseMatrix<ElementType>::L2Norm() const
{
    return MatrixNorm_L2(*this);
}


template<typename ElementType>
inline 
DenseMatrix<ElementType> DenseMatrix<ElementType>::Transpose() const
{
    return MatrixTranspose(*this);
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::TransposeInPlace()
{
    MatrixTransposeInPlace(*this);
}


template<typename ElementType>
inline
int_max DenseMatrix<ElementType>::Rank() const
{
    return MatrixRank(*this);
}


template<typename ElementType>
inline
ElementType DenseMatrix<ElementType>::Det() const
{
	return MatrixDeterminant(*this);
}

template<typename ElementType>
inline 
DenseMatrix<ElementType> DenseMatrix<ElementType>::Inv() const
{
	return MatrixInverse(*this);
}


template<typename ElementType>
inline
DenseMatrix<ElementType> DenseMatrix<ElementType>::PInv() const
{
	return MatrixPseudoInverse(*this);
}


template<typename ElementType>
inline
DenseMatrixSVDResult<ElementType> DenseMatrix<ElementType>::SVD() const
{
    return MatrixSVD(*this);
}

}//end namespace mdk
