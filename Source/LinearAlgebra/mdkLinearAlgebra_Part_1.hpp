#ifndef __mdkLinearAlgebra_Part_1_hpp
#define __mdkLinearAlgebra_Part_1_hpp

#include "mdkLinearAlgebra_Part_1.h"

namespace mdk
{

//=========================================================================================================================================//
//------------ MatrixAdd, MatrixSubtract, MatrixMultiply, MatrixElementMultiply, MatrixElementDivide ---------------------------------------//
//=========================================================================================================================================//


//------------------- OutputMatrixC = MatrixA operator MatrixB --------------------------------------------//

template<typename ElementType>
inline 
mdkMatrix<ElementType> MatrixAdd(const mdkMatrix<ElementType>& MatrixA, const mdkMatrix<ElementType>& MatrixB)
{
    mdkMatrix<ElementType> tempMatrix;

    MatrixAdd(tempMatrix, MatrixA, MatrixB);

    return  tempMatrix;
}


template<typename ElementType>
inline
bool MatrixAdd(mdkMatrix<ElementType>& OutputMatrixC, const mdkMatrix<ElementType>& MatrixA, const mdkMatrix<ElementType>& MatrixB)
{
    auto SizeA = MatrixA.GetSize();

    auto SizeB = MatrixB.GetSize();

    if (SizeA.RowNumber == 0 || SizeB.RowNumber == 0)
    {
        mdkError << "MatrixA or MatrixB is empty @ mdkLinearAlgebra MatrixAdd(OutputMatrixC, MatrixA, MatrixB)" << '\n';
        return false;
    }

    if (SizeA.ColNumber == 1 && SizeA.RowNumber == 1)
    {
        return MatrixAdd(OutputMatrixC, MatrixA(0), MatrixB);
    }

    if (SizeB.ColNumber == 1 && SizeB.RowNumber == 1)
    {        
        return MatrixAdd(OutputMatrixC, MatrixA, MatrixB(0));
    }

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        mdkError << "MatrixA Size does not match MatrixB Size @ mdkLinearAlgebra MatrixAdd(OutputMatrixC, MatrixA, MatrixB)" << '\n';
        return false;
    }

    auto SizeC = OutputMatrixC.GetSize();

    if (SizeC.RowNumber != SizeA.RowNumber || SizeC.ColNumber != SizeA.ColNumber)
    {
        if (OutputMatrixC.IsSizeFixed() == false)
        {
            OutputMatrixC.Clear();

            OutputMatrixC.Resize(SizeA.RowNumber, SizeA.ColNumber);
        }
        else
        {
            mdkError << "OutputMatrixC Size does not match @ mdkLinearAlgebra MatrixAdd(OutputMatrixC, MatrixA, MatrixB)" << '\n';
            return false;
        }
    }

    auto ptrC = OutputMatrixC.GetElementDataRawPointer();

    auto ptrA = MatrixA.GetElementDataRawPointer();

    auto ptrB = MatrixB.GetElementDataRawPointer();

    auto ElementNumber = SizeA.RowNumber*SizeA.ColNumber;

    for (uint64 i = 0; i < ElementNumber; ++i)
    {
        ptrC[i] = ptrA[i] + ptrB[i];
    }

    return true;
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> MatrixSubtract(const mdkMatrix<ElementType>& MatrixA, const mdkMatrix<ElementType>& MatrixB)
{
    mdkMatrix<ElementType> tempMatrix;

    MatrixSubtract(tempMatrix, MatrixA, MatrixB);

    return  tempMatrix;
}


template<typename ElementType>
inline
bool MatrixSubtract(mdkMatrix<ElementType>& OutputMatrixC, const mdkMatrix<ElementType>& MatrixA, const mdkMatrix<ElementType>& MatrixB)
{
    auto SizeA = MatrixA.GetSize();

    auto SizeB = MatrixB.GetSize();

    if (SizeA.RowNumber == 0 || SizeA.ColNumber == 0)
    {
        mdkError << "MatrixA or MatrixB is empty @ mdkLinearAlgebra MatrixSubtract(OutputMatrixC, MatrixA, MatrixB)" << '\n';
        return false;
    }

    if (SizeA.ColNumber == 1 && SizeA.RowNumber == 1)
    {        
        return MatrixSubtract(OutputMatrixC, MatrixA(0), MatrixB);
    }

    if (SizeB.ColNumber == 1 && SizeB.RowNumber == 1)
    {        
        return MatrixSubtract(OutputMatrixC, MatrixA, MatrixB(0));
    }

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        mdkError << "MatrixA Size does not match MatrixB Size @ mdkLinearAlgebra MatrixSubtract(OutputMatrixC, MatrixA, MatrixB)" << '\n';
        return false;
    }

    auto SizeC = OutputMatrixC.GetSize();

    if (SizeC.RowNumber != SizeA.RowNumber || SizeC.ColNumber != SizeA.ColNumber)
    {
        if (OutputMatrixC.IsSizeFixed() == false)
        {
            OutputMatrixC.Clear();

            OutputMatrixC.Resize(SizeA.RowNumber, SizeA.ColNumber);
        }
        else
        {
            mdkError << "OutputMatrixC Size does not match @ mdkLinearAlgebra MatrixSubtract(OutputMatrixC, MatrixA, MatrixB)" << '\n';
            return false;
        }
    }

    auto ptrC = OutputMatrixC.GetElementDataRawPointer();

    auto ptrA = MatrixA.GetElementDataRawPointer();

    auto ptrB = MatrixB.GetElementDataRawPointer();

    auto ElementNumber = SizeA.RowNumber*SizeA.ColNumber;

    for (uint64 i = 0; i < ElementNumber; ++i)
    {
        ptrC[i] = ptrA[i] - ptrB[i];
    }

    return true;
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> MatrixMultiply(const mdkMatrix<ElementType>& MatrixA, const mdkMatrix<ElementType>& MatrixB)
{
    mdkMatrix<ElementType> tempMatrix;

    MatrixMultiply(tempMatrix, MatrixA, MatrixB);

    return  tempMatrix;
}


template<typename ElementType>
inline
bool MatrixMultiply(mdkMatrix<ElementType>& OutputMatrixC, const mdkMatrix<ElementType>& MatrixA, const mdkMatrix<ElementType>& MatrixB)
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

        MatrixMultiply(tempMatrix, MatrixA, MatrixB);

        OutputMatrixC.Take(tempMatrix);

        return true;
    }

    auto SizeA = MatrixA.GetSize();

    auto SizeB = MatrixB.GetSize();

    if (SizeA.RowNumber == 0 || SizeB.RowNumber == 0)
    {
        mdkError << "MatrixA or MatrixB is empty @ mdkLinearAlgebra MatrixMultiply(OutputMatrixC, MatrixA, MatrixB)" << '\n';
        return false;
    }

    if (SizeA.ColNumber == 1 && SizeA.RowNumber == 1)
    {
        return MatrixMultiply(OutputMatrixC, MatrixA(0), MatrixB);
    }

    if (SizeB.ColNumber == 1 && SizeB.RowNumber == 1)
    {
        return MatrixMultiply(OutputMatrixC, MatrixA, MatrixB(0));
    }

    if (SizeA.ColNumber != SizeB.RowNumber)
    {
        mdkError << "MatrixA Size does not match MatrixB Size @ mdkLinearAlgebra MatrixMultiply(OutputMatrixC, MatrixA, MatrixB)" << '\n';
        return false;
    }

    auto SizeC = OutputMatrixC.GetSize();

    if (SizeC.RowNumber != SizeA.RowNumber || SizeC.ColNumber != SizeB.ColNumber)
    {
        if (OutputMatrixC.IsSizeFixed() == false)
        {
            OutputMatrixC.Clear();

            OutputMatrixC.Resize(SizeA.RowNumber, SizeB.ColNumber);
        }
        else
        {
            mdkError << "OutputMatrixC Size does not match @ mdkLinearAlgebra MatrixMultiply(OutputMatrixC, MatrixA, MatrixB)" << '\n';
            return false;
        }
    }


    //--------------------- call lapack via armadillo --------------------------------------------------------------------------------

    arma::Mat<double> A(ptrA, arma::uword(MatrixA.GetRowNumber()), arma::uword(MatrixA.GetColNumber()), false);
    arma::Mat<double> B(ptrB, arma::uword(MatrixB.GetRowNumber()), arma::uword(MatrixB.GetColNumber()), false);
    arma::Mat<double> C(ptrC, arma::uword(OutputMatrixC.GetRowNumber()), arma::uword(OutputMatrixC.GetColNumber()), false);

    C = A*B;

    return true;

    //--------------------- for-loop : slow ------------------------------------------------------------------------------------

    uint64 IndexB = 0;

    for (uint64 j = 0; j < SizeB.ColNumber; ++j)
    {
        for (uint64 i = 0; i < SizeA.RowNumber; ++i)
        {
            ElementType sum = ElementType(0);

            uint64 IndexA = 0;

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

    return true;
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> MatrixElementMultiply(const mdkMatrix<ElementType>& MatrixA, const mdkMatrix<ElementType>& MatrixB)
{
    mdkMatrix<ElementType> tempMatrix;

    MatrixElementMultiply(tempMatrix, MatrixA, MatrixB);

    return  tempMatrix;
}


template<typename ElementType>
inline 
bool MatrixElementMultiply(mdkMatrix<ElementType>& OutputMatrixC, const mdkMatrix<ElementType>& MatrixA, const mdkMatrix<ElementType>& MatrixB)
{
    auto SizeA = MatrixA.GetSize();

    auto SizeB = MatrixB.GetSize();

    if (SizeA.RowNumber == 0 || SizeA.ColNumber == 0)
    {
        mdkError << "MatrixA or MatrixB is empty @ mdkLinearAlgebra MatrixElementMultiply(OutputMatrixC, MatrixA, MatrixB)" << '\n';
        return false;
    }

    if (SizeA.ColNumber == 1 && SizeA.RowNumber == 1)
    {
        return MatrixElementMultiply(OutputMatrixC, MatrixA(0), MatrixB);
    }

    if (SizeB.ColNumber == 1 && SizeB.RowNumber == 1)
    {
        return MatrixElementMultiply(OutputMatrixC, MatrixA, MatrixB(0));
    }

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        mdkError << "MatrixA Size does not match MatrixB Size @ mdkLinearAlgebra MatrixElementMultiply(OutputMatrixC, MatrixA, MatrixB)" << '\n';
        return false;
    }

    auto SizeC = OutputMatrixC.GetSize();

    if (SizeC.RowNumber != SizeA.RowNumber || SizeC.ColNumber != SizeA.ColNumber)
    {
        if (OutputMatrixC.IsSizeFixed() == false)
        {
            OutputMatrixC.Clear();

            OutputMatrixC.Resize(SizeA.RowNumber, SizeA.ColNumber);
        }
        else
        {
            mdkError << "OutputMatrixC Size does not match @ mdkLinearAlgebra MatrixElementMultiply(OutputMatrixC, MatrixA, MatrixB)" << '\n';
            return false;
        }
    }

    auto ptrC = OutputMatrixC.GetElementDataRawPointer();

    auto ptrA = MatrixA.GetElementDataRawPointer();

    auto ptrB = MatrixB.GetElementDataRawPointer();

    auto ElementNumber = SizeA.RowNumber*SizeA.ColNumber;

    for (uint64 i = 0; i < ElementNumber; ++i)
    {
        ptrC[i] = ptrA[i] * ptrB[i];
    }

    return true;
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> MatrixElementDivide(const mdkMatrix<ElementType>& MatrixA, const mdkMatrix<ElementType>& MatrixB)
{
    mdkMatrix<ElementType> tempMatrix;

    MatrixElementDivide(tempMatrix, MatrixA, MatrixB);

    return  tempMatrix;
}


template<typename ElementType>
inline 
bool MatrixElementDivide(mdkMatrix<ElementType>& OutputMatrixC, const mdkMatrix<ElementType>& MatrixA, const mdkMatrix<ElementType>& MatrixB)
{
    auto SizeA = MatrixA.GetSize();

    auto SizeB = MatrixB.GetSize();

    if (SizeA.RowNumber == 0 || SizeA.ColNumber == 0)
    {
        mdkError << "MatrixA or MatrixB is empty @ mdkLinearAlgebra MatrixElementDivide(OutputMatrixC, MatrixA, MatrixB)" << '\n';
        return false;
    }

    if (SizeA.ColNumber == 1 && SizeA.RowNumber == 1)
    {
        return MatrixElementDivide(OutputMatrixC, MatrixA(0), MatrixB);
    }

    if (SizeB.ColNumber == 1 && SizeB.RowNumber == 1)
    {
        return MatrixElementDivide(OutputMatrixC, MatrixA, MatrixB(0));
    }

    if (SizeA.RowNumber != SizeB.RowNumber || SizeA.ColNumber != SizeB.ColNumber)
    {
        mdkError << "MatrixA Size does not match MatrixB Size @ mdkLinearAlgebra MatrixElementDivide(OutputMatrixC, MatrixA, MatrixB)" << '\n';
        return false;
    }

    auto SizeC = OutputMatrixC.GetSize();

    if (SizeC.RowNumber != SizeA.RowNumber || SizeC.ColNumber != SizeA.ColNumber)
    {
        if (OutputMatrixC.IsSizeFixed() == false)
        {
            OutputMatrixC.Clear();

            OutputMatrixC.Resize(SizeA.RowNumber, SizeA.ColNumber);
        }
        else
        {
            mdkError << "OutputMatrixC Size does not match @ mdkLinearAlgebra MatrixElementDivide(OutputMatrixC, MatrixA, MatrixB)" << '\n';
            return false;
        }
    }

    auto ptrC = OutputMatrixC.GetElementDataRawPointer();

    auto ptrA = MatrixA.GetElementDataRawPointer();

    auto ptrB = MatrixB.GetElementDataRawPointer();

    auto ElementNumber = SizeA.RowNumber*SizeA.ColNumber;

    for (uint64 i = 0; i < ElementNumber; ++i)
    {
        ptrC[i] = ptrA[i] / ptrB[i];
    }

    return true;
}


//------------------- OutputMatrixC =  ElementA operator MatrixB --------------------------------------------//

template<typename ElementType>
inline 
mdkMatrix<ElementType> MatrixAdd(const ElementType& ElementA, const mdkMatrix<ElementType>& MatrixB)
{
    mdkMatrix<ElementType> tempMatrix;

    MatrixAdd(tempMatrix, ElementA, MatrixB);

    return tempMatrix;
}


template<typename ElementType>
inline
bool MatrixAdd(mdkMatrix<ElementType>& OutputMatrixC, const ElementType& ElementA, const mdkMatrix<ElementType>& MatrixB)
{
    auto SizeB = MatrixB.GetSize();

    if (SizeB.RowNumber == 0)
    {
        mdkError << "MatrixB is empty @ mdkLinearAlgebra MatrixAdd(OutputMatrixC, ElementA, MatrixB)" << '\n';
        return false;
    }

    auto SizeC = OutputMatrixC.GetSize();

    if (SizeC.RowNumber != SizeB.RowNumber || SizeC.ColNumber != SizeB.ColNumber)
    {
        if (OutputMatrixC.IsSizeFixed() == false)
        {
            OutputMatrixC.Clear();

            OutputMatrixC.Resize(SizeB.RowNumber, SizeB.ColNumber);
        }
        else
        {
            mdkError << "OutputMatrixC Size does not match @ mdkLinearAlgebra MatrixAdd(OutputMatrixC, ElementA, MatrixB)" << '\n';
            return false;
        }
    }

    auto ptrC = OutputMatrixC.GetElementDataRawPointer();

    auto ptrB = MatrixB.GetElementDataRawPointer();

    auto ElementNumber = SizeB.RowNumber*SizeB.ColNumber;

    if (ptrC == ptrB) // in place
    {
        for (auto tempPtr = ptrC; tempPtr < ptrC + ElementNumber; ++tempPtr)
        {
            tempPtr[0] += ElementA;
        }
    }
    else
    {
        for (uint64 i = 0; i < ElementNumber; ++i)
        {
            ptrC[i] = ElementA + ptrB[i];
        }
    }

    return true;
}


template<typename ElementType>
inline
mdkMatrix<ElementType> MatrixSubtract(const ElementType& ElementA, const mdkMatrix<ElementType>& MatrixB)
{
    mdkMatrix<ElementType> tempMatrix;

    MatrixSubtract(tempMatrix, ElementA, MatrixB);

    return tempMatrix;
}


template<typename ElementType>
inline
void MatrixSubtract(mdkMatrix<ElementType> OutputMatrixC, const ElementType& ElementA, const mdkMatrix<ElementType>& MatrixB)
{
    auto SizeB = MatrixB.GetSize();

    if (SizeB.RowNumber == 0)
    {
        mdkError << "MatrixB is empty @ mdkLinearAlgebra MatrixSubtract(OutputMatrixC, ElementA, MatrixB)" << '\n';
        return;
    }

    auto SizeC = OutputMatrixC.GetSize();

    if (SizeC.RowNumber != SizeB.RowNumber || SizeC.ColNumber != SizeB.ColNumber)
    {
        if (OutputMatrixC.IsSizeFixed() == false)
        {
            OutputMatrixC.Clear();

            OutputMatrixC.Resize(SizeB.RowNumber, SizeB.ColNumber);
        }
        else
        {
            mdkError << "OutputMatrixC Size does not match @ mdkLinearAlgebra MatrixSubtract(OutputMatrixC, ElementA, MatrixB)" << '\n';
            return false;
        }
    }

    auto ptrC = OutputMatrixC.GetElementDataRawPointer();

    auto ptrB = MatrixB.GetElementDataRawPointer();

    auto ElementNumber = SizeB.RowNumber*SizeB.ColNumber;

    if (ptrC == ptrB) // in place
    {
        for (auto tempPtr = ptrC; tempPtr < ptrC + ElementNumber; ++tempPtr)
        {
            tempPtr[0] -= ElementA;
        }
    }
    else
    {
        for (uint64 i = 0; i < ElementNumber; ++i)
        {
            ptrC[i] = ElementA - ptrB[i];
        }
    }

    return true;
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> MatrixMultiply(const ElementType& ElementA, mdkMatrix<ElementType>& MatrixB)
{
    mdkMatrix<ElementType> tempMatrix;

    MatrixMultiply(tempMatrix, ElementA, MatrixB);

    return tempMatrix;
}


template<typename ElementType>
inline
bool MatrixMultiply(mdkMatrix<ElementType>& OutputMatrixC, const ElementType& ElementA, const mdkMatrix<ElementType>& MatrixB)
{
    auto SizeB = MatrixB.GetSize();

    if (SizeB.RowNumber == 0)
    {
        mdkError << "MatrixB is empty @ mdkLinearAlgebra MatrixMultiply(OutputMatrixC, ElementA, MatrixB)" << '\n';
        return false;
    }

    auto SizeC = OutputMatrixC.GetSize();

    if (SizeC.RowNumber != SizeB.RowNumber || SizeC.ColNumber != SizeB.ColNumber)
    {
        if (OutputMatrixC.IsSizeFixed() == false)
        {
            OutputMatrixC.Clear();

            OutputMatrixC.Resize(SizeB.RowNumber, SizeB.ColNumber);
        }
        else
        {
            mdkError << "OutputMatrixC Size does not match @ mdkLinearAlgebra MatrixMultiply(OutputMatrixC, ElementA, MatrixB)" << '\n';
            return false;
        }
    }

    auto ptrC = OutputMatrixC.GetElementDataRawPointer();

    auto ptrB = MatrixB.GetElementDataRawPointer();

    auto ElementNumber = SizeB.RowNumber*SizeB.ColNumber;

    if (ptrC == ptrB) // in place
    {
        for (auto tempPtr = ptrC; tempPtr < ptrC + ElementNumber; ++tempPtr)
        {
            tempPtr[0] *= ElementA;
        }
    }
    else
    {
        for (uint64 i = 0; i < ElementNumber; ++i)
        {
            ptrC[i] = ElementA * ptrB[i];
        }
    }

    return true;
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> MatrixElementMultiply(const ElementType& ElementA, const mdkMatrix<ElementType>& MatrixB)
{
    mdkMatrix<ElementType> tempMatrix;

    MatrixElementMultiply(tempMatrix, ElementA, MatrixB);

    return tempMatrix;
}


template<typename ElementType>
inline 
bool MatrixElementMultiply(mdkMatrix<ElementType>& OutputMatrixC, const ElementType& ElementA, const mdkMatrix<ElementType>& MatrixB)
{
    return MatrixMultiply(OutputMatrixC, ElementA, MatrixB);
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> MatrixElementDivide(const ElementType& ElementA, const mdkMatrix<ElementType>& MatrixB)
{
    mdkMatrix<ElementType> tempMatrix;

    MatrixElementDivide(tempMatrix, ElementA, MatrixB);

    return tempMatrix;
}


template<typename ElementType>
inline 
void MatrixElementDivide(mdkMatrix<ElementType>& OutputMatrixC, const ElementType& ElementA, const mdkMatrix<ElementType>& MatrixB)
{
    auto SizeB = MatrixB.GetSize();

    if (SizeB.RowNumber == 0)
    {
        mdkError << "MatrixB is empty @ mdkLinearAlgebra MatrixElementDivide(OutputMatrixC, ElementA, MatrixB)" << '\n';
        return;
    }

    auto SizeC = OutputMatrixC.GetSize();

    if (SizeC.RowNumber != SizeB.RowNumber || SizeC.ColNumber != SizeB.ColNumber)
    {
        if (OutputMatrixC.IsSizeFixed() == false)
        {
            OutputMatrixC.Clear();

            OutputMatrixC.Resize(SizeB.RowNumber, SizeB.ColNumber);
        }
        else
        {
            mdkError << "OutputMatrixC Size does not match @ mdkLinearAlgebra MatrixElementDivide(OutputMatrixC, ElementA, MatrixB)" << '\n';
            return false;
        }
    }

    auto ptrC = OutputMatrixC.GetElementDataRawPointer();

    auto ptrB = MatrixB.GetElementDataRawPointer();

    auto ElementNumber = SizeB.RowNumber*SizeB.ColNumber;

    if (ptrC == ptrB) // in place
    {
        for (auto tempPtr = ptrC; tempPtr < ptrC + ElementNumber; ++tempPtr)
        {
            tempPtr[0] = ElementA / tempPtr[0];
        }
    }
    else
    {
        for (uint64 i = 0; i < ElementNumber; ++i)
        {
            ptrC[i] = ElementA / ptrB[i];
        }
    }

    return true;
}

//-----------------------------------OutputMatrixC = MatrixA operator ElementB ----------------------------------------------------//

template<typename ElementType>
inline
mdkMatrix<ElementType> MatrixAdd(const mdkMatrix<ElementType>& MatrixA, const ElementType& ElementB)
{
    mdkMatrix<ElementType> tempMatrix;

    MatrixAdd(tempMatrix, MatrixA, ElementB);

    return tempMatrix;
}


template<typename ElementType>
inline
bool MatrixAdd(mdkMatrix<ElementType>& OutputMatrixC, const mdkMatrix<ElementType>& MatrixA, const ElementType& ElementB)
{
    auto SizeA = MatrixA.GetSize();

    if (SizeA.RowNumber == 0)
    {
        mdkError << "MatrixA is empty @ mdkLinearAlgebra MatrixAdd(OutputMatrixC, MatrixA, ElementB)" << '\n';
        return false;
    }

    auto SizeC = OutputMatrixC.GetSize();

    if (SizeC.RowNumber != SizeA.RowNumber || SizeC.ColNumber != SizeA.ColNumber)
    {
        if (OutputMatrixC.IsSizeFixed() == false)
        {
            OutputMatrixC.Clear();

            OutputMatrixC.Resize(SizeA.RowNumber, SizeA.ColNumber);
        }
        else
        {
            mdkError << "OutputMatrixC Size does not match @ mdkLinearAlgebra MatrixAdd(OutputMatrixC, MatrixA, ElementB)" << '\n';
            return false;
        }
    }

    auto ptrC = OutputMatrixC.GetElementDataRawPointer();

    auto ptrA = MatrixA.GetElementDataRawPointer();

    auto ElementNumber = SizeA.RowNumber*SizeA.ColNumber;

    if (ptrC == ptrA) // in place
    {
        for (auto tempPtr = ptrC; tempPtr < ptrC + ElementNumber; ++tempPtr)
        {
            tempPtr[0] += ElementB;
        }
    }
    else
    {
        for (uint64 i = 0; i < ElementNumber; ++i)
        {
            ptrC[i] = ptrA[i] + ElementB;
        }
    }

    return true;
}


template<typename ElementType>
inline
mdkMatrix<ElementType> MatrixSubtract(const mdkMatrix<ElementType>& MatrixA, const ElementType& ElementB)
{
    mdkMatrix<ElementType> tempMatrix;

    MatrixSubtract(tempMatrix, MatrixA, ElementB);

    return tempMatrix;
}


template<typename ElementType>
inline
void MatrixSubtract(mdkMatrix<ElementType> OutputMatrixC, const mdkMatrix<ElementType>& MatrixA, const ElementType& ElementB)
{
    auto SizeA = MatrixA.GetSize();

    if (SizeA.RowNumber == 0)
    {
        mdkError << "MatrixA is empty @ mdkLinearAlgebra MatrixSubtract(OutputMatrixC, MatrixA, ElementB)" << '\n';
        return;
    }

    auto SizeC = OutputMatrixC.GetSize();

    if (SizeC.RowNumber != SizeA.RowNumber || SizeC.ColNumber != SizeA.ColNumber)
    {
        if (OutputMatrixC.IsSizeFixed() == false)
        {
            OutputMatrixC.Clear();

            OutputMatrixC.Resize(SizeA.RowNumber, SizeA.ColNumber);
        }
        else
        {
            mdkError << "OutputMatrixC Size does not match @ mdkLinearAlgebra MatrixSubtract(OutputMatrixC, MatrixA, ElementB)" << '\n';
            return false;
        }
    }

    auto ptrC = OutputMatrixC.GetElementDataRawPointer();

    auto ptrA = MatrixA.GetElementDataRawPointer();

    auto ElementNumber = SizeA.RowNumber*SizeA.ColNumber;

    if (ptrC == ptrA) // in place
    {
        for (auto tempPtr = ptrC; tempPtr < ptrC + ElementNumber; ++tempPtr)
        {
            tempPtr[0] -= ElementB;
        }
    }
    else
    {
        for (uint64 i = 0; i < ElementNumber; ++i)
        {
            ptrC[i] = ptrA[i] - ElementB;
        }
    }

    return true;
}


template<typename ElementType>
inline
mdkMatrix<ElementType> MatrixMultiply(const mdkMatrix<ElementType>& MatrixA, const ElementType& ElementB)
{
    mdkMatrix<ElementType> tempMatrix;

    MatrixMultiply(tempMatrix, MatrixA, ElementB);

    return tempMatrix;
}


template<typename ElementType>
inline
bool MatrixMultiply(mdkMatrix<ElementType>& OutputMatrixC, const mdkMatrix<ElementType>& MatrixA, const ElementType& ElementB)
{
    auto SizeA = MatrixA.GetSize();

    if (SizeA.RowNumber == 0)
    {
        mdkError << "MatrixA is empty @ mdkLinearAlgebra MatrixMultiply(OutputMatrixC, MatrixA, ElementB)" << '\n';
        return false;
    }

    auto SizeC = OutputMatrixC.GetSize();

    if (SizeC.RowNumber != SizeA.RowNumber || SizeC.ColNumber != SizeA.ColNumber)
    {
        if (OutputMatrixC.IsSizeFixed() == false)
        {
            OutputMatrixC.Clear();

            OutputMatrixC.Resize(SizeA.RowNumber, SizeA.ColNumber);
        }
        else
        {
            mdkError << "OutputMatrixC Size does not match @ mdkLinearAlgebra MatrixMultiply(OutputMatrixC, MatrixA, ElementB)" << '\n';
            return false;
        }
    }

    auto ptrC = OutputMatrixC.GetElementDataRawPointer();

    auto ptrA = MatrixA.GetElementDataRawPointer();

    auto ElementNumber = SizeA.RowNumber*SizeA.ColNumber;

    if (ptrC == ptrA) // in place
    {
        for (auto tempPtr = ptrC; tempPtr < ptrC + ElementNumber; ++tempPtr)
        {
            tempPtr[0] *= ElementB;
        }
    }
    else
    {
        for (uint64 i = 0; i < ElementNumber; ++i)
        {
            ptrC[i] = ptrA[i] * ElementB;
        }
    }
   
    return true;
}


template<typename ElementType>
inline
mdkMatrix<ElementType> MatrixElementMultiply(const mdkMatrix<ElementType>& MatrixA, const ElementType& ElementB)
{
    mdkMatrix<ElementType> tempMatrix;

    MatrixElementMultiply(tempMatrix, MatrixA, ElementB);

    return tempMatrix;
}


template<typename ElementType>
inline
bool MatrixElementMultiply(mdkMatrix<ElementType>& OutputMatrixC, const mdkMatrix<ElementType>& MatrixA, const ElementType& ElementB)
{
    return MatrixMultiply(OutputMatrixC, MatrixA, ElementB);
}


template<typename ElementType>
inline
mdkMatrix<ElementType> MatrixElementDivide(const mdkMatrix<ElementType>& MatrixA, const ElementType& ElementB)
{
    mdkMatrix<ElementType> tempMatrix;

    MatrixElementDivide(tempMatrix, MatrixA, ElementB);

    return tempMatrix;
}


template<typename ElementType>
inline
void MatrixElementDivide(mdkMatrix<ElementType>& OutputMatrixC, const mdkMatrix<ElementType>& MatrixA, const ElementType& ElementB)
{
    auto SizeA = MatrixA.GetSize();

    if (SizeA.RowNumber == 0)
    {
        mdkError << "MatrixA is empty @ mdkLinearAlgebra MatrixElementDivide(OutputMatrixC, MatrixA, ElementB)" << '\n';
        return;
    }

    auto SizeC = OutputMatrixC.GetSize();

    if (SizeC.RowNumber != SizeA.RowNumber || SizeC.ColNumber != SizeA.ColNumber)
    {
        if (OutputMatrixC.IsSizeFixed() == false)
        {
            OutputMatrixC.Clear();

            OutputMatrixC.Resize(SizeA.RowNumber, SizeA.ColNumber);
        }
        else
        {
            mdkError << "OutputMatrixC Size does not match @ mdkLinearAlgebra MatrixElementDivide(OutputMatrixC, MatrixA, ElementB)" << '\n';
            return false;
        }
    }

    auto ptrC = OutputMatrixC.GetElementDataRawPointer();

    auto ptrA = MatrixA.GetElementDataRawPointer();

    auto ElementNumber = SizeA.RowNumber*SizeA.ColNumber;

    if (ptrC == ptrA) // in place
    {
        for (auto tempPtr = ptrC; tempPtr < ptrC + ElementNumber; ++tempPtr)
        {
            tempPtr[0] /= ElementB;
        }
    }
    else
    {
        for (uint64 i = 0; i < ElementNumber; ++i)
        {
            ptrC[i] = ptrA[i] / ElementB;
        }
    }
}

//=========================================================================================================================================//
//------------------------------------------ MatrixElementOperation ----------------------------------------------------------------------//
//=========================================================================================================================================//

// ------------------------ MatrixElementOperation on InputMatrix

template<typename ElementType>
inline
mdkMatrix<ElementType> MatrixElementOperation(const char* OperationName, const mdkMatrix<ElementType>& InputMatrix)
{
    return MatrixElementOperation(std::string(OperationName), InputMatrix);
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> MatrixElementOperation(const std::string& OperationName, const mdkMatrix<ElementType>& InputMatrix)
{
    auto InputSize = InputMatrix.GetSize();

    mdkMatrix<ElementType> tempMatrix(InputSize.RowNumber, InputSize.ColNumber);

    MatrixElementOperation(tempMatrix, OperationName, InputMatrix);

    return tempMatrix;
}


template<typename ElementType>
inline 
bool MatrixElementOperation(mdkMatrix<ElementType>& OutputMatrix, const std::string& OperationName, const mdkMatrix<ElementType>& InputMatrix)
{
    // note: must use  MatrixElementOperation<ElementType>, not MatrixElementOperation
    // otherwise compiler error (vs2013)

    if (OperationName == "abs")
    {
        return MatrixElementOperation<ElementType>(OutputMatrix, [](const ElementType& a){return std::abs(a); }, InputMatrix);
    }
    else if (OperationName == "sin")
    {
        return MatrixElementOperation<ElementType>(OutputMatrix, [](const ElementType& a){return std::sin(a); }, InputMatrix);
    }
    else if (OperationName == "cos")
    {
        return MatrixElementOperation<ElementType>(OutputMatrix, [](const ElementType& a){return std::cos(a); }, InputMatrix);
    }
    else if (OperationName == "tan")
    {
        return MatrixElementOperation<ElementType>(OutputMatrix, [](const ElementType& a){return std::tan(a); }, InputMatrix);
    }
    else if (OperationName == "sqrt")
    {
        return  MatrixElementOperation<ElementType>(OutputMatrix, [](const ElementType& a){return std::sqrt(a); }, InputMatrix);
    }
    else
    {
        mdkError << " unknown Operation @ mdkLinearAlgebra MatrixElementOperation(OutputMatrix, OperationName, InputMatrix)" << '\n';

        return false;
    }
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> MatrixElementOperation(std::function<ElementType(const ElementType&)> Operation,
                                              const mdkMatrix<ElementType>& InputMatrix)
{
    auto InputSize = InputMatrix.GetSize();

    mdkMatrix<ElementType> tempMatrix(InputSize.RowNumber, InputSize.ColNumber);

    MatrixElementOperation(tempMatrix, Operation, InputMatrix);

    return tempMatrix;
}


template<typename ElementType>
inline 
bool MatrixElementOperation(mdkMatrix<ElementType>& OutputMatrix, 
                            std::function<ElementType(const ElementType&)> Operation, 
                            const mdkMatrix<ElementType>& InputMatrix)
{
    auto InputSize = InputMatrix.GetSize();

    if (InputSize.RowNumber == 0)
    {
        mdkError << "InputMatrix is empty @ mdkLinearAlgebra MatrixElementOperation(OutputMatrix, Operation, InputMatrix)" << '\n';

        return false;
    }

    auto OutputSize = OutputMatrix.GetSize();

    if (InputSize.RowNumber != OutputSize.RowNumber || InputSize.ColNumber != OutputSize.ColNumber)
    {
        if (OutputMatrix.IsSizeFixed() == false)
        {
            OutputMatrix.Clear();

            OutputMatrix.Resize(InputSize.RowNumber, InputSize.ColNumber);
        }
        else
        {
            mdkError << "Size does not match @ mdkLinearAlgebra MatrixElementOperation(OutputMatrix, Operation, InputMatrix)" << '\n';
            return false;
        }
    }

    auto ptrOutput = OutputMatrix.GetElementDataRawPointer();

    auto ptrInput = InputMatrix.GetElementDataRawPointer();

    auto ElementNumber = InputSize.ColNumber * InputSize.RowNumber;

    if (ptrOutput == ptrInput) // in place operation
    {
        for (auto Ptr = ptrOutput; Ptr < ptrOutput + ElementNumber; ++Ptr)
        {
            Ptr[0] = Operation(Ptr[0]);
        }
    }
    else
    {
        for (uint64 i = 0; i < ElementNumber; ++i)
        {
            ptrOutput[i] = Operation(ptrOutput[i]);
        }
    }

    return true;
}


// ------------------------ MatrixElementOperation on InputMatrixA with InputMatrixB

template<typename ElementType>
inline 
mdkMatrix<ElementType> MatrixElementOperation(const char* OperationName,
                                              const mdkMatrix<ElementType>& InputMatrixA,
                                              const mdkMatrix<ElementType>& InputMatrixB)
{                                              
    return MatrixElementOperation(std::string(OperationName), InputMatrixA, InputMatrixB);
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> MatrixElementOperation(const std::string& OperationName,
                                              const mdkMatrix<ElementType>& InputMatrixA,
                                              const mdkMatrix<ElementType>& InputMatrixB)
{                                              
    mdkMatrix<ElementType> tempMatrix;

    MatrixElementOperation(tempMatrix, OperationName, InputMatrixA, InputMatrixB);

    return tempMatrix;
}


template<typename ElementType>
inline 
bool MatrixElementOperation(mdkMatrix<ElementType>& OutputMatrixC,
                            const std::string& OperationName,
                            const mdkMatrix<ElementType>& InputMatrixA,
                            const mdkMatrix<ElementType>& InputMatrixB)
{
    // note: must use  MatrixElementOperation<ElementType>, not MatrixElementOperation
    // otherwise compiler error (vs2013)

    if (OperationName == "+")
    {
        return MatrixElementOperation<ElementType>(OutputMatrixC, [](const ElementType& a, const ElementType& b){return a + b; }, InputMatrixA, InputMatrixB);
    }
    else if (OperationName == "-")
    {
        return MatrixElementOperation<ElementType>(OutputMatrixC, [](const ElementType& a, const ElementType& b){return a - b; }, InputMatrixA, InputMatrixB);
    }
    else if (OperationName == "*")
    {
        return MatrixElementOperation<ElementType>(OutputMatrixC, [](const ElementType& a, const ElementType& b){return a * b; }, InputMatrixA, InputMatrixB);
    }
    else if (OperationName == "/")
    {
        return MatrixElementOperation<ElementType>(OutputMatrixC, [](const ElementType& a, const ElementType& b){return a / b; }, InputMatrixA, InputMatrixB);
    }
    else if (OperationName == "^")
    {
        return MatrixElementOperation<ElementType>(OutputMatrixC, [](const ElementType& a, const ElementType& b){return std::pow(a, b); }, InputMatrixA, InputMatrixB);
    }
    else
    {
        mdkError << " unknown Operation @ MatrixElementOperation(OutputMatrixC, OperationName, InputMatrixA, InputMatrixB)" << '\n';

        return false;
    }

}


template<typename ElementType>
inline 
mdkMatrix<ElementType> MatrixElementOperation(std::function<ElementType(const ElementType&, const ElementType&)> Operation,
                                              const mdkMatrix<ElementType>& InputMatrixA,
                                              const mdkMatrix<ElementType>& InputMatrixB)
{
    mdkMatrix<ElementType> tempMatrix;

    MatrixElementOperation<ElementType>(tempMatrix, Operation, InputMatrixA, InputMatrixB);

    return tempMatrix;
}


template<typename ElementType>
inline 
bool MatrixElementOperation(mdkMatrix<ElementType>& OutputMatrixC,
                            std::function<ElementType(const ElementType&, const ElementType&)> Operation,
                            const mdkMatrix<ElementType>& InputMatrixA,
                            const mdkMatrix<ElementType>& InputMatrixB)
{
    auto SizeA = InputMatrixA.GetSize();

    auto SizeB = InputMatrixB.GetSize();

    if (SizeA.RowNumber == 0 || SizeB.RowNumber ==0)
    {
        mdkError << "InputMatrixA or InputMatrixB is empty @ mdkLinearAlgebra MatrixElementOperation(OutputMatrix, Operation, InputMatrixA, InputMatrixB)" << '\n';

        return false;
    }

    if (SizeB.RowNumber == 1 && SizeB.ColNumber == 1)
    {
        return MatrixElementOperation(OutputMatrixC, Operation, InputMatrixA, InputMatrixB(0));
    }

    auto Flag_row = 0;

    auto Flag_col = 0;

    auto Flag_full = 0;

    if (SizeB.ColNumber == SizeA.ColNumber && SizeB.RowNumber == SizeA.RowNumber)
    {
        Flag_full = 1;
    }
    else if (SizeB.ColNumber == 1 && SizeB.RowNumber == SizeA.RowNumber)
    {
        Flag_col = 1;
    }
    else if (SizeB.ColNumber == SizeA.ColNumber && SizeB.RowNumber == 1)
    {
        Flag_row = 1;
    }
    else
    {
        mdkError << "Size does not match @ mdkLinearAlgebra MatrixElementOperation(OutputMatrix, Operation, InputMatrixA, InputMatrixB)" << '\n';

        return false;
    }

    auto SizeC = OutputMatrixC.GetSize();

    if (SizeC.RowNumber != SizeA.RowNumber || SizeC.ColNumber != SizeA.ColNumber)
    {
        if (OutputMatrixC.IsSizeFixed() == false)
        {
            OutputMatrixC.Clear();

            OutputMatrixC.Resize(SizeA.RowNumber, SizeA.ColNumber);
        }
        else
        {
            mdkError << "Size of OutputMatrixC does not match @ mdkLinearAlgebra MatrixElementOperation(OutputMatrixC, Operation, InputMatrixA, InputMatrixB)" << '\n';
            return false;
        }
    }

    auto ptrC = OutputMatrixC.GetElementDataRawPointer();

    auto ptrA = InputMatrixA.GetElementDataRawPointer();

    auto ptrB = InputMatrixB.GetElementDataRawPointer();

    if (Flag_full == 1)
    {
        auto ElementNumber = SizeA.RowNumber * SizeA.ColNumber;

        for (uint64 i = 0; i < ElementNumber; ++i)
        {
            ptrC[i] = Operation(ptrA[i], ptrB[i]);
        }
    }
    else if (Flag_col == 1)
    {
        for (uint64 j = 0; j < SizeA.ColNumber; ++j)
        {
            for (uint64 i = 0; i < SizeA.RowNumber; ++i)
            {
                ptrC[0] = Operation(ptrA[0], ptrB[i]);

                ++ptrC;
                ++ptrA;
            }
        }
    }
    else if (Flag_row == 1)
    {
        for (uint64 i = 0; i < SizeA.RowNumber; ++i)
        {
            uint64 tempIndex = 0;

            for (uint64 j = 0; j < SizeA.ColNumber; ++j)
            {
                ptrC[tempIndex + i] = Operation(ptrA[tempIndex + i], ptrB[i]);

                tempIndex += SizeA.RowNumber;
            }
        }
    }

    return true;
}

// ------------------------ MatrixElementOperation on InputMatrixA with InputElementB

template<typename ElementType>
inline 
mdkMatrix<ElementType> MatrixElementOperation(const char* OperationName,
                                              const mdkMatrix<ElementType>& InputMatrixA,
                                              const ElementType& InputElementB)
{
    return MatrixElementOperation(std::string(OperationName), InputMatrixA, InputElementB);
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> MatrixElementOperation(const std::string& OperationName,
                                              const mdkMatrix<ElementType>& InputMatrixA,
                                              const ElementType& InputElementB)
{
    mdkMatrix<ElementType> tempMatrix;

    MatrixElementOperation(tempMatrix, OperationName, InputMatrixA, InputElementB);

    return tempMatrix;
}


template<typename ElementType>
inline 
bool MatrixElementOperation(mdkMatrix<ElementType>& OutputMatrixC,
                            const std::string& OperationName,
                            const mdkMatrix<ElementType>& InputMatrixA,
                            const ElementType& InputElementB)

{
    // note: must use  MatrixElementOperation<ElementType>, not MatrixElementOperation
    // otherwise compiler error (vs2013)

    std::string FunctionName(OperationName);

    if (FunctionName == "+")
    {
        return MatrixElementOperation<ElementType>(OutputMatrixC, [](const ElementType& a, const ElementType& b){return a + b; }, InputMatrixA, InputElementB);
    }
    else if (FunctionName == "-")
    {
        return MatrixElementOperation<ElementType>(OutputMatrixC, [](const ElementType& a, const ElementType& b){return a - b; }, InputMatrixA, InputElementB);
    }
    else if (FunctionName == "*")
    {
        return MatrixElementOperation<ElementType>(OutputMatrixC, [](const ElementType& a, const ElementType& b){return a * b; }, InputMatrixA, InputElementB);
    }
    else if (FunctionName == "/")
    {
        return MatrixElementOperation<ElementType>(OutputMatrixC, [](const ElementType& a, const ElementType& b){return a / b; }, InputMatrixA, InputElementB);
    }
    else if (FunctionName == "^")
    {
        return MatrixElementOperation<ElementType>(OutputMatrixC, [](const ElementType& a, const ElementType& b){return std::pow(a, b); }, InputMatrixA, InputElementB);
    }
    else
    {
        mdkError << " unknown operator @ mdkLinearAlgebra MatrixElementOperation(OutputMatrixC, OperationName, InputMatrixA, InputElementB)" << '\n';

        return false;
    }
}


template<typename ElementType>
inline 
mdkMatrix<ElementType> MatrixElementOperation(std::function<ElementType(const ElementType&, const ElementType&)> Operation,
                                              const mdkMatrix<ElementType>& InputMatrixA,
                                              const ElementType& InputElementB)
{
    mdkMatrix<ElementType> tempMatrix;

    MatrixElementOperation<ElementType>(tempMatrix, Operation, InputMatrixA, InputElementB);

    return tempMatrix;
}



template<typename ElementType>
inline
bool MatrixElementOperation(mdkMatrix<ElementType>& OutputMatrixC, 
                            std::function<ElementType(const ElementType&, const ElementType&)> Operation,
                            const mdkMatrix<ElementType>& InputMatrixA,
                            const ElementType& InputElementB)
{
    auto SizeA = InputMatrixA.GetSize();

    if (SizeA.RowNumber == 0)
    {
        mdkError << "InputMatrixA is empty @ MatrixElementOperation(OutputMatrixC, Operation, InputMatrixA, InputElementB)" << '\n';

        return false;
    }

    auto SizeC = OutputMatrixC.GetSize();

    if (SizeC.RowNumber != SizeA.RowNumber || SizeC.ColNumber != SizeA.ColNumber)
    {
        if (OutputMatrixC.IsSizeFixed() == false)
        {
            OutputMatrixC.Clear();

            OutputMatrixC.Resize(SizeA.RowNumber, SizeA.ColNumber);
        }
        else
        {
            mdkError << "Size of OutputMatrixC does not match @ mdkLinearAlgebra MatrixElementOperation(OutputMatrixC, Operation, InputMatrixA, InputElementB)" << '\n';
            return false;
        }
    }

    auto ptrC = OutputMatrixC.GetElementDataRawPointer();

    auto ptrA = InputMatrixA.GetElementDataRawPointer();

    auto ElementNumber = SizeA.RowNumber * SizeA.ColNumber;

    if (ptrC == ptrA) // in place MatrixElementOperation
    {
        for (auto Ptr = ptrC; Ptr < ptrC + ElementNumber; ++Ptr)
        {
            Ptr[0] = Operation(Ptr[0], InputElementB);
        }
    }
    else
    {
        for (uint64 i = 0; i < ElementNumber; ++i)
        {
            ptrC[i] = Operation(ptrC[i], InputElementB);
        }
    }

    return true;
}


//======================================================================================================================================//
//------------------------------------------ MatrixLinearCombine ----------------------------------------------------------------------//
//======================================================================================================================================//


template<typename ElementType>
inline
mdkMatrix<ElementType> MatrixLinearCombine(const std::vector<ElementType>& CoefList, const std::vector<mdkMatrix<ElementType>*>& MatrixList)
{
    auto Size = MatrixList[0]->GetSize();

    mdkMatrix<ElementType> tempMatrix(Size.RowNumber, Size.ColNumber);

    MatrixLinearCombine(tempMatrix, CoefList, MatrixList);

    return tempMatrix;
}


template<typename ElementType>
inline 
bool MatrixLinearCombine(mdkMatrix<ElementType>& OutputMatrix, const std::vector<ElementType>& CoefList, const std::vector<mdkMatrix<ElementType>*>& MatrixList)
{
	auto MatrixNumber = MatrixList.size();

    auto CoefNumber = CoefList.size();

    if (MatrixNumber != CoefNumber)
    {
        mdkError << "MatrixNumber != CoefNumber @ mdkLinearAlgebra MatrixLinearCombine(OutputMatrix, AlphaList, MatrixList)" << '\n';
        return false;
    }

    if (MatrixNumber == 0)
    {
        mdkWarning << "MatrixList is empty @ mdkLinearAlgebra MatrixLinearCombine(OutputMatrix, AlphaList, MatrixList)" << '\n';
        return false;
    }

	auto Size = MatrixList[0]->GetSize();

    if (Size.RowNumber != OutputMatrix.GetRowNumber() || Size.ColNumber != OutputMatrix.GetColNumber())
    {
        if (OutputMatrix.IsSizeFixed() == false)
        {
            OutputMatrix.Clear();

            OutputMatrix.Resize(Size.RowNumber, Size.ColNumber);
        }
        else
        {
            mdkError << "OutputMatrix Size does not match @ mdkLinearAlgebra MatrixLinearCombine(OutputMatrix, AlphaList, MatrixList)" << '\n';
            return false;
        }
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

    case 7:
        MatrixLinearCombine_UnrollForLoop_7(OutputRawPointer, ElementNumber, CoefRawPtr, MatrixElementDataRawPtrList);
        break;

    case 8:
        MatrixLinearCombine_UnrollForLoop_8(OutputRawPointer, ElementNumber, CoefRawPtr, MatrixElementDataRawPtrList);
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

    return true;
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

//======================================================================================================================================================//



}//end namespace mdk

#endif