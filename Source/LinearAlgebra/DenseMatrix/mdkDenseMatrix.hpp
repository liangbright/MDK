#ifndef __mdkDenseMatrix_hpp
#define __mdkDenseMatrix_hpp

//#include "mdkDenseMatrix.h"

namespace mdk
{
 
template<typename ElementType>
inline
DenseMatrix<ElementType>::DenseMatrix()
{
    m_IsSizeFixed = false;

    this->Resize(0, 0);
}


template<typename ElementType>
inline
DenseMatrix<ElementType>::DenseMatrix(int64 RowNumber, int64 ColNumber)
{
    m_IsSizeFixed = false;

    this->Resize(RowNumber, ColNumber);
}


template<typename ElementType>
inline
DenseMatrix<ElementType>::DenseMatrix(const ElementType& Element)
{
    m_IsSizeFixed = false;

    this->Resize(1, 1);

    (*this)(0) = Element;
}


template<typename ElementType>
inline
DenseMatrix<ElementType>::DenseMatrix(const DenseMatrix<ElementType>& InputMatrix, 
                                      ObjectConstructionTypeEnum Method = ObjectConstructionTypeEnum::Copy)
{
    m_IsSizeFixed = false;

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
DenseMatrix<ElementType>::DenseMatrix(DenseMatrix<ElementType>&& InputMatrix)
{
    m_MatrixData = std::move(InputMatrix.m_MatrixData);

    m_ElementPointer = m_MatrixData->DataArray.data();

    m_NaNElement = InputMatrix.m_NaNElement;

    m_IsSizeFixed = InputMatrix.m_IsSizeFixed;
}


template<typename ElementType>
inline
DenseMatrix<ElementType>::DenseMatrix(const DenseShadowMatrix<ElementType>& ShadowMatrix)
{
    m_IsSizeFixed = false;

    this->Resize(0, 0);

    this->Take(ShadowMatrix.CreateDenseMatrix());
}


template<typename ElementType>
inline
DenseMatrix<ElementType>::DenseMatrix(const DenseGlueMatrixForLinearCombination<ElementType>& GlueMatrix)
{
    m_IsSizeFixed = false;

    this->Resize(0, 0);

    this->Take(GlueMatrix.CreateDenseMatrix());
}


template<typename ElementType>
inline
DenseMatrix<ElementType>::DenseMatrix(const DenseGlueMatrixForMultiplication<ElementType>& GlueMatrix)
{
    m_IsSizeFixed = false;

    this->Resize(0, 0);

    this->Take(GlueMatrix.CreateDenseMatrix());
}


template<typename ElementType>
inline
DenseMatrix<ElementType>::DenseMatrix(const ElementType* InputElementPointer, int64 InputRowNumber, int64 InputColNumber)
{
    m_IsSizeFixed = false;

    this->Resize(0, 0);

    if (InputElementPointer == nullptr)
    {
        MDK_Warning << "Empty Input @ mdkDenseMatrix::mdkDenseMatrix(const ElementType*, int64, int64, bool)" << '\n';
        return;
    }

    if (InputRowNumber <= 0 || InputColNumber <= 0)
    {
        if (InputElementPointer != nullptr)
        {
            MDK_Error << "Invalid Input @ mdkDenseMatrix::mdkDenseMatrix(const ElementType*, int64, int64, bool)" << '\n';
            return;
        }
    }

    this->Copy(InputElementPointer, InputRowNumber, InputColNumber);
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
    this->Take(std::forward<DenseMatrix<ElementType>&&>(InputMatrix));
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::operator=(const ElementType& Element)
{
    auto ElementNumber = this->GetElementNumber();

    if (m_IsSizeFixed == true)
    {
        if (ElementNumber <= 0)
        {
            MDK_Error << "Can not change matrix size @ mdkDenseMatrix::operator=(Element)" << '\n';
            return;
        }
    }

    if (ElementNumber <= 0)
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
        MDK_Warning << "The size of the matrix changes to 1x1 @ mdkDenseMatrix::operator=(Element)" << '\n';

        this->Clear();

        this->Resize(1, 1);

        (*this)[0] = Element;
    }

    return;
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::operator=(const std::initializer_list<ElementType>& list)
{
    auto SelfSize = this->GetSize();

    auto ElementNumber = SelfSize.RowNumber * SelfSize.ColNumber;

    //if Self is empty
    if (ElementNumber <= 0)
    {
        MDK_Error << "operator=(list) can not be called if self is empty @ mdkDenseMatrix::operator=(list)" << '\n';
        return;
    }

    if (ElementNumber != list.size())
    {
        MDK_Error << "Size does not match @ mdkDenseMatrix::operator=(list)" << '\n';
        return;
    }

    auto RawPointer = this->GetElementPointer();

    int64 Counter = 0;

    for (int64 i = 0; i < SelfSize.RowNumber; ++i)
    {
        int64 Index = 0;

        for (int64 j = 0; j < SelfSize.ColNumber; ++j)
        {
            RawPointer[Index + i] = list.begin()[Counter];

            Index += SelfSize.RowNumber;

            Counter += 1;
        }
    }

    return;
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::operator=(const std::initializer_list<std::initializer_list<ElementType>>& list)
{
    auto SelfSize = this->GetSize();

    //if Self is empty
    if (SelfSize.RowNumber <= 0)
    {
        MDK_Error << "operator=(list in list) can not be called if self is empty @ mdkDenseMatrix::operator=(list in list)" << '\n';
        return;
    }

    if (SelfSize.RowNumber != list.size())
    {
        MDK_Error << "Row Size does not match @ mdkDenseMatrix::operator=(list in list)" << '\n';
        return;
    }

    //check each row-list 
    for (int64 i = 0; i < SelfSize.RowNumber; ++i)
    {
        auto subList = list.begin()[i];

        if (subList.size() != SelfSize.ColNumber)
        {
            MDK_Error << "Col Size does not match in row: " << i << " @ mdkDenseMatrix::operator=(list in list)" << '\n';
            return;
        }
    }

    auto RawPointer = this->GetElementPointer();

    for (int64 i = 0; i < SelfSize.RowNumber; ++i)
    {
        auto subList = list.begin()[i];

        int64 Index = 0;

        for (int64 j = 0; j < SelfSize.ColNumber; ++j)
        {
            RawPointer[Index + i] = subList.begin()[j];

            Index += SelfSize.RowNumber;
        }
    }

    return;
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
    if (this == &InputMatrix)
    {
        MDK_Warning << "A Matrix tries to Copy itself @ mdkDenseMatrix::Copy(InputMatrix)" << '\n';
        return false;
    }

    if (InputMatrix.IsEmpty() == true)
    {
        MDK_Warning << "InputMatrix is empty, and this matrix is set to be empty @ mdkDenseMatrix::Copy(InputMatrix)" << '\n';

        this->Clear();

        return true;
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
        MDK_Error << "Input is nullptr @ mdkDenseMatrix::Copy(mdkDenseMatrix* InputMatrix)" << '\n';
        return false;
    }

    return this->Copy(*InputMatrix);
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool DenseMatrix<ElementType>::Copy(const ElementType_Input* InputElementPointer, int64 InputRowNumber, int64 InputColNumber)
{
    if (InputElementPointer == nullptr || InputRowNumber <= 0 || InputColNumber <= 0)
    {
        MDK_Error << "Input pointer is nullptr @ mdkDenseMatrix::Copy(ElementType_Input*, RowNumber, ColNumber)" << '\n';
        return false;
    }

    auto tempElementType = FindMatrixElementType(InputElementPointer[0]);

    if (tempElementType == MatrixElementTypeEnum::UNKNOWN)
    {
        MDK_Error << "Input type is unknown @ mdkDenseMatrix::Copy(ElementType_Input*, RowNumber, ColNumber)" << '\n';
        return false;
    }

    // if this matrix is not empty, check if this and Input Share the same data
    if (this->IsEmpty() == false)
    {
        if (std::size_t(InputElementPointer) == std::size_t(this->GetElementPointer()))
        {
            MDK_Warning << "A Matrix tries to Copy itself @ mdkDenseMatrix::Copy(ElementType_Input*, RowNumber, ColNumber)" << '\n';
            return false;
        }
    }

    //------------------------------------------------------------------

    auto SelfSize = this->GetSize();

    auto Self_ElementNumber = SelfSize.RowNumber * SelfSize.ColNumber;

    //------------------------------------------------------------------

    if (m_IsSizeFixed == true)
    {
        if (InputRowNumber != SelfSize.RowNumber || InputColNumber != SelfSize.ColNumber)
        {
            MDK_Error << "Can not change matrix size @ mdkDenseMatrix::Copy(ElementType_Input*, InputRowNumber, InputColNumber)" << '\n';
            return false;
        }
    }
    else
    {
        // check to see if new memory allocation is needed --------------------------------

        bool IsNewMemoryNeeded = false;

        //if self is empty
        if (Self_ElementNumber == 0)
        {
            IsNewMemoryNeeded = true;
        }
        else
        {
            if (InputRowNumber != SelfSize.RowNumber || InputColNumber != SelfSize.ColNumber)
            {
                IsNewMemoryNeeded = true;
            }
        }

        if (IsNewMemoryNeeded == true)
        {
            this->Clear();

            this->Resize(InputRowNumber, InputColNumber);

            SelfSize = this->GetSize();

            Self_ElementNumber = SelfSize.RowNumber * SelfSize.ColNumber;
        }
    }

    //copy data ----------------------------------------------------------

    auto BeginPointer = this->GetElementPointer();

    auto tempPtr = InputElementPointer;

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
        MDK_Error << "Self is empty @ mdkDenseMatrix::Fill" << '\n';
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
    // self assignment test is not necessary, just to output some information

    // Matrix = Matrix
    if (this == &InputMatrix)
    {
        MDK_Warning << "A Matrix tries to Share itself @ mdkDenseMatrix::Share(InputMatrix)" << '\n';
        return true;
    }

    // data = data
    if (this->GetElementPointer() == InputMatrix.GetElementPointer())
    {
        MDK_Warning << "A Matrix tries to Share the same data @ mdkDenseMatrix::Share(InputMatrix)" << '\n';
        return true;
    }

    auto InputSize = InputMatrix.GetSize();

    auto SelfSize = this->GetSize();

    if (m_IsSizeFixed == true)
    {
        if (InputSize.RowNumber != SelfSize.RowNumber || InputSize.ColNumber != SelfSize.ColNumber)
        {
            MDK_Error << "Matrix size can not be changed @ mdkDenseMatrix::Share(InputMatrix)" << '\n';
            return false;
        }
    }

    //--------------------------------------------------------------------------------------------------------

    m_MatrixData = InputMatrix.m_MatrixData; // std::Shared_ptr, self assignment test is not necessary

    m_ElementPointer = m_MatrixData->DataArray.data();

    return true;
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::Share(DenseMatrix<ElementType>* InputMatrix)
{
    if (InputMatrix == nullptr)
    {
        MDK_Error << "Input is nullptr @ mdkDenseMatrix::Share(mdkDenseMatrix* InputMatrix)" << '\n';
        return false;
    }

    return this->Share(*InputMatrix);
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::ForceShare(const DenseMatrix<ElementType>& InputMatrix)
{
    // data = data
    if (this->GetElementPointer() == InputMatrix.GetElementPointer())
    {
        MDK_Warning << "A Matrix tries to ForceShare the same data @ mdkDenseMatrix::Share(InputMatrix)" << '\n';
        return;
    }

    m_MatrixData = InputMatrix.m_MatrixData; // std::Shared_ptr, self assignment test is not necessary

    m_ElementPointer = m_MatrixData->DataArray.data();
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::ForceShare(const DenseMatrix<ElementType>* InputMatrix)
{
    if (InputMatrix == nullptr)
    {
        MDK_Error << "Input is nullptr @ mdkDenseMatrix::ForceShare(mdkDenseMatrix* InputMatrix)" << '\n';
        return false;
    }

    return this->ForceShare(*InputMatrix);
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
    // MatrixA = MatrixA
    if (this == &InputMatrix)
    {
        MDK_Warning << "A Matrix tries to take itself @ mdkDenseMatrix::Take(InputMatrix)" << '\n';
        return false;
    }

    auto InputSize = InputMatrix.GetSize();

    auto SelfSize = this->GetSize();

    if (m_IsSizeFixed == true)
    {
        if (InputSize.RowNumber != SelfSize.RowNumber || InputSize.ColNumber != SelfSize.ColNumber)
        {
            MDK_Error << "Size does not match @ mdkDenseMatrix::Take(InputMatrix)" << '\n';
            return false;
        }
    }

    if (InputMatrix.IsEmpty() == true)
    {
        if (SelfSize.RowNumber > 0)
        {
            MDK_Warning << "InputMatrix is empty, and this matrix is set to be empty @ mdkDenseMatrix::Take(InputMatrix)" << '\n';
            this->Clear();
        }

        return true;
    }

    // now, InputMatrix is not empty

    m_MatrixData->RowNumber = InputMatrix.m_MatrixData->RowNumber;

    m_MatrixData->ColNumber = InputMatrix.m_MatrixData->ColNumber;

    m_MatrixData->DataArray = std::move(InputMatrix.m_MatrixData->DataArray);

    m_ElementPointer = m_MatrixData->DataArray.data();

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
        MDK_Error << "Input is nullptr @ mdkDenseMatrix::Take(mdkDenseMatrix* InputMatrix)" << '\n';
        return false;
    }

    return this->Take(*InputMatrix);
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::Take(const DenseShadowMatrix<ElementType>& ShadowMatrix)
{
    auto InputSize = ShadowMatrix.GetSize();

    auto SelfSize = this->GetSize();

    if (InputSize.RowNumber == SelfSize.RowNumber && InputSize.ColNumber == SelfSize.ColNumber)
    {
        ShadowMatrix.CreateDenseMatrix(*this);
    }
    else
    {
        if (m_IsSizeFixed == true)
        {
            MDK_Error << "Size does not match @ mdkDenseMatrix::Take(ShadowMatrix)" << '\n';
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
    auto InputSize = GlueMatrix.GetSize();

    auto SelfSize = this->GetSize();

    if (InputSize.RowNumber == SelfSize.RowNumber && InputSize.ColNumber == SelfSize.ColNumber)
    {
        GlueMatrix.CreateDenseMatrix(*this);
    }
    else
    {
        if (m_IsSizeFixed == true)
        {
            MDK_Error << "Size does not match @ mdkDenseMatrix::Take(GlueMatrix_ForLinearCombination)" << '\n';
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
    auto InputSize = GlueMatrix.GetSize();

    auto SelfSize = this->GetSize();

    if (InputSize.RowNumber == SelfSize.RowNumber && InputSize.ColNumber == SelfSize.ColNumber)
    {
        GlueMatrix.CreateDenseMatrix(*this);
    }
    else
    {
        if (m_IsSizeFixed == true)
        {
            MDK_Error << "Size does not match @ mdkDenseMatrix::Take(GlueMatrix_ForMultiplication)" << '\n';
            return false;
        }

        this->Take(GlueMatrix.CreateDenseMatrix());
    }

    return true;
}


template<typename ElementType>
inline
void DenseMatrix<ElementType>::Clear()
{
    m_MatrixData->RowNumber = 0;

    m_MatrixData->ColNumber = 0;

    m_MatrixData->DataArray.clear();

    m_ElementPointer = nullptr;
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::Reshape(int64 InputRowNumber, int64 InputColNumber)
{
    if (m_IsSizeFixed == true)
    {
        MDK_Error << "Matrix size can not be changed @ mdkDenseMatrix::Reshape()" << '\n';
        return false;
    }

    if (this->IsEmpty() == true)
    {
        if (InputRowNumber > 0 || InputColNumber > 0)
        {
            MDK_Error << "Self is empty and Size does not match @ mdkDenseMatrix::Reshape()" << '\n';
            return false;
        }
    }

    if (InputRowNumber*InputColNumber != this->GetElementNumber())
    {
        MDK_Error << "Size does not match @ mdkDenseMatrix::Reshape" << '\n';
        return false;
    }

    m_MatrixData->RowNumber = InputRowNumber;

    m_MatrixData->ColNumber = InputColNumber;

    return true;
}


template<typename ElementType>
inline 
bool DenseMatrix<ElementType>::Resize(int64 InputRowNumber, int64 InputColNumber)
{
    if (m_IsSizeFixed == true)
    {
        MDK_Error << "Matrix Size can not be changed @ mdkDenseMatrix::Resize(int64 RowNumber, int64 ColNumber)" << '\n';
        return false;
    }

    if (InputRowNumber < 0 || InputColNumber < 0)
    {
        MDK_Error << "Invalid Input: negtive @ mdkDenseMatrix::Resize(int64 RowNumber, int64 ColNumber)" << '\n';
        return false;
    }

    //--------initialize the matrix data ----------------------------------------
    if (!m_MatrixData)
    {
        m_MatrixData = std::make_shared<DenseMatrixData<ElementType>>();

        m_ElementPointer = nullptr;

        m_NaNElement = m_NaNElement - m_NaNElement;

        m_NaNElement = GetMatrixNaNElement(m_NaNElement);
    }
    //-------------------------------------------------------------------------

    if (InputRowNumber == 0 || InputColNumber == 0)
    {               
        m_MatrixData->RowNumber = 0;

        m_MatrixData->ColNumber = 0;

        m_MatrixData->DataArray.resize(0);

        m_ElementPointer = nullptr;

        return true;
    }

    auto SelfSize = this->GetSize();

    // if self is empty
    if (SelfSize.RowNumber <= 0)
    {
        m_MatrixData->RowNumber = InputRowNumber;

        m_MatrixData->ColNumber = InputColNumber;

        m_MatrixData->DataArray.resize(InputRowNumber*InputColNumber);

        m_ElementPointer = m_MatrixData->DataArray.data();

        return true;
    }

    // if self is not empty, and nothing will change
    if (InputRowNumber == SelfSize.RowNumber && InputColNumber == SelfSize.ColNumber)
    {
        return true;
    }

    // if only Col changes and self is not empty

    if (InputRowNumber == SelfSize.RowNumber)
    {
        auto Self_ElementNumber = SelfSize.RowNumber * SelfSize.ColNumber;

        if (int64(m_MatrixData->DataArray.capacity()) - Self_ElementNumber < (InputColNumber - SelfSize.ColNumber)*SelfSize.RowNumber)
        {
            m_MatrixData->DataArray.reserve((SelfSize.ColNumber + MDK_DenseMatrix_ColExpansionStep)*SelfSize.RowNumber);
        }

        m_MatrixData->RowNumber = InputRowNumber;

        m_MatrixData->ColNumber = InputColNumber;

        m_MatrixData->DataArray.resize(InputRowNumber*InputColNumber);

        m_ElementPointer = m_MatrixData->DataArray.data();

        return true;
    }

    // RowNumber != SelfSize.RowNumber and self is not empty -----------------------------------------------------------------------------

    auto tempDataArray = std::vector<ElementType>(InputRowNumber*InputColNumber);

    auto RawPointer = m_MatrixData->DataArray.data();

    auto ColNumber_min = std::min(SelfSize.ColNumber, InputColNumber);

    auto RowNumber_min = std::min(SelfSize.RowNumber, InputRowNumber);

    for (int64 j = 0; j < ColNumber_min; ++j)
    {
        auto tempIndex = j*InputRowNumber;

        auto Index = j*SelfSize.RowNumber;

        for (int64 i = 0; i < RowNumber_min; ++i)
        {
            tempDataArray[tempIndex + i] = RawPointer[Index + i];
        }
    }

    //-------------------------------------------------------------

    m_MatrixData->RowNumber = InputRowNumber;

    m_MatrixData->ColNumber = InputColNumber;

    m_MatrixData->DataArray.clear();
    m_MatrixData->DataArray = std::move(tempDataArray);

    m_ElementPointer = m_MatrixData->DataArray.data();

    return true;
}


template<typename ElementType>
inline 
void DenseMatrix<ElementType>::FixSize()
{
    m_IsSizeFixed = true;

    m_MatrixData->DataArray.shrink_to_fit();

    m_ElementPointer = m_MatrixData->DataArray.data();
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::IsSizeFixed() const
{
    return m_IsSizeFixed;
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::IsEmpty() const
{
    if (m_MatrixData->RowNumber > 0)
    {
        return false;
    }
    
    return true;
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::IsShared() const
{
    if (m_MatrixData.use_count() > 1)
    {
        return true;
    }

    return false;
}


template<typename ElementType>
inline
MatrixSize DenseMatrix<ElementType>::GetSize() const
{
    MatrixSize Size;

    Size.RowNumber = m_MatrixData->RowNumber;

    Size.ColNumber = m_MatrixData->ColNumber;

    return Size;
}


template<typename ElementType>
inline
int64 DenseMatrix<ElementType>::GetElementNumber() const
{
    return m_MatrixData->RowNumber * m_MatrixData->ColNumber;
}


template<typename ElementType>
inline 
int64 DenseMatrix<ElementType>::GetColNumber() const
{
    return m_MatrixData->ColNumber;
}


template<typename ElementType>
inline 
int64 DenseMatrix<ElementType>::GetRowNumber() const
{
    return m_MatrixData->RowNumber;
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::IsNonemptyVector() const
{
    if (m_MatrixData->RowNumber == 1 || m_MatrixData->ColNumber == 1)
    {
        return true;
    }

    return false;
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::IsNonemptyRowVector() const
{
    if (m_MatrixData->RowNumber == 1)
    {
        return true;
    }

    return false;
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::IsNonemptyColVector() const
{
    if (m_MatrixData->ColNumber == 1)
    {
        return true;
    }

    return false;
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::IsNonemptySquare() const
{
    if (m_MatrixData->RowNumber > 0 && m_MatrixData->RowNumber == m_MatrixData->ColNumber)
    {
        return true;
    }

    return false;
}


template<typename ElementType>
inline
const ElementType& DenseMatrix<ElementType>::GetNaNElement()  const
{
    return m_NaNElement;
}


template<typename ElementType>
inline
MatrixElementTypeEnum DenseMatrix<ElementType>::GetElementType() const
{
    return FindMatrixElementType(m_NaNElement);
}


template<typename ElementType>
inline
ElementType* DenseMatrix<ElementType>::GetElementPointer()
{
    return m_MatrixData->DataArray.data();
}


template<typename ElementType>
inline
const ElementType* DenseMatrix<ElementType>::GetElementPointer() const
{
    return m_MatrixData->DataArray.data();
}

//----------- Get/Set Matrix(LinearIndex) -----------------------------------//

// operator[] (): no bound check in release mode

template<typename ElementType>
inline
ElementType& DenseMatrix<ElementType>::operator[](int64 LinearIndex)
{
#if defined(MDK_DEBUG_DenseMatrix_Operator_CheckBound)

    if (LinearIndex >= this->GetElementNumber()|| LinearIndex < 0)
    {
        MDK_Error << "Invalid Input @ mdkDenseMatrix::operator[](i)" << '\n';

        return m_NaNElement;
    }

#endif //MDK_DEBUG_DenseMatrix_Operator_CheckBound

    return m_ElementPointer[LinearIndex];
}


template<typename ElementType>
inline
const ElementType& DenseMatrix<ElementType>::operator[](int64 LinearIndex) const
{
#if defined(MDK_DEBUG_DenseMatrix_Operator_CheckBound)

    if (LinearIndex >= this->GetElementNumber() || LinearIndex < 0)
    {
        MDK_Error << "Invalid Input @ mdkDenseMatrix::operator[](i) const" << '\n';

        return m_NaNElement;
    }

#endif //MDK_DEBUG_DenseMatrix_Operator_CheckBound

    return m_ElementPointer[LinearIndex];
}


template<typename ElementType>
inline
ElementType& DenseMatrix<ElementType>::operator()(int64 LinearIndex)
{
#if defined(MDK_DEBUG_DenseMatrix_Operator_CheckBound)

    if (LinearIndex >= this->GetElementNumber() || LinearIndex < 0)
    {
        MDK_Error << "Invalid Input @ mdkDenseMatrix::operator()(i)" << '\n';

        return m_NaNElement;
    }

#endif //MDK_DEBUG_DenseMatrix_Operator_CheckBound

	return m_ElementPointer[LinearIndex];
}


template<typename ElementType>
inline
const ElementType& DenseMatrix<ElementType>::operator()(int64 LinearIndex) const
{
#if defined(MDK_DEBUG_DenseMatrix_Operator_CheckBound)

    if (LinearIndex >= this->GetElementNumber() || LinearIndex < 0)
    {
        MDK_Error << "Invalid Input @ mdkDenseMatrix::operator()(i) const" << '\n';

        return m_NaNElement;
    }

#endif //MDK_DEBUG_DenseMatrix_Operator_CheckBound

	return m_ElementPointer[LinearIndex];
}

// at(): bound check

template<typename ElementType>
inline
ElementType& DenseMatrix<ElementType>::at(int64 LinearIndex)
{
    if (LinearIndex >= this->GetElementNumber() || LinearIndex < 0)
	{
		MDK_Error << "Invalid Input @ mdkDenseMatrix::at(i)" << '\n';
        
        return m_NaNElement;
	}

	return m_ElementPointer[LinearIndex];
}


template<typename ElementType>
inline
const ElementType& DenseMatrix<ElementType>::at(int64 LinearIndex) const
{
    if (LinearIndex >= this->GetElementNumber() || LinearIndex < 0)
	{
		MDK_Error << "Invalid Input @ mdkDenseMatrix::at(i) const" << '\n';
        
        return m_NaNElement;
	}

	return m_ElementPointer[LinearIndex];
}

//----------- Get/Set Matrix(i,j)  ---------------------------------------------//

// operator(): no bound check in release mode

template<typename ElementType>
inline
ElementType& DenseMatrix<ElementType>::operator()(int64 RowIndex, int64 ColIndex)
{
#if defined(MDK_DEBUG_DenseMatrix_Operator_CheckBound)

    auto SelfSize = this->GetSize();

    if (RowIndex >= SelfSize.RowNumber || RowIndex < 0 || ColIndex >= SelfSize.ColNumber || ColIndex < 0)
    {
        MDK_Error << "Invalid Input @ mdkDenseMatrix::operator()(i,j)" << '\n';

        return m_NaNElement;
    }

#endif //MDK_DEBUG_DenseMatrix_Operator_CheckBound
    
    return (*m_MatrixData)(RowIndex, ColIndex);

}


template<typename ElementType>
inline
const ElementType& DenseMatrix<ElementType>::operator()(int64 RowIndex, int64 ColIndex) const
{
#if defined(MDK_DEBUG_DenseMatrix_Operator_CheckBound)

    auto SelfSize = this->GetSize();

    if (RowIndex >= SelfSize.RowNumber || RowIndex < 0 || ColIndex >= SelfSize.ColNumber || ColIndex < 0)
    {
        MDK_Error << "Invalid Input @ mdkDenseMatrix::operator()(i,j) const" << '\n';

        return m_NaNElement;
    }

#endif //MDK_DEBUG_DenseMatrix_Operator_CheckBound

    return (*m_MatrixData)(RowIndex, ColIndex);
}


template<typename ElementType>
inline
ElementType& DenseMatrix<ElementType>::at(int64 RowIndex, int64 ColIndex)
{
    auto SelfSize = this->GetSize();

    if (RowIndex >= SelfSize.RowNumber || RowIndex < 0 || ColIndex >= SelfSize.ColNumber || ColIndex < 0)
    {
        MDK_Error << "Invalid Input @ mdkDenseMatrix::at(i,j)" << '\n';
        
        return m_NaNElement;
    }
    
    return (*m_MatrixData)(RowIndex, ColIndex);
}


template<typename ElementType>
inline
const ElementType& DenseMatrix<ElementType>::at(int64 RowIndex, int64 ColIndex) const
{
    auto SelfSize = this->GetSize();

    if (RowIndex >= SelfSize.RowNumber || RowIndex < 0 || ColIndex >= SelfSize.ColNumber || ColIndex < 0)
    {
        MDK_Error << "Invalid Input @ mdkDenseMatrix::at(i,j) const" << '\n';
        
        return m_NaNElement;
    }

    return (*m_MatrixData)(RowIndex, ColIndex);
}

//---------------------- Get/Set a set of elements ------------------------------------------------------//

// operator(): no bound check in release mode
//
// note: operator[] is for single element access only, operator[{}] is not defined

template<typename ElementType>
inline
DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::operator()(std::initializer_list<int64>& LinearIndexList)
{
#if defined(MDK_DEBUG_DenseMatrix_Operator_CheckBound)

    if (LinearIndexList.size() <= 0)
    {
        MDK_Error << "LinearIndexList is empty @ mdkDenseMatrix::operator()(std::initializer_list {LinearIndexList})" << '\n';
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto Self_ElementNumber = this->GetElementNumber();

    for (auto it = LinearIndexList.begin(); it != LinearIndexList.end(); ++it)
    {
        if (*it >= Self_ElementNumber || *it < 0)
        {
            MDK_Error << "Invalid LinearIndexList @ mdkDenseMatrix::operator()(std::initializer_list {LinearIndexList})" << '\n';
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
DenseMatrix<ElementType>::operator()(std::initializer_list<int64>& LinearIndexList) const
{
#if defined(MDK_DEBUG_DenseMatrix_Operator_CheckBound)

    if (LinearIndexList.size() <= 0)
    {
        MDK_Error << "LinearIndexList is empty @ mdkDenseMatrix::operator()(std::initializer_list {LinearIndexList}) const" << '\n';
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto Self_ElementNumber = this->GetElementNumber();

    for (auto it = LinearIndexList.begin(); it != LinearIndexList.end(); ++it)
    {
        if (*it >= Self_ElementNumber || *it < 0)
        {
            MDK_Error << "Invalid LinearIndexList @ mdkDenseMatrix::operator()(std::initializer_list {LinearIndexList}) const" << '\n';
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
DenseMatrix<ElementType>::operator()(const std::vector<int64>& LinearIndexList)
{
#if defined(MDK_DEBUG_DenseMatrix_Operator_CheckBound)

    if (LinearIndexList.size() <= 0)
    {
        MDK_Error << "LinearIndexList is empty @ mdkDenseMatrix::operator()(std::vector {LinearIndexList})" << '\n';
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto Self_ElementNumber = this->GetElementNumber();

    for (auto it = LinearIndexList.begin(); it != LinearIndexList.end(); ++it)
    {
        if (*it >= Self_ElementNumber || *it < 0)
        {
            MDK_Error << "Invalid LinearIndexList @ mdkDenseMatrix::operator()(std::vector {LinearIndexList})" << '\n';
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
DenseMatrix<ElementType>::operator()(const std::vector<int64>& LinearIndexList) const
{
#if defined(MDK_DEBUG_DenseMatrix_Operator_CheckBound)

    if (LinearIndexList.size() <= 0)
    {
        MDK_Error << "LinearIndexList is empty @ mdkDenseMatrix::operator()(std::vector {LinearIndexList}) const" << '\n';
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto Self_ElementNumber = this->GetElementNumber();

    for (auto it = LinearIndexList.begin(); it != LinearIndexList.end(); ++it)
    {
        if (*it >= Self_ElementNumber || *it < 0)
        {
            MDK_Error << "Invalid LinearIndexList @ mdkDenseMatrix::operator()(std::vector {LinearIndexList}) const" << '\n';
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
DenseMatrix<ElementType>::operator()(const ALL_Symbol_For_Matrix_Operator& ALL_Symbol)
{
    DenseShadowMatrix<ElementType> tempShadowMatrix(*this, ALL_Symbol);

    return tempShadowMatrix;
}


template<typename ElementType>
inline
const DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::operator()(const ALL_Symbol_For_Matrix_Operator& ALL_Symbol) const
{
    DenseShadowMatrix<ElementType> tempShadowMatrix(*this, ALL_Symbol);

    return tempShadowMatrix;
}


template<typename ElementType>
inline
DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::at(std::initializer_list<int64>& LinearIndexList)
{
    if (LinearIndexList.size() <= 0)
    {
        MDK_Error << "LinearIndexList is empty @ mdkDenseMatrix::at(std::initializer_list {LinearIndexList})" << '\n';
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto Self_ElementNumber = this->GetElementNumber();

    for (auto it = LinearIndexList.begin(); it != LinearIndexList.end(); ++it)
    {
        if (*it >= Self_ElementNumber || *it < 0)
        {
            MDK_Error << "Invalid LinearIndexList @ mdkDenseMatrix::at(std::initializer_list {LinearIndexList})" << '\n';
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
DenseMatrix<ElementType>::at(std::initializer_list<int64>& LinearIndexList) const
{
    if (LinearIndexList.size() <= 0)
    {
        MDK_Error << "LinearIndexList is empty @ mdkDenseMatrix::at(std::initializer_list {LinearIndexList}) const" << '\n';
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto Self_ElementNumber = this->GetElementNumber();

    for (auto it = LinearIndexList.begin(); it != LinearIndexList.end(); ++it)
    {
        if (*it >= Self_ElementNumber || *it < 0)
        {
            MDK_Error << "Invalid LinearIndexList @ mdkDenseMatrix::at(std::initializer_list {LinearIndexList}) const" << '\n';
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
DenseMatrix<ElementType>::at(const std::vector<int64>& LinearIndexList)
{
    if (LinearIndexList.size() <= 0)
    {
        MDK_Error << "LinearIndexList is empty @ mdkDenseMatrix::at(std::vector {LinearIndexList})" << '\n';
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto Self_ElementNumber = this->GetElementNumber();

    for (auto it = LinearIndexList.begin(); it != LinearIndexList.end(); ++it)
    {
        if (*it >= Self_ElementNumber || *it < 0)
        {
            MDK_Error << "Invalid LinearIndexList @ mdkDenseMatrix::at(std::vector {LinearIndexList})" << '\n';
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
DenseMatrix<ElementType>::at(const std::vector<int64>& LinearIndexList) const
{
    if (LinearIndexList.size() <= 0)
    {
        MDK_Error << "LinearIndexList is empty @ mdkDenseMatrix::at(std::vector {LinearIndexList} const)" << '\n';
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto Self_ElementNumber = this->GetElementNumber();

    for (auto it = LinearIndexList.begin(); it != LinearIndexList.end(); ++it)
    {
        if (*it >= Self_ElementNumber || *it < 0)
        {
            MDK_Error << "Invalid LinearIndexList @ mdkDenseMatrix::at(std::vector {LinearIndexList} const)" << '\n';
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
DenseMatrix<ElementType>::at(const ALL_Symbol_For_Matrix_Operator& ALL_Symbol)
{
    DenseShadowMatrix<ElementType> tempShadowMatrix(*this, ALL_Symbol);

    return tempShadowMatrix;
}


template<typename ElementType>
inline
const DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::at(const ALL_Symbol_For_Matrix_Operator& ALL_Symbol) const
{
    DenseShadowMatrix<ElementType> tempShadowMatrix(*this, ALL_Symbol);

    return tempShadowMatrix;
}

//---------------------- Get/Set SubMatrix ------------------------------------------------------//

// operator(): no bound check

template<typename ElementType>
inline
DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::operator()(std::initializer_list<int64>& RowIndexList, std::initializer_list<int64>& ColIndexList)
{
#if defined(MDK_DEBUG_DenseMatrix_Operator_CheckBound)

    if (RowIndexList.size() <= 0 || ColIndexList.size() <= 0)
    {
        MDK_Error << "RowIndexList or  ColIndexList is empty @ mdkDenseMatrix::operator()(std::initializer_list {RowIndexList}, {ColIndexList})" << '\n';
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = RowIndexList.begin(); it != RowIndexList.end(); ++it)
    {
        if (*it >= SelfSize.RowNumber || *it < 0)
        {
            MDK_Error << "Invalid RowIndexList @ mdkDenseMatrix::operator()(std::initializer_list {RowIndexList}, {ColIndexList})" << '\n';
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= SelfSize.ColNumber || *it < 0)
        {
            MDK_Error << "Invalid ColIndexList @ mdkDenseMatrix::operator()(std::initializer_list {RowIndexList}, {ColIndexList})" << '\n';
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
DenseMatrix<ElementType>::operator()(std::initializer_list<int64>& RowIndexList, std::initializer_list<int64>& ColIndexList) const
{
#if defined(MDK_DEBUG_DenseMatrix_Operator_CheckBound)

    if (RowIndexList.size() <= 0 || ColIndexList.size() <= 0)
    {
        MDK_Error << "RowIndexList or  ColIndexList is empty @ mdkDenseMatrix::operator()(std::initializer_list {RowIndexList}, {ColIndexList}) const" << '\n';
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = RowIndexList.begin(); it != RowIndexList.end(); ++it)
    {
        if (*it >= SelfSize.RowNumber || *it < 0)
        {
            MDK_Error << "Invalid RowIndexList @ mdkDenseMatrix::operator()(std::initializer_list {RowIndexList}, {ColIndexList}) const" << '\n';
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= SelfSize.ColNumber || *it < 0)
        {
            MDK_Error << "Invalid ColIndexList @ mdkDenseMatrix::operator()(std::initializer_list {RowIndexList}, {ColIndexList}) const" << '\n';
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
DenseMatrix<ElementType>::operator()(const std::initializer_list<int64>& RowIndexList, const ALL_Symbol_For_Matrix_Operator& ALL_Symbol)
{
#if defined(MDK_DEBUG_DenseMatrix_Operator_CheckBound)

    if (RowIndexList.size() <= 0)
    {
        MDK_Error << "RowIndexList is empty @ mdkDenseMatrix::operator()(std::initializer_list {RowIndexList}, ALL)" << '\n';
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = RowIndexList.begin(); it != RowIndexList.end(); ++it)
    {
        if (*it >= SelfSize.RowNumber || *it < 0)
        {
            MDK_Error << "Invalid RowIndexList @ mdkDenseMatrix::operator()(std::initializer_list {RowIndexList}, ALL)" << '\n';
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
DenseMatrix<ElementType>::operator()(const std::initializer_list<int64>& RowIndexList, const ALL_Symbol_For_Matrix_Operator& ALL_Symbol) const
{
#if defined(MDK_DEBUG_DenseMatrix_Operator_CheckBound)

    if (RowIndexList.size() <= 0)
    {
        MDK_Error << "RowIndexList is empty @ mdkDenseMatrix::operator()(std::initializer_list {RowIndexList}, ALL) const" << '\n';
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = RowIndexList.begin(); it != RowIndexList.end(); ++it)
    {
        if (*it >= SelfSize.RowNumber || *it < 0)
        {
            MDK_Error << "Invalid RowIndexList @ mdkDenseMatrix::operator()(std::initializer_list {RowIndexList}, ALL) const" << '\n';
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
DenseMatrix<ElementType>::operator()(const ALL_Symbol_For_Matrix_Operator& ALL_Symbol, const std::initializer_list<int64>& ColIndexList)
{
#if defined(MDK_DEBUG_DenseMatrix_Operator_CheckBound)

    if (ColIndexList.size() <= 0)
    {
        MDK_Error << "ColIndexList is empty @ mdkDenseMatrix::operator()(ALL, std::initializer_list {ColIndexList})" << '\n';
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= SelfSize.ColNumber || *it < 0)
        {
            MDK_Error << "Invalid ColIndexList @ mdkDenseMatrix::operator()(ALL, std::initializer_list {ColIndexList})" << '\n';
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
DenseMatrix<ElementType>::operator()(const ALL_Symbol_For_Matrix_Operator& ALL_Symbol, const std::initializer_list<int64>& ColIndexList) const
{
#if defined(MDK_DEBUG_DenseMatrix_Operator_CheckBound)

    if (ColIndexList.size() <= 0)
    {
        MDK_Error << "ColIndexList is empty @ mdkDenseMatrix::operator()(ALL, std::initializer_list {ColIndexList}) const" << '\n';
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= SelfSize.ColNumber || *it < 0)
        {
            MDK_Error << "Invalid ColIndexList @ mdkDenseMatrix::operator()(ALL, std::initializer_list {ColIndexList}) const" << '\n';
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
DenseMatrix<ElementType>::operator()(const std::vector<int64>& RowIndexList, const std::vector<int64>& ColIndexList)
{
#if defined(MDK_DEBUG_DenseMatrix_Operator_CheckBound)

    if (RowIndexList.size() <= 0 || ColIndexList.size() <= 0)
    {
        MDK_Error << "RowIndexList or  ColIndexList is empty @ mdkDenseMatrix::operator()(std::vector {RowIndexList}, {ColIndexList})" << '\n';
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = RowIndexList.begin(); it != RowIndexList.end(); ++it)
    {
        if (*it >= SelfSize.RowNumber || *it < 0)
        {
            MDK_Error << "Invalid RowIndexList @ mdkDenseMatrix::operator()(std::vector {RowIndexList}, {ColIndexList})" << '\n';
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= SelfSize.ColNumber || *it < 0)
        {
            MDK_Error << "Invalid ColIndexList @ mdkDenseMatrix::operator()(std::vector {RowIndexList}, {ColIndexList})" << '\n';
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
DenseMatrix<ElementType>::operator()(const std::vector<int64>& RowIndexList, const std::vector<int64>& ColIndexList) const 
{
#if defined(MDK_DEBUG_DenseMatrix_Operator_CheckBound)

    if (RowIndexList.size() <= 0 || ColIndexList.size() <= 0)
    {
        MDK_Error << "RowIndexList or  ColIndexList is empty @ mdkDenseMatrix::operator()(std::vector {RowIndexList}, {ColIndexList}) const" << '\n';
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = RowIndexList.begin(); it != RowIndexList.end(); ++it)
    {
        if (*it >= SelfSize.RowNumber || *it < 0)
        {
            MDK_Error << "Invalid RowIndexList @ mdkDenseMatrix::operator()(std::vector {RowIndexList}, {ColIndexList}) const" << '\n';
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= SelfSize.ColNumber)
        {
            MDK_Error << "Invalid ColIndexList @ mdkDenseMatrix::operator()(std::vector {RowIndexList}, {ColIndexList})" << '\n';
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
DenseMatrix<ElementType>::operator()(const std::vector<int64>& RowIndexList, const ALL_Symbol_For_Matrix_Operator& ALL_Symbol)
{
#if defined(MDK_DEBUG_DenseMatrix_Operator_CheckBound)

    if (RowIndexList.size() <= 0)
    {
        MDK_Error << "RowIndexList or  ColIndexList is empty @ mdkDenseMatrix::operator()(std::vector {RowIndexList}, ALL)" << '\n';
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = RowIndexList.begin(); it != RowIndexList.end(); ++it)
    {
        if (*it >= SelfSize.RowNumber || *it < 0)
        {
            MDK_Error << "Invalid RowIndexList @ mdkDenseMatrix::operator()(std::vector {RowIndexList}, ALL)" << '\n';
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
DenseMatrix<ElementType>::operator()(const std::vector<int64>& RowIndexList, const ALL_Symbol_For_Matrix_Operator& ALL_Symbol) const
{
#if defined(MDK_DEBUG_DenseMatrix_Operator_CheckBound)

    if (RowIndexList.size() <= 0)
    {
        MDK_Error << "RowIndexList or  ColIndexList is empty @ mdkDenseMatrix::operator()(std::vector {RowIndexList}, ALL) const" << '\n';
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = RowIndexList.begin(); it != RowIndexList.end(); ++it)
    {
        if (*it >= SelfSize.RowNumber || *it < 0)
        {
            MDK_Error << "Invalid RowIndexList @ mdkDenseMatrix::operator()(std::vector {RowIndexList}, ALL) const" << '\n';
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
DenseMatrix<ElementType>::operator()(const ALL_Symbol_For_Matrix_Operator& ALL_Symbol, const std::vector<int64>& ColIndexList)
{
#if defined(MDK_DEBUG_DenseMatrix_Operator_CheckBound)

    if (ColIndexList.size() <= 0)
    {
        MDK_Error << "ColIndexList is empty @ mdkDenseMatrix::operator()(ALL, std::vector {ColIndexList})" << '\n';
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= SelfSize.ColNumber || *it < 0)
        {
            MDK_Error << "Invalid ColIndexList @ mdkDenseMatrix::operator()(ALL, std::vector {ColIndexList})" << '\n';
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
DenseMatrix<ElementType>::operator()(const ALL_Symbol_For_Matrix_Operator& ALL_Symbol, const std::vector<int64>& ColIndexList) const
{
#if defined(MDK_DEBUG_DenseMatrix_Operator_CheckBound)

    if (ColIndexList.size() <= 0)
    {
        MDK_Error << "ColIndexList is empty @ mdkDenseMatrix::operator()(ALL, std::vector {ColIndexList}) const" << '\n';
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= SelfSize.ColNumber || *it < 0)
        {
            MDK_Error << "Invalid ColIndexList @ mdkDenseMatrix::operator()(ALL, std::vector {ColIndexList}) const" << '\n';
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
DenseMatrix<ElementType>::at(std::initializer_list<int64>& RowIndexList, std::initializer_list<int64>& ColIndexList)
{
    if (RowIndexList.size() <= 0 || ColIndexList.size() <= 0)
    {
        MDK_Error << "RowIndexList or  ColIndexList is empty @ mdkDenseMatrix::at(std::initializer_list {RowIndexList}, {ColIndexList})" << '\n';
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = RowIndexList.begin(); it != RowIndexList.end(); ++it)
    {
        if (*it >= SelfSize.RowNumber || *it < 0)
        {
            MDK_Error << "Invalid RowIndexList @ mdkDenseMatrix::at(std::initializer_list {RowIndexList}, {ColIndexList})" << '\n';
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= SelfSize.ColNumber || *it < 0)
        {
            MDK_Error << "Invalid ColIndexList @ mdkDenseMatrix::at({RowIndexList}, {ColIndexList})" << '\n';
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
DenseMatrix<ElementType>::at(std::initializer_list<int64>& RowIndexList, std::initializer_list<int64>& ColIndexList) const
{
    if (RowIndexList.size() <= 0 || ColIndexList.size() <= 0)
    {
        MDK_Error << "RowIndexList or  ColIndexList is empty @ mdkDenseMatrix::at(std::initializer_list {RowIndexList}, {ColIndexList}) const" << '\n';
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = RowIndexList.begin(); it != RowIndexList.end(); ++it)
    {
        if (*it >= SelfSize.RowNumber || *it < 0)
        {
            MDK_Error << "Invalid RowIndexList @ mdkDenseMatrix::at(std::initializer_list {RowIndexList}, {ColIndexList}) const" << '\n';
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= SelfSize.ColNumber || *it < 0)
        {
            MDK_Error << "Invalid ColIndexList @ mdkDenseMatrix::at({RowIndexList}, {ColIndexList})" << '\n';
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
DenseMatrix<ElementType>::at(const std::initializer_list<int64>& RowIndexList, const ALL_Symbol_For_Matrix_Operator& ALL_Symbol)
{
    if (RowIndexList.size() <= 0)
    {
        MDK_Error << "RowIndexList is empty @ mdkDenseMatrix::at(std::initializer_list {RowIndexList}, ALL)" << '\n';
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = RowIndexList.begin(); it != RowIndexList.end(); ++it)
    {
        if (*it >= SelfSize.RowNumber || *it < 0)
        {
            MDK_Error << "Invalid RowIndexList @ mdkDenseMatrix::at(std::initializer_list {RowIndexList}, ALL)" << '\n';
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
DenseMatrix<ElementType>::at(const std::initializer_list<int64>& RowIndexList, const ALL_Symbol_For_Matrix_Operator& ALL_Symbol) const
{
    if (RowIndexList.size() <= 0)
    {
        MDK_Error << "RowIndexList is empty @ mdkDenseMatrix::at(std::initializer_list {RowIndexList}, ALL) const" << '\n';
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = RowIndexList.begin(); it != RowIndexList.end(); ++it)
    {
        if (*it >= SelfSize.RowNumber || *it < 0)
        {
            MDK_Error << "Invalid RowIndexList @ mdkDenseMatrix::at(std::initializer_list {RowIndexList}, ALL) const" << '\n';
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
DenseMatrix<ElementType>::at(const ALL_Symbol_For_Matrix_Operator& ALL_Symbol, const std::initializer_list<int64>& ColIndexList)
{
    if (ColIndexList.size() <= 0)
    {
        MDK_Error << "ColIndexList is empty @ mdkDenseMatrix::at(ALL, std::initializer_list {ColIndexList})" << '\n';
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= SelfSize.ColNumber || *it < 0)
        {
            MDK_Error << "Invalid ColIndexList @ mdkDenseMatrix::at(ALL, std::initializer_list {ColIndexList})" << '\n';
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
DenseMatrix<ElementType>::at(const ALL_Symbol_For_Matrix_Operator& ALL_Symbol, const std::initializer_list<int64>& ColIndexList) const
{
    if (ColIndexList.size() <= 0)
    {
        MDK_Error << "ColIndexList is empty @ mdkDenseMatrix::at(ALL, std::initializer_list {ColIndexList}) const" << '\n';
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= SelfSize.ColNumber || *it < 0)
        {
            MDK_Error << "Invalid ColIndexList @ mdkDenseMatrix::at(ALL, std::initializer_list {ColIndexList}) const" << '\n';
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
DenseMatrix<ElementType>::at(const std::vector<int64>& RowIndexList, const std::vector<int64>& ColIndexList)
{
    if (RowIndexList.size() <= 0 || ColIndexList.size() <= 0)
    {
        MDK_Error << "RowIndexList or  ColIndexList is empty @ mdkDenseMatrix::at(std::vector {RowIndexList}, {ColIndexList})" << '\n';
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = RowIndexList.begin(); it != RowIndexList.end(); ++it)
    {
        if (*it >= SelfSize.RowNumber || *it < 0)
        {
            MDK_Error << "Invalid RowIndexList @ mdkDenseMatrix::at(std::vector {RowIndexList}, {ColIndexList})" << '\n';
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= SelfSize.ColNumber || *it < 0)
        {
            MDK_Error << "Invalid ColIndexList @ mdkDenseMatrix::at(std::vector {RowIndexList}, {ColIndexList})" << '\n';
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
DenseMatrix<ElementType>::at(const std::vector<int64>& RowIndexList, const std::vector<int64>& ColIndexList) const
{
    if (RowIndexList.size() <= 0 || ColIndexList.size() <= 0)
    {
        MDK_Error << "RowIndexList or  ColIndexList is empty @ mdkDenseMatrix::at(std::vector {RowIndexList}, {ColIndexList}) const" << '\n';
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = RowIndexList.begin(); it != RowIndexList.end(); ++it)
    {
        if (*it >= SelfSize.RowNumber || *it < 0)
        {
            MDK_Error << "Invalid RowIndexList @ mdkDenseMatrix::at(std::vector {RowIndexList}, {ColIndexList}) const" << '\n';
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= SelfSize.ColNumber || *it < 0)
        {
            MDK_Error << "Invalid ColIndexList @ mdkDenseMatrix::at(std::vector {RowIndexList}, {ColIndexList}) const" << '\n';
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
DenseMatrix<ElementType>::at(const std::vector<int64>& RowIndexList, const ALL_Symbol_For_Matrix_Operator& ALL_Symbol)
{
    if (RowIndexList.size() <= 0)
    {
        MDK_Error << "RowIndexList is empty @ mdkDenseMatrix::at(std::vector {RowIndexList}, ALL)" << '\n';
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = RowIndexList.begin(); it != RowIndexList.end(); ++it)
    {
        if (*it >= SelfSize.RowNumber || *it < 0)
        {
            MDK_Error << "Invalid RowIndexList @ mdkDenseMatrix::at(std::vector {RowIndexList}, ALL)" << '\n';
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
DenseMatrix<ElementType>::at(const std::vector<int64>& RowIndexList, const ALL_Symbol_For_Matrix_Operator& ALL_Symbol) const
{
    if (RowIndexList.size() <= 0)
    {
        MDK_Error << "RowIndexList is empty @ mdkDenseMatrix::at(std::vector {RowIndexList}, ALL) const" << '\n';
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = RowIndexList.begin(); it != RowIndexList.end(); ++it)
    {
        if (*it >= SelfSize.RowNumber || *it < 0)
        {
            MDK_Error << "Invalid RowIndexList @ mdkDenseMatrix::at(std::vector {RowIndexList}, ALL) const" << '\n';
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
DenseMatrix<ElementType>::at(const ALL_Symbol_For_Matrix_Operator& ALL_Symbol, const std::vector<int64>& ColIndexList)
{
    if (ColIndexList.size() <= 0)
    {
        MDK_Error << "ColIndexList is empty @ mdkDenseMatrix::at(ALL, std::vector {ColIndexList})" << '\n';
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= SelfSize.ColNumber || *it < 0)
        {
            MDK_Error << "Invalid ColIndexList @ mdkDenseMatrix::at(ALL, std::vector {ColIndexList})" << '\n';
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
DenseMatrix<ElementType>::at(const ALL_Symbol_For_Matrix_Operator& ALL_Symbol, const std::vector<int64>& ColIndexList) const
{
    if (ColIndexList.size() <= 0)
    {
        MDK_Error << "ColIndexList is empty @ mdkDenseMatrix::at(ALL, std::vector {ColIndexList}) const" << '\n';
        DenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= SelfSize.ColNumber || *it < 0)
        {
            MDK_Error << "Invalid ColIndexList @ mdkDenseMatrix::at(ALL, std::vector {ColIndexList}) const" << '\n';
            DenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    DenseShadowMatrix<ElementType> tempShadowMatrix(*this, ALL_Symbol, ColIndexList);

    return tempShadowMatrix;
}

// return SubMatrix as Matrix

template<typename ElementType>
inline
DenseMatrix<ElementType> DenseMatrix<ElementType>::GetSubMatrix(const std::vector<int64>& RowIndexList, const std::vector<int64>& ColIndexList) const
{
    DenseMatrix<ElementType> tempMatrix; // empty matrix

    GetSubMatrix(tempMatrix, RowIndexList, ColIndexList);

    return tempMatrix;
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::GetSubMatrix(DenseMatrix<ElementType>& OutputMatrix,
                                          const std::vector<int64>& RowIndexList, 
                                          const std::vector<int64>& ColIndexList) const
{
    if (this == &OutputMatrix)
    {
        MDK_Error << "A Matrix tries to get a SubMatrix of itself @ mdkDenseMatrix::GetSubMatrix(OutputMatrix, RowIndexList, ColIndexList)" << '\n';
        return false;
    }

    auto OutputRowNumber = int64(RowIndexList.size());
    
    auto OutputColNumber = int64(ColIndexList.size());

    if (OutputRowNumber <= 0 || OutputColNumber <= 0)
    {
        MDK_Error << "RowIndexList or ColIndexList is empty @ mdkDenseMatrix::GetSubMatrix(OutputMatrix, RowIndexList, ColIndexList)" << '\n';
        return false;
    }

    auto SelfSize = this->GetSize();

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= SelfSize.ColNumber || *it < 0)
        {
            MDK_Error << "Invalid ColIndexList @ mdkDenseMatrix::GetSubMatrix(OutputMatrix, RowIndexList, ColIndexList)" << '\n';
            return false;
        }
    }

    for (auto it = RowIndexList.begin(); it != RowIndexList.end(); ++it)
    {
        if (*it >= SelfSize.RowNumber || *it < 0)
        {
            MDK_Error << "Invalid RowIndexList @ mdkDenseMatrix::GetSubMatrix(OutputMatrix, RowIndexList, ColIndexList)" << '\n';
            return false;
        }
    }

    if (OutputMatrix.GetColNumber() != OutputColNumber || OutputMatrix.GetRowNumber() != OutputRowNumber)
    {
        if (OutputMatrix.IsSizeFixed() == false)
        {
            OutputMatrix.Clear();
            OutputMatrix.Resize(OutputRowNumber, OutputColNumber);
        }
        else
        {
            MDK_Error << "Invalid OutputMatrix @ mdkDenseMatrix::GetSubMatrix(OutputMatrix, RowIndexList, ColIndexList)" << '\n';
            return false;
        }
    }


    auto tempRawPointer = OutputMatrix.GetElementPointer();

    auto RawPointer = this->GetElementPointer();

    for (int64 j = 0; j < OutputColNumber; ++j)
    {
        auto Index = ColIndexList[j] * SelfSize.RowNumber;

        for (int64 i = 0; i < OutputRowNumber; ++i)
        {
            int64 LinearIndex = Index + RowIndexList[i];

            tempRawPointer[0] = RawPointer[LinearIndex];

            ++tempRawPointer;
        }
    }

    return true;
}


template<typename ElementType>
inline 
DenseMatrix<ElementType> DenseMatrix<ElementType>::GetSubMatrix(const std::vector<int64>& RowIndexList, const ALL_Symbol_For_Matrix_Operator& ALL_Symbol) const
{
    auto SelfSize = this->GetSize();

    std::vector<int64> ColIndexList(SelfSize.ColNumber);

    for (int64 i = 0; i < SelfSize.ColNumber; ++i)
    {
        ColIndexList[i] = i;
    }

    return this->GetSubMatrix(RowIndexList, ColIndexList);
}

template<typename ElementType>
inline
DenseMatrix<ElementType> DenseMatrix<ElementType>::GetSubMatrix(const ALL_Symbol_For_Matrix_Operator& ALL_Symbol, const std::vector<int64>& ColIndexList) const
{
    auto SelfSize = this->GetSize();

    std::vector<int64> RowIndexList(SelfSize.RowNumber);

    for (int64 i = 0; i < SelfSize.RowNumber; ++i)
    {
        RowIndexList[i] = i;
    }

    return this->GetSubMatrix(RowIndexList, ColIndexList);
}


template<typename ElementType>
inline
DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::Col(int64 ColIndex)
{
    return this->at(ALL, { ColIndex });

    /*
    if (ColIndex >= SelfSize.ColNumber || ColIndex < 0)
    {
        MDK_Error << "Invalid Input @ mdkDenseMatrix::Col(ColIndex)" << '\n';
        mdkDenseShadowMatrix<ElementType> tempShadowMatrix;        
        return tempShadowMatrix;
    }

    mdkDenseShadowMatrix<ElementType> tempShadowMatrix(*this, ALL, {ColIndex});

    return tempShadowMatrix;
    */
}


template<typename ElementType>
inline
DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::Col(std::initializer_list<int64>& ColIndexList)
{
    return this->at(ALL, ColIndexList);

    /*
    if (ColIndexList.size() <= 0)
    {
        MDK_Error << "ColIndexList is empty @ mdkDenseMatrix::Col()(std::initializer_list {ColIndexList})" << '\n';
        mdkDenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= SelfSize.ColNumber)
        {
            MDK_Error << "Invalid ColIndexList @ mdkDenseMatrix::Col({ColIndexList})" << '\n';
            mdkDenseShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    mdkDenseShadowMatrix<ElementType> tempShadowMatrix(*this, ALL, ColIndexList);

    return tempShadowMatrix;
    */
}


template<typename ElementType>
inline
DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::Col(const std::vector<int64>& ColIndexList)
{
    return this->at(ALL, ColIndexList);

    /*
    if (ColIndexList.size() <= 0)
    {
        MDK_Error << "ColIndexList is empty @ mdkDenseMatrix::operator()(std::vector {ColIndexList})" << '\n';
        mdkDenseShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {        
        if (*it >= SelfSize.ColNumber)
        {
            MDK_Error << "Invalid ColIndexList @ mdkDenseMatrix::Col(std::vector ColIndexList)" << '\n';
            mdkDenseShadowMatrix<ElementType> tempShadowMatrix;            
            return tempShadowMatrix;
        }
    }

    mdkDenseShadowMatrix<ElementType> tempShadowMatrix(*this, ALL, ColIndexList);

    return tempShadowMatrix;
    */
}


template<typename ElementType>
inline 
DenseMatrix<ElementType> DenseMatrix<ElementType>::GetCol(int64 ColIndex) const
{
	DenseMatrix<ElementType> tempMatrix;

    auto SelfSize = this->GetSize();

    if (ColIndex >= SelfSize.ColNumber || ColIndex < 0)
	{
		MDK_Error << "Invalid Input @ mdkDenseMatrix::GetCol(ColIndex)" << '\n';
        
        return tempMatrix;
	}

    tempMatrix.Resize(SelfSize.RowNumber, 1);

    auto tempRawPointer = tempMatrix.GetElementPointer();

	auto RawPointer this->GetElementPointer();

	int64 Index = ColIndex*SelfSize.RowNumber;

	for (int64 i = 0; i < SelfSize.RowNumber; ++i)
	{
		tempRawPointer[i] = RawPointer[Index + i];
	}

    return tempMatrix;
}


template<typename ElementType>
inline 
bool DenseMatrix<ElementType>::GetCol(int64 ColIndex, std::vector<ElementType>& ColData) const
{
    auto SelfSize = this->GetSize();

	if (ColIndex >= SelfSize.ColNumber || ColIndex < 0)
	{
		MDK_Error << "Invalid Input @ mdkDenseMatrix::GetCol(int64 ColIndex, std::vector<ElementType>& ColData)" << '\n';
		return false;
	}

	ColData.resize(SelfSize.RowNumber);

	auto tempRawPointer = ColData.data();

	auto RawPointer this->GetElementPointer();

	int64 Index = ColIndex*SelfSize.RowNumber;

	for (int64 i = 0; i < SelfSize.RowNumber; ++i)
	{
		tempRawPointer[i] = RawPointer[Index + i];
	}

	return true;
}


template<typename ElementType>
inline 
bool DenseMatrix<ElementType>::GetCol(int64 ColIndex, ElementType* ColData) const
{
    auto SelfSize = this->GetSize();

    if (ColIndex >= SelfSize.ColNumber || ColIndex < 0 || ColData == nullptr)
	{
		MDK_Error << "Invalid Input @ mdkDenseMatrix::GetCol(int64 ColIndex, ElementType* ColData)" << '\n';
		return false;
	}

	auto RawPointer this->GetElementPointer();

	int64 Index = ColIndex*SelfSize.RowNumber;

	for (int64 i = 0; i < SelfSize.RowNumber; ++i)
	{
		ColData[i] = RawPointer[Index + i];
	}

	return true;
}


template<typename ElementType>
template<typename ElementType_Input>
inline 
bool DenseMatrix<ElementType>::SetCol(int64 ColIndex, const DenseMatrix<ElementType_Input>& ColData)
{
    auto SelfSize = this->GetSize();

	if (ColIndex >= SelfSize.ColNumber || ColIndex < 0)
	{
		MDK_Error << "Invalid Input : ColIndex is out of bound @ mdkDenseMatrix::SetCol(ColIndex, mdkDenseMatrix)" << '\n';
		return false;
	}

	auto Size = ColData.GetSize();

	if (Size.ColNumber != 1 || Size.RowNumber != 1)
	{
		MDK_Error << "Invalid Input : must be a vector @ mdkDenseMatrix::SetCol(ColIndex, mdkDenseMatrix)" << '\n';
		return false;
	}

	if (Size.RowNumber > 1 && Size.RowNumber != SelfSize.RowNumber)
	{
		MDK_Error << "Invalid Input : size does not match @ mdkDenseMatrix::SetCol(ColIndex, mdkDenseMatrix)" << '\n';
		return false;
	}

	if (Size.ColNumber > 1 && Size.ColNumber != SelfSize.RowNumber)
	{
		MDK_Error << "Invalid Input : size does not match @ mdkDenseMatrix::SetCol(ColIndex, mdkDenseMatrix)" << '\n';
		return false;
	}

    return this->SetCol(ColIndex, ColData.GetElementPointer(), std::max(Size.RowNumber, Size.ColNumber));
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool DenseMatrix<ElementType>::SetCol(int64 ColIndex, const std::initializer_list<ElementType_Input>& ColData)
{
    return this->SetCol(ColIndex, ColData.begin(), ColData.size());
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool DenseMatrix<ElementType>::SetCol(int64 ColIndex, const std::vector<ElementType_Input>& ColData)
{
    return this->SetCol(ColIndex, ColData.data(), ColData.size());
}


template<typename ElementType>
template<typename ElementType_Input>
inline 
bool DenseMatrix<ElementType>::SetCol(int64 ColIndex, const ElementType_Input* ColData, int64 Length)
{
    auto SelfSize = this->GetSize();

    if (ColData == nullptr || Length <= 0 || ColIndex >= SelfSize.ColNumber || ColIndex < 0)
	{
		MDK_Error << "Invalid Input @ mdkDenseMatrix::SetCol(ColIndex, const ElementType_Input* ColData, int64 Length)" << '\n';
		return false;
	}
    
    auto BeginPointer = this->GetElementPointer();

    int64 Index = ColIndex*SelfSize.RowNumber;

    // just for reference:
    //
    //for (int64 i = 0; i < SelfSize.RowNumber; ++i)
    //{
    //	RawPointer[Index + i] = ElementType(ColData[i]);
    //}

    auto tempColData = ColData;

    BeginPointer += Index;
    for (auto Ptr = BeginPointer; Ptr < BeginPointer + SelfSize.RowNumber; ++Ptr)
    {
        Ptr[0] = ElementType(tempColData[0]);

        ++tempColData;
    }

	return true;
}


template<typename ElementType>
inline 
bool DenseMatrix<ElementType>::FillCol(int64 ColIndex, const ElementType& Element)
{
    auto SelfSize = this->GetSize();

    if (ColIndex >= SelfSize.ColNumber || ColIndex < 0)
    {
        MDK_Error << "Invalid Input @ mdkDenseMatrix::FillCol(int64 ColIndex, const ElementType& Element)" << '\n';
        return false;
    }

    auto BeginPointer = this->GetElementPointer();

    int64 Index = ColIndex*SelfSize.RowNumber;

    BeginPointer += Index;
    for (auto Ptr = BeginPointer; Ptr < BeginPointer + SelfSize.RowNumber; ++Ptr)
    {
        Ptr[0] = Element;
    }

    return true;
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool DenseMatrix<ElementType>::AppendCol(const DenseMatrix<ElementType_Input>& ColData)
{
    if (m_IsSizeFixed == true)
    {
        MDK_Error << "Matrix Size can not change @ mdkDenseMatrix::AppendCol(const mdkDenseMatrix<ElementType_Input>& ColData)" << '\n';
        return false;
    }

    if (ColData.IsEmpty() == true)
    {
        MDK_Error << "Empty Input @ mdkDenseMatrix::AppendCol(const mdkDenseMatrix<ElementType_Input>& ColData)" << '\n';
        return false;
    }

    auto SelfSize = this->GetSize();

    auto RowNumber = SelfSize.RowNumber;

    if (RowNumber <= 0)
    {
        RowNumber = std::max(ColData.GetRowNumber(), ColData.GetColNumber());
    }

    this->Resize(RowNumber, SelfSize.ColNumber + 1);

    return this->SetCol(SelfSize.ColNumber, ColData);
}


template<typename ElementType>
template<typename ElementType_Input>
inline 
bool DenseMatrix<ElementType>::AppendCol(const std::initializer_list<ElementType_Input>& ColData)
{
    if (m_IsSizeFixed == true)
    {
        MDK_Error << "Matrix Size can not change @ mdkDenseMatrix::AppendCol(const std::initializer_list<ElementType_Input>& ColData)" << '\n';
        return false;
    }

    if (ColData.size() <= 0)
    {
        MDK_Error << "Empty Input @ mdkDenseMatrix::AppendCol(const std::initializer_list<ElementType_Input>& ColData)" << '\n';
        return false;
    }

    auto SelfSize = this->GetSize();

    auto  RowNumber = SelfSize.RowNumber;

    if (RowNumber <= 0)
    {
        RowNumber = ColData.size();
    }

    this->Resize(RowNumber, SelfSize.ColNumber + 1);

    return this->SetCol(SelfSize.ColNumber, ColData);
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool DenseMatrix<ElementType>::AppendCol(const std::vector<ElementType_Input>& ColData)
{
    if (m_IsSizeFixed == true)
    {
        MDK_Error << "Matrix Size can not change @ mdkDenseMatrix::AppendCol(const std::vector<ElementType_Input>& ColData)" << '\n';
        return false;
    }

    if (ColData.size() <= 0)
    {
        MDK_Error << "Empty Input @ mdkDenseMatrix::AppendCol(const std::vector<ElementType_Input>& ColData)" << '\n';
        return false;
    }

    auto SelfSize = this->GetSize();

    auto  RowNumber = SelfSize.RowNumber;

    if (RowNumber <= 0)
    {
        RowNumber = ColData.size();
    }

    this->Resize(RowNumber, SelfSize.ColNumber + 1);

    return this->SetCol(SelfSize.ColNumber, ColData);
}


template<typename ElementType>
template<typename ElementType_Input>
inline 
bool DenseMatrix<ElementType>::AppendCol(const ElementType_Input* ColData, int64 Length)
{
    if (m_IsSizeFixed == true)
    {
        MDK_Error << "Matrix Size can not change @ mdkDenseMatrix::AppendCol(const ElementType_Input* ColData, int64 Length)" << '\n';
        return false;
    }

    if (ColData == nullptr || Length <= 0)
    {
        MDK_Error << "Empty Input @ mdkDenseMatrix::AppendCol(const ElementType_Input* ColData, int64 Length)" << '\n';
        return false;
    }

    auto SelfSize = this->GetSize();

    auto  RowNumber = SelfSize.RowNumber;

    if (RowNumber <= 0)
    {
        RowNumber = Length;
    }

    this->Resize(RowNumber, SelfSize.ColNumber + 1);

    return this->SetCol(SelfSize.ColNumber, ColData, Length);
}


template<typename ElementType>
inline 
bool DenseMatrix<ElementType>::DeleteCol(int64 ColIndex)
{
    if (m_IsSizeFixed == true)
    {
        MDK_Error << "Matrix Size can not change @ mdkDenseMatrix::DeleteCol(int64 ColIndex)" << '\n';
        return false;
    }

    auto SelfSize = this->GetSize();

    if (ColIndex >= SelfSize.ColNumber || ColIndex < 0)
    {
        MDK_Error << "Invalid Input @ mdkDenseMatrix::DeleteCol(int64 ColIndex)" << '\n';
        return false;
    }

    m_MatrixData->DataArray.erase(m_MatrixData->DataArray.begin() + ColIndex*SelfSize.RowNumber, 
                                  m_MatrixData->DataArray.begin() + (ColIndex + 1)*SelfSize.RowNumber);

    m_MatrixData->ColNumber -= 1;

    m_ElementPointer = m_MatrixData->DataArray.data();

    return true;
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::DeleteCol(std::initializer_list<int64>& ColIndexList)
{
    if (m_IsSizeFixed == true)
    {
        MDK_Error << "Matrix Size can not change @ mdkDenseMatrix::DeleteCol(std::initializer_list<int64>& ColIndexList)" << '\n';
        return false;
    }

    if (ColIndexList.size() <= 0)
    {
        MDK_Error << "Empty Input @ mdkDenseMatrix::DeleteCol(std::initializer_list ColIndexList)" << '\n';
        return false;
    }

    auto SelfSize = this->GetSize();

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= SelfSize.ColNumber || *it < 0)
        {
            MDK_Error << "Out of bound Input @ mdkDenseMatrix::DeleteCol(std::initializer_list ColIndexList)" << '\n';
            return false;
        }
    }

    return this->DeleteCol(ColIndexList.begin(), ColIndexList.size());
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::DeleteCol(const std::vector<int64>& ColIndexList)
{
    if (m_IsSizeFixed == true)
    {
        MDK_Error << "Matrix Size can not change @ mdkDenseMatrix::DeleteCol(const std::vector<int64>& ColIndexList)" << '\n';
        return false;
    }

    if (ColIndexList.size() <= 0)
    {
        MDK_Error << "Empty Input @ mdkDenseMatrix::DeleteCol(std::vector ColIndexList)" << '\n';
        return false;
    }

    auto SelfSize = this->GetSize();

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= SelfSize.ColNumber || *it < 0)
        {
            MDK_Error << "Out of bound Input @ mdkDenseMatrix::DeleteCol(std::vector ColIndexList)" << '\n';
            return false;
        }
    }

    return this->DeleteCol(ColIndexList.data(), ColIndexList.size());
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::DeleteCol(const int64* ColIndexListPtr, int64 Length)
{
    if (m_IsSizeFixed == true)
    {
        MDK_Error << "Matrix Size can not change @ mdkDenseMatrix::DeleteCol(const int64* ColIndexPtr, int64 Length)" << '\n';
        return false;
    }

    if (ColIndexListPtr == nullptr || Length <= 0)
    {
        MDK_Error << "Empty Input @ mdkDenseMatrix::DeleteCol(const int64* ColIndexPtr, int64 Length)" << '\n';
        return false;
    }

    if (Length == 1)
    {
        return this->DeleteCol(ColIndexListPtr[0]);
    }

    // Length > 1 -------------------------------------------

    auto SelfSize = this->GetSize();

    std::vector<int64> ColIndexList_max_to_min(Length);

    for (int64 i = 0; i < Length; ++i)
    {
        ColIndexList_max_to_min[i] = ColIndexListPtr[i];
    }

    std::sort(ColIndexList_max_to_min.begin(), ColIndexList_max_to_min.end(), [](int64 a, int64 b) { return a > b; });

    int64 Index_prev = ColIndexList_max_to_min[0] + 1;

    for (int64 i = 0; i < int64(ColIndexList_max_to_min.size()); ++i)
    {
        auto Index_i = ColIndexList_max_to_min[i];

        if (Index_i == Index_prev)
        {
            MDK_Warning << "duplicate Input @ mdkDenseMatrix::DeleteCol(const int64* ColIndexPtr, int64 Length)" << '\n';
        }
        else
        {
            m_MatrixData->DataArray.erase(m_MatrixData->DataArray.begin() + Index_i * SelfSize.RowNumber,
                                          m_MatrixData->DataArray.begin() + (Index_i + 1)* SelfSize.RowNumber);

            Index_prev = Index_i;

            m_MatrixData->ColNumber -= 1;
        }
    }

    m_ElementPointer = m_MatrixData->DataArray.data();

    return true;
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool DenseMatrix<ElementType>::InsertCol(int64 ColIndex, const DenseMatrix<ElementType_Input>& ColData)
{
    if (m_IsSizeFixed == true)
    {
        MDK_Error << "Matrix Size can not change @ mdkDenseMatrix::InsertCol(ColIndex, mdkDenseMatrix)" << '\n';
        return false;
    }

    if (ColIndex >= SelfSize.ColNumber || ColIndex < 0)
    {
        MDK_Error << "Invalid Input : ColIndex is out of bound @ mdkDenseMatrix::InsertCol(ColIndex, mdkDenseMatrix)" << '\n';
        return false;
    }

    auto InputSize = ColData.GetSize();

    if (InputSize.ColNumber != 1 || InputSize.RowNumber != 1)
    {
        MDK_Error << "Invalid Input : must be a vector @ mdkDenseMatrix::InsertCol(ColIndex, mdkDenseMatrix)" << '\n';
        return false;
    }

    auto SelfSize = this->GetSize();

    if (InputSize.RowNumber > 1 && InputSize.RowNumber != SelfSize.RowNumber)
    {
        MDK_Error << "Invalid Input : size does not match @ mdkDenseMatrix::InsertCol(ColIndex, mdkDenseMatrix)" << '\n';
        return false;
    }

    if (InputSize.ColNumber > 1 && InputSize.ColNumber != SelfSize.RowNumber)
    {
        MDK_Error << "Invalid Input : size does not match @ mdkDenseMatrix::InsertCol(ColIndex, mdkDenseMatrix)" << '\n';
        return false;
    }

    return this->InsertCol(ColIndex, ColData.GetElementPointer(), std::max(InputSize.RowNumber, InputSize.ColNumber));

}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool DenseMatrix<ElementType>::InsertCol(int64 ColIndex, const std::initializer_list<ElementType_Input>& ColData)
{
    return this->InsertCol(ColIndex, ColData.begin(), ColData.size());
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool DenseMatrix<ElementType>::InsertCol(int64 ColIndex, const std::vector<ElementType_Input>& ColData)
{
    return this->InsertCol(ColIndex, ColData.data(), ColData.size());
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool DenseMatrix<ElementType>::InsertCol(int64 ColIndex, const ElementType_Input* ColData, int64 Length)
{
    if (m_IsSizeFixed == true)
    {
        MDK_Error << "Matrix Size can not change @ mdkDenseMatrix::InsertCol(ColIndex, const ElementType_Input* ColData, int64 Length)" << '\n';
        return false;
    }

    auto SelfSize = this->GetSize();

    if (ColData == nullptr || Length <= 0 || ColIndex >= SelfSize.ColNumber || ColIndex < 0)
    {
        MDK_Error << "Invalid Input @ mdkDenseMatrix::InsertCol(ColIndex, const ElementType_Input* ColData, int64 Length)" << '\n';
        return false;
    }

    m_MatrixData->DataArray.insert(m_MatrixData->DataArray.begin() + ColIndex*SelfSize.RowNumber, ColData, ColData + Length);

    m_MatrixData->ColNumber += 1;

    m_ElementPointer = m_MatrixData->DataArray.data();

    return true;
}


template<typename ElementType>
inline
DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::Row(int64 RowIndex)
{
    return this->at({RowIndex}, ALL);
}


template<typename ElementType>
inline
DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::Row(std::initializer_list<int64>& RowIndexList)
{
    return this->at(RowIndexList, ALL);
}


template<typename ElementType>
inline
DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::Row(const std::vector<int64>& RowIndexList)
{
    return this->at(RowIndexList, ALL);
}


template<typename ElementType>
inline
DenseMatrix<ElementType> DenseMatrix<ElementType>::GetRow(int64 RowIndex) const
{
    DenseMatrix<ElementType> tempMatrix;	

    auto SelfSize = this->GetSize();

	if (RowIndex >= SelfSize.RowNumber || RowIndex < 0)
	{
		MDK_Error << "Invalid Input @ mdkDenseMatrix::GetRow(RowIndex)" << '\n';
        
        return tempMatrix;
	}

    tempMatrix.Resize(1, SelfSize.ColNumber);

    auto tempRawPointer = tempMatrix.GetElementPointer();

    auto RawPointer = this->GetElementPointer();

	int64 Index = 0;

	for (int64 j = 0; j < SelfSize.ColNumber; ++j)
	{
		tempRawPointer[j] = RawPointer[Index + RowIndex];

		Index += SelfSize.RowNumber;
	}

    return tempMatrix;
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::GetRow(int64 RowIndex, std::vector<ElementType>& RowData) const
{
    auto SelfSize = this->GetSize();

	if (RowIndex >= SelfSize.RowNumber || RowIndex < 0)
	{
		MDK_Error << "Invalid Input @ mdkDenseMatrix::GetRow(RowIndex, std::vector<ElementType>& RowData)" << '\n';
		return false;
	}

	RowData.resize(SelfSize.ColNumber);

	auto tempRawPointer = RowData.data();

    auto RawPointer = this->GetElementPointer();

	int64 Index = 0;

	for (int64 j = 0; j < SelfSize.ColNumber; ++j)
	{
		tempRawPointer[j] = RawPointer[Index + RowIndex];

		Index += SelfSize.RowNumber;
	}

	return true;
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::GetRow(int64 RowIndex, ElementType* RowData) const
{
    auto SelfSize = this->GetSize();

	if (RowIndex >= SelfSize.RowNumber || RowIndex < 0)
	{
		MDK_Error << "Invalid Input @ mdkDenseMatrix GetRow(RowIndex, ElementType* RowData)" << '\n';
		return false;
	}

	auto RawPointer this->GetElementPointer();

	int64 Index = 0;

	for (int64 j = 0; j < SelfSize.ColNumber; ++j)
	{
		RowData[j] = RawPointer[Index + RowIndex];

		Index += SelfSize.RowNumber;
	}

	return true;
}


template<typename ElementType>
template<typename ElementType_Input>
inline 
bool DenseMatrix<ElementType>::SetRow(int64 RowIndex, const DenseMatrix<ElementType_Input>& RowData)
{
    auto SelfSize = this->GetSize();

    if (RowIndex >= SelfSize.RowNumber || RowIndex < 0)
    {
        MDK_Error << "Invalid Input @ mdkDenseMatrix SetRow(int64 RowIndex, const mdkDenseMatrix<ElementType_Input>& RowData)" << '\n';
        return false;
    }

	auto InputSize = RowData.GetSize();

    if (InputSize.ColNumber != 1 || InputSize.RowNumber != 1)
	{
		MDK_Error << "Invalid Input @ mdkDenseMatrix SetRow(RowIndex,mdkDenseMatrix)" << '\n';
		return false;
	}

    if (InputSize.RowNumber > 1 && InputSize.RowNumber != SelfSize.ColNumber)
	{
		MDK_Error << "Invalid Input @ mdkDenseMatrix SetRow(RowIndex,mdkDenseMatrix)" << '\n';
		return false;
	}

    if (InputSize.ColNumber > 1 && InputSize.ColNumber != SelfSize.ColNumber)
	{
		MDK_Error << "Invalid Input @ mdkDenseMatrix SetRow(RowIndex,mdkDenseMatrix)" << '\n';
		return false;
	}

    return this->SetRow(RowIndex, RowData.GetElementPointer(), std::max(InputSize.RowNumber, InputSize.ColNumber));
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool DenseMatrix<ElementType>::SetRow(int64 RowIndex, const std::initializer_list<ElementType_Input>& RowData)
{
    return this->SetRow(RowIndex, RowData.begin(), RowData.size());
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool DenseMatrix<ElementType>::SetRow(int64 RowIndex, const std::vector<ElementType_Input>& RowData)
{
    return this->SetRow(RowIndex, RowData.data(), RowData.size());
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool DenseMatrix<ElementType>::SetRow(int64 RowIndex, const ElementType_Input* RowData, int64 Length)
{
    auto SelfSize = this->GetSize();

    if (RowData == nullptr || Length <= 0 || RowIndex >= SelfSize.RowNumber || RowIndex < 0)
    {
        MDK_Error << "Empty Input @ mdkDenseMatrix::SetRow(RowIndex, const ElementType_Input* RowData, int64 Length)" << '\n';
        return false;
    }

    if (Length != SelfSize.ColNumber)
    {
        MDK_Error << "Length does not match @ mdkDenseMatrix::SetRow(RowIndex, const ElementType_Input* RowData, int64 Length)" << '\n';
        return false;
    }

    auto RawPointer = this->GetElementPointer();

    int64 Index = 0;

    for (int64 j = 0; j < SelfSize.ColNumber; ++j)
    {
        RawPointer[Index + RowIndex] = ElementType(RowData[j]);

        Index += SelfSize.RowNumber;
     }

    return true;
}


template<typename ElementType>
inline 
bool DenseMatrix<ElementType>::FillRow(int64 RowIndex, const ElementType& Element)
{
    auto SelfSize = this->GetSize();

    if (RowIndex >= SelfSize.RowNumber || RowIndex < 0)
    {
        MDK_Error << "Invalid Input @ mdkDenseMatrix::FillRow(int64 RowIndex, const ElementType& Element)" << '\n';
        return false;
    }

    auto RawPointer = this->GetElementPointer();

    int64 Index = 0;

    for (int64 j = 0; j < SelfSize.ColNumber; ++j)
    {
        RawPointer[Index + RowIndex] = Element;

        Index += SelfSize.RowNumber;
    }

    return true;
}


template<typename ElementType>
template<typename ElementType_Input>
inline 
bool DenseMatrix<ElementType>::AppendRow(const DenseMatrix<ElementType_Input>& RowData)
{
    if (m_IsSizeFixed == true)
    {
        MDK_Error << "Size can not change @ mdkDenseMatrix::AppendRow(const mdkDenseMatrix<ElementType_Input>& RowData)" << '\n';
        return false;
    }

    if (RowData.IsEmpty() == true)
    {
        MDK_Error << "Input is empty @ mdkDenseMatrix::AppendRow(const mdkDenseMatrix<ElementType_Input>& RowData)" << '\n';
        return false;
    }

    auto SelfSize = this->GetSize();

    auto  ColNumber = SelfSize.ColNumber;

    if (ColNumber <= 0)
    {
        ColNumber = std::max(RowData.GetRowNumber(), RowData.GetColNumber);
    }

    this->Resize(SelfSize.RowNumber + 1, ColNumber);

    return this->SetRow(SelfSize.RowNumber, RowData);
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool  DenseMatrix<ElementType>::AppendRow(const std::initializer_list<ElementType_Input>& RowData)
{
    if (m_IsSizeFixed == true)
    {
        MDK_Error << "Size can not change @ mdkDenseMatrix::AppendRow(const std::initializer_list<ElementType_Input>& RowData)" << '\n';
        return false;
    }

    if (RowData.size() <= 0)
    {
        MDK_Error << "Input is empty @ mdkDenseMatrix::AppendRow(const std::initializer_list<ElementType_Input>& RowData)" << '\n';
        return false;
    }

    auto SelfSize = this->GetSize();

    auto  ColNumber = SelfSize.ColNumber;

    if (ColNumber <= 0)
    {
        ColNumber = RowData.size();
    }

    this->Resize(SelfSize.RowNumber + 1, ColNumber);

    return this->SetRow(SelfSize.RowNumber, RowData);
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool  DenseMatrix<ElementType>::AppendRow(const std::vector<ElementType_Input>& RowData)
{
    if (m_IsSizeFixed == true)
    {
        MDK_Error << "Size can not change @ mdkDenseMatrix::AppendRow(const std::vector<ElementType_Input>& RowData)" << '\n';
        return false;
    }

    if (RowData.size() <= 0)
    {
        MDK_Error << "Input is empty @ mdkDenseMatrix::AppendRow(const std::vector<ElementType_Input>& RowData)" << '\n';
        return false;
    }

    auto SelfSize = this->GetSize();

    auto  ColNumber = SelfSize.ColNumber;

    if (ColNumber <= 0)
    {
        ColNumber = RowData.size();
    }

    this->Resize(SelfSize.RowNumber + 1, ColNumber);

    return this->SetRow(SelfSize.RowNumber, RowData);
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool DenseMatrix<ElementType>::AppendRow(const ElementType_Input* RowData, int64 Length)
{
    if (m_IsSizeFixed == true)
    {
        MDK_Error << "Size can not change @ mdkDenseMatrix::AppendRow(const ElementType_Input* RowData, int64 Length)" << '\n';
        return false;
    }

    if (RowData == nullptr || Length <= 0)
    {
        MDK_Error << "Input is empty @ mdkDenseMatrix::AppendRow(const ElementType_Input* RowData, int64 Length)" << '\n';
        return false;
    }

    auto SelfSize = this->GetSize();

    auto  ColNumber = SelfSize.ColNumber;

    if (ColNumber <= 0)
    {
        ColNumber = RowData.size();
    }

    this->Resize(SelfSize.RowNumber + 1, ColNumber);

    return this->SetRow(SelfSize.RowNumber, RowData, Length);
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::DeleteRow(int64 RowIndex)
{
    if (m_IsSizeFixed == true)
    {
        MDK_Error << "Size can not change @ mdkDenseMatrix::DeleteRow(int64 RowIndex)" << '\n';
        return false;
    }

    auto SelfSize = this->GetSize();

    if (RowIndex >= SelfSize.RowNumber || RowIndex < 0)
    {
        MDK_Error << "Invalid Input @ mdkDenseMatrix::DeleteRow(int64 RowIndex)" << '\n';
        return false;
    }

    return this->DeleteRow(&RowIndex, 1);
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::DeleteRow(const std::vector<int64>& RowIndexList)
{
    if (m_IsSizeFixed == true)
    {
        MDK_Error << "Size can not change @ mdkDenseMatrix::DeleteRow(const std::vector<int64>& RowIndexList)" << '\n';
        return false;
    }

    auto SelfSize = this->GetSize();

    for (auto it = RowIndexList.begin(); it != RowIndexList.end(); ++it)
    {
        if (*it >= SelfSize.RowNumber || *it < 0)
        {
            MDK_Error << "Invalid Input @ mdkDenseMatrix::DeleteRow(const std::vector<int64>& RowIndexList)" << '\n';
            return false;
        }
    }

    return this->DeleteRow(RowIndexList.data(), RowIndexList.size());
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::DeleteRow(const int64* RowIndexListPtr, int64 Length)
{
    if (m_IsSizeFixed == true)
    {
        MDK_Error << "Size can not change @ mdkDenseMatrix::DeleteRow(const int64* RowIndexPtr, int64 Length)" << '\n';
        return false;
    }

    if (RowIndexListPtr == nullptr || Length <= 0)
    {
        MDK_Error << "Invalid Input @ mdkDenseMatrix::DeleteRow(const int64* RowIndexPtr, int64 Length)" << '\n';
        return false;
    }

    auto SelfSize = this->GetSize();

    std::vector<int64> CounterList(SelfSize.RowNumber);
    
    for (int64 i = 0; i < SelfSize.RowNumber; ++i)
    {
        CounterList[i] = 0;
    }

    for (int64 i = 0; i < Length; ++i)
    {
        CounterList[RowIndexListPtr[i]] = 1;
    }

    std::vector<int64> RowIndexList_output;

    for (int64 i = 0; i < SelfSize.RowNumber; ++i)
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
bool DenseMatrix<ElementType>::InsertRow(int64 RowIndex, const DenseMatrix<ElementType_Input>& RowData)
{
    if (m_IsSizeFixed == true)
    {
        MDK_Error << "Size can not change @ mdkDenseMatrix::InsertRow(int64 RowIndex, const mdkDenseMatrix<ElementType_Input>& RowData)" << '\n';
        return false;
    }

    if (RowIndex >= SelfSize.RowNumber || RowIndex < 0)
    {
        MDK_Error << "Invalid Input @ mdkDenseMatrix InsertRow(int64 RowIndex, const mdkDenseMatrix<ElementType_Input>& RowData)" << '\n';
        return false;
    }

    auto InputSize = RowData.GetSize();

    auto SelfSize = this->GetSize();

    if (InputSize.ColNumber != 1 || InputSize.RowNumber != 1)
    {
        MDK_Error << "Invalid Input @ mdkDenseMatrix InsertRow(RowIndex,mdkDenseMatrix)" << '\n';
        return false;
    }

    if (InputSize.RowNumber > 1 && InputSize.RowNumber != SelfSize.ColNumber)
    {
        MDK_Error << "Invalid Input @ mdkDenseMatrix InsertRow(RowIndex,mdkDenseMatrix)" << '\n';
        return false;
    }

    if (InputSize.ColNumber > 1 && InputSize.ColNumber != SelfSize.ColNumber)
    {
        MDK_Error << "Invalid Input @ mdkDenseMatrix InsertRow(RowIndex,mdkDenseMatrix)" << '\n';
        return false;
    }

    return this->InsertRow(RowIndex, RowData.GetElementPointer(), std::max(Size.RowNumber, Size.ColNumber));
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool DenseMatrix<ElementType>::InsertRow(int64 RowIndex, const std::initializer_list<ElementType_Input>& RowData)
{
    return this->InsertRow(RowIndex, RowData.begin(), RowData.size());
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool DenseMatrix<ElementType>::InsertRow(int64 RowIndex, const std::vector<ElementType_Input>& RowData)
{
    return this->InsertRow(RowIndex, RowData.data(), RowData.size());
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool DenseMatrix<ElementType>::InsertRow(int64 RowIndex, const ElementType_Input* RowData, int64 Length)
{
    if (m_IsSizeFixed == true)
    {
        MDK_Error << "Size can not change @ mdkDenseMatrix::InsertRow(RowIndex, const ElementType_Input* RowData, int64 Length)" << '\n';
        return false;
    }

    auto SelfSize = this->GetSize();

    if (RowData == nullptr || Length <= 0 || RowIndex >= SelfSize.RowNumber || RowIndex < 0)
    {
        MDK_Error << "Empty Input @ mdkDenseMatrix::InsertRow(RowIndex, const ElementType_Input* RowData, int64 Length)" << '\n';
        return false;
    }

    if (Length != SelfSize.ColNumber)
    {
        MDK_Error << "Length does not match @ mdkDenseMatrix::InsertRow(RowIndex, const ElementType_Input* RowData, int64 Length)" << '\n';
        return false;
    }

    DenseMatrix<ElementType> tempMatrix(SelfSize.RowNumber + 1, SelfSize.ColNumber);

    auto tempRawPointer = tempMatrix.GetElementPointer();

    auto RawPointer = this->GetElementPointer();

    for (int64 i = 0; i < RowIndex; ++i)
    {
        for (int64 j = 0; j < SelfSize.ColNumber; ++j)
        {
            tempRawPointer[j*(SelfSize.RowNumber + 1) + i] = RawPointer[j*SelfSize.RowNumber + i];
        }
    }

    for (int64 j = 0; j < SelfSize.ColNumber; ++j)
    {
        tempRawPointer[j*(SelfSize.RowNumber + 1) + RowIndex] = ElementType(RowData[j]);
    }

    for (int64 i = RowIndex + 1; i < SelfSize.RowNumber + 1; ++i)
    {
        for (int64 j = 0; j < SelfSize.ColNumber; ++j)
        {
            tempRawPointer[j*(SelfSize.RowNumber + 1) + i] = RawPointer[j*SelfSize.RowNumber + i - 1];
        }
    }

    //---------------------------------------------------

    this->Take(tempMatrix);

    //-------------------------------------------------------

    return true;
}


template<typename ElementType>
inline
DenseShadowMatrix<ElementType>
DenseMatrix<ElementType>::Diangonal()
{
    auto SelfSize = this->GetSize();

    if (SelfSize.RowNumber <= 0 || SelfSize.RowNumber != SelfSize.ColNumber)
    {
        MDK_Error << " Self is empty or not square @ mdkDenseMatrix::Diangonal" << '\n';
        DenseShadowMatrix<ElementType> tempShadowMatrix;     
        return  tempShadowMatrix;
    }

    std::vector<int64> LinearIndexList(SelfSize.ColNumber);

    int64 Index = 0;

    for (int64 i = 0; i < SelfSize.RowNumber; ++i)
    {
        LinearIndexList[j] = Index + i;

        Index += SelfSize.RowNumber;
    }

    DenseShadowMatrix<ElementType> tempShadowMatrix(*this, LinearIndexList);

    return tempShadowMatrix;
}


template<typename ElementType>
inline
DenseMatrix<ElementType> DenseMatrix<ElementType>::GetDiangonal() const
{
    DenseMatrix<ElementType> tempMatrix; 

    auto SelfSize = this->GetSize();

    if (SelfSize.RowNumber <= 0 || SelfSize.RowNumber != SelfSize.ColNumber)
    {
        MDK_Error << " Self is empty or not square @ mdkDenseMatrix::GetDiangonal()" << '\n';
        
        return  tempMatrix;
    }

    tempMatrix.Resize(SelfSize.RowNumber, 1);

    auto tempRawPointer = tempMatrix.GetElementPointer();

    this->GetDiangonal(tempRawPointer);

    return tempMatrix;
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::GetDiangonal(std::vector<ElementType>& DiangonalData) const
{
    auto SelfSize = this->GetSize();

    if (SelfSize.RowNumber <= 0 || SelfSize.RowNumber != SelfSize.ColNumber)
    {
        MDK_Error << " Self is empty or not square @ mdkDenseMatrix::GetDiangonal(std::vector)" << '\n';
        return  false;
    }

    DiangonalData.resize(SelfSize.RowNumber);

    auto outputRawPointer = DiangonalData.data();

    return this->GetDiangonal(outputRawPointer);
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::GetDiangonal(ElementType* DiangonalData) const
{
    if (DiangonalData == nullptr)
    {
        MDK_Error << " Input is nullptr @ mdkDenseMatrix::GetDiangonal(ElementType* DiangonalData)" << '\n';
        return  false;
    }

    auto SelfSize = this->GetSize();

    if (SelfSize.RowNumber <= 0 || SelfSize.RowNumber != SelfSize.ColNumber)
    {
        MDK_Error << " Self is empty or not square @ mdkDenseMatrix::GetDiangonal(ElementType* DiangonalData)" << '\n';
        return  false;
    }

    auto RawPointer this->GetElementPointer();

    int64 Index = 0;

    for (int64 i = 0; i < SelfSize.RowNumber; ++i)
    {
        DiangonalData[j] = RawPointer[Index + i];

        Index += SelfSize.RowNumber;
    }

    return true;
}


template<typename ElementType>
template<typename ElementType_Input>
inline 
bool DenseMatrix<ElementType>::SetDiangonal(const DenseMatrix<ElementType_Input>& DiangonalData)
{
	auto InputSize = DiangonalData.GetSize();

    if (InputSize.ColNumber != 1 || InputSize.RowNumber != 1)
	{
		MDK_Error << "Invalid Input @ mdkDenseMatrix::SetDiangonal(Matrix)" << '\n';
		return false;
	}

    auto SelfSize = this->GetSize();

    if (InputSize.RowNumber > 1 && InputSize.RowNumber != SelfSize.ColNumber)
	{
		MDK_Error << "Invalid Input @ mdkDenseMatrix::SetDiangonal(Matrix)" << '\n';
		return false;
	}

    if (InputSize.ColNumber > 1 && InputSize.ColNumber != SelfSize.ColNumber)
	{
		MDK_Error << "Invalid Input @ mdkDenseMatrix::SetDiangonal(Matrix)" << '\n';
		return false;
	}

    if (InputSize.RowNumber == 1 && InputSize.ColNumber == 1)
    {
        MDK_Warning << "Input matrix is 1x1 and treated as a scalar @ mdkDenseMatrix::SetDiangonal(Matrix)" << '\n';
        return this->FillDiangonal(ElementType(DiangonalData(0)));
    }
    else
    {
        return this->SetDiangonal(DiangonalData.GetElementPointer());
    }
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool DenseMatrix<ElementType>::SetDiangonal(const std::initializer_list<ElementType_Input>& DiangonalData)
{
    auto SelfSize = this->GetSize();

    if (SelfSize.RowNumber <= 0 || SelfSize.RowNumber != SelfSize.ColNumber)
    {
        MDK_Error << "Self is empty or not square @ mdkDenseMatrix::SetDiangonal(std::initializer_list)" << '\n';
        return false;
    }

    if (DiangonalData.size() != SelfSize.RowNumber)
    {
        MDK_Error << "Invalid Input @ mdkDenseMatrix::SetDiangonal(std::initializer_list)" << '\n';
        return false;
    }

    return this->SetDiangonal(DiangonalData.begin());
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool DenseMatrix<ElementType>::SetDiangonal(const std::vector<ElementType_Input>& DiangonalData)
{
    auto SelfSize = this->GetSize();

    if (SelfSize.RowNumber <= 0 || SelfSize.RowNumber != SelfSize.ColNumber)
	{
		MDK_Error << " Self is empty or not square @ mdkDenseMatrix::SetDiangonal(std::vector)" << '\n';
		return false;
	}

    if (DiangonalData.size() != SelfSize.RowNumber)
    {
        MDK_Error << " Input is invalid @ mdkDenseMatrix::SetDiangonal(std::vector)" << '\n';
        return false;
    }

	return this->SetDiangonal(DiangonalData.data());
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool DenseMatrix<ElementType>::SetDiangonal(const ElementType_Input* DiangonalData)
{
    if (DiangonalData == nullptr)
    {
        MDK_Error << " Input is nullptr @ mdkDenseMatrix::SetDiangonal(const ElementType_Input* DiangonalData)" << '\n';
        return false;
    }

    auto SelfSize = this->GetSize();

    if (SelfSize.RowNumber <= 0 || SelfSize.RowNumber != SelfSize.ColNumber)
    {
        MDK_Error << " Self is empty or not square @ mdkDenseMatrix::SetDiangonal(const ElementType_Input* DiangonalData)" << '\n';
        return false;
    }

    auto RawPointer = this->GetElementPointer();

    int64 Index = 0;

    for (int64 i = 0; i < SelfSize.RowNumber; ++i)
    {
        RawPointer[Index + i] = ElementType(DiangonalData[i]);

        Index += SelfSize.RowNumber;
    }

    return true;
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::FillDiangonal(const ElementType& Element)
{
    auto SelfSize = this->GetSize();

    if (SelfSize.RowNumber <= 0 || SelfSize.RowNumber != SelfSize.ColNumber)
    {
        MDK_Error << " Self is empty or not square @ mdkDenseMatrix::FillDiangonal" << '\n';
        return false;
    }

    auto RawPointer = this->GetElementPointer();

    int64 Index = 0;

    for (int64 i = 0; i < SelfSize.RowNumber; ++i)
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
        MDK_Error << "ShadowMatrix is empty @ mdkDenseMatrix::operator+=(ShadowMatrix)" << '\n';
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
        MDK_Error << "Size does not match @ mdkDenseMatrix::operator+=(ShadowMatrix)" << '\n';
        return;
    }

    auto RawPointer this->GetElementPointer();

    for (int64 i = 0; i < Self_ElementNumber; ++i)
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
        MDK_Error << "ShadowMatrix is empty @ mdkDenseMatrix::operator-=(ShadowMatrix)" << '\n';
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
        MDK_Error << "Size does not match @ mdkDenseMatrix::operator-=(ShadowMatrix)" << '\n';
        return;
    }

    auto RawPointer this->GetElementPointer();

    for (int64 i = 0; i < Self_ElementNumber; ++i)
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
        MDK_Error << "ShadowMatrix is empty @ mdkDenseMatrix::operator/=(ShadowMatrix)" << '\n';
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
        MDK_Error << "Size does not match @ mdkDenseMatrix::operator/=(ShadowMatrix)" << '\n';
        return;
    }

    auto RawPointer this->GetElementPointer();

    for (int64 i = 0; i < Self_ElementNumber; ++i)
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
    DenseMatrix<ElementType> tempMatrix;

    auto SelfSize = this->GetSize();

    if (SelfSize.RowNumber <= 0)
    {
        MDK_Error << "Self is empty @ mdkDenseMatrix::operator^(value)" << '\n';

        return tempMatrix;
    }

    tempMatrix.Resize(SelfSize.RowNumber, SelfSize.ColNumber);

    auto tempRawPointer = tempMatrix.GetElementPointer();

    auto RawPointer = this->GetElementPointer();

    auto Self_ElementNumber = SelfSize.RowNumber*SelfSize.ColNumber;

    for (int64 i = 0; i < Self_ElementNumber; ++i)
    {
        tempRawPointer[i] = std::pow(RawPointer[i], Element);
    }

    return tempMatrix;
}

//-------------------- special element operation {^=} -----------------------------------------------------------//

template<typename ElementType>
inline void DenseMatrix<ElementType>::operator^=(const ElementType& Element)
{
    auto SelfSize = this->GetSize();

    if (SelfSize.RowNumber <= 0)
    {
        MDK_Error << "Self is empty @ mdkDenseMatrix::operator^(value)" << '\n';
        return;
    }

    auto BeginPointer this->GetElementPointer();

    auto Self_ElementNumber = SelfSize.RowNumber*SelfSize.ColNumber;

    for (auto Ptr = BeginPointer; Ptr < BeginPointer + Self_ElementNumber; ++Ptr)
    {
        Ptr[0] = std::pow(Ptr[0], Element);
    }
}


//-------------------- special element operation ( .* in matlab ) ------------------------------------------//

template<typename ElementType>
inline 
DenseMatrix<ElementType> DenseMatrix<ElementType>::ElementMultiply(const DenseMatrix<ElementType>& InputMatrix) const
{
    return MatrixElementMultiply(*this, InputMatrix);

}


template<typename ElementType>
inline
DenseMatrix<ElementType> DenseMatrix<ElementType>::ElementMultiply(const ElementType& Element) const
{
    return MatrixElementMultiply(*this, Element);
}


template<typename ElementType>
inline
DenseMatrix<ElementType> DenseMatrix<ElementType>::ElementMultiply(const DenseShadowMatrix<ElementType>& ShadowMatrix) const
{
    return MatrixElementMultiply(*this, ShadowMatrix.CreateDenseMatrix());
}


template<typename ElementType>
inline
DenseMatrix<ElementType> DenseMatrix<ElementType>::ElementMultiply(const DenseGlueMatrixForLinearCombination<ElementType>& GlueMatrix) const
{
    return MatrixElementMultiply(*this, GlueMatrix.CreateDenseMatrix());
}


template<typename ElementType>
inline
DenseMatrix<ElementType> DenseMatrix<ElementType>::ElementMultiply(const DenseGlueMatrixForMultiplication<ElementType>& GlueMatrix) const
{
    return MatrixElementMultiply(*this, GlueMatrix.CreateDenseMatrix());
}


//-------------------- general element operation : output a new matrix ------------------------------------------//

template<typename ElementType>
inline 
DenseMatrix<ElementType> DenseMatrix<ElementType>::ElementNamedOperation(const std::string& OperationName) const
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
DenseMatrix<ElementType> DenseMatrix<ElementType>::ElementNamedOperation(const char OperationName, const DenseMatrix<ElementType>& InputMatrix) const
{
    return MatrixElementNamedOperation(OperationName, *this, InputMatrix);
}


template<typename ElementType>
inline
DenseMatrix<ElementType> DenseMatrix<ElementType>::ElementNamedOperation(const std::string& OperationName, const DenseMatrix<ElementType>& InputMatrix) const
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
DenseMatrix<ElementType> DenseMatrix<ElementType>::ElementNamedOperation(const char OperationName, const ElementType& Element) const
{
    return MatrixElementNamedOperation(OperationName, *this, Element);
}


template<typename ElementType>
inline
DenseMatrix<ElementType> DenseMatrix<ElementType>::ElementNamedOperation(const std::string& OperationName, const ElementType& Element) const
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
bool DenseMatrix<ElementType>::ElementNamedOperationInPlace(const std::string& OperationName)
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
bool DenseMatrix<ElementType>::ElementNamedOperationInPlace(const char OperationName, const DenseMatrix<ElementType>& InputMatrix)
{
    return MatrixNamedElementOperation(*this, OperationName, *this, InputMatrix);
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::ElementNamedOperationInPlace(const std::string& OperationName, const DenseMatrix<ElementType>& InputMatrix)
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
bool DenseMatrix<ElementType>::ElementNamedOperationInPlace(const char OperationName, const ElementType& Element)
{
    return MatrixElementNamedOperation(*this, OperationName, *this, Element);
}


template<typename ElementType>
inline
bool DenseMatrix<ElementType>::ElementNamedOperationInPlace(const std::string& OperationName, const ElementType& Element)
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
DenseMatrix<ElementType>::ColNamedOperation(int64 ColIndex, const std::string& OperationName, bool EnableBoundCheck = true) const
{    
    return MatrixColNamedOperation(OperationName, *this, ColIndex, EnableBoundCheck);
}


template<typename ElementType>
template<typename OperationType>
inline
DenseMatrix<ElementType>
DenseMatrix<ElementType>::ColOperation(int64 ColIndex, OperationType Operation, bool EnableBoundCheck = true) const
{
    return MatrixColOperation(OperationName, *this, ColIndex, EnableBoundCheck);
}


template<typename ElementType>
inline
DenseMatrix<ElementType>
DenseMatrix<ElementType>::ColNamedOperation(int64 ColIndex, const char OperationName, const DenseMatrix<ElementType>& InputMatrix, bool EnableBoundCheck = true) const
{
    return MatrixColNamedOperation(OperationName, *this, ColIndex, InputMatrix, EnableBoundCheck);
}


template<typename ElementType>
inline 
DenseMatrix<ElementType>
DenseMatrix<ElementType>::ColNamedOperation(int64 ColIndex, const std::string& OperationName, const DenseMatrix<ElementType>& InputMatrix, bool EnableBoundCheck = true) const
{
    return MatrixColNamedOperation(OperationName, *this, ColIndex, InputMatrix, EnableBoundCheck);
}


template<typename ElementType>
template<typename OperationType>
inline
DenseMatrix<ElementType> 
DenseMatrix<ElementType>::ColOperation(int64 ColIndex, OperationType Operation, const DenseMatrix<ElementType>& InputMatrix, bool EnableBoundCheck = true) const
{
    return MatrixColOperation(Operation, *this, ColIndex, InputMatrix, EnableBoundCheck);
}


template<typename ElementType>
inline 
DenseMatrix<ElementType> 
DenseMatrix<ElementType>::ColNamedOperation(int64 ColIndex, const char OperationName, const ElementType& Element, bool EnableBoundCheck = true) const
{
    return MatrixColNamedOperation(OperationName, *this, ColIndex, Element, EnableBoundCheck);
}


template<typename ElementType>
inline
DenseMatrix<ElementType> 
DenseMatrix<ElementType>::ColNamedOperation(int64 ColIndex, const std::string& OperationName, const ElementType& Element, bool EnableBoundCheck = true) const
{
    return MatrixColNamedOperation(OperationName, *this, ColIndex, Element, EnableBoundCheck);
}


template<typename ElementType>
template<typename OperationType>
inline 
DenseMatrix<ElementType>
DenseMatrix<ElementType>::ColOperation(int64 ColIndex, OperationType Operation, const ElementType& Element, bool EnableBoundCheck = true) const
{
    return MatrixColOperation(Operation, *this, ColIndex, Element, EnableBoundCheck);
}

//-------------------- general col operation in place : Object.ColOperationInPlace modify the object itself ---------------//

template<typename ElementType>
inline 
bool
DenseMatrix<ElementType>::ColNamedOperationInPlace(int64 ColIndex, const std::string& OperationName, bool EnableBoundCheck = true)
{
    return MatrixColNamedOperation(*this, ColIndex, OperationName, *this, ColIndex, EnableBoundCheck);
}


template<typename ElementType>
template<typename OperationType>
inline
bool
DenseMatrix<ElementType>::ColOperationInPlace(int64 ColIndex, OperationType Operation, bool EnableBoundCheck = true)
{
    return MatrixColOperation(*this, ColIndex, Operation, *this, ColIndex, EnableBoundCheck);
}


template<typename ElementType>
inline 
bool
DenseMatrix<ElementType>::ColNamedOperationInPlace(int64 ColIndex, const char OperationName, const DenseMatrix<ElementType>& InputMatrix, bool EnableBoundCheck = true)
{
    return MatrixColNamedOperation(*this, ColIndex, OperationName, *this, ColIndex, InputMatrix, EnableBoundCheck);
}


template<typename ElementType>
inline
bool
DenseMatrix<ElementType>::ColNamedOperationInPlace(int64 ColIndex, const std::string& OperationName, const DenseMatrix<ElementType>& InputMatrix, bool EnableBoundCheck = true)
{
    return MatrixColNamedOperation(*this, ColIndex, OperationName, *this, ColIndex, InputMatrix, EnableBoundCheck);
}


template<typename ElementType>
template<typename OperationType>
inline
bool 
DenseMatrix<ElementType>::ColOperationInPlace(int64 ColIndex, OperationType Operation, const DenseMatrix<ElementType>& InputMatrix, bool EnableBoundCheck = true)
{
    return MatrixColOperation(*this, ColIndex, Operation, *this, ColIndex, InputMatrix, EnableBoundCheck);
}


template<typename ElementType>
inline 
bool
DenseMatrix<ElementType>::ColNamedOperationInPlace(int64 ColIndex, const char OperationName, const ElementType& Element, bool EnableBoundCheck = true)
{
    return MatrixColNamedOperation(*this, ColIndex, OperationName, *this, ColIndex, Element, EnableBoundCheck);
}


template<typename ElementType>
inline
bool 
DenseMatrix<ElementType>::ColNamedOperationInPlace(int64 ColIndex, const std::string& OperationName, const ElementType& Element, bool EnableBoundCheck = true)
{
    return MatrixColNamedOperation(*this, ColIndex, OperationName, *this, ColIndex, Element, EnableBoundCheck);
}


template<typename ElementType>
template<typename OperationType>
inline
bool
DenseMatrix<ElementType>::ColOperationInPlace(int64 ColIndex, OperationType Operation, const ElementType& Element, bool EnableBoundCheck = true)
{
    return MatrixColOperation(*this, ColIndex, Operation, *this, ColIndex, Element, EnableBoundCheck);
}

//-------------------- general Row operation : output a new row-matrix ------------------------------------------//

template<typename ElementType>
inline 
DenseMatrix<ElementType>
DenseMatrix<ElementType>::RowNamedOperation(int64 RowIndex, const std::string& OperationName, bool EnableBoundCheck = true) const
{
    return MatrixRowNamedOperation(OperationName, *this, RowIndex, EnableBoundCheck);
}


template<typename ElementType>
template<typename OperationType>
inline 
DenseMatrix<ElementType> 
DenseMatrix<ElementType>::RowOperation(int64 RowIndex, OperationType Operation, bool EnableBoundCheck = true) const
{
    return MatrixRowOperation(Operation, *this, RowIndex, EnableBoundCheck);
}


template<typename ElementType>
inline 
DenseMatrix<ElementType>
DenseMatrix<ElementType>::RowNamedOperation(int64 RowIndex, const char OperationName, const DenseMatrix<ElementType>& InputMatrix, bool EnableBoundCheck = true) const
{
    return MatrixRowNamedOperation(OperationName, *this, RowIndex, InputMatrix, EnableBoundCheck);
}


template<typename ElementType>
inline 
DenseMatrix<ElementType>
DenseMatrix<ElementType>::RowNamedOperation(int64 RowIndex, const std::string& OperationName, const DenseMatrix<ElementType>& InputMatrix, bool EnableBoundCheck = true) const
{
    return MatrixRowNamedOperation(OperationName, *this, RowIndex, InputMatrix, EnableBoundCheck);
}


template<typename ElementType>
template<typename OperationType>
inline 
DenseMatrix<ElementType> 
DenseMatrix<ElementType>::RowOperation(int64 RowIndex, OperationType Operation, const DenseMatrix<ElementType>& InputMatrix, bool EnableBoundCheck = true) const
{
    return MatrixRowOperation(Operation, *this, RowIndex, InputMatrix, EnableBoundCheck);
}


template<typename ElementType>
inline 
DenseMatrix<ElementType> 
DenseMatrix<ElementType>::RowNamedOperation(int64 RowIndex, const char OperationName, const ElementType& Element, bool EnableBoundCheck = true) const
{
    return MatrixRowNamedOperation(OperationName, *this, RowIndex, Element, EnableBoundCheck);
}


template<typename ElementType>
inline 
DenseMatrix<ElementType> 
DenseMatrix<ElementType>::RowNamedOperation(int64 RowIndex, const std::string& OperationName, const ElementType& Element, bool EnableBoundCheck = true) const
{
    return MatrixRowNamedOperation(OperationName, *this, RowIndex, Element, EnableBoundCheck);
}


template<typename ElementType>
template<typename OperationType>
inline
DenseMatrix<ElementType> 
DenseMatrix<ElementType>::RowOperation(int64 RowIndex, OperationType Operation, const ElementType& Element, bool EnableBoundCheck = true) const
{
    return MatrixRowOperation(Operation, *this, RowIndex, Element, EnableBoundCheck);
}

//-------------------- general row operation in place : Object.RowOperationInPlace modify the object itself ---------------//

template<typename ElementType>
inline 
bool
DenseMatrix<ElementType>::RowNamedOperationInPlace(int64 RowIndex, const std::string& OperationName, bool EnableBoundCheck = true)
{
    return MatrixRowNamedOperation(*this, RowIndex, OperationName, *this, RowIndex, EnableBoundCheck);
}


template<typename ElementType>
template<typename OperationType>
inline 
bool
DenseMatrix<ElementType>::RowOperationInPlace(int64 RowIndex, OperationType Operation, bool EnableBoundCheck = true)
{
    return MatrixRowOperation(*this, RowIndex, Operation, *this, RowIndex, EnableBoundCheck);
}


template<typename ElementType>
inline 
bool
DenseMatrix<ElementType>::RowNamedOperationInPlace(int64 RowIndex, const char OperationName, const DenseMatrix<ElementType>& InputMatrix, bool EnableBoundCheck = true)
{
    return MatrixRowNamedOperation(*this, RowIndex, OperationName, *this, RowIndex, InputMatrix, EnableBoundCheck);
}


template<typename ElementType>
inline 
bool 
DenseMatrix<ElementType>::RowNamedOperationInPlace(int64 RowIndex, const std::string& OperationName, const DenseMatrix<ElementType>& InputMatrix, bool EnableBoundCheck = true)
{
    return MatrixRowNamedOperation(*this, RowIndex, OperationName, *this, RowIndex, InputMatrix, EnableBoundCheck);
}


template<typename ElementType>
template<typename OperationType>
inline 
bool
DenseMatrix<ElementType>::RowOperationInPlace(int64 RowIndex, OperationType Operation, const DenseMatrix<ElementType>& InputMatrix, bool EnableBoundCheck = true)
{
    return MatrixRowOperation(*this, RowIndex, Operation, *this, RowIndex, InputMatrix, EnableBoundCheck);
}


template<typename ElementType>
inline 
bool
DenseMatrix<ElementType>::RowNamedOperationInPlace(int64 RowIndex, const char OperationName, const ElementType& Element, bool EnableBoundCheck = true)
{
    return MatrixRowNamedOperation(*this, RowIndex, OperationName, *this, RowIndex, Element, EnableBoundCheck);
}


template<typename ElementType>
inline 
bool
DenseMatrix<ElementType>::RowNamedOperationInPlace(int64 RowIndex, const std::string& OperationName, const ElementType& Element, bool EnableBoundCheck = true)
{
    return MatrixRowNamedOperation(*this, RowIndex, OperationName, *this, RowIndex, Element, EnableBoundCheck);
}


template<typename ElementType>
template<typename OperationType>
inline
bool
DenseMatrix<ElementType>::RowOperationInPlace(int64 RowIndex, OperationType Operation, const ElementType& Element, bool EnableBoundCheck = true)
{
    return MatrixRowOperation(*this, RowIndex, Operation, *this, RowIndex, Element, EnableBoundCheck);
}

//-----------------------------------------------------------------------------------------------------------//

template<typename ElementType>
inline ElementType DenseMatrix<ElementType>::Mean() const
{
    auto Self_ElementNumber = this->GetElementNumber();

    if (Self_ElementNumber <= 0)
    {
        MDK_Error << "self is empty Matrix @ mdkDenseMatrix::Mean" << '\n';
        return m_NaNElement;
    }

    auto RawPointer = this->GetElementPointer();

    ElementType value = RawPointer[0];

    for (auto Ptr = RawPointer + 1; Ptr < RawPointer + Self_ElementNumber; ++Ptr)
    {
        value += Ptr[0];
    }

    value /= Self_ElementNumber;

    return value;
}


template<typename ElementType>
inline DenseMatrix<ElementType> DenseMatrix<ElementType>::MeanToRow() const
{
	DenseMatrix<ElementType> tempMatrix;

    auto SelfSize = this->GetSize();

    if (SelfSize.RowNumber <= 0)
	{
		MDK_Error << "self is empty Matrix @ mdkDenseMatrix::MeanToRow" << '\n';

		return tempMatrix;
	}

	tempMatrix.Resize(1, SelfSize.ColNumber);

	auto tempRawPointer = tempMatrix.GetElementPointer();

	auto RawPointer = this->GetElementPointer();

	for (int64 j = 0; j < SelfSize.ColNumber; ++j)
	{
        auto value = RawPointer[0];

        ++RawPointer;

		for (int64 i = 1; i < SelfSize.RowNumber; ++i)
		{            
			value += RawPointer[0];      

            ++RawPointer;
		}

        tempRawPointer[j] = value / SelfSize.RowNumber;
	}
    
	return tempMatrix;
}


template<typename ElementType>
inline DenseMatrix<ElementType> DenseMatrix<ElementType>::MeanToCol() const
{
	DenseMatrix<ElementType> tempMatrix;

    auto SelfSize = this->GetSize();

    if (SelfSize.RowNumber <= 0)
	{
		MDK_Error << "self is empty Matrix @ mdkDenseMatrix::MeanToCol" << '\n';
  
		return tempMatrix;
	}

	tempMatrix.Resize(SelfSize.ColNumber, 1);

	auto tempRawPointer = tempMatrix.GetElementPointer();

	auto RawPointer = this->GetElementPointer();

	for (int64 i = 0; i < SelfSize.RowNumber; ++i)
	{
        auto value = RawPointer[0];

        int64 Index = SelfSize.RowNumber;

		for (int64 j = 1; j < SelfSize.ColNumber; ++j)
		{
			value += RawPointer[Index + i];

			Index += SelfSize.RowNumber;
		}

        tempRawPointer[i] = value / SelfSize.ColNumber;
	}

	return tempMatrix;
}


template<typename ElementType>
inline ElementType DenseMatrix<ElementType>::Sum() const
{
    auto Self_ElementNumber = this->GetElementNumber();

    if (Self_ElementNumber <= 0)
	{
		MDK_Error << "self is empty Matrix @ mdkDenseMatrix::Sum" << '\n';
        return m_NaNElement;
	}

    auto RawPointer = this->GetElementPointer();

	ElementType value = RawPointer[0];

    for (auto Ptr = RawPointer + 1; Ptr < RawPointer + Self_ElementNumber; ++Ptr)
    {
        value += Ptr[0];
    }

	return value;
}


template<typename ElementType>
inline DenseMatrix<ElementType> DenseMatrix<ElementType>::SumToRow() const
{
	DenseMatrix<ElementType> tempMatrix;

    auto SelfSize = this->GetSize();

    if (SelfSize.RowNumber <= 0)
	{
		MDK_Error << "self is empty Matrix @ mdkDenseMatrix::SumToRow" << '\n';
        
		return tempMatrix;
	}

	tempMatrix.Resize(1, SelfSize.ColNumber);

	auto tempRawPointer = tempMatrix.GetElementPointer();

	auto RawPointer = this->GetElementPointer();

	for (int64 j = 0; j < SelfSize.ColNumber; ++j)
	{
        auto value = RawPointer[0];

        ++RawPointer;

		for (int64 i = 1; i < SelfSize.RowNumber; ++i)
		{
			value += RawPointer[0];

            ++RawPointer;
		}

		tempRawPointer[j] = value;
	}
    
	return tempMatrix;
}


template<typename ElementType>
inline DenseMatrix<ElementType> DenseMatrix<ElementType>::SumToCol() const
{
	DenseMatrix<ElementType> tempMatrix;

    auto SelfSize = this->GetSize();

    if (SelfSize.RowNumber <= 0)
	{
		MDK_Error << "self is empty Matrix @ mdkDenseMatrix::SumToCol" << '\n';
        
		return tempMatrix;
	}

	tempMatrix.Resize(SelfSize.RowNumber, 1);

	auto tempRawPointer = tempMatrix.GetElementPointer();

	auto RawPointer = this->GetElementPointer();

	for (int64 i = 0; i < SelfSize.RowNumber; ++i)
	{
        auto value = RawPointer[i];

        int64 Index = SelfSize.RowNumber;

		for (int64 j = 1; j < SelfSize.ColNumber; ++j)
		{
			value += RawPointer[Index + i];

			Index += SelfSize.RowNumber;
		}

		tempRawPointer[i] = value;
	}
    
	return tempMatrix;
}


template<typename ElementType>
inline ElementType DenseMatrix<ElementType>::Max() const
{
    auto Self_ElementNumber = this->GetElementNumber();

    if (Self_ElementNumber <= 0)
	{
		MDK_Error << "self is empty Matrix @ mdkDenseMatrix::Max" << '\n';
        return m_NaNElement;
	}

	auto RawPointer = this->GetElementPointer();

	ElementType value = RawPointer[0];

    for (auto Ptr = RawPointer + 1; Ptr < RawPointer + Self_ElementNumber; ++Ptr)
	{
        value = std::max(value, Ptr[0]);
	}

	return value;
}


template<typename ElementType>
inline DenseMatrix<ElementType> DenseMatrix<ElementType>::MaxToRow() const
{
	DenseMatrix<ElementType> tempMatrix;

    auto SelfSize = this->GetSize();

    if (SelfSize.RowNumber <= 0)
	{
		MDK_Error << "self is empty Matrix @ mdkDenseMatrix::MaxToRow" << '\n';
        
		return tempMatrix;
	}

	tempMatrix.Resize(1, SelfSize.ColNumber);

	auto tempRawPointer = tempMatrix.GetElementPointer();

	auto RawPointer = this->GetElementPointer();

	for (int64 j = 0; j < SelfSize.ColNumber; ++j)
	{
        auto value = RawPointer[0];

        ++RawPointer;

		for (int64 i = 1; i < SelfSize.RowNumber; ++i)
		{
			value = std::max(value, RawPointer[0]);

            ++ RawPointer;
		}

		tempRawPointer[j] = value;
	}
    
	return tempMatrix;
}


template<typename ElementType>
inline DenseMatrix<ElementType> DenseMatrix<ElementType>::MaxToCol() const
{
	DenseMatrix<ElementType> tempMatrix;

    auto SelfSize = this->GetSize();

    if (SelfSize.RowNumber <= 0)
	{
		MDK_Error << "self is empty Matrix @ mdkDenseMatrix::MaxToCol" << '\n';
        
		return tempMatrix;
	}

	tempMatrix.Resize(SelfSize.ColNumber, 1);

	auto tempRawPointer = tempMatrix.GetElementPointer();

	auto RawPointer = this->GetElementPointer();

	for (int64 i = 0; i < SelfSize.RowNumber; ++i)
	{
        auto value = RawPointer[i];

        auto Index = SelfSize.RowNumber;

		for (int64 j = 1; j < SelfSize.ColNumber; ++j)
		{
			value = std::max(value, RawPointer[Index + i]);

			Index += SelfSize.RowNumber;
		}

		tempRawPointer[i] = value;
	}

	return tempMatrix;
}


template<typename ElementType>
inline ElementType DenseMatrix<ElementType>::Min() const
{
    auto Self_ElementNumber = this->GetElementNumber();

    if (Self_ElementNumber <= 0)
	{
		MDK_Error << "self is empty Matrix @ mdkDenseMatrix::Min" << '\n';
        return m_NaNElement;
	}

	auto RawPointer = this->GetElementPointer();

    ElementType value = RawPointer[0];

    for (auto Ptr = RawPointer + 1; Ptr < RawPointer + Self_ElementNumber; ++Ptr)
    {
        value = std::min(value, Ptr[0]);
    }

	return value;
}


template<typename ElementType>
inline DenseMatrix<ElementType> DenseMatrix<ElementType>::MinToRow() const
{
	DenseMatrix<ElementType> tempMatrix;

    auto SelfSize = this->GetSize();

    if (SelfSize.RowNumber <= 0)
	{
		MDK_Error << "self is empty Matrix @ mdkDenseMatrix::MinToRow" << '\n';
        
		return tempMatrix;
	}

	tempMatrix.Resize(1, SelfSize.ColNumber);

	auto tempRawPointer = tempMatrix.GetElementPointer();

	auto RawPointer = this->GetElementPointer();

	for (int64 j = 0; j < SelfSize.ColNumber; ++j)
	{
		auto value = RawPointer[0];

        ++RawPointer;

		for (int64 i = 1; i < SelfSize.RowNumber; ++i)
		{
			value = std::min(value, RawPointer[0]);

            ++RawPointer;
		}

		tempRawPointer[j] = value;
	}
    
	return tempMatrix;
}


template<typename ElementType>
inline DenseMatrix<ElementType> DenseMatrix<ElementType>::MinToCol() const
{
	DenseMatrix<ElementType> tempMatrix;

    auto SelfSize = this->GetSize();

    if (SelfSize.RowNumber <= 0)
	{
		MDK_Error << "self is empty Matrix @ mdkDenseMatrix::MinToCol" << '\n';
        
		return tempMatrix;
	}

	tempMatrix.Resize(SelfSize.ColNumber, 1);

	auto tempRawPointer = tempMatrix.GetElementPointer();

	auto RawPointer = this->GetElementPointer();

	for (int64 i = 0; i < SelfSize.RowNumber; ++i)
	{
        auto value = RawPointer[i];

        auto Index = SelfSize.RowNumber;

		for (int64 j = 1; j < SelfSize.ColNumber; ++j)
		{
			value = std::min(value, RawPointer[Index + i]);

			Index += SelfSize.RowNumber;
		}

		tempRawPointer[i] = value;
	}
    
	return tempMatrix;
}


template<typename ElementType>
inline 
DenseMatrix<ElementType> DenseMatrix<ElementType>::Transpose() const
{
    return MatrixTranspose(*this);
}


template<typename ElementType>
inline
int64 DenseMatrix<ElementType>::Rank() const
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