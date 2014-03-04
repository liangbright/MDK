#ifndef __mdkLinearAlgebra_Part_1_hpp
#define __mdkLinearAlgebra_Part_1_hpp

#include "mdkLinearAlgebra_Part_1.h"

namespace mdk
{

//------------ MatrixAdd, MatrixSubtract, MatrixMultiply, MatrixElementMultiply, MatrixElementDivide ---------------------------------------//

//------------------- OutputMatrixC = MatrixA operator MatrixB --------------------------------------------

template<typename ElementType>
inline 
mdkMatrix<ElementType> MatrixAdd(const mdkMatrix<ElementType>& MatrixA, const mdkMatrix<ElementType>& MatrixB)
{
    mdkMatrix<ElementType> tempMatrix;

    tempMatrix.SetTobeTemporaryBeforeReturn();

    auto SizeA = MatrixA.GetSize();

    auto SizeB = MatrixB.GetSize();

    if (SizeA.RowNumber == 0 || SizeB.RowNumber == 0)
    {
        mdkError << "MatrixA or MatrixB is empty @ mdkLinearAlgebra MatrixAdd(OutputMatrixC, MatrixA, MatrixB)" << '\n';
        return tempMatrix;
    }

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        mdkError << "Size does not match @ mdkLinearAlgebra MatrixAdd(MatrixA, MatrixB)" << '\n';
        return tempMatrix;
    }

    tempMatrix.SetSize(SizeA.RowNumber, SizeA.ColNumber);

    MatrixAdd(tempMatrix, MatrixA, MatrixB);

    return  tempMatrix;
}


template<typename ElementType>
inline
void MatrixAdd(mdkMatrix<ElementType>& OutputMatrixC, const mdkMatrix<ElementType>& MatrixA, const mdkMatrix<ElementType>& MatrixB)
{
    auto SizeA = MatrixA.GetSize();

    auto SizeB = MatrixB.GetSize();

    if (SizeA.RowNumber == 0 || SizeB.RowNumber == 0)
    {
        mdkError << "MatrixA or MatrixB is empty @ mdkLinearAlgebra MatrixAdd(OutputMatrixC, MatrixA, MatrixB)" << '\n';
        return;
    }

    bool Is_A_Scalar = false;

    bool Is_B_Scalar = false;

    if (SizeA.ColNumber == 1 && SizeA.RowNumber == 1)
    {
        Is_A_Scalar = true;
    }

    if (SizeB.ColNumber == 1 && SizeB.RowNumber == 1)
    {        
        Is_B_Scalar = true;
    }

    if (Is_A_Scalar == true && Is_B_Scalar == false)
    {
        mdkWarning << "MatrixA is treated as a scalar @ mdkLinearAlgebra MatrixAdd(OutputMatrixC, MatrixA, MatrixB)" << '\n';
        MatrixAdd(OutputMatrixC, MatrixA(0), MatrixB);
        return;
    }
    else if (Is_A_Scalar == false && Is_B_Scalar == true)
    {
        mdkWarning << "MatrixB is treated as a scalar @ mdkLinearAlgebra MatrixAdd(OutputMatrixC, MatrixA, MatrixB)" << '\n';
        MatrixAdd(OutputMatrixC, MatrixA, MatrixB(0));
        return;
    }

    // only one of MatrixA and MatrixB can be scalar

    auto SizeC = OutputMatrixC.GetSize();

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber
        || SizeA.RowNumber != SizeC.RowNumber || SizeA.ColNumber != SizeC.ColNumber)
    {
        mdkError << "Size does not match @ mdkLinearAlgebra MatrixAdd(OutputMatrixC, MatrixA, MatrixB)" << '\n';
        return;
    }

    auto ptrC = OutputMatrixC.GetElementDataRawPointer();

    auto ptrA = MatrixA.GetElementDataRawPointer();

    auto ptrB = MatrixB.GetElementDataRawPointer();

    auto ElementNumber = SizeA.RowNumber*SizeA.ColNumber;

    for (uint64 i = 0; i < ElementNumber; ++i)
    {
        ptrC[i] = ptrA[i] + ptrB[i];
    }
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> MatrixSubtract(const mdkMatrix<ElementType>& MatrixA, const mdkMatrix<ElementType>& MatrixB)
{
    mdkMatrix<ElementType> tempMatrix;

    tempMatrix.SetTobeTemporaryBeforeReturn();

    auto SizeA = MatrixA.GetSize();

    auto SizeB = MatrixB.GetSize();

    if (SizeA.RowNumber == 0 || SizeB.RowNumber == 0)
    {
        mdkError << "MatrixA or MatrixB is empty @ mdkLinearAlgebra MatrixSubtract(MatrixA, MatrixB)" << '\n';
        return tempMatrix;
    }

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        mdkError << "Size does not match @ mdkLinearAlgebra MatrixSubtract(MatrixA, MatrixB)" << '\n';
        return tempMatrix;
    }

    tempMatrix.SetSize(SizeA.RowNumber, SizeA.ColNumber);

    MatrixSubtract(tempMatrix, MatrixA, MatrixB);

    return  tempMatrix;
}


template<typename ElementType>
inline
void MatrixSubtract(mdkMatrix<ElementType>& OutputMatrixC, const mdkMatrix<ElementType>& MatrixA, const mdkMatrix<ElementType>& MatrixB)
{
    auto SizeA = MatrixA.GetSize();

    auto SizeB = MatrixB.GetSize();

    if (SizeA.RowNumber == 0 || SizeA.ColNumber == 0)
    {
        mdkError << "MatrixA or MatrixB is empty @ mdkLinearAlgebra MatrixSubtract(OutputMatrixC, MatrixA, MatrixB)" << '\n';
        return;
    }

    bool Is_A_Scalar = false;

    bool Is_B_Scalar = false;

    if (SizeA.ColNumber == 1 && SizeA.RowNumber == 1)
    {        
        Is_A_Scalar = true;    
    }

    if (SizeB.ColNumber == 1 && SizeB.RowNumber == 1)
    {        
        Is_B_Scalar = true;
        return;
    }

    if (Is_A_Scalar == true && Is_B_Scalar == false)
    {
        mdkWarning << "MatrixA is treated as a scalar @ mdkLinearAlgebra MatrixSubtract(OutputMatrixC, MatrixA, MatrixB)" << '\n';
        MatrixSubtract(OutputMatrixC, MatrixA(0), MatrixB);
        return;
    }
    else if (Is_A_Scalar == false && Is_B_Scalar == true)
    {
        mdkWarning << "MatrixB is treated as a scalar @ mdkLinearAlgebra MatrixSubtract(OutputMatrixC, MatrixA, MatrixB)" << '\n';
        MatrixSubtract(OutputMatrixC, MatrixA, MatrixB(0));
        return;
    }

    // only one of MatrixA and MatrixB can be scalar

    auto SizeC = OutputMatrixC.GetSize();

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber
        || SizeA.RowNumber != SizeC.RowNumber || SizeA.ColNumber != SizeC.ColNumber)
    {
        mdkError << "Size does not match @ mdkLinearAlgebra MatrixSubtract(OutputMatrixC, MatrixA, MatrixB)" << '\n';
        return;
    }

    auto ptrC = OutputMatrixC.GetElementDataRawPointer();

    auto ptrA = MatrixA.GetElementDataRawPointer();

    auto ptrB = MatrixB.GetElementDataRawPointer();

    auto ElementNumber = SizeA.RowNumber*SizeA.ColNumber;

    for (uint64 i = 0; i < ElementNumber; ++i)
    {
        ptrC[i] = ptrA[i] - ptrB[i];
    }
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> MatrixMultiply(const mdkMatrix<ElementType>& MatrixA, const mdkMatrix<ElementType>& MatrixB)
{
    mdkMatrix<ElementType> tempMatrix;

    tempMatrix.SetTobeTemporaryBeforeReturn();

    auto SizeA = MatrixA.GetSize();

    auto SizeB = MatrixB.GetSize();

    if (SizeA.RowNumber == 0 || SizeB.RowNumber == 0)
    {
        mdkError << "MatrixA or MatrixB is empty @ mdkLinearAlgebra MatrixMultiply(MatrixA, MatrixB)" << '\n';
        return  tempMatrix;
    }

    if (SizeA.ColNumber != SizeB.RowNumber)
    {
        mdkError << "Size does not match @ mdkLinearAlgebra MatrixMultiply(MatrixA, MatrixB)" << '\n';
        return  tempMatrix;
    }

    tempMatrix.SetSize(SizeA.RowNumber, SizeB.ColNumber);

    MatrixMultiply(tempMatrix, MatrixA, MatrixB);

    return  tempMatrix;
}


template<typename ElementType>
inline
void MatrixMultiply(mdkMatrix<ElementType>& OutputMatrixC, const mdkMatrix<ElementType>& MatrixA, const mdkMatrix<ElementType>& MatrixB)
{
    auto ptrA = MatrixA.GetElementDataSharedPointer()->data();

    auto ptrB = MatrixB.GetElementDataSharedPointer()->data();

    auto ptrC = OutputMatrixC.GetElementDataSharedPointer()->data();

    if (ptrC == ptrA || ptrC == ptrB)
    {
        // OutputMatrixC is MatrixA or MatrixB
        // create temp matrix and call this function again
        // 

        mdkMatrix<ElementType> tempMatrix;

        tempMatrix.SetSize(MatrixA.GetRowNumber(), MatrixB.GetColNumber());

        MatrixMultiply(tempMatrix, MatrixA, MatrixB);

        OutputMatrixC.Share(tempMatrix);

        return;
    }

    auto SizeA = MatrixA.GetSize();

    auto SizeB = MatrixB.GetSize();

    if (SizeA.RowNumber == 0 || SizeB.RowNumber == 0)
    {
        mdkError << "MatrixA or MatrixB is empty @ mdkLinearAlgebra MatrixMultiply(OutputMatrixC, MatrixA, MatrixB)" << '\n';
        return;
    }

    bool Is_A_Scalar = false;

    bool Is_B_Scalar = false;

    if (SizeA.ColNumber == 1 && SizeA.RowNumber == 1)
    {
        Is_A_Scalar = true;
    }

    if (SizeB.ColNumber == 1 && SizeB.RowNumber == 1)
    {
        Is_B_Scalar = true;
    }

    if (Is_A_Scalar == true && Is_B_Scalar == false)
    {
        mdkWarning << "MatrixA is treated as a scalar @ mdkLinearAlgebra MatrixMultiply(OutputMatrixC, MatrixA, MatrixB)" << '\n';
        MatrixMultiply(OutputMatrixC, MatrixA(0), MatrixB);
        return;
    }
    else if (Is_A_Scalar == false && Is_B_Scalar == true)
    {
        mdkWarning << "MatrixB is treated as a scalar @ mdkLinearAlgebra MatrixMultiply(OutputMatrixC, MatrixA, MatrixB)" << '\n';
        MatrixMultiply(OutputMatrixC, MatrixA, MatrixB(0));
        return;
    }

    // only one of MatrixA and MatrixB can be scalar

    auto SizeC = OutputMatrixC.GetSize();

    if (SizeA.ColNumber != SizeB.RowNumber || SizeA.RowNumber != SizeC.RowNumber || SizeB.ColNumber != SizeC.ColNumber)
    {
        mdkError << "Size does not match @ mdkLinearAlgebra MatrixMultiply(OutputMatrixC, MatrixA, MatrixB)" << '\n';
        return;
    }

    //--------------------- lapack ------------------------------------------------

    arma::Mat<double> A(ptrA, arma::uword(MatrixA.GetRowNumber()), arma::uword(MatrixA.GetColNumber()), false);
    arma::Mat<double> B(ptrB, arma::uword(MatrixB.GetRowNumber()), arma::uword(MatrixB.GetColNumber()), false);
    arma::Mat<double> C(ptrC, arma::uword(OutputMatrixC.GetRowNumber()), arma::uword(OutputMatrixC.GetColNumber()), false);

    C = A*B;

    return;

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

            ptrC[0] = sum;

            ++ptrC;
        }

        IndexB += SizeB.RowNumber;
    }

}


template<typename ElementType>
inline 
mdkMatrix<ElementType> MatrixElementMultiply(const mdkMatrix<ElementType>& MatrixA, const mdkMatrix<ElementType>& MatrixB)
{
    mdkMatrix<ElementType> tempMatrix;

    tempMatrix.SetTobeTemporaryBeforeReturn();

    auto SizeA = MatrixA.GetSize();

    auto SizeB = MatrixB.GetSize();

    if (SizeA.RowNumber == 0 || SizeB.RowNumber == 0)
    {
        mdkError << "MatrixA or MatrixB is empty @ mdkLinearAlgebra MatrixElementMultiply(MatrixA, MatrixB)" << '\n';
        return;
    }

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        mdkError << "Size does not match @ mdkLinearAlgebra MatrixElementMultiply(MatrixA, MatrixB)" << '\n';
        return tempMatrix;
    }

    tempMatrix.SetSize(SizeA.RowNumber, SizeA.ColNumber);

    MatrixElementMultiply(tempMatrix, MatrixA, MatrixB);

    return  tempMatrix;
}


template<typename ElementType>
inline 
void MatrixElementMultiply(mdkMatrix<ElementType>& OutputMatrixC, const mdkMatrix<ElementType>& MatrixA, const mdkMatrix<ElementType>& MatrixB)
{
    auto SizeA = MatrixA.GetSize();

    auto SizeB = MatrixB.GetSize();

    if (SizeA.RowNumber == 0 || SizeA.ColNumber == 0)
    {
        mdkError << "MatrixA or MatrixB is empty @ mdkLinearAlgebra MatrixElementMultiply(OutputMatrixC, MatrixA, MatrixB)" << '\n';
        return;
    }

    bool Is_A_Scalar = false;

    bool Is_B_Scalar = false;

    if (SizeA.ColNumber == 1 && SizeA.RowNumber == 1)
    {
        Is_A_Scalar = true;
    }

    if (SizeB.ColNumber == 1 && SizeB.RowNumber == 1)
    {
        Is_B_Scalar = true;
        return;
    }

    if (Is_A_Scalar == true && Is_B_Scalar == false)
    {
        mdkWarning << "MatrixA is treated as a scalar @ mdkLinearAlgebra MatrixElementMultiply(OutputMatrixC, MatrixA, MatrixB)" << '\n';
        MatrixElementMultiply(OutputMatrixC, MatrixA(0), MatrixB);
        return;
    }
    else if (Is_A_Scalar == false && Is_B_Scalar == true)
    {
        mdkWarning << "MatrixB is treated as a scalar @ mdkLinearAlgebra MatrixElementMultiply(OutputMatrixC, MatrixA, MatrixB)" << '\n';
        MatrixElementMultiply(OutputMatrixC, MatrixA, MatrixB(0));
        return;
    }

    // only one of MatrixA and MatrixB can be scalar

    auto SizeC = OutputMatrixC.GetSize();

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber
        || SizeA.RowNumber != SizeC.RowNumber || SizeA.ColNumber != SizeC.ColNumber)
    {
        mdkError << "Size does not match @ mdkLinearAlgebra MatrixElementMultiply(OutputMatrixC, MatrixA, MatrixB)" << '\n';
        return;
    }

    auto ptrC = OutputMatrixC.GetElementDataRawPointer();

    auto ptrA = MatrixA.GetElementDataRawPointer();

    auto ptrB = MatrixB.GetElementDataRawPointer();

    auto ElementNumber = SizeA.RowNumber*SizeA.ColNumber;

    for (uint64 i = 0; i < ElementNumber; ++i)
    {
        ptrC[i] = ptrA[i] * ptrB[i];
    }
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> MatrixElementDivide(const mdkMatrix<ElementType>& MatrixA, const mdkMatrix<ElementType>& MatrixB)
{
    mdkMatrix<ElementType> tempMatrix;

    tempMatrix.SetTobeTemporaryBeforeReturn();

    auto SizeA = MatrixA.GetSize();

    auto SizeB = MatrixB.GetSize();

    if (SizeA.RowNumber == 0 || SizeB.RowNumber == 0)
    {
        mdkError << "MatrixA or MatrixB is empty @ mdkLinearAlgebra MatrixElementDivide(MatrixA, MatrixB)" << '\n';
        return tempMatrix;
    }

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        mdkError << "Size does not match @ mdkLinearAlgebra MatrixElementDivide(MatrixA, MatrixB)" << '\n';
        return tempMatrix;
    }

    tempMatrix.SetSize(SizeA.RowNumber, SizeA.ColNumber);

    MatrixElementDivide(tempMatrix, MatrixA, MatrixB);

    return  tempMatrix;
}


template<typename ElementType>
inline 
void MatrixElementDivide(mdkMatrix<ElementType>& OutputMatrixC, const mdkMatrix<ElementType>& MatrixA, const mdkMatrix<ElementType>& MatrixB)
{
    auto SizeA = MatrixA.GetSize();

    auto SizeB = MatrixB.GetSize();

    if (SizeA.RowNumber == 0 || SizeA.ColNumber == 0)
    {
        mdkError << "MatrixA or MatrixB is empty @ mdkLinearAlgebra MatrixElementDivide(OutputMatrixC, MatrixA, MatrixB)" << '\n';
        return;
    }

    bool Is_A_Scalar = false;

    bool Is_B_Scalar = false;

    if (SizeA.ColNumber == 1 && SizeA.RowNumber == 1)
    {
        Is_A_Scalar = true;
    }

    if (SizeB.ColNumber == 1 && SizeB.RowNumber == 1)
    {
        Is_B_Scalar = true;
        return;
    }

    if (Is_A_Scalar == true && Is_B_Scalar == false)
    {
        mdkWarning << "MatrixA is treated as a scalar @ mdkLinearAlgebra MatrixElementDivide(OutputMatrixC, MatrixA, MatrixB)" << '\n';
        MatrixElementDivide(OutputMatrixC, MatrixA(0), MatrixB);
        return;
    }
    else if (Is_A_Scalar == false && Is_B_Scalar == true)
    {
        mdkWarning << "MatrixB is treated as a scalar @ mdkLinearAlgebra MatrixElementDivide(OutputMatrixC, MatrixA, MatrixB)" << '\n';
        MatrixElementDivide(OutputMatrixC, MatrixA, MatrixB(0));
        return;
    }

    // only one of MatrixA and MatrixB can be scalar

    auto SizeC = OutputMatrixC.GetSize();

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber
        || SizeA.RowNumber != SizeC.RowNumber || SizeA.ColNumber != SizeC.ColNumber)
    {
        mdkError << "Size does not match @ mdkLinearAlgebra MatrixElementDivide(OutputMatrixC, MatrixA, MatrixB)" << '\n';
        return;
    }

    auto ptrC = OutputMatrixC.GetElementDataRawPointer();

    auto ptrA = MatrixA.GetElementDataRawPointer();

    auto ptrB = MatrixB.GetElementDataRawPointer();

    auto ElementNumber = SizeA.RowNumber*SizeA.ColNumber;

    for (uint64 i = 0; i < ElementNumber; ++i)
    {
        ptrC[i] = ptrA[i] / ptrB[i];
    }
}


//------------------- OutputMatrixC =  ElementA operator MatrixB --------------------------------------------

template<typename ElementType>
inline 
mdkMatrix<ElementType> MatrixAdd(const ElementType& ElementA, const mdkMatrix<ElementType>& MatrixB)
{
    mdkMatrix<ElementType> tempMatrix;

    tempMatrix.SetTobeTemporaryBeforeReturn();

    auto SizeB = MatrixB.GetSize();

    if (SizeB.RowNumber == 0)
    {
        mdkError << "MatrixB is empty @ mdkLinearAlgebra MatrixAdd(ElementA, MatrixB)" << '\n';
        return tempMatrix;
    }

    tempMatrix.SetSize(SizeB.RowNumber, SizeB.ColNumber);

    MatrixAdd(tempMatrix, ElementA, MatrixB);

    return tempMatrix;
}


template<typename ElementType>
inline
void MatrixAdd(mdkMatrix<ElementType>& OutputMatrixC, const ElementType& ElementA, const mdkMatrix<ElementType>& MatrixB)
{
    auto SizeB = MatrixB.GetSize();

    auto SizeC = OutputMatrixC.GetSize();

    if (SizeB.RowNumber == 0)
    {
        mdkError << "MatrixB is empty @ mdkLinearAlgebra MatrixAdd(OutputMatrixC, ElementA, MatrixB)" << '\n';
        return;
    }

    if (SizeB.RowNumber != SizeC.RowNumber || SizeB.ColNumber != SizeC.ColNumber)
    {
        mdkError << "Size does not match @ mdkLinearAlgebra MatrixAdd(OutputMatrixC, ElementA, MatrixB)" << '\n';
        return;
    }

    auto ptrC = OutputMatrixC.GetElementDataRawPointer();

    auto ptrB = MatrixB.GetElementDataRawPointer();

    auto ElementNumber = SizeB.RowNumber*SizeB.ColNumber;

    for (uint64 i = 0; i < ElementNumber; ++i)
    {
        ptrC[i] = ElementA + ptrB[i];
    }
}


template<typename ElementType>
inline
mdkMatrix<ElementType> MatrixSubtract(const ElementType& ElementA, const mdkMatrix<ElementType>& MatrixB)
{
    mdkMatrix<ElementType> tempMatrix;

    tempMatrix.SetTobeTemporaryBeforeReturn();

    auto SizeB = MatrixB.GetSize();

    if (SizeB.RowNumber == 0)
    {
        mdkError << "MatrixB is empty @ mdkLinearAlgebra MatrixSubtract(ElementA, MatrixB)" << '\n';
        return tempMatrix;
    }

    tempMatrix.SetSize(SizeB.RowNumber, SizeB.ColNumber);

    MatrixSubtract(tempMatrix, ElementA, MatrixB);

    return tempMatrix;
}


template<typename ElementType>
inline
void MatrixSubtract(mdkMatrix<ElementType> OutputMatrixC, const ElementType& ElementA, const mdkMatrix<ElementType>& MatrixB)
{
    auto SizeB = MatrixB.GetSize();

    auto SizeC = OutputMatrixC.GetSize();

    if (SizeB.RowNumber == 0 || SizeB.ColNumber == 0)
    {
        mdkError << "MatrixB is empty @ mdkLinearAlgebra MatrixSubtract(OutputMatrixC, ElementA, MatrixB)" << '\n';
        return;
    }

    if (SizeB.RowNumber != SizeC.RowNumber || SizeB.ColNumber != SizeC.ColNumber)
    {
        mdkError << "Size does not match @ mdkLinearAlgebra MatrixSubtract(OutputMatrixC, ElementA, MatrixB)" << '\n';
        return;
    }

    auto ptrC = OutputMatrixC.GetElementDataRawPointer();

    auto ptrB = MatrixB.GetElementDataRawPointer();

    auto ElementNumber = SizeB.RowNumber*SizeB.ColNumber;

    for (uint64 i = 0; i < ElementNumber; ++i)
    {
        ptrC[i] = ElementA - ptrB[i];
    }
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> MatrixMultiply(const ElementType& ElementA, mdkMatrix<ElementType>& MatrixB)
{
    mdkMatrix<ElementType> tempMatrix;

    tempMatrix.SetTobeTemporaryBeforeReturn();

    auto SizeB = MatrixB.GetSize();

    if (SizeB.RowNumber == 0)
    {
        mdkError << "MatrixB is empty @ mdkLinearAlgebra MatrixMultiply(ElementA, MatrixB)" << '\n';
        return tempMatrix;
    }

    tempMatrix.SetSize(SizeB.RowNumber, SizeB.ColNumber);

    MatrixMultiply(tempMatrix, ElementA, MatrixB);

    return tempMatrix;
}


template<typename ElementType>
inline
void MatrixMultiply(mdkMatrix<ElementType>& OutputMatrixC, const ElementType& ElementA, const mdkMatrix<ElementType>& MatrixB)
{
    auto SizeB = MatrixB.GetSize();

    if (SizeB.RowNumber == 0 || SizeB.ColNumber == 0)
    {
        mdkError << "MatrixB is empty @ mdkLinearAlgebra MatrixMultiply(OutputMatrixC, ElementA, MatrixB)" << '\n';
        return;
    }

    auto SizeC = OutputMatrixC.GetSize();

    if (SizeB.RowNumber != SizeC.RowNumber || SizeB.ColNumber != SizeC.ColNumber)
    {
        mdkError << "Size does not match @ mdkLinearAlgebra MatrixMultiply(OutputMatrixC, ElementA, MatrixB)" << '\n';
        return;
    }

    auto ptrC = OutputMatrixC.GetElementDataRawPointer();

    auto ptrB = MatrixB.GetElementDataRawPointer();

    auto ElementNumber = SizeB.RowNumber*SizeB.ColNumber;

    for (uint64 i = 0; i < ElementNumber; ++i)
    {
        ptrC[i] = ElementA * ptrB[i];
    }
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> MatrixElementMultiply(const ElementType& ElementA, const mdkMatrix<ElementType>& MatrixB)
{
    mdkMatrix<ElementType> tempMatrix;

    tempMatrix.SetTobeTemporaryBeforeReturn();

    auto SizeB = MatrixB.GetSize();

    if (SizeB.RowNumber == 0)
    {
        mdkError << "MatrixB is empty @ mdkLinearAlgebra MatrixElementMultiply(ElementA, MatrixB)" << '\n';
        return tempMatrix;
    }

    tempMatrix.SetSize(SizeB.RowNumber, SizeB.ColNumber);

    MatrixElementMultiply(tempMatrix, ElementA, MatrixB);

    return tempMatrix;
}


template<typename ElementType>
inline 
void MatrixElementMultiply(mdkMatrix<ElementType>& OutputMatrixC, const ElementType& ElementA, const mdkMatrix<ElementType>& MatrixB)
{
    MatrixMultiply(OutputMatrixC, ElementA, MatrixB);
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> MatrixElementDivide(const ElementType& ElementA, const mdkMatrix<ElementType>& MatrixB)
{
    mdkMatrix<ElementType> tempMatrix;

    tempMatrix.SetTobeTemporaryBeforeReturn();

    auto SizeB = MatrixB.GetSize();

    if (SizeB.RowNumber == 0)
    {
        mdkError << "MatrixB is empty @ mdkLinearAlgebra MatrixElementDivide(ElementA, MatrixB)" << '\n';
        return tempMatrix;
    }

    tempMatrix.SetSize(SizeB.RowNumber, SizeB.ColNumber);

    MatrixElementDivide(tempMatrix, ElementA, MatrixB);

    return tempMatrix;
}


template<typename ElementType>
inline 
void MatrixElementDivide(mdkMatrix<ElementType>& OutputMatrixC, const ElementType& ElementA, const mdkMatrix<ElementType>& MatrixB)
{
    auto SizeB = MatrixB.GetSize();

    if (SizeB.RowNumber == 0 || SizeB.ColNumber == 0)
    {
        mdkError << "MatrixB is empty @ mdkLinearAlgebra MatrixElementDivide(OutputMatrixC, ElementA, MatrixB)" << '\n';
        return;
    }

    auto SizeC = OutputMatrixC.GetSize();

    if (SizeB.RowNumber != SizeC.RowNumber || SizeB.ColNumber != SizeC.ColNumber)
    {
        mdkError << "Size does not match @ mdkLinearAlgebra MatrixElementDivide(OutputMatrixC, ElementA, MatrixB)" << '\n';
        return;
    }

    auto ptrC = OutputMatrixC.GetElementDataRawPointer();

    auto ptrB = MatrixB.GetElementDataRawPointer();

    auto ElementNumber = SizeB.RowNumber*SizeB.ColNumber;

    for (uint64 i = 0; i < ElementNumber; ++i)
    {
        ptrC[i] = ElementA / ptrB[i];
    }
}

//----------------------------------------------- MatrixA operator ElementB ----------------------------------------------------

template<typename ElementType>
inline
mdkMatrix<ElementType> MatrixAdd(const mdkMatrix<ElementType>& MatrixA, const ElementType& ElementB)
{
    mdkMatrix<ElementType> tempMatrix;

    tempMatrix.SetTobeTemporaryBeforeReturn();

    auto SizeA = MatrixA.GetSize();

    if (SizeA.RowNumber == 0)
    {
        mdkError << "MatrixA is empty @ mdkLinearAlgebra MatrixAdd(MatrixA, ElementB)" << '\n';
        return tempMatrix;
    }

    tempMatrix.SetSize(SizeA.RowNumber, SizeA.ColNumber);

    MatrixAdd(tempMatrix, MatrixA, ElementB);

    return tempMatrix;
}


template<typename ElementType>
inline
void MatrixAdd(mdkMatrix<ElementType>& OutputMatrixC, const mdkMatrix<ElementType>& MatrixA, const ElementType& ElementB)
{
    auto SizeA = MatrixA.GetSize();

    if (SizeA.RowNumber == 0)
    {
        mdkError << "MatrixA is empty @ mdkLinearAlgebra MatrixAdd(OutputMatrixC, MatrixA, ElementB)" << '\n';
        return;
    }

    auto SizeC = OutputMatrixC.GetSize();

    if (SizeA.RowNumber != SizeC.RowNumber || SizeA.ColNumber != SizeC.ColNumber)
    {
        mdkError << "Size does not match @ mdkLinearAlgebra MatrixAdd(OutputMatrixC, MatrixA, ElementB)" << '\n';
        return;
    }

    auto ptrC = OutputMatrixC.GetElementDataRawPointer();

    auto ptrA = MatrixA.GetElementDataRawPointer();

    auto ElementNumber = SizeA.RowNumber*SizeA.ColNumber;

    for (uint64 i = 0; i < ElementNumber; ++i)
    {
        ptrC[i] = ptrA[i] + ElementB;
    }
}


template<typename ElementType>
inline
mdkMatrix<ElementType> MatrixSubtract(const mdkMatrix<ElementType>& MatrixA, const ElementType& ElementB)
{
    mdkMatrix<ElementType> tempMatrix;

    tempMatrix.SetTobeTemporaryBeforeReturn();

    auto SizeA = MatrixA.GetSize();

    if (SizeA.RowNumber == 0)
    {
        mdkError << "MatrixA is empty @ mdkLinearAlgebra MatrixSubtract(MatrixA, ElementB)" << '\n';
        return tempMatrix;
    }

    tempMatrix.SetSize(SizeA.RowNumber, SizeA.ColNumber);

    MatrixSubtract(tempMatrix, MatrixA, ElementB);

    return tempMatrix;
}


template<typename ElementType>
inline
void MatrixSubtract(mdkMatrix<ElementType> OutputMatrixC, const mdkMatrix<ElementType>& MatrixA, const ElementType& ElementB)
{
    auto SizeA = MatrixA.GetSize();

    auto SizeC = OutputMatrixC.GetSize();

    if (SizeA.RowNumber == 0 || SizeA.ColNumber == 0)
    {
        mdkError << "MatrixA is empty @ mdkLinearAlgebra MatrixSubtract(OutputMatrixC, MatrixA, ElementB)" << '\n';
        return;
    }

    if (SizeA.RowNumber != SizeC.RowNumber || SizeA.ColNumber != SizeC.ColNumber)
    {
        mdkError << "Size does not match @ mdkLinearAlgebra MatrixSubtract(OutputMatrixC, MatrixA, ElementB)" << '\n';
        return;
    }

    auto ptrC = OutputMatrixC.GetElementDataRawPointer();

    auto ptrA = MatrixA.GetElementDataRawPointer();

    auto ElementNumber = SizeA.RowNumber*SizeA.ColNumber;

    for (uint64 i = 0; i < ElementNumber; ++i)
    {
        ptrC[i] = ptrA[i] - ElementB;
    }
}


template<typename ElementType>
inline
mdkMatrix<ElementType> MatrixMultiply(const mdkMatrix<ElementType>& MatrixA, const ElementType& ElementB)
{
    mdkMatrix<ElementType> tempMatrix;

    tempMatrix.SetTobeTemporaryBeforeReturn();

    auto SizeA = MatrixA.GetSize();

    if (SizeA.RowNumber == 0)
    {
        mdkError << "MatrixA is empty @ mdkLinearAlgebra MatrixMultiply(MatrixA, ElementB)" << '\n';
        return tempMatrix;
    }

    tempMatrix.SetSize(SizeA.RowNumber, SizeA.ColNumber);

    MatrixMultiply(tempMatrix, MatrixA, ElementB);

    return tempMatrix;
}


template<typename ElementType>
inline
void MatrixMultiply(mdkMatrix<ElementType>& OutputMatrixC, const mdkMatrix<ElementType>& MatrixA, const ElementType& ElementB)
{
    auto SizeA = MatrixA.GetSize();

    if (SizeA.RowNumber == 0 || SizeA.ColNumber == 0)
    {
        mdkError << "MatrixA is empty @ mdkLinearAlgebra MatrixMultiply(OutputMatrixC, MatrixA, ElementB)" << '\n';
        return;
    }

    auto SizeC = OutputMatrixC.GetSize();

    if (SizeA.RowNumber != SizeC.RowNumber || SizeA.ColNumber != SizeC.ColNumber)
    {
        mdkError << "Size does not match @ mdkLinearAlgebra MatrixMultiply(OutputMatrixC, MatrixA, ElementB)" << '\n';
        return;
    }

    auto ptrC = OutputMatrixC.GetElementDataRawPointer();

    auto ptrA = MatrixA.GetElementDataRawPointer();

    auto ElementNumber = SizeA.RowNumber*SizeA.ColNumber;

    for (uint64 i = 0; i < ElementNumber; ++i)
    {
        ptrC[i] = ptrA[i] * ElementB;
    }
}


template<typename ElementType>
inline
mdkMatrix<ElementType> MatrixElementMultiply(const mdkMatrix<ElementType>& MatrixA, const ElementType& ElementB)
{
    mdkMatrix<ElementType> tempMatrix;

    tempMatrix.SetTobeTemporaryBeforeReturn();

    auto SizeA = MatrixA.GetSize();

    if (SizeA.RowNumber == 0)
    {
        mdkError << "MatrixA is empty @ mdkLinearAlgebra MatrixElementMultiply(MatrixA, ElementB)" << '\n';
        return tempMatrix;
    }

    tempMatrix.SetSize(SizeA.RowNumber, SizeA.ColNumber);

    MatrixElementMultiply(tempMatrix, MatrixA, ElementB);

    return tempMatrix;
}


template<typename ElementType>
inline
void MatrixElementMultiply(mdkMatrix<ElementType>& OutputMatrixC, const mdkMatrix<ElementType>& MatrixA, const ElementType& ElementB)
{
    MatrixMultiply(OutputMatrixC, MatrixA, ElementB);
}


template<typename ElementType>
inline
mdkMatrix<ElementType> MatrixElementDivide(const mdkMatrix<ElementType>& MatrixA, const ElementType& ElementB)
{
    mdkMatrix<ElementType> tempMatrix;

    tempMatrix.SetTobeTemporaryBeforeReturn();

    auto SizeA = MatrixA.GetSize();

    if (SizeA.RowNumber == 0)
    {
        mdkError << "MatrixA is empty @ mdkLinearAlgebra MatrixElementDivide(MatrixA, ElementB)" << '\n';
        return tempMatrix;
    }

    tempMatrix.SetSize(SizeA.RowNumber, SizeA.ColNumber);

    MatrixElementDivide(tempMatrix, MatrixA, ElementB);

    return tempMatrix;
}


template<typename ElementType>
inline
void MatrixElementDivide(mdkMatrix<ElementType>& OutputMatrixC, const mdkMatrix<ElementType>& MatrixA, const ElementType& ElementB)
{
    auto SizeA = MatrixA.GetSize();

    if (SizeA.RowNumber == 0 || SizeA.ColNumber == 0)
    {
        mdkError << "MatrixA is empty @ mdkLinearAlgebra MatrixElementDivide(OutputMatrixC, MatrixA, ElementB)" << '\n';
        return;
    }

    auto SizeC = OutputMatrixC.GetSize();

    if (SizeA.RowNumber != SizeC.RowNumber || SizeA.ColNumber != SizeC.ColNumber)
    {
        mdkError << "Size does not match @ mdkLinearAlgebra MatrixElementDivide(OutputMatrixC, MatrixA, ElementB)" << '\n';
        return;
    }

    auto ptrC = OutputMatrixC.GetElementDataRawPointer();

    auto ptrA = MatrixA.GetElementDataRawPointer();

    auto ElementNumber = SizeA.RowNumber*SizeA.ColNumber;

    for (uint64 i = 0; i < ElementNumber; ++i)
    {
        ptrC[i] = ptrA[i] / ElementB;
    }
}

//------------------------------------------ MatrixElementOperation ----------------------------------------------------------------------//


//------------------------------------------ MatrixLinearCombine ----------------------------------------------------------------------//

template<typename ElementType>
inline 
void MatrixLinearCombine(mdkMatrix<ElementType>& OutputMatrix, const std::vector<ElementType>& CoefList, const std::vector<mdkMatrix<ElementType>*>& MatrixList)
{
	auto MatrixNumber = MatrixList.size();

    auto CoefNumber = CoefList.size();

    if (MatrixNumber != CoefNumber)
    {
        mdkError << "MatrixNumber != AlphaNumber @ MatrixLinearCombine(OutputMatrix, AlphaList, MatrixList)" << '\n';
        return;
    }

    if (MatrixNumber == 0)
    {
        mdkWarning << "Input is empty @ MatrixLinearCombine(OutputMatrix, AlphaList, MatrixList)" << '\n';
        return;
    }

	auto Size = MatrixList[0]->GetSize();

    if (OutputMatrix.IsEmpty() == true)
    {
        OutputMatrix.SetSize(Size.RowNumber, Size.ColNumber);
    }

    auto OutputRawPointer = OutputMatrix.GetElementDataRawPointer();

    std::vector<ElementType*> MatrixElementDataRawPtrList(MatrixNumber);

    for (uint64 k = 0; k < MatrixNumber; ++k)
    {
        MatrixElementDataRawPtrList[k] = MatrixList[k]->GetElementDataRawPointer();
    }

    auto CoefRawPtr = CoefList.data();

    auto ElementNumber = Size.ColNumber*Size.RowNumber;

    switch (MatrixNumber)
    {
    case 1:
        MatrixLinearCombine_UnrollForLoop_1(OutputRawPointer, ElementNumber, CoefRawPtr, MatrixElementDataRawPtrList);
        break;

    case 2:
        MatrixLinearCombine_UnrollForLoop_2(OutputRawPointer, ElementNumber, CoefRawPtr, MatrixElementDataRawPtrList);
        break;

    case 3:
        MatrixLinearCombine_UnrollForLoop_3(OutputRawPointer, ElementNumber, CoefRawPtr, MatrixElementDataRawPtrList);
        break;

    case 4:
        MatrixLinearCombine_UnrollForLoop_4(OutputRawPointer, ElementNumber, CoefRawPtr, MatrixElementDataRawPtrList);
        break;

    case 5:
        MatrixLinearCombine_UnrollForLoop_5(OutputRawPointer, ElementNumber, CoefRawPtr, MatrixElementDataRawPtrList);
        break;

    case 6:       
        MatrixLinearCombine_UnrollForLoop_6(OutputRawPointer, ElementNumber, CoefRawPtr, MatrixElementDataRawPtrList);        
        break;

    default:
            
        ElementType tempElement = CoefRawPtr[0];

        for (uint64 LinearIndex = 0; LinearIndex < ElementNumber; ++LinearIndex)
        {
            tempElement -= tempElement;

            for (uint64 k = 0; k < MatrixNumber; ++k)
            {
                tempElement += CoefRawPtr[k] * MatrixElementDataRawPtrList[k][LinearIndex];
            }

            OutputRawPointer[LinearIndex] = tempElement;
        }
    }
}


template<typename ElementType>
inline
mdkMatrix<ElementType> MatrixLinearCombine(const std::vector<ElementType>& CoefList, const std::vector<mdkMatrix<ElementType>*>& MatrixList)
{
    mdkMatrix<ElementType> tempMatrix;

    tempMatrix.SetTobeTemporaryBeforeReturn();

    auto MatrixNumber = MatrixList.size();

    auto CoefNumber = CoefList.size();

    if (MatrixNumber != CoefNumber)
    {
        mdkError << "MatrixNumber != AlphaNumber @ MatrixLinearCombine(AlphaList, MatrixList)" << '\n';
        return tempMatrix;
    }

    if (MatrixNumber == 0)
    {
        mdkWarning << "Input is empty @ MatrixLinearCombine(AlphaList, MatrixList)" << '\n';
        return tempMatrix;
    }

    auto Size = MatrixList[0]->GetSize();

    tempMatrix.SetSize(Size.RowNumber, Size.ColNumber);

    MatrixLinearCombine(tempMatrix, CoefList, MatrixList);

    return tempMatrix;
}


template<typename ElementType>
inline
void MatrixLinearCombine_UnrollForLoop_1(ElementType* Output, uint64 ElementNumber,
                                         const ElementType* Coef, 
                                         const std::vector<ElementType*>& MatrixElementDataRawPtrList)
{
    for (uint64 LinearIndex = 0; LinearIndex < ElementNumber; ++LinearIndex)
    {
        Output[LinearIndex] = Coef[0] * MatrixElementDataRawPtrList[0][LinearIndex];
    }
}


template<typename ElementType>
inline
void MatrixLinearCombine_UnrollForLoop_2(ElementType* Output, uint64 ElementNumber,
                                         const ElementType* Coef, 
                                         const std::vector<ElementType*>& MatrixElementDataRawPtrList)
{
    for (uint64 LinearIndex = 0; LinearIndex < ElementNumber; ++LinearIndex)
    {
        Output[LinearIndex] = Coef[0] * MatrixElementDataRawPtrList[0][LinearIndex]
                            + Coef[1] * MatrixElementDataRawPtrList[1][LinearIndex];
    }
}


template<typename ElementType>
inline
void MatrixLinearCombine_UnrollForLoop_3(ElementType* Output, uint64 ElementNumber,
                                         const ElementType* Coef, 
                                         const std::vector<ElementType*>& MatrixElementDataRawPtrList)
{
    for (uint64 LinearIndex = 0; LinearIndex < ElementNumber; ++LinearIndex)
    {
        Output[LinearIndex] =   Coef[0] * MatrixElementDataRawPtrList[0][LinearIndex]
                              + Coef[1] * MatrixElementDataRawPtrList[1][LinearIndex]
                              + Coef[2] * MatrixElementDataRawPtrList[2][LinearIndex];
    }
}



template<typename ElementType>
inline
void MatrixLinearCombine_UnrollForLoop_4(ElementType* Output, uint64 ElementNumber,
                                         const ElementType* Coef, 
                                         const std::vector<ElementType*>& MatrixElementDataRawPtrList)
{
    for (uint64 LinearIndex = 0; LinearIndex < ElementNumber; ++LinearIndex)
    {
        Output[LinearIndex] =  Coef[0] * MatrixElementDataRawPtrList[0][LinearIndex]
                             + Coef[1] * MatrixElementDataRawPtrList[1][LinearIndex]
                             + Coef[2] * MatrixElementDataRawPtrList[2][LinearIndex]
                             + Coef[3] * MatrixElementDataRawPtrList[3][LinearIndex];
    }
}


template<typename ElementType>
inline
void MatrixLinearCombine_UnrollForLoop_5(ElementType* Output, uint64 ElementNumber,
                                         const ElementType* Coef, 
                                         const std::vector<ElementType*>& MatrixElementDataRawPtrList)
{
    for (uint64 LinearIndex = 0; LinearIndex < ElementNumber; ++LinearIndex)
    {
        Output[LinearIndex] =  Coef[0] * MatrixElementDataRawPtrList[0][LinearIndex]
                             + Coef[1] * MatrixElementDataRawPtrList[1][LinearIndex]
                             + Coef[2] * MatrixElementDataRawPtrList[2][LinearIndex]
                             + Coef[3] * MatrixElementDataRawPtrList[3][LinearIndex]
                             + Coef[4] * MatrixElementDataRawPtrList[4][LinearIndex];
    }
}


template<typename ElementType>
inline
void MatrixLinearCombine_UnrollForLoop_6(ElementType* Output, uint64 ElementNumber,
                                         const ElementType* Coef, 
                                         const std::vector<ElementType*>& MatrixElementDataRawPtrList)
{
    for (uint64 LinearIndex = 0; LinearIndex < ElementNumber; ++LinearIndex)
    {
        Output[LinearIndex] = Coef[0] * MatrixElementDataRawPtrList[0][LinearIndex]
                            + Coef[1] * MatrixElementDataRawPtrList[1][LinearIndex]
                            + Coef[2] * MatrixElementDataRawPtrList[2][LinearIndex]
                            + Coef[3] * MatrixElementDataRawPtrList[3][LinearIndex]
                            + Coef[4] * MatrixElementDataRawPtrList[4][LinearIndex]
                            + Coef[5] * MatrixElementDataRawPtrList[5][LinearIndex];
    }
}


template<typename ElementType>
inline
void MatrixLinearCombine_UnrollForLoop_7(ElementType* Output, uint64 ElementNumber,
                                         const ElementType* Coef, 
                                         const std::vector<ElementType*>& MatrixElementDataRawPtrList)
{
    for (uint64 LinearIndex = 0; LinearIndex < ElementNumber; ++LinearIndex)
    {
        Output[LinearIndex] = Coef[0] * MatrixElementDataRawPtrList[0][LinearIndex]
                            + Coef[1] * MatrixElementDataRawPtrList[1][LinearIndex]
                            + Coef[2] * MatrixElementDataRawPtrList[2][LinearIndex]
                            + Coef[3] * MatrixElementDataRawPtrList[3][LinearIndex]
                            + Coef[4] * MatrixElementDataRawPtrList[4][LinearIndex]
                            + Coef[5] * MatrixElementDataRawPtrList[5][LinearIndex]
                            + Coef[6] * MatrixElementDataRawPtrList[6][LinearIndex];
    }
}


template<typename ElementType>
inline
void MatrixLinearCombine_UnrollForLoop_8(ElementType* Output, uint64 ElementNumber,
                                         const ElementType* Coef, 
                                         const std::vector<ElementType*>& MatrixElementDataRawPtrList)
{
    for (uint64 LinearIndex = 0; LinearIndex < ElementNumber; ++LinearIndex)
    {
        Output[LinearIndex] = Coef[0] * MatrixElementDataRawPtrList[0][LinearIndex]
                            + Coef[1] * MatrixElementDataRawPtrList[1][LinearIndex]
                            + Coef[2] * MatrixElementDataRawPtrList[2][LinearIndex]
                            + Coef[3] * MatrixElementDataRawPtrList[3][LinearIndex]
                            + Coef[4] * MatrixElementDataRawPtrList[4][LinearIndex]
                            + Coef[5] * MatrixElementDataRawPtrList[5][LinearIndex]
                            + Coef[6] * MatrixElementDataRawPtrList[6][LinearIndex]
                            + Coef[7] * MatrixElementDataRawPtrList[7][LinearIndex];
    }
}



template<typename ElementType, uint64 Length>
inline
std::array<ElementType, Length> VectorLinearCombine(const std::vector<double>& CoefList, const std::vector<std::array<ElementType, Length>*>& ArrayList)
{
	std::array<ElementType, Length> tempArray;

	uint64 ArrayNumber = ArrayList.size();

	for (uint64 i = 0; i < ArrayList[0]->size(); ++i)
	{
		tempArray[i] = 0;

		for (uint64 k = 0; k < ArrayNumber; ++k)
		{
			tempArray[i] += CoefList[i] * ArrayList[k]->operator[](i);
		}
	}

	return tempArray;
}


template<typename ElementType>
inline
std::vector<ElementType> VectorLinearCombine(const std::vector<double>& CoefList, const std::vector<std::vector<ElementType>*>& ArrayList)
{
	std::vector<ElementType> tempArray;

	uint64 ArrayNumber = ArrayList.size();

	for (uint64 i = 0; i < ArrayList[0]->size(); ++i)
	{
		tempArray[i] = 0;

		for (uint64 k = 0; k < ArrayNumber; ++k)
		{
			tempArray[i] += CoefList[i] * ArrayList[k]->operator[](i);
		}
	}

	return tempArray;
}


}//end namespace mdk

#endif