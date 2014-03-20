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
mdkMatrix<ElementType>::mdkMatrix(int64 RowNumber, int64 ColNumber)
{
    this->Reset();

    this->Resize(RowNumber, ColNumber);
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
mdkMatrix<ElementType>::mdkMatrix(const mdkMatrix<ElementType>& InputMatrix, mdkObjectCopyConstructionTypeEnum Method = mdkObjectCopyConstructionTypeEnum::DeepCopy)
{
    this->Reset();

    if (Method == mdkObjectCopyConstructionTypeEnum::DeepCopy)
    {
        this->DeepCopy(InputMatrix);
    }
    else
    {
        this->ForceSharedCopy(InputMatrix);
    }
}


// move constructor
template<typename ElementType>
inline
mdkMatrix<ElementType>::mdkMatrix(mdkMatrix<ElementType>&& InputMatrix)
{
    m_DenseMatrix = std::move(InputMatrix.m_DenseMatrix);

    m_SparseMatrix = std::move(InputMatrix.m_SparseMatrix);

    m_IsSizeFixed = InputMatrix.m_IsSizeFixed;
}


template<typename ElementType>
inline
mdkMatrix<ElementType>::mdkMatrix(const mdkShadowMatrix<ElementType>& ShadowMatrix)
{
    // not necessary to use this->Reset()

    m_IsSizeFixed = false;
    this->Take(ShadowMatrix.CreateMatrix());

    m_NaNElement = ShadowMatrix.m_NaNElement;
}


template<typename ElementType>
inline
mdkMatrix<ElementType>::mdkMatrix(const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrix)
{
    // not necessary to use this->Reset()

    m_IsSizeFixed = false;
    this->Take(GlueMatrix.CreateMatrix());

    m_NaNElement = GetMatrixNaNElement(m_NaNElement);
}


template<typename ElementType>
inline
mdkMatrix<ElementType>::mdkMatrix(const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrix)
{
    // not necessary to use this->Reset()

    m_IsSizeFixed = false;
    this->Take(GlueMatrix.CreateMatrix());

    m_NaNElement = GetMatrixNaNElement(m_NaNElement);
}


template<typename ElementType>
inline
mdkMatrix<ElementType>::mdkMatrix(const ElementType* InputElementPointer, int64 InputRowNumber, int64 InputColNumber)
{
    this->Reset();

    m_Is_DenseMatrix == true;

    m_DenseMatrix.DeepCopy(InputElementPointer, InputRowNumber, InputColNumber);
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
    this->DeepCopy(InputMatrix);
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
        if (ElementNumber <= 0)
        {
            mdkError << "Can not change matrix size @ mdkMatrix::operator=(Element)" << '\n';
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
    if (ElementNumber <= 0)
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
void mdkMatrix<ElementType>::operator=(const std::initializer_list<std::initializer_list<ElementType>>& list)
{

    m_DenseMatrix = list;
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
bool mdkMatrix<ElementType>::DeepCopy(const mdkMatrix<ElementType_Input>& InputMatrix)
{
    if (m_Is_DenseMatrix == true)
    {        
        return m_DenseMatrix.DeepCopy(InputMatrix.m_DenseMatrix);
    }
    else
    {
        return m_SparseMatrix.DeepCopy(InputMatrix.m_SparseMatrix);
    }
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool mdkMatrix<ElementType>::DeepCopy(const mdkMatrix<ElementType_Input>* InputMatrix)
{
    if (InputMatrix == nullptr)
    {
        mdkError << "Input is nullptr @ mdkMatrix::DeepCopy(mdkMatrix* InputMatrix)" << '\n';
        return false;
    }

    return this->DeepCopy(*InputMatrix);
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool mdkMatrix<ElementType>::DeepCopy(const ElementType_Input* InputElementPointer, int64 InputRowNumber, int64 InputColNumber)
{
    if (m_Is_DenseMatrix == false)
    {
        if (m_SparseMatrix.IsSizeFixed() == true)
        {
            auto Size = m_SparseMatrix.GetSize();
            if (Size.RowNumber != InputRowNumber || Size.ColNumber != InputColNumber)
            {
                mdkError << "Can not change size @ mdkMatrix::DeepCopy(ElementType_Input*, int64, int64)" << '\n';
                return false;
            }
        }
    }

    m_SparseMatrix.Clear();
    
    m_Is_DenseMatrix = true;

    m_DenseMatrix.Clear();

    m_DenseMatrix.Resize(InputRowNumber, InputColNumber);

    if (m_SparseMatrix.IsSizeFixed() == true)
    {      
        m_DenseMatrix.FixSize();
    }

    return m_DenseMatrix.DeepCopy(InputElementPointer, InputRowNumber, InputColNumber);
}


template<typename ElementType>
inline
bool mdkMatrix<ElementType>::Fill(const ElementType& Element)
{
    if (m_Is_DenseMatrix == false)
    {        
        m_Is_DenseMatrix = true;

        m_DenseMatrix.Reset();

        auto Size = m_SparseMatrix.GetSize();

        m_DenseMatrix.Resize(Size.RowNumber, Size.ColNumber);

        if (m_SparseMatrix.IsSizeFixed() == true)
        {
            m_DenseMatrix.FixSize();
        }

        m_SparseMatrix.Reset();
    }

    return m_DenseMatrix.Fill(Element);
}


template<typename ElementType>
inline
bool mdkMatrix<ElementType>::SharedCopy(mdkMatrix<ElementType>& InputMatrix)
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.SharedCopy(InputMatrix.m_DenseMatrix);
    }
    else
    {
        return m_SparseMatrix.SharedCopy(InputMatrix.m_SparseMatrix);
    }  
}


template<typename ElementType>
inline
bool mdkMatrix<ElementType>::SharedCopy(mdkMatrix<ElementType>* InputMatrix)
{
    if (InputMatrix == nullptr)
    {
        mdkError << "Input is nullptr @ mdkMatrix::SharedCopy(mdkMatrix* InputMatrix)" << '\n';
        return false;
    }

    return this->SharedCopy(*InputMatrix);
}


template<typename ElementType>
inline
void mdkMatrix<ElementType>::ForceSharedCopy(const mdkMatrix<ElementType>& InputMatrix)
{
    if (m_Is_DenseMatrix == true)
    {
        m_DenseMatrix.ForceSharedCopy(InputMatrix.m_DenseMatrix);
    }
    else
    {
        m_SparseMatrix.ForceSharedCopy(InputMatrix.m_SparseMatrix);
    }
}


template<typename ElementType>
inline
bool mdkMatrix<ElementType>::ForceSharedCopy(const mdkMatrix<ElementType>* InputMatrix)
{
    if (InputMatrix == nullptr)
    {
        mdkError << "Input is nullptr @ mdkMatrix::ForceSharedCopy(mdkMatrix* InputMatrix)" << '\n';
        return false;
    }

    return this->ForceSharedCopy(*InputMatrix);
}


template<typename ElementType>
inline
bool mdkMatrix<ElementType>::Take(mdkMatrix<ElementType>& InputMatrix)
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.Take(InputMatrix.m_DenseMatrix);
    }
    else
    {
        return m_SparseMatrix.Take(InputMatrix.m_SparseMatrix);
    }
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
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.Take(ShadowMatrix);
    }
    else
    {
        return m_SparseMatrix.Take(ShadowMatrix);
    }
}


template<typename ElementType>
inline
bool mdkMatrix<ElementType>::Take(const mdkGlueMatrixForLinearCombination<ElementType>& GlueMatrix)
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.Take(GlueMatrix);
    }
    else
    {
        return m_SparseMatrix.Take(GlueMatrix);
    }
}


template<typename ElementType>
inline
bool mdkMatrix<ElementType>::Take(const mdkGlueMatrixForMultiplication<ElementType>& GlueMatrix)
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.Take(GlueMatrix);
    }
    else
    {
        return m_SparseMatrix.Take(GlueMatrix);
    }
}


template<typename ElementType>
inline
void mdkMatrix<ElementType>::Reset()
{
    m_DenseMatrix.Reset();

    m_SparseMatrix.Reset();

    m_IsSizeFixed = false;

    m_Is_DenseMatrix = true;
}


template<typename ElementType>
inline 
void mdkMatrix<ElementType>::Clear()
{
    m_DenseMatrix.Clear();

    m_SparseMatrix.Clear();

    m_IsSizeFixed = false;

    m_Is_DenseMatrix = true;
}


template<typename ElementType>
inline
bool mdkMatrix<ElementType>::Reshape(int64 InputRowNumber, int64 InputColNumber)
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.Reshape(InputRowNumber, InputColNumber);
    }
    else
    {
        return m_SparseMatrix.Reshape(InputRowNumber, InputColNumber);
    }
}


template<typename ElementType>
inline 
bool mdkMatrix<ElementType>::Resize(int64 InputRowNumber, int64 InputColNumber)
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.Resize(InputRowNumber, InputColNumber);
    }
    else
    {
        return m_SparseMatrix.Resize(InputRowNumber, InputColNumber);
    }
}


template<typename ElementType>
inline 
void mdkMatrix<ElementType>::FixSize()
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.FixSize();
    }
    else
    {
        return m_SparseMatrix.FixSize();
    }
}


template<typename ElementType>
inline
bool mdkMatrix<ElementType>::IsSizeFixed() const
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.IsSizeFixed();
    }
    else
    {
        return m_SparseMatrix.IsSizeFixed();
    }
}


template<typename ElementType>
inline
bool mdkMatrix<ElementType>::IsEmpty() const
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.IsEmpty();
    }
    else
    {
        return m_SparseMatrix.IsEmpty();
    }
}


template<typename ElementType>
inline
bool mdkMatrix<ElementType>::IsNoneEmptyVector() const
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.IsNoneEmptyVector();
    }
    else
    {
        return m_SparseMatrix.IsNoneEmptyVector();
    }
}


template<typename ElementType>
inline
bool mdkMatrix<ElementType>::IsNoneEmptyRowVector() const
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.IsNoneEmptyRowVector();
    }
    else
    {
        return m_SparseMatrix.IsNoneEmptyRowVector();
    }
}


template<typename ElementType>
inline
bool mdkMatrix<ElementType>::IsNoneEmptyColVector() const
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.IsNoneEmptyColVector();
    }
    else
    {
        return m_SparseMatrix.IsNoneEmptyColVector();
    }
}



template<typename ElementType>
inline
mdkMatrixSize mdkMatrix<ElementType>::GetSize() const
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.GetSize();
    }
    else
    {
        return m_SparseMatrix.GetSize();
    }
}


template<typename ElementType>
inline
int64 mdkMatrix<ElementType>::GetElementNumber() const
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.GetElementNumber();
    }
    else
    {
        return m_SparseMatrix.GetElementNumber();
    }
}


template<typename ElementType>
inline
int64 mdkMatrix<ElementType>::GetNonZeroElementNumber() const
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.GetNonZeroElementNumber();
    }
    else
    {
        return m_SparseMatrix.GetNonZeroElementNumber();
    }
}


template<typename ElementType>
inline 
int64 mdkMatrix<ElementType>::GetColNumber() const
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.GetColNumber();
    }
    else
    {
        return m_SparseMatrix.GetColNumber();
    }
}


template<typename ElementType>
inline 
int64 mdkMatrix<ElementType>::GetRowNumber() const
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.GetRowNumber();
    }
    else
    {
        return m_SparseMatrix.GetRowNumber();
    }
}


template<typename ElementType>
inline
const ElementType& mdkMatrix<ElementType>::GetNaNElement()  const
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.GetNaNElement();
    }
    else
    {
        return m_SparseMatrix.GetNaNElement();
    }
}


template<typename ElementType>
inline
mdkMatrixElementTypeEnum mdkMatrix<ElementType>::GetElementType() const
{
    return FindMatrixElementType<ElementType>();
}


template<typename ElementType>
inline
ElementType* mdkMatrix<ElementType>::GetElementPointer()
{
    if (m_Is_DenseMatrix == false)
    {
        mdkError << "This function can only be used for dense matrix @ mdkMatrix::GetElementPointer()" << '\n';
        return nullptr;
    }

    return m_DenseMatrix.GetElementPointer();
}


template<typename ElementType>
inline
const ElementType* mdkMatrix<ElementType>::GetElementPointer() const
{
    if (m_Is_DenseMatrix == false)
    {
        mdkError << "This function can only be used for dense matrix @ mdkMatrix::GetElementPointer() const" << '\n';
        return nullptr;
    }

    return m_DenseMatrix.GetElementPointer();
}

//----------- Get/Set Matrix(LinearIndex) -----------------------------------//

// operator[] (): no bound check in release mode

template<typename ElementType>
inline
ElementType& mdkMatrix<ElementType>::operator[](int64 LinearIndex)
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix[LinearIndex];
    }
    else
    {
        return m_SparseMatrix[LinearIndex];
    }
}


template<typename ElementType>
inline
const ElementType& mdkMatrix<ElementType>::operator[](int64 LinearIndex) const
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix[LinearIndex];
    }
    else
    {
        return m_SparseMatrix[LinearIndex];
    }
}


template<typename ElementType>
inline
ElementType& mdkMatrix<ElementType>::operator()(int64 LinearIndex)
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix(LinearIndex);
    }
    else
    {
        return m_SparseMatrix(LinearIndex);
    }
}


template<typename ElementType>
inline
const ElementType& mdkMatrix<ElementType>::operator()(int64 LinearIndex) const
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix(LinearIndex);
    }
    else
    {
        return m_SparseMatrix(LinearIndex);
    }
}

// at(): bound check

template<typename ElementType>
inline
ElementType& mdkMatrix<ElementType>::at(int64 LinearIndex)
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.at(LinearIndex);
    }
    else
    {
        return m_SparseMatrix.at(LinearIndex);
    }
}


template<typename ElementType>
inline
const ElementType& mdkMatrix<ElementType>::at(int64 LinearIndex) const
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.at(LinearIndex);
    }
    else
    {
        return m_SparseMatrix.at(LinearIndex);
    }
}

//----------- Get/Set Matrix(i,j)  ---------------------------------------------//

// operator(): no bound check in release mode

template<typename ElementType>
inline
ElementType& mdkMatrix<ElementType>::operator()(int64 RowIndex, int64 ColIndex)
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix(RowIndex, ColIndex);
    }
    else
    {
        return m_SparseMatrix(RowIndex, ColIndex);
    }
}


template<typename ElementType>
inline
const ElementType& mdkMatrix<ElementType>::operator()(int64 RowIndex, int64 ColIndex) const
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix(RowIndex, ColIndex);
    }
    else
    {
        return m_SparseMatrix(RowIndex, ColIndex);
    }
}


template<typename ElementType>
inline
ElementType& mdkMatrix<ElementType>::at(int64 RowIndex, int64 ColIndex)
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.at(RowIndex, ColIndex);
    }
    else
    {
        return m_SparseMatrix.at(RowIndex, ColIndex);
    }
}


template<typename ElementType>
inline
const ElementType& mdkMatrix<ElementType>::at(int64 RowIndex, int64 ColIndex) const
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.at(RowIndex, ColIndex);
    }
    else
    {
        return m_SparseMatrix.at(RowIndex, ColIndex);
    }
}

//---------------------- Get/Set a set of elements ------------------------------------------------------//

// operator(): no bound check in release mode
//
// note: operator[] is for single element access only, operator[{}] is not defined

template<typename ElementType>
inline
mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::operator()(std::initializer_list<int64>& LinearIndexList)
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix(LinearIndexList);
    }
    else
    {
        return m_SparseMatrix(LinearIndexList);
    }
}


template<typename ElementType>
inline
const mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::operator()(std::initializer_list<int64>& LinearIndexList) const
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix(LinearIndexList);
    }
    else
    {
        return m_SparseMatrix(LinearIndexList);
    }
}


template<typename ElementType>
inline
mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::operator()(const std::vector<int64>& LinearIndexList)
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix(LinearIndexList);
    }
    else
    {
        return m_SparseMatrix(LinearIndexList);
    }
}


template<typename ElementType>
inline
const mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::operator()(const std::vector<int64>& LinearIndexList) const
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix(LinearIndexList);
    }
    else
    {
        return m_SparseMatrix(LinearIndexList);
    }
}


template<typename ElementType>
inline
mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::operator()(const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol)
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix(ALL_Symbol);
    }
    else
    {
        return m_SparseMatrix(ALL_Symbol);
    }
}


template<typename ElementType>
inline
const mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::operator()(const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol) const
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix(ALL_Symbol);
    }
    else
    {
        return m_SparseMatrix(ALL_Symbol);
    }
}


template<typename ElementType>
inline
mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::at(std::initializer_list<int64>& LinearIndexList)
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.at(LinearIndexList);
    }
    else
    {
        return m_SparseMatrix.at(LinearIndexList);
    }
}


template<typename ElementType>
inline
const mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::at(std::initializer_list<int64>& LinearIndexList) const
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.at(LinearIndexList);
    }
    else
    {
        return m_SparseMatrix.at(LinearIndexList);
    }
}


template<typename ElementType>
inline
mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::at(const std::vector<int64>& LinearIndexList)
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.at(LinearIndexList);
    }
    else
    {
        return m_SparseMatrix.at(LinearIndexList);
    }
}


template<typename ElementType>
inline
const mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::at(const std::vector<int64>& LinearIndexList) const
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.at(LinearIndexList);
    }
    else
    {
        return m_SparseMatrix.at(LinearIndexList);
    }
}


template<typename ElementType>
inline
mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::at(const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol)
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.at(ALL_Symbol);
    }
    else
    {
        return m_SparseMatrix.at(ALL_Symbol);
    }
}


template<typename ElementType>
inline
const mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::at(const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol) const
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.at(ALL_Symbol);
    }
    else
    {
        return m_SparseMatrix.at(ALL_Symbol);
    }
}

//---------------------- Get/Set SubMatrix ------------------------------------------------------//

// operator(): no bound check

template<typename ElementType>
inline
mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::operator()(std::initializer_list<int64>& RowIndexList, std::initializer_list<int64>& ColIndexList)
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix(RowIndexList, ColIndexList);
    }
    else
    {
        return m_SparseMatrix(RowIndexList, ColIndexList);
    }
}


template<typename ElementType>
inline
const mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::operator()(std::initializer_list<int64>& RowIndexList, std::initializer_list<int64>& ColIndexList) const
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix(RowIndexList, ColIndexList);
    }
    else
    {
        return m_SparseMatrix(RowIndexList, ColIndexList);
    }
}


template<typename ElementType>
inline
mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::operator()(const std::initializer_list<int64>& RowIndexList, const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol)
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix(RowIndexList, ALL_Symbol);
    }
    else
    {
        return m_SparseMatrix(RowIndexList, ALL_Symbol);
    }
}


template<typename ElementType>
inline
const mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::operator()(const std::initializer_list<int64>& RowIndexList, const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol) const
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix(RowIndexList, ALL_Symbol);
    }
    else
    {
        return m_SparseMatrix(RowIndexList, ALL_Symbol);
    }
}


template<typename ElementType>
inline
mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::operator()(const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol, const std::initializer_list<int64>& ColIndexList)
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix(ALL_Symbol, ColIndexList);
    }
    else
    {
        return m_SparseMatrix(ALL_Symbol, ColIndexList);
    }
}


template<typename ElementType>
inline
const mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::operator()(const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol, const std::initializer_list<int64>& ColIndexList) const
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix(ALL_Symbol, ColIndexList);
    }
    else
    {
        return m_SparseMatrix(ALL_Symbol, ColIndexList);
    }
}


template<typename ElementType>
inline
mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::operator()(const std::vector<int64>& RowIndexList, const std::vector<int64>& ColIndexList)
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix(RowIndexList, ColIndexList);
    }
    else
    {
        return m_SparseMatrix(RowIndexList, ColIndexList);
    }
}


template<typename ElementType>
inline
const mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::operator()(const std::vector<int64>& RowIndexList, const std::vector<int64>& ColIndexList) const 
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix(RowIndexList, ColIndexList);
    }
    else
    {
        return m_SparseMatrix(RowIndexList, ColIndexList);
    }
}


template<typename ElementType>
inline
mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::operator()(const std::vector<int64>& RowIndexList, const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol)
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix(RowIndexList, ALL_Symbol);
    }
    else
    {
        return m_SparseMatrix(RowIndexList, ALL_Symbol);
    }
}


template<typename ElementType>
inline
const mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::operator()(const std::vector<int64>& RowIndexList, const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol) const
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix(RowIndexList, ALL_Symbol);
    }
    else
    {
        return m_SparseMatrix(RowIndexList, ALL_Symbol);
    }
}


template<typename ElementType>
inline
mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::operator()(const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol, const std::vector<int64>& ColIndexList)
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix(ALL_Symbol, ColIndexList);
    }
    else
    {
        return m_SparseMatrix(ALL_Symbol, ColIndexList);
    }
}


template<typename ElementType>
inline
const mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::operator()(const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol, const std::vector<int64>& ColIndexList) const
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix(ALL_Symbol, ColIndexList);
    }
    else
    {
        return m_SparseMatrix(ALL_Symbol, ColIndexList);
    }
}

// at(): bound check

template<typename ElementType>
inline
mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::at(std::initializer_list<int64>& RowIndexList, std::initializer_list<int64>& ColIndexList)
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.at(RowIndexList, ColIndexList);
    }
    else
    {
        return m_SparseMatrix.at(RowIndexList, ColIndexList);
    }
}


template<typename ElementType>
inline
const mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::at(std::initializer_list<int64>& RowIndexList, std::initializer_list<int64>& ColIndexList) const
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.at(RowIndexList, ColIndexList);
    }
    else
    {
        return m_SparseMatrix.at(RowIndexList, ColIndexList);
    }
}


template<typename ElementType>
inline
mdkShadowMatrix<ElementType> 
mdkMatrix<ElementType>::at(const std::initializer_list<int64>& RowIndexList, const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol)
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.at(RowIndexList, ALL_Symbol);
    }
    else
    {
        return m_SparseMatrix.at(RowIndexList, ALL_Symbol);
    }
}


template<typename ElementType>
inline
const mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::at(const std::initializer_list<int64>& RowIndexList, const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol) const
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.at(RowIndexList, ALL_Symbol);
    }
    else
    {
        return m_SparseMatrix.at(RowIndexList, ALL_Symbol);
    }
}


template<typename ElementType>
inline
mdkShadowMatrix<ElementType> 
mdkMatrix<ElementType>::at(const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol, const std::initializer_list<int64>& ColIndexList)
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.at(ALL_Symbol, ColIndexList);
    }
    else
    {
        return m_SparseMatrix.at(ALL_Symbol, ColIndexList);
    }
}


template<typename ElementType>
inline
const mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::at(const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol, const std::initializer_list<int64>& ColIndexList) const
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.at(ALL_Symbol, ColIndexList);
    }
    else
    {
        return m_SparseMatrix.at(ALL_Symbol, ColIndexList);
    }
}


template<typename ElementType>
inline
mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::at(const std::vector<int64>& RowIndexList, const std::vector<int64>& ColIndexList)
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.at(RowIndexList, ColIndexList);
    }
    else
    {
        return m_SparseMatrix.at(RowIndexList, ColIndexList);
    }
}


template<typename ElementType>
inline
const mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::at(const std::vector<int64>& RowIndexList, const std::vector<int64>& ColIndexList) const
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.at(RowIndexList, ColIndexList);
    }
    else
    {
        return m_SparseMatrix.at(RowIndexList, ColIndexList);
    }
}


template<typename ElementType>
inline
mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::at(const std::vector<int64>& RowIndexList, const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol)
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.at(RowIndexList, ALL_Symbol);
    }
    else
    {
        return m_SparseMatrix.at(RowIndexList, ALL_Symbol);
    }
}


template<typename ElementType>
inline
const mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::at(const std::vector<int64>& RowIndexList, const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol) const
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.at(RowIndexList, ALL_Symbol);
    }
    else
    {
        return m_SparseMatrix.at(RowIndexList, ALL_Symbol);
    }
}


template<typename ElementType>
inline
mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::at(const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol, const std::vector<int64>& ColIndexList)
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.at(ALL_Symbol, ColIndexList);
    }
    else
    {
        return m_SparseMatrix.at(ALL_Symbol, ColIndexList);
    }
}


template<typename ElementType>
inline
const mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::at(const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol, const std::vector<int64>& ColIndexList) const
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.at(ALL_Symbol, ColIndexList);
    }
    else
    {
        return m_SparseMatrix.at(ALL_Symbol, ColIndexList);
    }
}

// return SubMatrix as Matrix

template<typename ElementType>
inline
mdkMatrix<ElementType> mdkMatrix<ElementType>::GetSubMatrix(const std::vector<int64>& RowIndexList, const std::vector<int64>& ColIndexList) const
{
    mdkMatrix<ElementType> tempMatrix; // empty matrix

    tempMatrix.m_Is_DenseMatrix = m_Is_DenseMatrix;

    if (m_Is_DenseMatrix == true)
    {
        tempMatrix.m_DenseMatrix = m_DenseMatrix.GetSubMatrix(RowIndexList, ColIndexList);
    }
    else
    {
        tempMatrix.m_SparseMatrix = m_SparseMatrix.GetSubMatrix(RowIndexList, ColIndexList);
    }

    return tempMatrix
}


template<typename ElementType>
inline
bool mdkMatrix<ElementType>::GetSubMatrix(mdkMatrix<ElementType>& OutputMatrix,
                                          const std::vector<int64>& RowIndexList, 
                                          const std::vector<int64>& ColIndexList) const
{
    if (this == &OutputMatrix)
    {
        mdkError << "A Matrix tries to get a SubMatrix of itself @ mdkMatrix::GetSubMatrix(OutputMatrix, RowIndexList, ColIndexList)" << '\n';
        return false;
    }

    mdkMatrix<ElementType> tempMatrix; // empty matrix

    tempMatrix.m_Is_DenseMatrix = m_Is_DenseMatrix;

    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.GetSubMatrix(tempMatrix.m_DenseMatrix, RowIndexList, ColIndexList);
    }
    else
    {
        return m_SparseMatrix.GetSubMatrix(tempMatrix.m_SparseMatrix, RowIndexList, ColIndexList);
    }
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> mdkMatrix<ElementType>::GetSubMatrix(const std::vector<int64>& RowIndexList, const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol) const
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
mdkMatrix<ElementType> mdkMatrix<ElementType>::GetSubMatrix(const ALL_Symbol_For_mdkMatrix_Operator& ALL_Symbol, const std::vector<int64>& ColIndexList) const
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
mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::Col(int64 ColIndex)
{
    return this->at(ALL, { ColIndex });
}


template<typename ElementType>
inline
mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::Col(std::initializer_list<int64>& ColIndexList)
{
    return this->at(ALL, ColIndexList);
}


template<typename ElementType>
inline
mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::Col(const std::vector<int64>& ColIndexList)
{
    return this->at(ALL, ColIndexList);
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> mdkMatrix<ElementType>::GetCol(int64 ColIndex) const
{
    mdkMatrix<ElementType> tempMatrix; // empty matrix

    tempMatrix.m_Is_DenseMatrix = m_Is_DenseMatrix;

    if (m_Is_DenseMatrix == true)
    {
        tempMatrix.m_DenseMatrix = m_DenseMatrix.GetCol(ColIndex);
    }
    else
    {
        tempMatrix.m_SparseMatrix = m_SparseMatrix.GetCol(ColIndex);
    }

    return tempMatrix;
}


template<typename ElementType>
inline 
bool mdkMatrix<ElementType>::GetCol(int64 ColIndex, std::vector<ElementType>& ColData) const
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.GetCol(ColIndex, ColData);
    }
    else
    {
        return m_SparseMatrix.GetCol(ColIndex, ColData);
    }
}


template<typename ElementType>
inline 
bool mdkMatrix<ElementType>::GetCol(int64 ColIndex, ElementType* ColData) const
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.GetCol(ColIndex, ColData);
    }
    else
    {
        return m_SparseMatrix.GetCol(ColIndex, ColData);
    }
}


template<typename ElementType>
template<typename ElementType_Input>
inline 
bool mdkMatrix<ElementType>::SetCol(int64 ColIndex, const mdkMatrix<ElementType_Input>& ColData)
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.GetCol(ColIndex, ColData);
    }
    else
    {
        return m_SparseMatrix.GetCol(ColIndex, ColData);
    }
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool mdkMatrix<ElementType>::SetCol(int64 ColIndex, const std::initializer_list<ElementType_Input>& ColData)
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.SetCol(ColIndex, ColData);
    }
    else
    {
        return m_SparseMatrix.SetCol(ColIndex, ColData);
    }
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool mdkMatrix<ElementType>::SetCol(int64 ColIndex, const std::vector<ElementType_Input>& ColData)
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.SetCol(ColIndex, ColData);
    }
    else
    {
        return m_SparseMatrix.SetCol(ColIndex, ColData);
    }
}


template<typename ElementType>
template<typename ElementType_Input>
inline 
bool mdkMatrix<ElementType>::SetCol(int64 ColIndex, const ElementType_Input* ColData, int64 Length)
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.SetCol(ColIndex, ColData);
    }
    else
    {
        return m_SparseMatrix.SetCol(ColIndex, ColData);
    }
}


template<typename ElementType>
inline 
bool mdkMatrix<ElementType>::FillCol(int64 ColIndex, const ElementType& Element)
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.FillCol(ColIndex, ColData);
    }
    else
    {
        return m_SparseMatrix.FillCol(ColIndex, ColData);
    }
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool mdkMatrix<ElementType>::AppendCol(const mdkMatrix<ElementType_Input>& ColData)
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.AppendCol(ColData);
    }
    else
    {
        return m_SparseMatrix.AppendCol(ColData);
    }
}


template<typename ElementType>
template<typename ElementType_Input>
inline 
bool mdkMatrix<ElementType>::AppendCol(const std::initializer_list<ElementType_Input>& ColData)
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.AppendCol(ColData);
    }
    else
    {
        return m_SparseMatrix.AppendCol(ColData);
    }
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool mdkMatrix<ElementType>::AppendCol(const std::vector<ElementType_Input>& ColData)
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.AppendCol(ColData);
    }
    else
    {
        return m_SparseMatrix.AppendCol(ColData);
    }
}


template<typename ElementType>
template<typename ElementType_Input>
inline 
bool mdkMatrix<ElementType>::AppendCol(const ElementType_Input* ColData, int64 Length)
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.AppendCol(ColData, Length);
    }
    else
    {
        return m_SparseMatrix.AppendCol(ColData, Length);
    }
}


template<typename ElementType>
inline 
bool mdkMatrix<ElementType>::DeleteCol(int64 ColIndex)
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.DeleteCol(ColIndex);
    }
    else
    {
        return m_SparseMatrix.DeleteCol(ColIndex);
    }
}


template<typename ElementType>
inline
bool mdkMatrix<ElementType>::DeleteCol(std::initializer_list<int64>& ColIndexList)
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.DeleteCol(ColIndexList);
    }
    else
    {
        return m_SparseMatrix.DeleteCol(ColIndexList);
    }
}


template<typename ElementType>
inline
bool mdkMatrix<ElementType>::DeleteCol(const std::vector<int64>& ColIndexList)
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.DeleteCol(ColIndexList);
    }
    else
    {
        return m_SparseMatrix.DeleteCol(ColIndexList);
    }
}


template<typename ElementType>
inline
bool mdkMatrix<ElementType>::DeleteCol(const int64* ColIndexListPtr, int64 Length)
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.DeleteCol(ColIndexListPtr, Length);
    }
    else
    {
        return m_SparseMatrix.DeleteCol(ColIndexListPtr, Length);
    }
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool mdkMatrix<ElementType>::InsertCol(int64 ColIndex, const mdkMatrix<ElementType_Input>& ColData)
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.InsertCol(ColIndex, ColData);
    }
    else
    {
        return m_SparseMatrix.InsertCol(ColIndex, ColData);
    }
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool mdkMatrix<ElementType>::InsertCol(int64 ColIndex, const std::initializer_list<ElementType_Input>& ColData)
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.InsertCol(ColIndex, ColData);
    }
    else
    {
        return m_SparseMatrix.InsertCol(ColIndex, ColData);
    }
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool mdkMatrix<ElementType>::InsertCol(int64 ColIndex, const std::vector<ElementType_Input>& ColData)
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.InsertCol(ColIndex, ColData);
    }
    else
    {
        return m_SparseMatrix.InsertCol(ColIndex, ColData);
    }
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool mdkMatrix<ElementType>::InsertCol(int64 ColIndex, const ElementType_Input* ColData, int64 Length)
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.InsertCol(ColIndex, ColData, Length);
    }
    else
    {
        return m_SparseMatrix.InsertCol(ColIndex, ColData, Length);
    }
}


template<typename ElementType>
inline
mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::Row(int64 RowIndex)
{
    return this->at({RowIndex}, ALL);
}


template<typename ElementType>
inline
mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::Row(std::initializer_list<int64>& RowIndexList)
{
    return this->at(RowIndexList, ALL);
}


template<typename ElementType>
inline
mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::Row(const std::vector<int64>& RowIndexList)
{
    return this->at(RowIndexList, ALL);
}


template<typename ElementType>
inline
mdkMatrix<ElementType> mdkMatrix<ElementType>::GetRow(int64 RowIndex) const
{
    mdkMatrix<ElementType> tempMatrix; // empty matrix

    tempMatrix.m_Is_DenseMatrix = m_Is_DenseMatrix;

    if (m_Is_DenseMatrix == true)
    {
        tempMatrix.m_DenseMatrix = m_DenseMatrix.GetRow(RowIndex);
    }
    else
    {
        tempMatrix.m_SparseMatrix = m_SparseMatrix.GetRow(RowIndex);
    }

    return tempMatrix;
}


template<typename ElementType>
inline
bool mdkMatrix<ElementType>::GetRow(int64 RowIndex, std::vector<ElementType>& RowData) const
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.GetRow(RowIndex, RowData);
    }
    else
    {
        return m_SparseMatrix.GetRow(RowIndex, RowData);
    }
}


template<typename ElementType>
inline
bool mdkMatrix<ElementType>::GetRow(int64 RowIndex, ElementType* RowData) const
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.GetRow(RowIndex, RowData);
    }
    else
    {
        return m_SparseMatrix.GetRow(RowIndex, RowData);
    }
}


template<typename ElementType>
template<typename ElementType_Input>
inline 
bool mdkMatrix<ElementType>::SetRow(int64 RowIndex, const mdkMatrix<ElementType_Input>& RowData)
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.SetRow(RowIndex, RowData);
    }
    else
    {
        return m_SparseMatrix.SetRow(RowIndex, RowData);
    }
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool mdkMatrix<ElementType>::SetRow(int64 RowIndex, const std::initializer_list<ElementType_Input>& RowData)
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.SetRow(RowIndex, RowData);
    }
    else
    {
        return m_SparseMatrix.SetRow(RowIndex, RowData);
    }
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool mdkMatrix<ElementType>::SetRow(int64 RowIndex, const std::vector<ElementType_Input>& RowData)
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.SetRow(RowIndex, RowData);
    }
    else
    {
        return m_SparseMatrix.SetRow(RowIndex, RowData);
    }
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool mdkMatrix<ElementType>::SetRow(int64 RowIndex, const ElementType_Input* RowData, int64 Length)
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.SetRow(RowIndex, RowData, Length);
    }
    else
    {
        return m_SparseMatrix.SetRow(RowIndex, RowData, Length);
    }
}


template<typename ElementType>
inline 
bool mdkMatrix<ElementType>::FillRow(int64 RowIndex, const ElementType& Element)
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.FillRow(RowIndex, Element);
    }
    else
    {
        return m_SparseMatrix.FillRow(RowIndex, Element);
    }
}


template<typename ElementType>
template<typename ElementType_Input>
inline 
bool mdkMatrix<ElementType>::AppendRow(const mdkMatrix<ElementType_Input>& RowData)
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.AppendRow(RowData);
    }
    else
    {
        return m_SparseMatrix.AppendRow(RowData);
    }
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool  mdkMatrix<ElementType>::AppendRow(const std::initializer_list<ElementType_Input>& RowData)
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.AppendRow(RowData);
    }
    else
    {
        return m_SparseMatrix.AppendRow(RowData);
    }
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool  mdkMatrix<ElementType>::AppendRow(const std::vector<ElementType_Input>& RowData)
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.AppendRow(RowData);
    }
    else
    {
        return m_SparseMatrix.AppendRow(RowData);
    }
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool mdkMatrix<ElementType>::AppendRow(const ElementType_Input* RowData, int64 Length)
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.AppendRow(RowData, Length);
    }
    else
    {
        return m_SparseMatrix.AppendRow(RowData, Length);
    }
}


template<typename ElementType>
inline
bool mdkMatrix<ElementType>::DeleteRow(int64 RowIndex)
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.DeleteRow(RowIndex);
    }
    else
    {
        return m_SparseMatrix.DeleteRow(RowIndex);
    }
}


template<typename ElementType>
inline
bool mdkMatrix<ElementType>::DeleteRow(const std::vector<int64>& RowIndexList)
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.DeleteRow(RowIndexList);
    }
    else
    {
        return m_SparseMatrix.DeleteRow(RowIndexList);
    }
}


template<typename ElementType>
inline
bool mdkMatrix<ElementType>::DeleteRow(const int64* RowIndexListPtr, int64 Length)
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.DeleteRow(RowIndexListPtr, Length);
    }
    else
    {
        return m_SparseMatrix.DeleteRow(RowIndexListPtr, Length);
    }
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool mdkMatrix<ElementType>::InsertRow(int64 RowIndex, const mdkMatrix<ElementType_Input>& RowData)
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.InsertRow(RowIndex, RowData);
    }
    else
    {
        return m_SparseMatrix.InsertRow(RowIndex, RowData);
    }
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool mdkMatrix<ElementType>::InsertRow(int64 RowIndex, const std::initializer_list<ElementType_Input>& RowData)
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.InsertRow(RowIndex, RowData);
    }
    else
    {
        return m_SparseMatrix.InsertRow(RowIndex, RowData);
    }
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool mdkMatrix<ElementType>::InsertRow(int64 RowIndex, const std::vector<ElementType_Input>& RowData)
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.InsertRow(RowIndex, RowData);
    }
    else
    {
        return m_SparseMatrix.InsertRow(RowIndex, RowData);
    }
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool mdkMatrix<ElementType>::InsertRow(int64 RowIndex, const ElementType_Input* RowData, int64 Length)
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.InsertRow(RowIndex, RowData, Length);
    }
    else
    {
        return m_SparseMatrix.InsertRow(RowIndex, RowData, Length);
    }
}


template<typename ElementType>
inline
mdkShadowMatrix<ElementType>
mdkMatrix<ElementType>::Diangonal()
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.Diangonal();
    }
    else
    {
        return m_SparseMatrix.Diangonal();
    }
}


template<typename ElementType>
inline
mdkMatrix<ElementType> mdkMatrix<ElementType>::GetDiangonal() const
{
    mdkMatrix<ElementType> tempMatrix; 

    tempMatrix.m_Is_DenseMatrix = m_Is_DenseMatrix;

    if (m_Is_DenseMatrix == true)
    {
        tempMatrix.m_DenseMatrix = m_DenseMatrix.GetDiangonal();
    }
    else
    {
        tempMatrix.m_SparseMatrix = m_SparseMatrix.GetDiangonal();
    }

    return tempMatrix;
}


template<typename ElementType>
inline
bool mdkMatrix<ElementType>::GetDiangonal(std::vector<ElementType>& DiangonalData) const
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.GetDiangonal(DiangonalData);
    }
    else
    {
        return m_SparseMatrix.GetDiangonal(DiangonalData);
    }
}


template<typename ElementType>
inline
bool mdkMatrix<ElementType>::GetDiangonal(ElementType* DiangonalData) const
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.GetDiangonal(DiangonalData);
    }
    else
    {
        return m_SparseMatrix.GetDiangonal(DiangonalData);
    }
}


template<typename ElementType>
template<typename ElementType_Input>
inline 
bool mdkMatrix<ElementType>::SetDiangonal(const mdkMatrix<ElementType_Input>& DiangonalData)
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.SetDiangonal(DiangonalData);
    }
    else
    {
        return m_SparseMatrix.SetDiangonal(DiangonalData);
    }
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool mdkMatrix<ElementType>::SetDiangonal(const std::initializer_list<ElementType_Input>& DiangonalData)
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.SetDiangonal(DiangonalData);
    }
    else
    {
        return m_SparseMatrix.SetDiangonal(DiangonalData);
    }
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool mdkMatrix<ElementType>::SetDiangonal(const std::vector<ElementType_Input>& DiangonalData)
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.SetDiangonal(DiangonalData);
    }
    else
    {
        return m_SparseMatrix.SetDiangonal(DiangonalData);
    }
}


template<typename ElementType>
template<typename ElementType_Input>
inline
bool mdkMatrix<ElementType>::SetDiangonal(const ElementType_Input* DiangonalData)
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.SetDiangonal(DiangonalData);
    }
    else
    {
        return m_SparseMatrix.SetDiangonal(DiangonalData);
    }
}


template<typename ElementType>
inline
bool mdkMatrix<ElementType>::FillDiangonal(const ElementType& Element)
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.FillDiangonal(Element);
    }
    else
    {
        return m_SparseMatrix.FillDiangonal(Element);
    }
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
    if (m_Is_DenseMatrix == true)
    {
        m_DenseMatrix += ShadowMatrix;
    }
    else
    {
        m_SparseMatrix += ShadowMatrix;
    }
}


template<typename ElementType>
inline
void mdkMatrix<ElementType>::operator-=(const mdkShadowMatrix<ElementType>& ShadowMatrix)
{
    if (m_Is_DenseMatrix == true)
    {
        m_DenseMatrix -= ShadowMatrix;
    }
    else
    {
        m_SparseMatrix -= ShadowMatrix;
    }
}


template<typename ElementType>
inline 
void mdkMatrix<ElementType>::operator*=(const mdkShadowMatrix<ElementType>& ShadowMatrix)
{
    if (m_Is_DenseMatrix == true)
    {
        m_DenseMatrix *= ShadowMatrix;
    }
    else
    {
        m_SparseMatrix *= ShadowMatrix;
    }


template<typename ElementType>
inline
void mdkMatrix<ElementType>::operator/=(const mdkShadowMatrix<ElementType>& ShadowMatrix)
{
    if (m_Is_DenseMatrix == true)
    {
        m_DenseMatrix /= ShadowMatrix;
    }
    else
    {
        m_SparseMatrix /= ShadowMatrix;
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

    tempMatrix.m_Is_DenseMatrix = m_Is_DenseMatrix;

    if (m_Is_DenseMatrix == true)
    {
        tempMatrix.m_DenseMatrix = m_DenseMatrix.operator^(Element);
    }
    else
    {
        tempMatrix.m_SparseMatrix = m_SparseMatrix.operator^(Element);
    }

    return tempMatrix;
}

//-------------------- element operation {^=} -----------------------------------------------------------//

template<typename ElementType>
inline void mdkMatrix<ElementType>::operator^=(const ElementType& Element)
{
    if (m_Is_DenseMatrix == true)
    {
        m_DenseMatrix.operator^=(Element);
    }
    else
    {
        m_SparseMatrix.operator^=(Element);
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
    return MatrixElementOperation(std::string(OperationName));
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
    return MatrixElementOperation(m_DenseMatrix, Operation, m_DenseMatrix, InputMatrix);
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
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.Mean();
    }
    else
    {
        return m_SparseMatrix.Mean();
    }
}


template<typename ElementType>
inline mdkMatrix<ElementType> mdkMatrix<ElementType>::MeanToRow() const
{
	mdkMatrix<ElementType> tempMatrix;

    tempMatrix.m_Is_DenseMatrix = m_Is_DenseMatrix;

    if (m_Is_DenseMatrix == true)
    {
        tempMatrix.m_DenseMatrix = m_DenseMatrix.MeanToRow();
    }
    else
    {
        tempMatrix.m_SparseMatrix = m_SparseMatrix.MeanToRow();
    }
    
	return tempMatrix;
}


template<typename ElementType>
inline mdkMatrix<ElementType> mdkMatrix<ElementType>::MeanToCol() const
{
    mdkMatrix<ElementType> tempMatrix;

    tempMatrix.m_Is_DenseMatrix = m_Is_DenseMatrix;

    if (m_Is_DenseMatrix == true)
    {
        tempMatrix.m_DenseMatrix = m_DenseMatrix.MeanToCol();
    }
    else
    {
        tempMatrix.m_SparseMatrix = m_SparseMatrix.MeanToCol();
    }

    return tempMatrix;
}


template<typename ElementType>
inline ElementType mdkMatrix<ElementType>::Sum() const
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.Sum();
    }
    else
    {
        return m_SparseMatrix.Sum();
    }
}


template<typename ElementType>
inline mdkMatrix<ElementType> mdkMatrix<ElementType>::SumToRow() const
{
    mdkMatrix<ElementType> tempMatrix;

    tempMatrix.m_Is_DenseMatrix = m_Is_DenseMatrix;

    if (m_Is_DenseMatrix == true)
    {
        tempMatrix.m_DenseMatrix = m_DenseMatrix.SumToRow();
    }
    else
    {
        tempMatrix.m_SparseMatrix = m_SparseMatrix.SumToRow();
    }

    return tempMatrix;
}


template<typename ElementType>
inline mdkMatrix<ElementType> mdkMatrix<ElementType>::SumToCol() const
{
    mdkMatrix<ElementType> tempMatrix;

    tempMatrix.m_Is_DenseMatrix = m_Is_DenseMatrix;

    if (m_Is_DenseMatrix == true)
    {
        tempMatrix.m_DenseMatrix = m_DenseMatrix.SumToCol();
    }
    else
    {
        tempMatrix.m_SparseMatrix = m_SparseMatrix.SumToCol();
    }

    return tempMatrix;
}


template<typename ElementType>
inline ElementType mdkMatrix<ElementType>::Max() const
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.Max();
    }
    else
    {
        return m_SparseMatrix.Max();
    }
}


template<typename ElementType>
inline mdkMatrix<ElementType> mdkMatrix<ElementType>::MaxToRow() const
{
    mdkMatrix<ElementType> tempMatrix;

    tempMatrix.m_Is_DenseMatrix = m_Is_DenseMatrix;

    if (m_Is_DenseMatrix == true)
    {
        tempMatrix.m_DenseMatrix = m_DenseMatrix.MaxToRow();
    }
    else
    {
        tempMatrix.m_SparseMatrix = m_SparseMatrix.MaxToRow();
    }

    return tempMatrix;
}


template<typename ElementType>
inline mdkMatrix<ElementType> mdkMatrix<ElementType>::MaxToCol() const
{
    mdkMatrix<ElementType> tempMatrix;

    tempMatrix.m_Is_DenseMatrix = m_Is_DenseMatrix;

    if (m_Is_DenseMatrix == true)
    {
        tempMatrix.m_DenseMatrix = m_DenseMatrix.MaxToCol();
    }
    else
    {
        tempMatrix.m_SparseMatrix = m_SparseMatrix.MaxToCol();
    }

    return tempMatrix;
}


template<typename ElementType>
inline ElementType mdkMatrix<ElementType>::Min() const
{
    if (m_Is_DenseMatrix == true)
    {
        return m_DenseMatrix.Min();
    }
    else
    {
        return m_SparseMatrix.Min();
    }
}


template<typename ElementType>
inline mdkMatrix<ElementType> mdkMatrix<ElementType>::MinToRow() const
{
    mdkMatrix<ElementType> tempMatrix;

    tempMatrix.m_Is_DenseMatrix = m_Is_DenseMatrix;

    if (m_Is_DenseMatrix == true)
    {
        tempMatrix.m_DenseMatrix = m_DenseMatrix.MinToRow();
    }
    else
    {
        tempMatrix.m_SparseMatrix = m_SparseMatrix.MinToRow();
    }

    return tempMatrix;
}


template<typename ElementType>
inline mdkMatrix<ElementType> mdkMatrix<ElementType>::MinToCol() const
{
    mdkMatrix<ElementType> tempMatrix;

    tempMatrix.m_Is_DenseMatrix = m_Is_DenseMatrix;

    if (m_Is_DenseMatrix == true)
    {
        tempMatrix.m_DenseMatrix = m_DenseMatrix.MinToCol();
    }
    else
    {
        tempMatrix.m_SparseMatrix = m_SparseMatrix.MinToCol();
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
int64 mdkMatrix<ElementType>::Rank() const
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