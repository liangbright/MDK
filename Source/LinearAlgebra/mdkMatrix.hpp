#ifndef __mdkMatrix_hpp
#define __mdkMatrix_hpp

#include <string>
#include <cmath>
#include <algorithm>

#include "armadillo.h"

#include "mdkMatrix.h"
#include "mdkShadowMatrix.h"
#include "mdkGlueMatrix.h"
#include "mdkType.h"
#include "mdkDebugConfig.h"

namespace mdk
{
    
//mdkObject mkdObject_For_mdkMatrix;

template<typename ElementType>
inline
mdkMatrix<ElementType>::mdkMatrix()
{
	this->Clear();

    m_IsTemporary = false;
}


template<typename ElementType>
inline
mdkMatrix<ElementType>::mdkMatrix(uint64 RowNumber, uint64 ColNumber, bool IsSizeFixed = true)
{
    this->Clear();

    m_IsTemporary = false;

    this->SetSize(RowNumber, ColNumber, IsSizeFixed);
}


template<typename ElementType>
inline
mdkMatrix<ElementType>::mdkMatrix(const mdkMatrix<ElementType>& targetMatrix, bool IsSizeFixed = true)
{
    auto Counter = targetMatrix.Get_Counter_SharedCopyConstruction_From_TemporaryMatrix();

    if (targetMatrix.IsTemporary() == true && Counter < 1)
    { // just returned by value as a local variable from a function
      // and this copy constructor will still create a Temporary matrix with counter + 1

        // not necessary to use this->Clear() here
        this->Share(targetMatrix);

        m_Counter_SharedCopyConstruction_From_TemporaryMatrix = Counter + 1;

        m_IsTemporary = true;
    }
    else
    {
        // must use this function here
        this->Clear();

        //force-copy data
        this->Copy(targetMatrix, true);     

        // just for record
        m_Counter_SharedCopyConstruction_From_TemporaryMatrix = Counter;

        m_IsTemporary = false;
    }    

    m_IsSizeFixed = IsSizeFixed;
}


template<typename ElementType>
inline
mdkMatrix<ElementType>::mdkMatrix(const mdkShadowMatrix<ElementType>& ShadowMatrix, bool IsSizeFixed = true)
{
    // not necessary to use this->Clear() here

    this->Share(ShadowMatrix.CreateMatrix());

    m_IsTemporary = false;

    m_IsSizeFixed = IsSizeFixed;
}


template<typename ElementType>
inline
mdkMatrix<ElementType>::mdkMatrix(const mdkGlueMatrix<ElementType>& GlueMatrix, bool IsSizeFixed = true)
{
    // not necessary to use this->Clear() here

    this->Share(GlueMatrix.CreateMatrix());

    m_IsTemporary = false;

    m_IsSizeFixed = IsSizeFixed;
}



template<typename ElementType>
inline
mdkMatrix<ElementType>::mdkMatrix(const ElementType* ElementPointer, uint64 RowNumber, uint64 ColNumber, bool IsSizeFixed = true)
{
    // must use this function here
    this->Clear();

    m_IsTemporary = false;

    m_IsSizeFixed = IsSizeFixed;

    if (ElementPointer == nullptr )
    {
        mdkWarning << "Empty input @ mdkMatrix::mdkMatrix(ElementType*, uint64, uint64, bool)" << '\n';
        return;
    }

    if (RowNumber == 0 || ColNumber == 0)
    {
        if (ElementPointer != nullptr)
        {
            mdkError << "Invalid input @ mdkMatrix::mdkMatrix(ElementType*, uint64, uint64, bool)" << '\n';
            return;
        }
    }

    // force-copy data
    this->Copy(ElementPointer, RowNumber, ColNumber, true);
}


template<typename ElementType>
inline
mdkMatrix<ElementType>::mdkMatrix(std::vector<ElementType>* ElementDataPointer, uint64 RowNumber, uint64 ColNumber, bool IsSizeFixed = true)
{
    // must use this function here
    this->Clear();

    if (ElementPointer == nullptr)
    {
        mdkWarning << "Empty input @ mdkMatrix::mdkMatrix(std::vector<ElementType>*, uint64, uint64, bool)" << '\n';
        return;
    }

    if (RowNumber == 0 || ColNumber == 0)
    {
        if (ElementPointer != nullptr)
        {
            mdkError << "Invalid input @ mdkMatrix::mdkMatrix(std::vector<ElementType>*, uint64, uint64, bool)" << '\n';
            return;
        }
    }

    // only use, do not own
    m_ElementData.reset(ElementDataPointer, [](ElementType*){});

    m_RowNumber = RowNumber;

    m_ColNumber = ColNumber;

    m_IsTemporary = false;

    m_IsSizeFixed = IsSizeFixed;
}


template<typename ElementType>
inline
mdkMatrix<ElementType>::~mdkMatrix()
{
}


template<typename ElementType>
inline
void mdkMatrix<ElementType>::Clear()
{
    m_RowNumber = 0;
    m_ColNumber = 0;

    m_ElementData.reset();

    m_IsSizeFixed = false;

    m_ElementType = FindMatrixElementType(m_NaNElement);

    this->SetNaNElement(m_ElementType);

    m_Counter_SharedCopyConstruction_From_TemporaryMatrix = 0;
}


template<typename ElementType>
inline
void mdkMatrix<ElementType>::SetTobeTemporaryBeforeReturn()
{
    // must clear the counter
    m_Counter_SharedCopyConstruction_From_TemporaryMatrix = 0;

    m_IsTemporary = true;
}


template<typename ElementType>
inline
bool mdkMatrix<ElementType>::IsTemporary() const
{
    return m_IsTemporary;
}


template<typename ElementType>
inline
uint64 mdkMatrix<ElementType>::Get_Counter_SharedCopyConstruction_From_TemporaryMatrix() const
{
    return m_Counter_SharedCopyConstruction_From_TemporaryMatrix;
}

template<typename ElementType>
inline
mdkMatrixElementTypeEnum mdkMatrix<ElementType>::GetElementType() const
{
    return m_ElementType;
}


template<typename ElementType>
inline 
ElementType mdkMatrix<ElementType>::GetNaNElement()  const
{
    return m_NaNElement;
}


template<typename ElementType>
inline
void mdkMatrix<ElementType>::SetNaNElement(mdkMatrixElementTypeEnum ElementType)
{
    switch (ElementType)
    {
    case mdkMatrixElementTypeEnum::Scalar_DOUBLE64:
        m_NaNElement = std::nan(nullptr);
        break;

    case mdkMatrixElementTypeEnum::Scalar_FLOAT32:
        m_NaNElement = std::nanf(nullptr);
        break;

    case mdkMatrixElementTypeEnum::StdVector_DOUBLE64:
        m_NaNElement += std::nan(nullptr);
        break;

    case mdkMatrixElementTypeEnum::StdVector_FLOAT32:
        m_NaNElement += std::nanf(nullptr);
        break;

    case mdkMatrixElementTypeEnum::StdArray_DOUBLE64:
        m_NaNElement += std::nan(nullptr);
        break;

    case mdkMatrixElementTypeEnum::StdArray_FLOAT32:
        m_NaNElement += std::nanf(nullptr);
        break;

    default:
        mdkWarning << "ElementType is not float or double, so m_NaNElement is set to zero @ mdkMatrix::SetNaNElement(ElementType)" << '\n';
        m_NaNElement -= m_NaNElement;
    }
}


template<typename ElementType>
inline
void mdkMatrix<ElementType>::SetNaNElement(ElementType NaNElement)
{
    m_NaNElement = NaNElement;
}


template<typename ElementType>
inline
const std::shared_ptr<std::vector<ElementType>>& mdkMatrix<ElementType>::GetElementDataSharedPointer() const
{
    return m_ElementData;
}


template<typename ElementType>
inline
ElementType* mdkMatrix<ElementType>::GetElementDataRawPointer()
{
    if (m_ElementData) // if (m_ElementData != nullptr)
    {
        return m_ElementData->data();
    }
    else
    {
        return nullptr;
    }
}


template<typename ElementType>
inline
const ElementType* mdkMatrix<ElementType>::GetElementDataRawPointer() const
{
    if (m_ElementData) // if (m_ElementData != nullptr)
    {
        return m_ElementData->data();
    }
    else
    {
        return nullptr;
    }
}


template<typename ElementType>
inline
bool mdkMatrix<ElementType>::SetSize(uint64 RowNumber, uint64 ColNumber, bool IsSizeFixed = true)
{
	if (m_ElementData) // if (m_ElementData != nullptr)
	{
		mdkError << "must call Clear before SetSize because the matrix is not empty @ mdkMatrix::SetSize" << '\n';
		return false;
	}

    if (RowNumber == 0 || ColNumber == 0)
    {
        m_RowNumber = 0;

        m_ColNumber = 0;
    }
    else
    {      
        m_RowNumber = RowNumber;

        m_ColNumber = ColNumber;
    }

    m_ElementData = std::make_shared<std::vector<ElementType>>(RowNumber*ColNumber);

	m_IsSizeFixed = IsSizeFixed;

	return true;
}


template<typename ElementType>
inline
bool mdkMatrix<ElementType>::Reshape(uint64 RowNumber, uint64 ColNumber)
{
    if (m_IsSizeFixed == true)
    {
        mdkError << "Matrix size can not be changed @ mdkMatrix::Reshape" << '\n';
        return false;
    }

    if (m_ElementData == nullptr)
    {
        if (RowNumber == 0 || ColNumber == 0)
        {
            m_RowNumber = 0;

            m_ColNumber = 0;

            return true;
        }
        else
        {
            mdkError << "Self is empty and Size does not match @ mdkMatrix::Reshape" << '\n';
            return false;
        }
    }

    if (RowNumber*ColNumber != this->GetElementNumber())
    {
        mdkError << "Size does not match @ mdkMatrix::Reshape" << '\n';
        return false;
    }

    m_RowNumber = RowNumber;

    m_ColNumber = ColNumber;

    return true;
}


template<typename ElementType>
inline
void mdkMatrix<ElementType>::FixSize(bool Fix = true)
{
    m_IsSizeFixed = Fix;
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
    if (m_RowNumber > 0)
    {
        return false;
    }

    return true;
}


template<typename ElementType>
inline
mdkMatrixSize mdkMatrix<ElementType>::GetSize() const
{
    mdkMatrixSize Size;

    Size.RowNumber = m_RowNumber;

    Size.ColNumber = m_ColNumber;

    return Size;
}


template<typename ElementType>
inline
void mdkMatrix<ElementType>::GetSize(uint64* RowNumber, uint64* ColNumber) const
{
    RowNumber[0] = m_RowNumber;

    ColNumber[0] = m_ColNumber;
}


template<typename ElementType>
inline
uint64 mdkMatrix<ElementType>::GetElementNumber() const
{
    if (m_ElementData) // m_ElementData != nullptr
    {
        return m_ElementData->size();
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
	return m_ColNumber;
}


template<typename ElementType>
inline 
uint64 mdkMatrix<ElementType>::GetRowNumber() const
{
	return m_RowNumber;
}


template<typename ElementType>
inline
void mdkMatrix<ElementType>::operator=(const ElementType& Element)
{
    auto ElementNumber = m_RowNumber*m_ColNumber;

	if (m_IsSizeFixed == true)
	{
        if (ElementNumber != 1)
		{
			mdkError << "Can not change matrix size @ mdkMatrix::operator=(Element)" << '\n';
			return;
		}
	}

    if (ElementNumber == 1)
    {
        (*m_ElementData)[0] = Element;
    }
    else
    {
        mdkWarning << "The size of the matrix changes to 1x1 @ mdkMatrix::operator=(Element)" << '\n';

        m_ElementData = std::make_shared<std::vector<ElementType>>(1);

        (*m_ElementData)[0] = Element;

        m_RowNumber = 1;
        m_ColNumber = 1;
    }

	return;
}


template<typename ElementType>
inline
void mdkMatrix<ElementType>::operator=(const std::initializer_list<ElementType>& list)
{
    //if Self is empty
	if (m_RowNumber == 0)
	{
		mdkError << "operator=(list) can only be called after SetSzie() @ mdkMatrix::operator=(list)" << '\n';
		return;
	}

	if (m_RowNumber*m_ColNumber != list.size())
	{
		mdkError << "Size does not match @ mdkMatrix::operator=(list)" << '\n';
		return;
	}

	auto RawPointer = m_ElementData->data();

	uint64 Counter = 0;

	for (uint64 i = 0; i < m_RowNumber; ++i)
	{
		uint64 Index = 0;

		for (uint64 j = 0; j < m_ColNumber; ++j)
		{
            RawPointer[Index + i] = list.begin()[Counter];

			Index += m_RowNumber;

			Counter += 1;
		}
	}

	return;
}


// Attention : operator=(const mdk::mdkMatrix<double>& targetMatrix) 
// if targetMatrix is a Temporary Matrix (when it is returned by value from a function)
// then this Matrix shares the data of targetMatrix
// I have to add const in operator=(), and use shared_ptr instead of unique_ptr
// because std::vector<mdkMatrix> A;  A =  B ((std::vector<mdkMatrix) requires operator=(const ...)
// and there is no way to tell the difference between operator=(const ...) and operator=(...)
// 
// shared_ptr is good

template<typename ElementType>
inline
void mdkMatrix<ElementType>::operator=(const mdkMatrix<ElementType>& targetMatrix)
{
    // MatrixA = MatrixA
    if (this == &targetMatrix)
    {
        mdkWarning << "A Matrix tries to Copy itself @ mdkMatrix::operator=(targetMatrix)" << '\n';
        return;
    }

    if (targetMatrix.IsEmpty() == true)
    {
        mdkWarning << "Input is empty @ mdkMatrix::operator=(targetMatrix)" << '\n';
    }

    auto targetSize = targetMatrix.GetSize();

    if (m_IsSizeFixed == true)
    {
        if (targetSize.RowNumber != m_RowNumber || targetSize.ColNumber != m_ColNumber)
        {
            mdkError << "Matrix size can not be changed @ mdkMatrix::operator=(mdkMatrix)" << '\n';
            return;
        }
    }

    if (targetMatrix.IsTemporary() == true)
    {
        this->Share(targetMatrix);

        // just for record
        m_Counter_SharedCopyConstruction_From_TemporaryMatrix = targetMatrix.Get_Counter_SharedCopyConstruction_From_TemporaryMatrix() + 1;
    }
    else
    {
        this->Copy(targetMatrix);
    }
}


template<typename ElementType>
inline
void mdkMatrix<ElementType>::operator=(const mdkShadowMatrix<ElementType>& ShadowMatrix)
{
    if (m_IsSizeFixed == true)
    {
        if (ShadowMatrix.GetRowNumber() != m_RowNumber || ShadowMatrix.GetColNumber() != m_ColNumber)
        {
            mdkError << "Matrix size can not be changed @ mdkMatrix::Copy(ShadowMatrix)" << '\n';
            return;
        }
    }

    if (ShadowMatrix.IsEmpty() == true)
    {
        mdkWarning << "Input is empty @ mdkMatrix::operator=(ShadowMatrix)" << '\n';
        this->Clear();
        return;
    }

    if (m_IsSizeFixed == true)
    {
        ShadowMatrix.CreateMatrix(*this);
    }
    else
    {
        this->Share(ShadowMatrix.CreateMatrix());
    }
}


template<typename ElementType>
inline
void mdkMatrix<ElementType>::operator=(const mdkGlueMatrix<ElementType>& GlueMatrix)
{
    if (m_IsSizeFixed == true)
    {
        if (GlueMatrix.GetRowNumber() != m_RowNumber || GlueMatrix.GetColNumber() != m_ColNumber)
        {
            mdkError << "Matrix size can not be changed @ mdkMatrix::Copy(GlueMatrix)" << '\n';
            return;
        }
    }

    if (GlueMatrix.IsEmpty() == true)
    {
        mdkWarning << "Input is empty @ mdkMatrix::operator=(GlueMatrix)" << '\n';
        this->Clear();
        return;
    }

    if (m_IsSizeFixed == true)
    {
        GlueMatrix.CreateMatrix(*this);
    }
    else
    {
        this->Share(GlueMatrix.CreateMatrix());   
    }
}


template<typename ElementType>
inline
void mdkMatrix<ElementType>::Share(const mdkMatrix<ElementType>& targetMatrix)
{
    m_RowNumber = targetMatrix.GetRowNumber();

    m_ColNumber = targetMatrix.GetColNumber();

    m_ElementData = targetMatrix.GetElementDataSharedPointer();

    m_IsSizeFixed = targetMatrix.IsSizeFixed();

    m_ElementType = targetMatrix.GetElementType();

    m_NaNElement = targetMatrix.GetNaNElement();
}


template<typename ElementType>
inline
void mdkMatrix<ElementType>::Share(const mdkShadowMatrix<ElementType>& ShadowMatrix)
{
    this->operator=(ShadowMatrix);
}


template<typename ElementType>
inline
void mdkMatrix<ElementType>::Share(const mdkGlueMatrix<ElementType>& GlueMatrix)
{
    this->operator=(GlueMatrix);
}


template<typename ElementType>
template<typename ElementType_target>
inline
bool mdkMatrix<ElementType>::Copy(const mdkMatrix<ElementType_target>& targetMatrix, bool IsForceCopy = false)
{
    if (this == &targetMatrix)
    {
        mdkWarning << "A Matrix tries to Copy itself @ mdkMatrix::Copy(targetMatrix)" << '\n';
        return false;
    }

    if (IsForceCopy == false)
    {
        if (m_IsSizeFixed == true)
        {
            if (targetMatrix.GetRowNumber() != m_RowNumber || targetMatrix.GetColNumber() != m_ColNumber)
            {
                mdkError << "Matrix size can not be changed @ mdkMatrix::Copy(targetMatrix)" << '\n';
                return false;
            }
        }
    }

	if (targetMatrix.IsEmpty() == true)
	{
		mdkWarning << "targetMatrix is empty @ mdkMatrix::Copy(targetMatrix)" << '\n';

        // if self is not empty, then set it to be empty
        if (m_RowNumber > 0)
        {
            m_RowNumber = 0;

            m_ColNumber = 0;

            m_ElementData.reset();

            m_IsSizeFixed = false;
        }

		return true;
	}

    return this->Copy(targetMatrix.GetElementDataRawPointer(), targetMatrix.GetRowNumber(), targetMatrix.GetColNumber(), IsForceCopy);
}


template<typename ElementType>
template<typename ElementType_target>
inline 
bool mdkMatrix<ElementType>::Copy(const ElementType_target* ElementPointer, uint64 RowNumber, uint64 ColNumber, bool IsForceCopy = false)
{
    if (ElementPointer == nullptr || RowNumber == 0 || ColNumber == 0)
	{
        mdkError << "Input is empty @ mdkMatrix::Copy(ElementType_target*, RowNumber, ColNumber)" << '\n';
		return true;
	}

    auto tempElementType = FindMatrixElementType(ElementPointer[0]);

	if (tempElementType == mdkMatrixElementTypeEnum::UNKNOWN)
	{
		mdkError << "input type is unknown @ mdkMatrix::Copy(ElementType_target*, RowNumber, ColNumber)" << '\n';
		return false;
	}

    // if this matrix is not empty, check if this and target share the same data
    if (m_RowNumber > 0)
    {
        if (std::size_t(ElementPointer) == std::size_t(m_ElementData->data()))
        {
            mdkWarning << "A Matrix tries to Copy itself @ mdkMatrix::Copy(ElementType_target*, RowNumber, ColNumber)" << '\n';
            return false;
        }
    }

    if (IsForceCopy == false)
    {
        if (m_IsSizeFixed == true)
        {
            if (RowNumber != m_RowNumber || ColNumber != m_ColNumber)
            {
                mdkError << "Can not change matrix size @ mdkMatrix::Copy(ElementType_target*, RowNumber, ColNumber)" << '\n';
                return false;
            }
        }
    }
    // check to see if new memory allocation is needed --------------------------------

    bool IsNewMemoryNeeded = false;

    //if self is empty
    if (m_RowNumber == 0)
    {
        IsNewMemoryNeeded = true;
    }
    else
    {
        if (RowNumber != m_RowNumber || ColNumber != m_ColNumber)
        {
            IsNewMemoryNeeded = true;
        }
    }

    if (IsNewMemoryNeeded == true)
    {
        m_RowNumber = RowNumber;

        m_ColNumber = ColNumber;

        m_ElementData = std::make_shared<std::vector<ElementType>>(RowNumber*ColNumber);
    }

    //copy data ----------------------------------------------------------

	auto RawPointer = m_ElementData->data();

    auto tempPtr = ElementPointer;

    auto ElementNumber = m_RowNumber*m_ColNumber;

    for (auto Ptr = RawPointer; Ptr < RawPointer + ElementNumber; ++Ptr)
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

    auto RawPointer = m_ElementData->data();

    for (auto Ptr = RawPointer; Ptr < RawPointer + ElementNumber; ++Ptr)
    {
        Ptr[0] = Element;
    }

	return true;
}


template<typename ElementType>
inline
ElementType& mdkMatrix<ElementType>::operator()(uint64 LinearIndex)
{
#if defined(MDK_Matrix_Operator_CheckBound)

    auto ElementNumber = this->GetElementNumber();

    if (LinearIndex >= ElementNumber)
	{
		mdkError << "Invalid input @ mdkMatrix::operator(LinearIndex)" <<'\n';
		return m_NaNElement;
	}

#endif

	return (*m_ElementData)[LinearIndex];
}


template<typename ElementType>
inline
const ElementType& mdkMatrix<ElementType>::operator()(uint64 LinearIndex) const
{
#if defined(MDK_Matrix_Operator_CheckBound)

    auto ElementNumber = this->GetElementNumber();

	if (LinearIndex >= ElementNumber)
	{
		mdkError << "Invalid input @ mdkMatrix::operator(LinearIndex) const" << '\n';
		return m_NaNElement;
	}

#endif

	return (*m_ElementData)[LinearIndex];
}


#if defined MDK_Matrix_Enable_Operator_Bracket //-------------------------------------------------------------------------------

template<typename ElementType>
inline
ElementType& mdkMatrix<ElementType>::operator[](uint64 LinearIndex)
{
#if defined(MDK_Matrix_Operator_CheckBound)

    auto ElementNumber = this->GetElementNumber();

	if (LinearIndex >= ElementNumber)
	{
		mdkError << "LinearIndex >= m_ElementNumber @ mdkMatrix::operator[LinearIndex]" << '\n';
		return m_NaNElement;
	}

#endif

	return (*m_ElementData)[LinearIndex];
}


template<typename ElementType>
inline
const ElementType& mdkMatrix<ElementType>::operator[](uint64 LinearIndex) const
{
#if defined(MDK_Matrix_Operator_CheckBound)

    auto ElementNumber = this->GetElementNumber();

	if (LinearIndex >= ElementNumber)
	{
		mdkError << "LinearIndex >= m_ElementNumber @ mdkMatrix::operator[LinearIndex] const" << '\n';
		return m_NaNElement;
	}

#endif

	return (*m_ElementData)[LinearIndex];
}

#endif // #if defined MDK_Matrix_Enable_Operator_Bracket ----------------------------------------------------------------------------------


template<typename ElementType>
inline
ElementType& mdkMatrix<ElementType>::at(uint64 LinearIndex)
{
    auto ElementNumber = this->GetElementNumber();

	if (LinearIndex >= ElementNumber)
	{
		mdkError << "Invalid input @ mdkMatrix::at(i)" << '\n';
		return m_NaNElement;
	}

	return (*m_ElementData)[LinearIndex];
}


template<typename ElementType>
inline
const ElementType& mdkMatrix<ElementType>::at(uint64 LinearIndex) const
{
    auto ElementNumber = this->GetElementNumber();

	if (LinearIndex >= ElementNumber)
	{
		mdkError << "Invalid input @ mdkMatrix::at(i) const" << '\n';
		return m_NaNElement;
	}

	return (*m_ElementData)[LinearIndex];
}


template<typename ElementType>
inline
ElementType& mdkMatrix<ElementType>::operator()(uint64 RowIndex, uint64 ColIndex)
{
#if defined(MDK_Matrix_Operator_CheckBound)

    if (RowIndex >= m_RowNumber || ColIndex >= m_ColNumber)
	{
		mdkError << "Invalid input @ mdkMatrix::operator(i,j)" << '\n';
		return m_NaNElement;
	}

#endif

    auto LinearIndex = ColIndex*m_RowNumber + RowIndex;

    return (*m_ElementData)[LinearIndex];
}


template<typename ElementType>
inline
const ElementType& mdkMatrix<ElementType>::operator()(uint64 RowIndex, uint64 ColIndex) const
{
#if defined(MDK_Matrix_Operator_CheckBound)

    if (RowIndex >= m_RowNumber || ColIndex >= m_ColNumber)
    {
        mdkError << "Invalid input @ mdkMatrix::operator(i,j) const" << '\n';
        return m_NaNElement;
    }

#endif

    auto LinearIndex = ColIndex*m_RowNumber + RowIndex;

    return (*m_ElementData)[LinearIndex];
}


template<typename ElementType>
inline
ElementType& mdkMatrix<ElementType>::at(uint64 RowIndex, uint64 ColIndex)
{
    if (RowIndex >= m_RowNumber || ColIndex >= m_ColNumber)
    {
        mdkError << "Invalid input @ mdkMatrix::at(i,j)" << '\n';
        return m_NaNElement;
    }

    auto LinearIndex = ColIndex*m_RowNumber + RowIndex;

    return (*m_ElementData)[LinearIndex];
}


template<typename ElementType>
inline
const ElementType& mdkMatrix<ElementType>::at(uint64 RowIndex, uint64 ColIndex) const
{
    if (RowIndex >= m_RowNumber || ColIndex >= m_ColNumber)
    {
        mdkError << "Invalid input @ mdkMatrix::at(i,j) const" << '\n';
        return m_NaNElement;
    }

    auto LinearIndex = ColIndex*m_RowNumber + RowIndex;

    return (*m_ElementData)[LinearIndex];
}



template<typename ElementType>
inline
mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::operator()(std::initializer_list<uint64>& LinearIndexList)
{
#if defined(MDK_Matrix_Operator_CheckBound)

    auto ElementNumber = this->GetElementNumber();

    for (auto it = LinearIndexList.begin(); it != LinearIndexList.end(); ++it)
    {
        if (*it >= ElementNumber)
        {
            mdkError << "Invalid LinearIndexList @ mdkMatrix::operator()({LinearIndexList})" << '\n';
            mdkShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

#endif

    mdkShadowMatrix<ElementType> tempShadowMatrix(*this, LinearIndexList);

    return tempShadowMatrix;
}


template<typename ElementType>
inline
mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::operator()(const std::vector<uint64>& LinearIndexList)
{
#if defined(MDK_Matrix_Operator_CheckBound)

    auto ElementNumber = this->GetElementNumber();

    for (auto it = LinearIndexList.begin(); it != LinearIndexList.end(); ++it)
    {
        if (*it >= ElementNumber)
        {
            mdkError << "Invalid LinearIndexList @ mdkMatrix::SubMatrix(std::vector LinearIndexList)" << '\n';
            mdkShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }
#endif

    mdkShadowMatrix<ElementType> tempShadowMatrix(*this, LinearIndexList);

    return tempShadowMatrix;
}


#if defined MDK_Matrix_Enable_Operator_Bracket //-------------------------------------------------------------------------------

template<typename ElementType>
inline
mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::operator[](std::initializer_list<uint64>& LinearIndexList)
{
#if defined(MDK_Matrix_Operator_CheckBound)

    auto ElementNumber = this->GetElementNumber();

    for (auto it = LinearIndexList.begin(); it != LinearIndexList.end(); ++it)
    {
        if (*it >= ElementNumber)
        {
            mdkError << "Invalid LinearIndexList @ mdkMatrix::operator()({LinearIndexList})" << '\n';
            mdkShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

#endif

    mdkShadowMatrix<ElementType> tempShadowMatrix(*this, LinearIndexList);

    return tempShadowMatrix;
}


template<typename ElementType>
inline
mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::operator[](const std::vector<uint64>& LinearIndexList)
{
#if defined(MDK_Matrix_Operator_CheckBound)

    auto ElementNumber = this->GetElementNumber();

    for (auto it = LinearIndexList.begin(); it != LinearIndexList.end(); ++it)
    {
        if (*it >= ElementNumber)
        {
            mdkError << "Invalid LinearIndexList @ mdkMatrix::SubMatrix(std::vector LinearIndexList)" << '\n';
            mdkShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }
#endif

    mdkShadowMatrix<ElementType> tempShadowMatrix(*this, LinearIndexList);

    return tempShadowMatrix;
}

#endif // #if defined MDK_Matrix_Enable_Operator_Bracket -------------------------------------------------------------------------------


template<typename ElementType>
inline
mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::operator()(std::initializer_list<uint64>& RowIndexList, std::initializer_list<uint64>& ColIndexList)
{
#if defined(MDK_Matrix_Operator_CheckBound)

    for (auto it = RowIndexList.begin(); it != RowIndexList.end(); ++it)
    {
        if (*it >= m_ColNumber)
        {
            mdkError << "Invalid ColIndexList @ mdkMatrix::operator()({RowIndexList}, {ColIndexList})" << '\n';
            mdkShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= m_RowNumber)
        {
            mdkError << "Invalid RowIndexList @ mdkMatrix::operator()({RowIndexList}, {ColIndexList})" << '\n';
            mdkShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

#endif

    if (RowIndexList.size() == 0 || ColIndexList.size() == 0)
    {
        mdkError << "Empty Input @ mdkMatrix::operator()({RowIndexList}, {ColIndexList})" << '\n';
        mdkShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    mdkShadowMatrix<ElementType> tempShadowMatrix(*this, RowIndexList, ColIndexList);

    return tempShadowMatrix;
}


template<typename ElementType>
inline
mdkShadowMatrix<ElementType> 
mdkMatrix<ElementType>::operator()(const std::initializer_list<uint64>& RowIndexList, const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol)
{
#if defined(MDK_Matrix_Operator_CheckBound)

    for (auto it = RowIndexList.begin(); it != RowIndexList.end(); ++it)
    {
        if (*it >= m_ColNumber)
        {
            mdkError << "Invalid ColIndexList @ mdkMatrix::operator()({RowIndexList}, ALL)" << '\n';
            mdkShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

#endif

    mdkShadowMatrix<ElementType> tempShadowMatrix(*this, RowIndexList, ALL_Symbol);

    return tempShadowMatrix;
}


template<typename ElementType>
inline
mdkShadowMatrix<ElementType> 
mdkMatrix<ElementType>::operator()(const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol, const std::initializer_list<uint64>& ColIndexList)
{
#if defined(MDK_Matrix_Operator_CheckBound)

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= m_RowNumber)
        {
            mdkError << "Invalid RowIndexList @ mdkMatrix::operator()(ALL, {ColIndexList})" << '\n';
            mdkShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

#endif

    mdkShadowMatrix<ElementType> tempShadowMatrix(*this, ALL_Symbol, ColIndexList);

    return tempShadowMatrix;
}


template<typename ElementType>
inline
mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::operator()(const std::vector<uint64>& RowIndexList, const std::vector<uint64>& ColIndexList)
{
#if defined(MDK_Matrix_Operator_CheckBound)

    for (auto it = RowIndexList.begin(); it != RowIndexList.end(); ++it)
    {
        if (*it >= m_ColNumber)
        {
            mdkError << "Invalid ColIndexList @ mdkMatrix::operator()(std::vector RowIndexList, ColIndexList)" << '\n';
            mdkShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= m_RowNumber)
        {
            mdkError << "Invalid RowIndexList @ mdkMatrix::operator()(std::vector RowIndexList, ColIndexList)" << '\n';
            mdkShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

#endif

    if (RowIndexList.size() == 0 || ColIndexList.size() == 0)
    {
        mdkError << "Empty Input @ mdkMatrix::operator()({RowIndexList}, {ColIndexList})" << '\n';
        mdkShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    mdkShadowMatrix<ElementType> tempShadowMatrix(*this, RowIndexList, ColIndexList);

    return tempShadowMatrix;
}


template<typename ElementType>
inline
mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::operator()(const std::vector<uint64>& RowIndexList, const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol)
{
#if defined(MDK_Matrix_Operator_CheckBound)

    for (auto it = RowIndexList.begin(); it != RowIndexList.end(); ++it)
    {
        if (*it >= m_ColNumber)
        {
            mdkError << "Invalid ColIndexList @ mdkMatrix::operator()({RowIndexList}, ALL)" << '\n';
            mdkShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

#endif

    mdkShadowMatrix<ElementType> tempShadowMatrix(*this, RowIndexList, ALL_Symbol);

    return tempShadowMatrix;
}


template<typename ElementType>
inline
mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::operator()(const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol, const std::vector<uint64>& ColIndexList)
{
#if defined(MDK_Matrix_Operator_CheckBound)

    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= m_RowNumber)
        {
            mdkError << "Invalid RowIndexList @ mdkMatrix::operator()(ALL, {ColIndexList})" << '\n';
            mdkShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

#endif

    mdkShadowMatrix<ElementType> tempShadowMatrix(*this, ALL_Symbol, ColIndexList);

    return tempShadowMatrix;
}


template<typename ElementType>
inline 
mdkShadowMatrix<ElementType> 
mdkMatrix<ElementType>::SubMatrix(uint64 RowIndex_start, uint64 RowIndex_end, uint64 ColIndex_start, uint64 ColIndex_end)
{
    if (RowIndex_start >= m_RowNumber || RowIndex_end >= m_RowNumber
        || ColIndex_start >= m_ColNumber || ColIndex_end >= m_ColNumber
        || RowIndex_start > RowIndex_end || ColIndex_start > ColIndex_end)
    {
        mdkError << "Invalid input @ mdkMatrix::SubMatrix(rs, re, cs, ce)" << '\n';
        mdkShadowMatrix<ElementType> tempShadowMatrix;
        return tempShadowMatrix;
    }

    std::vector<uint64>& RowIndexList(RowIndex_end - RowIndex_start + 1);

    std::vector<uint64>& ColIndexList(ColIndex_end - ColIndex_start + 1);

    mdkShadowMatrix<ElementType> tempShadowMatrix(*this, RowIndexList, ColIndexList);

    return tempShadowMatrix;
}


template<typename ElementType>
inline ElementType& 
mdkMatrix<ElementType>::SubMatrix(uint64 RowIndex, uint64 ColIndex)
{
    return (*this)(RowIndex, ColIndex);
}


template<typename ElementType>
inline
mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::SubMatrix(std::initializer_list<uint64>& LinearIndexList)
{
    return (*this)(LinearIndexList);
}


template<typename ElementType>
inline
mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::SubMatrix(std::initializer_list<uint64>& RowIndexList,
                                  std::initializer_list<uint64>& ColIndexList)
{
    return (*this)(RowIndexList, ColIndexList);
}


template<typename ElementType>
inline 
mdkShadowMatrix<ElementType> 
mdkMatrix<ElementType>::SubMatrix(const std::initializer_list<uint64>& RowIndexList,
                                  const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol)
{
    return (*this)(RowIndexList, ALL_Symbol);
}


template<typename ElementType>
inline mdkShadowMatrix<ElementType> 
mdkMatrix<ElementType>::SubMatrix(const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol,
                                  const std::initializer_list<uint64>& ColIndexList)
{
    return (*this)(ALL_Symbol, ColIndexList);
}


template<typename ElementType>
inline
mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::SubMatrix(const std::vector<uint64>& LinearIndexList)
{
    return (*this)(LinearIndexList);
}


template<typename ElementType>
inline 
mdkShadowMatrix<ElementType> 
mdkMatrix<ElementType>::SubMatrix(const std::vector<uint64>& RowIndexList,
                                  const std::vector<uint64>& ColIndexList)
{
    return (*this)(RowIndexList, ColIndexList);
}


template<typename ElementType>
inline
mdkShadowMatrix<ElementType> 
mdkMatrix<ElementType>::SubMatrix(const std::vector<uint64>& RowIndexList,
                                  const ALL_Symbol_For_mdkMatrix_Operator& All_Symbol)
{
    return (*this)(RowIndexList, All_Symbol);
}


template<typename ElementType>
inline 
mdkShadowMatrix<ElementType> 
mdkMatrix<ElementType>::SubMatrix(const ALL_Symbol_For_mdkMatrix_Operator&,
                                  const std::vector<uint64>& ColIndexList)
{
    return (*this)(All_Symbol, ColIndexList);
}


template<typename ElementType>
inline
mdkMatrix<ElementType> mdkMatrix<ElementType>::GetSubMatrix(uint64 RowIndex_start, uint64 RowIndex_end,
                                                            uint64 ColIndex_start, uint64 ColIndex_end) const
{
    mdkMatrix<ElementType> tempMatrix; // empty matrix

    tempMatrix.SetTobeTemporaryBeforeReturn();

    if (RowIndex_start >= m_RowNumber || RowIndex_end >= m_RowNumber
        || ColIndex_start >= m_ColNumber || ColIndex_end >= m_ColNumber
        || RowIndex_start > RowIndex_end || ColIndex_start > ColIndex_end)
    {
        mdkError << "Invalid input @ mdkMatrix::GetSubMatrix(rs,re,cs,ce)" << '\n';
        return tempMatrix;
    }

    auto RowNumber = RowIndex_end - RowIndex_start + 1;
    auto ColNumber = ColIndex_end - ColIndex_start + 1;

    tempMatrix.SetSize(RowNumber, ColNumber);

    auto tempRawPointer = tempMatrix.GetElementDataRawPointer();

    auto RawPointer = m_ElementData->data();

    for (uint64 j = ColIndex_start; j <= ColIndex_end; ++j)
    {
        for (uint64 i = RowIndex_start; i <= RowIndex_end; ++i)
        {
            tempRawPointer[0] = RawPointer[j*m_RowNumber + i];

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

    tempMatrix.SetTobeTemporaryBeforeReturn();

    auto RowNumber = uint64(RowIndexList.size());
    auto ColNumber = uint64(ColIndexList.size());

    if (RowNumber == 0 || ColNumber == 0)
    {
        mdkWarning << "RowIndexList or ColIndexList is empty @ mdkMatrix::GetSubMatrix(std::vector RowIndexList, ColIndexList)" << '\n';
        return tempMatrix;
    }

    for (uint64 j = 0; j < ColNumber; ++j)
    {
        if (ColIndexList[j] >= m_ColNumber)
        {
            mdkError << "Invalid ColIndexList @ mdkMatrix::GetSubMatrix(std::vector RowIndexList, ColIndexList)" << '\n';
            return tempMatrix;
        }
    }

    for (uint64 i = 0; i < RowNumber; ++i)
    {
        if (RowIndexList[i] >= m_RowNumber)
        {
            mdkError << "Invalid RowIndexList @ mdkMatrix::GetSubMatrix(std::vector RowIndexList, ColIndexList)" << '\n';
            return tempMatrix;
        }
    }

    tempMatrix.SetSize(RowNumber, ColNumber);

    auto tempRawPointer = tempMatrix.GetElementDataRawPointer();

    auto RawPointer = m_ElementData->data();

    for (uint64 j = 0; j < ColNumber; ++j)
    {
        for (uint64 i = 0; i < RowNumber; ++i)
        {
            uint64 LinearIndex = j*m_RowNumber + i;

            tempRawPointer[0] = RawPointer[LinearIndex];

            ++tempRawPointer;
        }
    }

    return tempMatrix;
}


// Attention:
// this function is used for mdkShadowMatrix.CreateMatrix(OutputMatrix)
// Try not to use it for other purpose because it does not check OutputMatrix
//
template<typename ElementType>
inline
void mdkMatrix<ElementType>::GetSubMatrix(mdkMatrix<ElementType>& OutputMatrix,
                                          const std::vector<uint64>& RowIndexList, 
                                          const std::vector<uint64>& ColIndexList) const
{
    if (this == &OutputMatrix)
    {
        mdkError << "A Matrix tries to get a SubMatrix of itself @ mdkMatrix::GetSubMatrix(OutputMatrix, RowIndexList, ColIndexList)" << '\n';
        return;
    }

    auto RowNumber = uint64(RowIndexList.size());
    
    auto ColNumber = uint64(ColIndexList.size());

    if (RowNumber == 0 || ColNumber == 0)
    {
        mdkWarning << "RowIndexList or ColIndexList is empty @ mdkMatrix::GetSubMatrix(OutputMatrix, RowIndexList, ColIndexList)" << '\n';
        return;
    }

    for (uint64 j = 0; j < ColNumber; ++j)
    {
        if (ColIndexList[j] >= m_ColNumber)
        {
            mdkError << "Invalid ColIndexList @ mdkMatrix::GetSubMatrix(OutputMatrix, RowIndexList, ColIndexList)" << '\n';
            return;
        }
    }

    for (uint64 i = 0; i < RowNumber; ++i)
    {
        if (RowIndexList[i] >= m_RowNumber)
        {
            mdkError << "Invalid RowIndexList @ mdkMatrix::GetSubMatrix(OutputMatrix RowIndexList, ColIndexList)" << '\n';
            return;
        }
    }

    if (OutputMatrix.IsEmpty() == true)
    {
        OutputMatrix.SetSize(RowNumber, ColNumber);
    }

    auto tempRawPointer = OutputMatrix.GetElementDataRawPointer();

    auto RawPointer = m_ElementData->data();

    for (uint64 j = 0; j < ColNumber; ++j)
    {
        for (uint64 i = 0; i < RowNumber; ++i)
        {
            uint64 LinearIndex = j*m_RowNumber + i;

            tempRawPointer[0] = RawPointer[LinearIndex];

            ++tempRawPointer;
        }
    }
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> mdkMatrix<ElementType>::GetSubMatrix(const std::vector<uint64>& RowIndexList, const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol) const
{
    std::vector<uint64> ColIndexList(m_ColNumber);

    for (uint64 i = 0; i < m_ColNumber; ++i)
    {
        ColIndexList[i] = i;
    }

    return this->GetSubMatrix(RowIndexList, ColIndexList);
}


template<typename ElementType>
inline
mdkMatrix<ElementType> mdkMatrix<ElementType>::GetSubMatrix(const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol, const std::vector<uint64>& ColIndexList) const
{
    std::vector<uint64> RowIndexList(m_RowNumber);

    for (uint64 i = 0; i < m_RowNumber; ++i)
    {
        RowIndexList[i] = i;
    }

    return this->GetSubMatrix(RowIndexList, ColIndexList);
}


template<typename ElementType>
inline
mdkMatrix<ElementType> mdkMatrix<ElementType>::GetSubMatrix(const std::vector<uint64>& LinearIndexList) const
{
    mdkMatrix<ElementType> tempMatrix; // empty matrix

    tempMatrix.SetTobeTemporaryBeforeReturn();

    auto ElementNumber_sub = uint64(LinearIndexList.size());

    if (ElementNumber_sub == 0)
    {
        return tempMatrix;
    }

    auto ElementNumber = this->GetElementNumber();

    for (uint64 j = 0; j < ElementNumber_sub; ++j)
    {
        if (LinearIndexList[j] >= m_ElementNumber)
        {
            mdkError << "Invalid LinearIndexList @ mdkMatrix::GetSubMatrix(std::vector LinearIndexList)" << '\n';
            return tempMatrix;
        }
    }

    if (ElementNumber_sub == 0)
    {
        mdkWarning << "Empty LinearIndexList @ mdkMatrix::GetSubMatrix(std::vector LinearIndexList)" << '\n';
        return tempMatrix;
    }

    tempMatrix.SetSize(ElementNumber_sub, 1);

    auto tempRawPointer = tempMatrix.GetElementDataRawPointer();

    auto RawPointer = m_ElementData->data();

    for (uint64 j = 0; j < ElementNumber_sub; ++j)
    {
        tempRawPointer[j] = RawPointer[LinearIndexList[j]];
    }

    return tempMatrix;
}


template<typename ElementType>
inline
mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::Col(uint64 ColIndex)
{
    if (ColIndex >= m_ColNumber)
    {
        mdkError << "Invalid input @ mdkMatrix::Col(ColIndex)" << '\n';
        mdkShadowMatrix<ElementType> tempShadowMatrix;
        
        return tempShadowMatrix;
    }

    mdkShadowMatrix<ElementType> tempShadowMatrix(*this, ALL, {ColIndex});

    return tempShadowMatrix;
}


template<typename ElementType>
inline
mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::Col(std::initializer_list<uint64>& ColIndexList)
{
    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {
        if (*it >= m_ColNumber)
        {
            mdkError << "Invalid ColIndexList @ mdkMatrix::Col({ColIndexList})" << '\n';
            mdkShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    mdkShadowMatrix<ElementType> tempShadowMatrix(*this, ALL, ColIndexList);

    return tempShadowMatrix;
}


template<typename ElementType>
inline
mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::Col(const std::vector<uint64>& ColIndexList)
{
    for (auto it = ColIndexList.begin(); it != ColIndexList.end(); ++it)
    {        
        if (*it >= m_ColNumber)
        {
            mdkError << "Invalid ColIndexList @ mdkMatrix::Col(std::vector ColIndexList)" << '\n';
            mdkShadowMatrix<ElementType> tempShadowMatrix;            
            return tempShadowMatrix;
        }
    }

    mdkShadowMatrix<ElementType> tempShadowMatrix(*this, ALL, ColIndexList);

    return tempShadowMatrix;
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> mdkMatrix<ElementType>::GetCol(uint64 ColIndex) const
{
	mdkMatrix<ElementType> tempMatrix; // empty matrix

	tempMatrix.SetTobeTemporaryBeforeReturn();

	if (ColIndex >= m_ColNumber)
	{
		mdkError << "Invalid input @ mdkMatrix::GetCol(ColIndex)" << '\n';
        return tempMatrix;
	}

    tempMatrix.SetSize(m_RowNumber, 1);

    auto tempRawPointer = tempMatrix.GetElementDataRawPointer();

	auto RawPointer = m_ElementData->data();

	uint64 Index = ColIndex*m_RowNumber;

	for (uint64 i = 0; i < m_RowNumber; ++i)
	{
		tempRawPointer[i] = RawPointer[Index + i];
	}

    return tempMatrix;
}


template<typename ElementType>
inline 
bool mdkMatrix<ElementType>::GetCol(uint64 ColIndex, std::vector<ElementType>& ColData) const
{
	if (ColIndex >= m_ColNumber)
	{
		mdkError << "Invalid input @ mdkMatrix::GetCol(uint64 ColIndex, std::vector<ElementType>& ColData)" << '\n';
		return false;
	}

	ColData.resize(m_RowNumber);

	auto tempRawPointer = ColData.data();

	auto RawPointer = m_ElementData->data();

	uint64 Index = ColIndex*m_RowNumber;

	for (uint64 i = 0; i < m_RowNumber; ++i)
	{
		tempRawPointer[i] = RawPointer[Index + i];
	}

	return true;
}


template<typename ElementType>
inline 
bool mdkMatrix<ElementType>::GetCol(uint64 ColIndex, ElementType* ColData) const
{
	if (ColIndex >= m_ColNumber)
	{
		mdkError << "Invalid input @ mdkMatrix::GetCol(uint64 ColIndex, ElementType* ColData)" << '\n';
		return false;
	}

	auto RawPointer = m_ElementData->data();

	uint64 Index = ColIndex*m_RowNumber;

	for (uint64 i = 0; i < m_RowNumber; ++i)
	{
		ColData[i] = RawPointer[Index + i];
	}

	return true;
}


template<typename ElementType>
template<typename ElementType_input>
inline 
bool mdkMatrix<ElementType>::SetCol(uint64 ColIndex, const mdkMatrix<ElementType_input>& ColData)
{
	if (ColIndex >= m_ColNumber)
	{
		mdkError << "Invalid input @ mdkMatrix::SetCol(ColIndex, mdkMatrix)" << '\n';
		return false;
	}

	auto Size = ColData.GetSize();

	if (Size.ColNumber != 1 || Size.RowNumber != 1)
	{
		mdkError << "Invalid input @ mdkMatrix::SetCol(ColIndex, mdkMatrix)" << '\n';
		return false;
	}

	if (Size.RowNumber > 1 && Size.RowNumber != m_RowNumber)
	{
		mdkError << "Invalid input @ mdkMatrix::SetCol(ColIndex, mdkMatrix)" << '\n';
		return false;
	}

	if (Size.ColNumber > 1 && Size.ColNumber != m_RowNumber)
	{
		mdkError << "Invalid input @ mdkMatrix::SetCol(ColIndex, mdkMatrix)" << '\n';
		return false;
	}

    return this->SetCol(ColIndex, ColData.GetElementDataRawPointer(), std::max(Size.RowNumber, Size.ColNumber));
}


template<typename ElementType>
template<typename ElementType_input>
inline
bool mdkMatrix<ElementType>::SetCol(uint64 ColIndex, const std::initializer_list<ElementType_input>& ColData)
{
    return this->SetCol(ColIndex, ColData.begin(), ColData.size());
}


template<typename ElementType>
template<typename ElementType_input>
inline
bool mdkMatrix<ElementType>::SetCol(uint64 ColIndex, const std::vector<ElementType_input>& ColData)
{
    return this->SetCol(ColIndex, ColData.data(), ColData.size());
}


template<typename ElementType>
template<typename ElementType_input>
inline 
bool mdkMatrix<ElementType>::SetCol(uint64 ColIndex, const ElementType_input* ColData, uint64 Length)
{
    if (ColData == nullptr || Length == 0)
	{
		mdkError << "Empty input @ mdkMatrix::SetCol(ColIndex, const ElementType_input* ColData, uint64 Length)" << '\n';
		return false;
	}
    
    if (ColIndex >= m_ColNumber)
    {
        if (m_IsSizeFixed == true)
        {
            mdkError << "Matrix Size can not be changed @ mdkMatrix::SetCol(ColIndex, const ElementType_input* ColData, uint64 Length)" << '\n';
            return false;
        }
        else
        {
            if (m_RowNumber > 0)
            {
                if (Length != m_RowNumber)
                {
                    mdkError << "Length does not match @ mdkMatrix::SetCol(ColIndex, const ElementType_input* ColData, uint64 Length)" << '\n';
                    return false;
                }
            }
            else
            {
                m_RowNumber = Length;
            }

            if (m_ElementData->capacity() - this->GetElementNumber() < (ColIndex + 2 - m_ColNumber)*m_RowNumber)
            {
                m_ElementData->reserve((m_ColNumber + MDK_Matrix_ColExpansionStep)*m_RowNumber);
            }

            m_ColNumber = ColIndex + 1;

            m_ElementData->resize(m_ColNumber*m_RowNumber);
        }
    }

    auto RawPointer = m_ElementData->data();

    uint64 Index = ColIndex*m_RowNumber;

    // just for referece:
    //
    //for (uint64 i = 0; i < m_RowNumber; ++i)
    //{
    //	RawPointer[Index + i] = ElementType(ColData[i]);
    //}

    auto tempColData = ColData;

    RawPointer += Index;
    for (auto Ptr = RawPointer; Ptr < RawPointer + m_RowNumber; ++Ptr)
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
    if (ColIndex >= m_ColNumber)
    {
        mdkError << "Invalid input @ mdkMatrix::FillCol(ColIndex, Element)" << '\n';
        return false;
    }

    auto RawPointer = m_ElementData->data();

    uint64 Index = ColIndex*m_RowNumber;

    RawPointer += Index;
    for (auto Ptr = RawPointer; Ptr < RawPointer + m_RowNumber; ++Ptr)
    {
        Ptr[0] = Element;
    }

    return true;
}


template<typename ElementType>
template<typename ElementType_input>
inline
bool mdkMatrix<ElementType>::AppendCol(const mdkMatrix<ElementType_input>& ColData)
{
    return this->SetCol(m_ColNumber, ColData);
}


template<typename ElementType>
template<typename ElementType_input>
inline 
bool mdkMatrix<ElementType>::AppendCol(const std::initializer_list<ElementType_input>& ColData)
{
    return this->SetCol(m_ColNumber, ColData);
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool mdkMatrix<ElementType>::AppendCol(const std::vector<ElementType_Input>& ColData)
{
    return this->SetCol(m_ColNumber, ColData);
}


template<typename ElementType>
template<typename ElementType_input>
inline 
bool mdkMatrix<ElementType>::AppendCol(const ElementType_input* ColData, uint64 Length)
{
    return this->SetCol(m_ColNumber, ColData, Length);
}


template<typename ElementType>
inline 
bool mdkMatrix<ElementType>::DeleteCol(uint64 ColIndex)
{
    if (ColIndex >= m_ColNumber)
    {
        mdkError << "Invalid input @ mdkMatrix::DeleteCol(uint64 ColIndex)" << '\n';
        return false;
    }

    m_ElementData->erase(m_ElementData->begin() + ColIndex*m_RowNumber, m_ElementData->begin() + (ColIndex+1)*m_RowNumber);

    m_ColNumber -= 1;

    return true;
}


template<typename ElementType>
inline
bool mdkMatrix<ElementType>::DeleteCol(std::initializer_list<uint64>& ColIndexList)
{
    return this->DeleteCol(ColIndexList.begin(), ColIndexList.size());
}


template<typename ElementType>
inline
bool mdkMatrix<ElementType>::DeleteCol(const std::vector<uint64>& ColIndexList)
{
    if (ColIndexList.size() == 0)
    {
        mdkError << "Empty input @ mdkMatrix::DeleteCol(std::vector ColIndexList)" << '\n';
        return false;
    }

    for (uint64 i = 0; i < ColIndexList.size(); ++i)
    {
        if (ColIndexList[i] >= m_ColNumber)
        {
            mdkError << "Out of bound input @ mdkMatrix::DeleteCol(std::vector ColIndexList)" << '\n';
            return false;
        }
    }

    return this->DeleteCol(ColIndexList.data(), ColIndexList.size());
}


template<typename ElementType>
inline
bool mdkMatrix<ElementType>::DeleteCol(const uint64* ColIndexPtr, uint64 Length)
{
    if (ColIndexPtr == nullptr || Length == 0)
    {
        mdkError << "Empty input @ mdkMatrix::DeleteRow(const uint64* ColIndexPtr, uint64 Length)" << '\n';
        return false;
    }

    if (Length == 1)
    {
        return this->DeleteCol(ColIndexPtr[0]);
    }
    else
    {
        std::vector<uint64> ColIndexList_max_to_min(Length);

        for (uint64 i = 0; i < Length; ++i)
        {
            ColIndexList_max_to_min[i] = ColIndexPtr[i];
        }

        std::sort(ColIndexList_max_to_min.begin(), ColIndexList_max_to_min.end(), [](uint64 a, uint64 b) { return a > b; });

        uint64 Index_prev = ColIndexList_max_to_min[0] + 1;

        for (uint64 i = 0; i < ColIndexList_max_to_min.size(); ++i)
        {
            auto Index_i = ColIndexList_max_to_min[i];

            if (Index_i == Index_prev)
            {
                mdkWarning << "duplicate input @ mdkMatrix::DeleteCol(const uint64* ColIndexPtr, uint64 Length)" << '\n';
            }
            else
            {
                m_ElementData->erase(m_ElementData->begin() + Index_i * m_RowNumber,
                    m_ElementData->begin() + (Index_i + 1)* m_RowNumber);

                Index_prev = Index_i;

                m_ColNumber -= 1;
            }
        }

        return true;
    }
}


template<typename ElementType>
inline
mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::Row(uint64 RowIndex)
{
    if (RowIndex >= m_RowNumber)
    {
        mdkError << "Invalid input @ mdkMatrix::Row(RowIndex)" << '\n';
        mdkShadowMatrix<ElementType> tempShadowMatrix;
        
        return tempShadowMatrix;
    }

    mdkShadowMatrix<ElementType> tempShadowMatrix(*this, {RowIndex}, ALL);

    return tempShadowMatrix;
}


template<typename ElementType>
inline
mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::Row(std::initializer_list<uint64>& RowIndexList)
{
    for (auto it = RowIndexList.begin(); it != RowIndexList.end(); ++it)
    {
        if (*it >= m_RowNumber)
        {
            mdkError << "Invalid RowIndexList @ mdkMatrix::Row({RowIndexList})" << '\n';
            mdkShadowMatrix<ElementType> tempShadowMatrix;
            return tempShadowMatrix;
        }
    }

    mdkShadowMatrix<ElementType> tempShadowMatrix(*this, RowIndexList, ALL);

    return tempShadowMatrix;
}


template<typename ElementType>
inline
mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::Row(const std::vector<uint64>& RowIndexList)
{
    auto RowNumber = uint64(RowIndexList.size());

    for (auto it = RowIndexList.begin(); it != RowIndexList.end(); ++it)
    {
        if (*it >= m_RowNumber)
        {
            mdkError << "Invalid RowIndexList @ mdkMatrix::Row(std::vector RowIndexList)" << '\n';
            mdkShadowMatrix<ElementType> tempShadowMatrix;
            
            return tempShadowMatrix;
        }
    }

    mdkShadowMatrix<ElementType> tempShadowMatrix(*this, RowIndexList, ALL);

    return tempShadowMatrix;
}


template<typename ElementType>
inline
mdkMatrix<ElementType> mdkMatrix<ElementType>::GetRow(uint64 RowIndex) const
{
    mdkMatrix<ElementType> tempMatrix;

	tempMatrix.SetTobeTemporaryBeforeReturn();

	if (RowIndex >= m_RowNumber)
	{
		mdkError << "Invalid input @ mdkMatrix::GetRow(RowIndex)" << '\n';
        return tempMatrix;
	}

    tempMatrix.SetSize(1, m_ColNumber);

    auto tempRawPointer = tempMatrix.GetElementDataRawPointer();

	auto RawPointer = m_ElementData->data();

	uint64 Index = 0;

	for (uint64 j = 0; j < m_ColNumber; ++j)
	{
		tempRawPointer[j] = RawPointer[Index + RowIndex];

		Index += m_RowNumber;
	}

    return tempMatrix;
}


template<typename ElementType>
inline
bool mdkMatrix<ElementType>::GetRow(uint64 RowIndex, std::vector<ElementType>& RowData) const
{
	if (RowIndex >= m_RowNumber)
	{
		mdkError << "Invalid input @ mdkMatrix::GetRow(RowIndex, std::vector<ElementType>& RowData)" << '\n';
		return false;
	}

	RowData.resize(m_ColNumber);

	auto tempRawPointer = RowData.data();

	auto RawPointer = m_ElementData->data();

	uint64 Index = 0;

	for (uint64 j = 0; j < m_ColNumber; ++j)
	{
		tempRawPointer[j] = RawPointer[Index + RowIndex];

		Index += m_RowNumber;
	}

	return true;
}


template<typename ElementType>
inline
bool mdkMatrix<ElementType>::GetRow(uint64 RowIndex, ElementType* RowData) const
{
	if (RowIndex >= m_RowNumber)
	{
		mdkError << "Invalid input @ mdkMatrix GetRow(RowIndex, ElementType* RowData)" << '\n';
		return false;
	}

	auto RawPointer = m_ElementData->data();

	uint64 Index = 0;

	for (uint64 j = 0; j < m_ColNumber; ++j)
	{
		RowData[j] = RawPointer[Index + RowIndex];

		Index += m_RowNumber;
	}

	return true;
}


template<typename ElementType>
template<typename ElementType_input>
inline 
bool mdkMatrix<ElementType>::SetRow(uint64 RowIndex, const mdkMatrix<ElementType_input>& RowData)
{
	auto Size = RowData.GetSize();

	if (Size.ColNumber != 1 && Size.RowNumber != 1)
	{
		mdkError << "Invalid input @ mdkMatrix SetRow(RowIndex,mdkMatrix)" << '\n';
		return false;
	}

	if (Size.RowNumber > 1 && Size.RowNumber != m_ColNumber)
	{
		mdkError << "Invalid input @ mdkMatrix SetRow(RowIndex,mdkMatrix)" << '\n';
		return false;
	}

	if (Size.ColNumber > 1 && Size.ColNumber != m_ColNumber)
	{
		mdkError << "Invalid input @ mdkMatrix SetRow(RowIndex,mdkMatrix)" << '\n';
		return false;
	}

    return this->SetRow(RowIndex, RowData.GetElementDataRawPointer(), std::max(Size.RowNumber, Size.ColNumber));
}


template<typename ElementType>
template<typename ElementType_input>
inline
bool mdkMatrix<ElementType>::SetRow(uint64 RowIndex, const std::initializer_list<ElementType_input>& RowData)
{
    return this->SetRow(RowIndex, RowData.begin(), RowData.size());
}


template<typename ElementType>
template<typename ElementType_input>
inline
bool mdkMatrix<ElementType>::SetRow(uint64 RowIndex, const std::vector<ElementType_input>& RowData)
{
    return this->SetRow(RowIndex, RowData.data(), RowData.size());
}


template<typename ElementType>
template<typename ElementType_input>
inline
bool mdkMatrix<ElementType>::SetRow(uint64 RowIndex, const ElementType_input* RowData, uint64 Length)
{
    if (RowData == nullptr || Length == 0)
    {
        mdkError << "Empty input @ mdkMatrix::SetRow(RowIndex, const ElementType_input* RowData, uint64 Length)" << '\n';
        return false;
    }

    if (RowIndex < m_RowNumber)
    {
        if (Length != m_ColNumber)
        {
            mdkError << "Length does not match @ mdkMatrix::SetRow(RowIndex, const ElementType_input* RowData, uint64 Length)" << '\n';
            return false;
        }

        auto RawPointer = m_ElementData->data();

        uint64 Index = 0;

        for (uint64 j = 0; j < m_ColNumber; ++j)
        {
            RawPointer[Index + RowIndex] = ElementType(RowData[j]);

            Index += m_RowNumber;
        }

        return true;
    }

    // RowIndex >= m_RowNumber --------------------------------------------------------------------------------------------------------------------

    if (m_IsSizeFixed == true)
    {
        mdkError << "Matrix Size can not be changed @ mdkMatrix::SetRow(RowIndex, const ElementType_input* RowData, uint64 Length)" << '\n';
        return false;
    }
    
    if (m_ColNumber > 0)
    {
        if (Length != m_ColNumber)
        {
            mdkError << "Length does not match @ mdkMatrix::SetRow(RowIndex, const ElementType_input* RowData, uint64 Length)" << '\n';
            return false;
        }
    }
    else
    {
        m_ColNumber = Length;
    }

    auto tempElementData = std::make_shared<std::vector<ElementType>>((RowIndex + 1)*m_ColNumber);

    auto tempRawPointer = tempElementData->data();

    auto RawPointer = m_ElementData->data();

    uint64 tempIndex = 0;

    for (uint64 j = 0; j < m_ColNumber; ++j)
    {
        for (uint64 i = 0; i < m_RowNumber; ++i)
        {
            tempRawPointer[tempIndex + i] = RawPointer[0];

            ++RawPointer;
        }

        tempRawPointer[tempIndex + RowIndex] = ElementType(RowData[j]);

        tempIndex += RowIndex + 1;
    }

    m_RowNumber = RowIndex + 1;

    m_ElementData = tempElementData;

    return true;
}


template<typename ElementType>
inline 
bool mdkMatrix<ElementType>::FillRow(uint64 RowIndex, const ElementType& Element)
{
    if (RowIndex >= m_RowNumber || RowData == nullptr)
    {
        mdkError << "Invalid input @ mdkMatrix::SetRow(RowIndex, const ElementType& Element)" << '\n';
        return false;
    }

    auto RawPointer = m_ElementData->data();

    uint64 Index = 0;

    for (uint64 j = 0; j < m_ColNumber; ++j)
    {
        RawPointer[Index + RowIndex] = Element;

        Index += m_RowNumber;
    }

    return true;
}


template<typename ElementType>
template<typename ElementType_input>
inline 
bool mdkMatrix<ElementType>::AppendRow(const mdkMatrix<ElementType_input>& RowData)
{
    return this->SetRow(m_RowNumber, RowData);
}


template<typename ElementType>
template<typename ElementType_input>
inline
bool  mdkMatrix<ElementType>::AppendRow(const std::initializer_list<ElementType_input>& RowData)
{
    return this->SetRow(m_RowNumber, RowData);
}


template<typename ElementType>
template<typename ElementType_input>
inline
bool  mdkMatrix<ElementType>::AppendRow(const std::vector<ElementType_input>& RowData)
{
    return this->SetRow(m_RowNumber, RowData);
}


template<typename ElementType>
template<typename ElementType_input>
inline
bool mdkMatrix<ElementType>::AppendRow(const ElementType_input* RowData, uint64 Length)
{
    return this->SetRow(m_RowNumber, RowData, Length);
}


template<typename ElementType>
inline
bool mdkMatrix<ElementType>::DeleteRow(uint64 RowIndex)
{
    if (RowIndex >= m_RowNumber)
    {
        mdkError << "Invalid input @ mdkMatrix::DeleteRow(uint64 RowIndex)" << '\n';
        return false;
    }

    uint64 tempRowIndex = RowIndex;

    return this->DeleteRow(&tempRowIndex, 1);
}


template<typename ElementType>
inline
bool mdkMatrix<ElementType>::DeleteRow(const std::vector<uint64>& RowIndexList)
{
    if (RowIndexList.size() == 0 || RowIndexList.size() > m_RowNumber)
    {
        mdkError << "Invalid input @ mdkMatrix::DeleteRow(const std::vector<uint64>& RowIndexList)" << '\n';
        return false;
    }

    return this->DeleteRow(RowIndexList.data(), RowIndexList.size());
}


template<typename ElementType>
inline
bool mdkMatrix<ElementType>::DeleteRow(const uint64* RowIndexPtr, uint64 Length)
{
    if (RowIndexPtr == nullptr || Length == 0)
    {
        mdkError << "Invalid input @ mdkMatrix::DeleteRow(const uint64* RowIndexPtr, uint64 Length)" << '\n';
        return false;
    }

    std::vector<uint64> CounterList(m_RowNumber);
    
    for (uint64 i = 0; i < m_RowNumber; ++i)
    {
        CounterList[i] = 0;
    }

    for (uint64 i = 0; i < Length; ++i)
    {
        CounterList[RowIndexPtr[i]] += 1;
    }

    std::vector<uint64> RowIndexList_other;

    for (uint64 i = 0; i < m_RowNumber; ++i)
    {
        if (CounterList[i] == 0)
        {
            RowIndexList_other.push_back(i);
        }
    }

    if (RowIndexList_other.size() > 0)
    {
        (*this) = this->GetSubMatrix(RowIndexList_other, ALL);
    }
    else
    {
        this->Clear();
    }

    return true;
}



template<typename ElementType>
inline
mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::Diangonal()
{
    auto ElementNumber = this->GetElementNumber();

    if (ElementNumber == 0 || m_RowNumber != m_ColNumber)
    {
        mdkError << " Self is empty or not square @ mdkMatrix::Diangonal" << '\n';
        mdkShadowMatrix<ElementType> tempShadowMatrix;     
        return  tempShadowMatrix;
    }

    std::vector<uint64> LinearIndexList(m_ColNumber);

    uint64 Index = 0;

    for (uint64 j = 0; j < m_ColNumber; ++j)
    {
        LinearIndexList[j] = Index + j;

        Index += m_RowNumber;
    }

    mdkShadowMatrix<ElementType> tempShadowMatrix(*this, LinearIndexList);

    return tempShadowMatrix;
}


template<typename ElementType>
inline
mdkMatrix<ElementType> mdkMatrix<ElementType>::GetDiangonal() const
{
    mdkMatrix<ElementType> tempMatrix;

    tempMatrix.SetTobeTemporaryBeforeReturn();

    auto ElementNumber = this->GetElementNumber();

    if (ElementNumber == 0 || m_RowNumber != m_ColNumber)
    {
        mdkError << " Self is empty or not square @ mdkMatrix::GetDiangonal()" << '\n';
        return  tempMatrix;
    }

    tempMatrix.SetSize(m_RowNumber, 1);

    auto tempRawPointer = tempMatrix.GetElementDataRawPointer();

    this->GetDiangonal(tempRawPointer);

    return tempMatrix;
}


template<typename ElementType>
inline
bool mdkMatrix<ElementType>::GetDiangonal(std::vector<ElementType>& DiangonalData) const
{
    auto ElementNumber = this->GetElementNumber();

    if (ElementNumber == 0 || m_RowNumber != m_ColNumber)
    {
        mdkError << " Self is empty or not square @ mdkMatrix::GetDiangonal(std::vector)" << '\n';
        return  false;
    }

    DiangonalData.resize(m_RowNumber);

    auto outputRawPointer = DiangonalData.data();

    return this->GetDiangonal(outputRawPointer);
}


template<typename ElementType>
inline
bool mdkMatrix<ElementType>::GetDiangonal(ElementType* DiangonalData) const
{
    auto ElementNumber = this->GetElementNumber();

    if (ElementNumber == 0 || m_RowNumber != m_ColNumber)
    {
        mdkError << " Self is empty or not square @ mdkMatrix::GetDiangonal(ElementType* DiangonalData)" << '\n';
        return  false;
    }

    auto RawPointer = m_ElementData->data();

    uint64 Index = 0;

    for (uint64 j = 0; j < m_ColNumber; ++j)
    {
        DiangonalData[j] = RawPointer[Index + j];

        Index += m_RowNumber;
    }

    return true;
}


template<typename ElementType>
template<typename ElementType_input>
inline 
bool mdkMatrix<ElementType>::SetDiangonal(const mdkMatrix<ElementType_input>& DiangonalData)
{
	auto Size = DiangonalData.GetSize();

	if (Size.ColNumber != 1 && Size.RowNumber != 1)
	{
		mdkError << "Invalid input @ mdkMatrix::SetDiangonal(Matrix)" << '\n';
		return false;
	}

	if (Size.RowNumber > 1 && Size.RowNumber != m_ColNumber)
	{
		mdkError << "Invalid input @ mdkMatrix::SetDiangonal(Matrix)" << '\n';
		return false;
	}

	if (Size.ColNumber > 1 && Size.ColNumber != m_ColNumber)
	{
		mdkError << "Invalid input @ mdkMatrix::SetDiangonal(Matrix)" << '\n';
		return false;
	}

	return this->SetDiangonal(DiangonalData.GetElementDataRawPointer());
}


template<typename ElementType>
template<typename ElementType_input>
inline
bool mdkMatrix<ElementType>::SetDiangonal(const std::initializer_list<ElementType_input>& DiangonalData)
{
    auto ElementNumber = this->GetElementNumber();

    if (ElementNumber == 0 || m_RowNumber != m_ColNumber)
    {
        mdkError << "Invalid input @ mdkMatrix::SetDiangonal({DiangonalData})" << '\n';
        return false;
    }

    if (DiangonalData.size() != m_RowNumber)
    {
        mdkError << "Invalid input @ mdkMatrix::SetDiangonal({DiangonalData})" << '\n';
        return false;
    }

    auto RawPointer = m_ElementData->data();

    uint64 Index = 0;

    for (uint64 j = 0; j < m_ColNumber; ++j)
    {
        RawPointer[Index + j] = ElementType(DiangonalData.begin()[j]);

        Index += m_RowNumber;
    }

    return true;
}


template<typename ElementType>
template<typename ElementType_input>
inline
bool mdkMatrix<ElementType>::SetDiangonal(const std::vector<ElementType_input>& DiangonalData)
{
    auto ElementNumber = this->GetElementNumber();

	if (ElementNumber == 0 || m_RowNumber != m_ColNumber || DiangonalData.size() != m_RowNumber)
	{
		mdkError << " Self is empty or not square or Input is empty @ mdkMatrix::SetDiangonal(std::vector)" << '\n';
		return false;
	}

	return this->SetDiangonal(DiangonalData.data());
}


template<typename ElementType>
template<typename ElementType_input>
inline
bool mdkMatrix<ElementType>::SetDiangonal(const ElementType_input* DiangonalData)
{
    auto ElementNumber = this->GetElementNumber();

    if (ElementNumber == 0 || m_RowNumber != m_ColNumber)
    {
        mdkError << " Self is empty or not square or Input is empty @ mdkMatrix::SetDiangonal(ElementType_input*)" << '\n';
        return false;
    }

    auto RawPointer = m_ElementData->data();

    uint64 Index = 0;

    for (uint64 j = 0; j < m_ColNumber; ++j)
    {
        RawPointer[Index + j] = ElementType(DiangonalData[j]);

        Index += m_RowNumber;
    }

    return true;
}


template<typename ElementType>
inline
bool mdkMatrix<ElementType>::FillDiangonal(const ElementType& Element)
{
    auto ElementNumber = this->GetElementNumber();

    if (ElementNumber == 0 || m_RowNumber != m_ColNumber)
    {
        mdkError << " Self is empty or not square or Input is empty @ mdkMatrix::FillDiangonal" << '\n';
        return false;
    }

    auto RawPointer = m_ElementData->data();

    uint64 Index = 0;

    for (uint64 j = 0; j < m_ColNumber; ++j)
    {
        RawPointer[Index + j] = Element;

        Index += m_RowNumber;
    }

    return true;
}


// ----------------------- Matrix {+ - * % /}  Matrix ------------------------------------------------//

#if !defined MDK_Enable_GlueMatrix  //-----------------------------------------------------------------------

template<typename ElementType>
inline	
mdkMatrix<ElementType> operator+(const mdkMatrix<ElementType>& MatrixA, const mdkMatrix<ElementType>& MatrixB)
{
    mdkMatrix<ElementType> tempMatrix;

    tempMatrix.SetTobeTemporaryBeforeReturn();

    if (SizeA.RowNumber == 0 || SizeB.RowNumber == 0)
    {
        mdkError << "MatrixA or MatrixB is empty @ mdkMatrix operator+(MatrixA, MatrixB)" << '\n';
        return  tempMatrix;
    }

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        mdkError << "Size does not match @ mdkMatrix operator+(MatrixA, MatrixB)" << '\n';
        return  tempMatrix;
    }

    tempMatrix.SetSize(SizeA.RowNumber, SizeA.ColNumber);

    auto ptrTemp = tempMatrix.GetElementDataRawPointer();

    auto ptrA = MatrixA.GetElementDataRawPointer();

    auto ptrB = MatrixB.GetElementDataRawPointer();

    auto ElementNumber = SizeA.RowNumber*SizeA.ColNumber;

    for (uint64 i = 0; i < ElementNumber; ++i)
    {
        ptrTemp[i] = ptrA[i] + ptrB[i];
    }

    tempMatrix.SetTobeTemporaryBeforeReturn();
	return  tempMatrix;
}


template<typename ElementType>
inline
mdkMatrix<ElementType> operator-(const mdkMatrix<ElementType>& MatrixA, const mdkMatrix<ElementType>& MatrixB)
{
    mdkMatrix<ElementType> tempMatrix;

	auto SizeA = MatrixA.GetSize();

	auto SizeB = MatrixB.GetSize();

    if (SizeA.RowNumber == 0 || SizeB.RowNumber == 0)
    {
        mdkError << "MatrixA or MatrixB is empty @ mdkMatrix operator-(MatrixA, MatrixB)" << '\n';
        tempMatrix.SetTobeTemporaryBeforeReturn();
        return  tempMatrix;
    }

	if (SizeA.ColNumber == 1 && SizeA.RowNumber == 1)
	{
        tempMatrix.share(MatrixA(0) - MatrixB);
        tempMatrix.SetTobeTemporaryBeforeReturn();
        return  tempMatrix;
	}

	if (SizeB.ColNumber == 1 && SizeB.RowNumber == 1)
	{
        tempMatrix.share(MatrixA - MatrixB(0));
        tempMatrix.SetTobeTemporaryBeforeReturn();
        return  tempMatrix;
	}

	if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
	{
		mdkError << "Size does not match @ mdkMatrix operator-(MatrixA, MatrixB)" << '\n';
        tempMatrix.SetTobeTemporaryBeforeReturn();
		return  tempMatrix;
	}

	tempMatrix.SetSize(SizeA.RowNumber, SizeA.ColNumber);

	auto ptrTemp = tempMatrix.GetElementDataRawPointer();

	auto ptrA = MatrixA.GetElementDataRawPointer();

	auto ptrB = MatrixB.GetElementDataRawPointer();

    auto ElementNumber = SizeA.RowNumber*SizeA.ColNumber;

    for (uint64 i = 0; i < ElementNumber; ++i)
	{
        ptrTemp[i] = ptrA[i] - ptrB[i];
	}

    tempMatrix.SetTobeTemporaryBeforeReturn();
	return  tempMatrix;
}

#endif //#if defined MDK_Enable_GlueMatrix ----------------------------------------------------------------------


template<typename ElementType>
inline
mdkMatrix<ElementType> operator*(const mdkMatrix<ElementType>& MatrixA, const mdkMatrix<ElementType>& MatrixB)
{
    mdkMatrix<ElementType> tempMatrix;
    
    auto SizeA = MatrixA.GetSize();

    auto SizeB = MatrixB.GetSize();

    if (SizeA.RowNumber == 0 || SizeB.RowNumber == 0)
    {
        mdkError << "MatrixA or MatrixB is empty @ mdkMatrix operator*(MatrixA, MatrixB)" << '\n';
        tempMatrix.SetTobeTemporaryBeforeReturn();
        return  tempMatrix;
    }

    if (SizeA.ColNumber != SizeB.RowNumber)
    {
        mdkError << "Size does not match @ mdkMatrix operator*(MatrixA, MatrixB)" << '\n';
        tempMatrix.SetTobeTemporaryBeforeReturn();
        return  tempMatrix;
    }

    tempMatrix.SetSize(SizeA.RowNumber, SizeB.ColNumber);

    auto ptrA = MatrixA.GetElementDataSharedPointer()->data();

    auto ptrB = MatrixB.GetElementDataSharedPointer()->data();

    auto ptrTemp = tempMatrix.GetElementDataSharedPointer()->data();

    //--------------------- lapack ------------------------------------------------

    arma::Mat<double> A(ptrA, arma::uword(MatrixA.GetRowNumber()), arma::uword(MatrixA.GetColNumber()), false);
    arma::Mat<double> B(ptrB, arma::uword(MatrixB.GetRowNumber()), arma::uword(MatrixB.GetColNumber()), false);
    arma::Mat<double> C(ptrTemp, arma::uword(tempMatrix.GetRowNumber()), arma::uword(tempMatrix.GetColNumber()), false);

    C = A*B;

    tempMatrix.SetTobeTemporaryBeforeReturn();
    return tempMatrix;

    //--------------------- for-loop : slow ----------------------------------------------------
    uint64 IndexA = 0;

    uint64 IndexB = 0;

    ElementType ZeroElement = MatrixA(0);

    ZeroElement = ZeroElement - ZeroElement;

    for (uint64 j = 0; j < SizeB.ColNumber; ++j)
    {
        for (uint64 i = 0; i < SizeA.RowNumber; ++i)
        {
            auto sum = ZeroElement;

            IndexA = 0;

            for (uint64 k = 0; k < SizeA.ColNumber; ++k)
            {
                //sum += MatrixA(i,k) * MatrixB(k,j);
                //sum += ptrA[k*SizeA.RowNumber + i] * ptrB[j*SizeB.RowNumber + k];
                sum += ptrA[IndexA + i] * ptrB[IndexB + k];

                IndexA += SizeA.RowNumber;
            }

            ptrTemp[0] = sum;

            ++ptrTemp;
        }

        IndexB += SizeB.RowNumber;
    }

    tempMatrix.SetTobeTemporaryBeforeReturn();
    return tempMatrix;   
}


template<typename ElementType>
inline
mdkMatrix<ElementType> operator/(const mdkMatrix<ElementType>& MatrixA, const mdkMatrix<ElementType>& MatrixB)
{
    mdkMatrix<ElementType> tempMatrix;

    tempMatrix.SetTobeTemporaryBeforeReturn();

	auto SizeA = MatrixA.GetSize();

	auto SizeB = MatrixB.GetSize();

    if (SizeA.RowNumber == 0 || SizeB.RowNumber == 0)
    {
        mdkWarning << "MatrixA or MatrixB is empty @ mdkMatrix operator/(MatrixA, MatrixB)" << '\n';
        return  tempMatrix;
    }

    if (SizeA.ColNumber == 1 && SizeA.RowNumber == 1)
    {
        tempMatrix.Share(MatrixB.ElementOperation("/", MatrixA(0)));
        return tempMatrix;
    }

    if (SizeB.ColNumber == 1 && SizeB.RowNumber == 1)
    {
        tempMatrix.share(MatrixA.ElementOperation("/", MatrixB(0)));
        return tempMatrix;
    }

	if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
	{
		mdkError << "Size does not match @ mdkMatrix operator/(MatrixA, MatrixB)" << '\n';
		return  tempMatrix;
	}

	tempMatrix.SetSize(SizeA.RowNumber, SizeA.ColNumber);

	auto ptrTemp = tempMatrix.GetElementDataRawPointer();

	auto ptrA = MatrixA.GetElementDataRawPointer();

	auto ptrB = MatrixB.GetElementDataRawPointer();

    auto ElementNumber = SizeA.RowNumber*SizeA.ColNumber;

    for (uint64 i = 0; i < ElementNumber; ++i)
	{
        ptrTemp[i] = ptrA[i] / ptrB[i];
	}

	return  tempMatrix;
}


//element-wise multiplication
template<typename ElementType>
inline
mdkMatrix<ElementType> operator%(const mdkMatrix<ElementType>& MatrixA, const mdkMatrix<ElementType>& MatrixB)
{
    mdkMatrix<ElementType> tempMatrix;

    tempMatrix.SetTobeTemporaryBeforeReturn();

	auto SizeA = MatrixA.GetSize();

	auto SizeB = MatrixB.GetSize();

    if (SizeA.RowNumber == 0 || SizeB.RowNumber == 0)
    {
        mdkError << "MatrixA or MatrixB is empty @ mdkMatrix operator%(MatrixA, MatrixB)" << '\n';
        return  tempMatrix;
    }

    if (SizeA.ColNumber == 1 && SizeA.RowNumber == 1)
    {
        tempMatrix.Share(MatrixB.ElementOperation("*", MatrixA(0)));
        return tempMatrix;
    }

    if (SizeB.ColNumber == 1 && SizeB.RowNumber == 1)
    {
        tempMatrix.Share(MatrixA.ElementOperation("*", MatrixB(0)));
        return tempMatrix;
    }

	if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
	{
		mdkError << "Size does not match @ mdkMatrix operator%(MatrixA, MatrixB)" << '\n';
		return  tempMatrix;
	}

	tempMatrix.SetSize(SizeA.RowNumber, SizeA.ColNumber);

	auto ptrTemp = tempMatrix.GetElementDataRawPointer();

	auto ptrA = MatrixA.GetElementDataRawPointer();

	auto ptrB = MatrixB.GetElementDataRawPointer();

    auto ElementNumber = SizeA.RowNumber*SizeA.ColNumber;

    for (uint64 i = 0; i < ElementNumber; ++i)
	{
        ptrTemp[i] = ptrA[i] * ptrB[i];
	}

	return  tempMatrix;
}


// ----------------------- Matrix {+ - * /}  Element ------------------------------------------------//

#if !defined MDK_Enable_GlueMatrix  //-----------------------------------------------------------------------

template<typename ElementType>
inline mdkMatrix<ElementType> operator+(const mdkMatrix<ElementType>& MatrixA, const ElementType& ElementB)
{
    mdkMatrix<ElementType> tempMatrix;

    tempMatrix.SetTobeTemporaryBeforeReturn();

    auto SizeA = MatrixA.GetSize();

    if (SizeA.RowNumber == 0)
    {
        mdkError << "MatrixA is empty @ mdkMatrix operator+(MatrixA, ElementB)" << '\n';
        return  tempMatrix;
    }

    tempMatrix.SetSize(SizeA.RowNumber, SizeA.ColNumber);

    auto ptrTemp = tempMatrix.GetElementDataRawPointer();

    auto ptrA = MatrixA.GetElementDataRawPointer();

    for (uint64 i = 0; i < Size.RowNumber*Size.ColNumber; ++i)
    {
        ptrTemp[i] = ptrA[i] + ElementB;
    }

    return tempMatrix;
}


template<typename ElementType>
inline mdkMatrix<ElementType> operator-(const mdkMatrix<ElementType>& MatrixA, const ElementType& ElementB)
{
	mdkMatrix<ElementType> tempMatrix;

	tempMatrix.SetTobeTemporaryBeforeReturn();

	auto SizeA = MatrixA.GetSize();

	if (SizeA.RowNumber == 0)
	{
		mdkError << "MatrixA is empty @ mdkMatrix operator-(MatrixA, ElementB)" << '\n';
		return  tempMatrix;
	}

	tempMatrix.SetSize(SizeA.RowNumber, SizeA.ColNumber);

	auto ptrTemp = tempMatrix.GetElementDataRawPointer();

	auto ptrA = MatrixA.GetElementDataRawPointer();

    auto ElementNumber = Size.RowNumber*Size.ColNumber;

    for (uint64 i = 0; i < ElementNumber; ++i)
	{
        ptrTemp[i] = ptrA[i] - ElementB;
	}

	return tempMatrix;
}


template<typename ElementType>
inline mdkMatrix<ElementType> operator*(const mdkMatrix<ElementType>& MatrixA, const ElementType& ElementB)
{
    mdkMatrix<ElementType> tempMatrix;

    tempMatrix.SetTobeTemporaryBeforeReturn();

    auto SizeA = MatrixA.GetSize();

    if (SizeA.RowNumber == 0)
    {
        mdkError << "MatrixA is empty @ mdkMatrix operator*(MatrixA, ElementB)" << '\n';
        return  tempMatrix;
    }

    tempMatrix.SetSize(SizeA.RowNumber, SizeA.ColNumber);

    auto ptrTemp = tempMatrix.GetElementDataRawPointer();

    auto ptrA = MatrixA.GetElementDataRawPointer();

    auto ElementNumber = Size.RowNumber*Size.ColNumber;

    for (uint64 i = 0; i < ElementNumber; ++i)
    {
        ptrTemp[i] = ptrA[i] * ElementB;
    }

    return tempMatrix;
}


template<typename ElementType>
inline mdkMatrix<ElementType> operator/(const mdkMatrix<ElementType>& MatrixA, const ElementType& ElementB)
{
    mdkMatrix<ElementType> tempMatrix;

    tempMatrix.SetTobeTemporaryBeforeReturn();

    auto SizeA = MatrixA.GetSize();

    if (SizeA.RowNumber == 0)
    {
        mdkError << "MatrixA is empty @ mdkMatrix operator/(MatrixA, ElementB)" << '\n';
        return  tempMatrix;
    }

    tempMatrix.SetSize(SizeA.RowNumber, SizeA.ColNumber);

    auto ptrTemp = tempMatrix.GetElementDataRawPointer();

    auto ptrA = MatrixA.GetElementDataRawPointer();

    auto ElementNumber = Size.RowNumber*Size.ColNumber;

    for (uint64 i = 0; i < ElementNumber; ++i)
    {
        ptrTemp[i] = ptrA[i] / ElementB;
    }

    return tempMatrix;
}

#endif //defined MDK_Enable_GlueMatrix -----------------------------------------------------------------------


// ----------------------- Element {+ - * /} Matrix ------------------------------------------------//


#if !defined MDK_Enable_GlueMatrix  //-----------------------------------------------------------------------

template<typename ElementType>
inline mdkMatrix<ElementType> operator+(const ElementType& ElementA, const mdkMatrix<ElementType>& MatrixB)
{
    mdkMatrix<ElementType> tempMatrix;

    tempMatrix.SetTobeTemporaryBeforeReturn();

    auto SizeB = MatrixB.GetSize();

    if (SizeB.RowNumber == 0)
    {
        mdkError << "MatrixB is empty @ mdkMatrix operator+(ElementA, MatrixB)" << '\n';
        return  tempMatrix;
    }

    tempMatrix.SetSize(SizeB.RowNumber, SizeB.ColNumber);

    auto ptrTemp = tempMatrix.GetElementDataRawPointer();

    auto ptrB = MatrixB.GetElementDataRawPointer();

    auto ElementNumber = SizeB.RowNumber*SizeB.ColNumber;

    for (uint64 i = 0; i < ElementNumber; ++i)
    {
        ptrTemp[i] = ElementA + ptrB[i];
    }

    return tempMatrix;
}


template<typename ElementType>
inline mdkMatrix<ElementType> operator-(const ElementType& ElementA, const mdkMatrix<ElementType>& MatrixB)
{
    mdkMatrix<ElementType> tempMatrix;

    tempMatrix.SetTobeTemporaryBeforeReturn();

    auto SizeB = MatrixB.GetSize();

    if (SizeB.RowNumber == 0)
    {
        mdkError << "MatrixB is empty @ mdkMatrix operator-(ElementA, MatrixB)" << '\n';
        return  tempMatrix;
    }

    tempMatrix.SetSize(SizeB.RowNumber, SizeB.ColNumber);

    auto ptrTemp = tempMatrix.GetElementDataRawPointer();

    auto ptrB = MatrixB.GetElementDataRawPointer();

    auto ElementNumber = SizeB.RowNumber*SizeB.ColNumber;

    for (uint64 i = 0; i < ElementNumber; ++i)
    {
        ptrTemp[i] = ElementA - ptrB[i];
    }

    return tempMatrix;
}


template<typename ElementType>
inline mdkMatrix<ElementType> operator*(const ElementType& ElementA, const mdkMatrix<ElementType>& MatrixB)
{
    mdkMatrix<ElementType> tempMatrix;

    tempMatrix.SetTobeTemporaryBeforeReturn();

    auto SizeB = MatrixB.GetSize();

    if (SizeB.RowNumber == 0)
    {
        mdkError << "MatrixB is empty @ mdkMatrix operator*(ElementA, MatrixB)" << '\n';
        return  tempMatrix;
    }

    tempMatrix.SetSize(SizeB.RowNumber, SizeB.ColNumber);

    auto ptrTemp = tempMatrix.GetElementDataRawPointer();

    auto ptrB = MatrixB.GetElementDataRawPointer();

    auto ElementNumber = SizeB.RowNumber*SizeB.ColNumber;

    for (uint64 i = 0; i < ElementNumber; ++i)
    {
        ptrTemp[i] = ElementA * ptrB[i];
    }

    return tempMatrix;
}

#endif //defined MDK_Enable_GlueMatrix -----------------------------------------------------------------------


template<typename ElementType>
inline mdkMatrix<ElementType> operator/(const ElementType& ElementA, const mdkMatrix<ElementType>& MatrixB)
{
    mdkMatrix<ElementType> tempMatrix;

    tempMatrix.SetTobeTemporaryBeforeReturn();

    auto SizeB = MatrixB.GetSize();

    if (SizeB.RowNumber == 0)
    {
        mdkError << "MatrixB is empty @ mdkMatrix operator/(ElementA, MatrixB)" << '\n';
        return  tempMatrix;
    }

    tempMatrix.SetSize(Size.RowNumber, Size.ColNumber);

    auto ptrTemp = tempMatrix.GetElementDataRawPointer();

    auto ptrB = MatrixB.GetElementDataRawPointer();

    auto ElementNumber = SizeB.RowNumber*SizeB.ColNumber;

    for (uint64 i = 0; i < ElementNumber; ++i)
    {
        ptrTemp[i] = ElementA / ptrB[i];
    }

    return tempMatrix;
}


//---------------------- Matrix {+= -= *= /= %=} Matrix ----------------------------------------//

template<typename ElementType>
inline
void mdkMatrix<ElementType>::operator+=(const mdkMatrix<ElementType>& targetMatrix)
{
    auto Size = targetMatrix.GetSize();

    if (Size.RowNumber == 0)
    {
        mdkError << "targetMatrix is empty @ mdkMatrix::operator+=(Matrix)" << '\n';
        return;
    }

    if (Size.RowNumber == 1 && Size.ColNumber == 1)
	{
		(*this) += targetMatrix(0);

		return;
	}

    if (Size.RowNumber != m_RowNumber || Size.ColNumber != m_ColNumber)
	{
		mdkError << "Size does not match @ mdkMatrix::operator+=(Matrix)" << '\n';
		return;
	}

	auto RawPointer = m_ElementData->data();

	auto targetRawPointer = targetMatrix.GetElementDataRawPointer();

    auto ElementNumber = m_RowNumber*m_ColNumber;

	for (uint64 i = 0; i < ElementNumber; ++i)
	{
		RawPointer[i] += targetRawPointer[i];
	}
}


template<typename ElementType>
inline
void mdkMatrix<ElementType>::operator-=(const mdkMatrix<ElementType>& targetMatrix)
{
    auto Size = targetMatrix.GetSize();

    if (Size.RowNumber == 0)
    {
        mdkWarning << "targetMatrix is empty @ mdkMatrix::operator-=(Matrix)" << '\n';
        return;
    }

    if (Size.RowNumber == 1 && Size.ColNumber == 1)
	{
		(*this) -= targetMatrix(0);

		return;
	}

    if (Size.RowNumber != m_RowNumber || Size.ColNumber != m_ColNumber)
	{
		mdkError << "Size does not match @ mdkMatrix::operator-=(Matrix)" << '\n';
		return;
	}

	auto RawPointer = m_ElementData->data();

	auto targetRawPointer = targetMatrix.GetElementDataRawPointer();

    auto ElementNumber = m_RowNumber*m_ColNumber;

	for (uint64 i = 0; i < ElementNumber; ++i)
	{
		RawPointer[i] -= targetRawPointer[i];
	}
}


template<typename ElementType>
inline
void mdkMatrix<ElementType>::operator*=(const mdkMatrix<ElementType>& targetMatrix)
{
	auto Size = targetMatrix.GetSize();

    if (Size.RowNumber == 0)
	{
        mdkError << "targetMatrix is empty @ mdkMatrix::operator*=(Matrix)" << '\n';
		return;
	}

    if (Size.RowNumber == 1 && Size.ColNumber == 1)
    {
        (*this) *= targetMatrix(0);

        return;
    }

    if (m_IsSizeFixed == true)
    {
        if (Size.RowNumber != m_ColNumber || Size.ColNumber != m_ColNumber)
        {
            mdkError << "Matrix Size can not change @ mdkMatrix::operator*=(Matrix)" << '\n';
            return;
        }
    }

    this->Share((*this) * targetMatrix);
}


template<typename ElementType>
inline
void mdkMatrix<ElementType>::operator/=(const mdkMatrix<ElementType>& targetMatrix)
{
    auto Size = targetMatrix.GetSize();

    if (Size.RowNumber == 0)
    {
        mdkError << "targetMatrix is empty @ mdkMatrix::operator/=(Matrix)" << '\n';
        return;
    }

    if (Size.RowNumber == 1 && Size.ColNumber == 1)
	{
		(*this) /= targetMatrix(0);

		return;
	}

    if (Size.RowNumber != m_RowNumber || Size.ColNumber != m_ColNumber)
	{
		mdkError << "Size does not match @ mdkMatrix::operator/=(Matrix)" << '\n';
		return;
	}

	auto RawPointer = m_ElementData->data();

	auto targetRawPointer = targetMatrix.GetElementDataRawPointer();

    auto ElementNumber = m_RowNumber*m_ColNumber;

	for (uint64 i = 0; i < ElementNumber; ++i)
	{
		RawPointer[i] /= targetRawPointer[i];
	}
}


template<typename ElementType>
inline
void mdkMatrix<ElementType>::operator%=(const mdkMatrix<ElementType>& targetMatrix)
{
    auto Size = targetMatrix.GetSize();

    if (Size.RowNumber == 0)
    {
        mdkError << "targetMatrix is empty @ mdkMatrix::operator%=(Matrix)" << '\n';
        return;
    }

    if (Size.RowNumber == 1 && Size.ColNumber == 1)
    {
        (*this) *= targetMatrix(0);

        return;
    }

    if (Size.RowNumber != m_RowNumber || Size.ColNumber != m_ColNumber)
    {
        mdkError << "Size does not match @ mdkMatrix::operator/=(Matrix)" << '\n';
        return;
    }

    auto RawPointer = m_ElementData->data();

    auto targetRawPointer = targetMatrix.GetElementDataRawPointer();

    auto ElementNumber = m_RowNumber*m_ColNumber;

    for (uint64 i = 0; i < ElementNumber; ++i)
    {
        RawPointer[i] *= targetRawPointer[i];
    }
}


template<typename ElementType>
inline 
void mdkMatrix<ElementType>::operator+=(const mdkGlueMatrix<ElementType>& GlueMatrix)
{
    this->operator+=(GlueMatrix.CreateMatrix());
}


template<typename ElementType>
inline 
void mdkMatrix<ElementType>::operator-=(const mdkGlueMatrix<ElementType>& GlueMatrix)
{
    this->operator-=(GlueMatrix.CreateMatrix());
}


template<typename ElementType>
inline 
void mdkMatrix<ElementType>::operator*=(const mdkGlueMatrix<ElementType>& GlueMatrix)
{
    this->operator*=(GlueMatrix.CreateMatrix());
}


template<typename ElementType>
inline 
void mdkMatrix<ElementType>::operator/=(const mdkGlueMatrix<ElementType>& GlueMatrix)
{
    this->operator/=(GlueMatrix.CreateMatrix());
}


template<typename ElementType>
inline
void mdkMatrix<ElementType>::operator%=(const mdkGlueMatrix<ElementType>& GlueMatrix)
{
    this->operator%=(GlueMatrix.CreateMatrix());
}


template<typename ElementType>
inline 
void mdkMatrix<ElementType>::operator+=(const mdkShadowMatrix<ElementType>& ShadowMatrix)
{
    this->operator+=(mdkShadowMatrix.CreateMatrix());
}


template<typename ElementType>
inline
void mdkMatrix<ElementType>::operator-=(const mdkShadowMatrix<ElementType>& ShadowMatrix)
{
    this->operator-=(mdkShadowMatrix.CreateMatrix());
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
    this->operator/=(mdkShadowMatrix.CreateMatrix());
}


template<typename ElementType>
inline
void mdkMatrix<ElementType>::operator%=(const mdkShadowMatrix<ElementType>& ShadowMatrix)
{
    this->operator%=(mdkShadowMatrix.CreateMatrix());
}


//---------------------- Matrix {+= -= *= /=} Element ----------------------------------------//

template<typename ElementType>
inline	
void mdkMatrix<ElementType>::operator+=(ElementType Element)
{
    auto ElementNumber = m_RowNumber*m_ColNumber;

	if (ElementNumber == 0)
	{
		mdkError << "Self is empty @ mdkMatrix::operator+=(Element)" << '\n';
		return;
	}

	auto BeginPointer = m_ElementData->data();

	for (auto Ptr = BeginPointer; Ptr < BeginPointer + ElementNumber; ++Ptr)
	{
		Ptr[0] += Element;
	}
}


template<typename ElementType>
inline
void mdkMatrix<ElementType>::operator-=(ElementType Element)
{
    auto ElementNumber = m_RowNumber*m_ColNumber;

	if (ElementNumber == 0)
	{
		mdkError << "Self is empty @ mdkMatrix::operator-=(Element)" << '\n';
		return;
	}

	auto BeginPointer = m_ElementData->data();

	for (auto Ptr = BeginPointer; Ptr < BeginPointer + ElementNumber; ++Ptr)
	{
		Ptr[0] -= Element;
	}
}


template<typename ElementType>
inline
void mdkMatrix<ElementType>::operator*=(ElementType Element)
{
    auto ElementNumber = m_RowNumber*m_ColNumber;

	if (ElementNumber == 0)
	{
		mdkError << "Self is empty @ mdkMatrix::operator*=(Element)" << '\n';
		return;
	}

	auto BeginPointer = m_ElementData->data();

	for (auto Ptr = BeginPointer; Ptr < BeginPointer + ElementNumber; ++Ptr)
	{
		Ptr[0] *= Element;
	}
}


template<typename ElementType>
inline
void mdkMatrix<ElementType>::operator/=(ElementType Element)
{
    auto ElementNumber = m_RowNumber*m_ColNumber;

	if (ElementNumber == 0)
	{
		mdkError << "Self is empty @ mdkMatrix::operator/=(Element)" << '\n';
		return;
	}

	if (double(abs(Element)) < 0.0000000001)
	{
		mdkWarning << " abs(Element) < 0.0000000001 @ mdkMatrix::operator/=(Element)" << '\n';
	}

	auto BeginPointer = m_ElementData->data();

	for (auto Ptr = BeginPointer; Ptr < BeginPointer + ElementNumber; ++Ptr)
	{
		Ptr[0] /= Element;
	}
}


//-------------------- element operation {^} -----------------------------------------------------------//

template<typename ElementType>
inline void mdkMatrix<ElementType>::operator^(double value)
{
    auto ElementNumber = m_RowNumber*m_ColNumber;

    if (ElementNumber == 0)
    {
        mdkError << "Self is empty @ mdkMatrix::operator^(value)" << '\n';
        return;
    }

    auto BeginPointer = m_ElementData->data();

    for (auto Ptr = BeginPointer; Ptr < BeginPointer + ElementNumber; ++Ptr)
    {
        Ptr[0] = std::pow(Ptr[0], value);
    }
}


//-------------------- element operation : output a new matrix ------------------------------------------//


template<typename ElementType>
inline
mdkMatrix<ElementType> mdkMatrix<ElementType>::ElementOperation(const char* FunctionName) const
{
    return this->ElementOperation(std::string(FunctionName));
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> mdkMatrix<ElementType>::ElementOperation(const std::string& FunctionName) const
{
	mdkMatrix<ElementType> tempMatrix;

	if (FunctionName == "abs")
	{
        tempMatrix = this->ElementOperation([](ElementType a){return std::abs(a); });
	}
	else if (FunctionName == "sin")
	{
        tempMatrix = this->ElementOperation([](ElementType a){return std::sin(a); });
	}
	else if (FunctionName == "cos")
	{
        tempMatrix = this->ElementOperation([](ElementType a){return std::cos(a); });
	}
	else if (FunctionName == "tan")
	{
        tempMatrix = this->ElementOperation([](ElementType a){return std::tan(a); });
	}
	else if (FunctionName == "sqrt")
	{
        tempMatrix =  this->ElementOperation([](ElementType a){return std::sqrt(a); });
	}
	else
	{
		mdkError << " unknown operator @ mdkMatrix::ElementOperation(std::string FunctionName)" << '\n';
	}

    tempMatrix.SetTobeTemporaryBeforeReturn();
    return tempMatrix;
}


template<typename ElementType>
template<typename FunctionType>
inline 
mdkMatrix<ElementType> mdkMatrix<ElementType>::ElementOperation(FunctionType Function) const
{
	mdkMatrix<ElementType> tempMatrix;

    auto ElementNumber = this->GetElementNumber();

	if (ElementNumber == 0)
	{
		mdkError << "Self is empty @ mdkMatrix::ElementOperation(Function)" << '\n';
        tempMatrix.SetTobeTemporaryBeforeReturn();
		return tempMatrix;
	}

	tempMatrix.SetSize(m_RowNumber, m_ColNumber);

	auto tempRawPointer = tempMatrix.GetElementDataRawPointer();

	auto RawPointer = m_ElementData->data();

    for (uint64 i = 0; i < ElementNumber; ++i)
	{
		tempRawPointer[i] = Function(RawPointer[i]);
	}

    tempMatrix.SetTobeTemporaryBeforeReturn();
	return tempMatrix;
}



template<typename ElementType>
template<typename ElementType_target>
inline
mdkMatrix<ElementType> mdkMatrix<ElementType>::ElementOperation(const char* FunctionName, const mdkMatrix<ElementType_target>& targetMatrix) const
{
    return this->ElementOperation(std::string(FunctionName), targetMatrix);
}


template<typename ElementType>
template<typename ElementType_target>
inline 
mdkMatrix<ElementType> mdkMatrix<ElementType>::ElementOperation(const std::string& FunctionName, const mdkMatrix<ElementType_target>& targetMatrix) const
{
	mdkMatrix<ElementType> tempMatrix;

	if (FunctionName == "+")
	{
        tempMatrix = this->ElementOperation([](ElementType a, ElementType b){return a + b; }, targetMatrix);
	}
	else if (FunctionName == "-")
	{
        tempMatrix = this->ElementOperation([](ElementType a, ElementType b){return a - b; }, targetMatrix);
	}
	else if (FunctionName == "*")
	{
        tempMatrix = this->ElementOperation([](ElementType a, ElementType b){return a * b; }, targetMatrix);
	}
	else if (FunctionName == "/")
	{
        tempMatrix = this->ElementOperation([](ElementType a, ElementType b){return a / b; }, targetMatrix);
	}
	else if (FunctionName == "^")
	{
        tempMatrix = this->ElementOperation([](ElementType a, ElementType b){return std::pow(a, b); }, targetMatrix);
	}
	else
	{
		mdkError << " unknown operator @ mdkMatrix::ElementOperation(std::string FunctionName, targetMatrix)" << '\n';        
	}

    tempMatrix.SetTobeTemporaryBeforeReturn();
    return tempMatrix;
}


template<typename ElementType>
template<typename FunctionType, typename ElementType_target>
inline 
mdkMatrix<ElementType> mdkMatrix<ElementType>::ElementOperation(FunctionType Function, const mdkMatrix<ElementType_target>& targetMatrix) const
{
	mdkMatrix<ElementType> tempMatrix;

	if (m_ElementNumber == 0)
	{
		mdkError << "self is empty Matrix @ mdkMatrix::ElementOperation(Function, targetMatrix)" << '\n';
        tempMatrix.SetTobeTemporaryBeforeReturn();
		return tempMatrix;
	}

	auto targetMatrixSize = targetMatrix.GetSize();

	if	(targetMatrixSize.ColNumber == 0 || targetMatrixSize.RowNumber == 0)
	{
		mdkError << "empty targetMatrix @ mdkMatrix ElementOperation(Function, targetMatrix)" << '\n';
        tempMatrix.SetTobeTemporaryBeforeReturn();
		return tempMatrix;
	}

	if (targetMatrixSize.RowNumber == 1 && targetMatrixSize.ColNumber == 1)
	{
        tempMatrix = this->ElementOperation(Function, targetMatrix(0));
        tempMatrix.SetTobeTemporaryBeforeReturn();
        return tempMatrix;
	}

	auto Flag_row = 0;

	auto Flag_col = 0;

	auto Flag_full = 0;

	if (targetMatrixSize.ColNumber == m_ColNumber && targetMatrixSize.RowNumber == m_RowNumber)
	{
		Flag_full = 1;
	}
	else if (targetMatrixSize.ColNumber == 1 && targetMatrixSize.RowNumber == m_RowNumber)
	{
		Flag_col = 1;
	}
	else if (targetMatrixSize.ColNumber == m_ColNumber && targetMatrixSize.RowNumber == 1)
	{
		Flag_row = 1;
	}
	else
	{
		mdkError << "Size does not match @ mdkMatrix::ElementOperation(Functor, targetMatrix)" << '\n';
        tempMatrix.SetTobeTemporaryBeforeReturn();
		return tempMatrix;
	}

	tempMatrix.SetSize(m_RowNumber, m_ColNumber);

	auto tempRawPointer = tempMatrix.GetElementDataRawPointer();

	auto RawPointer = m_ElementData->data();

	auto targetRawPointer = targetMatrix.GetElementDataRawPointer();

	if (Flag_full == 1)
	{
        auto ElementNumber = this->GetElementNumber();

		for (uint64 i = 0; i < ElementNumber; ++i)
		{
			tempRawPointer[i] = Function(RawPointer[i], ElementType(targetRawPointer[i]));
		}
	}
	else if (Flag_col == 1)
	{
		for (uint64 j = 0; j < m_ColNumber; ++j)
		{
			for (uint64 i = 0; i < m_RowNumber; ++i)
			{
				tempRawPointer[0] = Function(RawPointer[0], ElementType(targetRawPointer[i]));

				++tempRawPointer;
				++RawPointer;
			}
		}
	}
	else if (Flag_row == 1)
	{
		uint64 tempIndex = 0;

		for (uint64 i = 0; i < m_RowNumber; ++i)
		{
			tempIndex = 0;

			for (uint64 j = 0; j < m_ColNumber; ++j)
			{
				tempRawPointer[tempIndex + i] = Function(RawPointer[tempIndex + i], ElementType(targetRawPointer[i]));

				tempIndex += m_RowNumber;
			}
		}
	}

    tempMatrix.SetTobeTemporaryBeforeReturn();
	return tempMatrix;
}


template<typename ElementType>
inline
mdkMatrix<ElementType> mdkMatrix<ElementType>::ElementOperation(const char* FunctionName, ElementType Element) const
{
    return this->ElementOperation(std::string(FunctionName), Element);
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> mdkMatrix<ElementType>::ElementOperation(const std::string& FunctionName, ElementType Element) const
{
	mdkMatrix<ElementType> tempMatrix;
    
	if (FunctionName == "+")
	{
        tempMatrix = this->ElementOperation([](ElementType a, ElementType b){return a + b; }, Element);
	}
	else if (FunctionName == "-")
	{
        tempMatrix = this->ElementOperation([](ElementType a, ElementType b){return a - b; }, Element);
	}
	else if (FunctionName == "*")
	{
        tempMatrix = this->ElementOperation([](ElementType a, ElementType b){return a * b; }, Element);
	}
	else if (FunctionName == "/")
	{
        tempMatrix = this->ElementOperation([](ElementType a, ElementType b){return a / b; }, Element);
	}
	else if (FunctionName == "^")
	{
        tempMatrix = this->ElementOperation([](ElementType a, ElementType b){return std::pow(a, b); }, Element);
	}
	else
	{
		mdkError << " unknown operator @ mdkMatrix::ElementOperation(std::string Operator, Element)" << '\n';        
	}

    tempMatrix.SetTobeTemporaryBeforeReturn();
    return tempMatrix;
}


template<typename ElementType>
template<typename FunctionType>
inline
mdkMatrix<ElementType> mdkMatrix<ElementType>::ElementOperation(FunctionType Function, ElementType Element) const
{
	mdkMatrix<ElementType> tempMatrix;

	if (m_RowNumber == 0 || m_ColNumber == 0)
	{
		mdkError << "Self is empty @ mdkMatrix::ElementOperation(Functor, Element)" << '\n';
        tempMatrix.SetTobeTemporaryBeforeReturn();
		return tempMatrix;
	}

	tempMatrix.SetSize(m_RowNumber, m_ColNumber);

	auto tempRawPointer = tempMatrix.GetElementDataRawPointer();

	auto RawPointer = m_ElementData->data();

    auto ElementNumber = this->GetElementNumber();

	for (uint64 i = 0; i < ElementNumber; ++i)
	{
		tempRawPointer[i] = Function(RawPointer[i], ElementType(Element));
	}
	
    tempMatrix.SetTobeTemporaryBeforeReturn();
	return tempMatrix;
}


template<typename ElementType>
inline ElementType mdkMatrix<ElementType>::Mean()
{
    auto ElementNumber = this->GetElementNumber();

    if (ElementNumber == 0)
	{
		mdkError << "self is empty Matrix @ mdkMatrix::Mean" << '\n';
		return m_NaNElement;
	}

	auto RawPointer = m_ElementData->data();

	ElementType value = RawPointer[0];

	for (uint64 i = 1; i < ElementNumber; ++i)
	{
		value += RawPointer[i];
	}

	value /= m_ColNumber*m_RowNumber;

	return value;
}


template<typename ElementType>
inline mdkMatrix<ElementType> mdkMatrix<ElementType>::MeanToRow()
{
	mdkMatrix<ElementType> tempMatrix;

	if (m_RowNumber == 0)
	{
		mdkError << "self is empty Matrix @ mdkMatrix::MeanToRow" << '\n';
        tempMatrix.SetTobeTemporaryBeforeReturn();
		return tempMatrix;
	}

	tempMatrix.SetSize(1, m_ColNumber);

	auto tempRawPointer = tempMatrix.GetElementDataRawPointer();

	auto RawPointer = m_ElementData->data();

    ElementType value;

	uint64 Index = 0;

	for (uint64 j = 0; j < m_ColNumber; ++j)
	{
		value = 0;

		for (uint64 i = 0; i < m_RowNumber; ++i)
		{
			value += RawPointer[Index];
			Index += 1;
		}

		value /= m_RowNumber;

		tempRawPointer[j] = value;
	}
	
    tempMatrix.SetTobeTemporaryBeforeReturn();
	return tempMatrix;
}


template<typename ElementType>
inline mdkMatrix<ElementType> mdkMatrix<ElementType>::MeanToCol()
{
	mdkMatrix<ElementType> tempMatrix;

	if (m_ColNumber == 0)
	{
		mdkError << "self is empty Matrix @ mdkMatrix::MeanToCol" << '\n';
        tempMatrix.SetTobeTemporaryBeforeReturn();
		return tempMatrix;
	}

	tempMatrix.SetSize(m_ColNumber, 1);

	auto tempRawPointer = tempMatrix.GetElementDataRawPointer();

	auto RawPointer = m_ElementData->data();

    ElementType value;

	uint64 Index = 0;

	for (uint64 i = 0; i < m_RowNumber; ++i)
	{
		value = 0;

		Index = 0;

		for (uint64 j = 0; j < m_ColNumber; ++j)
		{
			value += RawPointer[Index + i];

			Index += m_RowNumber;
		}

		value /= m_ColNumber;

		tempRawPointer[i] = value;
	}

    tempMatrix.SetTobeTemporaryBeforeReturn();
	return tempMatrix;
}


template<typename ElementType>
inline ElementType mdkMatrix<ElementType>::Sum()
{
    auto ElementNumber = this->GetElementNumber();

	if (ElementNumber == 0)
	{
		mdkError << "self is empty Matrix @ mdkMatrix::Sum" << '\n';
		return 0;
	}

	auto RawPointer = m_ElementData->data();

	ElementType value = RawPointer[0];

	for (uint64 i = 1; i < ElementNumber; ++i)
	{
		value += RawPointer[i];
	}

	return value;
}


template<typename ElementType>
inline mdkMatrix<ElementType> mdkMatrix<ElementType>::SumToRow()
{
	mdkMatrix<ElementType> tempMatrix;

	if (m_RowNumber == 0)
	{
		mdkError << "self is empty Matrix @ mdkMatrix::SumToRow" << '\n';
        tempMatrix.SetTobeTemporaryBeforeReturn();
		return tempMatrix;
	}

	tempMatrix.SetSize(1, m_ColNumber);

	auto tempRawPointer = tempMatrix.GetElementDataRawPointer();

	auto RawPointer = m_ElementData->data();

    ElementType value;

	uint64 Index = 0;

	for (uint64 j = 0; j < m_ColNumber; ++j)
	{
		value = 0;

		for (uint64 i = 0; i < m_RowNumber; ++i)
		{
			value += RawPointer[Index];
			Index += 1;
		}

		tempRawPointer[j] = value;
	}

    tempMatrix.SetTobeTemporaryBeforeReturn();
	return tempMatrix;
}


template<typename ElementType>
inline mdkMatrix<ElementType> mdkMatrix<ElementType>::SumToCol()
{
	mdkMatrix<ElementType> tempMatrix;

	if (m_ColNumber == 0)
	{
		mdkError << "self is empty Matrix @ mdkMatrix::SumToCol" << '\n';
        tempMatrix.SetTobeTemporaryBeforeReturn();
		return tempMatrix;
	}

	tempMatrix.SetSize(m_RowNumber, 1);

	auto tempRawPointer = tempMatrix.GetElementDataRawPointer();

	auto RawPointer = m_ElementData->data();

    ElementType value;

	uint64 Index = 0;

	for (uint64 i = 0; i < m_RowNumber; ++i)
	{
		value = 0;

		Index = 0;

		for (uint64 j = 0; j < m_ColNumber; ++j)
		{
			value += RawPointer[Index + i];

			Index += m_RowNumber;
		}

		tempRawPointer[i] = value;
	}

    tempMatrix.SetTobeTemporaryBeforeReturn();
	return tempMatrix;
}


template<typename ElementType>
inline ElementType mdkMatrix<ElementType>::Max()
{
    auto ElementNumber = this->GetElementNumber();

	if (ElementNumber == 0)
	{
		mdkError << "self is empty Matrix @ mdkMatrix::Max" << '\n';
		return 0;
	}

	auto RawPointer = m_ElementData->data();

	ElementType value = RawPointer[0];

	for (uint64 i = 1; i < ElementNumber; ++i)
	{
		value = std::max(value, RawPointer[i]);
	}

	return value;
}


template<typename ElementType>
inline mdkMatrix<ElementType> mdkMatrix<ElementType>::MaxToRow()
{
	mdkMatrix<ElementType> tempMatrix;

	if (m_RowNumber == 0)
	{
		mdkError << "self is empty Matrix @ mdkMatrix::MaxToRow" << '\n';
        tempMatrix.SetTobeTemporaryBeforeReturn();
		return tempMatrix;
	}

	tempMatrix.SetSize(1, m_ColNumber);

	auto tempRawPointer = tempMatrix.GetElementDataRawPointer();

	auto RawPointer = m_ElementData->data();

	uint64 Index = 0;

	for (uint64 j = 0; j < m_ColNumber; ++j)
	{
		auto value = RawPointer[Index];

		for (uint64 i = 0; i < m_RowNumber; ++i)
		{
			value = std::max(value, RawPointer[Index]);
			Index += 1;
		}

		tempRawPointer[j] = value;
	}

    tempMatrix.SetTobeTemporaryBeforeReturn();
	return tempMatrix;
}


template<typename ElementType>
inline mdkMatrix<ElementType> mdkMatrix<ElementType>::MaxToCol()
{
	mdkMatrix<ElementType> tempMatrix;

	if (m_ColNumber == 0)
	{
		mdkError << "self is empty Matrix @ mdkMatrix::MaxToCol" << '\n';
        tempMatrix.SetTobeTemporaryBeforeReturn();
		return tempMatrix;
	}

	tempMatrix.SetSize(m_ColNumber, 1);

	auto tempRawPointer = tempMatrix.GetElementDataRawPointer();

	auto RawPointer = m_ElementData->data();

	uint64 Index = 0;

	for (uint64 i = 0; i < m_RowNumber; ++i)
	{
		Index = 0;

		auto value = RawPointer[Index + i];

		for (uint64 j = 0; j < m_ColNumber; ++j)
		{
			value = std::max(value, RawPointer[Index + i]);

			Index += m_RowNumber;
		}

		tempRawPointer[i] = value;
	}

    tempMatrix.SetTobeTemporaryBeforeReturn();
	return tempMatrix;
}


template<typename ElementType>
inline ElementType mdkMatrix<ElementType>::Min()
{
    auto ElementNumber = this->GetElementNumber();

	if (ElementNumber == 0)
	{
		mdkError << "self is empty Matrix @ mdkMatrix::Min" << '\n';
		return 0;
	}

	auto RawPointer = m_ElementData->data();

	ElementType value = RawPointer[0];

	for (uint64 i = 1; i < ElementNumber; ++i)
	{
		value = std::min(value, RawPointer[i]);
	}

	return value;
}


template<typename ElementType>
inline mdkMatrix<ElementType> mdkMatrix<ElementType>::MinToRow()
{
	mdkMatrix<ElementType> tempMatrix;

	if (m_RowNumber == 0)
	{
		mdkError << "self is empty Matrix @ mdkMatrix::MinToRow" << '\n';
        tempMatrix.SetTobeTemporaryBeforeReturn();
		return tempMatrix;
	}

	tempMatrix.SetSize(1, m_ColNumber);

	auto tempRawPointer = tempMatrix.GetElementDataRawPointer();

	auto RawPointer = m_ElementData->data();

	uint64 Index = 0;

	for (uint64 j = 0; j < m_ColNumber; ++j)
	{
		auto value = RawPointer[Index];

		for (uint64 i = 0; i < m_RowNumber; ++i)
		{
			value = std::min(value, RawPointer[Index]);
			Index += 1;
		}

		tempRawPointer[j] = value;
	}

    tempMatrix.SetTobeTemporaryBeforeReturn();
	return tempMatrix;
}


template<typename ElementType>
inline mdkMatrix<ElementType> mdkMatrix<ElementType>::MinToCol()
{
	mdkMatrix<ElementType> tempMatrix;

	if (m_ColNumber == 0)
	{
		mdkError << "self is empty Matrix @ mdkMatrix::MinToCol" << '\n';
        tempMatrix.SetTobeTemporaryBeforeReturn();
		return tempMatrix;
	}

	tempMatrix.SetSize(m_ColNumber, 1);

	auto tempRawPointer = tempMatrix.GetElementDataRawPointer();

	auto RawPointer = m_ElementData->data();

	uint64 Index = 0;

	for (uint64 i = 0; i < m_RowNumber; ++i)
	{
		Index = 0;

		auto value = RawPointer[Index+i];

		for (uint64 j = 0; j < m_ColNumber; ++j)
		{
			value = std::min(value, RawPointer[Index + i]);

			Index += m_RowNumber;
		}

		tempRawPointer[i] = value;
	}

    tempMatrix.SetTobeTemporaryBeforeReturn();
	return tempMatrix;
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> mdkMatrix<ElementType>::GetTranspose()
{
	mdkMatrix<ElementType> tempMatrix;

	if (m_RowNumber == 0)
	{
		mdkError << "self is empty Matrix @ mdkMatrix::GetTranspose" << '\n';
        tempMatrix.SetTobeTemporaryBeforeReturn();
		return tempMatrix;
	}

	tempMatrix.SetSize(m_ColNumber, m_RowNumber);

	auto tempRawPointer = tempMatrix.GetElementDataRawPointer();

	auto RawPointer = m_ElementData->data();

	uint64 Index = 0;

	for (uint64 i = 0; i < m_RowNumber; ++i)
	{
		Index = 0;

		for (uint64 j = 0; j < m_ColNumber; ++j)
		{	
		    tempRawPointer[0] = RawPointer[Index + i];

			Index += m_RowNumber;

			++tempRawPointer;
		}
	}

    tempMatrix.SetTobeTemporaryBeforeReturn();
	return tempMatrix;
}


template<typename ElementType>
inline
mdkMatrix<ElementType> mdkMatrix<ElementType>::Tran()
{
    return this->GetTranspose();
}


template<typename ElementType>
inline
uint64 mdkMatrix<ElementType>::Rank()
{
    if (m_RowNumber == 0)
    {
        mdkError << "Self is empty matrix @ mdkMatrix::Rank" << '\n';
        return 0;
    }

	// call Armadillo 
    
    arma::Mat<ElementType> tempMat(m_ElementData->data(), arma::uword(m_RowNumber), arma::uword(m_ColNumber), false);

	uint64 value = arma::rank(tempMat);

	return value;
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> mdkMatrix<ElementType>::Inv()
{
	mdkMatrix<ElementType> tempMatrix;

    if (m_RowNumber == 0)
    {
        mdkError << "Self is empty matrix @ mdkMatrix::Inv" << '\n';
        tempMatrix.SetTobeTemporaryBeforeReturn();
        return tempMatrix;
    }

	tempMatrix.SetSize(m_RowNumber, m_ColNumber);

	// call Armadillo 

	arma::Mat<ElementType> tempMat(m_ElementData->data(), arma::uword(m_RowNumber), arma::uword(m_ColNumber), false);

	arma::Mat<ElementType> tempInv(tempMatrix.GetElementDataRawPointer(), arma::uword(m_RowNumber), arma::uword(m_ColNumber), false);
	
	tempInv = arma::inv(tempMat);

    tempMatrix.SetTobeTemporaryBeforeReturn();
	return tempMatrix;
}


template<typename ElementType>
inline
mdkMatrix<ElementType> mdkMatrix<ElementType>::PseudoInv()
{
	// call Armadillo 


}


template<typename ElementType>
inline
mdkMatrixSVDResult<ElementType> mdkMatrix<ElementType>::SVD()
{
	mdkMatrixSVDResult<ElementType> Result;

    Result.U.SetTobeTemporaryBeforeReturn();
    Result.S.SetTobeTemporaryBeforeReturn();
    Result.V.SetTobeTemporaryBeforeReturn();

    if (m_RowNumber == 0)
    {
        mdkError << "Self is empty matrix @ mdkMatrix::SVD" << '\n';
        return Result;
    }

	Result.U.SetSize(m_RowNumber, m_ColNumber);
	Result.S.SetSize(m_RowNumber, m_ColNumber);
	Result.V.SetSize(m_RowNumber, m_ColNumber);

	// call Armadillo 

	arma::Mat<ElementType> X(m_ElementData->data(), arma::uword(m_RowNumber), arma::uword(m_ColNumber), false);
	
	arma::Mat<ElementType> U(Result.U.GetElementDataRawPointer(), arma::uword(m_RowNumber), arma::uword(m_ColNumber), false);
	arma::Col<ElementType> S;
	arma::Mat<ElementType> V(Result.V.GetElementDataRawPointer(), arma::uword(m_RowNumber), arma::uword(m_ColNumber), false);

	arma::svd(U, S, V, X);

	Result.S.SetDiangonal(S.memptr());

	return Result;
}

}//end namespace mdk

#endif