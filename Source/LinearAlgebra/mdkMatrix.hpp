#ifndef __mdkMatrix_hpp
#define __mdkMatrix_hpp

#include <string>
#include <cmath>
#include <algorithm>

#include "mdkType.h"
#include "mdkDebugConfig.h"
#include "mdkMatrix.h"
#include "mdkShadowMatrix.h"
#include "mdkGlueMatrixForLinearCombination.h"
#include "mdkGlueMatrixForMultiplication.h"
#include "mdkMatrixOperator.h"
#include "mdkLinearAlgebra.h"

namespace mdk
{
 
template<typename ElementType>
inline
mdkMatrix<ElementType>::mdkMatrix()
{
	this->Reset();
}


template<typename ElementType>
inline
mdkMatrix<ElementType>::mdkMatrix(uint64 RowNumber, uint64 ColNumber, bool IsSizeFixed = false)
{
    this->Reset();

    this->Resize(RowNumber, ColNumber);

    m_IsSizeFixed = IsSizeFixed;

    if (IsSizeFixed == true)
    {
        m_CoreData->DataArray.shrink_to_fit();

        m_ElementPointer = m_CoreData->DataArray.data();
    }
}


template<typename ElementType>
inline
mdkMatrix<ElementType>::mdkMatrix(const mdkMatrix<ElementType>& InputMatrix, bool IsSizeFixed = false)
{
    this->Reset();

    //force-copy data
    this->Copy(InputMatrix);     

    m_IsSizeFixed = IsSizeFixed;
}


// move constructor
template<typename ElementType>
inline
mdkMatrix<ElementType>::mdkMatrix(mdkMatrix<ElementType>&& InputMatrix)
{
    m_CoreData = std::move(InputMatrix.m_CoreData);

    m_ElementPointer = m_CoreData->DataArray.data();

    m_NaNElement = InputMatrix.m_NaNElement;

    m_IsSizeFixed = InputMatrix.m_IsSizeFixed;
}


template<typename ElementType>
inline
mdkMatrix<ElementType>::mdkMatrix(const ElementType& Element)
{
    this->Reset();

    this->Resize(1, 1);

    (*this)(0) = Element;
}


template<typename ElementType>
inline
mdkMatrix<ElementType>::mdkMatrix(const mdkShadowMatrix<ElementType>& ShadowMatrix, bool IsSizeFixed = false)
{
    // not necessary to use this->Reset()

    m_IsSizeFixed = false;
    this->Take(ShadowMatrix.CreateMatrix());

    m_NaNElement = ShadowMatrix.m_NaNElement;

    m_IsSizeFixed = IsSizeFixed;
}


template<typename ElementType>
inline
mdkMatrix<ElementType>::mdkMatrix(const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrix, bool IsSizeFixed = false)
{
    // not necessary to use this->Reset()

    m_IsSizeFixed = false;
    this->Take(GlueMatrix.CreateMatrix());

    m_NaNElement = GetMatrixNaNElement(m_NaNElement);

    m_IsSizeFixed = IsSizeFixed;
}


template<typename ElementType>
inline
mdkMatrix<ElementType>::mdkMatrix(const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrix, bool IsSizeFixed = false)
{
    // not necessary to use this->Reset()

    m_IsSizeFixed = false;
    this->Take(GlueMatrix.CreateMatrix());

    m_NaNElement = GetMatrixNaNElement(m_NaNElement);

    m_IsSizeFixed = IsSizeFixed;
}


template<typename ElementType>
inline
mdkMatrix<ElementType>::mdkMatrix(const ElementType* InputElementPointer, uint64 InputRowNumber, uint64 InputColNumber, bool IsSizeFixed = false)
{
    this->Reset();

    if (InputElementPointer == nullptr)
    {
        mdkWarning << "Empty Input @ mdkMatrix::mdkMatrix(const ElementType*, uint64, uint64, bool)" << '\n';
        return;
    }

    if (InputRowNumber == 0 || InputColNumber == 0)
    {
        if (InputElementPointer != nullptr)
        {
            mdkError << "Invalid Input @ mdkMatrix::mdkMatrix(const ElementType*, uint64, uint64, bool)" << '\n';
            return;
        }
    }

    this->Copy(InputElementPointer, InputRowNumber, InputColNumber);

    m_IsSizeFixed = IsSizeFixed;
}


template<typename ElementType>
inline
mdkMatrix<ElementType>::~mdkMatrix()
{
}


template<typename ElementType>
inline
void mdkMatrix<ElementType>::operator=(const mdkMatrix<ElementType>& InputMatrix)
{
    this->Copy(InputMatrix);
}


// move assignment operator
template<typename ElementType>
inline
void mdkMatrix<ElementType>::operator=(mdkMatrix<ElementType>&& InputMatrix)
{
    this->Take(InputMatrix);
}


template<typename ElementType>
inline
void mdkMatrix<ElementType>::operator=(const ElementType& Element)
{
    auto ElementNumber = this->GetElementNumber();

    if (m_IsSizeFixed == true)
    {
        if (ElementNumber == 0)
        {
            mdkError << "Can not change matrix size @ mdkMatrix::operator=(Element)" << '\n';
            return;
        }
    }

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
        mdkWarning << "The size of the matrix changes to 1x1 @ mdkMatrix::operator=(Element)" << '\n';

        this->Clear();

        this->Resize(1, 1);

        (*this)[0] = Element;
    }

    return;
}


template<typename ElementType>
inline
void mdkMatrix<ElementType>::operator=(const std::initializer_list<ElementType>& list)
{
    auto SelfSize = this->GetSize();

    auto ElementNumber = SelfSize.RowNumber * SelfSize.ColNumber;

    //if Self is empty
    if (ElementNumber == 0)
    {
        mdkError << "operator=(list) can not be called if self is empty @ mdkMatrix::operator=(list)" << '\n';
        return;
    }

    if (ElementNumber != list.size())
    {
        mdkError << "Size does not match @ mdkMatrix::operator=(list)" << '\n';
        return;
    }

    auto RawPointer = this->GetElementPointer();

    uint64 Counter = 0;

    for (uint64 i = 0; i < SelfSize.RowNumber; ++i)
    {
        uint64 Index = 0;

        for (uint64 j = 0; j < SelfSize.ColNumber; ++j)
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
void mdkMatrix<ElementType>::operator=(const std::initializer_list<std::initializer_list<ElementType>>& list)
{
    auto SelfSize = this->GetSize();

    //if Self is empty
    if (SelfSize.RowNumber == 0)
    {
        mdkError << "operator=(list in list) can not be called if self is empty @ mdkMatrix::operator=(list in list)" << '\n';
        return;
    }

    if (SelfSize.RowNumber != list.size())
    {
        mdkError << "Row Size does not match @ mdkMatrix::operator=(list in list)" << '\n';
        return;
    }

    //check each row-list 
    for (uint64 i = 0; i < SelfSize.RowNumber; ++i)
    {
        auto subList = list.begin()[i];

        if (subList.size() != SelfSize.ColNumber)
        {
            mdkError << "Col Size does not match in row: " << i << " @ mdkMatrix::operator=(list in list)" << '\n';
            return;
        }
    }

    auto RawPointer = this->GetElementPointer();

    for (uint64 i = 0; i < SelfSize.RowNumber; ++i)
    {
        auto subList = list.begin()[i];

        uint64 Index = 0;

        for (uint64 j = 0; j < SelfSize.ColNumber; ++j)
        {
            RawPointer[Index + i] = subList.begin()[j];

            Index += SelfSize.RowNumber;
        }
    }

    return;
}


template<typename ElementType>
inline
void mdkMatrix<ElementType>::operator=(const mdkShadowMatrix<ElementType>& ShadowMatrix)
{
    this->Take(ShadowMatrix);
}


template<typename ElementType>
inline
void mdkMatrix<ElementType>::operator=(const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrix)
{
    this->Take(GlueMatrix);
}


template<typename ElementType>
inline
void mdkMatrix<ElementType>::operator=(const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrix)
{
    this->Take(GlueMatrix);
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool mdkMatrix<ElementType>::Copy(const mdkMatrix<ElementType_Input>& InputMatrix)
{
    if (this == &InputMatrix)
    {
        mdkWarning << "A Matrix tries to Copy itself @ mdkMatrix::Copy(InputMatrix)" << '\n';
        return false;
    }

    if (InputMatrix.IsEmpty() == true)
    {
        mdkWarning << "InputMatrix is empty, and this matrix is set to be empty @ mdkMatrix::Copy(InputMatrix)" << '\n';

        this->Clear();

        return true;
    }

    // copy data
    return this->Copy(InputMatrix.GetElementPointer(), InputMatrix.GetRowNumber(), InputMatrix.GetColNumber());
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool mdkMatrix<ElementType>::Copy(const ElementType_Input* InputElementPointer, uint64 InputRowNumber, uint64 InputColNumber)
{
    if (InputElementPointer == nullptr || InputRowNumber == 0 || InputColNumber == 0)
    {
        mdkError << "Input pointer is nullptr @ mdkMatrix::Copy(ElementType_Input*, RowNumber, ColNumber)" << '\n';
        return true;
    }

    auto tempElementType = FindMatrixElementType(InputElementPointer[0]);

    if (tempElementType == mdkMatrixElementTypeEnum::UNKNOWN)
    {
        mdkError << "Input type is unknown @ mdkMatrix::Copy(ElementType_Input*, RowNumber, ColNumber)" << '\n';
        return false;
    }

    // if this matrix is not empty, check if this and Input share the same data
    if (this->IsEmpty() == false)
    {
        if (std::size_t(InputElementPointer) == std::size_t(this->GetElementPointer()))
        {
            mdkWarning << "A Matrix tries to Copy itself @ mdkMatrix::Copy(ElementType_Input*, RowNumber, ColNumber)" << '\n';
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
            mdkError << "Can not change matrix size @ mdkMatrix::Copy(ElementType_Input*, InputRowNumber, InputColNumber)" << '\n';
            return false;
        }
    }
    else
    {
        // check to see if new memory allocation is needed --------------------------------

        bool IsNewMemoryNeeded = false;

        //if self is empty
        if (this->IsEmpty() == true)
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
bool mdkMatrix<ElementType>::Fill(const ElementType& Element)
{
    auto ElementNumber = this->GetElementNumber();

    if (ElementNumber == 0)
    {
        mdkError << "Self is empty @ mdkMatrix::Fill" << '\n';
        return false;
    }

    auto BeginPointer = this->GetElementPointer();

    for (auto Ptr = BeginPointer; Ptr < BeginPointer + ElementNumber; ++Ptr)
    {
        Ptr[0] = Element;
    }

    return true;
}


template<typename ElementType>
inline
bool mdkMatrix<ElementType>::Share(mdkMatrix<ElementType>& InputMatrix)
{
    // MatrixA = MatrixA
    if (this == &InputMatrix)
    {
        mdkWarning << "A Matrix tries to ShallowCopy itself @ mdkMatrix::Share(InputMatrix)" << '\n';
        return false;
    }

    auto InputSize = InputMatrix.GetSize();

    auto SelfSize = this->GetSize();

    if (m_IsSizeFixed == true)
    {
        if (InputSize.RowNumber != SelfSize.RowNumber || InputSize.ColNumber != SelfSize.ColNumber)
        {
            mdkError << "Matrix size can not be changed @ mdkMatrix::Share(InputMatrix)" << '\n';
            return false;
        }
    }

    m_CoreData = InputMatrix.m_CoreData; // std::share_ptr

    m_ElementPointer = m_CoreData->DataArray.data();

    return true;
}


template<typename ElementType>
inline
bool mdkMatrix<ElementType>::Share(mdkMatrix<ElementType>&& InputMatrix)
{
    mdkMatrix<ElementType>& tempMatrix = InputMatrix;

    return this->Share(tempMatrix);
}



template<typename ElementType>
inline
void mdkMatrix<ElementType>::ForceShare(const mdkMatrix<ElementType>& InputMatrix)
{
    m_CoreData = InputMatrix.m_CoreData; // std::share_ptr

    m_ElementPointer = m_CoreData->DataArray.data();
}


template<typename ElementType>
inline
bool mdkMatrix<ElementType>::Take(mdkMatrix<ElementType>& InputMatrix)
{
    // MatrixA = MatrixA
    if (this == &InputMatrix)
    {
        mdkWarning << "A Matrix tries to take itself @ mdkMatrix::Take(InputMatrix)" << '\n';
        return false;
    }

    auto InputSize = InputMatrix.GetSize();

    auto SelfSize = this->GetSize();

    if (m_IsSizeFixed == true)
    {
        if (InputSize.RowNumber != SelfSize.RowNumber || InputSize.ColNumber != SelfSize.ColNumber)
        {
            mdkError << "Size does not match @ mdkMatrix::Take(InputMatrix)" << '\n';
            return false;
        }
    }
    else
    {
        if (InputMatrix.IsEmpty() == true)
        {
            mdkWarning << "InputMatrix is empty, and this matrix is set to be empty @ mdkMatrix::Take(InputMatrix)" << '\n';

            this->Clear();

            return true;
        }
    }


    m_CoreData = std::move(InputMatrix.m_CoreData);

    m_ElementPointer = m_CoreData->DataArray.data();

    // clear
    InputMatrix.ForceClear();

    return true;
}


template<typename ElementType>
inline
bool mdkMatrix<ElementType>::Take(mdkMatrix<ElementType>&& InputMatrix)
{
    mdkMatrix<ElementType>& tempMatrix = InputMatrix;

    return this->Take(tempMatrix);
}


template<typename ElementType>
inline
bool mdkMatrix<ElementType>::Take(const mdkShadowMatrix<ElementType>& ShadowMatrix)
{
    auto InputSize = ShadowMatrix.GetSize();

    auto SelfSize = this->GetSize();

    if (InputSize.RowNumber == SelfSize.RowNumber && InputSize.ColNumber == SelfSize.ColNumber)
    {
        ShadowMatrix.CreateMatrix(*this);
    }
    else
    {
        if (m_IsSizeFixed == true)
        {
            mdkError << "Size does not match @ mdkMatrix::Take(ShadowMatrix)" << '\n';
            return false;
        }

        this->Take(ShadowMatrix.CreateMatrix());
    }

    return true;
}


template<typename ElementType>
inline
bool mdkMatrix<ElementType>::Take(const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrix)
{
    auto InputSize = GlueMatrix.GetSize();

    auto SelfSize = this->GetSize();

    if (InputSize.RowNumber == SelfSize.RowNumber && InputSize.ColNumber == SelfSize.ColNumber)
    {
        GlueMatrix.CreateMatrix(*this);
    }
    else
    {
        if (m_IsSizeFixed == true)
        {
            mdkError << "Size does not match @ mdkMatrix::Take(GlueMatrix_ForLinearCombination)" << '\n';
            return false;
        }

        this->Take(GlueMatrix.CreateMatrix());
    }

    return true;
}


template<typename ElementType>
inline
bool mdkMatrix<ElementType>::Take(const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrix)
{
    auto InputSize = GlueMatrix.GetSize();

    auto SelfSize = this->GetSize();

    if (InputSize.RowNumber == SelfSize.RowNumber && InputSize.ColNumber == SelfSize.ColNumber)
    {
        GlueMatrix.CreateMatrix(*this);
    }
    else
    {
        if (m_IsSizeFixed == true)
        {
            mdkError << "Size does not match @ mdkMatrix::Take(GlueMatrix_ForMultiplication)" << '\n';
            return false;
        }

        this->Take(GlueMatrix.CreateMatrix());
    }

    return true;
}


template<typename ElementType>
inline
void mdkMatrix<ElementType>::Reset()
{
    m_CoreData.reset();

    m_ElementPointer = nullptr;

    m_IsSizeFixed = false;

    m_NaNElement = GetMatrixNaNElement(m_NaNElement);
}


template<typename ElementType>
inline
bool mdkMatrix<ElementType>::Clear()
{
    if (m_IsSizeFixed == true)
    {
        if (m_CoreData) // m_CoreData != nullptr
        {
            mdkError << "m_IsSizeFixed is true, Size can not change @ mdkMatrix::Clear()" << '\n';
            return false;
        }

        return true;
    }

    // if (m_IsSizeFixed == false)

    m_CoreData.reset();

    m_ElementPointer = nullptr;

    return true;
}


template<typename ElementType>
inline 
void mdkMatrix<ElementType>::ForceClear()
{
    m_CoreData.reset();

    m_ElementPointer = nullptr;

    m_IsSizeFixed = false;
}


template<typename ElementType>
inline
bool mdkMatrix<ElementType>::Reshape(uint64 InputRowNumber, uint64 InputColNumber)
{
    if (m_IsSizeFixed == true)
    {
        mdkError << "Matrix size can not be changed @ mdkMatrix::Reshape()" << '\n';
        return false;
    }

    if (this->IsEmpty() == true)
    {
        if (InputRowNumber > 0 || InputColNumber > 0)
        {
            mdkError << "Self is empty and Size does not match @ mdkMatrix::Reshape()" << '\n';
            return false;
        }
    }

    if (InputRowNumber*InputColNumber != this->GetElementNumber())
    {
        mdkError << "Size does not match @ mdkMatrix::Reshape" << '\n';
        return false;
    }

    m_CoreData->RowNumber = InputRowNumber;

    m_CoreData->ColNumber = InputColNumber;

    return true;
}


template<typename ElementType>
inline 
bool mdkMatrix<ElementType>::Resize(uint64 InputRowNumber, uint64 InputColNumber, bool IsSizeFixed = false)
{
    if (m_IsSizeFixed == true)
    {
        mdkError << "Matrix Size can not be changed @ mdkMatrix::Resize(uint64 RowNumber, uint64 ColNumber)" << '\n';
        return false;
    }

    auto SelfSize = this->GetSize();

    if (InputRowNumber == SelfSize.RowNumber && InputColNumber == SelfSize.ColNumber)
    {
        m_IsSizeFixed = IsSizeFixed;

        return true;
    }

    if (InputRowNumber == 0 || InputColNumber == 0)
    {
        this->Clear();

        m_IsSizeFixed = IsSizeFixed;

        return true;
    }

    // if self is empty
    if (SelfSize.RowNumber == 0)
    {
        m_CoreData = std::make_shared<mdkMatrixCoreData<ElementType>>();

        m_CoreData->RowNumber = InputRowNumber;

        m_CoreData->ColNumber = InputColNumber;

        m_CoreData->DataArray.resize(InputRowNumber*InputColNumber);

        m_ElementPointer = m_CoreData->DataArray.data();

        m_IsSizeFixed = IsSizeFixed;

        return true;
    }

    // if only Col changes

    if (InputRowNumber == SelfSize.RowNumber)
    {
        auto Self_ElementNumber = SelfSize.RowNumber * SelfSize.ColNumber;

        if (m_CoreData->DataArray.capacity() - Self_ElementNumber < (InputColNumber - SelfSize.ColNumber)*SelfSize.RowNumber)
        {
            m_CoreData->DataArray.reserve((SelfSize.ColNumber + MDK_Matrix_ColExpansionStep)*SelfSize.RowNumber);
        }

        m_CoreData->RowNumber = InputRowNumber;

        m_CoreData->ColNumber = InputColNumber;

        m_CoreData->DataArray.resize(SelfSize.ColNumber*SelfSize.RowNumber);

        m_ElementPointer = m_CoreData->DataArray.data();

        m_IsSizeFixed = IsSizeFixed;

        return true;
    }

    // RowNumber != SelfSize.RowNumber -----------------------------------------------------------------------------

    auto tempDataArray = std::vector<ElementType>(InputRowNumber*InputColNumber);

    auto RawPointer = m_CoreData->DataArray.data();

    auto ColNumber_min = std::min(SelfSize.ColNumber, InputColNumber);

    auto RowNumber_min = std::min(SelfSize.RowNumber, InputRowNumber);

    for (uint64 j = 0; j < ColNumber_min; ++j)
    {
        auto tempIndex = j*InputRowNumber;

        auto Index = j*SelfSize.RowNumber;

        for (uint64 i = 0; i < RowNumber_min; ++i)
        {
            tempDataArray[tempIndex + i] = RawPointer[Index + i];
        }
    }

    m_CoreData->RowNumber = InputRowNumber;

    m_CoreData->ColNumber = InputColNumber;

    m_CoreData->DataArray.clear();
    m_CoreData->DataArray = std::move(tempDataArray);

    m_ElementPointer = m_CoreData->DataArray.data();

    m_IsSizeFixed = IsSizeFixed;

    return true;
}


template<typename ElementType>
inline
bool mdkMatrix<ElementType>::IsSizeFixed() const
{
    return m_IsSizeFixed;
}


template<typename ElementType>
inline
bool mdkMatrix<ElementType>::IsEmpty() const
{
    return !(m_CoreData); 
}


template<typename ElementType>
inline
mdkMatrixSize mdkMatrix<ElementType>::GetSize() const
{
    mdkMatrixSize Size;

    if (m_CoreData) // m_CoreData != nullptr
    {
        Size.RowNumber = m_CoreData->RowNumber;

        Size.ColNumber = m_CoreData->ColNumber;
    }
    else
    {
        Size.RowNumber = 0;
        Size.ColNumber = 0;
    }

    return Size;
}


template<typename ElementType>
inline
void mdkMatrix<ElementType>::GetSize(uint64* RowNumber, uint64* ColNumber) const
{
    if (m_CoreData) // m_CoreData != nullptr
    {
        RowNumber[0] = m_CoreData->RowNumber;

        ColNumber[0] = m_CoreData->ColNumber;
    }
    else
    {
        RowNumber[0] = 0;
        ColNumber[0] = 0;
    }
}


template<typename ElementType>
inline
uint64 mdkMatrix<ElementType>::GetElementNumber() const
{
    if (m_CoreData) // m_CoreData != nullptr
    {
        return m_CoreData->RowNumber * m_CoreData->ColNumber;
    }
    else
    {
        return 0;
    }
}


template<typename ElementType>
inline 
uint64 mdkMatrix<ElementType>::GetColNumber() const
{
    if (m_CoreData) // m_CoreData != nullptr
    {
        return m_CoreData->ColNumber;
    }
    else
    {
        return 0;
    }
}


template<typename ElementType>
inline 
uint64 mdkMatrix<ElementType>::GetRowNumber() const
{
    if (m_CoreData) // m_CoreData != nullptr
    {
        return m_CoreData->RowNumber;
    }
    else
    {
        return 0;
    }
}


template<typename ElementType>
inline
const ElementType& mdkMatrix<ElementType>::GetNaNElement()  const
{
    return m_NaNElement;
}


template<typename ElementType>
inline
mdkMatrixElementTypeEnum mdkMatrix<ElementType>::GetElementType() const
{
    return FindMatrixElementType(m_NaNElement);
}


template<typename ElementType>
inline
ElementType* mdkMatrix<ElementType>::GetElementPointer()
{
    //return m_ElementPointer;

    if (m_CoreData) // m_CoreData != nullptr
    {
        return m_CoreData->DataArray.data();
    }
    else
    {
        return nullptr;
    }
}


template<typename ElementType>
inline
const ElementType* mdkMatrix<ElementType>::GetElementPointer() const
{
    //return m_ElementPointer;

    if (m_CoreData) // m_CoreData != nullptr
    {
        return m_CoreData->DataArray.data();
    }
    else
    {
        return nullptr;
    }
}

//----------- Get/Set Matrix(LinearIndex) -----------------------------------//

// operator[] (): no bound check in release mode

template<typename ElementType>
inline
ElementType& mdkMatrix<ElementType>::operator[](uint64 LinearIndex)
{
#if defined(MDK_DEBUG_Matrix_Operator_CheckBound)

    if (LinearIndex >= Self_ElementNumber)
    {
        mdkError << "Invalid Input @ mdkMatrix::operator[](i)" << '\n';

        return m_NaNElement;
    }

#endif //MDK_DEBUG_Matrix_Operator_CheckBound

    return m_ElementPointer[LinearIndex];
   //return (*m_CoreData)[LinearIndex];
}


template<typename ElementType>
inline
const ElementType& mdkMatrix<ElementType>::operator[](uint64 LinearIndex) const
{
#if defined(MDK_DEBUG_Matrix_Operator_CheckBound)

    if (LinearIndex >= Self_ElementNumber)
    {
        mdkError << "Invalid Input @ mdkMatrix::operator[](i) const" << '\n';

        return m_NaNElement;
    }

#endif //MDK_DEBUG_Matrix_Operator_CheckBound

    return m_ElementPointer[LinearIndex];
}


template<typename ElementType>
inline
ElementType& mdkMatrix<ElementType>::operator()(uint64 LinearIndex)
{
#if defined(MDK_DEBUG_Matrix_Operator_CheckBound)

    if (LinearIndex >= Self_ElementNumber)
    {
        mdkError << "Invalid Input @ mdkMatrix::operator()(i)" << '\n';

        return m_NaNElement;
    }

#endif //MDK_DEBUG_Matrix_Operator_CheckBound

	return m_ElementPointer[LinearIndex];
}


template<typename ElementType>
inline
const ElementType& mdkMatrix<ElementType>::operator()(uint64 LinearIndex) const
{
#if defined(MDK_DEBUG_Matrix_Operator_CheckBound)

    if (LinearIndex >= Self_ElementNumber)
    {
        mdkError << "Invalid Input @ mdkMatrix::operator()(i) const" << '\n';

        return m_NaNElement;
    }

#endif //MDK_DEBUG_Matrix_Operator_CheckBound

	return m_ElementPointer[LinearIndex];
}

// at(): bound check

template<typename ElementType>
inline
ElementType& mdkMatrix<ElementType>::at(uint64 LinearIndex)
{
	if (LinearIndex >= this->GetElementNumber())
	{
		mdkError << "Invalid Input @ mdkMatrix::at(i)" << '\n';
        
        return m_NaNElement;
	}

	return m_ElementPointer[LinearIndex];
}


template<typename ElementType>
inline
const ElementType& mdkMatrix<ElementType>::at(uint64 LinearIndex) const
{
    if (LinearIndex >= this->GetElementNumber())
	{
		mdkError << "Invalid Input @ mdkMatrix::at(i) const" << '\n';
        
        return m_NaNElement;
	}

	return m_ElementPointer[LinearIndex];
}

//----------- Get/Set Matrix(i,j)  ---------------------------------------------//

// operator(): no bound check in release mode

template<typename ElementType>
inline
ElementType& mdkMatrix<ElementType>::operator()(uint64 RowIndex, uint64 ColIndex)
{
#if defined(MDK_DEBUG_Matrix_Operator_CheckBound)

    auto SelfSize = this->GetSize();

    if (RowIndex >= SelfSize.RowNumber || ColIndex >= SelfSize.ColNumber)
    {
        mdkError << "Invalid Input @ mdkMatrix::operator()(i,j)" << '\n';

        return m_NaNElement;
    }

#endif //MDK_DEBUG_Matrix_Operator_CheckBound

    auto LinearIndex = ColIndex*m_CoreData->RowNumber + RowIndex;
    
    return m_ElementPointer[LinearIndex];

}


template<typename ElementType>
inline
const ElementType& mdkMatrix<ElementType>::operator()(uint64 RowIndex, uint64 ColIndex) const
{
#if defined(MDK_DEBUG_Matrix_Operator_CheckBound)

    auto SelfSize = this->GetSize();

    if (RowIndex >= SelfSize.RowNumber || ColIndex >= SelfSize.ColNumber)
    {
        mdkError << "Invalid Input @ mdkMatrix::operator()(i,j) const" << '\n';

        return m_NaNElement;
    }

#endif //MDK_DEBUG_Matrix_Operator_CheckBound

    auto LinearIndex = ColIndex*m_CoreData->RowNumber + RowIndex;

    return m_ElementPointer[LinearIndex];
}


template<typename ElementType>
inline
ElementType& mdkMatrix<ElementType>::at(uint64 RowIndex, uint64 ColIndex)
{
    auto SelfSize = this->GetSize();

    if (RowIndex >= SelfSize.RowNumber || ColIndex >= SelfSize.ColNumber)
    {
        mdkError << "Invalid Input @ mdkMatrix::at(i,j)" << '\n';
        
        return m_NaNElement;
    }
    
    auto LinearIndex = ColIndex*m_CoreData->RowNumber + RowIndex;

    return m_ElementPointer[LinearIndex];
}


template<typename ElementType>
inline
const ElementType& mdkMatrix<ElementType>::at(uint64 RowIndex, uint64 ColIndex) const
{
    if (RowIndex >= SelfSize.RowNumber || ColIndex >= SelfSize.ColNumber)
    {
        mdkError << "Invalid Input @ mdkMatrix::at(i,j) const" << '\n';
        
        return m_NaNElement;
    }

    auto LinearIndex = ColIndex*m_CoreData->RowNumber + RowIndex;

    return m_ElementPointer[LinearIndex];
}

//---------------------- Get/Set a set of elements ------------------------------------------------------//

// operator(): no bound check in release mode
//
// note: operator[] is for single element access only, operator[{}] is not defined

template<typename ElementType>
inline
mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::operator()(std::initializer_list<uint64>& LinearIndexList)
{
#if defined(MDK_DEBUG_Matrix_Operator_CheckBound)

    if (LinearIndexList.size() == 0)
    {
        mdkError << "LinearIndexList is empty @ mdkMatrix::operator()(std::initializer_list {LinearIndexList})" << '\n';
        mdkShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto Self_ElementNumber = this->GetElementNumber();

    for (auto it = LinearIndexList.begin(); it != LinearIndexList.end(); ++it)
    {
        if (*it >= Self_ElementNumber)
        {
            mdkError << "Invalid LinearIndexList @ mdkMatrix::operator()(std::initializer_list {LinearIndexList})" << '\n';
            mdkShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

#endif //MDK_DEBUG_Matrix_Operator_CheckBound

    mdkShadowMatrix<ElementType> tempShadowMatrix(*this, LinearIndexList);

    return tempShadowMatrix;
}


template<typename ElementType>
inline
const mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::operator()(std::initializer_list<uint64>& LinearIndexList) const
{
#if defined(MDK_DEBUG_Matrix_Operator_CheckBound)

    if (LinearIndexList.size() == 0)
    {
        mdkError << "LinearIndexList is empty @ mdkMatrix::operator()(std::initializer_list {LinearIndexList}) const" << '\n';
        mdkShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto Self_ElementNumber = this->GetElementNumber();

    for (auto it = LinearIndexList.begin(); it != LinearIndexList.end(); ++it)
    {
        if (*it >= Self_ElementNumber)
        {
            mdkError << "Invalid LinearIndexList @ mdkMatrix::operator()(std::initializer_list {LinearIndexList}) const" << '\n';
            mdkShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

#endif //MDK_DEBUG_Matrix_Operator_CheckBound

    mdkShadowMatrix<ElementType> tempShadowMatrix(*this, LinearIndexList);

    return tempShadowMatrix;
}


template<typename ElementType>
inline
mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::operator()(const std::vector<uint64>& LinearIndexList)
{
#if defined(MDK_DEBUG_Matrix_Operator_CheckBound)

    if (LinearIndexList.size() == 0)
    {
        mdkError << "LinearIndexList is empty @ mdkMatrix::operator()(std::vector {LinearIndexList})" << '\n';
        mdkShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto Self_ElementNumber = this->GetElementNumber();

    for (auto it = LinearIndexList.begin(); it != LinearIndexList.end(); ++it)
    {
        if (*it >= Self_ElementNumber)
        {
            mdkError << "Invalid LinearIndexList @ mdkMatrix::operator()(std::vector {LinearIndexList})" << '\n';
            mdkShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

#endif //MDK_DEBUG_Matrix_Operator_CheckBound

    mdkShadowMatrix<ElementType> tempShadowMatrix(*this, LinearIndexList);

    return tempShadowMatrix;
}


template<typename ElementType>
inline
const mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::operator()(const std::vector<uint64>& LinearIndexList) const
{
#if defined(MDK_DEBUG_Matrix_Operator_CheckBound)

    if (LinearIndexList.size() == 0)
    {
        mdkError << "LinearIndexList is empty @ mdkMatrix::operator()(std::vector {LinearIndexList}) const" << '\n';
        mdkShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto Self_ElementNumber = this->GetElementNumber();

    for (auto it = LinearIndexList.begin(); it != LinearIndexList.end(); ++it)
    {
        if (*it >= Self_ElementNumber)
        {
            mdkError << "Invalid LinearIndexList @ mdkMatrix::operator()(std::vector {LinearIndexList}) const" << '\n';
            mdkShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

#endif //MDK_DEBUG_Matrix_Operator_CheckBound

    mdkShadowMatrix<ElementType> tempShadowMatrix(*this, LinearIndexList);

    return tempShadowMatrix;
}


template<typename ElementType>
inline
mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::operator()(const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol)
{
    mdkShadowMatrix<ElementType> tempShadowMatrix(*this, ALL_Symbol);

    return tempShadowMatrix;
}


template<typename ElementType>
inline
const mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::operator()(const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol) const
{
    mdkShadowMatrix<ElementType> tempShadowMatrix(*this, ALL_Symbol);

    return tempShadowMatrix;
}


template<typename ElementType>
inline
mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::at(std::initializer_list<uint64>& LinearIndexList)
{
    if (LinearIndexList.size() == 0)
    {
        mdkError << "LinearIndexList is empty @ mdkMatrix::at(std::initializer_list {LinearIndexList})" << '\n';
        mdkShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto Self_ElementNumber = this->GetElementNumber();

    for (auto it = LinearIndexList.begin(); it != LinearIndexList.end(); ++it)
    {
        if (*it >= Self_ElementNumber)
        {
            mdkError << "Invalid LinearIndexList @ mdkMatrix::at(std::initializer_list {LinearIndexList})" << '\n';
            mdkShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    mdkShadowMatrix<ElementType> tempShadowMatrix(*this, LinearIndexList);

    return tempShadowMatrix;
}


template<typename ElementType>
inline
const mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::at(std::initializer_list<uint64>& LinearIndexList) const
{
    if (LinearIndexList.size() == 0)
    {
        mdkError << "LinearIndexList is empty @ mdkMatrix::at(std::initializer_list {LinearIndexList}) const" << '\n';
        mdkShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto Self_ElementNumber = this->GetElementNumber();

    for (auto it = LinearIndexList.begin(); it != LinearIndexList.end(); ++it)
    {
        if (*it >= Self_ElementNumber)
        {
            mdkError << "Invalid LinearIndexList @ mdkMatrix::at(std::initializer_list {LinearIndexList}) const" << '\n';
            mdkShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    mdkShadowMatrix<ElementType> tempShadowMatrix(*this, LinearIndexList);

    return tempShadowMatrix;
}


template<typename ElementType>
inline
mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::at(const std::vector<uint64>& LinearIndexList)
{
    if (LinearIndexList.size() == 0)
    {
        mdkError << "LinearIndexList is empty @ mdkMatrix::at(std::vector {LinearIndexList})" << '\n';
        mdkShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto Self_ElementNumber = this->GetElementNumber();

    for (auto it = LinearIndexList.begin(); it != LinearIndexList.end(); ++it)
    {
        if (*it >= Self_ElementNumber)
        {
            mdkError << "Invalid LinearIndexList @ mdkMatrix::at(std::vector {LinearIndexList})" << '\n';
            mdkShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    mdkShadowMatrix<ElementType> tempShadowMatrix(*this, LinearIndexList);

    return tempShadowMatrix;
}


template<typename ElementType>
inline
const mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::at(const std::vector<uint64>& LinearIndexList) const
{
    if (LinearIndexList.size() == 0)
    {
        mdkError << "LinearIndexList is empty @ mdkMatrix::at(std::vector {LinearIndexList} const)" << '\n';
        mdkShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto Self_ElementNumber = this->GetElementNumber();

    for (auto it = LinearIndexList.begin(); it != LinearIndexList.end(); ++it)
    {
        if (*it >= Self_ElementNumber)
        {
            mdkError << "Invalid LinearIndexList @ mdkMatrix::at(std::vector {LinearIndexList} const)" << '\n';
            mdkShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    mdkShadowMatrix<ElementType> tempShadowMatrix(*this, LinearIndexList);

    return tempShadowMatrix;
}


template<typename ElementType>
inline
mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::at(const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol)
{
    mdkShadowMatrix<ElementType> tempShadowMatrix(*this, ALL_Symbol);

    return tempShadowMatrix;
}


template<typename ElementType>
inline
const mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::at(const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol) const
{
    mdkShadowMatrix<ElementType> tempShadowMatrix(*this, ALL_Symbol);

    return tempShadowMatrix;
}

//---------------------- Get/Set SubMatrix ------------------------------------------------------//

// operator(): no bound check

template<typename ElementType>
inline
mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::operator()(std::initializer_list<uint64>& RowIndexList, std::initializer_list<uint64>& ColIndexList)
{
#if defined(MDK_DEBUG_Matrix_Operator_CheckBound)

    if (RowIndexList.size() == 0 || ColIndexList.size() == 0)
    {
        mdkError << "RowIndexList or  ColIndexList is empty @ mdkMatrix::operator()(std::initializer_list {RowIndexList}, {ColIndexList})" << '\n';
        mdkShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = RowIndexList.begin(); it != RowIndexList.end(); ++it)
    {
        if (*it >= SelfSize.RowNumber)
        {
            mdkError << "Invalid RowIndexList @ mdkMatrix::operator()(std::initializer_list {RowIndexList}, {ColIndexList})" << '\n';
            mdkShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= SelfSize.ColNumber)
        {
            mdkError << "Invalid ColIndexList @ mdkMatrix::operator()(std::initializer_list {RowIndexList}, {ColIndexList})" << '\n';
            mdkShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

#endif //MDK_DEBUG_Matrix_Operator_CheckBound

    mdkShadowMatrix<ElementType> tempShadowMatrix(*this, RowIndexList, ColIndexList);

    return tempShadowMatrix;
}


template<typename ElementType>
inline
const mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::operator()(std::initializer_list<uint64>& RowIndexList, std::initializer_list<uint64>& ColIndexList) const
{
#if defined(MDK_DEBUG_Matrix_Operator_CheckBound)

    if (RowIndexList.size() == 0 || ColIndexList.size() == 0)
    {
        mdkError << "RowIndexList or  ColIndexList is empty @ mdkMatrix::operator()(std::initializer_list {RowIndexList}, {ColIndexList}) const" << '\n';
        mdkShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = RowIndexList.begin(); it != RowIndexList.end(); ++it)
    {
        if (*it >= SelfSize.RowNumber)
        {
            mdkError << "Invalid RowIndexList @ mdkMatrix::operator()(std::initializer_list {RowIndexList}, {ColIndexList}) const" << '\n';
            mdkShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= SelfSize.ColNumber)
        {
            mdkError << "Invalid ColIndexList @ mdkMatrix::operator()(std::initializer_list {RowIndexList}, {ColIndexList}) const" << '\n';
            mdkShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

#endif //MDK_DEBUG_Matrix_Operator_CheckBound

    mdkShadowMatrix<ElementType> tempShadowMatrix(*this, RowIndexList, ColIndexList);

    return tempShadowMatrix;
}


template<typename ElementType>
inline
mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::operator()(const std::initializer_list<uint64>& RowIndexList, const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol)
{
#if defined(MDK_DEBUG_Matrix_Operator_CheckBound)

    if (RowIndexList.size() == 0)
    {
        mdkError << "RowIndexList is empty @ mdkMatrix::operator()(std::initializer_list {RowIndexList}, ALL)" << '\n';
        mdkShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = RowIndexList.begin(); it != RowIndexList.end(); ++it)
    {
        if (*it >= SelfSize.RowNumber)
        {
            mdkError << "Invalid RowIndexList @ mdkMatrix::operator()(std::initializer_list {RowIndexList}, ALL)" << '\n';
            mdkShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

#endif //MDK_DEBUG_Matrix_Operator_CheckBound

    mdkShadowMatrix<ElementType> tempShadowMatrix(*this, RowIndexList, ALL_Symbol);

    return tempShadowMatrix;
}


template<typename ElementType>
inline
const mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::operator()(const std::initializer_list<uint64>& RowIndexList, const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol) const
{
#if defined(MDK_DEBUG_Matrix_Operator_CheckBound)

    if (RowIndexList.size() == 0)
    {
        mdkError << "RowIndexList is empty @ mdkMatrix::operator()(std::initializer_list {RowIndexList}, ALL) const" << '\n';
        mdkShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = RowIndexList.begin(); it != RowIndexList.end(); ++it)
    {
        if (*it >= SelfSize.RowNumber)
        {
            mdkError << "Invalid RowIndexList @ mdkMatrix::operator()(std::initializer_list {RowIndexList}, ALL) const" << '\n';
            mdkShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

#endif //MDK_DEBUG_Matrix_Operator_CheckBound

    mdkShadowMatrix<ElementType> tempShadowMatrix(*this, RowIndexList, ALL_Symbol);

    return tempShadowMatrix;
}


template<typename ElementType>
inline
mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::operator()(const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol, const std::initializer_list<uint64>& ColIndexList)
{
#if defined(MDK_DEBUG_Matrix_Operator_CheckBound)

    if (ColIndexList.size() == 0)
    {
        mdkError << "ColIndexList is empty @ mdkMatrix::operator()(ALL, std::initializer_list {ColIndexList})" << '\n';
        mdkShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= SelfSize.ColNumber)
        {
            mdkError << "Invalid ColIndexList @ mdkMatrix::operator()(ALL, std::initializer_list {ColIndexList})" << '\n';
            mdkShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

#endif //MDK_DEBUG_Matrix_Operator_CheckBound

    mdkShadowMatrix<ElementType> tempShadowMatrix(*this, ALL_Symbol, ColIndexList);

    return tempShadowMatrix;
}


template<typename ElementType>
inline
const mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::operator()(const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol, const std::initializer_list<uint64>& ColIndexList) const
{
#if defined(MDK_DEBUG_Matrix_Operator_CheckBound)

    if (ColIndexList.size() == 0)
    {
        mdkError << "ColIndexList is empty @ mdkMatrix::operator()(ALL, std::initializer_list {ColIndexList}) const" << '\n';
        mdkShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= SelfSize.ColNumber)
        {
            mdkError << "Invalid ColIndexList @ mdkMatrix::operator()(ALL, std::initializer_list {ColIndexList}) const" << '\n';
            mdkShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

#endif //MDK_DEBUG_Matrix_Operator_CheckBound

    mdkShadowMatrix<ElementType> tempShadowMatrix(*this, ALL_Symbol, ColIndexList);

    return tempShadowMatrix;
}


template<typename ElementType>
inline
mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::operator()(const std::vector<uint64>& RowIndexList, const std::vector<uint64>& ColIndexList)
{
#if defined(MDK_DEBUG_Matrix_Operator_CheckBound)

    if (RowIndexList.size() == 0 || ColIndexList.size() == 0)
    {
        mdkError << "RowIndexList or  ColIndexList is empty @ mdkMatrix::operator()(std::vector {RowIndexList}, {ColIndexList})" << '\n';
        mdkShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = RowIndexList.begin(); it != RowIndexList.end(); ++it)
    {
        if (*it >= SelfSize.RowNumber)
        {
            mdkError << "Invalid RowIndexList @ mdkMatrix::operator()(std::vector {RowIndexList}, {ColIndexList})" << '\n';
            mdkShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= SelfSize.ColNumber)
        {
            mdkError << "Invalid ColIndexList @ mdkMatrix::operator()(std::vector {RowIndexList}, {ColIndexList})" << '\n';
            mdkShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

#endif //MDK_DEBUG_Matrix_Operator_CheckBound

    mdkShadowMatrix<ElementType> tempShadowMatrix(*this, RowIndexList, ColIndexList);

    return tempShadowMatrix;
}


template<typename ElementType>
inline
const mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::operator()(const std::vector<uint64>& RowIndexList, const std::vector<uint64>& ColIndexList) const 
{
#if defined(MDK_DEBUG_Matrix_Operator_CheckBound)

    if (RowIndexList.size() == 0 || ColIndexList.size() == 0)
    {
        mdkError << "RowIndexList or  ColIndexList is empty @ mdkMatrix::operator()(std::vector {RowIndexList}, {ColIndexList}) const" << '\n';
        mdkShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = RowIndexList.begin(); it != RowIndexList.end(); ++it)
    {
        if (*it >= SelfSize.RowNumber)
        {
            mdkError << "Invalid RowIndexList @ mdkMatrix::operator()(std::vector {RowIndexList}, {ColIndexList}) const" << '\n';
            mdkShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= SelfSize.ColNumber)
        {
            mdkError << "Invalid ColIndexList @ mdkMatrix::operator()(std::vector {RowIndexList}, {ColIndexList})" << '\n';
            mdkShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

#endif //MDK_DEBUG_Matrix_Operator_CheckBound

    mdkShadowMatrix<ElementType> tempShadowMatrix(*this, RowIndexList, ColIndexList);

    return tempShadowMatrix;
}


template<typename ElementType>
inline
mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::operator()(const std::vector<uint64>& RowIndexList, const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol)
{
#if defined(MDK_DEBUG_Matrix_Operator_CheckBound)

    if (RowIndexList.size() == 0)
    {
        mdkError << "RowIndexList or  ColIndexList is empty @ mdkMatrix::operator()(std::vector {RowIndexList}, ALL)" << '\n';
        mdkShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = RowIndexList.begin(); it != RowIndexList.end(); ++it)
    {
        if (*it >= SelfSize.RowNumber)
        {
            mdkError << "Invalid RowIndexList @ mdkMatrix::operator()(std::vector {RowIndexList}, ALL)" << '\n';
            mdkShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

#endif //MDK_DEBUG_Matrix_Operator_CheckBound

    mdkShadowMatrix<ElementType> tempShadowMatrix(*this, RowIndexList, ALL_Symbol);

    return tempShadowMatrix;
}


template<typename ElementType>
inline
const mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::operator()(const std::vector<uint64>& RowIndexList, const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol) const
{
#if defined(MDK_DEBUG_Matrix_Operator_CheckBound)

    if (RowIndexList.size() == 0)
    {
        mdkError << "RowIndexList or  ColIndexList is empty @ mdkMatrix::operator()(std::vector {RowIndexList}, ALL) const" << '\n';
        mdkShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = RowIndexList.begin(); it != RowIndexList.end(); ++it)
    {
        if (*it >= SelfSize.RowNumber)
        {
            mdkError << "Invalid RowIndexList @ mdkMatrix::operator()(std::vector {RowIndexList}, ALL) const" << '\n';
            mdkShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

#endif //MDK_DEBUG_Matrix_Operator_CheckBound

    mdkShadowMatrix<ElementType> tempShadowMatrix(*this, RowIndexList, ALL_Symbol);

    return tempShadowMatrix;
}


template<typename ElementType>
inline
mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::operator()(const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol, const std::vector<uint64>& ColIndexList)
{
#if defined(MDK_DEBUG_Matrix_Operator_CheckBound)

    if (ColIndexList.size() == 0)
    {
        mdkError << "ColIndexList is empty @ mdkMatrix::operator()(ALL, std::vector {ColIndexList})" << '\n';
        mdkShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= SelfSize.ColNumber)
        {
            mdkError << "Invalid ColIndexList @ mdkMatrix::operator()(ALL, std::vector {ColIndexList})" << '\n';
            mdkShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

#endif //MDK_DEBUG_Matrix_Operator_CheckBound

    mdkShadowMatrix<ElementType> tempShadowMatrix(*this, ALL_Symbol, ColIndexList);

    return tempShadowMatrix;
}


template<typename ElementType>
inline
const mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::operator()(const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol, const std::vector<uint64>& ColIndexList) const
{
#if defined(MDK_DEBUG_Matrix_Operator_CheckBound)

    if (ColIndexList.size() == 0)
    {
        mdkError << "ColIndexList is empty @ mdkMatrix::operator()(ALL, std::vector {ColIndexList}) const" << '\n';
        mdkShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= SelfSize.ColNumber)
        {
            mdkError << "Invalid ColIndexList @ mdkMatrix::operator()(ALL, std::vector {ColIndexList}) const" << '\n';
            mdkShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

#endif //MDK_DEBUG_Matrix_Operator_CheckBound

    mdkShadowMatrix<ElementType> tempShadowMatrix(*this, ALL_Symbol, ColIndexList);

    return tempShadowMatrix;
}

// at(): bound check

template<typename ElementType>
inline
mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::at(std::initializer_list<uint64>& RowIndexList, std::initializer_list<uint64>& ColIndexList)
{
    if (RowIndexList.size() == 0 || ColIndexList.size() == 0)
    {
        mdkError << "RowIndexList or  ColIndexList is empty @ mdkMatrix::at(std::initializer_list {RowIndexList}, {ColIndexList})" << '\n';
        mdkShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = RowIndexList.begin(); it != RowIndexList.end(); ++it)
    {
        if (*it >= SelfSize.RowNumber)
        {
            mdkError << "Invalid RowIndexList @ mdkMatrix::at(std::initializer_list {RowIndexList}, {ColIndexList})" << '\n';
            mdkShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= SelfSize.ColNumber)
        {
            mdkError << "Invalid ColIndexList @ mdkMatrix::at({RowIndexList}, {ColIndexList})" << '\n';
            mdkShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    mdkShadowMatrix<ElementType> tempShadowMatrix(*this, RowIndexList, ColIndexList);

    return tempShadowMatrix;
}


template<typename ElementType>
inline
const mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::at(std::initializer_list<uint64>& RowIndexList, std::initializer_list<uint64>& ColIndexList) const
{
    if (RowIndexList.size() == 0 || ColIndexList.size() == 0)
    {
        mdkError << "RowIndexList or  ColIndexList is empty @ mdkMatrix::at(std::initializer_list {RowIndexList}, {ColIndexList}) const" << '\n';
        mdkShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = RowIndexList.begin(); it != RowIndexList.end(); ++it)
    {
        if (*it >= SelfSize.RowNumber)
        {
            mdkError << "Invalid RowIndexList @ mdkMatrix::at(std::initializer_list {RowIndexList}, {ColIndexList}) const" << '\n';
            mdkShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= SelfSize.ColNumber)
        {
            mdkError << "Invalid ColIndexList @ mdkMatrix::at({RowIndexList}, {ColIndexList})" << '\n';
            mdkShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    mdkShadowMatrix<ElementType> tempShadowMatrix(*this, RowIndexList, ColIndexList);

    return tempShadowMatrix;
}


template<typename ElementType>
inline
mdkShadowMatrix<ElementType> 
mdkMatrix<ElementType>::at(const std::initializer_list<uint64>& RowIndexList, const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol)
{
    if (RowIndexList.size() == 0)
    {
        mdkError << "RowIndexList is empty @ mdkMatrix::at(std::initializer_list {RowIndexList}, ALL)" << '\n';
        mdkShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = RowIndexList.begin(); it != RowIndexList.end(); ++it)
    {
        if (*it >= SelfSize.RowNumber)
        {
            mdkError << "Invalid RowIndexList @ mdkMatrix::at(std::initializer_list {RowIndexList}, ALL)" << '\n';
            mdkShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    mdkShadowMatrix<ElementType> tempShadowMatrix(*this, RowIndexList, ALL_Symbol);

    return tempShadowMatrix;
}


template<typename ElementType>
inline
const mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::at(const std::initializer_list<uint64>& RowIndexList, const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol) const
{
    if (RowIndexList.size() == 0)
    {
        mdkError << "RowIndexList is empty @ mdkMatrix::at(std::initializer_list {RowIndexList}, ALL) const" << '\n';
        mdkShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = RowIndexList.begin(); it != RowIndexList.end(); ++it)
    {
        if (*it >= SelfSize.RowNumber)
        {
            mdkError << "Invalid RowIndexList @ mdkMatrix::at(std::initializer_list {RowIndexList}, ALL) const" << '\n';
            mdkShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    mdkShadowMatrix<ElementType> tempShadowMatrix(*this, RowIndexList, ALL_Symbol);

    return tempShadowMatrix;
}


template<typename ElementType>
inline
mdkShadowMatrix<ElementType> 
mdkMatrix<ElementType>::at(const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol, const std::initializer_list<uint64>& ColIndexList)
{
    if (ColIndexList.size() == 0)
    {
        mdkError << "ColIndexList is empty @ mdkMatrix::at(ALL, std::initializer_list {ColIndexList})" << '\n';
        mdkShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= SelfSize.ColNumber)
        {
            mdkError << "Invalid ColIndexList @ mdkMatrix::at(ALL, std::initializer_list {ColIndexList})" << '\n';
            mdkShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    mdkShadowMatrix<ElementType> tempShadowMatrix(*this, ALL_Symbol, ColIndexList);

    return tempShadowMatrix;
}


template<typename ElementType>
inline
const mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::at(const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol, const std::initializer_list<uint64>& ColIndexList) const
{
    if (ColIndexList.size() == 0)
    {
        mdkError << "ColIndexList is empty @ mdkMatrix::at(ALL, std::initializer_list {ColIndexList}) const" << '\n';
        mdkShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= SelfSize.ColNumber)
        {
            mdkError << "Invalid ColIndexList @ mdkMatrix::at(ALL, std::initializer_list {ColIndexList}) const" << '\n';
            mdkShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    mdkShadowMatrix<ElementType> tempShadowMatrix(*this, ALL_Symbol, ColIndexList);

    return tempShadowMatrix;
}


template<typename ElementType>
inline
mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::at(const std::vector<uint64>& RowIndexList, const std::vector<uint64>& ColIndexList)
{
    if (RowIndexList.size() == 0 || ColIndexList.size() == 0)
    {
        mdkError << "RowIndexList or  ColIndexList is empty @ mdkMatrix::at(std::vector {RowIndexList}, {ColIndexList})" << '\n';
        mdkShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = RowIndexList.begin(); it != RowIndexList.end(); ++it)
    {
        if (*it >= SelfSize.RowNumber)
        {
            mdkError << "Invalid RowIndexList @ mdkMatrix::at(std::vector {RowIndexList}, {ColIndexList})" << '\n';
            mdkShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= SelfSize.ColNumber)
        {
            mdkError << "Invalid ColIndexList @ mdkMatrix::at(std::vector {RowIndexList}, {ColIndexList})" << '\n';
            mdkShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    mdkShadowMatrix<ElementType> tempShadowMatrix(*this, RowIndexList, ColIndexList);

    return tempShadowMatrix;
}


template<typename ElementType>
inline
const mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::at(const std::vector<uint64>& RowIndexList, const std::vector<uint64>& ColIndexList) const
{
    if (RowIndexList.size() == 0 || ColIndexList.size() == 0)
    {
        mdkError << "RowIndexList or  ColIndexList is empty @ mdkMatrix::at(std::vector {RowIndexList}, {ColIndexList}) const" << '\n';
        mdkShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = RowIndexList.begin(); it != RowIndexList.end(); ++it)
    {
        if (*it >= SelfSize.RowNumber)
        {
            mdkError << "Invalid RowIndexList @ mdkMatrix::at(std::vector {RowIndexList}, {ColIndexList}) const" << '\n';
            mdkShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= SelfSize.ColNumber)
        {
            mdkError << "Invalid ColIndexList @ mdkMatrix::at(std::vector {RowIndexList}, {ColIndexList}) const" << '\n';
            mdkShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    mdkShadowMatrix<ElementType> tempShadowMatrix(*this, RowIndexList, ColIndexList);

    return tempShadowMatrix;
}


template<typename ElementType>
inline
mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::at(const std::vector<uint64>& RowIndexList, const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol)
{
    if (RowIndexList.size() == 0)
    {
        mdkError << "RowIndexList is empty @ mdkMatrix::at(std::vector {RowIndexList}, ALL)" << '\n';
        mdkShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = RowIndexList.begin(); it != RowIndexList.end(); ++it)
    {
        if (*it >= SelfSize.RowNumber)
        {
            mdkError << "Invalid RowIndexList @ mdkMatrix::at(std::vector {RowIndexList}, ALL)" << '\n';
            mdkShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    mdkShadowMatrix<ElementType> tempShadowMatrix(*this, RowIndexList, ALL_Symbol);

    return tempShadowMatrix;
}


template<typename ElementType>
inline
const mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::at(const std::vector<uint64>& RowIndexList, const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol) const
{
    if (RowIndexList.size() == 0)
    {
        mdkError << "RowIndexList is empty @ mdkMatrix::at(std::vector {RowIndexList}, ALL) const" << '\n';
        mdkShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = RowIndexList.begin(); it != RowIndexList.end(); ++it)
    {
        if (*it >= SelfSize.RowNumber)
        {
            mdkError << "Invalid RowIndexList @ mdkMatrix::at(std::vector {RowIndexList}, ALL) const" << '\n';
            mdkShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    mdkShadowMatrix<ElementType> tempShadowMatrix(*this, RowIndexList, ALL_Symbol);

    return tempShadowMatrix;
}


template<typename ElementType>
inline
mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::at(const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol, const std::vector<uint64>& ColIndexList)
{
    if (ColIndexList.size() == 0)
    {
        mdkError << "ColIndexList is empty @ mdkMatrix::at(ALL, std::vector {ColIndexList})" << '\n';
        mdkShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= SelfSize.ColNumber)
        {
            mdkError << "Invalid ColIndexList @ mdkMatrix::at(ALL, std::vector {ColIndexList})" << '\n';
            mdkShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    mdkShadowMatrix<ElementType> tempShadowMatrix(*this, ALL_Symbol, ColIndexList);

    return tempShadowMatrix;
}


template<typename ElementType>
inline
const mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::at(const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol, const std::vector<uint64>& ColIndexList) const
{
    if (ColIndexList.size() == 0)
    {
        mdkError << "ColIndexList is empty @ mdkMatrix::at(ALL, std::vector {ColIndexList}) const" << '\n';
        mdkShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    auto SelfSize = this->GetSize();

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= SelfSize.ColNumber)
        {
            mdkError << "Invalid ColIndexList @ mdkMatrix::at(ALL, std::vector {ColIndexList}) const" << '\n';
            mdkShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    mdkShadowMatrix<ElementType> tempShadowMatrix(*this, ALL_Symbol, ColIndexList);

    return tempShadowMatrix;
}


template<typename ElementType>
inline
mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::SubMatrix(uint64 RowIndex_start, uint64 RowIndex_end, uint64 ColIndex_start, uint64 ColIndex_end)
{
    auto SelfSize = this->GetSize();

    if (RowIndex_start >= SelfSize.RowNumber || RowIndex_end >= SelfSize.RowNumber
        || ColIndex_start >= SelfSize.ColNumber || ColIndex_end >= SelfSize.ColNumber
        || RowIndex_start > RowIndex_end || ColIndex_start > ColIndex_end)
    {
        mdkError << "Invalid Input @ mdkMatrix::SubMatrix(RowIndex_start, RowIndex_end, ColIndex_start, ColIndex_end)" << '\n';
        mdkShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    std::vector<uint64> RowIndexList(RowIndex_end - RowIndex_start + 1);

    for (uint64 i = RowIndex_start; i <= RowIndex_end; ++i)
    {
        RowIndexList[i - RowIndex_start] = i;
    }

    std::vector<uint64> ColIndexList(ColIndex_end - ColIndex_start + 1);

    for (uint64 j = ColIndex_start; j <= ColIndex_end; ++j)
    {
        ColIndexList[j - ColIndex_start] = j;
    }

    mdkShadowMatrix<ElementType> tempShadowMatrix(*this, RowIndexList, ColIndexList);

    return tempShadowMatrix;
}


template<typename ElementType>
inline
const mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::SubMatrix(uint64 RowIndex_start, uint64 RowIndex_end, uint64 ColIndex_start, uint64 ColIndex_end) const
{
    auto SelfSize = this->GetSize();

    if (RowIndex_start >= SelfSize.RowNumber || RowIndex_end >= SelfSize.RowNumber
        || ColIndex_start >= SelfSize.ColNumber || ColIndex_end >= SelfSize.ColNumber
        || RowIndex_start > RowIndex_end || ColIndex_start > ColIndex_end)
    {
        mdkError << "Invalid Input @ mdkMatrix::SubMatrix(RowIndex_start, RowIndex_end, ColIndex_start, ColIndex_end) const" << '\n';
        mdkShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    std::vector<uint64> RowIndexList(RowIndex_end - RowIndex_start + 1);

    for (uint64 i = RowIndex_start; i <= RowIndex_end; ++i)
    {
        RowIndexList[i - RowIndex_start] = i;
    }

    std::vector<uint64> ColIndexList(ColIndex_end - ColIndex_start + 1);

    for (uint64 j = ColIndex_start; j <= ColIndex_end; ++j)
    {
        ColIndexList[j - ColIndex_start] = j;
    }

    mdkShadowMatrix<ElementType> tempShadowMatrix(*this, RowIndexList, ColIndexList);

    return tempShadowMatrix;
}

// return SubMatrix as Matrix

template<typename ElementType>
inline
mdkMatrix<ElementType> mdkMatrix<ElementType>::GetSubMatrix(uint64 RowIndex_start, uint64 RowIndex_end,
                                                            uint64 ColIndex_start, uint64 ColIndex_end) const
{
    mdkMatrix<ElementType> tempMatrix;

    auto SelfSize = this->GetSize();

    if (RowIndex_start >= SelfSize.RowNumber || RowIndex_end >= SelfSize.RowNumber
        || ColIndex_start >= SelfSize.ColNumber || ColIndex_end >= SelfSize.ColNumber
        || RowIndex_start > RowIndex_end || ColIndex_start > ColIndex_end)
    {
        mdkError << "Invalid Input @ mdkMatrix::GetSubMatrix(RowIndex_start, RowIndex_end, ColIndex_start, ColIndex_end)" << '\n';
        
        return tempMatrix;
    }

    auto RowNumber = RowIndex_end - RowIndex_start + 1;
    auto ColNumber = ColIndex_end - ColIndex_start + 1;

    tempMatrix.Resize(RowNumber, ColNumber);

    auto tempRawPointer = tempMatrix.GetElementPointer();

    auto RawPointer = this->GetElementPointer();

    for (uint64 j = ColIndex_start; j <= ColIndex_end; ++j)
    {
        for (uint64 i = RowIndex_start; i <= RowIndex_end; ++i)
        {
            tempRawPointer[0] = RawPointer[j*SelfSize.RowNumber + i];

            ++tempRawPointer;
        }
    }

    return tempMatrix;
}


template<typename ElementType>
inline
mdkMatrix<ElementType> mdkMatrix<ElementType>::GetSubMatrix(const std::vector<uint64>& RowIndexList, const std::vector<uint64>& ColIndexList) const
{
    mdkMatrix<ElementType> tempMatrix; // empty matrix

    GetSubMatrix(tempMatrix, RowIndexList, ColIndexList);

    return tempMatrix;
}


template<typename ElementType>
inline
bool mdkMatrix<ElementType>::GetSubMatrix(mdkMatrix<ElementType>& OutputMatrix,
                                          const std::vector<uint64>& RowIndexList, 
                                          const std::vector<uint64>& ColIndexList) const
{
    if (this == &OutputMatrix)
    {
        mdkError << "A Matrix tries to get a SubMatrix of itself @ mdkMatrix::GetSubMatrix(OutputMatrix, RowIndexList, ColIndexList)" << '\n';
        return false;
    }

    auto RowNumber = uint64(RowIndexList.size());
    
    auto ColNumber = uint64(ColIndexList.size());

    if (RowNumber == 0 || ColNumber == 0)
    {
        mdkError << "RowIndexList or ColIndexList is empty @ mdkMatrix::GetSubMatrix(OutputMatrix, RowIndexList, ColIndexList)" << '\n';
        return false;
    }

    auto SelfSize = this->GetSize();

    for (uint64 j = 0; j < ColNumber; ++j)
    {
        if (ColIndexList[j] >= SelfSize.ColNumber)
        {
            mdkError << "Invalid ColIndexList @ mdkMatrix::GetSubMatrix(OutputMatrix, RowIndexList, ColIndexList)" << '\n';
            return false;
        }
    }

    for (uint64 i = 0; i < RowNumber; ++i)
    {
        if (RowIndexList[i] >= SelfSize.RowNumber)
        {
            mdkError << "Invalid RowIndexList @ mdkMatrix::GetSubMatrix(OutputMatrix, RowIndexList, ColIndexList)" << '\n';
            return false;
        }
    }

    if (OutputMatrix.GetColNumber() != ColNumber || OutputMatrix.GetRowNumber() != RowNumber)
    {
        if (OutputMatrix.IsSizeFixed() == false)
        {
            OutputMatrix.Clear();
            OutputMatrix.Resize(RowNumber, ColNumber);
        }
        else
        {
            mdkError << "Invalid OutputMatrix @ mdkMatrix::GetSubMatrix(OutputMatrix, RowIndexList, ColIndexList)" << '\n';
            return false;
        }
    }


    auto tempRawPointer = OutputMatrix.GetElementPointer();

    auto RawPointer = this->GetElementPointer();

    for (uint64 j = 0; j < ColNumber; ++j)
    {
        auto Index = ColIndexList[j] * SelfSize.RowNumber;

        for (uint64 i = 0; i < RowNumber; ++i)
        {
            uint64 LinearIndex = Index + RowIndexList[i];

            tempRawPointer[0] = RawPointer[LinearIndex];

            ++tempRawPointer;
        }
    }

    return true;
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> mdkMatrix<ElementType>::GetSubMatrix(const std::vector<uint64>& RowIndexList, const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol) const
{
    auto SelfSize = this->GetSize();

    std::vector<uint64> ColIndexList(SelfSize.ColNumber);

    for (uint64 i = 0; i < SelfSize.ColNumber; ++i)
    {
        ColIndexList[i] = i;
    }

    return this->GetSubMatrix(RowIndexList, ColIndexList);
}

template<typename ElementType>
inline
mdkMatrix<ElementType> mdkMatrix<ElementType>::GetSubMatrix(const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol, const std::vector<uint64>& ColIndexList) const
{
    auto SelfSize = this->GetSize();

    std::vector<uint64> RowIndexList(SelfSize.RowNumber);

    for (uint64 i = 0; i < SelfSize.RowNumber; ++i)
    {
        RowIndexList[i] = i;
    }

    return this->GetSubMatrix(RowIndexList, ColIndexList);
}


template<typename ElementType>
inline
mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::Col(uint64 ColIndex)
{
    return this->at(ALL, { ColIndex });

    /*
    if (ColIndex >= SelfSize.ColNumber)
    {
        mdkError << "Invalid Input @ mdkMatrix::Col(ColIndex)" << '\n';
        mdkShadowMatrix<ElementType> tempShadowMatrix;        
        return tempShadowMatrix;
    }

    mdkShadowMatrix<ElementType> tempShadowMatrix(*this, ALL, {ColIndex});

    return tempShadowMatrix;
    */
}


template<typename ElementType>
inline
mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::Col(std::initializer_list<uint64>& ColIndexList)
{
    return this->at(ALL, ColIndexList);

    /*
    if (ColIndexList.size() == 0)
    {
        mdkError << "ColIndexList is empty @ mdkMatrix::Col()(std::initializer_list {ColIndexList})" << '\n';
        mdkShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= SelfSize.ColNumber)
        {
            mdkError << "Invalid ColIndexList @ mdkMatrix::Col({ColIndexList})" << '\n';
            mdkShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    mdkShadowMatrix<ElementType> tempShadowMatrix(*this, ALL, ColIndexList);

    return tempShadowMatrix;
    */
}


template<typename ElementType>
inline
mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::Col(const std::vector<uint64>& ColIndexList)
{
    return this->at(ALL, ColIndexList);

    /*
    if (ColIndexList.size() == 0)
    {
        mdkError << "ColIndexList is empty @ mdkMatrix::operator()(std::vector {ColIndexList})" << '\n';
        mdkShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {        
        if (*it >= SelfSize.ColNumber)
        {
            mdkError << "Invalid ColIndexList @ mdkMatrix::Col(std::vector ColIndexList)" << '\n';
            mdkShadowMatrix<ElementType> tempShadowMatrix;            
            return tempShadowMatrix;
        }
    }

    mdkShadowMatrix<ElementType> tempShadowMatrix(*this, ALL, ColIndexList);

    return tempShadowMatrix;
    */
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> mdkMatrix<ElementType>::GetCol(uint64 ColIndex) const
{
	mdkMatrix<ElementType> tempMatrix;

    auto SelfSize = this->GetSize();

	if (ColIndex >= SelfSize.ColNumber)
	{
		mdkError << "Invalid Input @ mdkMatrix::GetCol(ColIndex)" << '\n';
        
        return tempMatrix;
	}

    tempMatrix.Resize(SelfSize.RowNumber, 1);

    auto tempRawPointer = tempMatrix.GetElementPointer();

	auto RawPointer this->GetElementPointer();

	uint64 Index = ColIndex*SelfSize.RowNumber;

	for (uint64 i = 0; i < SelfSize.RowNumber; ++i)
	{
		tempRawPointer[i] = RawPointer[Index + i];
	}

    return tempMatrix;
}


template<typename ElementType>
inline 
bool mdkMatrix<ElementType>::GetCol(uint64 ColIndex, std::vector<ElementType>& ColData) const
{
    auto SelfSize = this->GetSize();

	if (ColIndex >= SelfSize.ColNumber)
	{
		mdkError << "Invalid Input @ mdkMatrix::GetCol(uint64 ColIndex, std::vector<ElementType>& ColData)" << '\n';
		return false;
	}

	ColData.resize(SelfSize.RowNumber);

	auto tempRawPointer = ColData.data();

	auto RawPointer this->GetElementPointer();

	uint64 Index = ColIndex*SelfSize.RowNumber;

	for (uint64 i = 0; i < SelfSize.RowNumber; ++i)
	{
		tempRawPointer[i] = RawPointer[Index + i];
	}

	return true;
}


template<typename ElementType>
inline 
bool mdkMatrix<ElementType>::GetCol(uint64 ColIndex, ElementType* ColData) const
{
    auto SelfSize = this->GetSize();

	if (ColIndex >= SelfSize.ColNumber || ColData == nullptr)
	{
		mdkError << "Invalid Input @ mdkMatrix::GetCol(uint64 ColIndex, ElementType* ColData)" << '\n';
		return false;
	}

	auto RawPointer this->GetElementPointer();

	uint64 Index = ColIndex*SelfSize.RowNumber;

	for (uint64 i = 0; i < SelfSize.RowNumber; ++i)
	{
		ColData[i] = RawPointer[Index + i];
	}

	return true;
}


template<typename ElementType>
template<typename ElementType_Input>
inline 
bool mdkMatrix<ElementType>::SetCol(uint64 ColIndex, const mdkMatrix<ElementType_Input>& ColData)
{
    auto SelfSize = this->GetSize();

	if (ColIndex >= SelfSize.ColNumber)
	{
		mdkError << "Invalid Input : ColIndex is out of bound @ mdkMatrix::SetCol(ColIndex, mdkMatrix)" << '\n';
		return false;
	}

	auto Size = ColData.GetSize();

	if (Size.ColNumber != 1 || Size.RowNumber != 1)
	{
		mdkError << "Invalid Input : must be a vector @ mdkMatrix::SetCol(ColIndex, mdkMatrix)" << '\n';
		return false;
	}

	if (Size.RowNumber > 1 && Size.RowNumber != SelfSize.RowNumber)
	{
		mdkError << "Invalid Input : size does not match @ mdkMatrix::SetCol(ColIndex, mdkMatrix)" << '\n';
		return false;
	}

	if (Size.ColNumber > 1 && Size.ColNumber != SelfSize.RowNumber)
	{
		mdkError << "Invalid Input : size does not match @ mdkMatrix::SetCol(ColIndex, mdkMatrix)" << '\n';
		return false;
	}

    return this->SetCol(ColIndex, ColData.GetElementPointer(), std::max(Size.RowNumber, Size.ColNumber));
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool mdkMatrix<ElementType>::SetCol(uint64 ColIndex, const std::initializer_list<ElementType_Input>& ColData)
{
    return this->SetCol(ColIndex, ColData.begin(), ColData.size());
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool mdkMatrix<ElementType>::SetCol(uint64 ColIndex, const std::vector<ElementType_Input>& ColData)
{
    return this->SetCol(ColIndex, ColData.data(), ColData.size());
}


template<typename ElementType>
template<typename ElementType_Input>
inline 
bool mdkMatrix<ElementType>::SetCol(uint64 ColIndex, const ElementType_Input* ColData, uint64 Length)
{
    auto SelfSize = this->GetSize();

    if (ColData == nullptr || Length == 0 || ColIndex >= SelfSize.ColNumber)
	{
		mdkError << "Invalid Input @ mdkMatrix::SetCol(ColIndex, const ElementType_Input* ColData, uint64 Length)" << '\n';
		return false;
	}
    
    auto RawPointer = this->GetElementPointer();

    uint64 Index = ColIndex*SelfSize.RowNumber;

    // just for reference:
    //
    //for (uint64 i = 0; i < SelfSize.RowNumber; ++i)
    //{
    //	RawPointer[Index + i] = ElementType(ColData[i]);
    //}

    auto tempColData = ColData;

    RawPointer += Index;
    for (auto Ptr = RawPointer; Ptr < RawPointer + SelfSize.RowNumber; ++Ptr)
    {
        Ptr[0] = ElementType(tempColData[0]);

        ++tempColData;
    }

	return true;
}


template<typename ElementType>
inline 
bool mdkMatrix<ElementType>::FillCol(uint64 ColIndex, const ElementType& Element)
{
    auto SelfSize = this->GetSize();

    if (ColIndex >= SelfSize.ColNumber)
    {
        mdkError << "Invalid Input @ mdkMatrix::FillCol(uint64 ColIndex, const ElementType& Element)" << '\n';
        return false;
    }

    auto RawPointer = this->GetElementPointer();

    uint64 Index = ColIndex*SelfSize.RowNumber;

    RawPointer += Index;
    for (auto Ptr = RawPointer; Ptr < RawPointer + SelfSize.RowNumber; ++Ptr)
    {
        Ptr[0] = Element;
    }

    return true;
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool mdkMatrix<ElementType>::AppendCol(const mdkMatrix<ElementType_Input>& ColData)
{
    if (m_IsSizeFixed == true)
    {
        mdkError << "Matrix Size can not change @ mdkMatrix::AppendCol(const mdkMatrix<ElementType_Input>& ColData)" << '\n';
        return false;
    }

    if (ColData.IsEmpty() == true)
    {
        mdkError << "Empty Input @ mdkMatrix::AppendCol(const mdkMatrix<ElementType_Input>& ColData)" << '\n';
        return false;
    }

    auto SelfSize = this->GetSize();

    auto RowNumber = SelfSize.RowNumber;

    if (RowNumber == 0)
    {
        RowNumber = std::max(ColData.GetRowNumber(), ColData.GetColNumber());
    }

    this->Resize(RowNumber, SelfSize.ColNumber + 1);

    return this->SetCol(SelfSize.ColNumber, ColData);
}


template<typename ElementType>
template<typename ElementType_Input>
inline 
bool mdkMatrix<ElementType>::AppendCol(const std::initializer_list<ElementType_Input>& ColData)
{
    if (m_IsSizeFixed == true)
    {
        mdkError << "Matrix Size can not change @ mdkMatrix::AppendCol(const std::initializer_list<ElementType_Input>& ColData)" << '\n';
        return false;
    }

    if (ColData.size() == 0)
    {
        mdkError << "Empty Input @ mdkMatrix::AppendCol(const std::initializer_list<ElementType_Input>& ColData)" << '\n';
        return false;
    }

    auto SelfSize = this->GetSize();

    auto  RowNumber = SelfSize.RowNumber;

    if (RowNumber == 0)
    {
        RowNumber = ColData.size();
    }

    this->Resize(RowNumber, SelfSize.ColNumber + 1);

    return this->SetCol(SelfSize.ColNumber, ColData);
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool mdkMatrix<ElementType>::AppendCol(const std::vector<ElementType_Input>& ColData)
{
    if (m_IsSizeFixed == true)
    {
        mdkError << "Matrix Size can not change @ mdkMatrix::AppendCol(const std::vector<ElementType_Input>& ColData)" << '\n';
        return false;
    }

    if (ColData.size() == 0)
    {
        mdkError << "Empty Input @ mdkMatrix::AppendCol(const std::vector<ElementType_Input>& ColData)" << '\n';
        return false;
    }

    auto SelfSize = this->GetSize();

    auto  RowNumber = SelfSize.RowNumber;

    if (RowNumber == 0)
    {
        RowNumber = ColData.size();
    }

    this->Resize(RowNumber, SelfSize.ColNumber + 1);

    return this->SetCol(SelfSize.ColNumber, ColData);
}


template<typename ElementType>
template<typename ElementType_Input>
inline 
bool mdkMatrix<ElementType>::AppendCol(const ElementType_Input* ColData, uint64 Length)
{
    if (m_IsSizeFixed == true)
    {
        mdkError << "Matrix Size can not change @ mdkMatrix::AppendCol(const ElementType_Input* ColData, uint64 Length)" << '\n';
        return false;
    }

    if (ColData == nullptr || Length == 0)
    {
        mdkError << "Empty Input @ mdkMatrix::AppendCol(const ElementType_Input* ColData, uint64 Length)" << '\n';
        return false;
    }

    auto SelfSize = this->GetSize();

    auto  RowNumber = SelfSize.RowNumber;

    if (RowNumber == 0)
    {
        RowNumber = Length;
    }

    this->Resize(RowNumber, SelfSize.ColNumber + 1);

    return this->SetCol(SelfSize.ColNumber, ColData, Length);
}


template<typename ElementType>
inline 
bool mdkMatrix<ElementType>::DeleteCol(uint64 ColIndex)
{
    if (m_IsSizeFixed == true)
    {
        mdkError << "Matrix Size can not change @ mdkMatrix::DeleteCol(uint64 ColIndex)" << '\n';
        return false;
    }

    auto SelfSize = this->GetSize();

    if (ColIndex >= SelfSize.ColNumber)
    {
        mdkError << "Invalid Input @ mdkMatrix::DeleteCol(uint64 ColIndex)" << '\n';
        return false;
    }

    m_CoreData->DataArray.erase(m_CoreData->DataArray.begin() + ColIndex*SelfSize.RowNumber, 
                                m_CoreData->DataArray.begin() + (ColIndex + 1)*SelfSize.RowNumber);

    m_CoreData->ColNumber -= 1;

    m_ElementPointer = m_CoreData->DataArray.data();

    return true;
}


template<typename ElementType>
inline
bool mdkMatrix<ElementType>::DeleteCol(std::initializer_list<uint64>& ColIndexList)
{
    if (m_IsSizeFixed == true)
    {
        mdkError << "Matrix Size can not change @ mdkMatrix::DeleteCol(std::initializer_list<uint64>& ColIndexList)" << '\n';
        return false;
    }

    if (ColIndexList.size() == 0)
    {
        mdkError << "Empty Input @ mdkMatrix::DeleteCol(std::initializer_list ColIndexList)" << '\n';
        return false;
    }

    auto SelfSize = this->GetSize();

    for (uint64 i = 0; i < ColIndexList.size(); ++i)
    {
        if (ColIndexList[i] >= SelfSize.ColNumber)
        {
            mdkError << "Out of bound Input @ mdkMatrix::DeleteCol(std::initializer_list ColIndexList)" << '\n';
            return false;
        }
    }

    return this->DeleteCol(ColIndexList.begin(), ColIndexList.size());
}


template<typename ElementType>
inline
bool mdkMatrix<ElementType>::DeleteCol(const std::vector<uint64>& ColIndexList)
{
    if (m_IsSizeFixed == true)
    {
        mdkError << "Matrix Size can not change @ mdkMatrix::DeleteCol(const std::vector<uint64>& ColIndexList)" << '\n';
        return false;
    }

    if (ColIndexList.size() == 0)
    {
        mdkError << "Empty Input @ mdkMatrix::DeleteCol(std::vector ColIndexList)" << '\n';
        return false;
    }

    auto SelfSize = this->GetSize();

    for (uint64 i = 0; i < ColIndexList.size(); ++i)
    {
        if (ColIndexList[i] >= SelfSize.ColNumber)
        {
            mdkError << "Out of bound Input @ mdkMatrix::DeleteCol(std::vector ColIndexList)" << '\n';
            return false;
        }
    }

    return this->DeleteCol(ColIndexList.data(), ColIndexList.size());
}


template<typename ElementType>
inline
bool mdkMatrix<ElementType>::DeleteCol(const uint64* ColIndexListPtr, uint64 Length)
{
    if (m_IsSizeFixed == true)
    {
        mdkError << "Matrix Size can not change @ mdkMatrix::DeleteCol(const uint64* ColIndexPtr, uint64 Length)" << '\n';
        return false;
    }

    if (ColIndexListPtr == nullptr || Length == 0)
    {
        mdkError << "Empty Input @ mdkMatrix::DeleteCol(const uint64* ColIndexPtr, uint64 Length)" << '\n';
        return false;
    }

    if (Length == 1)
    {
        return this->DeleteCol(ColIndexListPtr[0]);
    }

    // Length > 1 -------------------------------------------

    auto SelfSize = this->GetSize();

    std::vector<uint64> ColIndexList_max_to_min(Length);

    for (uint64 i = 0; i < Length; ++i)
    {
        ColIndexList_max_to_min[i] = ColIndexListPtr[i];
    }

    std::sort(ColIndexList_max_to_min.begin(), ColIndexList_max_to_min.end(), [](uint64 a, uint64 b) { return a > b; });

    uint64 Index_prev = ColIndexList_max_to_min[0] + 1;

    for (uint64 i = 0; i < ColIndexList_max_to_min.size(); ++i)
    {
        auto Index_i = ColIndexList_max_to_min[i];

        if (Index_i == Index_prev)
        {
            mdkWarning << "duplicate Input @ mdkMatrix::DeleteCol(const uint64* ColIndexPtr, uint64 Length)" << '\n';
        }
        else
        {
            m_CoreData->DataArray.erase(m_CoreData->DataArray.begin() + Index_i * SelfSize.RowNumber,
                                        m_CoreData->DataArray.begin() + (Index_i + 1)* SelfSize.RowNumber);

            Index_prev = Index_i;

            m_CoreData->ColNumber -= 1;
        }
    }

    m_ElementPointer = m_CoreData->DataArray.data();

    return true;
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool mdkMatrix<ElementType>::InsertCol(uint64 ColIndex, const mdkMatrix<ElementType_Input>& ColData)
{
    if (ColIndex >= SelfSize.ColNumber)
    {
        mdkError << "Invalid Input : ColIndex is out of bound @ mdkMatrix::InsertCol(ColIndex, mdkMatrix)" << '\n';
        return false;
    }

    auto InputSize = ColData.GetSize();

    if (InputSize.ColNumber != 1 || InputSize.RowNumber != 1)
    {
        mdkError << "Invalid Input : must be a vector @ mdkMatrix::InsertCol(ColIndex, mdkMatrix)" << '\n';
        return false;
    }

    auto SelfSize = this->GetSize();

    if (InputSize.RowNumber > 1 && InputSize.RowNumber != SelfSize.RowNumber)
    {
        mdkError << "Invalid Input : size does not match @ mdkMatrix::InsertCol(ColIndex, mdkMatrix)" << '\n';
        return false;
    }

    if (InputSize.ColNumber > 1 && InputSize.ColNumber != SelfSize.RowNumber)
    {
        mdkError << "Invalid Input : size does not match @ mdkMatrix::InsertCol(ColIndex, mdkMatrix)" << '\n';
        return false;
    }

    return this->InsertCol(ColIndex, ColData.GetElementPointer(), std::max(InputSize.RowNumber, InputSize.ColNumber));

}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool mdkMatrix<ElementType>::InsertCol(uint64 ColIndex, const std::initializer_list<ElementType_Input>& ColData)
{
    return this->InsertCol(ColIndex, ColData.begin(), ColData.size());
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool mdkMatrix<ElementType>::InsertCol(uint64 ColIndex, const std::vector<ElementType_Input>& ColData)
{
    return this->InsertCol(ColIndex, ColData.data(), ColData.size());
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool mdkMatrix<ElementType>::InsertCol(uint64 ColIndex, const ElementType_Input* ColData, uint64 Length)
{
    auto SelfSize = this->GetSize();

    if (ColData == nullptr || Length == 0 || ColIndex >= SelfSize.ColNumber)
    {
        mdkError << "Invalid Input @ mdkMatrix::InsertCol(ColIndex, const ElementType_Input* ColData, uint64 Length)" << '\n';
        return false;
    }

    m_CoreData->DataArray.insert(m_CoreData->DataArray.begin() + ColIndex*SelfSize.RowNumber, ColData, ColData + Length);

    m_CoreData->ColNumber += 1;

    m_ElementPointer = m_CoreData->DataArray.data();

    return true;
}


template<typename ElementType>
inline
mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::Row(uint64 RowIndex)
{
    return this->at({RowIndex}, ALL);
}


template<typename ElementType>
inline
mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::Row(std::initializer_list<uint64>& RowIndexList)
{
    return this->at(RowIndexList, ALL);
}


template<typename ElementType>
inline
mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::Row(const std::vector<uint64>& RowIndexList)
{
    return this->at(RowIndexList, ALL);
}


template<typename ElementType>
inline
mdkMatrix<ElementType> mdkMatrix<ElementType>::GetRow(uint64 RowIndex) const
{
    mdkMatrix<ElementType> tempMatrix;	

    auto SelfSize = this->GetSize();

	if (RowIndex >= SelfSize.RowNumber)
	{
		mdkError << "Invalid Input @ mdkMatrix::GetRow(RowIndex)" << '\n';
        
        return tempMatrix;
	}

    tempMatrix.Resize(1, SelfSize.ColNumber);

    auto tempRawPointer = tempMatrix.GetElementPointer();

    auto RawPointer = this->GetElementPointer();

	uint64 Index = 0;

	for (uint64 j = 0; j < SelfSize.ColNumber; ++j)
	{
		tempRawPointer[j] = RawPointer[Index + RowIndex];

		Index += SelfSize.RowNumber;
	}

    return tempMatrix;
}


template<typename ElementType>
inline
bool mdkMatrix<ElementType>::GetRow(uint64 RowIndex, std::vector<ElementType>& RowData) const
{
    auto SelfSize = this->GetSize();

	if (RowIndex >= SelfSize.RowNumber)
	{
		mdkError << "Invalid Input @ mdkMatrix::GetRow(RowIndex, std::vector<ElementType>& RowData)" << '\n';
		return false;
	}

	RowData.resize(SelfSize.ColNumber);

	auto tempRawPointer = RowData.data();

    auto RawPointer = this->GetElementPointer();

	uint64 Index = 0;

	for (uint64 j = 0; j < SelfSize.ColNumber; ++j)
	{
		tempRawPointer[j] = RawPointer[Index + RowIndex];

		Index += SelfSize.RowNumber;
	}

	return true;
}


template<typename ElementType>
inline
bool mdkMatrix<ElementType>::GetRow(uint64 RowIndex, ElementType* RowData) const
{
    auto SelfSize = this->GetSize();

	if (RowIndex >= SelfSize.RowNumber)
	{
		mdkError << "Invalid Input @ mdkMatrix GetRow(RowIndex, ElementType* RowData)" << '\n';
		return false;
	}

	auto RawPointer this->GetElementPointer();

	uint64 Index = 0;

	for (uint64 j = 0; j < SelfSize.ColNumber; ++j)
	{
		RowData[j] = RawPointer[Index + RowIndex];

		Index += SelfSize.RowNumber;
	}

	return true;
}


template<typename ElementType>
template<typename ElementType_Input>
inline 
bool mdkMatrix<ElementType>::SetRow(uint64 RowIndex, const mdkMatrix<ElementType_Input>& RowData)
{
    auto SelfSize = this->GetSize();

    if (RowIndex >= SelfSize.RowNumber)
    {
        mdkError << "Invalid Input @ mdkMatrix SetRow(uint64 RowIndex, const mdkMatrix<ElementType_Input>& RowData)" << '\n';
        return false;
    }

	auto InputSize = RowData.GetSize();

    if (InputSize.ColNumber != 1 || InputSize.RowNumber != 1)
	{
		mdkError << "Invalid Input @ mdkMatrix SetRow(RowIndex,mdkMatrix)" << '\n';
		return false;
	}

    if (InputSize.RowNumber > 1 && InputSize.RowNumber != SelfSize.ColNumber)
	{
		mdkError << "Invalid Input @ mdkMatrix SetRow(RowIndex,mdkMatrix)" << '\n';
		return false;
	}

    if (InputSize.ColNumber > 1 && InputSize.ColNumber != SelfSize.ColNumber)
	{
		mdkError << "Invalid Input @ mdkMatrix SetRow(RowIndex,mdkMatrix)" << '\n';
		return false;
	}

    return this->SetRow(RowIndex, RowData.GetElementPointer(), std::max(InputSize.RowNumber, InputSize.ColNumber));
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool mdkMatrix<ElementType>::SetRow(uint64 RowIndex, const std::initializer_list<ElementType_Input>& RowData)
{
    return this->SetRow(RowIndex, RowData.begin(), RowData.size());
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool mdkMatrix<ElementType>::SetRow(uint64 RowIndex, const std::vector<ElementType_Input>& RowData)
{
    return this->SetRow(RowIndex, RowData.data(), RowData.size());
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool mdkMatrix<ElementType>::SetRow(uint64 RowIndex, const ElementType_Input* RowData, uint64 Length)
{
    auto SelfSize = this->GetSize();

    if (RowData == nullptr || Length == 0 || RowIndex >= SelfSize.RowNumber)
    {
        mdkError << "Empty Input @ mdkMatrix::SetRow(RowIndex, const ElementType_Input* RowData, uint64 Length)" << '\n';
        return false;
    }

    if (Length != SelfSize.ColNumber)
    {
        mdkError << "Length does not match @ mdkMatrix::SetRow(RowIndex, const ElementType_Input* RowData, uint64 Length)" << '\n';
        return false;
    }

    auto RawPointer = this->GetElementPointer();

    uint64 Index = 0;

    for (uint64 j = 0; j < SelfSize.ColNumber; ++j)
    {
        RawPointer[Index + RowIndex] = ElementType(RowData[j]);

        Index += SelfSize.RowNumber;
     }

    return true;
}


template<typename ElementType>
inline 
bool mdkMatrix<ElementType>::FillRow(uint64 RowIndex, const ElementType& Element)
{
    auto SelfSize = this->GetSize();

    if (RowIndex >= SelfSize.RowNumber)
    {
        mdkError << "Invalid Input @ mdkMatrix::FillRow(uint64 RowIndex, const ElementType& Element)" << '\n';
        return false;
    }

    auto RawPointer = this->GetElementPointer();

    uint64 Index = 0;

    for (uint64 j = 0; j < SelfSize.ColNumber; ++j)
    {
        RawPointer[Index + RowIndex] = Element;

        Index += SelfSize.RowNumber;
    }

    return true;
}


template<typename ElementType>
template<typename ElementType_Input>
inline 
bool mdkMatrix<ElementType>::AppendRow(const mdkMatrix<ElementType_Input>& RowData)
{
    if (m_IsSizeFixed == true)
    {
        mdkError << "Size can not change @ mdkMatrix::AppendRow(const mdkMatrix<ElementType_Input>& RowData)" << '\n';
        return false;
    }

    if (RowData.IsEmpty() == true)
    {
        mdkError << "Input is empty @ mdkMatrix::AppendRow(const mdkMatrix<ElementType_Input>& RowData)" << '\n';
        return false;
    }

    auto SelfSize = this->GetSize();

    auto  ColNumber = SelfSize.ColNumber;

    if (ColNumber == 0)
    {
        ColNumber = std::max(RowData.GetRowNumber(), RowData.GetColNumber);
    }

    this->Resize(SelfSize.RowNumber + 1, ColNumber);

    return this->SetRow(SelfSize.RowNumber, RowData);
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool  mdkMatrix<ElementType>::AppendRow(const std::initializer_list<ElementType_Input>& RowData)
{
    if (m_IsSizeFixed == true)
    {
        mdkError << "Size can not change @ mdkMatrix::AppendRow(const std::initializer_list<ElementType_Input>& RowData)" << '\n';
        return false;
    }

    if (RowData.size() == 0)
    {
        mdkError << "Input is empty @ mdkMatrix::AppendRow(const std::initializer_list<ElementType_Input>& RowData)" << '\n';
        return false;
    }

    auto SelfSize = this->GetSize();

    auto  ColNumber = SelfSize.ColNumber;

    if (ColNumber == 0)
    {
        ColNumber = RowData.size();
    }

    this->Resize(SelfSize.RowNumber + 1, ColNumber);

    return this->SetRow(SelfSize.RowNumber, RowData);
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool  mdkMatrix<ElementType>::AppendRow(const std::vector<ElementType_Input>& RowData)
{
    if (m_IsSizeFixed == true)
    {
        mdkError << "Size can not change @ mdkMatrix::AppendRow(const std::vector<ElementType_Input>& RowData)" << '\n';
        return false;
    }

    if (RowData.size() == 0)
    {
        mdkError << "Input is empty @ mdkMatrix::AppendRow(const std::vector<ElementType_Input>& RowData)" << '\n';
        return false;
    }

    auto SelfSize = this->GetSize();

    auto  ColNumber = SelfSize.ColNumber;

    if (ColNumber == 0)
    {
        ColNumber = RowData.size();
    }

    this->Resize(SelfSize.RowNumber + 1, ColNumber);

    return this->SetRow(SelfSize.RowNumber, RowData);
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool mdkMatrix<ElementType>::AppendRow(const ElementType_Input* RowData, uint64 Length)
{
    if (m_IsSizeFixed == true)
    {
        mdkError << "Size can not change @ mdkMatrix::AppendRow(const ElementType_Input* RowData, uint64 Length)" << '\n';
        return false;
    }

    if (RowData == nullptr || Length == 0)
    {
        mdkError << "Input is empty @ mdkMatrix::AppendRow(const ElementType_Input* RowData, uint64 Length)" << '\n';
        return false;
    }

    auto SelfSize = this->GetSize();

    auto  ColNumber = SelfSize.ColNumber;

    if (ColNumber == 0)
    {
        ColNumber = RowData.size();
    }

    this->Resize(SelfSize.RowNumber + 1, ColNumber);

    return this->SetRow(SelfSize.RowNumber, RowData, Length);
}


template<typename ElementType>
inline
bool mdkMatrix<ElementType>::DeleteRow(uint64 RowIndex)
{
    if (m_IsSizeFixed == true)
    {
        mdkError << "Size can not change @ mdkMatrix::DeleteRow(uint64 RowIndex)" << '\n';
        return false;
    }

    auto SelfSize = this->GetSize();

    if (RowIndex >= SelfSize.RowNumber)
    {
        mdkError << "Invalid Input @ mdkMatrix::DeleteRow(uint64 RowIndex)" << '\n';
        return false;
    }

    return this->DeleteRow(&RowIndex, 1);
}


template<typename ElementType>
inline
bool mdkMatrix<ElementType>::DeleteRow(const std::vector<uint64>& RowIndexList)
{
    if (m_IsSizeFixed == true)
    {
        mdkError << "Size can not change @ mdkMatrix::DeleteRow(const std::vector<uint64>& RowIndexList)" << '\n';
        return false;
    }

    auto SelfSize = this->GetSize();

    if (RowIndexList.size() == 0 || RowIndexList.size() > SelfSize.RowNumber)
    {
        mdkError << "Invalid Input @ mdkMatrix::DeleteRow(const std::vector<uint64>& RowIndexList)" << '\n';
        return false;
    }

    return this->DeleteRow(RowIndexList.data(), RowIndexList.size());
}


template<typename ElementType>
inline
bool mdkMatrix<ElementType>::DeleteRow(const uint64* RowIndexListPtr, uint64 Length)
{
    if (m_IsSizeFixed == true)
    {
        mdkError << "Size can not change @ mdkMatrix::DeleteRow(const uint64* RowIndexPtr, uint64 Length)" << '\n';
        return false;
    }

    if (RowIndexListPtr == nullptr || Length == 0)
    {
        mdkError << "Invalid Input @ mdkMatrix::DeleteRow(const uint64* RowIndexPtr, uint64 Length)" << '\n';
        return false;
    }

    auto SelfSize = this->GetSize();

    std::vector<uint64> CounterList(SelfSize.RowNumber);
    
    for (uint64 i = 0; i < SelfSize.RowNumber; ++i)
    {
        CounterList[i] = 0;
    }

    for (uint64 i = 0; i < Length; ++i)
    {
        CounterList[RowIndexListPtr[i]] += 1;
    }

    std::vector<uint64> RowIndexList_output;

    for (uint64 i = 0; i < SelfSize.RowNumber; ++i)
    {
        if (CounterList[i] == 0)
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
        this->Reset();
    }

    return true;
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool mdkMatrix<ElementType>::InsertRow(uint64 RowIndex, const mdkMatrix<ElementType_Input>& RowData)
{
    if (m_IsSizeFixed == true)
    {
        mdkError << "Size can not change @ mdkMatrix::InsertRow(uint64 RowIndex, const mdkMatrix<ElementType_Input>& RowData)" << '\n';
        return false;
    }

    if (RowIndex >= SelfSize.RowNumber)
    {
        mdkError << "Invalid Input @ mdkMatrix InsertRow(uint64 RowIndex, const mdkMatrix<ElementType_Input>& RowData)" << '\n';
        return false;
    }

    auto InputSize = RowData.GetSize();

    auto SelfSize = this->GetSize();

    if (InputSize.ColNumber != 1 || InputSize.RowNumber != 1)
    {
        mdkError << "Invalid Input @ mdkMatrix InsertRow(RowIndex,mdkMatrix)" << '\n';
        return false;
    }

    if (InputSize.RowNumber > 1 && InputSize.RowNumber != SelfSize.ColNumber)
    {
        mdkError << "Invalid Input @ mdkMatrix InsertRow(RowIndex,mdkMatrix)" << '\n';
        return false;
    }

    if (InputSize.ColNumber > 1 && InputSize.ColNumber != SelfSize.ColNumber)
    {
        mdkError << "Invalid Input @ mdkMatrix InsertRow(RowIndex,mdkMatrix)" << '\n';
        return false;
    }

    return this->InsertRow(RowIndex, RowData.GetElementPointer(), std::max(Size.RowNumber, Size.ColNumber));
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool mdkMatrix<ElementType>::InsertRow(uint64 RowIndex, const std::initializer_list<ElementType_Input>& RowData)
{
    return this->InsertRow(RowIndex, RowData.begin(), RowData.size());
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool mdkMatrix<ElementType>::InsertRow(uint64 RowIndex, const std::vector<ElementType_Input>& RowData)
{
    return this->InsertRow(RowIndex, RowData.data(), RowData.size());
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool mdkMatrix<ElementType>::InsertRow(uint64 RowIndex, const ElementType_Input* RowData, uint64 Length)
{
    if (m_IsSizeFixed == true)
    {
        mdkError << "Size can not change @ mdkMatrix::InsertRow(RowIndex, const ElementType_Input* RowData, uint64 Length)" << '\n';
        return false;
    }

    auto SelfSize = this->GetSize();

    if (RowData == nullptr || Length == 0 || RowIndex >= SelfSize.RowNumber)
    {
        mdkError << "Empty Input @ mdkMatrix::InsertRow(RowIndex, const ElementType_Input* RowData, uint64 Length)" << '\n';
        return false;
    }

    if (Length != SelfSize.ColNumber)
    {
        mdkError << "Length does not match @ mdkMatrix::InsertRow(RowIndex, const ElementType_Input* RowData, uint64 Length)" << '\n';
        return false;
    }

    mdkMatrix<ElementType> tempMatrix(SelfSize.RowNumber + 1, SelfSize.ColNumber);

    auto tempRawPointer = tempMatrix.GetElementPointer();

    auto RawPointer = this->GetElementPointer();

    for (uint64 i = 0; i < RowIndex; ++i)
    {
        for (uint64 j = 0; j < SelfSize.ColNumber; ++j)
        {
            tempRawPointer[j*(SelfSize.RowNumber + 1) + i] = RawPointer[j*SelfSize.RowNumber + i];
        }
    }

    for (uint64 j = 0; j < SelfSize.ColNumber; ++j)
    {
        tempRawPointer[j*(SelfSize.RowNumber + 1) + RowIndex] = ElementType(RowData[j]);
    }

    for (uint64 i = RowIndex + 1; i < SelfSize.RowNumber + 1; ++i)
    {
        for (uint64 j = 0; j < SelfSize.ColNumber; ++j)
        {
            tempRawPointer[j*(SelfSize.RowNumber + 1) + i] = RawPointer[j*SelfSize.RowNumber + i - 1];
        }
    }

    this->ForceClear();

    this->Take(tempMatrix);

    return true;
}


template<typename ElementType>
inline
mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::Diangonal()
{
    auto SelfSize = this->GetSize();

    if (this->IsEmpty() == true || SelfSize.RowNumber != SelfSize.ColNumber)
    {
        mdkError << " Self is empty or not square @ mdkMatrix::Diangonal" << '\n';
        mdkShadowMatrix<ElementType> tempShadowMatrix;     
        return  tempShadowMatrix;
    }

    std::vector<uint64> LinearIndexList(SelfSize.ColNumber);

    uint64 Index = 0;

    for (uint64 i = 0; i < SelfSize.RowNumber; ++i)
    {
        LinearIndexList[j] = Index + i;

        Index += SelfSize.RowNumber;
    }

    mdkShadowMatrix<ElementType> tempShadowMatrix(*this, LinearIndexList);

    return tempShadowMatrix;
}


template<typename ElementType>
inline
mdkMatrix<ElementType> mdkMatrix<ElementType>::GetDiangonal() const
{
    mdkMatrix<ElementType> tempMatrix; 

    auto SelfSize = this->GetSize();

    if (SelfSize.RowNumber == 0 || SelfSize.RowNumber != SelfSize.ColNumber)
    {
        mdkError << " Self is empty or not square @ mdkMatrix::GetDiangonal()" << '\n';
        
        return  tempMatrix;
    }

    tempMatrix.Resize(SelfSize.RowNumber, 1);

    auto tempRawPointer = tempMatrix.GetElementPointer();

    this->GetDiangonal(tempRawPointer);

    return tempMatrix;
}


template<typename ElementType>
inline
bool mdkMatrix<ElementType>::GetDiangonal(std::vector<ElementType>& DiangonalData) const
{
    auto SelfSize = this->GetSize();

    if (SelfSize.RowNumber == 0 || SelfSize.RowNumber != SelfSize.ColNumber)
    {
        mdkError << " Self is empty or not square @ mdkMatrix::GetDiangonal(std::vector)" << '\n';
        return  false;
    }

    DiangonalData.resize(SelfSize.RowNumber);

    auto outputRawPointer = DiangonalData.data();

    return this->GetDiangonal(outputRawPointer);
}


template<typename ElementType>
inline
bool mdkMatrix<ElementType>::GetDiangonal(ElementType* DiangonalData) const
{
    if (DiangonalData == nullptr)
    {
        mdkError << " Input is nullptr @ mdkMatrix::GetDiangonal(ElementType* DiangonalData)" << '\n';
        return  false;
    }

    auto SelfSize = this->GetSize();

    if (SelfSize.RowNumber == 0 || SelfSize.RowNumber != SelfSize.ColNumber)
    {
        mdkError << " Self is empty or not square @ mdkMatrix::GetDiangonal(ElementType* DiangonalData)" << '\n';
        return  false;
    }

    auto RawPointer this->GetElementPointer();

    uint64 Index = 0;

    for (uint64 i = 0; i < SelfSize.RowNumber; ++i)
    {
        DiangonalData[j] = RawPointer[Index + i];

        Index += SelfSize.RowNumber;
    }

    return true;
}


template<typename ElementType>
template<typename ElementType_Input>
inline 
bool mdkMatrix<ElementType>::SetDiangonal(const mdkMatrix<ElementType_Input>& DiangonalData)
{
	auto InputSize = DiangonalData.GetSize();

    if (InputSize.ColNumber != 1 || InputSize.RowNumber != 1)
	{
		mdkError << "Invalid Input @ mdkMatrix::SetDiangonal(Matrix)" << '\n';
		return false;
	}

    auto SelfSize = this->GetSize();

    if (InputSize.RowNumber > 1 && InputSize.RowNumber != SelfSize.ColNumber)
	{
		mdkError << "Invalid Input @ mdkMatrix::SetDiangonal(Matrix)" << '\n';
		return false;
	}

    if (InputSize.ColNumber > 1 && InputSize.ColNumber != SelfSize.ColNumber)
	{
		mdkError << "Invalid Input @ mdkMatrix::SetDiangonal(Matrix)" << '\n';
		return false;
	}

    if (InputSize.RowNumber == 1 && InputSize.ColNumber == 1)
    {
        mdkWarning << "Input matrix is 1x1 and treated as a scalar @ mdkMatrix::SetDiangonal(Matrix)" << '\n';
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
bool mdkMatrix<ElementType>::SetDiangonal(const std::initializer_list<ElementType_Input>& DiangonalData)
{
    auto SelfSize = this->GetSize();

    if (SelfSize.RowNumber == 0 || SelfSize.RowNumber != SelfSize.ColNumber)
    {
        mdkError << "Self is empty or not square @ mdkMatrix::SetDiangonal({DiangonalData})" << '\n';
        return false;
    }

    if (DiangonalData.size() != SelfSize.RowNumber)
    {
        mdkError << "Invalid Input @ mdkMatrix::SetDiangonal({DiangonalData})" << '\n';
        return false;
    }

    return this->SetDiangonal(DiangonalData.begin());
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool mdkMatrix<ElementType>::SetDiangonal(const std::vector<ElementType_Input>& DiangonalData)
{
    auto SelfSize = this->GetSize();

    if (SelfSize.RowNumber == 0 || SelfSize.RowNumber != SelfSize.ColNumber)
	{
		mdkError << " Self is empty or not square @ mdkMatrix::SetDiangonal(std::vector)" << '\n';
		return false;
	}

    if (DiangonalData.size() != SelfSize.RowNumber)
    {
        mdkError << " Input is invalid @ mdkMatrix::SetDiangonal(std::vector)" << '\n';
        return false;
    }

	return this->SetDiangonal(DiangonalData.data());
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool mdkMatrix<ElementType>::SetDiangonal(const ElementType_Input* DiangonalData)
{
    if (DiangonalData == nullptr)
    {
        mdkError << " Input is nullptr @ mdkMatrix::SetDiangonal(const ElementType_Input* DiangonalData)" << '\n';
        return false;
    }

    auto SelfSize = this->GetSize();

    if (SelfSize.RowNumber == 0 || SelfSize.RowNumber != SelfSize.ColNumber)
    {
        mdkError << " Self is empty or not square @ mdkMatrix::SetDiangonal(const ElementType_Input* DiangonalData)" << '\n';
        return false;
    }

    auto RawPointer = this->GetElementPointer();

    uint64 Index = 0;

    for (uint64 i = 0; i < SelfSize.RowNumber; ++i)
    {
        RawPointer[Index + i] = ElementType(DiangonalData[i]);

        Index += SelfSize.RowNumber;
    }

    return true;
}


template<typename ElementType>
inline
bool mdkMatrix<ElementType>::FillDiangonal(const ElementType& Element)
{
    auto SelfSize = this->GetSize();

    if (SelfSize.RowNumber == 0 || SelfSize.RowNumber != SelfSize.ColNumber)
    {
        mdkError << " Self is empty or not square @ mdkMatrix::FillDiangonal" << '\n';
        return false;
    }

    auto RawPointer this->GetElementPointer();

    uint64 Index = 0;

    for (uint64 i = 0; i < SelfSize.RowNumber; ++i)
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
mdkMatrix<ElementType> operator+(const mdkMatrix<ElementType>& MatrixA, const mdkMatrix<ElementType>& MatrixB)
{
    return MatrixAdd(MatrixA, MatrixB);
}


template<typename ElementType>
inline
mdkMatrix<ElementType> operator-(const mdkMatrix<ElementType>& MatrixA, const mdkMatrix<ElementType>& MatrixB)
{
    return MatrixSubtract(MatrixA, MatrixB);
}


template<typename ElementType>
inline
mdkMatrix<ElementType> operator*(const mdkMatrix<ElementType>& MatrixA, const mdkMatrix<ElementType>& MatrixB)
{
    return MatrixMultiply(MatrixA, MatrixB);
}


template<typename ElementType>
inline
mdkMatrix<ElementType> operator/(const mdkMatrix<ElementType>& MatrixA, const mdkMatrix<ElementType>& MatrixB)
{
    return MatrixElementDivide(MatrixA, MatrixB);
}

// ----------------------- Matrix {+ - * /}  Element ------------------------------------------------//

template<typename ElementType>
inline mdkMatrix<ElementType> operator+(const mdkMatrix<ElementType>& MatrixA, const ElementType& ElementB)
{
    return MatrixAdd(MatrixA, ElementB);
}


template<typename ElementType>
inline mdkMatrix<ElementType> operator-(const mdkMatrix<ElementType>& MatrixA, const ElementType& ElementB)
{
    return MatrixSubtract(MatrixA, ElementB);
}


template<typename ElementType>
inline mdkMatrix<ElementType> operator*(const mdkMatrix<ElementType>& MatrixA, const ElementType& ElementB)
{
    return MatrixMultiply(MatrixA, ElementB);
}


template<typename ElementType>
inline mdkMatrix<ElementType> operator/(const mdkMatrix<ElementType>& MatrixA, const ElementType& ElementB)
{
    return MatrixElementDivide(MatrixA, ElementB);
}

// ----------------------- Element {+ - * /} Matrix ------------------------------------------------//

template<typename ElementType>
inline mdkMatrix<ElementType> operator+(const ElementType& ElementA, const mdkMatrix<ElementType>& MatrixB)
{
    return MatrixAdd(ElementA, MatrixB);
}


template<typename ElementType>
inline mdkMatrix<ElementType> operator-(const ElementType& ElementA, const mdkMatrix<ElementType>& MatrixB)
{
    return MatrixSubtract(ElementA, MatrixB);
}


template<typename ElementType>
inline mdkMatrix<ElementType> operator*(const ElementType& ElementA, const mdkMatrix<ElementType>& MatrixB)
{
    return MatrixMultiply(ElementA, MatrixB);
}


template<typename ElementType>
inline mdkMatrix<ElementType> operator/(const ElementType& ElementA, const mdkMatrix<ElementType>& MatrixB)
{
    return MatrixElementDivide(ElementA, MatrixB);
}

*/
//===========================================================================================================================//


//---------------------- Matrix {+= -= *= /=} Matrix ----------------------------------------//

template<typename ElementType>
inline
void mdkMatrix<ElementType>::operator+=(const mdkMatrix<ElementType>& InputMatrix)
{
    MatrixAdd(*this, *this, InputMatrix);
}


template<typename ElementType>
inline
void mdkMatrix<ElementType>::operator-=(const mdkMatrix<ElementType>& InputMatrix)
{
    MatrixSubtract(*this, *this, InputMatrix);
}


template<typename ElementType>
inline
void mdkMatrix<ElementType>::operator*=(const mdkMatrix<ElementType>& InputMatrix)
{
    MatrixMultiply(*this, *this, InputMatrix);
}


template<typename ElementType>
inline
void mdkMatrix<ElementType>::operator/=(const mdkMatrix<ElementType>& InputMatrix)
{
    MatrixElementDivide(*this, *this, InputMatrix);
}

//---------------------- Matrix {+= -= *= /=} mdkShadowMatrix ----------------------------------------//

template<typename ElementType>
inline 
void mdkMatrix<ElementType>::operator+=(const mdkShadowMatrix<ElementType>& ShadowMatrix)
{
    auto Size = ShadowMatrix.GetSize();

    if (Size.RowNumber == 0)
    {
        mdkError << "ShadowMatrix is empty @ mdkMatrix::operator+=(ShadowMatrix)" << '\n';
        return;
    }

    if (Size.RowNumber == 1 && Size.ColNumber == 1)
    {
        (*this) += ShadowMatrix[0];

        return;
    }

    if (Size.RowNumber != SelfSize.RowNumber || Size.ColNumber != SelfSize.ColNumber)
    {
        mdkError << "Size does not match @ mdkMatrix::operator+=(ShadowMatrix)" << '\n';
        return;
    }

    auto RawPointer this->GetElementPointer();

    for (uint64 i = 0; i < Self_ElementNumber; ++i)
    {
        RawPointer[i] += ShadowMatrix[i];
    }
}


template<typename ElementType>
inline
void mdkMatrix<ElementType>::operator-=(const mdkShadowMatrix<ElementType>& ShadowMatrix)
{
    auto Size = ShadowMatrix.GetSize();

    if (Size.RowNumber == 0)
    {
        mdkError << "ShadowMatrix is empty @ mdkMatrix::operator-=(ShadowMatrix)" << '\n';
        return;
    }

    if (Size.RowNumber == 1 && Size.ColNumber == 1)
    {
        (*this) -= ShadowMatrix[0];

        return;
    }

    if (Size.RowNumber != SelfSize.RowNumber || Size.ColNumber != SelfSize.ColNumber)
    {
        mdkError << "Size does not match @ mdkMatrix::operator-=(ShadowMatrix)" << '\n';
        return;
    }

    auto RawPointer this->GetElementPointer();

    for (uint64 i = 0; i < Self_ElementNumber; ++i)
    {
        RawPointer[i] -= ShadowMatrix[i];
    }
}


template<typename ElementType>
inline 
void mdkMatrix<ElementType>::operator*=(const mdkShadowMatrix<ElementType>& ShadowMatrix)
{
    this->operator*=(mdkShadowMatrix.CreateMatrix());
}


template<typename ElementType>
inline
void mdkMatrix<ElementType>::operator/=(const mdkShadowMatrix<ElementType>& ShadowMatrix)
{
    auto Size = ShadowMatrix.GetSize();

    if (Size.RowNumber == 0)
    {
        mdkError << "ShadowMatrix is empty @ mdkMatrix::operator/=(ShadowMatrix)" << '\n';
        return;
    }

    if (Size.RowNumber == 1 && Size.ColNumber == 1)
    {
        (*this) /= ShadowMatrix[0];

        return;
    }

    if (Size.RowNumber != SelfSize.RowNumber || Size.ColNumber != SelfSize.ColNumber)
    {
        mdkError << "Size does not match @ mdkMatrix::operator/=(ShadowMatrix)" << '\n';
        return;
    }

    auto RawPointer this->GetElementPointer();

    for (uint64 i = 0; i < Self_ElementNumber; ++i)
    {
        RawPointer[i] /= ShadowMatrix[i];
    }
}

//---------------------- Matrix {+= -= *= /=} GlueMatrixForLinearCombination ----------------------------------------//

template<typename ElementType>
inline
void mdkMatrix<ElementType>::operator+=(mdkGlueMatrixForLinearCombination<ElementType> GlueMatrix)
{
    (*this) = (*this) + std::move(GlueMatrix);
}


template<typename ElementType>
inline
void mdkMatrix<ElementType>::operator-=(mdkGlueMatrixForLinearCombination<ElementType> GlueMatrix)
{
    (*this) = (*this) - std::move(GlueMatrix);
}


template<typename ElementType>
inline
void mdkMatrix<ElementType>::operator*=(const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrix)
{
    this->operator*=(GlueMatrix.CreateMatrix());
}


template<typename ElementType>
inline
void mdkMatrix<ElementType>::operator/=(const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrix)
{
    this->operator/=(GlueMatrix.CreateMatrix());
}

//---------------------- Matrix {+= -= *= /=} GlueMatrixForMultiplication ----------------------------------------//

template<typename ElementType>
inline
void mdkMatrix<ElementType>::operator+=(mdkGlueMatrixForMultiplication<ElementType> GlueMatrix)
{
    (*this) = (*this) + std::move(GlueMatrix);
}


template<typename ElementType>
inline
void mdkMatrix<ElementType>::operator-=(mdkGlueMatrixForMultiplication<ElementType> GlueMatrix)
{
    (*this) = (*this) - std::move(GlueMatrix);
}


template<typename ElementType>
inline
void mdkMatrix<ElementType>::operator*=(mdkGlueMatrixForMultiplication<ElementType> GlueMatrix)
{
    (*this) = (*this) * std::move(GlueMatrix);
}


template<typename ElementType>
inline
void mdkMatrix<ElementType>::operator/=(const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrix)
{
    this->operator/=(GlueMatrix.CreateMatrix());
}

//---------------------- Matrix {+= -= *= /=} Element ----------------------------------------//

template<typename ElementType>
inline	
void mdkMatrix<ElementType>::operator+=(const ElementType& Element)
{
    MatrixAdd(*this, *this, Element);
}


template<typename ElementType>
inline
void mdkMatrix<ElementType>::operator-=(const ElementType& Element)
{
    MatrixSubtract(*this, *this, Element);
}


template<typename ElementType>
inline
void mdkMatrix<ElementType>::operator*=(const ElementType& Element)
{
    MatrixElementMultiply(*this, *this, Element);
}


template<typename ElementType>
inline
void mdkMatrix<ElementType>::operator/=(const ElementType& Element)
{
    MatrixElementDivide(*this, *this, Element);
}



//-------------------- element operation {^} -----------------------------------------------------------//

template<typename ElementType>
inline 
mdkMatrix<ElementType> mdkMatrix<ElementType>::operator^(const ElementType& Element)
{
    mdkMatrix<ElementType> tempMatrix;

    auto SelfSize = this->GetSize();

    if (SelfSize.RowNumber == 0)
    {
        mdkError << "Self is empty @ mdkMatrix::operator^(value)" << '\n';

        return tempMatrix;
    }

    tempMatrix.Resize(SelfSize.RowNumber, SelfSize.ColNumber);

    auto tempRawPointer = tempMatrix.GetElementPointer();

    auto RawPointer = this->GetElementPointer();

    auto Self_ElementNumber = SelfSize.RowNumber*SelfSize.ColNumber;

    for (uint64 i = 0; i < Self_ElementNumber; ++i)
    {
        tempRawPointer[i] = std::pow(RawPointer[i], Element);
    }

    return tempMatrix;
}

//-------------------- element operation {^=} -----------------------------------------------------------//

template<typename ElementType>
inline void mdkMatrix<ElementType>::operator^=(const ElementType& Element)
{
    auto SelfSize = this->GetSize();

    if (SelfSize.RowNumber == 0)
    {
        mdkError << "Self is empty @ mdkMatrix::operator^(value)" << '\n';
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
mdkMatrix<ElementType> mdkMatrix<ElementType>::ElementMultiply(const mdkMatrix<ElementType>& InputMatrix)
{
    return MatrixElementMultiply(*this, InputMatrix);

}


template<typename ElementType>
inline
mdkMatrix<ElementType> mdkMatrix<ElementType>::ElementMultiply(const ElementType& Element)
{
    return MatrixElementMultiply(*this, Element);
}


template<typename ElementType>
inline
mdkMatrix<ElementType> mdkMatrix<ElementType>::ElementMultiply(const mdkShadowMatrix<ElementType>& ShadowMatrix)
{
    return MatrixElementMultiply(*this, ShadowMatrix.CreateMatrix());
}


template<typename ElementType>
inline
mdkMatrix<ElementType> mdkMatrix<ElementType>::ElementMultiply(const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrix)
{
    return MatrixElementMultiply(*this, GlueMatrix.CreateMatrix());
}


template<typename ElementType>
inline
mdkMatrix<ElementType> mdkMatrix<ElementType>::ElementMultiply(const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrix)
{
    return MatrixElementMultiply(*this, GlueMatrix.CreateMatrix());
}


//-------------------- general element operation : output a new matrix ------------------------------------------//

template<typename ElementType>
inline
mdkMatrix<ElementType> mdkMatrix<ElementType>::ElementOperation(const char* OperationName) const
{
    return MatrixElementOperation(std::string(OperationName), *this);
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> mdkMatrix<ElementType>::ElementOperation(const std::string& OperationName) const
{
    return MatrixElementOperation(OperationName, *this);
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> mdkMatrix<ElementType>::ElementOperation(std::function<ElementType(const ElementType&)> Operation) const
{
    return MatrixElementOperation(Operation, *this);
}


template<typename ElementType>
inline
mdkMatrix<ElementType> mdkMatrix<ElementType>::ElementOperation(const std::string& OperationName, const mdkMatrix<ElementType>& InputMatrix) const
{
    return MatrixElementOperation(OperationName, *this, InputMatrix);
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> mdkMatrix<ElementType>::ElementOperation(std::function<ElementType(const ElementType&, const ElementType&)> Operation, 
                                                                const mdkMatrix<ElementType>& InputMatrix) const
{
    return MatrixElementOperation(Operation, *this, InputMatrix);
}


template<typename ElementType>
inline
mdkMatrix<ElementType> mdkMatrix<ElementType>::ElementOperation(const char* OperationName, const ElementType& Element) const
{
    return MatrixElementOperation(std::string(OperationName), *this, Element);
}


template<typename ElementType>
inline
mdkMatrix<ElementType> mdkMatrix<ElementType>::ElementOperation(const std::string& OperationName, const ElementType& Element) const
{
    return MatrixElementOperation(OperationName, *this, Element);
}


template<typename ElementType>
inline
mdkMatrix<ElementType> mdkMatrix<ElementType>::ElementOperation(std::function<ElementType(const ElementType&, const ElementType&)> Operation, 
                                                                const ElementType& Element) const
{
    return MatrixElementOperation(Operation, *this, Element);
}


//-------------------- element operation in place : Object.ElementOperationInPlace update the current data of the object ---------------//

template<typename ElementType>
inline
bool mdkMatrix<ElementType>::ElementOperationInPlace(const char* OperationName)
{
    return MatrixElementOperation(*this, std::string(OperationName), *this);
}


template<typename ElementType>
inline
bool mdkMatrix<ElementType>::ElementOperationInPlace(const std::string& OperationName)
{
    return MatrixElementOperation(*this, OperationName, *this);
}


template<typename ElementType>
inline
bool mdkMatrix<ElementType>::ElementOperationInPlace(std::function<ElementType(const ElementType&)> Operation)
{
    return MatrixElementOperation(*this, Operation, *this);
}


template<typename ElementType>
inline
bool mdkMatrix<ElementType>::ElementOperationInPlace(const char* OperationName, const mdkMatrix<ElementType>& InputMatrix)
{
    return MatrixElementOperation(*this, std::string(OperationName), *this, InputMatrix);
}


template<typename ElementType>
inline
bool mdkMatrix<ElementType>::ElementOperationInPlace(const std::string& OperationName, const mdkMatrix<ElementType>& InputMatrix)
{
    return MatrixElementOperation(*this, OperationName, *this, InputMatrix);
}


template<typename ElementType>
inline
bool mdkMatrix<ElementType>::ElementOperationInPlace(std::function<ElementType(const ElementType&, const ElementType&)> Operation,
                                                     const mdkMatrix<ElementType>& InputMatrix)
{
    return MatrixElementOperation(*this, Operation, *this, InputMatrix);
}


template<typename ElementType>
inline
bool mdkMatrix<ElementType>::ElementOperationInPlace(const char* OperationName, const ElementType& Element)
{
    return MatrixElementOperation(*this, std::string(OperationName), *this, Element);
}


template<typename ElementType>
inline
bool mdkMatrix<ElementType>::ElementOperationInPlace(const std::string& OperationName, const ElementType& Element)
{
    return MatrixElementOperation(*this, OperationName, *this, Element);
}


template<typename ElementType>
inline
bool mdkMatrix<ElementType>::ElementOperationInPlace(std::function<ElementType(const ElementType&, const ElementType&)> Operation, const ElementType& Element)
{
    return MatrixElementOperation(*this, Operation, *this, Element);
}

//-----------------------------------------------------------------------------------------------------------//


template<typename ElementType>
inline ElementType mdkMatrix<ElementType>::Mean() const
{
    auto Self_ElementNumber = this->GetElementNumber();

    if (Self_ElementNumber == 0)
    {
        mdkError << "self is empty Matrix @ mdkMatrix::Mean" << '\n';
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
inline mdkMatrix<ElementType> mdkMatrix<ElementType>::MeanToRow() const
{
	mdkMatrix<ElementType> tempMatrix;

    auto SelfSize = this->GetSize();

    if (SelfSize.RowNumber == 0)
	{
		mdkError << "self is empty Matrix @ mdkMatrix::MeanToRow" << '\n';

		return tempMatrix;
	}

	tempMatrix.Resize(1, SelfSize.ColNumber);

	auto tempRawPointer = tempMatrix.GetElementPointer();

	auto RawPointer = this->GetElementPointer();

	for (uint64 j = 0; j < SelfSize.ColNumber; ++j)
	{
        auto value = RawPointer[0];

        ++RawPointer;

		for (uint64 i = 1; i < SelfSize.RowNumber; ++i)
		{            
			value += RawPointer[0];      

            ++RawPointer;
		}

        tempRawPointer[j] = value / SelfSize.RowNumber;
	}
    
	return tempMatrix;
}


template<typename ElementType>
inline mdkMatrix<ElementType> mdkMatrix<ElementType>::MeanToCol() const
{
	mdkMatrix<ElementType> tempMatrix;

    auto SelfSize = this->GetSize();

    if (SelfSize.RowNumber == 0)
	{
		mdkError << "self is empty Matrix @ mdkMatrix::MeanToCol" << '\n';
  
		return tempMatrix;
	}

	tempMatrix.Resize(SelfSize.ColNumber, 1);

	auto tempRawPointer = tempMatrix.GetElementPointer();

	auto RawPointer = this->GetElementPointer();

	for (uint64 i = 0; i < SelfSize.RowNumber; ++i)
	{
        auto value = RawPointer[0];

        uint64 Index = SelfSize.RowNumber;

		for (uint64 j = 1; j < SelfSize.ColNumber; ++j)
		{
			value += RawPointer[Index + i];

			Index += SelfSize.RowNumber;
		}

        tempRawPointer[i] = value / SelfSize.ColNumber;
	}

	return tempMatrix;
}


template<typename ElementType>
inline ElementType mdkMatrix<ElementType>::Sum() const
{
    auto Self_ElementNumber = this->GetElementNumber();

    if (Self_ElementNumber == 0)
	{
		mdkError << "self is empty Matrix @ mdkMatrix::Sum" << '\n';
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
inline mdkMatrix<ElementType> mdkMatrix<ElementType>::SumToRow() const
{
	mdkMatrix<ElementType> tempMatrix;

    auto SelfSize = this->GetSize();

    if (SelfSize.RowNumber == 0)
	{
		mdkError << "self is empty Matrix @ mdkMatrix::SumToRow" << '\n';
        
		return tempMatrix;
	}

	tempMatrix.Resize(1, SelfSize.ColNumber);

	auto tempRawPointer = tempMatrix.GetElementPointer();

	auto RawPointer = this->GetElementPointer();

	for (uint64 j = 0; j < SelfSize.ColNumber; ++j)
	{
        auto value = RawPointer[0];

        ++RawPointer;

		for (uint64 i = 1; i < SelfSize.RowNumber; ++i)
		{
			value += RawPointer[0];

            ++RawPointer;
		}

		tempRawPointer[j] = value;
	}
    
	return tempMatrix;
}


template<typename ElementType>
inline mdkMatrix<ElementType> mdkMatrix<ElementType>::SumToCol() const
{
	mdkMatrix<ElementType> tempMatrix;

    auto SelfSize = this->GetSize();

    if (SelfSize.RowNumber == 0)
	{
		mdkError << "self is empty Matrix @ mdkMatrix::SumToCol" << '\n';
        
		return tempMatrix;
	}

	tempMatrix.Resize(SelfSize.RowNumber, 1);

	auto tempRawPointer = tempMatrix.GetElementPointer();

	auto RawPointer = this->GetElementPointer();

	for (uint64 i = 0; i < SelfSize.RowNumber; ++i)
	{
        auto value = RawPointer[i];

        uint64 Index = SelfSize.RowNumber;

		for (uint64 j = 1; j < SelfSize.ColNumber; ++j)
		{
			value += RawPointer[Index + i];

			Index += SelfSize.RowNumber;
		}

		tempRawPointer[i] = value;
	}
    
	return tempMatrix;
}


template<typename ElementType>
inline ElementType mdkMatrix<ElementType>::Max() const
{
    auto Self_ElementNumber = this->GetElementNumber();

    if (Self_ElementNumber == 0)
	{
		mdkError << "self is empty Matrix @ mdkMatrix::Max" << '\n';
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
inline mdkMatrix<ElementType> mdkMatrix<ElementType>::MaxToRow() const
{
	mdkMatrix<ElementType> tempMatrix;

    auto SelfSize = this->GetSize();

    if (SelfSize.RowNumber == 0)
	{
		mdkError << "self is empty Matrix @ mdkMatrix::MaxToRow" << '\n';
        
		return tempMatrix;
	}

	tempMatrix.Resize(1, SelfSize.ColNumber);

	auto tempRawPointer = tempMatrix.GetElementPointer();

	auto RawPointer = this->GetElementPointer();

	for (uint64 j = 0; j < SelfSize.ColNumber; ++j)
	{
        auto value = RawPointer[0];

        ++RawPointer;

		for (uint64 i = 1; i < SelfSize.RowNumber; ++i)
		{
			value = std::max(value, RawPointer[0]);

            ++ RawPointer;
		}

		tempRawPointer[j] = value;
	}
    
	return tempMatrix;
}


template<typename ElementType>
inline mdkMatrix<ElementType> mdkMatrix<ElementType>::MaxToCol() const
{
	mdkMatrix<ElementType> tempMatrix;

    auto SelfSize = this->GetSize();

    if (SelfSize.RowNumber == 0)
	{
		mdkError << "self is empty Matrix @ mdkMatrix::MaxToCol" << '\n';
        
		return tempMatrix;
	}

	tempMatrix.Resize(SelfSize.ColNumber, 1);

	auto tempRawPointer = tempMatrix.GetElementPointer();

	auto RawPointer = this->GetElementPointer();

	for (uint64 i = 0; i < SelfSize.RowNumber; ++i)
	{
        auto value = RawPointer[i];

        auto Index = SelfSize.RowNumber;

		for (uint64 j = 1; j < SelfSize.ColNumber; ++j)
		{
			value = std::max(value, RawPointer[Index + i]);

			Index += SelfSize.RowNumber;
		}

		tempRawPointer[i] = value;
	}

	return tempMatrix;
}


template<typename ElementType>
inline ElementType mdkMatrix<ElementType>::Min() const
{
    auto Self_ElementNumber = this->GetElementNumber();

    if (Self_ElementNumber == 0)
	{
		mdkError << "self is empty Matrix @ mdkMatrix::Min" << '\n';
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
inline mdkMatrix<ElementType> mdkMatrix<ElementType>::MinToRow() const
{
	mdkMatrix<ElementType> tempMatrix;

    auto SelfSize = this->GetSize();

    if (SelfSize.RowNumber == 0)
	{
		mdkError << "self is empty Matrix @ mdkMatrix::MinToRow" << '\n';
        
		return tempMatrix;
	}

	tempMatrix.Resize(1, SelfSize.ColNumber);

	auto tempRawPointer = tempMatrix.GetElementPointer();

	auto RawPointer = this->GetElementPointer();

	for (uint64 j = 0; j < SelfSize.ColNumber; ++j)
	{
		auto value = RawPointer[0];

        ++RawPointer;

		for (uint64 i = 1; i < SelfSize.RowNumber; ++i)
		{
			value = std::min(value, RawPointer[0]);

            ++RawPointer;
		}

		tempRawPointer[j] = value;
	}
    
	return tempMatrix;
}


template<typename ElementType>
inline mdkMatrix<ElementType> mdkMatrix<ElementType>::MinToCol() const
{
	mdkMatrix<ElementType> tempMatrix;

    auto SelfSize = this->GetSize();

    if (SelfSize.RowNumber == 0)
	{
		mdkError << "self is empty Matrix @ mdkMatrix::MinToCol" << '\n';
        
		return tempMatrix;
	}

	tempMatrix.Resize(SelfSize.ColNumber, 1);

	auto tempRawPointer = tempMatrix.GetElementPointer();

	auto RawPointer = this->GetElementPointer();

	for (uint64 i = 0; i < SelfSize.RowNumber; ++i)
	{
        auto value = RawPointer[i];

        auto Index = SelfSize.RowNumber;

		for (uint64 j = 1; j < SelfSize.ColNumber; ++j)
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
mdkMatrix<ElementType> mdkMatrix<ElementType>::Transpose() const
{
    return MatrixTranspose(*this);
}


template<typename ElementType>
inline
uint64 mdkMatrix<ElementType>::Rank() const
{
    return MatrixRank(*this);
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> mdkMatrix<ElementType>::Inv() const
{
    return MatrixInv(*this);
}


template<typename ElementType>
inline
mdkMatrix<ElementType> mdkMatrix<ElementType>::PseudoInv() const
{
	// call Armadillo 


}


template<typename ElementType>
inline
mdkMatrixSVDResult<ElementType> mdkMatrix<ElementType>::SVD() const
{
    return MatrixSVD(*this);
}

}//end namespace mdk

#endif