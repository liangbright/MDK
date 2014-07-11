#ifndef __mdkDenseMatrix_hpp
#define __mdkDenseMatrix_hpp


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
DenseMatrix<ElementType>::DenseMatrix(const MDK_Symbol_Empty&)
{
    this->Resize(0, 0);
    this->FixSize();
}


template<typename ElementType>
inline
DenseMatrix<ElementType>::DenseMatrix(int_max RowNumber, int_max ColNumber)
{
    this->Resize(RowNumber, ColNumber);
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
DenseMatrix<ElementType>::DenseMatrix(const std::vector<ElementType>& InputVector)
{
    this->Resize(0, 0);

    if (InputVector.size() > 0)
    {
        (*this) = InputVector;
    }
}


template<typename ElementType>
template<int_max Length>
inline
DenseMatrix<ElementType>::DenseMatrix(const DenseVector<ElementType, Length>& InputVector)
{
    this->Resize(0, 0);

    if (InputVector.GetElementNumber() > 0)
    {
        (*this) = InputVector;
    }
}

/*
template<typename ElementType>
inline
DenseMatrix<ElementType>::DenseMatrix(const DenseVector<ElementType>& InputVector)
{
    this->Resize(0, 0);

    if (InputVector.GetElementNumber() > 0)
    {
        (*this) = InputVector;
    }
}
*/

template<typename ElementType>
inline
DenseMatrix<ElementType>::DenseMatrix(const DataArray<ElementType>& InputVector)
{
    this->Resize(0, 0);

    if (InputVector.GetElementNumber() > 0)
    {
        (*this) = InputVector;
    }
}


template<typename ElementType>
inline
DenseMatrix<ElementType>::DenseMatrix(const SimpleDataArray<ElementType>& InputVector)
{
    this->Resize(0, 0);

    if (InputVector.GetElementNumber() > 0)
    {
        (*this) = InputVector;
    }
}


template<typename ElementType>
inline
DenseMatrix<ElementType>::DenseMatrix(const DenseMatrix<ElementType>& InputMatrix, ObjectConstructionTypeEnum Method)
{
    if (Method == ObjectConstructionTypeEnum::Copy)
    {
        this->Resize(0, 0);

        this->Copy(InputMatrix);
    }
    else
    {
        this->ForceShare(InputMatrix);
    }
}


// move constructor
template<typename ElementType>
inline
DenseMatrix<ElementType>::DenseMatrix(DenseMatrix<ElementType>&& InputMatrix) noexcept
{
    m_MatrixData = std::move(InputMatrix.m_MatrixData);

    m_ElementPointer = m_MatrixData->ElementPointer;

    // InputMatrix may not be destructed, e.g., sort a list of DenseMatrix in DataArray
    InputMatrix.m_ElementPointer = nullptr;
}


template<typename ElementType>
inline
DenseMatrix<ElementType>::DenseMatrix(const DenseShadowMatrix<ElementType>& ShadowMatrix)
{
    this->Resize(0, 0);

    this->Take(ShadowMatrix.CreateDenseMatrix());
}


template<typename ElementType>
inline
DenseMatrix<ElementType>::DenseMatrix(const DenseGlueMatrixForLinearCombination<ElementType>& GlueMatrix)
{
    this->Resize(0, 0);

    this->Take(GlueMatrix.CreateDenseMatrix());
}


template<typename ElementType>
inline
DenseMatrix<ElementType>::DenseMatrix(const DenseGlueMatrixForMultiplication<ElementType>& GlueMatrix)
{
    this->Resize(0, 0);

    this->Take(GlueMatrix.CreateDenseMatrix());
}


template<typename ElementType>
inline
DenseMatrix<ElementType>::DenseMatrix(ElementType* InputElementPointer, int_max InputRowNumber, int_max InputColNumber, bool IsSizeFixed = true)
{
    this->Resize(0, 0);

    this->Share(InputElementPointer, InputRowNumber, InputColNumber);

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


// move assignment operator
template<typename ElementType>
inline
void DenseMatrix<ElementType>::operator=(DenseMatrix<ElementType>&& InputMatrix)
{
    this->Take(std::forward<DenseMatrix<ElementType>&>(InputMatrix));
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::operator=(const ElementType& Element)
{
    auto ElementNumber = this->GetElementNumber();

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
        MDK_Error("Input is empty @ DenseMatrix::operator=(initializer_list)")
        return;
    }

    auto SelfSize = this->GetSize();

    if (SelfSize.RowNumber == 1 && SelfSize.ColNumber == InputLength)
    {
        this->SetRow(0, InputList);
        return;
    }
    else if (SelfSize.ColNumber == 1 && SelfSize.RowNumber == InputLength)
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

    auto InputRowNumber = int_max(InputListInList.size());

    if (InputRowNumber <= 0)
    {
        MDK_Error("Input is empty @ DenseMatrix::operator=(InputListInList)")
        return;
    }

    auto InputColNumber = (InputListInList.begin()[0]).size();

    //check each row-list 
    for (int_max i = 1; i < InputRowNumber; ++i)
    {
        auto subListLength = int_max((InputListInList.begin()[i]).size());

        if (subListLength != InputColNumber)
        {
            MDK_Error("The number of Elements in each row is not the same: #" << i << " @ DenseMatrix::operator=(InputListInList)")
            return;
        }
    }

    auto SelfSize = this->GetSize();

    if (this->IsSizeFixed() == true)
    {
        if (SelfSize.RowNumber != InputRowNumber || SelfSize.ColNumber != InputColNumber)
        {
            MDK_Error("Size can not change @ DenseMatrix::operator=(list in list)");
            return;
        }
    }
    else
    {
        this->FastResize(InputRowNumber, InputColNumber);

        SelfSize.RowNumber = InputRowNumber;
        SelfSize.ColNumber = InputColNumber;
    }

    auto RawPointer = this->GetElementPointer();

    for (int_max i = 0; i < SelfSize.RowNumber; ++i)
    {
        auto subList = InputListInList.begin()[i];

        int_max Index = 0;

        for (int_max j = 0; j < SelfSize.ColNumber; ++j)
        {
            RawPointer[Index + i] = subList.begin()[j];

            Index += SelfSize.RowNumber;
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
        MDK_Error("Input is empty @ DenseMatrix::operator=(initializer_list of matrix pointer)")
        return;
    }

    bool IsSelfInInputList = false;

    std::vector<int_max> ColNumberList;
    ColNumberList.resize(InputMatrixNumber);

    int_max InputRowNumber = 0;

    int_max TotalColNumber = 0;

    for (int_max k = 0; k < InputMatrixNumber; k++)
    {
        auto InputMatrixPtr = InputList.begin()[k];

        if (InputMatrixPtr == nullptr)
        {
            MDK_Error("nullptr in InputList @ @ DenseMatrix::operator=(initializer_list of matrix pointer)")
            return;
        }

        if (InputRowNumber == 0)
        {
            InputRowNumber = InputMatrixPtr->GetRowNumber();
        }

        ColNumberList[k] = InputMatrixPtr->GetColNumber();

        TotalColNumber += InputMatrixPtr->GetColNumber();

        if (InputRowNumber != InputMatrixPtr->GetRowNumber())
        {
            MDK_Error("RowNumber is not the same in the list @ DenseMatrix::operator=(initializer_list of matrix pointer)")
            return;
        }

        if (this->GetElementPointer() == InputMatrixPtr->GetElementPointer())
        {
            IsSelfInInputList = true;
        }
    }

    if (TotalColNumber <= 0)
    {
        MDK_Error("TotalColNumber is 0 @ DenseMatrix::operator=(initializer_list)")
        return;
    }

    auto SelfSize = this->GetSize();

    if (IsSelfInInputList == false)
    {
        if (this->IsSizeFixed() == true)
        {
            if (SelfSize.RowNumber != InputRowNumber || SelfSize.ColNumber != TotalColNumber)
            {
                MDK_Error("Can not change matrix size @ DenseMatrix::operator=(initializer_list)")
                return;
            }
        }
        else
        {
            this->FastResize(InputRowNumber, TotalColNumber);
        }

        int_max temp_ColNumber = 0;

        for (int_max k = 0; k < InputMatrixNumber; k++)
        {
            auto InputMatrixPtr = InputList.begin()[k];

            auto ColPtr = InputMatrixPtr->GetElementPointer();

            for (int_max j = temp_ColNumber; j < temp_ColNumber + ColNumberList[k]; ++j, ColPtr += InputRowNumber)
            {
                this->SetCol(j, ColPtr);
            }

            temp_ColNumber += ColNumberList[k];
        }
    }
    else // Self is in InputList 
    {
        if (TotalColNumber == SelfSize.ColNumber)
        {
            MDK_Warning("Self = {&Self} @  @ DenseMatrix::operator=(initializer_list)")
            return;
        }
        else
        {
            DenseMatrix<ElementType> tempMatrix = InputList;

            this->Take(tempMatrix);
        }
    }
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::operator=(const std::initializer_list<std::initializer_list<const DenseMatrix<ElementType>*>>& InputListInList)
{
    MDK_Error("Not implemented yet @ DenseMatrix::operator=(InputListInList of matrix pointer)")
}


template<typename ElementType>
inline 
void DenseMatrix<ElementType>::operator=(const std::vector<ElementType>& InputVector)
{
    //InputVector is treated as a row vector

    auto InputVectorLength = int_max(InputVector.size());

    if (InputVectorLength <= 0)
    {
        MDK_Error("Input is empty @ DenseMatrix::operator=(std::vector)")
        return;
    }

    auto SelfSize = this->GetSize();

    if (SelfSize.RowNumber == 1 && SelfSize.ColNumber == InputVectorLength)
    {
        this->SetRow(0, InputVector);
        return;
    }
    else if (SelfSize.ColNumber == 1 && SelfSize.RowNumber == InputVectorLength)
    {
        this->SetCol(0, InputVector);
        return;
    }

    if (this->IsSizeFixed() == true)
    {
        MDK_Error("Can not change matrix size @ DenseMatrix::operator=(std::vector)")
    }
    else
    {
        this->FastResize(1, InputVectorLength);
        this->SetRow(0, InputVector);
    }
}


template<typename ElementType>
template<int_max Length>
inline
void DenseMatrix<ElementType>::operator=(const DenseVector<ElementType, Length>& InputVector)
{
    //InputVector is treated as a row vector

    // do not use template parameter Length, it can be -1 for DenseVector with variable length
    auto InputVectorLength = InputVector.GetLength();

    if (InputVectorLength <= 0)
    {
        MDK_Error("Input is empty @ DenseMatrix::operator=(DenseVector)")
        return;
    }

    auto SelfSize = this->GetSize();

    if (SelfSize.RowNumber == 1 && SelfSize.ColNumber == InputVectorLength)
    {
        this->SetRow(0, InputVector.GetElementPointer());
        return;
    }
    else if (SelfSize.ColNumber == 1 && SelfSize.RowNumber == InputVectorLength)
    {
        this->SetCol(0, InputVector.GetElementPointer());
        return;
    }

    if (this->IsSizeFixed() == true)
    {
        MDK_Error("Can not change matrix size @ DenseMatrix::operator=(DenseVector)")
    }
    else
    {
        this->FastResize(1, InputVectorLength);
        this->SetRow(0, InputVector.GetElementPointer());
    }
}

/*
template<typename ElementType>
inline
void DenseMatrix<ElementType>::operator=(const DenseVector<ElementType>& InputVector)
{
    //InputVector is treated as a row vector

    auto InputVectorLength = InputVector.GetLength();

    if (InputVectorLength <= 0)
    {
        MDK_Error("Input is empty @ DenseMatrix::operator=(DenseVector)")
        return;
    }

    auto SelfSize = this->GetSize();

    if (SelfSize.RowNumber == 1 && SelfSize.ColNumber == InputVectorLength)
    {
        this->SetRow(0, InputVector.GetElementPointer());
        return;
    }
    else if (SelfSize.ColNumber == 1 && SelfSize.RowNumber == InputVectorLength)
    {
        this->SetCol(0, InputVector.GetElementPointer());
        return;
    }

    if (this->IsSizeFixed() == true)
    {
        MDK_Error("Can not change matrix size @ DenseMatrix::operator=(DenseVector)")
    }
    else
    {
        this->FastResize(1, InputVectorLength);
        this->SetRow(0, InputVector.GetElementPointer());
    }
}
*/

template<typename ElementType>
inline
void DenseMatrix<ElementType>::operator=(const DataArray<ElementType>& InputVector)
{
    //InputVector is treated as a row vector

    auto InputVectorLength = InputVector.GetLength();

    if (InputVectorLength <= 0)
    {
        MDK_Error("Input is empty @ DenseMatrix::operator=(DataArray)")
        return;
    }

    auto SelfSize = this->GetSize();

    if (SelfSize.RowNumber == 1 && SelfSize.ColNumber == InputVectorLength)
    {
        this->SetRow(0, InputVector.GetElementPointer());
        return;
    }
    else if (SelfSize.ColNumber == 1 && SelfSize.RowNumber == InputVectorLength)
    {
        this->SetCol(0, InputVector.GetElementPointer());
        return;
    }

    if (this->IsSizeFixed() == true)
    {
        MDK_Error("Can not change matrix size @ DenseMatrix::operator=(DataArray)")
    }
    else
    {
        this->FastResize(1, InputVectorLength);
        this->SetRow(0, InputVector.GetElementPointer());
    }
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::operator=(const SimpleDataArray<ElementType>& InputVector)
{
    //InputVector is treated as a row vector

    auto InputVectorLength = InputVector.GetLength();

    if (InputVectorLength <= 0)
    {
        MDK_Error("Input is empty @ DenseMatrix::operator=(DataArray)")
        return;
    }

    auto SelfSize = this->GetSize();

    if (SelfSize.RowNumber == 1 && SelfSize.ColNumber == InputVectorLength)
    {
        this->SetRow(0, InputVector.GetElementPointer());
        return;
    }
    else if (SelfSize.ColNumber == 1 && SelfSize.RowNumber == InputVectorLength)
    {
        this->SetCol(0, InputVector.GetElementPointer());
        return;
    }

    if (this->IsSizeFixed() == true)
    {
        MDK_Error("Can not change matrix size @ DenseMatrix::operator=(std::vector)")
    }
    else
    {
        this->FastResize(1, InputVectorLength);
        this->SetRow(0, InputVector.GetElementPointer());
    }
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::operator=(const DenseShadowMatrix<ElementType>& ShadowMatrix)
{
    this->Take(ShadowMatrix);
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::operator=(const DenseGlueMatrixForLinearCombination<ElementType>& GlueMatrix)
{
    this->Take(GlueMatrix);
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::operator=(const DenseGlueMatrixForMultiplication<ElementType>& GlueMatrix)
{
    this->Take(GlueMatrix);
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool DenseMatrix<ElementType>::Copy(const DenseMatrix<ElementType_Input>& InputMatrix)
{
    if (InputMatrix.IsEmpty() == true)
    {
        if (this->IsSizeFixed() == true)
        {
            if (this->IsEmpty() == false)
            {
                MDK_Error("Can not change size @ DenseMatrix::Copy(InputMatrix)")
                return false;
            }

            return true;
        }
        else
        {
            //MDK_Warning("InputMatrix is empty, and this matrix is set to be empty @ DenseMatrix::Copy(InputMatrix)")
            this->Clear();
            return true;
        }       
    }

    // copy data
    return this->Copy(InputMatrix.GetElementPointer(), InputMatrix.GetRowNumber(), InputMatrix.GetColNumber());
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool DenseMatrix<ElementType>::Copy(const DenseMatrix<ElementType_Input>* InputMatrix)
{
    if (InputMatrix == nullptr)
    {
        MDK_Error("Input is nullptr @ DenseMatrix::Copy(mdkDenseMatrix* InputMatrix)")
        return false;
    }

    return this->Copy(*InputMatrix);
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool DenseMatrix<ElementType>::Copy(const ElementType_Input* InputElementPointer, int_max InputRowNumber, int_max InputColNumber)
{
    if (InputElementPointer == nullptr || InputRowNumber <= 0 || InputColNumber <= 0)
    {
        MDK_Warning("Input is empty, try to clear self @ DenseMatrix::Copy(ElementType_Input*, RowNumber, ColNumber)")
        
        if (this->IsSizeFixed() == true)
        {
            if (this->IsEmpty() == false)
            {
                MDK_Error("Can not change matrix size @ DenseMatrix::Copy(ElementType_Input*, InputRowNumber, InputColNumber)")
                return false;
            }
        }

        this->Clear();
        return true;
    }

    auto tempElementType = FindMatrixElementType(InputElementPointer[0]);

    if (tempElementType == MatrixElementTypeEnum::UNKNOWN)
    {
        MDK_Error("Input type is unknown @ DenseMatrix::Copy(ElementType_Input*, RowNumber, ColNumber)")
        return false;
    }

    // if this matrix is not empty, check if this and Input Share the same data
    if (this->IsEmpty() == false)
    {
        if (std::size_t(InputElementPointer) == std::size_t(this->GetElementPointer()))
        {
           // MDK_Warning("A Matrix tries to Copy itself @ DenseMatrix::Copy(ElementType_Input*, RowNumber, ColNumber)")
            return true;
        }
    }

    //------------------------------------------------------------------

    auto SelfSize = this->GetSize();

    //------------------------------------------------------------------

    if (this->IsSizeFixed() == true)
    {
        if (InputRowNumber != SelfSize.RowNumber || InputColNumber != SelfSize.ColNumber)
        {
            MDK_Error("Can not change matrix size @ DenseMatrix::Copy(ElementType_Input*, InputRowNumber, InputColNumber)")
            return false;
        }
    }
    else
    {
        this->FastResize(InputRowNumber, InputColNumber);

        SelfSize.RowNumber = InputRowNumber;
        SelfSize.ColNumber = InputColNumber;
    }

    //copy data ----------------------------------------------------------

    auto BeginPointer = this->GetElementPointer();

    auto tempPtr = InputElementPointer;

    auto Self_ElementNumber = SelfSize.RowNumber * SelfSize.ColNumber;

    for (auto Ptr = BeginPointer; Ptr < BeginPointer + Self_ElementNumber; ++Ptr)
    {
        Ptr[0] = ElementType(tempPtr[0]);

        ++tempPtr;
    }

    return true;
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::Fill(const ElementType& Element)
{
    auto Self_ElementNumber = this->GetElementNumber();

    if (Self_ElementNumber <= 0)
    {
        MDK_Error("Self is empty @ DenseMatrix::Fill")
        return false;
    }

    auto BeginPointer = this->GetElementPointer();

    for (auto Ptr = BeginPointer; Ptr < BeginPointer + Self_ElementNumber; ++Ptr)
    {
        Ptr[0] = Element;
    }

    return true;
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::Share(DenseMatrix<ElementType>& InputMatrix)
{
    // Matrix = Matrix
    if (this == &InputMatrix)
    {
        MDK_Warning("A Matrix tries to Share itself @ DenseMatrix::Share(InputMatrix)")
        return true;
    }

    if (InputMatrix.IsPureEmpty() == true)
    {
        MDK_Error("InputMatrix is pure empty (m_MatrixData is empty) @ DenseMatrix::Share(InputMatrix)")
        return false;
    }

    if (this->IsSizeFixed() == true)
    {
        auto InputSize = InputMatrix.GetSize();

        auto SelfSize = this->GetSize();

        if (InputSize.RowNumber != SelfSize.RowNumber || InputSize.ColNumber != SelfSize.ColNumber)
        {
            MDK_Error("Matrix size can not be changed @ DenseMatrix::Share(InputMatrix)")
            return false;
        }
    }

    //--------------------------------------------------------------------------------------------------------

    m_MatrixData = InputMatrix.m_MatrixData; // std::Shared_ptr, self assignment test is not necessary

    m_ElementPointer = m_MatrixData->ElementPointer;

    return true;
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::Share(DenseMatrix<ElementType>* InputMatrix)
{
    if (InputMatrix == nullptr)
    {
        MDK_Error("Input is nullptr @ DenseMatrix::Share(mdkDenseMatrix* InputMatrix)")
        return false;
    }

    return this->Share(*InputMatrix);
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::ForceShare(const DenseMatrix<ElementType>& InputMatrix)
{
    // Matrix = Matrix
    if (this == &InputMatrix)
    {
        MDK_Warning("A Matrix tries to ForceShare itself @ DenseMatrix::ForceShare(InputMatrix)")
        return;
    }

    m_MatrixData = InputMatrix.m_MatrixData; // std::Shared_ptr, self assignment check is not necessary

    if (!m_MatrixData)
    {
        m_ElementPointer = nullptr;
    }
    else
    {
        m_ElementPointer = m_MatrixData->ElementPointer;
    }
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::ForceShare(const DenseMatrix<ElementType>* InputMatrix)
{
    if (InputMatrix == nullptr)
    {
        MDK_Error("Input is nullptr @ DenseMatrix::ForceShare(mdkDenseMatrix* InputMatrix)")
        return false;
    }

    this->ForceShare(*InputMatrix);

    return true;
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::Share(ElementType* InputElementPointer, int_max InputRowNumber, int_max InputColNumber, bool IsSizeFixed)
{
    if (InputElementPointer == nullptr || InputRowNumber <= 0 || InputColNumber <= 0)
    {
        MDK_Error("Invalid input @ DenseMatrix::Share(...)")
        return false;
    }

    if (!m_MatrixData)
    {
        this->Resize(0, 0);
    }

    if (this->IsSizeFixed() == true)
    {
        if (m_MatrixData->RowNumber != InputRowNumber || m_MatrixData->ColNumber != InputColNumber)
        {
            MDK_Error("Size can not change @ DenseMatrix::Share(...)")
            return false;
        }
    }

    m_MatrixData->IsSizeFixed = IsSizeFixed;

    m_MatrixData->RowNumber = InputRowNumber;
    m_MatrixData->ColNumber = InputColNumber;

    m_MatrixData->DataArray.clear();

    m_MatrixData->ElementPointer = InputElementPointer;

    m_ElementPointer = m_MatrixData->ElementPointer;

    return true;
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::ForceShare(const ElementType* InputElementPointer, int_max InputRowNumber, int_max InputColNumber, bool IsSizeFixed)
{
    return this->Share(const_cast<ElementType*>(InputElementPointer), InputRowNumber, InputColNumber, IsSizeFixed);
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::Take(DenseMatrix<ElementType>&& InputMatrix)
{
    this->Take(std::forward<DenseMatrix<ElementType>&>(InputMatrix));
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::Take(DenseMatrix<ElementType>& InputMatrix)
{
    // Matrix = Matrix
    if (this == &InputMatrix)
    {
        MDK_Warning("A Matrix tries to take itself @ DenseMatrix::take(InputMatrix)")
        return true;
    }

    if (!m_MatrixData)
    {
        this->Resize(0, 0);
    }

    auto InputSize = InputMatrix.GetSize();

    auto SelfSize = this->GetSize();

    if (this->IsSizeFixed() == true)
    {
        if (InputSize.RowNumber != SelfSize.RowNumber || InputSize.ColNumber != SelfSize.ColNumber)
        {
            MDK_Error("Size does not match @ DenseMatrix::Take(InputMatrix)")
            return false;
        }
    }

    if (InputMatrix.IsEmpty() == true)
    {
        if (SelfSize.RowNumber > 0)
        {
            //MDK_Warning("InputMatrix is empty, and this matrix is set to be empty @ DenseMatrix::Take(InputMatrix)")
            this->Clear();
        }

        return true;
    }

    // MatrixA = MatrixA
    if (this->GetElementPointer() == InputMatrix.GetElementPointer())
    {
        //MDK_Warning("A Matrix tries to take itself @ DenseMatrix::Take(InputMatrix)")
        return true;
    }

    // now, InputMatrix is not empty, and is not self
    
    //note: m_MatrixData.swap(InputMatrix.m_MatrixData) will invalidate Share()

    m_MatrixData->RowNumber = InputMatrix.m_MatrixData->RowNumber;

    m_MatrixData->ColNumber = InputMatrix.m_MatrixData->ColNumber;

    m_MatrixData->DataArray = std::move(InputMatrix.m_MatrixData->DataArray);

    m_MatrixData->ElementPointer = InputMatrix.m_MatrixData->ElementPointer;

    m_MatrixData->NaNElement = InputMatrix.m_MatrixData->NaNElement;

    m_ElementPointer = m_MatrixData->ElementPointer;

    // Clear InputMatrix to be empty
    InputMatrix.Clear();

    return true;
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::Take(DenseMatrix<ElementType>* InputMatrix)
{
    if (InputMatrix == nullptr)
    {
        MDK_Error("Input is nullptr @ DenseMatrix::Take(mdkDenseMatrix* InputMatrix)")
        return false;
    }

    return this->Take(*InputMatrix);
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::Take(std::vector<ElementType>&& InputRowVector)
{
    return this->Take(std::forward<std::vector<ElementType>&>(InputRowVector));
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::Take(std::vector<ElementType>& InputRowVector)
{
    // MatrixA = MatrixA
    if (this->GetElementPointer() == InputRowVector.data())
    {
        MDK_Warning("A Matrix tries to take itself @ DenseMatrix::Take(std::vector)")
        return true;
    }

    auto InputLength = int_max(InputRowVector.size());

    auto SelfSize = this->GetSize();

    if (this->IsSizeFixed() == true)
    {
        if (SelfSize.RowNumber == 1 && SelfSize.ColNumber == InputLength)
        {
            m_MatrixData->DataArray = std::move(InputRowVector);
            m_MatrixData->ElementPointer = m_MatrixData->DataArray.data();
            m_ElementPointer = m_MatrixData->ElementPointer;

            return true;
        }
        else if (SelfSize.ColNumber == 1 && SelfSize.RowNumber == InputLength)
        {
            m_MatrixData->DataArray = std::move(InputRowVector);
            m_MatrixData->ElementPointer = m_MatrixData->DataArray.data();
            m_ElementPointer = m_MatrixData->ElementPointer;

            return true;
        }
        else
        {
            MDK_Error("Size does not match @ DenseMatrix::Take(InputVector)")
            return false;
        }
    }
    else
    {
        if (InputLength == 0)
        {
            if (SelfSize.RowNumber > 0)
            {
                MDK_Warning("InputRowVector is empty, and this matrix is set to be empty @ DenseMatrix::Take(std::vector)")
                this->Clear();
            }

            return true;
        }

        // now, InputRowVector is not empty

        if (SelfSize.RowNumber == 1)
        {            
            m_MatrixData->DataArray = std::move(InputRowVector);
            m_MatrixData->ElementPointer = m_MatrixData->DataArray.data();
            m_MatrixData->ColNumber = InputLength;

            m_ElementPointer = m_MatrixData->ElementPointer;
        }
        else if (SelfSize.ColNumber == 1)
        {
            m_MatrixData->DataArray = std::move(InputRowVector);
            m_MatrixData->ElementPointer = m_MatrixData->DataArray.data();
            m_MatrixData->RowNumber = InputLength;

            m_ElementPointer = m_MatrixData->ElementPointer;
        }
        else
        {
            if (!m_MatrixData)
            {
                this->Resize(0, 0);
            }

            m_MatrixData->DataArray = std::move(InputRowVector);
            m_MatrixData->ElementPointer = m_MatrixData->DataArray.data();
            m_MatrixData->RowNumber = InputLength;
            m_MatrixData->ColNumber = 1;

            m_ElementPointer = m_MatrixData->ElementPointer;
        }

        return true;
    }
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::Take(DenseVector<ElementType>& InputRowVector)
{
    if (this->Take(InputRowVector.StdVector()) == false)
    {
        return false;
    }
    InputRowVector.Clear();
    return true;
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::Take(DataArray<ElementType>& InputRowVector)
{
    if (this->Take(InputRowVector.StdVector()) == false)
    {
        return false;
    }
    InputRowVector.Clear();
    return true;
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::Take(SimpleDataArray<ElementType>& InputRowVector)
{
    if (this->Take(InputRowVector.StdVector()) == false)
    {
        return false;
    }
    InputRowVector.Clear();
    return true;
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::Take(const DenseShadowMatrix<ElementType>& ShadowMatrix)
{
    if (!m_MatrixData)
    {
        this->Resize(0, 0);
    }

    auto InputSize = ShadowMatrix.GetSize();

    auto SelfSize = this->GetSize();

    if (InputSize.RowNumber == SelfSize.RowNumber && InputSize.ColNumber == SelfSize.ColNumber)
    {
        ShadowMatrix.CreateDenseMatrix(*this);
    }
    else
    {
        if (this->IsSizeFixed() == true)
        {
            MDK_Error("Size does not match @ DenseMatrix::Take(ShadowMatrix)")
            return false;
        }

        this->Take(ShadowMatrix.CreateDenseMatrix());
    }

    return true;
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::Take(const DenseGlueMatrixForLinearCombination<ElementType>& GlueMatrix)
{
    if (!m_MatrixData)
    {
        this->Resize(0, 0);
    }

    auto InputSize = GlueMatrix.GetSize();

    auto SelfSize = this->GetSize();

    if (InputSize.RowNumber == SelfSize.RowNumber && InputSize.ColNumber == SelfSize.ColNumber)
    {
        GlueMatrix.CreateDenseMatrix(*this);
    }
    else
    {
        if (this->IsSizeFixed() == true)
        {
            MDK_Error("Size does not match @ DenseMatrix::Take(GlueMatrix_ForLinearCombination)")
            return false;
        }

        this->Take(GlueMatrix.CreateDenseMatrix());
    }

    return true;
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::Take(const DenseGlueMatrixForMultiplication<ElementType>& GlueMatrix)
{
    if (!m_MatrixData)
    {
        this->Resize(0, 0);
    }

    auto InputSize = GlueMatrix.GetSize();

    auto SelfSize = this->GetSize();

    if (InputSize.RowNumber == SelfSize.RowNumber && InputSize.ColNumber == SelfSize.ColNumber)
    {
        GlueMatrix.CreateDenseMatrix(*this);
    }
    else
    {
        if (this->IsSizeFixed() == true)
        {
            MDK_Error("Size does not match @ DenseMatrix::Take(GlueMatrix_ForMultiplication)")
            return false;
        }

        this->Take(GlueMatrix.CreateDenseMatrix());
    }

    return true;
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::SwapSmartPointer(DenseMatrix<ElementType>& InputMatrix)
{
// for GlueMatrix

    // Matrix = Matrix
    if (this == &InputMatrix)
    {
        MDK_Warning("A Matrix tries to Swap with itself @ DenseMatrix::SwapSmartPointer(InputMatrix)")
        return;
    }

    m_MatrixData.swap(InputMatrix.m_MatrixData); // shared_ptr self swap check is not necessary

    if (m_MatrixData)
    {
        m_ElementPointer = m_MatrixData->ElementPointer;
    }
    else
    {
        m_ElementPointer = nullptr;

        MDK_Warning("m_MatrixData is empty after SwapSmartPointer @ DenseMatrix::SwapSmartPointer(InputMatrix)")
    }

    if (InputMatrix.m_MatrixData)
    {
        InputMatrix.m_ElementPointer = InputMatrix.m_MatrixData->ElementPointer;
    }
    else
    {
        InputMatrix.m_ElementPointer = nullptr;
    }
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::Clear()
{
    if (!m_MatrixData)
    {
        return;
    }

    m_MatrixData->RowNumber = 0;

    m_MatrixData->ColNumber = 0;

    m_MatrixData->DataArray.clear();         // change size
    m_MatrixData->DataArray.shrink_to_fit(); // release memory

    m_MatrixData->ElementPointer = nullptr;

    m_MatrixData->IsSizeFixed = false;

    m_ElementPointer = nullptr;
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::Reshape(int_max InputRowNumber, int_max InputColNumber)
{
    if (this->IsSizeFixed() == true)
    {
        MDK_Error("Matrix size can not be changed @ DenseMatrix::Reshape()")
        return false;
    }

    if (this->IsEmpty() == true)
    {
        if (InputRowNumber > 0 || InputColNumber > 0)
        {
            MDK_Error("Self is empty and Size does not match @ DenseMatrix::Reshape()")
            return false;
        }

        return true;
    }

    if (InputRowNumber*InputColNumber != this->GetElementNumber())
    {
        MDK_Error("Size does not match @ DenseMatrix::Reshape")
        return false;
    }

    m_MatrixData->RowNumber = InputRowNumber;

    m_MatrixData->ColNumber = InputColNumber;

    return true;
}


template<typename ElementType>
inline 
bool DenseMatrix<ElementType>::Resize(int_max InputRowNumber, int_max InputColNumber)
{
    if (InputRowNumber < 0 || InputColNumber < 0)
    {
        MDK_Error("Invalid Input: negtive @ DenseMatrix::Resize(int_max RowNumber, int_max ColNumber)")
        return false;
    }

try
{
    //--------initialize the matrix data ----------------------------------------
    if (!m_MatrixData)
    {
        m_MatrixData = std::make_shared<DenseMatrixData<ElementType>>();

        m_ElementPointer = nullptr;
    }
    //-------------------------------------------------------------------------

    auto SelfSize = this->GetSize();

    if (InputRowNumber == SelfSize.RowNumber && InputColNumber == SelfSize.ColNumber)
    {
        return true;
    }

    if (this->IsSizeFixed() == true)
    {
        MDK_Error("Matrix Size can not be changed @ DenseMatrix::Resize(int_max RowNumber, int_max ColNumber)")
        return false;
    }

    if (InputRowNumber == 0 || InputColNumber == 0)
    {               
        m_MatrixData->RowNumber = 0;

        m_MatrixData->ColNumber = 0;

        m_MatrixData->DataArray.resize(0);

        m_MatrixData->ElementPointer = nullptr;

        m_ElementPointer = nullptr;

        return true;
    }

    // if self is empty
    if (this->IsEmpty() == true)
    {
        m_MatrixData->RowNumber = InputRowNumber;

        m_MatrixData->ColNumber = InputColNumber;

        m_MatrixData->DataArray.resize(InputRowNumber*InputColNumber);

        m_MatrixData->ElementPointer = m_MatrixData->DataArray.data();

        m_ElementPointer = m_MatrixData->ElementPointer;

        return true;
    }

    // if only Col changes and self is not empty

    if (InputRowNumber == SelfSize.RowNumber)
    {
        auto Self_ElementNumber = SelfSize.RowNumber * SelfSize.ColNumber;

        //m_MatrixData->CopyDataToInternalDataArrayIfNecessary();
        if (m_MatrixData->ElementPointer != m_MatrixData->DataArray.data())
        {
            auto ElementNumber_min = std::min(Self_ElementNumber, InputColNumber*InputRowNumber);

            m_MatrixData->DataArray.resize(ElementNumber_min);

            for (int_max i = 0; i < ElementNumber_min; ++i)
            {
                m_MatrixData->DataArray[i] = m_MatrixData->ElementPointer[i];
            }

            m_MatrixData->ElementPointer = m_MatrixData->DataArray.data();
        }

        m_MatrixData->RowNumber = InputRowNumber;

        m_MatrixData->ColNumber = InputColNumber;

        m_MatrixData->DataArray.resize(InputRowNumber*InputColNumber);

        m_MatrixData->ElementPointer = m_MatrixData->DataArray.data();

        m_ElementPointer = m_MatrixData->ElementPointer;

        return true;
    }

    // InputRowNumber != SelfSize.RowNumber and self is not empty -----------------------------------------------------------------------------

    auto tempDataArray = std::vector<ElementType>(InputRowNumber*InputColNumber);

    auto RawPointer = m_MatrixData->ElementPointer;

    auto ColNumber_min = std::min(SelfSize.ColNumber, InputColNumber);

    auto RowNumber_min = std::min(SelfSize.RowNumber, InputRowNumber);

    // try to keep old data as much as possible
    for (int_max j = 0; j < ColNumber_min; ++j)
    {
        auto tempIndex = j*InputRowNumber;

        auto Index = j*SelfSize.RowNumber;

        for (int_max i = 0; i < RowNumber_min; ++i)
        {
            tempDataArray[tempIndex + i] = RawPointer[Index + i];
        }
    }

    //-------------------------------------------------------------

    m_MatrixData->RowNumber = InputRowNumber;

    m_MatrixData->ColNumber = InputColNumber;

    m_MatrixData->DataArray = std::move(tempDataArray);

    m_MatrixData->ElementPointer = m_MatrixData->DataArray.data();

    m_ElementPointer = m_MatrixData->ElementPointer;
}
catch (...)
{
    MDK_Error("Out of Memory @ DenseMatrix::Resize(int_max InputRowNumber, int_max InputColNumber)")

    this->Clear();

    return false;
}

    return true;
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::FastResize(int_max InputRowNumber, int_max InputColNumber)
{
    if (InputRowNumber < 0 || InputColNumber < 0)
    {
        MDK_Error("Invalid input @ DenseMatrix::FastResize(int_max InputRowNumber, int_max InputColNumber)")
        return false;    
    }

    if (!m_MatrixData)
    {
        this->Resize(0, 0);
    }

    auto Size = this->GetSize();

    if (Size.RowNumber == InputRowNumber && Size.ColNumber == InputColNumber)
    {
        return true;
    }

    if (this->IsSizeFixed() == true)
    {
        MDK_Error("Can not change size @ DenseMatrix::FastResize(int_max InputRowNumber, int_max InputColNumber)")
        return false;
    }

    if (InputRowNumber == 0 || InputColNumber == 0)
    {
        m_MatrixData->RowNumber = 0;
        m_MatrixData->ColNumber = 0;

        m_MatrixData->DataArray.clear();         // change size, but not release memory

        m_MatrixData->ElementPointer = nullptr;

        m_MatrixData->IsSizeFixed = false;

        m_ElementPointer = nullptr;

        return true;
    }

try
{
    int_max InputElementNumber = InputRowNumber * InputColNumber;

    if (InputElementNumber != Size.RowNumber *  Size.ColNumber)
    {
        if (InputElementNumber > int_max(m_MatrixData->DataArray.capacity()))
        {
            m_MatrixData->DataArray.clear();
        }

        m_MatrixData->DataArray.resize(InputElementNumber);

        m_MatrixData->ElementPointer = m_MatrixData->DataArray.data();

        m_ElementPointer = m_MatrixData->ElementPointer;
    }
   
    m_MatrixData->RowNumber = InputRowNumber;
    m_MatrixData->ColNumber = InputColNumber;
}
catch (...)
{
    MDK_Error("Out of Memory @ DenseMatrix::FastResize(int_max InputRowNumber, int_max InputColNumber)")

    this->Clear();

    return false;
}

    return true;
}


template<typename ElementType>
inline 
bool DenseMatrix<ElementType>::Resize(int_max InputElementNumber) // try to keep the old data, can not use this to resize a m x n matrix (m>1 or n>1)
{
    if (InputElementNumber < 0)
    {
        MDK_Error("Invalid input @ DenseMatrix::Resize(int_max InputElementNumber)")
        return false;
    }

    if (!m_MatrixData)
    {
        this->Resize(0, 0);
    }

    auto Size = this->GetSize();

    if (Size.RowNumber > 1 && Size.ColNumber > 1)
    {
        MDK_Error("Self is not empty and not a vector @ DenseMatrix::Resize(int_max InputElementNumber)")
        return false;
    }

    if (Size.RowNumber*Size.ColNumber == InputElementNumber)
    {
        return true;
    }

    if (this->IsSizeFixed() == true)
    {
        MDK_Error("Can not change size @ DenseMatrix::Resize(int_max InputElementNumber)")
        return false;
    }

    if (InputElementNumber == 0)
    {
        this->Clear();
        return true;
    }

try
{
    m_MatrixData->DataArray.resize(InputElementNumber);

    m_MatrixData->ElementPointer = m_MatrixData->DataArray.data();

    m_ElementPointer = m_MatrixData->ElementPointer;

    if (Size.RowNumber == 0) // empty -> row vector
    {
        m_MatrixData->RowNumber = 1;
        m_MatrixData->ColNumber = InputElementNumber;
    }
    else if (Size.RowNumber == 1) // row vector -> row vector
    {
        m_MatrixData->RowNumber = 1;
        m_MatrixData->ColNumber = InputElementNumber;
    }
    else if (Size.ColNumber == 1) // col vector -> col vector
    {
        m_MatrixData->RowNumber = InputElementNumber;
        m_MatrixData->ColNumber = 1;
    }
}
catch (...)
{
    MDK_Error("Out of Memory @ DenseMatrix::Resize(int_max InputElementNumber)")

    this->Clear();

    return false;
}
    return true;
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::FastResize(int_max InputElementNumber) // do not care about old data, can not use this to resize a m x n matrix (m>1 or n>1)
{
    if (InputElementNumber < 0)
    {
        MDK_Error("Invalid input @ DenseMatrix::FastResize(int_max InputElementNumber)")
        return false;
    }

    if (!m_MatrixData)
    {
        this->Resize(0, 0);
    }

    auto Size = this->GetSize();

    if (Size.RowNumber == 0) // empty -> row vector
    {
        return this->FastResize(1, InputElementNumber);
    }
    else if (Size.RowNumber == 1) // row vector -> row vector
    {
        return this->FastResize(1, InputElementNumber);
    }
    else if (Size.ColNumber == 1) // col vector -> col vector
    {
        return this->FastResize(InputElementNumber, 1);
    }
    else
    {
        MDK_Error("Self is not empty or not a vector @ DenseMatrix::FastResize(int_max InputElementNumber)")
        return false;
    }    
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::ReserveCapacity(int_max InputRowNumber, int_max InputColNumber)
{
    return this->ReserveCapacity(InputRowNumber*InputColNumber);
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::ReserveCapacity(int_max InputElementNumber)
{
    if (!m_MatrixData)
    {
        this->Resize(0, 0);
    }

try
{
    auto Self_ElementNumber = this->GetElementNumber();

    if (InputElementNumber > Self_ElementNumber)
    {
        m_MatrixData->CopyDataToInternalDataArrayIfNecessary();

        m_MatrixData->DataArray.reserve(InputElementNumber);

        m_MatrixData->ElementPointer = m_MatrixData->DataArray.data();

        m_ElementPointer = m_MatrixData->ElementPointer;
    }
}
catch (...)
{
    MDK_Error("Out of Memory @ DenseMatrix::ReserveCapacity(int_max InputElementNumber)")
    return false;
}
    
    return true;
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::ReleaseUnusedCapacity()
{
    if (!m_MatrixData)
    {
        return;
    }

    if (m_MatrixData->DataArray.data() != nullptr)
    {
        m_MatrixData->DataArray.shrink_to_fit();
        m_MatrixData->ElementPointer = m_MatrixData->DataArray.data();
        m_ElementPointer = m_MatrixData->ElementPointer;
    }
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::Squeeze()
{
    this->ReleaseUnusedCapacity();
}


template<typename ElementType>
inline 
void DenseMatrix<ElementType>::FixSize()
{
    if (!m_MatrixData)
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
        return (m_MatrixData->RowNumber <= 0);
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
    return (this->GetElementPointer() == InputMatrix.GetElementPointer());
}


template<typename ElementType>
inline
MatrixSize DenseMatrix<ElementType>::GetSize() const
{
    MatrixSize Size;

    if (!m_MatrixData)
    {
        Size.RowNumber = 0;

        Size.ColNumber = 0;
    }
    else
    {
        Size.RowNumber = m_MatrixData->RowNumber;

        Size.ColNumber = m_MatrixData->ColNumber;
    }

    return Size;
}


template<typename ElementType>
inline
int_max DenseMatrix<ElementType>::GetElementNumber() const
{
    if (!m_MatrixData)
    {
        return 0;
    }
    else
    {
        return m_MatrixData->RowNumber * m_MatrixData->ColNumber;
    }
}


template<typename ElementType>
inline 
int_max DenseMatrix<ElementType>::GetColNumber() const
{
    if (!m_MatrixData)
    {
        return 0;
    }
    else
    {
        return m_MatrixData->ColNumber;
    }
}


template<typename ElementType>
inline 
int_max DenseMatrix<ElementType>::GetRowNumber() const
{
    if (!m_MatrixData)
    {
        return 0;
    }
    else
    {
        return m_MatrixData->RowNumber;
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
        return (m_MatrixData->RowNumber == 1 || m_MatrixData->ColNumber == 1);
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
        return (m_MatrixData->RowNumber == 1);
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
        return (m_MatrixData->ColNumber == 1);
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
        return (m_MatrixData->RowNumber > 0 && m_MatrixData->RowNumber == m_MatrixData->ColNumber);
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

    auto Value_sum = this->Sum();

    if (std::abs(Value_sum - SelfSize.RowNumber) > Threshold)
    {
        return false;
    }

    auto RawPointer this->GetElementPointer();

    int_max Index = 0;

    for (int_max i = 0; i < SelfSize.RowNumber; ++i)
    {
        auto DiagonalElement_j = RawPointer[Index + i];

        if (std::abs(DiagonalElement_j - ElementType(1)) > Threshold) // must use ">" becuause ElementType may be int and Threshold is 0
        {
            return false;
        }

        Index += SelfSize.RowNumber;
    }

    return true;
}


template<typename ElementType>
inline
const ElementType& DenseMatrix<ElementType>::GetNaNElement()  const
{
    if (!m_MatrixData)
    {
        MDK_Error("Something is wrong here @ DenseMatrix::GetNaNElement()")
    }

    return m_MatrixData->NaNElement;
}


template<typename ElementType>
inline
MatrixElementTypeEnum DenseMatrix<ElementType>::GetElementType() const
{
    if (!m_MatrixData)
    {
        MDK_Error("Something is wrong here @ DenseMatrix::GetNaNElement()")
    }

    return FindMatrixElementType(m_MatrixData->NaNElement);
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
        endPtr += this->GetElementNumber();
    }
    return endPtr;
}


template<typename ElementType>
inline const ElementType* DenseMatrix<ElementType>::end() const
{
    auto endPtr = this->GetElementPointer();
    if (endPtr != nullptr)
    {
        endPtr += this->GetElementNumber();
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

    if (ColIndex < 0 || ColIndex >= Size.ColNumber)
    {
        MDK_Error("Invalid ColIndex @ DenseMatrix::GetElementPointerOfCol(...)")
        return nullptr;
    }

    auto PointerOfCol = this->GetElementPointer() + ColIndex*Size.RowNumber;

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

    if (ColIndex < 0 || ColIndex >= Size.ColNumber)
    {
        MDK_Error("Invalid ColIndex @ DenseMatrix::GetElementPointerOfCol(...) const")
        return nullptr;
    }

    auto PointerOfCol = this->GetElementPointer() + ColIndex*Size.RowNumber;

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

    if (LinearIndex >= this->GetElementNumber()|| LinearIndex < 0)
    {
        MDK_Error("Invalid Input @ DenseMatrix::operator[](i)")

        return m_MatrixData->NaNElement;
    }

#endif //MDK_DEBUG_DenseMatrix_Operator_CheckBound

    return m_ElementPointer[LinearIndex]; 

    // return (*m_MatrixData)[LinearIndex]; this is ~26% slower
}


template<typename ElementType>
inline
const ElementType& DenseMatrix<ElementType>::operator[](int_max LinearIndex) const
{
#if defined(MDK_DEBUG_DenseMatrix_Operator_CheckBound)

    if (LinearIndex >= this->GetElementNumber() || LinearIndex < 0)
    {
        MDK_Error("Invalid Input @ DenseMatrix::operator[](i) const")

        return m_MatrixData->NaNElement;
    }

#endif //MDK_DEBUG_DenseMatrix_Operator_CheckBound

    return m_ElementPointer[LinearIndex];
}


template<typename ElementType>
inline
ElementType& DenseMatrix<ElementType>::operator()(int_max LinearIndex)
{
#if defined(MDK_DEBUG_DenseMatrix_Operator_CheckBound)

    if (LinearIndex >= this->GetElementNumber() || LinearIndex < 0)
    {
        MDK_Error("Invalid Input @ DenseMatrix::operator()(i)")

        return m_MatrixData->NaNElement;
    }

#endif //MDK_DEBUG_DenseMatrix_Operator_CheckBound

	return m_ElementPointer[LinearIndex];
}


template<typename ElementType>
inline
const ElementType& DenseMatrix<ElementType>::operator()(int_max LinearIndex) const
{
#if defined(MDK_DEBUG_DenseMatrix_Operator_CheckBound)

    if (LinearIndex >= this->GetElementNumber() || LinearIndex < 0)
    {
        MDK_Error("Invalid Input @ DenseMatrix::operator()(i) const")

        return m_MatrixData->NaNElement;
    }

#endif //MDK_DEBUG_DenseMatrix_Operator_CheckBound

	return m_ElementPointer[LinearIndex];
}

// at(): bound check

template<typename ElementType>
inline
ElementType& DenseMatrix<ElementType>::at(int_max LinearIndex)
{
    if (LinearIndex >= this->GetElementNumber() || LinearIndex < 0)
	{
		MDK_Error("Invalid Input @ DenseMatrix::at(i)")
        
        return m_MatrixData->NaNElement;
	}

	return m_ElementPointer[LinearIndex];
}


template<typename ElementType>
inline
const ElementType& DenseMatrix<ElementType>::at(int_max LinearIndex) const
{
    if (LinearIndex >= this->GetElementNumber() || LinearIndex < 0)
	{
		MDK_Error("Invalid Input @ DenseMatrix::at(i) const")
        
        return m_MatrixData->NaNElement;
	}

	return m_ElementPointer[LinearIndex];
}

//----------- Get/Set Matrix(i,j)  ---------------------------------------------//

// operator(): no bound check in release mode

template<typename ElementType>
inline
ElementType& DenseMatrix<ElementType>::operator()(int_max RowIndex, int_max ColIndex)
{
#if defined(MDK_DEBUG_DenseMatrix_Operator_CheckBound)

    auto SelfSize = this->GetSize();

    if (RowIndex >= SelfSize.RowNumber || RowIndex < 0 || ColIndex >= SelfSize.ColNumber || ColIndex < 0)
    {
        MDK_Error("Invalid Input @ DenseMatrix::operator()(i,j)")

        return m_MatrixData->NaNElement;
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

    if (RowIndex >= SelfSize.RowNumber || RowIndex < 0 || ColIndex >= SelfSize.ColNumber || ColIndex < 0)
    {
        MDK_Error("Invalid Input @ DenseMatrix::operator()(i,j) const")

        return m_MatrixData->NaNElement;
    }

#endif //MDK_DEBUG_DenseMatrix_Operator_CheckBound

    return (*m_MatrixData)(RowIndex, ColIndex);
}


template<typename ElementType>
inline
ElementType& DenseMatrix<ElementType>::at(int_max RowIndex, int_max ColIndex)
{
    auto SelfSize = this->GetSize();

    if (RowIndex >= SelfSize.RowNumber || RowIndex < 0 || ColIndex >= SelfSize.ColNumber || ColIndex < 0)
    {
        MDK_Error("Invalid Input @ DenseMatrix::at(i,j)")
        
        return m_MatrixData->NaNElement;
    }
    
    return (*m_MatrixData)(RowIndex, ColIndex);
}


template<typename ElementType>
inline
const ElementType& DenseMatrix<ElementType>::at(int_max RowIndex, int_max ColIndex) const
{
    auto SelfSize = this->GetSize();

    if (RowIndex >= SelfSize.RowNumber || RowIndex < 0 || ColIndex >= SelfSize.ColNumber || ColIndex < 0)
    {
        MDK_Error("Invalid Input @ DenseMatrix::at(i,j) const")
        
        return m_MatrixData->NaNElement;
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

    auto Self_ElementNumber = this->GetElementNumber();

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

    auto Self_ElementNumber = this->GetElementNumber();

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
DenseMatrix<ElementType>::operator()(std::vector<int_max> LinearIndexList)
{
#if defined(MDK_DEBUG_DenseMatrix_Operator_CheckBound)

    if (LinearIndexList.size() <= 0)
    {
        MDK_Error("LinearIndexList is empty @ DenseMatrix::operator()(std::vector {LinearIndexList})")
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto Self_ElementNumber = this->GetElementNumber();

    for (auto it = LinearIndexList.begin(); it != LinearIndexList.end(); ++it)
    {
        if (*it >= Self_ElementNumber || *it < 0)
        {
            MDK_Error("Invalid LinearIndexList @ DenseMatrix::operator()(std::vector {LinearIndexList})")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

#endif //MDK_DEBUG_DenseMatrix_Operator_CheckBound

    DenseShadowMatrix<ElementType> tempShadowMatrix(*this, std::move(LinearIndexList));

    return tempShadowMatrix;
}


template<typename ElementType>
inline
const DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::operator()(std::vector<int_max> LinearIndexList) const
{
#if defined(MDK_DEBUG_DenseMatrix_Operator_CheckBound)

    if (LinearIndexList.size() <= 0)
    {
        MDK_Error("LinearIndexList is empty @ DenseMatrix::operator()(std::vector {LinearIndexList}) const")
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto Self_ElementNumber = this->GetElementNumber();

    for (auto it = LinearIndexList.begin(); it != LinearIndexList.end(); ++it)
    {
        if (*it >= Self_ElementNumber || *it < 0)
        {
            MDK_Error("Invalid LinearIndexList @ DenseMatrix::operator()(std::vector {LinearIndexList}) const")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

#endif //MDK_DEBUG_DenseMatrix_Operator_CheckBound

    DenseShadowMatrix<ElementType> tempShadowMatrix(*this, std::move(LinearIndexList));

    return tempShadowMatrix;
}


template<typename ElementType>
inline
DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::operator()(const DenseVector<int_max>& LinearIndexList)
{
    return (*this)(LinearIndexList.CreateStdVector());
}


template<typename ElementType>
inline
const DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::operator()(const DenseVector<int_max>& LinearIndexList) const
{
    return (*this)(LinearIndexList.CreateStdVector());
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

    auto Self_ElementNumber = this->GetElementNumber();

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

    auto Self_ElementNumber = this->GetElementNumber();

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

    auto Self_ElementNumber = this->GetElementNumber();

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

    auto Self_ElementNumber = this->GetElementNumber();

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
DenseMatrix<ElementType>::at(std::vector<int_max> LinearIndexList)
{
    if (LinearIndexList.size() <= 0)
    {
        MDK_Error("LinearIndexList is empty @ DenseMatrix::at(std::vector {LinearIndexList})")
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto Self_ElementNumber = this->GetElementNumber();

    for (auto it = LinearIndexList.begin(); it != LinearIndexList.end(); ++it)
    {
        if (*it >= Self_ElementNumber || *it < 0)
        {
            MDK_Error("Invalid LinearIndexList @ DenseMatrix::at(std::vector {LinearIndexList})")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    DenseShadowMatrix<ElementType> tempShadowMatrix(*this, std::move(LinearIndexList));

    return tempShadowMatrix;
}


template<typename ElementType>
inline
const DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::at(std::vector<int_max> LinearIndexList) const
{
    if (LinearIndexList.size() <= 0)
    {
        MDK_Error("LinearIndexList is empty @ DenseMatrix::at(std::vector {LinearIndexList} const)")
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto Self_ElementNumber = this->GetElementNumber();

    for (auto it = LinearIndexList.begin(); it != LinearIndexList.end(); ++it)
    {
        if (*it >= Self_ElementNumber || *it < 0)
        {
            MDK_Error("Invalid LinearIndexList @ DenseMatrix::at(std::vector {LinearIndexList} const)")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    DenseShadowMatrix<ElementType> tempShadowMatrix(*this, std::move(LinearIndexList));

    return tempShadowMatrix;
}


template<typename ElementType>
inline
DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::at(const DenseVector<int_max>& LinearIndexList)
{
    return this->at(LinearIndexList.CreateStdVector());
}


template<typename ElementType>
inline
const DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::at(const DenseVector<int_max>& LinearIndexList) const
{
    return this->at(LinearIndexList.CreateStdVector());
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

    auto Self_ElementNumber = this->GetElementNumber();

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

    auto Self_ElementNumber = this->GetElementNumber();

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
        if (*it >= SelfSize.RowNumber || *it < 0)
        {
            MDK_Error("Invalid RowIndexList @ DenseMatrix::operator()(std::initializer_list {RowIndexList}, {ColIndexList})")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= SelfSize.ColNumber || *it < 0)
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
        if (*it >= SelfSize.RowNumber || *it < 0)
        {
            MDK_Error("Invalid RowIndexList @ DenseMatrix::operator()(std::initializer_list {RowIndexList}, {ColIndexList}) const")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= SelfSize.ColNumber || *it < 0)
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
        if (*it >= SelfSize.RowNumber || *it < 0)
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
        if (*it >= SelfSize.RowNumber || *it < 0)
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
        if (*it >= SelfSize.ColNumber || *it < 0)
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
        if (*it >= SelfSize.ColNumber || *it < 0)
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
DenseMatrix<ElementType>::operator()(std::vector<int_max> RowIndexList, std::vector<int_max> ColIndexList)
{
#if defined(MDK_DEBUG_DenseMatrix_Operator_CheckBound)

    if (RowIndexList.size() <= 0 || ColIndexList.size() <= 0)
    {
        MDK_Error("RowIndexList or  ColIndexList is empty @ DenseMatrix::operator()(std::vector {RowIndexList}, {ColIndexList})")
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = RowIndexList.begin(); it != RowIndexList.end(); ++it)
    {
        if (*it >= SelfSize.RowNumber || *it < 0)
        {
            MDK_Error("Invalid RowIndexList @ DenseMatrix::operator()(std::vector {RowIndexList}, {ColIndexList})")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= SelfSize.ColNumber || *it < 0)
        {
            MDK_Error("Invalid ColIndexList @ DenseMatrix::operator()(std::vector {RowIndexList}, {ColIndexList})")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

#endif //MDK_DEBUG_DenseMatrix_Operator_CheckBound

    DenseShadowMatrix<ElementType> tempShadowMatrix(*this, std::move(RowIndexList), std::move(ColIndexList));

    return tempShadowMatrix;
}


template<typename ElementType>
inline
const DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::operator()(std::vector<int_max> RowIndexList, std::vector<int_max> ColIndexList) const 
{
#if defined(MDK_DEBUG_DenseMatrix_Operator_CheckBound)

    if (RowIndexList.size() <= 0 || ColIndexList.size() <= 0)
    {
        MDK_Error("RowIndexList or  ColIndexList is empty @ DenseMatrix::operator()(std::vector {RowIndexList}, {ColIndexList}) const")
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = RowIndexList.begin(); it != RowIndexList.end(); ++it)
    {
        if (*it >= SelfSize.RowNumber || *it < 0)
        {
            MDK_Error("Invalid RowIndexList @ DenseMatrix::operator()(std::vector {RowIndexList}, {ColIndexList}) const")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= SelfSize.ColNumber)
        {
            MDK_Error("Invalid ColIndexList @ DenseMatrix::operator()(std::vector {RowIndexList}, {ColIndexList}) const")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

#endif //MDK_DEBUG_DenseMatrix_Operator_CheckBound

    DenseShadowMatrix<ElementType> tempShadowMatrix(*this, std::move(RowIndexList), std::move(ColIndexList));

    return tempShadowMatrix;
}


template<typename ElementType>
inline
DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::operator()(std::vector<int_max> RowIndexList, const MDK_Symbol_ALL& ALL_Symbol)
{
#if defined(MDK_DEBUG_DenseMatrix_Operator_CheckBound)

    if (RowIndexList.size() <= 0)
    {
        MDK_Error("RowIndexList or  ColIndexList is empty @ DenseMatrix::operator()(std::vector {RowIndexList}, ALL)")
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = RowIndexList.begin(); it != RowIndexList.end(); ++it)
    {
        if (*it >= SelfSize.RowNumber || *it < 0)
        {
            MDK_Error("Invalid RowIndexList @ DenseMatrix::operator()(std::vector {RowIndexList}, ALL)")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

#endif //MDK_DEBUG_DenseMatrix_Operator_CheckBound

    DenseShadowMatrix<ElementType> tempShadowMatrix(*this, std::move(RowIndexList), ALL_Symbol);

    return tempShadowMatrix;
}


template<typename ElementType>
inline
const DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::operator()(std::vector<int_max> RowIndexList, const MDK_Symbol_ALL& ALL_Symbol) const
{
#if defined(MDK_DEBUG_DenseMatrix_Operator_CheckBound)

    if (RowIndexList.size() <= 0)
    {
        MDK_Error("RowIndexList or  ColIndexList is empty @ DenseMatrix::operator()(std::vector {RowIndexList}, ALL) const")
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = RowIndexList.begin(); it != RowIndexList.end(); ++it)
    {
        if (*it >= SelfSize.RowNumber || *it < 0)
        {
            MDK_Error("Invalid RowIndexList @ DenseMatrix::operator()(std::vector {RowIndexList}, ALL) const")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

#endif //MDK_DEBUG_DenseMatrix_Operator_CheckBound

    DenseShadowMatrix<ElementType> tempShadowMatrix(*this, std::move(RowIndexList), ALL_Symbol);

    return tempShadowMatrix;
}


template<typename ElementType>
inline
DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::operator()(const MDK_Symbol_ALL& ALL_Symbol, std::vector<int_max> ColIndexList)
{
#if defined(MDK_DEBUG_DenseMatrix_Operator_CheckBound)

    if (ColIndexList.size() <= 0)
    {
        MDK_Error("ColIndexList is empty @ DenseMatrix::operator()(ALL, std::vector {ColIndexList})")
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= SelfSize.ColNumber || *it < 0)
        {
            MDK_Error("Invalid ColIndexList @ DenseMatrix::operator()(ALL, std::vector {ColIndexList})")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

#endif //MDK_DEBUG_DenseMatrix_Operator_CheckBound

    DenseShadowMatrix<ElementType> tempShadowMatrix(*this, ALL_Symbol, std::move(ColIndexList));

    return tempShadowMatrix;
}


template<typename ElementType>
inline
const DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::operator()(const MDK_Symbol_ALL& ALL_Symbol, std::vector<int_max> ColIndexList) const
{
#if defined(MDK_DEBUG_DenseMatrix_Operator_CheckBound)

    if (ColIndexList.size() <= 0)
    {
        MDK_Error("ColIndexList is empty @ DenseMatrix::operator()(ALL, std::vector {ColIndexList}) const")
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= SelfSize.ColNumber || *it < 0)
        {
            MDK_Error("Invalid ColIndexList @ DenseMatrix::operator()(ALL, std::vector {ColIndexList}) const")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

#endif //MDK_DEBUG_DenseMatrix_Operator_CheckBound

    DenseShadowMatrix<ElementType> tempShadowMatrix(*this, ALL_Symbol, std::move(ColIndexList));

    return tempShadowMatrix;
}


template<typename ElementType>
inline
DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::operator()(const DenseVector<int_max>& RowIndexList, const DenseVector<int_max>& ColIndexList)
{
    return (*this)(RowIndexList.CreateStdVector(), ColIndexList.CreateStdVector());
}


template<typename ElementType>
inline
const DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::operator()(const DenseVector<int_max>& RowIndexList, const DenseVector<int_max>& ColIndexList) const
{
    return (*this)(RowIndexList.CreateStdVector(), ColIndexList.CreateStdVector());
}


template<typename ElementType>
inline
DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::operator()(const DenseVector<int_max>& RowIndexList, const MDK_Symbol_ALL& ALL_Symbol)
{
    return (*this)(RowIndexList.CreateStdVector(), ALL_Symbol);
}


template<typename ElementType>
inline
const DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::operator()(const DenseVector<int_max>& RowIndexList, const MDK_Symbol_ALL& ALL_Symbol) const
{
    return (*this)(RowIndexList.CreateStdVector(), ALL_Symbol);
}


template<typename ElementType>
inline
DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::operator()(const MDK_Symbol_ALL& ALL_Symbol, const DenseVector<int_max>& ColIndexList)
{
    return (*this)(ALL_Symbol, ColIndexList.CreateStdVector());
}


template<typename ElementType>
inline
const DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::operator()(const MDK_Symbol_ALL& ALL_Symbol, const DenseVector<int_max>& ColIndexList) const
{
    return (*this)(ALL_Symbol, ColIndexList.CreateStdVector());
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
        if (*it >= SelfSize.RowNumber || *it < 0)
        {
            MDK_Error("Invalid RowIndexList @ DenseMatrix::operator()(DenseMatrix {RowIndexList}, {ColIndexList})")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= SelfSize.ColNumber)
        {
            MDK_Error("Invalid ColIndexList @ DenseMatrix::operator()(DenseMatrix {RowIndexList}, {ColIndexList})")
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
        if (*it >= SelfSize.RowNumber || *it < 0)
        {
            MDK_Error("Invalid RowIndexList @ DenseMatrix::operator()(DenseMatrix {RowIndexList}, {ColIndexList}) const")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= SelfSize.ColNumber)
        {
            MDK_Error("Invalid ColIndexList @ DenseMatrix::operator()(DenseMatrix {RowIndexList}, {ColIndexList}) const")
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
        if (*it >= SelfSize.RowNumber || *it < 0)
        {
            MDK_Error("Invalid RowIndexList @ DenseMatrix::operator()(DenseMatrix: {RowIndexList}, ALL)")
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
        if (*it >= SelfSize.RowNumber || *it < 0)
        {
            MDK_Error("Invalid RowIndexList @ DenseMatrix::operator()(DenseMatrix: {RowIndexList}, ALL) const")
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
        if (*it >= SelfSize.ColNumber || *it < 0)
        {
            MDK_Error("Invalid ColIndexList @ DenseMatrix::operator()(DenseMatrix: ALL, ColIndexList)")
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
        if (*it >= SelfSize.ColNumber || *it < 0)
        {
            MDK_Error("Invalid ColIndexList @ DenseMatrix::operator()(DenseMatrix: ALL, ColIndexList) const")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

#endif //MDK_DEBUG_DenseMatrix_Operator_CheckBound

    DenseShadowMatrix<ElementType> tempShadowMatrix(*this, ALL_Symbol, ColIndexList);

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
        if (*it >= SelfSize.RowNumber || *it < 0)
        {
            MDK_Error("Invalid RowIndexList @ DenseMatrix::at(std::initializer_list {RowIndexList}, {ColIndexList})")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= SelfSize.ColNumber || *it < 0)
        {
            MDK_Error("Invalid ColIndexList @ DenseMatrix::at({RowIndexList}, {ColIndexList})")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    DenseShadowMatrix<ElementType> tempShadowMatrix(*this, RowIndexList, ColIndexList);

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
        if (*it >= SelfSize.RowNumber || *it < 0)
        {
            MDK_Error("Invalid RowIndexList @ DenseMatrix::at(std::initializer_list {RowIndexList}, {ColIndexList}) const")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= SelfSize.ColNumber || *it < 0)
        {
            MDK_Error("Invalid ColIndexList @ DenseMatrix::at({RowIndexList}, {ColIndexList})")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    DenseShadowMatrix<ElementType> tempShadowMatrix(*this, RowIndexList, ColIndexList);

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
        if (*it >= SelfSize.RowNumber || *it < 0)
        {
            MDK_Error("Invalid RowIndexList @ DenseMatrix::at(std::initializer_list {RowIndexList}, ALL)")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    DenseShadowMatrix<ElementType> tempShadowMatrix(*this, RowIndexList, ALL_Symbol);

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
        if (*it >= SelfSize.RowNumber || *it < 0)
        {
            MDK_Error("Invalid RowIndexList @ DenseMatrix::at(std::initializer_list {RowIndexList}, ALL) const")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    DenseShadowMatrix<ElementType> tempShadowMatrix(*this, RowIndexList, ALL_Symbol);

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
        if (*it >= SelfSize.ColNumber || *it < 0)
        {
            MDK_Error("Invalid ColIndexList @ DenseMatrix::at(ALL, std::initializer_list {ColIndexList})")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    DenseShadowMatrix<ElementType> tempShadowMatrix(*this, ALL_Symbol, ColIndexList);

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
        if (*it >= SelfSize.ColNumber || *it < 0)
        {
            MDK_Error("Invalid ColIndexList @ DenseMatrix::at(ALL, std::initializer_list {ColIndexList}) const")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    DenseShadowMatrix<ElementType> tempShadowMatrix(*this, ALL_Symbol, ColIndexList);

    return tempShadowMatrix;
}


template<typename ElementType>
inline
DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::at(std::vector<int_max> RowIndexList, std::vector<int_max> ColIndexList)
{
    if (RowIndexList.size() <= 0 || ColIndexList.size() <= 0)
    {
        MDK_Error("RowIndexList or  ColIndexList is empty @ DenseMatrix::at(std::vector {RowIndexList}, {ColIndexList})")
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = RowIndexList.begin(); it != RowIndexList.end(); ++it)
    {
        if (*it >= SelfSize.RowNumber || *it < 0)
        {
            MDK_Error("Invalid RowIndexList @ DenseMatrix::at(std::vector {RowIndexList}, {ColIndexList})")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= SelfSize.ColNumber || *it < 0)
        {
            MDK_Error("Invalid ColIndexList @ DenseMatrix::at(std::vector {RowIndexList}, {ColIndexList})")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    DenseShadowMatrix<ElementType> tempShadowMatrix(*this, std::move(RowIndexList), std::move(ColIndexList));

    return tempShadowMatrix;
}


template<typename ElementType>
inline
const DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::at(std::vector<int_max> RowIndexList, std::vector<int_max> ColIndexList) const
{
    if (RowIndexList.size() <= 0 || ColIndexList.size() <= 0)
    {
        MDK_Error("RowIndexList or  ColIndexList is empty @ DenseMatrix::at(std::vector {RowIndexList}, {ColIndexList}) const")
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = RowIndexList.begin(); it != RowIndexList.end(); ++it)
    {
        if (*it >= SelfSize.RowNumber || *it < 0)
        {
            MDK_Error("Invalid RowIndexList @ DenseMatrix::at(std::vector {RowIndexList}, {ColIndexList}) const")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= SelfSize.ColNumber || *it < 0)
        {
            MDK_Error("Invalid ColIndexList @ DenseMatrix::at(std::vector {RowIndexList}, {ColIndexList}) const")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    DenseShadowMatrix<ElementType> tempShadowMatrix(*this, std::move(RowIndexList), std::move(ColIndexList));

    return tempShadowMatrix;
}


template<typename ElementType>
inline
DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::at(std::vector<int_max> RowIndexList, const MDK_Symbol_ALL& ALL_Symbol)
{
    if (RowIndexList.size() <= 0)
    {
        MDK_Error("RowIndexList is empty @ DenseMatrix::at(std::vector {RowIndexList}, ALL)")
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = RowIndexList.begin(); it != RowIndexList.end(); ++it)
    {
        if (*it >= SelfSize.RowNumber || *it < 0)
        {
            MDK_Error("Invalid RowIndexList @ DenseMatrix::at(std::vector {RowIndexList}, ALL)")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    DenseShadowMatrix<ElementType> tempShadowMatrix(*this, std::move(RowIndexList), ALL_Symbol);

    return tempShadowMatrix;
}


template<typename ElementType>
inline
const DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::at(std::vector<int_max> RowIndexList, const MDK_Symbol_ALL& ALL_Symbol) const
{
    if (RowIndexList.size() <= 0)
    {
        MDK_Error("RowIndexList is empty @ DenseMatrix::at(std::vector {RowIndexList}, ALL) const")
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = RowIndexList.begin(); it != RowIndexList.end(); ++it)
    {
        if (*it >= SelfSize.RowNumber || *it < 0)
        {
            MDK_Error("Invalid RowIndexList @ DenseMatrix::at(std::vector {RowIndexList}, ALL) const")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    DenseShadowMatrix<ElementType> tempShadowMatrix(*this, std::move(RowIndexList), ALL_Symbol);

    return tempShadowMatrix;
}


template<typename ElementType>
inline
DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::at(const MDK_Symbol_ALL& ALL_Symbol, std::vector<int_max> ColIndexList)
{
    if (ColIndexList.size() <= 0)
    {
        MDK_Error("ColIndexList is empty @ DenseMatrix::at(ALL, std::vector {ColIndexList})")
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= SelfSize.ColNumber || *it < 0)
        {
            MDK_Error("Invalid ColIndexList @ DenseMatrix::at(ALL, std::vector {ColIndexList})")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    DenseShadowMatrix<ElementType> tempShadowMatrix(*this, ALL_Symbol, std::move(ColIndexList));

    return tempShadowMatrix;
}


template<typename ElementType>
inline
const DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::at(const MDK_Symbol_ALL& ALL_Symbol, std::vector<int_max> ColIndexList) const
{
    if (ColIndexList.size() <= 0)
    {
        MDK_Error("ColIndexList is empty @ DenseMatrix::at(ALL, std::vector {ColIndexList}) const")
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= SelfSize.ColNumber || *it < 0)
        {
            MDK_Error("Invalid ColIndexList @ DenseMatrix::at(ALL, std::vector {ColIndexList}) const")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    DenseShadowMatrix<ElementType> tempShadowMatrix(*this, ALL_Symbol, std::move(ColIndexList));

    return tempShadowMatrix;
}


template<typename ElementType>
inline
DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::at(const DenseVector<int_max>& RowIndexList, const DenseVector<int_max>& ColIndexList)
{
    return this->at(RowIndexList.CreateStdVector(), ColIndexList.CreateStdVector());
}


template<typename ElementType>
inline
const DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::at(const DenseVector<int_max>& RowIndexList, const DenseVector<int_max>& ColIndexList) const
{
    return this->at(RowIndexList.CreateStdVector(), ColIndexList.CreateStdVector());
}


template<typename ElementType>
inline
DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::at(const DenseVector<int_max>& RowIndexList, const MDK_Symbol_ALL& ALL_Symbol)
{
    return this->at(RowIndexList.CreateStdVector(), ALL_Symbol);
}


template<typename ElementType>
inline
const DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::at(const DenseVector<int_max>& RowIndexList, const MDK_Symbol_ALL& ALL_Symbol) const
{
    return this->at(RowIndexList.CreateStdVector(), ColIndexList.CreateStdVector());
}


template<typename ElementType>
inline
DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::at(const MDK_Symbol_ALL& ALL_Symbol, const DenseVector<int_max>& ColIndexList)
{
    return this->at(ALL_Symbol, ColIndexList.CreateStdVector());
}


template<typename ElementType>
inline
const DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::at(const MDK_Symbol_ALL& ALL_Symbol, const DenseVector<int_max>& ColIndexList) const
{
    return this->at(ALL_Symbol, ColIndexList.CreateStdVector());
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
        if (*it >= SelfSize.RowNumber || *it < 0)
        {
            MDK_Error("Invalid RowIndexList @ DenseMatrix::at(DenseMatrix: {RowIndexList}, {ColIndexList})")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= SelfSize.ColNumber || *it < 0)
        {
            MDK_Error("Invalid ColIndexList @ DenseMatrix::at(DenseMatrix: {RowIndexList}, {ColIndexList})")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    DenseShadowMatrix<ElementType> tempShadowMatrix(*this, RowIndexList, ColIndexList);

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
        if (*it >= SelfSize.RowNumber || *it < 0)
        {
            MDK_Error("Invalid RowIndexList @ DenseMatrix::at(DenseMatrix: {RowIndexList}, {ColIndexList}) const")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= SelfSize.ColNumber || *it < 0)
        {
            MDK_Error("Invalid ColIndexList @ DenseMatrix::at(DenseMatrix: {RowIndexList}, {ColIndexList}) const")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    DenseShadowMatrix<ElementType> tempShadowMatrix(*this, RowIndexList, ColIndexList);

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
        if (*it >= SelfSize.RowNumber || *it < 0)
        {
            MDK_Error("Invalid RowIndexList @ DenseMatrix::at(DenseMatrix: {RowIndexList}, ALL)")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    DenseShadowMatrix<ElementType> tempShadowMatrix(*this, RowIndexList, ALL_Symbol);

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
        if (*it >= SelfSize.RowNumber || *it < 0)
        {
            MDK_Error("Invalid RowIndexList @ DenseMatrix::at(DenseMatrix: {RowIndexList}, ALL) const")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    DenseShadowMatrix<ElementType> tempShadowMatrix(*this, RowIndexList, ALL_Symbol);

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
        if (*it >= SelfSize.ColNumber || *it < 0)
        {
            MDK_Error("Invalid ColIndexList @ DenseMatrix::at(DenseMatrix: ALL, {ColIndexList})")
                DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    DenseShadowMatrix<ElementType> tempShadowMatrix(*this, ALL_Symbol, ColIndexList);

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
        if (*it >= SelfSize.ColNumber || *it < 0)
        {
            MDK_Error("Invalid ColIndexList @ DenseMatrix::at(DenseMatrix: ALL, ColIndexList) const")
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    DenseShadowMatrix<ElementType> tempShadowMatrix(*this, ALL_Symbol, ColIndexList);

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
DenseMatrix<ElementType>::Col(std::vector<int_max> ColIndexList)
{
    return (*this)(ALL, std::move(ColIndexList));
}


template<typename ElementType>
inline
const DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::Col(std::vector<int_max> ColIndexList) const
{
    return (*this)(ALL, std::move(ColIndexList));
}


template<typename ElementType>
inline
DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::Col(const DenseVector<int_max>& ColIndexList)
{
    return (*this)(ALL, ColIndexList.CreateStdVector());
}


template<typename ElementType>
inline
const DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::Col(const DenseVector<int_max>& ColIndexList) const
{
    return (*this)(ALL, ColIndexList.CreateStdVector());
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
DenseMatrix<ElementType>::Row(std::vector<int_max> RowIndexList)
{
    return (*this)(std::move(RowIndexList), ALL);
}


template<typename ElementType>
inline
const DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::Row(std::vector<int_max> RowIndexList) const
{
    return (*this)(std::move(RowIndexList), ALL);
}


template<typename ElementType>
inline
DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::Row(const DenseVector<int_max>& RowIndexList)
{
    return (*this)(RowIndexList.CreateStdVector(), ALL);
}


template<typename ElementType>
inline
const DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::Row(const DenseVector<int_max>& RowIndexList) const
{
    return (*this)(RowIndexList.CreateStdVector(), ALL);
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

    if (SelfSize.RowNumber <= 0 || SelfSize.RowNumber != SelfSize.ColNumber)
    {
        MDK_Error(" Self is empty or not square @ DenseMatrix::Diagonal()")
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return  tempShadowMatrix;
    }

    std::vector<int_max> LinearIndexList(SelfSize.ColNumber);

    int_max Index = 0;

    for (int_max i = 0; i < SelfSize.RowNumber; ++i)
    {
        LinearIndexList[j] = Index + i;

        Index += SelfSize.RowNumber;
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

    if (SelfSize.RowNumber <= 0 || SelfSize.RowNumber != SelfSize.ColNumber)
    {
        MDK_Error(" Self is empty or not square @ DenseMatrix::Diagonal() const")
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return  tempShadowMatrix;
    }

    std::vector<int_max> LinearIndexList(SelfSize.ColNumber);

    int_max Index = 0;

    for (int_max i = 0; i < SelfSize.RowNumber; ++i)
    {
        LinearIndexList[j] = Index + i;

        Index += SelfSize.RowNumber;
    }

    DenseShadowMatrix<ElementType> tempShadowMatrix(*this, LinearIndexList);

    return tempShadowMatrix;
}


template<typename ElementType>
inline 
DenseMatrix<ElementType> DenseMatrix<ElementType>::RefCol(int_max ColIndex)
{
    DenseMatrix<ElementType> tempCol(this->GetElementPointerOfCol(ColIndex), this->GetRowNumber(), 1, true);
    return tempCol;
}


template<typename ElementType>
inline 
const DenseMatrix<ElementType> DenseMatrix<ElementType>::RefCol(int_max ColIndex) const
{
    DenseMatrix<ElementType> tempCol(const_cast<ElementType*>(this->GetElementPointerOfCol(ColIndex)), this->GetRowNumber(), 1, true);
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
inline
DenseMatrix<ElementType> DenseMatrix<ElementType>::GetSubMatrix(const std::vector<int_max>& LinearIndexList) const
{
    return this->GetSubMatrix(LinearIndexList.data(), int_max(LinearIndexList.size()));
}


template<typename ElementType>
inline
DenseMatrix<ElementType> DenseMatrix<ElementType>::GetSubMatrix(const DenseVector<int_max>& LinearIndexList) const
{
    return this->GetSubMatrix(LinearIndexList.GetElementPointer(), LinearIndexList.GetElementNumber());
}


template<typename ElementType>
inline
DenseMatrix<ElementType> DenseMatrix<ElementType>::GetSubMatrix(const DenseMatrix<int_max>& LinearIndexList) const
{
    return this->GetSubMatrix(LinearIndexList.GetElementPointer(), LinearIndexList.GetElementNumber());
}


template<typename ElementType>
inline
DenseMatrix<ElementType> DenseMatrix<ElementType>::GetSubMatrix(const int_max* LinearIndexList, int_max ListLength) const
{
    DenseMatrix<ElementType> OutputMatrix;

    auto Self_ElementNumber = this->GetElementNumber();

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
bool DenseMatrix<ElementType>::GetSubMatrix(DenseMatrix<ElementType>& OutputMatrix,
                                            const std::initializer_list<int_max>& RowIndexList,
                                            const std::initializer_list<int_max>& ColIndexList) const
{
    return this->GetSubMatrix(OutputMatrix,
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
bool DenseMatrix<ElementType>::GetSubMatrix(DenseMatrix<ElementType>& OutputMatrix,
                                            const std::initializer_list<int_max>& RowIndexList,
                                            const MDK_Symbol_ALL& ALL_Symbol) const
{
    return this->GetSubMatrix(OutputMatrix, RowIndexList.begin(), int_max(RowIndexList.size()), ALL_Symbol);
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
bool DenseMatrix<ElementType>::GetSubMatrix(DenseMatrix<ElementType>& OutputMatrix,
                                            const MDK_Symbol_ALL& ALL_Symbol,
                                            const std::initializer_list<int_max>& ColIndexList) const
{
    return this->GetSubMatrix(OutputMatrix, ALL_Symbol, ColIndexList.begin(), int_max(ColIndexList.size()));
}


template<typename ElementType>
inline
DenseMatrix<ElementType> DenseMatrix<ElementType>::GetSubMatrix(const std::vector<int_max>& RowIndexList, const std::vector<int_max>& ColIndexList) const
{
    DenseMatrix<ElementType> tempMatrix;

    this->GetSubMatrix(tempMatrix,
                       RowIndexList.data(), int_max(RowIndexList.size()),
                       ColIndexList.data(), int_max(ColIndexList.size()));

    return tempMatrix;
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::GetSubMatrix(DenseMatrix<ElementType>& OutputMatrix,
                                            const std::vector<int_max>& RowIndexList, 
                                            const std::vector<int_max>& ColIndexList) const
{
    return this->GetSubMatrix(OutputMatrix, 
                              RowIndexList.data(), int_max(RowIndexList.size()), 
                              ColIndexList.data(), int_max(ColIndexList.size()));
}


template<typename ElementType>
inline 
DenseMatrix<ElementType> DenseMatrix<ElementType>::GetSubMatrix(const std::vector<int_max>& RowIndexList, 
                                                                const MDK_Symbol_ALL& ALL_Symbol) const
{
    DenseMatrix<ElementType> tempMatrix;

    this->GetSubMatrix(tempMatrix, RowIndexList.data(), int_max(RowIndexList.size()), ALL_Symbol);

    return tempMatrix;
}


template<typename ElementType>
inline 
bool DenseMatrix<ElementType>::GetSubMatrix(DenseMatrix<ElementType>& OutputMatrix,
                                            const std::vector<int_max>& RowIndexList,
                                            const MDK_Symbol_ALL& ALL_Symbol) const
{
    return this->GetSubMatrix(OutputMatrix, RowIndexList.data(), int_max(RowIndexList.size()), ALL_Symbol);
}


template<typename ElementType>
inline
DenseMatrix<ElementType> DenseMatrix<ElementType>::GetSubMatrix(const MDK_Symbol_ALL& ALL_Symbol, 
                                                                const std::vector<int_max>& ColIndexList) const
{
    DenseMatrix<ElementType> tempMatrix;

    this->GetSubMatrix(tempMatrix, ALL_Symbol, ColIndexList.data(), int_max(ColIndexList.size()));

    return tempMatrix;
}


template<typename ElementType>
inline 
bool DenseMatrix<ElementType>::GetSubMatrix(DenseMatrix<ElementType>& OutputMatrix,
                                            const MDK_Symbol_ALL& ALL_Symbol,
                                            const std::vector<int_max>& ColIndexList) const
{
    return this->GetSubMatrix(OutputMatrix, ALL_Symbol, ColIndexList.data(), int_max(ColIndexList.size()));
}


template<typename ElementType>
inline
DenseMatrix<ElementType> DenseMatrix<ElementType>::GetSubMatrix(const DenseVector<int_max>& RowIndexList, const DenseVector<int_max>& ColIndexList) const
{
    DenseMatrix<ElementType> tempMatrix;

    this->GetSubMatrix(tempMatrix, 
                       RowIndexList.GetElementPointer(), RowIndexList.GetElementNumber(), 
                       ColIndexList.GetElementPointer(), ColIndexList.GetElementNumber());

    return tempMatrix;
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::GetSubMatrix(DenseMatrix<ElementType>& OutputMatrix,
                                            const DenseVector<int_max>& RowIndexList,
                                            const DenseVector<int_max>& ColIndexList) const
{
    return this->GetSubMatrix(OutputMatrix,
                              RowIndexList.GetElementPointer(), RowIndexList.GetElementNumber(), 
                              ColIndexList.GetElementPointer(), ColIndexList.GetElementNumber());
}


template<typename ElementType>
inline
DenseMatrix<ElementType> DenseMatrix<ElementType>::GetSubMatrix(const DenseVector<int_max>& RowIndexList,
                                                                const MDK_Symbol_ALL& ALL_Symbol) const
{
    DenseMatrix<ElementType> tempMatrix;

    this->GetSubMatrix(tempMatrix, RowIndexList.GetElementPointer(), RowIndexList.GetElementNumber(), ALL_Symbol);

    return tempMatrix;
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::GetSubMatrix(DenseMatrix<ElementType>& OutputMatrix, 
                                            const DenseVector<int_max>& RowIndexList,
                                            const MDK_Symbol_ALL& ALL_Symbol) const
{
    return this->GetSubMatrix(OutputMatrix, RowIndexList.GetElementPointer(), RowIndexList.GetElementNumber(), ALL_Symbol);
}


template<typename ElementType>
inline
DenseMatrix<ElementType> DenseMatrix<ElementType>::GetSubMatrix(const MDK_Symbol_ALL& ALL_Symbol, 
                                                                const DenseVector<int_max>& ColIndexList) const
{
    DenseMatrix<ElementType> tempMatrix;

    this->GetSubMatrix(tempMatrix, ALL_Symbol, ColIndexList.GetElementPointer(), ColIndexList.GetElementNumber());

    return tempMatrix;
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::GetSubMatrix(DenseMatrix<ElementType>& OutputMatrix, 
                                            const MDK_Symbol_ALL& ALL_Symbol,
                                            const DenseVector<int_max>& ColIndexList) const
{
    return this->GetSubMatrix(OutputMatrix, ALL_Symbol, ColIndexList.GetElementPointer(), ColIndexList.GetElementNumber());
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
                       RowIndexList.GetElementPointer(), RowIndexList.GetElementNumber(), 
                       ColIndexList.GetElementPointer(), ColIndexList.GetElementNumber());

    return tempMatrix;
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::GetSubMatrix(DenseMatrix<ElementType>& OutputMatrix,
                                            const DenseMatrix<int_max>& RowIndexList,
                                            const DenseMatrix<int_max>& ColIndexList) const
{
    if (RowIndexList.IsVector() == false || ColIndexList.IsVector() == false)
    {
        MDK_Error("RowIndexList or ColIndexList is NOT a vector @ DenseMatrix::GetSubMatrix(OutputMatrix, DenseMatrix: RowIndexList, ColIndexList)")
        return false;
    }

    return this->GetSubMatrix(OutputMatrix,
                              RowIndexList.GetElementPointer(), RowIndexList.GetElementNumber(), 
                              ColIndexList.GetElementPointer(), ColIndexList.GetElementNumber());
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

    this->GetSubMatrix(tempMatrix, RowIndexList.GetElementPointer(), RowIndexList.GetElementNumber(), ALL_Symbol);

    return tempMatrix;
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::GetSubMatrix(DenseMatrix<ElementType>& OutputMatrix, 
                                            const DenseMatrix<int_max>& RowIndexList,
                                            const MDK_Symbol_ALL& ALL_Symbol) const
{
    if (RowIndexList.IsVector() == false)
    {
        MDK_Error("RowIndexList is NOT a vector @ DenseMatrix::GetSubMatrix(DenseMatrix: RowIndexList, ALL)")
        return false;
    }

    return this->GetSubMatrix(OutputMatrix, RowIndexList.GetElementPointer(), RowIndexList.GetElementNumber(), ALL_Symbol);
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

    this->GetSubMatrix(tempMatrix, ALL_Symbol, ColIndexList.GetElementPointer(), ColIndexList.GetElementNumber());

    return tempMatrix;
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::GetSubMatrix(DenseMatrix<ElementType>& OutputMatrix, 
                                            const MDK_Symbol_ALL& ALL_Symbol,
                                            const DenseMatrix<int_max>& ColIndexList) const
{
    if (ColIndexList.IsVector() == false)
    {
        MDK_Error("ColIndexList is NOT a vector @ DenseMatrix::GetSubMatrix(OutputMatrix, ALL, DenseMatrix: ColIndexList)")
        return false;
    }
    return this->GetSubMatrix(OutputMatrix, ALL_Symbol, ColIndexList.GetElementPointer(), ColIndexList.GetElementNumber());
}


template<typename ElementType>
inline
DenseMatrix<ElementType> DenseMatrix<ElementType>::GetSubMatrix(const int_max* RowIndexList, int_max OutputRowNumber,
                                                                const int_max* ColIndexList, int_max OutputColNumber) const
{
    DenseMatrix<ElementType> OutputMatrix;
    this->GetSubMatrix(OutputMatrix, RowIndexList, OutputRowNumber, ColIndexList, OutputColNumber);
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::GetSubMatrix(DenseMatrix<ElementType>& OutputMatrix,
                                            const int_max* RowIndexList, int_max OutputRowNumber,
                                            const int_max* ColIndexList, int_max OutputColNumber) const
{
    auto SelfSize = this->GetSize();

    if (SelfSize.ColNumber == 0)
    {
        MDK_Error("Self is empty @ DenseMatrix::GetSubMatrix(OutputMatrix, pointer...)")
        return false;
    }

    if (this->GetElementPointer() == OutputMatrix.GetElementPointer())
    {
        DenseMatrix<ElementType> tempOutputMatrix;
        this->GetSubMatrix(tempOutputMatrix, RowIndexList, OutputRowNumber, ColIndexList, OutputColNumber);
        OutputMatrix.Take(tempOutputMatrix);
        return true;
    }

    if (RowIndexList == nullptr || OutputRowNumber <= 0 || ColIndexList == nullptr || OutputColNumber <= 0)
    {
        MDK_Error("RowIndexList or ColIndexList is empty @ DenseMatrix::GetSubMatrix(OutputMatrix, pointer,..., pointer,...)")
        return false;
    }

    for (int_max i = 0; i < OutputRowNumber; ++i)
    {
        auto RowIndex = RowIndexList[i];

        if (RowIndex < 0 || RowIndex >= SelfSize.RowNumber)
        {
            MDK_Error("Invalid RowIndex @ DenseMatrix::GetSubMatrix(OutputMatrix, pointer,..., pointer,...)")
            return false;
        }
    }

    for (int_max j = 0; j < OutputColNumber; ++j)
    {
        auto ColIndex = ColIndexList[j];

        if (ColIndex < 0 || ColIndex >= SelfSize.ColNumber)
        {
            MDK_Error("Invalid ColIndex @ DenseMatrix::GetSubMatrix(OutputMatrix, pointer,..., pointer,...)")
            return false;
        }
    }

    auto IsOK = OutputMatrix.FastResize(OutputRowNumber, OutputColNumber);
    if (IsOK == false)
    {
        MDK_Error("Invalid OutputMatrix @ DenseMatrix::GetSubMatrix(OutputMatrix, pointer,..., pointer,...)")
        return false;
    }

    auto OutputPointer = OutputMatrix.GetElementPointer();

    auto RawPointer = this->GetElementPointer();

    for (int_max j = 0; j < OutputColNumber; ++j)
    {
        auto ColIndex = ColIndexList[j];
        auto Offset = ColIndex * SelfSize.RowNumber;

        for (int_max i = 0; i < OutputRowNumber; ++i)
        {
            auto RowIndex = RowIndexList[i];
            int_max LinearIndex = Offset + RowIndex;

            OutputPointer[0] = RawPointer[LinearIndex];
            ++OutputPointer;
        }
    }

    return true;
}


template<typename ElementType>
inline
DenseMatrix<ElementType> DenseMatrix<ElementType>::GetSubMatrix(const MDK_Symbol_ALL& ALL_Symbol,
                                                                const int_max* ColIndexList, int_max OutputColNumber) const
{
    DenseMatrix<ElementType> OutputMatrix;
    this->GetSubMatrix(OutputMatrix, ALL_Symbol, ColIndexList, OutputColNumber);
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::GetSubMatrix(DenseMatrix<ElementType>& OutputMatrix,
                                            const MDK_Symbol_ALL& ALL_Symbol, 
                                            const int_max* ColIndexList, int_max OutputColNumber) const
{
    auto SelfSize = this->GetSize();

    if (SelfSize.ColNumber == 0)
    {
        MDK_Error("Self is empty @ DenseMatrix::GetSubMatrix(OutputMatrix, ALL, pointer...)")
        return false;
    }

    if (this->GetElementPointer() == OutputMatrix.GetElementPointer())
    {
        DenseMatrix<ElementType> tempOutputMatrix;
        this->GetSubMatrix(tempOutputMatrix, ALL_Symbol, ColIndexList, OutputColNumber);
        OutputMatrix.Take(tempOutputMatrix);
        return true;
    }

    if (ColIndexList == nullptr || OutputColNumber <= 0)
    {
        MDK_Error("ColIndexList is empty @ DenseMatrix::GetSubMatrix(OutputMatrix, ALL, pointer...)")
        return false;
    }

    for (int_max j = 0; j < OutputColNumber; ++j)
    {
        auto ColIndex = ColIndexList[j];

        if (ColIndex < 0 || ColIndex >= SelfSize.ColNumber)
        {
            MDK_Error("Invalid ColIndex @ DenseMatrix::GetSubMatrix(OutputMatrix, ALL, pointer...)")
            return false;
        }
    }

    auto IsOK = OutputMatrix.FastResize(SelfSize.RowNumber, OutputColNumber);
    if (IsOK == false)
    {
        MDK_Error("Invalid OutputMatrix @ DenseMatrix::GetSubMatrix(OutputMatrix, ALL, pointer...)")
        return false;
    }

    auto OutputPointer = OutputMatrix.GetElementPointer();

    auto RawPointer = this->GetElementPointer();

    for (int_max j = 0; j < OutputColNumber; ++j)
    {
        auto ColIndex = ColIndexList[j];
        auto Offset = ColIndex * SelfSize.RowNumber;

        for (int_max i = 0; i < SelfSize.RowNumber; ++i)
        {
            int_max LinearIndex = Offset + i;

            OutputPointer[0] = RawPointer[LinearIndex];
            ++OutputPointer;
        }
    }

    return true;
}


template<typename ElementType>
inline
DenseMatrix<ElementType> DenseMatrix<ElementType>::GetSubMatrix(const int_max* RowIndexList, int_max OutputRowNumber,
                                                                const MDK_Symbol_ALL& ALL_Symbol) const
{
    DenseMatrix<ElementType> OutputMatrix;
    this->GetSubMatrix(OutputMatrix, RowIndexList, OutputRowNumber, ALL_Symbol);
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::GetSubMatrix(DenseMatrix<ElementType>& OutputMatrix,
                                            const int_max* RowIndexList, int_max OutputRowNumber,
                                            const MDK_Symbol_ALL& ALL_Symbol) const
{
    auto SelfSize = this->GetSize();

    if (SelfSize.RowNumber == 0)
    {
        MDK_Error("Self is empty @ DenseMatrix::GetSubMatrix(OutputMatrix, pointer, ALL)")
        return false;
    }

    if (this->GetElementPointer() == OutputMatrix.GetElementPointer())
    {
        DenseMatrix<ElementType> tempOutputMatrix;
        this->GetSubMatrix(tempOutputMatrix, RowIndexList, OutputRowNumber, ALL_Symbol);
        OutputMatrix.Take(tempOutputMatrix);
        return true;
    }

    if (RowIndexList == nullptr || OutputRowNumber <= 0)
    {
        MDK_Error("RowIndexList is empty @ DenseMatrix::GetSubMatrix(OutputMatrix, pointer, ALL)")
        return false;
    }

    for (int_max i = 0; i < OutputRowNumber; ++i)
    {
        auto RowIndex = RowIndexList[i];

        if (RowIndex < 0 || RowIndex >= SelfSize.RowNumber)
        {
            MDK_Error("Invalid RowIndex @ DenseMatrix::GetSubMatrix(OutputMatrix, pointer, ALL)")
            return false;
        }
    }

    auto IsOK = OutputMatrix.FastResize(OutputRowNumber, SelfSize.ColNumber);
    if (IsOK ==false)
    {
        MDK_Error("Invalid OutputMatrix @ DenseMatrix::GetSubMatrix(OutputMatrix, pointer, ALL)")
        return false;
    }

    auto OutputPointer = OutputMatrix.GetElementPointer();

    auto RawPointer = this->GetElementPointer();

    for (int_max j = 0; j < SelfSize.ColNumber; ++j)
    {
        auto Offset = j * SelfSize.RowNumber;

        for (int_max i = 0; i < OutputRowNumber; ++i)
        {
            auto RowIndex = RowIndexList[i];
           
            int_max LinearIndex = Offset + RowIndex;

            OutputPointer[0] = RawPointer[LinearIndex];

            ++OutputPointer;
        }
    }

    return true;
}




template<typename ElementType>
inline 
DenseMatrix<ElementType> DenseMatrix<ElementType>::GetCol(int_max ColIndex) const
{
	DenseMatrix<ElementType> tempMatrix;

    tempMatrix.Resize(this->GetRowNumber(), 1);

    this->GetCol(ColIndex, tempMatrix.GetElementPointer());

    return tempMatrix;
}


template<typename ElementType>
inline 
bool DenseMatrix<ElementType>::GetCol(int_max ColIndex, std::vector<ElementType>& ColData) const
{
    ColData.resize(this->GetRowNumber());

    return this->GetCol(ColIndex, ColData.data());
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::GetCol(int_max ColIndex, DenseVector<ElementType>& ColData) const
{
    ColData.FastResize(this->GetRowNumber(), 1);

    return this->GetCol(ColIndex, ColData.GetElementPointer());
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::GetCol(int_max ColIndex, DenseMatrix<ElementType>& ColData) const
{
    ColData.FastResize(this->GetRowNumber(), 1);

    return this->GetCol(ColIndex, ColData.GetElementPointer());
}


template<typename ElementType>
inline 
bool DenseMatrix<ElementType>::GetCol(int_max ColIndex, ElementType* ColData) const
{
    auto SelfSize = this->GetSize();

    if (SelfSize.ColNumber == 0)
    {
        MDK_Error("Self is empty @ DenseMatrix::GetCol(ColIndex, ElementType* ColData)")
        return false;
    }

    if (ColIndex >= SelfSize.ColNumber || ColIndex < 0 || ColData == nullptr)
	{
		MDK_Error("Invalid Input @ DenseMatrix::GetCol(int_max ColIndex, ElementType* ColData)")
		return false;
	}

	auto RawPointer = this->GetElementPointer();

    RawPointer += ColIndex*SelfSize.RowNumber;

	for (int_max i = 0; i < SelfSize.RowNumber; ++i)
	{
		ColData[i] = RawPointer[i];
	}

	return true;
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool DenseMatrix<ElementType>::SetCol(int_max ColIndex, const std::initializer_list<ElementType_Input>& ColData)
{
    if (int_max(ColData.size()) != this->GetRowNumber())
    {
        MDK_Error("Invalid Input size @ DenseMatrix::SetCol(ColIndex, std::initializer_list)")
        return false;
    }

    return this->SetCol(ColIndex, ColData.begin());
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool DenseMatrix<ElementType>::SetCol(int_max ColIndex, const std::vector<ElementType_Input>& ColData)
{
    if (int_max(ColData.size()) != this->GetRowNumber())
    {
        MDK_Error("Invalid Input size @ DenseMatrix::SetCol(ColIndex, std::vector)")
        return false;
    }

    return this->SetCol(ColIndex, ColData.data());
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool DenseMatrix<ElementType>::SetCol(int_max ColIndex, const DenseVector<ElementType_Input>& ColData)
{
    if (ColData.GetElementNumber() != this->GetRowNumber())
    {
        MDK_Error("Invalid Input size @ DenseMatrix::SetCol(ColIndex, DenseVector)")
        return false;
    }

    return this->SetCol(ColIndex, ColData.GetElementPointer());
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool DenseMatrix<ElementType>::SetCol(int_max ColIndex, const DenseMatrix<ElementType_Input>& ColData)
{
    if (ColData.IsVector() == false)
    {
        MDK_Error("Invalid Input : must be a vector @ DenseMatrix::SetCol(ColIndex, DenseMatrix)")
        return false;
    }

    if (ColData.GetElementNumber() != this->GetRowNumber())
    {
        MDK_Error("Invalid Input size @ DenseMatrix::SetCol(ColIndex, DenseMatrix)")
        return false;
    }

    return this->SetCol(ColIndex, ColData.GetElementPointer());
}


template<typename ElementType>
template<typename ElementType_Input>
inline 
bool DenseMatrix<ElementType>::SetCol(int_max ColIndex, const ElementType_Input* ColData)
{
    auto SelfSize = this->GetSize();

    if (SelfSize.ColNumber == 0)
    {
        MDK_Error("Self is empty @ DenseMatrix::SetCol(ColIndex, const ElementType_Input* ColData)")
        return false;
    }

    if (ColIndex >= SelfSize.ColNumber || ColIndex < 0 || ColData == nullptr)
	{
		MDK_Error("Invalid Input @ DenseMatrix::SetCol(ColIndex, const ElementType_Input* ColData)")
		return false;
	}
    
    // just for reference:
    // int_max Index = ColIndex*SelfSize.RowNumber;
    //for (int_max i = 0; i < SelfSize.RowNumber; ++i)
    //{
    //	RawPointer[Index + i] = ElementType(ColData[i]);
    //}

    auto tempColData = ColData;

    auto BeginPointer = this->GetElementPointer();
    BeginPointer += ColIndex*SelfSize.RowNumber;

    for (auto Ptr = BeginPointer; Ptr < BeginPointer + SelfSize.RowNumber; ++Ptr)
    {
        Ptr[0] = ElementType(tempColData[0]);

        ++tempColData;
    }

	return true;
}


template<typename ElementType>
inline 
bool DenseMatrix<ElementType>::FillCol(int_max ColIndex, const ElementType& Element)
{
    auto SelfSize = this->GetSize();

    if (SelfSize.ColNumber == 0)
    {
        MDK_Error("Self is empty @ DenseMatrix::FillCol(int_max ColIndex, const ElementType& Element)")
        return false;
    }

    if (ColIndex >= SelfSize.ColNumber || ColIndex < 0)
    {
        MDK_Error("Invalid Input @ DenseMatrix::FillCol(int_max ColIndex, const ElementType& Element)")
        return false;
    }

    auto BeginPointer = this->GetElementPointer() + ColIndex*SelfSize.RowNumber;

    for (auto Ptr = BeginPointer; Ptr < BeginPointer + SelfSize.RowNumber; ++Ptr)
    {
        Ptr[0] = Element;
    }

    return true;
}


template<typename ElementType>
template<typename ElementType_Input>
inline 
bool DenseMatrix<ElementType>::AppendCol(const std::initializer_list<ElementType_Input>& ColData)
{
    return this->AppendCol(ColData.begin(), int_max(ColData.size()));
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool DenseMatrix<ElementType>::AppendCol(const std::vector<ElementType_Input>& ColData)
{
    return this->AppendCol(ColData.data(), int_max(ColData.size()));
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool DenseMatrix<ElementType>::AppendCol(const DenseVector<ElementType_Input>& ColData)
{
    return this->AppendCol(ColData.GetElementPointer(), ColData.GetElementNumber());
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool DenseMatrix<ElementType>::AppendCol(const DenseMatrix<ElementType_Input>& ColData)
{
    if (ColData.IsVector() == false)
    {
        MDK_Error("Input must be a vector @ DenseMatrix::AppendCol(const DenseMatrix<ElementType_Input>& ColData)")
        return false;
    }

    return this->AppendCol(ColData.GetElementPointer(), ColData.GetElementNumber());
}


template<typename ElementType>
template<typename ElementType_Input>
inline 
bool DenseMatrix<ElementType>::AppendCol(const ElementType_Input* ColData, int_max Length)
{
    if (this->IsSizeFixed() == true)
    {
        MDK_Error("Matrix Size can not change @ DenseMatrix::AppendCol(const ElementType_Input* ColData, int_max Length)")
        return false;
    }

    if (ColData == nullptr || Length <= 0)
    {
        MDK_Error("Invalid Input: empty @ DenseMatrix::AppendCol(const ElementType_Input* ColData, int_max Length)")
        return false;
    }

    auto SelfSize = this->GetSize();

    if (SelfSize.RowNumber > 0 && Length != SelfSize.RowNumber)
    {
        MDK_Error("Invalid Input: size does not match @ DenseMatrix::AppendCol(const ElementType_Input* ColData, int_max Length)")
        return false;
    }

    // for reference
    //if (SelfSize.RowNumber == 0)
    //{
    //   SelfSize.RowNumber = Length;
    //}

    this->Resize(Length, SelfSize.ColNumber + 1);

    return this->SetCol(SelfSize.ColNumber, ColData);
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool DenseMatrix<ElementType>::AppendCol(const ElementType_Input* ColData)
{
    auto RowNumber = this->GetRowNumber();

    if (RowNumber == 0)
    {
        MDK_Error("must input RowNumber because Self is empty @ DenseMatrix::AppendCol(...)")
        return false;
    }

    return this->AppendCol(ColData, RowNumber);
}


template<typename ElementType>
inline 
bool DenseMatrix<ElementType>::DeleteCol(int_max ColIndex)
{
    return DeleteCol(&ColIndex, 1);
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::DeleteCol(const std::initializer_list<int_max>& ColIndexList)
{
    return this->DeleteCol(ColIndexList.begin(), int_max(ColIndexList.size()));
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::DeleteCol(const std::vector<int_max>& ColIndexList)
{    
    return this->DeleteCol(ColIndexList.data(), int_max(ColIndexList.size()));
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::DeleteCol(const DenseVector<int_max>& ColIndexList)
{
    return this->DeleteCol(ColIndexList.GetElementPointer(), ColIndexList.GetElementNumber());
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::DeleteCol(const DenseMatrix<int_max>& ColIndexList)
{
    if (ColIndexList.IsVector() == false)
    {
        MDK_Error("Input is NOT a vector @ DenseMatrix::DeleteCol(const DenseMatrix<int_max>& ColIndexList)")
        return false;
    }

    return this->DeleteCol(ColIndexList.GetElementPointer(), ColIndexList.GetElementNumber());
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::DeleteCol(const int_max* ColIndexList, int_max ListLength)
{
    if (ColIndexList == nullptr || ListLength <= 0)
    {
        //MDK_Warning("Empty Input @ DenseMatrix::DeleteCol(const int_max* ColIndexList, int_max ListLength)")
        return true;
    }

    if (this->IsSizeFixed() == true)
    {
        MDK_Error("Matrix Size can not change @ DenseMatrix::DeleteCol(const int_max* ColIndexList, int_max ListLength)")
        return false;
    }

    auto SelfSize = this->GetSize();

    if (SelfSize.ColNumber == 0)
    {
        MDK_Error("Self is empty @ DenseMatrix::DeleteCol(const int_max* ColIndexList, int_max ListLength)")
        return false;
    }

    for (auto it = ColIndexList; it != ColIndexList + ListLength; ++it)
    {
        if (*it >= SelfSize.ColNumber || *it < 0)
        {
            MDK_Error("Out of bound Input @ DenseMatrix::DeleteCol(const int_max* ColIndexList, int_max ListLength)")
            return false;
        }
    }

    m_MatrixData->CopyDataToInternalDataArrayIfNecessary();

    std::vector<int_max> ColIndexList_max_to_min(ListLength);

    for (int_max i = 0; i < ListLength; ++i)
    {
        ColIndexList_max_to_min[i] = ColIndexList[i];
    }

    std::sort(ColIndexList_max_to_min.begin(), ColIndexList_max_to_min.end(), [](int_max a, int_max b) { return a > b; });

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
            m_MatrixData->DataArray.erase(m_MatrixData->DataArray.begin() + Index_i * SelfSize.RowNumber,
                                          m_MatrixData->DataArray.begin() + (Index_i + 1)* SelfSize.RowNumber);

            Index_prev = Index_i;

            m_MatrixData->ColNumber -= 1;
        }
    }

    m_MatrixData->ElementPointer = m_MatrixData->DataArray.data();

    m_ElementPointer = m_MatrixData->ElementPointer;

    return true;
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool DenseMatrix<ElementType>::InsertCol(int_max ColIndex, const std::initializer_list<ElementType_Input>& ColData)
{
    return this->InsertCol(ColIndex, ColData.begin(), int_max(ColData.size()));
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool DenseMatrix<ElementType>::InsertCol(int_max ColIndex, const std::vector<ElementType_Input>& ColData)
{
    return this->InsertCol(ColIndex, ColData.data(), int_max(ColData.size()));
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool DenseMatrix<ElementType>::InsertCol(int_max ColIndex, const DenseVector<ElementType_Input>& ColData)
{
    return this->InsertCol(ColIndex, ColData.GetElementPointer(), ColData.GetElementNumber());
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool DenseMatrix<ElementType>::InsertCol(int_max ColIndex, const DenseMatrix<ElementType_Input>& ColData)
{
    if (ColData.IsVector() == false)
    {
        MDK_Error("Input is NOT a vector @ DenseMatrix::InsertCol(ColIndex, DenseMatrix)")
        return false;
    }

    return this->InsertCol(ColIndex, ColData.GetElementPointer(), ColData.GetElementNumber());
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool DenseMatrix<ElementType>::InsertCol(int_max ColIndex, const ElementType_Input* ColData, int_max Length)
{
    if (this->IsSizeFixed() == true)
    {
        MDK_Error("Matrix Size can not change @ DenseMatrix::InsertCol(ColIndex, const ElementType_Input* ColData, int_max Length)")
        return false;
    }

    auto SelfSize = this->GetSize();

    if (SelfSize.ColNumber == 0)
    {
        if (ColIndex != 0 || ColData == nullptr || Length <= 0)
        {
            MDK_Error("Invalid Input @ DenseMatrix::InsertCol(ColIndex, const ElementType_Input* ColData, int_max Length)")
            return false;
        }
    }
    else
    {
        if (ColIndex >= SelfSize.ColNumber || ColIndex < 0 || ColData == nullptr || Length != SelfSize.RowNumber || Length <= 0)
        {
            MDK_Error("Invalid Input @ DenseMatrix::InsertCol(ColIndex, const ElementType_Input* ColData, int_max Length)")
            return false;
        }
    }

    if (!m_MatrixData)
    {
        this->Resize(0, 0);
    }

    m_MatrixData->CopyDataToInternalDataArrayIfNecessary();

    m_MatrixData->DataArray.insert(m_MatrixData->DataArray.begin() + ColIndex*SelfSize.RowNumber, ColData, ColData + Length);

    m_MatrixData->RowNumber = Length;

    m_MatrixData->ColNumber += 1;

    m_MatrixData->ElementPointer = m_MatrixData->DataArray.data();

    m_ElementPointer = m_MatrixData->ElementPointer;

    return true;
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool DenseMatrix<ElementType>::InsertCol(int_max ColIndex, const ElementType_Input* ColData)
{
    auto RowNumber = this->GetRowNumber();
    if (RowNumber == 0)
    {
        MDK_Error("must input RowNumber because Self is empty @ DenseMatrix::InsertCol(...)")
        return false;
    }

    return this->InsertCol(ColIndex, ColData, RowNumber);
}


template<typename ElementType>
inline
DenseMatrix<ElementType> DenseMatrix<ElementType>::GetRow(int_max RowIndex) const
{
    DenseMatrix<ElementType> tempMatrix;	

    tempMatrix.Resize(1, this->GetColNumber());

    this->GetRow(RowIndex, tempMatrix.GetElementPointer());

    return tempMatrix;
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::GetRow(int_max RowIndex, std::vector<ElementType>& RowData) const
{
	RowData.resize(this->GetColNumber());

    return this->GetRow(RowIndex, RowData.data());
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::GetRow(int_max RowIndex, DenseVector<ElementType>& RowData) const
{
    RowData.FastResize(1, this->GetColNumber());

    return this->GetRow(RowIndex, RowData.GetElementPointer());
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::GetRow(int_max RowIndex, DenseMatrix<ElementType>& RowData) const
{
    RowData.FastResize(1, this->GetColNumber());

    return this->GetRow(RowIndex, RowData.GetElementPointer());
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::GetRow(int_max RowIndex, ElementType* RowData) const
{
    auto SelfSize = this->GetSize();

    if (SelfSize.RowNumber == 0)
    {
        MDK_Error("Self is empty @ DenseMatrix GetRow(RowIndex, ElementType* RowData)")
        return false;
    }

    if (RowIndex >= SelfSize.RowNumber || RowIndex < 0 || RowData == nullptr)
	{
		MDK_Error("Invalid Input @ DenseMatrix GetRow(RowIndex, ElementType* RowData)")
		return false;
	}

	auto RawPointer = this->GetElementPointer();

	int_max Index = 0;

	for (int_max j = 0; j < SelfSize.ColNumber; ++j)
	{
		RowData[j] = RawPointer[Index + RowIndex];

		Index += SelfSize.RowNumber;
	}

	return true;
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool DenseMatrix<ElementType>::SetRow(int_max RowIndex, const std::initializer_list<ElementType_Input>& RowData)
{
    if (int_max(RowData.size()) != this->GetColNumber())
    {
        MDK_Error("Input size is wrong @ DenseMatrix::SetRow(int_max RowIndex, std::initializer_list)")
        return false;
    }

    return this->SetRow(RowIndex, RowData.begin());
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool DenseMatrix<ElementType>::SetRow(int_max RowIndex, const std::vector<ElementType_Input>& RowData)
{
    if (int_max(RowData.size()) != this->GetColNumber())
    {
        MDK_Error("Input size is wrong @ DenseMatrix::SetRow(int_max RowIndex, std::vector)")
        return false;
    }

    return this->SetRow(RowIndex, RowData.data());
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool DenseMatrix<ElementType>::SetRow(int_max RowIndex, const DenseVector<ElementType_Input>& RowData)
{
    if (RowData.GetElementNumber() != this->GetColNumber())
    {
        MDK_Error("Input size is wrong @ DenseMatrix::SetRow(int_max RowIndex, DenseVector)")
        return false;
    }

    return this->SetRow(RowIndex, RowData.GetElementPointer());
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool DenseMatrix<ElementType>::SetRow(int_max RowIndex, const DenseMatrix<ElementType_Input>& RowData)
{
    if (RowData.IsVector() == false)
    {
        MDK_Error("Input must be a vector @ DenseMatrix::SetRow(int_max RowIndex, DenseMatrix)")
        return false;
    }

    if (RowData.GetElementNumber() != this->GetColNumber())
    {
        MDK_Error("Input size is wrong @ DenseMatrix::SetRow(int_max RowIndex, DenseMatrix)")
        return false;
    }

    return this->SetRow(RowIndex, RowData.GetElementPointer());
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool DenseMatrix<ElementType>::SetRow(int_max RowIndex, const ElementType_Input* RowData)
{
    auto SelfSize = this->GetSize();

    if (SelfSize.RowNumber == 0)
    {
        MDK_Error("Self is empty @ DenseMatrix::SetRow(int_max RowIndex, const ElementType_Input* RowData)")
        return false;
    }

    if (RowIndex >= SelfSize.RowNumber || RowIndex < 0 || RowData == nullptr)
    {
        MDK_Error("Invalid Input @ DenseMatrix::SetRow(RowIndex, const ElementType_Input* RowData)")
        return false;
    }

    auto RawPointer = this->GetElementPointer();

    int_max Index = 0;

    for (int_max j = 0; j < SelfSize.ColNumber; ++j)
    {
        RawPointer[Index + RowIndex] = ElementType(RowData[j]);

        Index += SelfSize.RowNumber;
     }

    return true;
}


template<typename ElementType>
inline 
bool DenseMatrix<ElementType>::FillRow(int_max RowIndex, const ElementType& Element)
{
    auto SelfSize = this->GetSize();

    if (SelfSize.RowNumber == 0)
    {
        MDK_Error("Self is empty @ DenseMatrix::FillRow(int_max RowIndex, const ElementType& Element)")
        return false;
    }

    if (RowIndex >= SelfSize.RowNumber || RowIndex < 0)
    {
        MDK_Error("Invalid Input @ DenseMatrix::FillRow(int_max RowIndex, const ElementType& Element)")
        return false;
    }

    auto RawPointer = this->GetElementPointer();

    int_max Index = 0;

    for (int_max j = 0; j < SelfSize.ColNumber; ++j)
    {
        RawPointer[Index + RowIndex] = Element;

        Index += SelfSize.RowNumber;
    }

    return true;
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool  DenseMatrix<ElementType>::AppendRow(const std::initializer_list<ElementType_Input>& RowData)
{
    return this->AppendRow(RowData.begin(), int_max(RowData.size()));
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool  DenseMatrix<ElementType>::AppendRow(const std::vector<ElementType_Input>& RowData)
{    
    return this->AppendRow(RowData.data(), int_max(RowData.size()));
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool DenseMatrix<ElementType>::AppendRow(const DenseVector<ElementType_Input>& RowData)
{
    return this->AppendRow(RowData.GetElementPointer(), RowData.GetElementNumber());
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool DenseMatrix<ElementType>::AppendRow(const DenseMatrix<ElementType_Input>& RowData)
{
    if (RowData.IsVector() == false)
    {
        MDK_Error("Input must be a vector @ DenseMatrix::AppendRow(const DenseMatrix<ElementType_Input>& RowData)")
        return false;
    }

    return this->AppendRow(RowData.GetElementPointer(), RowData.GetElementNumber());
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool DenseMatrix<ElementType>::AppendRow(const ElementType_Input* RowData, int_max Length)
{
    if (this->IsSizeFixed() == true)
    {
        MDK_Error("Size can not change @ DenseMatrix::AppendRow(const ElementType_Input* RowData, int_max Length)")
        return false;
    }

    if (RowData == nullptr || Length <= 0)
    {
        MDK_Error("Invalid Input: empty @ DenseMatrix::AppendRow(const ElementType_Input* RowData, int_max Length)")
        return false;
    }

    auto SelfSize = this->GetSize();

    if (SelfSize.ColNumber > 0 && Length != SelfSize.ColNumber)
    {
        MDK_Error("Invalid Input: size does not match @ DenseMatrix::AppendRow(const ElementType_Input* RowData, int_max Length)")
        return false;
    }

    // for reference
    //if (SelfSize.ColNumber <= 0)
    //{
    //    SelfSize.ColNumber = Length;
    //}

    this->Resize(SelfSize.RowNumber + 1, Length);

    return this->SetRow(SelfSize.RowNumber, RowData);
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool DenseMatrix<ElementType>::AppendRow(const ElementType_Input* RowData)
{
    auto ColNumber = this->GetColNumber();

    if (ColNumber == 0)
    {
        MDK_Error("must input ColNumber because Self is empty @ DenseMatrix::AppendRow(...)")
        return false;
    }

    return this->AppendRow(RowData, ColNumber);
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::DeleteRow(int_max RowIndex)
{
    return this->DeleteRow(&RowIndex, 1);
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::DeleteRow(const std::initializer_list<int_max>& RowIndexList)
{
    return this->DeleteRow(RowIndexList.begin(), int_max(RowIndexList.size()));
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::DeleteRow(const std::vector<int_max>& RowIndexList)
{
    return this->DeleteRow(RowIndexList.data(), int_max(RowIndexList.size()));
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::DeleteRow(const DenseVector<int_max>& RowIndexList)
{
    return this->DeleteRow(RowIndexList.GetElementPointer(), RowIndexList.GetElementNumber());
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::DeleteRow(const DenseMatrix<int_max>& RowIndexList)
{
    if (RowIndexList.IsVector() == false)
    {
        MDK_Error("Input must be a vector @ DenseMatrix::DeleteRow(const DenseMatrix<int_max>& RowIndexList)")
        return false;
    }

    return this->DeleteRow(RowIndexList.GetElementPointer(), RowIndexList.GetElementNumber());
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::DeleteRow(const int_max* RowIndexList, int_max ListLength)
{
    if (RowIndexList == nullptr || ListLength <= 0)
    {
        //MDK_Warning("Empty Input @ DenseMatrix::DeleteRow(const int_max* RowIndexList, int_max ListLength)")
        return true;
    }

    if (this->IsSizeFixed() == true)
    {
        MDK_Error("Size can not change @ DenseMatrix::DeleteRow(const int_max* RowIndexList, int_max ListLength)")
        return false;
    }

    auto SelfSize = this->GetSize();

    if (SelfSize.RowNumber == 0)
    {
        MDK_Error("Self is empty @ DenseMatrix::DeleteRow(const int_max* RowIndexList, int_max ListLength)")
        return false;
    }

    for (auto it = RowIndexList; it != RowIndexList + ListLength; ++it)
    {
        if (*it >= SelfSize.RowNumber || *it < 0)
        {
            MDK_Error("Invalid Input @ DenseMatrix::DeleteRow(const int_max* RowIndexList, int_max ListLength)")
            return false;
        }
    }

    std::vector<int_max> CounterList(SelfSize.RowNumber);
    
    for (int_max i = 0; i < SelfSize.RowNumber; ++i)
    {
        CounterList[i] = 0;
    }

    for (int_max i = 0; i < ListLength; ++i)
    {
        CounterList[RowIndexList[i]] = 1;
    }

    std::vector<int_max> RowIndexList_output;

    for (int_max i = 0; i < SelfSize.RowNumber; ++i)
    {
        if (CounterList[i] <= 0)
        {
            RowIndexList_output.push_back(i);
        }
    }

    if (RowIndexList_output.size() > 0)
    {
        this->Take(this->GetSubMatrix(RowIndexList_output, ALL));
    }
    else
    {
        this->Clear();
    }

    return true;
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool DenseMatrix<ElementType>::InsertRow(int_max RowIndex, const std::initializer_list<ElementType_Input>& RowData)
{
    return this->InsertRow(RowIndex, RowData.begin(), int_max(RowData.size()));
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool DenseMatrix<ElementType>::InsertRow(int_max RowIndex, const std::vector<ElementType_Input>& RowData)
{
    return this->InsertRow(RowIndex, RowData.data(), int_max(RowData.size()));
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool DenseMatrix<ElementType>::InsertRow(int_max RowIndex, const DenseVector<ElementType_Input>& RowData)
{
    return this->InsertRow(RowIndex, RowData.GetElementPointer(), RowData.GetElementNumber());
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool DenseMatrix<ElementType>::InsertRow(int_max RowIndex, const DenseMatrix<ElementType_Input>& RowData)
{
    if (RowData.IsVector() == false)
    {
        MDK_Error("Input is NOT a vector @ DenseMatrix::InsertRow(int_max RowIndex, const DenseMatrix<ElementType_Input>& RowData)")
        return false;
    }

    return this->InsertRow(RowIndex, RowData.GetElementPointer(), RowData.GetElementNumber());
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool DenseMatrix<ElementType>::InsertRow(int_max RowIndex, const ElementType_Input* RowData, int_max Length)
{
    if (this->IsSizeFixed() == true)
    {
        MDK_Error("Size can not change @ DenseMatrix::InsertRow(RowIndex, const ElementType_Input* RowData, int_max Length)")
        return false;
    }

    if (this->IsEmpty() == true)
    {
        if (RowIndex == 0 && RowData != nullptr && Length > 0)
        {
            return this->AppendRow(RowData, Length);
        }

        MDK_Error("Invalid Input @ DenseMatrix::InsertRow(RowIndex, const ElementType_Input* RowData, int_max Length)")
        return false;
    }

    // Self is not empty ----------------------------------------------------------------------------------------------------------

    auto SelfSize = this->GetSize();

    if (RowIndex >= SelfSize.RowNumber || RowIndex < 0 || RowData == nullptr || Length != SelfSize.ColNumber)
    {
        MDK_Error("Invalid Input @ DenseMatrix::InsertRow(RowIndex, const ElementType_Input* RowData, int_max Length)")
        return false;
    }

    DenseMatrix<ElementType> tempMatrix(SelfSize.RowNumber + 1, SelfSize.ColNumber);

    auto tempRawPointer = tempMatrix.GetElementPointer();

    auto RawPointer = this->GetElementPointer();

    for (int_max j = 0; j < SelfSize.ColNumber; ++j)
    {
        auto tempIndex = j*SelfSize.RowNumber;

        for (int_max i = 0; i < RowIndex; ++i)
        {
            tempRawPointer[tempIndex + j + i] = RawPointer[tempIndex + i];
        }
    }

    for (int_max j = 0; j < SelfSize.ColNumber; ++j)
    {
        tempRawPointer[j*(SelfSize.RowNumber + 1) + RowIndex] = ElementType(RowData[j]);
    }

    for (int_max j = 0; j < SelfSize.ColNumber; ++j)
    {
        auto tempIndex = j*SelfSize.RowNumber;

        for (int_max i = RowIndex + 1; i < SelfSize.RowNumber + 1; ++i)
        {            
            tempRawPointer[tempIndex + j + i] = RawPointer[tempIndex + i - 1];
        }
    }

    //---------------------------------------------------

    this->Take(tempMatrix);

    //-------------------------------------------------------

    return true;
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool DenseMatrix<ElementType>::InsertRow(int_max RowIndex, const ElementType_Input* RowData)
{
    auto ColNumber = this->GetColNumber();
    if (ColNumber == 0)
    {
        MDK_Error("must input ColNumber because Self is empty @ DenseMatrix::InsertRow(...)")
        return false;
    }

    return this->InsertRow(RowIndex, RowData, ColNumber);
}


template<typename ElementType>
inline
DenseMatrix<ElementType> DenseMatrix<ElementType>::GetDiagonal() const
{
    DenseMatrix<ElementType> tempMatrix; 

    auto SelfSize = this->GetSize();

    if (SelfSize.RowNumber <= 0 || SelfSize.RowNumber != SelfSize.ColNumber)
    {
        MDK_Error(" Self is empty or not square @ DenseMatrix::GetDiagonal()")
        
        return  tempMatrix;
    }

    tempMatrix.Resize(SelfSize.RowNumber, 1);

    auto tempRawPointer = tempMatrix.GetElementPointer();

    this->GetDiagonal(tempRawPointer);

    return tempMatrix;
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::Append(const ElementType& Element)
{
    if (this->IsEmpty() == true)
    {
        return this->AppendRow({ Element });
    }

    if (this->IsRowVector() == true)
    {
        return this->AppendCol({ Element });
    }
    else if (this->IsColVector() == true)
    {
        return this->AppendRow({ Element });
    }
    else
    {
        MDK_Error("Self is not a vector @ DenseMatrix::Append(...)")
        return false;
    }
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool DenseMatrix<ElementType>::Append(const std::initializer_list<ElementType_Input>& ElementData)
{
    if (this->IsEmpty() == true)
    {
        return this->AppendRow(ElementData);
    }

    if (this->IsRowVector() == true)
    {
        return this->AppendCol(ElementData);
    }
    else if (this->IsColVector() == true)
    {
        return this->AppendRow(ElementData);
    }
    else
    {
        MDK_Error("Self is not a vector @ DenseMatrix::Append(...)")
        return false;
    }
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool DenseMatrix<ElementType>::Append(const std::vector<ElementType_Input>& ElementData)
{
    if (this->IsEmpty() == true)
    {
        return this->AppendRow(ElementData);
    }

    if (this->IsRowVector() == true)
    {
        return this->AppendCol(ElementData);
    }
    else if (this->IsColVector() == true)
    {
        return this->AppendRow(ElementData);
    }
    else
    {
        MDK_Error("Self is not a vector @ DenseMatrix::Append(...)")
        return false;
    }
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool DenseMatrix<ElementType>::Append(const DenseVector<ElementType_Input>& ElementData)
{
    if (this->IsEmpty() == true)
    {
        return this->AppendRow(ElementData);
    }

    if (this->IsRowVector() == true)
    {
        return this->AppendCol(ElementData);
    }
    else if (this->IsColVector() == true)
    {
        return this->AppendRow(ElementData);
    }
    else
    {
        MDK_Error("Self is not a vector @ DenseMatrix::Append(...)")
        return false;
    }
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool DenseMatrix<ElementType>::Append(const DenseMatrix<ElementType_Input>& ElementData)
{
    if (this->IsEmpty() == true)
    {
        return this->AppendRow(ElementData);
    }

    if (this->IsRowVector() == true)
    {
        return this->AppendCol(ElementData);
    }
    else if (this->IsColVector() == true)
    {
        return this->AppendRow(ElementData);
    }
    else
    {
        MDK_Error("Self is not a vector @ DenseMatrix::Append(...)")
        return false;
    }
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool DenseMatrix<ElementType>::Append(const ElementType_Input* ElementData, int_max Length)
{
    if (this->IsEmpty() == true)
    {
        return this->AppendRow(ElementData, Length);
    }

    if (this->IsRowVector() == true)
    {
        return this->AppendCol(ElementData, Length);
    }
    else if (this->IsColVector() == true)
    {
        return this->AppendRow(ElementData, Length);
    }
    else
    {
        MDK_Error("Self is not a vector @ DenseMatrix::Append(...)")
        return false;
    }
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::Delete(int_max LinearIndex)
{
    if (this->IsRowVector() == true)
    {
        return this->DeleteCol(LinearIndex);
    }
    else if (this->IsColVector() == true)
    {
        return this->DeleteRow(LinearIndex);
    }
    else
    {
        MDK_Error("Self is not a vector @ DenseMatrix::Delete(...)")
        return false;
    }
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::Delete(const std::initializer_list<int_max>& LinearIndexList)
{
    if (this->IsRowVector() == true)
    {
        return this->DeleteCol(LinearIndexList);
    }
    else if (this->IsColVector() == true)
    {
        return this->DeleteRow(LinearIndexList);
    }
    else
    {
        MDK_Error("Self is not a vector @ DenseMatrix::Delete(...)")
        return false;
    }
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::Delete(const std::vector<int_max>& LinearIndexList)
{
    if (this->IsRowVector() == true)
    {
        return this->DeleteCol(LinearIndexList);
    }
    else if (this->IsColVector() == true)
    {
        return this->DeleteRow(LinearIndexList);
    }
    else
    {
        MDK_Error("Self is not a vector @ DenseMatrix::Delete(...)")
        return false;
    }
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::Delete(const DenseVector<int_max>& LinearIndexList)
{
    if (this->IsRowVector() == true)
    {
        return this->DeleteCol(LinearIndexList);
    }
    else if (this->IsColVector() == true)
    {
        return this->DeleteRow(LinearIndexList);
    }
    else
    {
        MDK_Error("Self is not a vector @ DenseMatrix::Delete(...)")
        return false;
    }
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::Delete(const DenseMatrix<int_max>& LinearIndexList)
{
    if (this->IsRowVector() == true)
    {
        return this->DeleteCol(LinearIndexList);
    }
    else if (this->IsColVector() == true)
    {
        return this->DeleteRow(LinearIndexList);
    }
    else
    {
        MDK_Error("Self is not a vector @ DenseMatrix::Delete(...)")
        return false;
    }
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::Delete(const int_max* LinearIndexList, int_max ListLength)
{
    if (LinearIndexList == nullptr || Length <= 0)
    {
        //MDK_Warning("Empty input @ DenseMatrix::Delete(...)")
        return true;
    }

    if (this->IsRowVector() == true)
    {
        return this->DeleteCol(LinearIndexList, int_max ListLength);
    }
    else if (this->IsColVector() == true)
    {
        return this->DeleteRow(LinearIndexList, int_max ListLength);
    }
    else
    {
        MDK_Error("Self is not a vector @ DenseMatrix::Delete(...)")
        return false;
    }
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::Insert(int_max LinearIndex, const ElementType& Element)
{
    if (this->IsRowVector() == true)
    {
        return this->InsertCol(LinearIndex, { Element });
    }
    else if (this->IsColVector() == true)
    {
        return this->InsertRow(LinearIndex, { Element });
    }
    else
    {
        MDK_Error("Self is not a vector @ DenseMatrix::Insert(...)")
        return false;
    }
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool DenseMatrix<ElementType>::Insert(int_max LinearIndex, const std::initializer_list<ElementType_Input>& ElementData)
{
    if (this->IsRowVector() == true)
    {
        return this->InsertCol(LinearIndex, ElementData);
    }
    else if (this->IsColVector() == true)
    {
        return this->InsertRow(LinearIndex, ElementData);
    }
    else
    {
        MDK_Error("Self is not a vector @ DenseMatrix::Insert(...)")
        return false;
    }
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool DenseMatrix<ElementType>::Insert(int_max LinearIndex, const std::vector<ElementType_Input>& ElementData)
{
    if (this->IsRowVector() == true)
    {
        return this->InsertCol(LinearIndex, ElementData);
    }
    else if (this->IsColVector() == true)
    {
        return this->InsertRow(LinearIndex, ElementData);
    }
    else
    {
        MDK_Error("Self is not a vector @ DenseMatrix::Insert(...)")
        return false;
    }
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool DenseMatrix<ElementType>::Insert(int_max LinearIndex, const DenseVector<ElementType_Input>& ElementData)
{
    if (this->IsRowVector() == true)
    {
        return this->InsertCol(LinearIndex, ElementData);
    }
    else if (this->IsColVector() == true)
    {
        return this->InsertRow(LinearIndex, ElementData);
    }
    else
    {
        MDK_Error("Self is not a vector @ DenseMatrix::Insert(...)")
        return false;
    }
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool DenseMatrix<ElementType>::Insert(int_max LinearIndex, const DenseMatrix<ElementType_Input>& ElementData)
{
    if (this->IsRowVector() == true)
    {
        return this->InsertCol(LinearIndex, ElementData);
    }
    else if (this->IsColVector() == true)
    {
        return this->InsertRow(LinearIndex, ElementData);
    }
    else
    {
        MDK_Error("Self is not a vector @ DenseMatrix::Insert(...)")
        return false;
    }
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool DenseMatrix<ElementType>::Insert(int_max LinearIndex, const ElementType_Input* ElementData, int_max Length)
{
    if (this->IsRowVector() == true)
    {
        return this->InsertCol(LinearIndex, ElementData, Length);
    }
    else if (this->IsColVector() == true)
    {
        return this->InsertRow(LinearIndex, ElementData, Length);
    }
    else
    {
        MDK_Error("Self is not a vector @ DenseMatrix::Insert(...)")
        return false;
    }
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::GetDiagonal(std::vector<ElementType>& DiagonalData) const
{
    auto SelfSize = this->GetSize();

    if (SelfSize.RowNumber <= 0 || SelfSize.RowNumber != SelfSize.ColNumber)
    {
        MDK_Error(" Self is empty or not square @ DenseMatrix::GetDiagonal(std::vector)")
        return  false;
    }

    DiagonalData.resize(SelfSize.RowNumber);

    auto outputRawPointer = DiagonalData.data();

    return this->GetDiagonal(outputRawPointer);
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::GetDiagonal(DenseMatrix<ElementType>& DiagonalData) const
{
    auto SelfSize = this->GetSize();

    if (SelfSize.RowNumber <= 0 || SelfSize.RowNumber != SelfSize.ColNumber)
    {
        MDK_Error(" Self is empty or not square @ DenseMatrix::GetDiagonal(DenseMatrix&)")
        return  false;
    }

    DiagonalData.FastResize(1, SelfSize.RowNumber);

    auto outputRawPointer = DiagonalData.GetElementPointer();

    return this->GetDiagonal(outputRawPointer);
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::GetDiagonal(ElementType* DiagonalData) const
{
    if (DiagonalData == nullptr)
    {
        MDK_Error(" Input is nullptr @ DenseMatrix::GetDiagonal(ElementType* DiagonalData)")
        return  false;
    }

    auto SelfSize = this->GetSize();

    if (SelfSize.RowNumber <= 0 || SelfSize.RowNumber != SelfSize.ColNumber)
    {
        MDK_Error(" Self is empty or not square @ DenseMatrix::GetDiagonal(ElementType* DiagonalData)")
        return  false;
    }

    auto RawPointer this->GetElementPointer();

    int_max Index = 0;

    for (int_max i = 0; i < SelfSize.RowNumber; ++i)
    {
        DiagonalData[j] = RawPointer[Index + i];

        Index += SelfSize.RowNumber;
    }

    return true;
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool DenseMatrix<ElementType>::SetDiagonal(const std::initializer_list<ElementType_Input>& DiagonalData)
{
    auto SelfSize = this->GetSize();

    if (SelfSize.RowNumber <= 0 || SelfSize.RowNumber != SelfSize.ColNumber)
    {
        MDK_Error("Self is empty or not square @ DenseMatrix::SetDiagonal(std::initializer_list)")
        return false;
    }

    if (DiagonalData.size() != SelfSize.RowNumber)
    {
        MDK_Error("Invalid Input @ DenseMatrix::SetDiagonal(std::initializer_list)")
        return false;
    }

    return this->SetDiagonal(DiagonalData.begin());
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool DenseMatrix<ElementType>::SetDiagonal(const std::vector<ElementType_Input>& DiagonalData)
{
    auto SelfSize = this->GetSize();

    if (SelfSize.RowNumber <= 0 || SelfSize.RowNumber != SelfSize.ColNumber)
	{
		MDK_Error(" Self is empty or not square @ DenseMatrix::SetDiagonal(std::vector)")
		return false;
	}

    if (DiagonalData.size() != SelfSize.RowNumber)
    {
        MDK_Error(" Input is invalid @ DenseMatrix::SetDiagonal(std::vector)")
        return false;
    }

	return this->SetDiagonal(DiagonalData.data());
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool DenseMatrix<ElementType>::SetDiagonal(const DenseVector<ElementType_Input>& DiagonalData)
{
    auto InputLength = DiagonalData.GetLength();

    if (InputLength <= 0)
    {
        MDK_Error("Empty input @ DenseMatrix::SetDiagonal(DenseVector)")
    }

    auto SelfSize = this->GetSize();

    if (InputSize == 1)
    {
        MDK_Warning("Input matrix is 1x1 and treated as a scalar @ DenseMatrix::SetDiagonal(DenseVector)")
        return this->FillDiagonal(ElementType(DiagonalData(0)));
    }
    else
    {
        return this->SetDiagonal(DiagonalData.GetElementPointer());
    }
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool DenseMatrix<ElementType>::SetDiagonal(const DenseMatrix<ElementType_Input>& DiagonalData)
{
    auto InputSize = DiagonalData.GetSize();

    if (InputSize.ColNumber != 1 || InputSize.RowNumber != 1)
    {
        MDK_Error("Invalid Input @ DenseMatrix::SetDiagonal(DenseMatrix)")
        return false;
    }

    auto SelfSize = this->GetSize();

    if (InputSize.RowNumber > 1 && InputSize.RowNumber != SelfSize.ColNumber)
    {
        MDK_Error("Invalid Input @ DenseMatrix::SetDiagonal(DenseMatrix)")
        return false;
    }

    if (InputSize.ColNumber > 1 && InputSize.ColNumber != SelfSize.ColNumber)
    {
        MDK_Error("Invalid Input @ DenseMatrix::SetDiagonal(Matrix)")
        return false;
    }

    if (InputSize.RowNumber == 1 && InputSize.ColNumber == 1)
    {
        MDK_Warning("Input matrix is 1x1 and treated as a scalar @ DenseMatrix::SetDiagonal(Matrix)")
        return this->FillDiagonal(ElementType(DiagonalData(0)));
    }
    else
    {
        return this->SetDiagonal(DiagonalData.GetElementPointer());
    }
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool DenseMatrix<ElementType>::SetDiagonal(const ElementType_Input* DiagonalData)
{
    if (DiagonalData == nullptr)
    {
        MDK_Error(" Input is nullptr @ DenseMatrix::SetDiagonal(const ElementType_Input* DiagonalData)")
        return false;
    }

    auto SelfSize = this->GetSize();

    if (SelfSize.RowNumber <= 0 || SelfSize.RowNumber != SelfSize.ColNumber)
    {
        MDK_Error(" Self is empty or not square @ DenseMatrix::SetDiagonal(const ElementType_Input* DiagonalData)")
        return false;
    }

    auto RawPointer = this->GetElementPointer();

    int_max Index = 0;

    for (int_max i = 0; i < SelfSize.RowNumber; ++i)
    {
        RawPointer[Index + i] = ElementType(DiagonalData[i]);

        Index += SelfSize.RowNumber;
    }

    return true;
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::FillDiagonal(const ElementType& Element)
{
    auto SelfSize = this->GetSize();

    if (SelfSize.RowNumber <= 0 || SelfSize.RowNumber != SelfSize.ColNumber)
    {
        MDK_Error(" Self is empty or not square @ DenseMatrix::FillDiagonal")
        return false;
    }

    auto RawPointer = this->GetElementPointer();

    int_max Index = 0;

    for (int_max i = 0; i < SelfSize.RowNumber; ++i)
    {
        RawPointer[Index + i] = Element;

        Index += SelfSize.RowNumber;
    }

    return true;
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

    if (InputSize.RowNumber <= 0)
    {
        MDK_Error("ShadowMatrix is empty @ DenseMatrix::operator+=(ShadowMatrix)")
        return;
    }

    if (InputSize.RowNumber == 1 && InputSize.ColNumber == 1)
    {
        (*this) += ShadowMatrix[0];

        return;
    }

    auto SelfSize = this->GetSize();

    auto Self_ElementNumber = SelfSize.RowNumber * SelfSize.ColNumber;

    if (InputSize.RowNumber != SelfSize.RowNumber || InputSize.ColNumber != SelfSize.ColNumber)
    {
        MDK_Error("Size does not match @ DenseMatrix::operator+=(ShadowMatrix)")
        return;
    }

    auto RawPointer this->GetElementPointer();

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

    if (InputSize.RowNumber <= 0)
    {
        MDK_Error("ShadowMatrix is empty @ DenseMatrix::operator-=(ShadowMatrix)")
        return;
    }

    if (InputSize.RowNumber == 1 && InputSize.ColNumber == 1)
    {
        (*this) -= ShadowMatrix[0];

        return;
    }

    auto SelfSize = this->GetSize();

    auto Self_ElementNumber = SelfSize.RowNumber * SelfSize.ColNumber;

    if (InputSize.RowNumber != SelfSize.RowNumber || InputSize.ColNumber != SelfSize.ColNumber)
    {
        MDK_Error("Size does not match @ DenseMatrix::operator-=(ShadowMatrix)")
        return;
    }

    auto RawPointer this->GetElementPointer();

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

    if (InputSize.RowNumber <= 0)
    {
        MDK_Error("ShadowMatrix is empty @ DenseMatrix::operator/=(ShadowMatrix)")
        return;
    }

    if (InputSize.RowNumber == 1 && InputSize.ColNumber == 1)
    {
        (*this) /= ShadowMatrix[0];

        return;
    }

    auto SelfSize = this->GetSize();

    auto Self_ElementNumber = SelfSize.RowNumber * SelfSize.ColNumber;

    if (InputSize.RowNumber != SelfSize.RowNumber || InputSize.ColNumber != SelfSize.ColNumber)
    {
        MDK_Error("Size does not match @ DenseMatrix::operator/=(ShadowMatrix)")
        return;
    }

    auto RawPointer this->GetElementPointer();

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
    if (m_ElementPointer == InputMatrix.GetElementPointer())
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


//-------------------- general element operation : output a new matrix ------------------------------------------//

template<typename ElementType>
inline
DenseMatrix<ElementType> DenseMatrix<ElementType>::ElementOperation(const char* OperationName) const
{
    return MatrixElementNamedOperation(OperationName, *this);
}


template<typename ElementType>
inline 
DenseMatrix<ElementType> DenseMatrix<ElementType>::ElementOperation(const std::string& OperationName) const
{
    return MatrixElementNamedOperation(OperationName, *this);
}


template<typename ElementType>
template<typename OperationType>
inline 
DenseMatrix<ElementType> DenseMatrix<ElementType>::ElementOperation(OperationType Operation) const
{
    return MatrixElementOperation(Operation, *this);
}


template<typename ElementType>
inline
DenseMatrix<ElementType> DenseMatrix<ElementType>::ElementOperation(const char OperationName, const DenseMatrix<ElementType>& InputMatrix) const
{
    return MatrixElementNamedOperation(OperationName, *this, InputMatrix);
}


template<typename ElementType>
inline
DenseMatrix<ElementType> DenseMatrix<ElementType>::ElementOperation(const char* OperationName, const DenseMatrix<ElementType>& InputMatrix) const
{
    return MatrixElementNamedOperation(OperationName, *this, InputMatrix);
}


template<typename ElementType>
inline
DenseMatrix<ElementType> DenseMatrix<ElementType>::ElementOperation(const std::string& OperationName, const DenseMatrix<ElementType>& InputMatrix) const
{
    return MatrixElementNamedOperation(OperationName, *this, InputMatrix);
}


template<typename ElementType>
template<typename OperationType>
inline 
DenseMatrix<ElementType> DenseMatrix<ElementType>::ElementOperation(OperationType Operation, const DenseMatrix<ElementType>& InputMatrix) const
{
    return MatrixElementOperation(Operation, *this, InputMatrix);
}


template<typename ElementType>
inline
DenseMatrix<ElementType> DenseMatrix<ElementType>::ElementOperation(const char OperationName, const ElementType& Element) const
{
    return MatrixElementNamedOperation(OperationName, *this, Element);
}


template<typename ElementType>
inline
DenseMatrix<ElementType> DenseMatrix<ElementType>::ElementOperation(const char* OperationName, const ElementType& Element) const
{
    return MatrixElementNamedOperation(OperationName, *this, Element);
}


template<typename ElementType>
inline
DenseMatrix<ElementType> DenseMatrix<ElementType>::ElementOperation(const std::string& OperationName, const ElementType& Element) const
{
    return MatrixElementNamedOperation(OperationName, *this, Element);
}


template<typename ElementType>
template<typename OperationType>
inline
DenseMatrix<ElementType> DenseMatrix<ElementType>::ElementOperation(OperationType Operation, const ElementType& Element) const
{
    return MatrixElementOperation(Operation, *this, Element);
}


//-------------------- element operation in place : Object.ElementOperationInPlace update the current data of the object ---------------//

template<typename ElementType>
inline
bool DenseMatrix<ElementType>::ElementOperationInPlace(const char* OperationName)
{
    return MatrixElementNamedOperation(*this, OperationName, *this);
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::ElementOperationInPlace(const std::string& OperationName)
{
    return MatrixElementNamedOperation(*this, OperationName, *this);
}


template<typename ElementType>
template<typename OperationType>
inline
bool DenseMatrix<ElementType>::ElementOperationInPlace(OperationType Operation)
{
    return MatrixElementOperation(*this, Operation, *this);
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::ElementOperationInPlace(const char OperationName, const DenseMatrix<ElementType>& InputMatrix)
{
    return MatrixElementNamedOperation(*this, OperationName, *this, InputMatrix);
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::ElementOperationInPlace(const char* OperationName, const DenseMatrix<ElementType>& InputMatrix)
{
    return MatrixElementNamedOperation(*this, OperationName, *this, InputMatrix);
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::ElementOperationInPlace(const std::string& OperationName, const DenseMatrix<ElementType>& InputMatrix)
{
    return MatrixElementNamedOperation(*this, OperationName, *this, InputMatrix);
}


template<typename ElementType>
template<typename OperationType>
inline
bool DenseMatrix<ElementType>::ElementOperationInPlace(OperationType Operation, const DenseMatrix<ElementType>& InputMatrix)
{
    return MatrixElementOperation(*this, Operation, *this, InputMatrix);
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::ElementOperationInPlace(const char OperationName, const ElementType& Element)
{
    return MatrixElementNamedOperation(*this, OperationName, *this, Element);
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::ElementOperationInPlace(const char* OperationName, const ElementType& Element)
{
    return MatrixElementNamedOperation(*this, OperationName, *this, Element);
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::ElementOperationInPlace(const std::string& OperationName, const ElementType& Element)
{
    return MatrixElementNamedOperation(*this, OperationName, *this, Element);
}


template<typename ElementType>
template<typename OperationType>
inline
bool DenseMatrix<ElementType>::ElementOperationInPlace(OperationType Operation, const ElementType& Element)
{
    return MatrixElementOperation(*this, Operation, *this, Element);
}


//-------------------- general Col operation : output a new col-matrix ------------------------------------------//

template<typename ElementType>
inline
DenseMatrix<ElementType>
DenseMatrix<ElementType>::ColOperation(int_max ColIndex, const char* OperationName, bool EnableBoundCheck = true) const
{
    return MatrixColNamedOperation(OperationName, *this, ColIndex, EnableBoundCheck);
}


template<typename ElementType>
inline
DenseMatrix<ElementType>
DenseMatrix<ElementType>::ColOperation(int_max ColIndex, const std::string& OperationName, bool EnableBoundCheck = true) const
{    
    return MatrixColNamedOperation(OperationName, *this, ColIndex, EnableBoundCheck);
}


template<typename ElementType>
template<typename OperationType>
inline
DenseMatrix<ElementType>
DenseMatrix<ElementType>::ColOperation(int_max ColIndex, OperationType Operation, bool EnableBoundCheck = true) const
{
    return MatrixColOperation(OperationName, *this, ColIndex, EnableBoundCheck);
}


template<typename ElementType>
inline
DenseMatrix<ElementType>
DenseMatrix<ElementType>::ColOperation(int_max ColIndex, const char OperationName, const DenseMatrix<ElementType>& InputMatrix, bool EnableBoundCheck = true) const
{
    return MatrixColNamedOperation(OperationName, *this, ColIndex, InputMatrix, EnableBoundCheck);
}


template<typename ElementType>
inline
DenseMatrix<ElementType>
DenseMatrix<ElementType>::ColOperation(int_max ColIndex, const char* OperationName, const DenseMatrix<ElementType>& InputMatrix, bool EnableBoundCheck = true) const
{
    return MatrixColNamedOperation(OperationName, *this, ColIndex, InputMatrix, EnableBoundCheck);
}


template<typename ElementType>
inline 
DenseMatrix<ElementType>
DenseMatrix<ElementType>::ColOperation(int_max ColIndex, const std::string& OperationName, const DenseMatrix<ElementType>& InputMatrix, bool EnableBoundCheck = true) const
{
    return MatrixColNamedOperation(OperationName, *this, ColIndex, InputMatrix, EnableBoundCheck);
}


template<typename ElementType>
template<typename OperationType>
inline
DenseMatrix<ElementType> 
DenseMatrix<ElementType>::ColOperation(int_max ColIndex, OperationType Operation, const DenseMatrix<ElementType>& InputMatrix, bool EnableBoundCheck = true) const
{
    return MatrixColOperation(Operation, *this, ColIndex, InputMatrix, EnableBoundCheck);
}


template<typename ElementType>
inline 
DenseMatrix<ElementType> 
DenseMatrix<ElementType>::ColOperation(int_max ColIndex, const char OperationName, const ElementType& Element, bool EnableBoundCheck = true) const
{
    return MatrixColNamedOperation(OperationName, *this, ColIndex, Element, EnableBoundCheck);
}


template<typename ElementType>
inline
DenseMatrix<ElementType>
DenseMatrix<ElementType>::ColOperation(int_max ColIndex, const char* OperationName, const ElementType& Element, bool EnableBoundCheck = true) const
{
    return MatrixColNamedOperation(OperationName, *this, ColIndex, Element, EnableBoundCheck);
}


template<typename ElementType>
inline
DenseMatrix<ElementType> 
DenseMatrix<ElementType>::ColOperation(int_max ColIndex, const std::string& OperationName, const ElementType& Element, bool EnableBoundCheck = true) const
{
    return MatrixColNamedOperation(OperationName, *this, ColIndex, Element, EnableBoundCheck);
}


template<typename ElementType>
template<typename OperationType>
inline 
DenseMatrix<ElementType>
DenseMatrix<ElementType>::ColOperation(int_max ColIndex, OperationType Operation, const ElementType& Element, bool EnableBoundCheck = true) const
{
    return MatrixColOperation(Operation, *this, ColIndex, Element, EnableBoundCheck);
}

//-------------------- general col operation in place : Object.ColOperationInPlace modify the object itself ---------------//

template<typename ElementType>
inline
bool DenseMatrix<ElementType>::ColOperationInPlace(int_max ColIndex, const char* OperationName, bool EnableBoundCheck = true)
{
    return MatrixColNamedOperation(*this, ColIndex, OperationName, *this, ColIndex, EnableBoundCheck);
}


template<typename ElementType>
inline 
bool DenseMatrix<ElementType>::ColOperationInPlace(int_max ColIndex, const std::string& OperationName, bool EnableBoundCheck = true)
{
    return MatrixColNamedOperation(*this, ColIndex, OperationName, *this, ColIndex, EnableBoundCheck);
}


template<typename ElementType>
template<typename OperationType>
inline
bool DenseMatrix<ElementType>::ColOperationInPlace(int_max ColIndex, OperationType Operation, bool EnableBoundCheck = true)
{
    return MatrixColOperation(*this, ColIndex, Operation, *this, ColIndex, EnableBoundCheck);
}


template<typename ElementType>
inline 
bool DenseMatrix<ElementType>::ColOperationInPlace(int_max ColIndex, const char OperationName, const DenseMatrix<ElementType>& InputMatrix, bool EnableBoundCheck = true)
{
    return MatrixColNamedOperation(*this, ColIndex, OperationName, *this, ColIndex, InputMatrix, EnableBoundCheck);
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::ColOperationInPlace(int_max ColIndex, const char* OperationName, const DenseMatrix<ElementType>& InputMatrix, bool EnableBoundCheck = true)
{
    return MatrixColNamedOperation(*this, ColIndex, OperationName, *this, ColIndex, InputMatrix, EnableBoundCheck);
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::ColOperationInPlace(int_max ColIndex, const std::string& OperationName, const DenseMatrix<ElementType>& InputMatrix, bool EnableBoundCheck = true)
{
    return MatrixColNamedOperation(*this, ColIndex, OperationName, *this, ColIndex, InputMatrix, EnableBoundCheck);
}


template<typename ElementType>
template<typename OperationType>
inline
bool DenseMatrix<ElementType>::ColOperationInPlace(int_max ColIndex, OperationType Operation, const DenseMatrix<ElementType>& InputMatrix, bool EnableBoundCheck = true)
{
    return MatrixColOperation(*this, ColIndex, Operation, *this, ColIndex, InputMatrix, EnableBoundCheck);
}


template<typename ElementType>
inline 
bool DenseMatrix<ElementType>::ColOperationInPlace(int_max ColIndex, const char OperationName, const ElementType& Element, bool EnableBoundCheck = true)
{
    return MatrixColNamedOperation(*this, ColIndex, OperationName, *this, ColIndex, Element, EnableBoundCheck);
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::ColOperationInPlace(int_max ColIndex, const char* OperationName, const ElementType& Element, bool EnableBoundCheck = true)
{
    return MatrixColNamedOperation(*this, ColIndex, OperationName, *this, ColIndex, Element, EnableBoundCheck);
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::ColOperationInPlace(int_max ColIndex, const std::string& OperationName, const ElementType& Element, bool EnableBoundCheck = true)
{
    return MatrixColNamedOperation(*this, ColIndex, OperationName, *this, ColIndex, Element, EnableBoundCheck);
}


template<typename ElementType>
template<typename OperationType>
inline
bool DenseMatrix<ElementType>::ColOperationInPlace(int_max ColIndex, OperationType Operation, const ElementType& Element, bool EnableBoundCheck = true)
{
    return MatrixColOperation(*this, ColIndex, Operation, *this, ColIndex, Element, EnableBoundCheck);
}

//-------------------- general Row operation : output a new row-matrix ------------------------------------------//

template<typename ElementType>
inline
DenseMatrix<ElementType>
DenseMatrix<ElementType>::RowOperation(int_max RowIndex, const char* OperationName, bool EnableBoundCheck = true) const
{
    return MatrixRowNamedOperation(OperationName, *this, RowIndex, EnableBoundCheck);
}


template<typename ElementType>
inline 
DenseMatrix<ElementType>
DenseMatrix<ElementType>::RowOperation(int_max RowIndex, const std::string& OperationName, bool EnableBoundCheck = true) const
{
    return MatrixRowNamedOperation(OperationName, *this, RowIndex, EnableBoundCheck);
}


template<typename ElementType>
template<typename OperationType>
inline 
DenseMatrix<ElementType> 
DenseMatrix<ElementType>::RowOperation(int_max RowIndex, OperationType Operation, bool EnableBoundCheck = true) const
{
    return MatrixRowOperation(Operation, *this, RowIndex, EnableBoundCheck);
}


template<typename ElementType>
inline 
DenseMatrix<ElementType>
DenseMatrix<ElementType>::RowOperation(int_max RowIndex, const char OperationName, const DenseMatrix<ElementType>& InputMatrix, bool EnableBoundCheck = true) const
{
    return MatrixRowNamedOperation(OperationName, *this, RowIndex, InputMatrix, EnableBoundCheck);
}


template<typename ElementType>
inline
DenseMatrix<ElementType>
DenseMatrix<ElementType>::RowOperation(int_max RowIndex, const char* OperationName, const DenseMatrix<ElementType>& InputMatrix, bool EnableBoundCheck = true) const
{
    return MatrixRowNamedOperation(OperationName, *this, RowIndex, InputMatrix, EnableBoundCheck);
}


template<typename ElementType>
inline 
DenseMatrix<ElementType>
DenseMatrix<ElementType>::RowOperation(int_max RowIndex, const std::string& OperationName, const DenseMatrix<ElementType>& InputMatrix, bool EnableBoundCheck = true) const
{
    return MatrixRowNamedOperation(OperationName, *this, RowIndex, InputMatrix, EnableBoundCheck);
}


template<typename ElementType>
template<typename OperationType>
inline 
DenseMatrix<ElementType> 
DenseMatrix<ElementType>::RowOperation(int_max RowIndex, OperationType Operation, const DenseMatrix<ElementType>& InputMatrix, bool EnableBoundCheck = true) const
{
    return MatrixRowOperation(Operation, *this, RowIndex, InputMatrix, EnableBoundCheck);
}


template<typename ElementType>
inline 
DenseMatrix<ElementType> 
DenseMatrix<ElementType>::RowOperation(int_max RowIndex, const char OperationName, const ElementType& Element, bool EnableBoundCheck = true) const
{
    return MatrixRowNamedOperation(OperationName, *this, RowIndex, Element, EnableBoundCheck);
}


template<typename ElementType>
inline
DenseMatrix<ElementType>
DenseMatrix<ElementType>::RowOperation(int_max RowIndex, const char* OperationName, const ElementType& Element, bool EnableBoundCheck = true) const
{
    return MatrixRowNamedOperation(OperationName, *this, RowIndex, Element, EnableBoundCheck);
}


template<typename ElementType>
inline 
DenseMatrix<ElementType> 
DenseMatrix<ElementType>::RowOperation(int_max RowIndex, const std::string& OperationName, const ElementType& Element, bool EnableBoundCheck = true) const
{
    return MatrixRowNamedOperation(OperationName, *this, RowIndex, Element, EnableBoundCheck);
}


template<typename ElementType>
template<typename OperationType>
inline
DenseMatrix<ElementType> 
DenseMatrix<ElementType>::RowOperation(int_max RowIndex, OperationType Operation, const ElementType& Element, bool EnableBoundCheck = true) const
{
    return MatrixRowOperation(Operation, *this, RowIndex, Element, EnableBoundCheck);
}

//-------------------- general row operation in place : Object.RowOperationInPlace modify the object itself ---------------//

template<typename ElementType>
inline
bool DenseMatrix<ElementType>::RowOperationInPlace(int_max RowIndex, const char* OperationName, bool EnableBoundCheck = true)
{
    return MatrixRowNamedOperation(*this, RowIndex, OperationName, *this, RowIndex, EnableBoundCheck);
}


template<typename ElementType>
inline 
bool DenseMatrix<ElementType>::RowOperationInPlace(int_max RowIndex, const std::string& OperationName, bool EnableBoundCheck = true)
{
    return MatrixRowNamedOperation(*this, RowIndex, OperationName, *this, RowIndex, EnableBoundCheck);
}


template<typename ElementType>
template<typename OperationType>
inline 
bool DenseMatrix<ElementType>::RowOperationInPlace(int_max RowIndex, OperationType Operation, bool EnableBoundCheck = true)
{
    return MatrixRowOperation(*this, RowIndex, Operation, *this, RowIndex, EnableBoundCheck);
}


template<typename ElementType>
inline 
bool DenseMatrix<ElementType>::RowOperationInPlace(int_max RowIndex, const char OperationName, const DenseMatrix<ElementType>& InputMatrix, bool EnableBoundCheck = true)
{
    return MatrixRowNamedOperation(*this, RowIndex, OperationName, *this, RowIndex, InputMatrix, EnableBoundCheck);
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::RowOperationInPlace(int_max RowIndex, const char* OperationName, const DenseMatrix<ElementType>& InputMatrix, bool EnableBoundCheck = true)
{
    return MatrixRowNamedOperation(*this, RowIndex, OperationName, *this, RowIndex, InputMatrix, EnableBoundCheck);
}


template<typename ElementType>
inline 
bool DenseMatrix<ElementType>::RowOperationInPlace(int_max RowIndex, const std::string& OperationName, const DenseMatrix<ElementType>& InputMatrix, bool EnableBoundCheck = true)
{
    return MatrixRowNamedOperation(*this, RowIndex, OperationName, *this, RowIndex, InputMatrix, EnableBoundCheck);
}


template<typename ElementType>
template<typename OperationType>
inline 
bool DenseMatrix<ElementType>::RowOperationInPlace(int_max RowIndex, OperationType Operation, const DenseMatrix<ElementType>& InputMatrix, bool EnableBoundCheck = true)
{
    return MatrixRowOperation(*this, RowIndex, Operation, *this, RowIndex, InputMatrix, EnableBoundCheck);
}


template<typename ElementType>
inline 
bool DenseMatrix<ElementType>::RowOperationInPlace(int_max RowIndex, const char OperationName, const ElementType& Element, bool EnableBoundCheck = true)
{
    return MatrixRowNamedOperation(*this, RowIndex, OperationName, *this, RowIndex, Element, EnableBoundCheck);
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::RowOperationInPlace(int_max RowIndex, const char* OperationName, const ElementType& Element, bool EnableBoundCheck = true)
{
    return MatrixRowNamedOperation(*this, RowIndex, OperationName, *this, RowIndex, Element, EnableBoundCheck);
}


template<typename ElementType>
inline 
bool
DenseMatrix<ElementType>::RowOperationInPlace(int_max RowIndex, const std::string& OperationName, const ElementType& Element, bool EnableBoundCheck = true)
{
    return MatrixRowNamedOperation(*this, RowIndex, OperationName, *this, RowIndex, Element, EnableBoundCheck);
}


template<typename ElementType>
template<typename OperationType>
inline
bool
DenseMatrix<ElementType>::RowOperationInPlace(int_max RowIndex, OperationType Operation, const ElementType& Element, bool EnableBoundCheck = true)
{
    return MatrixRowOperation(*this, RowIndex, Operation, *this, RowIndex, Element, EnableBoundCheck);
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
    return FindElementInMatrix(*this, MaxOutputColNumber, MatchFunction);
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
int_max DenseMatrix<ElementType>::Match(MatchFunctionType MatchFunction) const
{
    return MatchElementInMatrix(*this, MatchFunction);
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
DenseMatrix<ElementType>::FindCol(int_max MaxOutputColNumber, MatchFunctionType MatchFunction) const
{
    return FindColInMatrix(*this, MaxOutputColNumber, MatchFunction);
}


template<typename ElementType>
template<typename MatchFunctionType>
inline
DenseMatrix<int_max>
DenseMatrix<ElementType>::FindCol(int_max MaxOutputColNumber, int_max ColIndex_start, int_max ColIndex_end, MatchFunctionType MatchFunction) const
{
    return FindColInMatrix(*this, MaxOutputColNumber, ColIndex_start, ColIndex_end, MatchFunction);
}

//-------------------------------------------------------------------------------------------//

template<typename ElementType>
template<typename CompareFunctionType>
inline
DenseMatrix<int_max> DenseMatrix<ElementType>::Sort(CompareFunctionType CompareFunction) const
{
    return this->Sort(0, this->GetElementNumber() - 1, CompareFunction);
}


template<typename ElementType>
template<typename CompareFunctionType>
inline 
DenseMatrix<int_max> DenseMatrix<ElementType>::Sort(int_max LinearIndex_start, int_max LinearIndex_end, CompareFunctionType CompareFunction) const
{
    DenseMatrix<int_max> LinearIndexList;

    auto ElementNumber = this->GetElementNumber();

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

    std::sort(LinearIndexList.begin(), LinearIndexList.end(), [&](int_max a, int_max b)
    {
        return CompareFunction((*this)[a], (*this)[b]);
    });

    return LinearIndexList;
}


template<typename ElementType>
template<typename CompareFunctionType>
inline 
void DenseMatrix<ElementType>::SortInPlace(CompareFunctionType CompareFunction)
{
    auto LinearIndexList = this->Sort(CompareFunction);

    if (LinearIndexList.IsEmpty() == true)
    {
        return LinearIndexList;
    }

    (*this) = (*this)(LinearIndexList);
}


template<typename ElementType>
template<typename CompareFunctionType>
inline 
void DenseMatrix<ElementType>::SortInPlace(int_max LinearIndex_start, int_max LinearIndex_end, CompareFunctionType CompareFunction)
{
    auto LinearIndexList = this->Sort(LinearIndex_start, LinearIndex_end, CompareFunction);

    if (LinearIndexList.IsEmpty() == true)
    {
        return LinearIndexList;
    }

    (*this)(span(LinearIndex_start, LinearIndex_end)) = (*this)(LinearIndexList);
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
    this->SortColInPlace(0, this->GetElementNumber()-1, CompareFunction);
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
inline ElementType DenseMatrix<ElementType>::Mean() const
{
    return MatrixMean(*this);
}


template<typename ElementType>
inline DenseMatrix<ElementType> DenseMatrix<ElementType>::MeanToRow() const
{
    return MatrixMeanToRow(*this);
}


template<typename ElementType>
inline DenseMatrix<ElementType> DenseMatrix<ElementType>::MeanToCol() const
{
    return MatrixMeanToCol(*this);
}


template<typename ElementType>
inline ElementType DenseMatrix<ElementType>::Sum() const
{
    return MatrixSum(*this);
}


template<typename ElementType>
inline DenseMatrix<ElementType> DenseMatrix<ElementType>::SumToRow() const
{
    return MatrixSumToRow(*this);
}


template<typename ElementType>
inline DenseMatrix<ElementType> DenseMatrix<ElementType>::SumToCol() const
{
    return MatrixSumToCol(*this);
}


template<typename ElementType>
inline int_max DenseMatrix<ElementType>::IndexOfMax() const
{
    return FindLinearIndexOfMaxInMatrix(*this);
}


template<typename ElementType>
inline ElementType DenseMatrix<ElementType>::Max() const
{
    return MatrixMax(*this);
}


template<typename ElementType>
inline DenseMatrix<ElementType> DenseMatrix<ElementType>::MaxToRow() const
{
    return MatrixMaxToRow(*this);
}


template<typename ElementType>
inline DenseMatrix<ElementType> DenseMatrix<ElementType>::MaxToCol() const
{
    return MatrixMaxToCol(*this);
}


template<typename ElementType>
inline int_max DenseMatrix<ElementType>::IndexOfMin() const
{
    return FindLinearIndexOfMinInMatrix(*this);
}


template<typename ElementType>
inline ElementType DenseMatrix<ElementType>::Min() const
{
    return MatrixMin(*this);
}


template<typename ElementType>
inline DenseMatrix<ElementType> DenseMatrix<ElementType>::MinToRow() const
{
    return MatrixMinToRow(*this);
}


template<typename ElementType>
inline DenseMatrix<ElementType> DenseMatrix<ElementType>::MinToCol() const
{
    return MatrixMinToCol(*this);
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
DenseMatrix<ElementType> DenseMatrix<ElementType>::Inv() const
{
    return MatrixInv(*this);
}


template<typename ElementType>
inline
DenseMatrix<ElementType> DenseMatrix<ElementType>::PseudoInv() const
{
	// call Armadillo 


}


template<typename ElementType>
inline
DenseMatrixSVDResult<ElementType> DenseMatrix<ElementType>::SVD() const
{
    return MatrixSVD(*this);
}

}//end namespace mdk

#endif