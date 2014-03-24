#ifndef __mdkLinearAlgebra_DenseMatrix_Function_DenseMatrix_Part_1_hpp
#define __mdkLinearAlgebra_DenseMatrix_Function_DenseMatrix_Part_1_hpp

//#include "mdkLinearAlgebra_DenseMatrix_Function_DenseMatrix_Part_1.h"

namespace mdk
{

//=========================================================================================================================================//
//------------ MatrixAdd, MatrixSubtract, MatrixMultiply, MatrixElementMultiply, MatrixElementDivide ---------------------------------------//
//=========================================================================================================================================//


//------------------- OutputMatrixC = MatrixA operator MatrixB --------------------------------------------//

template<typename ElementType>
inline 
mdkDenseMatrix<ElementType> MatrixAdd(const mdkDenseMatrix<ElementType>& MatrixA, const mdkDenseMatrix<ElementType>& MatrixB)
{
    mdkDenseMatrix<ElementType> tempMatrix;

    MatrixAdd(tempMatrix, MatrixA, MatrixB);

    return  tempMatrix;
}


template<typename ElementType>
inline
bool MatrixAdd(mdkDenseMatrix<ElementType>& OutputMatrixC, const mdkDenseMatrix<ElementType>& MatrixA, const mdkDenseMatrix<ElementType>& MatrixB)
{
    auto SizeA = MatrixA.GetSize();

    auto SizeB = MatrixB.GetSize();

    if (SizeA.RowNumber == 0 || SizeB.RowNumber == 0)
    {
        mdkError << "MatrixA or MatrixB is empty @ mdkLinearAlgebra_DenseMatrix MatrixAdd(OutputMatrixC, MatrixA, MatrixB)" << '\n';
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
        mdkError << "MatrixA Size does not match MatrixB Size @ mdkLinearAlgebra_DenseMatrix MatrixAdd(OutputMatrixC, MatrixA, MatrixB)" << '\n';
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
            mdkError << "OutputMatrixC Size does not match @ mdkLinearAlgebra_DenseMatrix MatrixAdd(OutputMatrixC, MatrixA, MatrixB)" << '\n';
            return false;
        }
    }

    auto ptrC = OutputMatrixC.GetElementPointer();

    auto ptrA = MatrixA.GetElementPointer();

    auto ptrB = MatrixB.GetElementPointer();

    auto ElementNumber = SizeA.RowNumber*SizeA.ColNumber;

    for (int64 i = 0; i < ElementNumber; ++i)
    {
        ptrC[i] = ptrA[i] + ptrB[i];
    }

    return true;
}


template<typename ElementType>
inline 
mdkDenseMatrix<ElementType> MatrixSubtract(const mdkDenseMatrix<ElementType>& MatrixA, const mdkDenseMatrix<ElementType>& MatrixB)
{
    mdkDenseMatrix<ElementType> tempMatrix;

    MatrixSubtract(tempMatrix, MatrixA, MatrixB);

    return  tempMatrix;
}


template<typename ElementType>
inline
bool MatrixSubtract(mdkDenseMatrix<ElementType>& OutputMatrixC, const mdkDenseMatrix<ElementType>& MatrixA, const mdkDenseMatrix<ElementType>& MatrixB)
{
    auto SizeA = MatrixA.GetSize();

    auto SizeB = MatrixB.GetSize();

    if (SizeA.RowNumber == 0 || SizeA.ColNumber == 0)
    {
        mdkError << "MatrixA or MatrixB is empty @ mdkLinearAlgebra_DenseMatrix MatrixSubtract(OutputMatrixC, MatrixA, MatrixB)" << '\n';
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
        mdkError << "MatrixA Size does not match MatrixB Size @ mdkLinearAlgebra_DenseMatrix MatrixSubtract(OutputMatrixC, MatrixA, MatrixB)" << '\n';
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
            mdkError << "OutputMatrixC Size does not match @ mdkLinearAlgebra_DenseMatrix MatrixSubtract(OutputMatrixC, MatrixA, MatrixB)" << '\n';
            return false;
        }
    }

    auto ptrC = OutputMatrixC.GetElementPointer();

    auto ptrA = MatrixA.GetElementPointer();

    auto ptrB = MatrixB.GetElementPointer();

    auto ElementNumber = SizeA.RowNumber*SizeA.ColNumber;

    for (int64 i = 0; i < ElementNumber; ++i)
    {
        ptrC[i] = ptrA[i] - ptrB[i];
    }

    return true;
}


template<typename ElementType>
inline 
mdkDenseMatrix<ElementType> MatrixMultiply(const mdkDenseMatrix<ElementType>& MatrixA, const mdkDenseMatrix<ElementType>& MatrixB)
{
    mdkDenseMatrix<ElementType> tempMatrix;

    MatrixMultiply(tempMatrix, MatrixA, MatrixB);

    return  tempMatrix;
}


template<typename ElementType>
inline
bool MatrixMultiply(mdkDenseMatrix<ElementType>& OutputMatrixC, const mdkDenseMatrix<ElementType>& MatrixA, const mdkDenseMatrix<ElementType>& MatrixB)
{
    auto SizeA = MatrixA.GetSize();

    auto SizeB = MatrixB.GetSize();

    if (SizeA.RowNumber == 0 || SizeB.RowNumber == 0)
    {
        mdkError << "MatrixA or MatrixB is empty @ mdkLinearAlgebra_DenseMatrix MatrixMultiply(OutputMatrixC, MatrixA, MatrixB)" << '\n';
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
        mdkError << "MatrixA Size does not match MatrixB Size @ mdkLinearAlgebra_DenseMatrix MatrixMultiply(OutputMatrixC, MatrixA, MatrixB)" << '\n';
        return false;
    }

    auto SizeC = OutputMatrixC.GetSize();

    if (SizeC.RowNumber > 0)
    {
        auto ptrA = MatrixA.GetElementPointer();

        auto ptrB = MatrixB.GetElementPointer();

        auto ptrC = OutputMatrixC.GetElementPointer();

        if (ptrC == ptrA || ptrC == ptrB)
        {
            // OutputMatrixC is MatrixA or MatrixB
            // create a temp matrix and call this function again
            // 

            mdkDenseMatrix<ElementType> tempMatrix;

            MatrixMultiply(tempMatrix, MatrixA, MatrixB);

            OutputMatrixC.Take(tempMatrix);

            return true;
        }
    }

    if (SizeC.RowNumber != SizeA.RowNumber || SizeC.ColNumber != SizeB.ColNumber)
    {
        if (OutputMatrixC.IsSizeFixed() == false)
        {
            OutputMatrixC.Clear();

            OutputMatrixC.Resize(SizeA.RowNumber, SizeB.ColNumber);
        }
        else
        {
            mdkError << "OutputMatrixC Size does not match @ mdkLinearAlgebra_DenseMatrix MatrixMultiply(OutputMatrixC, MatrixA, MatrixB)" << '\n';
            return false;
        }
    }

    // get non- const pointer

    auto ptrA = const_cast<ElementType*>(MatrixA.GetElementPointer());

    auto ptrB = const_cast<ElementType*>(MatrixB.GetElementPointer());

    auto ptrC = OutputMatrixC.GetElementPointer();

    //--------------------- call lapack via armadillo --------------------------------------------------------------------------------

    arma::Mat<double> A(ptrA, arma::uword(MatrixA.GetRowNumber()), arma::uword(MatrixA.GetColNumber()), false);
    arma::Mat<double> B(ptrB, arma::uword(MatrixB.GetRowNumber()), arma::uword(MatrixB.GetColNumber()), false);
    arma::Mat<double> C(ptrC, arma::uword(OutputMatrixC.GetRowNumber()), arma::uword(OutputMatrixC.GetColNumber()), false);

    C = A*B;

    return true;

    //--------------------- for-loop : slow ------------------------------------------------------------------------------------

    int64 IndexB = 0;

    for (int64 j = 0; j < SizeB.ColNumber; ++j)
    {
        for (int64 i = 0; i < SizeA.RowNumber; ++i)
        {
            ElementType sum = ElementType(0);

            int64 IndexA = 0;

            for (int64 k = 0; k < SizeA.ColNumber; ++k)
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
mdkDenseMatrix<ElementType> MatrixElementMultiply(const mdkDenseMatrix<ElementType>& MatrixA, const mdkDenseMatrix<ElementType>& MatrixB)
{
    mdkDenseMatrix<ElementType> tempMatrix;

    MatrixElementMultiply(tempMatrix, MatrixA, MatrixB);

    return  tempMatrix;
}


template<typename ElementType>
inline 
bool MatrixElementMultiply(mdkDenseMatrix<ElementType>& OutputMatrixC, const mdkDenseMatrix<ElementType>& MatrixA, const mdkDenseMatrix<ElementType>& MatrixB)
{
    auto SizeA = MatrixA.GetSize();

    auto SizeB = MatrixB.GetSize();

    if (SizeA.RowNumber == 0 || SizeA.ColNumber == 0)
    {
        mdkError << "MatrixA or MatrixB is empty @ mdkLinearAlgebra_DenseMatrix MatrixElementMultiply(OutputMatrixC, MatrixA, MatrixB)" << '\n';
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
        mdkError << "MatrixA Size does not match MatrixB Size @ mdkLinearAlgebra_DenseMatrix MatrixElementMultiply(OutputMatrixC, MatrixA, MatrixB)" << '\n';
        return false;
    }

    auto SizeC = OutputMatrixC.GetSize();

    if (SizeC.RowNumber != SizeA.RowNumber || SizeC.ColNumber != SizeB.ColNumber)
    {
        if (OutputMatrixC.IsSizeFixed() == false)
        {
            if (SizeC.RowNumber > 0)
            {
                mdkWarning << "OutputMatrixC Size is changed @ mdkLinearAlgebra_DenseMatrix MatrixElementMultiply(OutputMatrixC, MatrixA, MatrixB)" << '\n';
            }

            OutputMatrixC.Clear();

            OutputMatrixC.Resize(SizeA.RowNumber, SizeB.ColNumber);
        }
        else
        {
            mdkError << "OutputMatrixC Size does not match @ mdkLinearAlgebra_DenseMatrix MatrixElementMultiply(OutputMatrixC, MatrixA, MatrixB)" << '\n';
            return false;
        }
    }

    auto ptrC = OutputMatrixC.GetElementPointer();

    auto ptrA = MatrixA.GetElementPointer();

    auto ptrB = MatrixB.GetElementPointer();

    auto ElementNumber = SizeA.RowNumber*SizeA.ColNumber;

    for (int64 i = 0; i < ElementNumber; ++i)
    {
        ptrC[i] = ptrA[i] * ptrB[i];
    }

    return true;
}


template<typename ElementType>
inline 
mdkDenseMatrix<ElementType> MatrixElementDivide(const mdkDenseMatrix<ElementType>& MatrixA, const mdkDenseMatrix<ElementType>& MatrixB)
{
    mdkDenseMatrix<ElementType> tempMatrix;

    MatrixElementDivide(tempMatrix, MatrixA, MatrixB);

    return  tempMatrix;
}


template<typename ElementType>
inline 
bool MatrixElementDivide(mdkDenseMatrix<ElementType>& OutputMatrixC, const mdkDenseMatrix<ElementType>& MatrixA, const mdkDenseMatrix<ElementType>& MatrixB)
{
    auto SizeA = MatrixA.GetSize();

    auto SizeB = MatrixB.GetSize();

    if (SizeA.RowNumber == 0 || SizeA.ColNumber == 0)
    {
        mdkError << "MatrixA or MatrixB is empty @ mdkLinearAlgebra_DenseMatrix MatrixElementDivide(OutputMatrixC, MatrixA, MatrixB)" << '\n';
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
        mdkError << "MatrixA Size does not match MatrixB Size @ mdkLinearAlgebra_DenseMatrix MatrixElementDivide(OutputMatrixC, MatrixA, MatrixB)" << '\n';
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
            mdkError << "OutputMatrixC Size does not match @ mdkLinearAlgebra_DenseMatrix MatrixElementDivide(OutputMatrixC, MatrixA, MatrixB)" << '\n';
            return false;
        }
    }

    auto ptrC = OutputMatrixC.GetElementPointer();

    auto ptrA = MatrixA.GetElementPointer();

    auto ptrB = MatrixB.GetElementPointer();

    auto ElementNumber = SizeA.RowNumber*SizeA.ColNumber;

    for (int64 i = 0; i < ElementNumber; ++i)
    {
        ptrC[i] = ptrA[i] / ptrB[i];
    }

    return true;
}


//------------------- OutputMatrixC =  ElementA operator MatrixB --------------------------------------------//

template<typename ElementType>
inline 
mdkDenseMatrix<ElementType> MatrixAdd(const ElementType& ElementA, const mdkDenseMatrix<ElementType>& MatrixB)
{
    mdkDenseMatrix<ElementType> tempMatrix;

    MatrixAdd(tempMatrix, ElementA, MatrixB);

    return tempMatrix;
}


template<typename ElementType>
inline
bool MatrixAdd(mdkDenseMatrix<ElementType>& OutputMatrixC, const ElementType& ElementA, const mdkDenseMatrix<ElementType>& MatrixB)
{
    auto SizeB = MatrixB.GetSize();

    if (SizeB.RowNumber == 0)
    {
        mdkError << "MatrixB is empty @ mdkLinearAlgebra_DenseMatrix MatrixAdd(OutputMatrixC, ElementA, MatrixB)" << '\n';
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
            mdkError << "OutputMatrixC Size does not match @ mdkLinearAlgebra_DenseMatrix MatrixAdd(OutputMatrixC, ElementA, MatrixB)" << '\n';
            return false;
        }
    }

    auto ptrC = OutputMatrixC.GetElementPointer();

    auto ptrB = MatrixB.GetElementPointer();

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
        for (int64 i = 0; i < ElementNumber; ++i)
        {
            ptrC[i] = ElementA + ptrB[i];
        }
    }

    return true;
}


template<typename ElementType>
inline
mdkDenseMatrix<ElementType> MatrixSubtract(const ElementType& ElementA, const mdkDenseMatrix<ElementType>& MatrixB)
{
    mdkDenseMatrix<ElementType> tempMatrix;

    MatrixSubtract(tempMatrix, ElementA, MatrixB);

    return tempMatrix;
}


template<typename ElementType>
inline
void MatrixSubtract(mdkDenseMatrix<ElementType> OutputMatrixC, const ElementType& ElementA, const mdkDenseMatrix<ElementType>& MatrixB)
{
    auto SizeB = MatrixB.GetSize();

    if (SizeB.RowNumber == 0)
    {
        mdkError << "MatrixB is empty @ mdkLinearAlgebra_DenseMatrix MatrixSubtract(OutputMatrixC, ElementA, MatrixB)" << '\n';
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
            mdkError << "OutputMatrixC Size does not match @ mdkLinearAlgebra_DenseMatrix MatrixSubtract(OutputMatrixC, ElementA, MatrixB)" << '\n';
            return false;
        }
    }

    auto ptrC = OutputMatrixC.GetElementPointer();

    auto ptrB = MatrixB.GetElementPointer();

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
        for (int64 i = 0; i < ElementNumber; ++i)
        {
            ptrC[i] = ElementA - ptrB[i];
        }
    }

    return true;
}


template<typename ElementType>
inline 
mdkDenseMatrix<ElementType> MatrixMultiply(const ElementType& ElementA, mdkDenseMatrix<ElementType>& MatrixB)
{
    mdkDenseMatrix<ElementType> tempMatrix;

    MatrixMultiply(tempMatrix, ElementA, MatrixB);

    return tempMatrix;
}


template<typename ElementType>
inline
bool MatrixMultiply(mdkDenseMatrix<ElementType>& OutputMatrixC, const ElementType& ElementA, const mdkDenseMatrix<ElementType>& MatrixB)
{
    auto SizeB = MatrixB.GetSize();

    if (SizeB.RowNumber == 0)
    {
        mdkError << "MatrixB is empty @ mdkLinearAlgebra_DenseMatrix MatrixMultiply(OutputMatrixC, ElementA, MatrixB)" << '\n';
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
            mdkError << "OutputMatrixC Size does not match @ mdkLinearAlgebra_DenseMatrix MatrixMultiply(OutputMatrixC, ElementA, MatrixB)" << '\n';
            return false;
        }
    }

    auto ptrC = OutputMatrixC.GetElementPointer();

    auto ptrB = MatrixB.GetElementPointer();

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
        for (int64 i = 0; i < ElementNumber; ++i)
        {
            ptrC[i] = ElementA * ptrB[i];
        }
    }

    return true;
}


template<typename ElementType>
inline 
mdkDenseMatrix<ElementType> MatrixElementMultiply(const ElementType& ElementA, const mdkDenseMatrix<ElementType>& MatrixB)
{
    mdkDenseMatrix<ElementType> tempMatrix;

    MatrixElementMultiply(tempMatrix, ElementA, MatrixB);

    return tempMatrix;
}


template<typename ElementType>
inline 
bool MatrixElementMultiply(mdkDenseMatrix<ElementType>& OutputMatrixC, const ElementType& ElementA, const mdkDenseMatrix<ElementType>& MatrixB)
{
    return MatrixMultiply(OutputMatrixC, ElementA, MatrixB);
}


template<typename ElementType>
inline 
mdkDenseMatrix<ElementType> MatrixElementDivide(const ElementType& ElementA, const mdkDenseMatrix<ElementType>& MatrixB)
{
    mdkDenseMatrix<ElementType> tempMatrix;

    MatrixElementDivide(tempMatrix, ElementA, MatrixB);

    return tempMatrix;
}


template<typename ElementType>
inline 
void MatrixElementDivide(mdkDenseMatrix<ElementType>& OutputMatrixC, const ElementType& ElementA, const mdkDenseMatrix<ElementType>& MatrixB)
{
    auto SizeB = MatrixB.GetSize();

    if (SizeB.RowNumber == 0)
    {
        mdkError << "MatrixB is empty @ mdkLinearAlgebra_DenseMatrix MatrixElementDivide(OutputMatrixC, ElementA, MatrixB)" << '\n';
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
            mdkError << "OutputMatrixC Size does not match @ mdkLinearAlgebra_DenseMatrix MatrixElementDivide(OutputMatrixC, ElementA, MatrixB)" << '\n';
            return false;
        }
    }

    auto ptrC = OutputMatrixC.GetElementPointer();

    auto ptrB = MatrixB.GetElementPointer();

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
        for (int64 i = 0; i < ElementNumber; ++i)
        {
            ptrC[i] = ElementA / ptrB[i];
        }
    }

    return true;
}

//-----------------------------------OutputMatrixC = MatrixA operator ElementB ----------------------------------------------------//

template<typename ElementType>
inline
mdkDenseMatrix<ElementType> MatrixAdd(const mdkDenseMatrix<ElementType>& MatrixA, const ElementType& ElementB)
{
    mdkDenseMatrix<ElementType> tempMatrix;

    MatrixAdd(tempMatrix, MatrixA, ElementB);

    return tempMatrix;
}


template<typename ElementType>
inline
bool MatrixAdd(mdkDenseMatrix<ElementType>& OutputMatrixC, const mdkDenseMatrix<ElementType>& MatrixA, const ElementType& ElementB)
{
    auto SizeA = MatrixA.GetSize();

    if (SizeA.RowNumber == 0)
    {
        mdkError << "MatrixA is empty @ mdkLinearAlgebra_DenseMatrix MatrixAdd(OutputMatrixC, MatrixA, ElementB)" << '\n';
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
            mdkError << "OutputMatrixC Size does not match @ mdkLinearAlgebra_DenseMatrix MatrixAdd(OutputMatrixC, MatrixA, ElementB)" << '\n';
            return false;
        }
    }

    auto ptrC = OutputMatrixC.GetElementPointer();

    auto ptrA = MatrixA.GetElementPointer();

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
        for (int64 i = 0; i < ElementNumber; ++i)
        {
            ptrC[i] = ptrA[i] + ElementB;
        }
    }

    return true;
}


template<typename ElementType>
inline
mdkDenseMatrix<ElementType> MatrixSubtract(const mdkDenseMatrix<ElementType>& MatrixA, const ElementType& ElementB)
{
    mdkDenseMatrix<ElementType> tempMatrix;

    MatrixSubtract(tempMatrix, MatrixA, ElementB);

    return tempMatrix;
}


template<typename ElementType>
inline
void MatrixSubtract(mdkDenseMatrix<ElementType> OutputMatrixC, const mdkDenseMatrix<ElementType>& MatrixA, const ElementType& ElementB)
{
    auto SizeA = MatrixA.GetSize();

    if (SizeA.RowNumber == 0)
    {
        mdkError << "MatrixA is empty @ mdkLinearAlgebra_DenseMatrix MatrixSubtract(OutputMatrixC, MatrixA, ElementB)" << '\n';
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
            mdkError << "OutputMatrixC Size does not match @ mdkLinearAlgebra_DenseMatrix MatrixSubtract(OutputMatrixC, MatrixA, ElementB)" << '\n';
            return false;
        }
    }

    auto ptrC = OutputMatrixC.GetElementPointer();

    auto ptrA = MatrixA.GetElementPointer();

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
        for (int64 i = 0; i < ElementNumber; ++i)
        {
            ptrC[i] = ptrA[i] - ElementB;
        }
    }

    return true;
}


template<typename ElementType>
inline
mdkDenseMatrix<ElementType> MatrixMultiply(const mdkDenseMatrix<ElementType>& MatrixA, const ElementType& ElementB)
{
    mdkDenseMatrix<ElementType> tempMatrix;

    MatrixMultiply(tempMatrix, MatrixA, ElementB);

    return tempMatrix;
}


template<typename ElementType>
inline
bool MatrixMultiply(mdkDenseMatrix<ElementType>& OutputMatrixC, const mdkDenseMatrix<ElementType>& MatrixA, const ElementType& ElementB)
{
    auto SizeA = MatrixA.GetSize();

    if (SizeA.RowNumber == 0)
    {
        mdkError << "MatrixA is empty @ mdkLinearAlgebra_DenseMatrix MatrixMultiply(OutputMatrixC, MatrixA, ElementB)" << '\n';
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
            mdkError << "OutputMatrixC Size does not match @ mdkLinearAlgebra_DenseMatrix MatrixMultiply(OutputMatrixC, MatrixA, ElementB)" << '\n';
            return false;
        }
    }

    //---------------------------------------------------------------------------------------------------------------------------------------

    auto ptrC = OutputMatrixC.GetElementPointer();

    auto ptrA = MatrixA.GetElementPointer();

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
        for (int64 i = 0; i < ElementNumber; ++i)
        {
            ptrC[i] = ptrA[i] * ElementB;
        }
    }
   
    return true;
}


template<typename ElementType>
inline
mdkDenseMatrix<ElementType> MatrixElementMultiply(const mdkDenseMatrix<ElementType>& MatrixA, const ElementType& ElementB)
{
    mdkDenseMatrix<ElementType> tempMatrix;

    MatrixElementMultiply(tempMatrix, MatrixA, ElementB);

    return tempMatrix;
}


template<typename ElementType>
inline
bool MatrixElementMultiply(mdkDenseMatrix<ElementType>& OutputMatrixC, const mdkDenseMatrix<ElementType>& MatrixA, const ElementType& ElementB)
{
    return MatrixMultiply(OutputMatrixC, MatrixA, ElementB);
}


template<typename ElementType>
inline
mdkDenseMatrix<ElementType> MatrixElementDivide(const mdkDenseMatrix<ElementType>& MatrixA, const ElementType& ElementB)
{
    mdkDenseMatrix<ElementType> tempMatrix;

    MatrixElementDivide(tempMatrix, MatrixA, ElementB);

    return tempMatrix;
}


template<typename ElementType>
inline
void MatrixElementDivide(mdkDenseMatrix<ElementType>& OutputMatrixC, const mdkDenseMatrix<ElementType>& MatrixA, const ElementType& ElementB)
{
    auto SizeA = MatrixA.GetSize();

    if (SizeA.RowNumber == 0)
    {
        mdkError << "MatrixA is empty @ mdkLinearAlgebra_DenseMatrix MatrixElementDivide(OutputMatrixC, MatrixA, ElementB)" << '\n';
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
            mdkError << "OutputMatrixC Size does not match @ mdkLinearAlgebra_DenseMatrix MatrixElementDivide(OutputMatrixC, MatrixA, ElementB)" << '\n';
            return false;
        }
    }

    auto ptrC = OutputMatrixC.GetElementPointer();

    auto ptrA = MatrixA.GetElementPointer();

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
        for (int64 i = 0; i < ElementNumber; ++i)
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
mdkDenseMatrix<ElementType> MatrixElementNamedOperation(const std::string& OperationName, const mdkDenseMatrix<ElementType>& InputMatrix)
{
    mdkDenseMatrix<ElementType> OutputMatrix;

    MatrixElementNamedOperation(OutputMatrix, OperationName, InputMatrix);

    return OutputMatrix;
}


template<typename ElementType>
inline 
bool MatrixElementNamedOperation(mdkDenseMatrix<ElementType>& OutputMatrix, const std::string& OperationName, const mdkDenseMatrix<ElementType>& InputMatrix)
{
    // note: must use  MatrixElementOperation<ElementType>, not MatrixElementOperation
    // otherwise compiler error (vs2013)

    if (OperationName == "abs")
    {
        return MatrixElementOperation<ElementType>(OutputMatrix, [](const ElementType& a){return std::abs(a); }, InputMatrix);
    }
    else if (OperationName == "sqrt")
    {
        return  MatrixElementOperation<ElementType>(OutputMatrix, [](const ElementType& a){return std::sqrt(a); }, InputMatrix);
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
    else
    {
        mdkError << " unknown Operation @ mdkLinearAlgebra_DenseMatrix MatrixElementNamedOperation(OutputMatrix, OperationName, InputMatrix)" << '\n';

        return false;
    }
}


template<typename ElementType, typename OperationType>
inline 
mdkDenseMatrix<ElementType> MatrixElementOperation(OperationType Operation, const mdkDenseMatrix<ElementType>& InputMatrix)
{
    auto InputSize = InputMatrix.GetSize();

    mdkDenseMatrix<ElementType> OutputMatrix(InputSize.RowNumber, InputSize.ColNumber);

    MatrixElementOperation(OutputMatrix, Operation, InputMatrix);

    return OutputMatrix;
}


template<typename ElementType, typename OperationType>
inline 
bool MatrixElementOperation(mdkDenseMatrix<ElementType>& OutputMatrix, OperationType Operation, const mdkDenseMatrix<ElementType>& InputMatrix)
{
    auto InputSize = InputMatrix.GetSize();

    if (InputSize.RowNumber == 0)
    {
        mdkError << "InputMatrix is empty @ mdkLinearAlgebra_DenseMatrix MatrixElementOperation(OutputMatrix, Operation, InputMatrix)" << '\n';

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
            mdkError << "Size does not match @ mdkLinearAlgebra_DenseMatrix MatrixElementOperation(OutputMatrix, Operation, InputMatrix)" << '\n';
            return false;
        }
    }

    auto ptrOutput = OutputMatrix.GetElementPointer();

    auto ptrInput = InputMatrix.GetElementPointer();

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
        for (int64 i = 0; i < ElementNumber; ++i)
        {
            ptrOutput[i] = Operation(ptrInput[i]);
        }
    }

    return true;
}


// ------------------------ MatrixElementOperation on InputMatrixA with InputMatrixB

template<typename ElementType>
inline 
mdkDenseMatrix<ElementType> MatrixElementNamedOperation(const char OperationName,
                                                        const mdkDenseMatrix<ElementType>& InputMatrixA,
                                                        const mdkDenseMatrix<ElementType>& InputMatrixB)
{                                              
    mdkDenseMatrix<ElementType> OutputMatrix;

    MatrixElementNamedOperation(OutputMatrix, OperationName, InputMatrixA, InputMatrixB);

    return OutputMatrix;
}


template<typename ElementType>
inline 
bool MatrixElementNamedOperation(mdkDenseMatrix<ElementType>& OutputMatrixC,
                                 const char OperationName,
                                 const mdkDenseMatrix<ElementType>& InputMatrixA,
                                 const mdkDenseMatrix<ElementType>& InputMatrixB)
{
    // note: must use  MatrixElementOperation<ElementType>, not MatrixElementOperation
    // otherwise compiler error (vs2013)

    if (OperationName == '+')
    {
        return MatrixElementOperation<ElementType>(OutputMatrixC, [](const ElementType& a, const ElementType& b){return a + b; }, InputMatrixA, InputMatrixB);
    }
    else if (OperationName == '-')
    {
        return MatrixElementOperation<ElementType>(OutputMatrixC, [](const ElementType& a, const ElementType& b){return a - b; }, InputMatrixA, InputMatrixB);
    }
    else if (OperationName == '*')
    {
        return MatrixElementOperation<ElementType>(OutputMatrixC, [](const ElementType& a, const ElementType& b){return a * b; }, InputMatrixA, InputMatrixB);
    }
    else if (OperationName == '/')
    {
        return MatrixElementOperation<ElementType>(OutputMatrixC, [](const ElementType& a, const ElementType& b){return a / b; }, InputMatrixA, InputMatrixB);
    }
    else if (OperationName == '^')
    {
        return MatrixElementOperation<ElementType>(OutputMatrixC, [](const ElementType& a, const ElementType& b){return std::pow(a, b); }, InputMatrixA, InputMatrixB);
    }
    else
    {
        mdkError << " unknown Operation @ MatrixElementNamedOperation(OutputMatrixC, OperationName, InputMatrixA, InputMatrixB)" << '\n';

        return false;
    }

}


template<typename ElementType>
inline 
mdkDenseMatrix<ElementType> MatrixElementNamedOperation(const std::string& OperationName,
                                                        const mdkDenseMatrix<ElementType>& InputMatrixA,
                                                        const mdkDenseMatrix<ElementType>& InputMatrixB)
{                                              
    mdkDenseMatrix<ElementType> OutputMatrix;

    MatrixElementNamedOperation(OutputMatrix, OperationName, InputMatrixA, InputMatrixB);

    return OutputMatrix;
}


template<typename ElementType>
inline 
bool MatrixElementNamedOperation(mdkDenseMatrix<ElementType>& OutputMatrixC,
                                 const std::string& OperationName,
                                 const mdkDenseMatrix<ElementType>& InputMatrixA,
                                 const mdkDenseMatrix<ElementType>& InputMatrixB)
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
        mdkError << " unknown Operation @ MatrixElementNamedOperation(OutputMatrixC, OperationName, InputMatrixA, InputMatrixB)" << '\n';

        return false;
    }

}


template<typename ElementType, typename OperationType>
inline 
mdkDenseMatrix<ElementType> MatrixElementOperation(OperationType Operation,
                                                   const mdkDenseMatrix<ElementType>& InputMatrixA,
                                                   const mdkDenseMatrix<ElementType>& InputMatrixB)
{
    mdkDenseMatrix<ElementType> OutputMatrix;

    MatrixElementOperation<ElementType>(OutputMatrix, Operation, InputMatrixA, InputMatrixB);

    return OutputMatrix;
}


template<typename ElementType, typename OperationType>
inline 
bool MatrixElementOperation(mdkDenseMatrix<ElementType>& OutputMatrixC,
                            OperationType Operation,
                            const mdkDenseMatrix<ElementType>& InputMatrixA,
                            const mdkDenseMatrix<ElementType>& InputMatrixB)
{
    auto SizeA = InputMatrixA.GetSize();

    auto SizeB = InputMatrixB.GetSize();

    if (SizeA.RowNumber == 0 || SizeB.RowNumber ==0)
    {
        mdkError << "InputMatrixA or InputMatrixB is empty @ mdkLinearAlgebra_DenseMatrix MatrixElementOperation(OutputMatrix, Operation, InputMatrixA, InputMatrixB)" << '\n';

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
        mdkError << "Size does not match @ mdkLinearAlgebra_DenseMatrix MatrixElementOperation(OutputMatrix, Operation, InputMatrixA, InputMatrixB)" << '\n';

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
            mdkError << "Size of OutputMatrixC does not match @ mdkLinearAlgebra_DenseMatrix MatrixElementOperation(OutputMatrixC, Operation, InputMatrixA, InputMatrixB)" << '\n';
            return false;
        }
    }

    auto ptrC = OutputMatrixC.GetElementPointer();

    auto ptrA = InputMatrixA.GetElementPointer();

    auto ptrB = InputMatrixB.GetElementPointer();

    if (Flag_full == 1)
    {
        auto ElementNumber = SizeA.RowNumber * SizeA.ColNumber;

        for (int64 i = 0; i < ElementNumber; ++i)
        {
            ptrC[i] = Operation(ptrA[i], ptrB[i]);
        }
    }
    else if (Flag_col == 1)
    {
        for (int64 j = 0; j < SizeA.ColNumber; ++j)
        {
            for (int64 i = 0; i < SizeA.RowNumber; ++i)
            {
                ptrC[i] = Operation(ptrA[i], ptrB[i]);
            }
        }
    }
    else if (Flag_row == 1)
    {
        for (int64 i = 0; i < SizeA.RowNumber; ++i)
        {
            int64 tempIndex = 0;

            for (int64 j = 0; j < SizeA.ColNumber; ++j)
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
mdkDenseMatrix<ElementType> MatrixElementNamedOperation(const char OperationName,
                                                        const mdkDenseMatrix<ElementType>& InputMatrixA,
                                                        const ElementType& InputElementB)
{
    mdkDenseMatrix<ElementType> OutputMatrix;

    MatrixElementNamedOperation(OutputMatrix, OperationName, InputMatrixA, InputElementB);

    return OutputMatrix;
}


template<typename ElementType>
inline 
bool MatrixElementNamedOperation(mdkDenseMatrix<ElementType>& OutputMatrixC,
                                 const char OperationName,
                                 const mdkDenseMatrix<ElementType>& InputMatrixA,
                                 const ElementType& InputElementB)

{
    // note: must use  MatrixElementOperation<ElementType>, not MatrixElementOperation
    // otherwise compiler error (vs2013)

    if (OperationName == '+')
    {
        return MatrixElementOperation<ElementType>(OutputMatrixC, [](const ElementType& a, const ElementType& b){return a + b; }, InputMatrixA, InputElementB);
    }
    else if (OperationName == '-')
    {
        return MatrixElementOperation<ElementType>(OutputMatrixC, [](const ElementType& a, const ElementType& b){return a - b; }, InputMatrixA, InputElementB);
    }
    else if (OperationName == '*')
    {
        return MatrixElementOperation<ElementType>(OutputMatrixC, [](const ElementType& a, const ElementType& b){return a * b; }, InputMatrixA, InputElementB);
    }
    else if (OperationName == '/')
    {
        return MatrixElementOperation<ElementType>(OutputMatrixC, [](const ElementType& a, const ElementType& b){return a / b; }, InputMatrixA, InputElementB);
    }
    else if (OperationName == '^')
    {
        return MatrixElementOperation<ElementType>(OutputMatrixC, [](const ElementType& a, const ElementType& b){return std::pow(a, b); }, InputMatrixA, InputElementB);
    }
    else
    {
        mdkError << " unknown operator @ mdkLinearAlgebra_DenseMatrix MatrixElementNamedOperation(OutputMatrixC, OperationName, InputMatrixA, InputElementB)" << '\n';

        return false;
    }
}


template<typename ElementType>
inline 
mdkDenseMatrix<ElementType> MatrixElementNamedOperation(const std::string& OperationName,
                                                        const mdkDenseMatrix<ElementType>& InputMatrixA,
                                                        const ElementType& InputElementB)
{
    mdkDenseMatrix<ElementType> OutputMatrix;

    MatrixElementNamedOperation(OutputMatrix, OperationName, InputMatrixA, InputElementB);

    return OutputMatrix;
}


template<typename ElementType>
inline 
bool MatrixElementNamedOperation(mdkDenseMatrix<ElementType>& OutputMatrixC,
                                 const std::string& OperationName,
                                 const mdkDenseMatrix<ElementType>& InputMatrixA,
                                 const ElementType& InputElementB)

{
    // note: must use  MatrixElementOperation<ElementType>, not MatrixElementOperation
    // otherwise compiler error (vs2013)

    if (OperationName == "+")
    {
        return MatrixElementOperation<ElementType>(OutputMatrixC, [](const ElementType& a, const ElementType& b){return a + b; }, InputMatrixA, InputElementB);
    }
    else if (OperationName == "-")
    {
        return MatrixElementOperation<ElementType>(OutputMatrixC, [](const ElementType& a, const ElementType& b){return a - b; }, InputMatrixA, InputElementB);
    }
    else if (OperationName == "*")
    {
        return MatrixElementOperation<ElementType>(OutputMatrixC, [](const ElementType& a, const ElementType& b){return a * b; }, InputMatrixA, InputElementB);
    }
    else if (OperationName == "/")
    {
        return MatrixElementOperation<ElementType>(OutputMatrixC, [](const ElementType& a, const ElementType& b){return a / b; }, InputMatrixA, InputElementB);
    }
    else if (OperationName == "^")
    {
        return MatrixElementOperation<ElementType>(OutputMatrixC, [](const ElementType& a, const ElementType& b){return std::pow(a, b); }, InputMatrixA, InputElementB);
    }
    else
    {
        mdkError << " unknown operator @ mdkLinearAlgebra_DenseMatrix MatrixElementOperation(OutputMatrixC, OperationName, InputMatrixA, InputElementB)" << '\n';

        return false;
    }
}


template<typename ElementType, typename OperationType>
inline 
mdkDenseMatrix<ElementType> MatrixElementOperation(OperationType Operation,
                                                   const mdkDenseMatrix<ElementType>& InputMatrixA,
                                                   const ElementType& InputElementB)
{
    mdkDenseMatrix<ElementType> OutputMatrix;

    MatrixElementOperation<ElementType>(OutputMatrix, Operation, InputMatrixA, InputElementB);

    return OutputMatrix;
}



template<typename ElementType, typename OperationType>
inline
bool MatrixElementOperation(mdkDenseMatrix<ElementType>& OutputMatrixC, 
                            OperationType Operation,
                            const mdkDenseMatrix<ElementType>& InputMatrixA,
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
            mdkError << "Size of OutputMatrixC does not match @ mdkLinearAlgebra_DenseMatrix MatrixElementOperation(OutputMatrixC, Operation, InputMatrixA, InputElementB)" << '\n';
            return false;
        }
    }

    auto ptrC = OutputMatrixC.GetElementPointer();

    auto ptrA = InputMatrixA.GetElementPointer();

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
        for (int64 i = 0; i < ElementNumber; ++i)
        {
            ptrC[i] = Operation(ptrA[i], InputElementB);
        }
    }

    return true;
}


//=========================================================================================================================================//
//------------------------------------------ MatrixColOperation ----------------------------------------------------------------------//
//==========================================================================================================================================//

// ------------------------ MatrixColOperation on InputMatrix at InputColIndex

template<typename ElementType>
inline 
mdkDenseMatrix<ElementType> 
MatrixColNamedOperation(const std::string& OperationName, const mdkDenseMatrix<ElementType>& InputMatrix, int64 InputColIndex, const bool Enable_BoundCheck)
{
    mdkDenseMatrix<ElementType> OutputMatrix(InputMatrix.GetRowNumber(), 1);

    MatrixColNamedOperation(OutputMatrix, 0, OperationName, InputMatrix, InputColIndex, Enable_BoundCheck);

    return OutputMatrix;
}


template<typename ElementType>
inline
bool 
MatrixColNamedOperation(mdkDenseMatrix<ElementType>& OutputMatrix, int64 OutputColIndex,
                        const std::string& OperationName, 
                        const mdkDenseMatrix<ElementType>& InputMatrix, int64 InputColIndex,
                        const bool Enable_BoundCheck)
{
    if (OperationName == "abs")
    {
        return MatrixColOperation<ElementType>(OutputMatrix, OutputColIndex, [](const ElementType& a){return std::abs(a); }, InputMatrix, InputColIndex, Enable_BoundCheck);
    }
    else if (OperationName == "sqrt")
    {
        return  MatrixColOperation<ElementType>(OutputMatrix, OutputColIndex, [](const ElementType& a){return std::sqrt(a); }, InputMatrix, InputColIndex, Enable_BoundCheck);
    }
    else if (OperationName == "sin")
    {
        return MatrixColOperation<ElementType>(OutputMatrix, OutputColIndex, [](const ElementType& a){return std::sin(a); }, InputMatrix, InputColIndex, Enable_BoundCheck);
    }
    else if (OperationName == "cos")
    {
        return MatrixColOperation<ElementType>(OutputMatrix, OutputColIndex, [](const ElementType& a){return std::cos(a); }, InputMatrix, InputColIndex, Enable_BoundCheck);
    }
    else if (OperationName == "tan")
    {
        return MatrixColOperation<ElementType>(OutputMatrix, OutputColIndex, [](const ElementType& a){return std::tan(a); }, InputMatrix, InputColIndex, Enable_BoundCheck);
    }
    else
    {
        mdkError << " unknown Operation @ mdkLinearAlgebra_DenseMatrix MatrixColNamedOperation(OutputMatrix, OutputColIndex, OperationName, InputMatrix, InputColIndex, Enable_BoundCheck)" << '\n';

        return false;
    }
}


template<typename ElementType, typename OperationType>
inline 
mdkDenseMatrix<ElementType>
MatrixColOperation(OperationType Operation,
                   const mdkDenseMatrix<ElementType>& InputMatrix, int64 InputColIndex,
                   const bool Enable_BoundCheck)
{
    mdkDenseMatrix<ElementType> OutputMatrix(InputMatrix.GetRowNumber(), 1);

    MatrixColOperation(OutputMatrix, 0, Operation, InputMatrix, InputColIndex, Enable_BoundCheck);

    return OutputMatrix;
}


template<typename ElementType, typename OperationType>
inline 
bool 
MatrixColOperation(mdkDenseMatrix<ElementType>& OutputMatrix, int64 OutputColIndex,
                   OperationType Operation,
                   const mdkDenseMatrix<ElementType>& InputMatrix, int64 InputColIndex, 
                   const bool Enable_BoundCheck)
{
    if (Enable_BoundCheck)
    {
        auto InputSize = InputMatrix.GetSize();

        if (InputSize.RowNumber == 0)
        {
            mdkError << "InputMatrix is empty @ mdkLinearAlgebra_DenseMatrix MatrixColOperation(OutputMatrix, OutputColIndex, Operation, InputMatrix, InputColIndex, Enable_BoundCheck)" << '\n';

            return false;
        }

        auto OutputSize = OutputMatrix.GetSize();

        if (InputSize.RowNumber != OutputSize.RowNumber || InputColIndex >= InputSize.ColNumber || OutputColIndex >= OutputSize.ColNumber)
        {
            mdkError << "Size does not match @ mdkLinearAlgebra_DenseMatrix MatrixColOperation(OutputMatrix, OutputColIndex, Operation, InputMatrix, InputColIndex, Enable_BoundCheck)" << '\n';
            return false;
        }
    }

    //-------------------------------------------------------------------

    auto ptrOutput = OutputMatrix.GetElementPointer();

    auto ptrInput = InputMatrix.GetElementPointer();

    auto RowNumber = OutputMatrix.GetRowNumber();

    if (ptrOutput == ptrInput && OutputColIndex == InputColIndex) // in place operation
    {
        auto Offset = OutputColIndex*RowNumber;

        for (auto Ptr = ptrOutput + Offset; Ptr < ptrOutput + Offset + RowNumber; ++Ptr)
        {
            Ptr[0] = Operation(Ptr[0]);
        }
    }
    else
    {
        auto Offset_output = OutputColIndex*RowNumber;

        auto Offset_input = InputColIndex*RowNumber;

        for (int64 i = 0; i < RowNumber; ++i)
        {
            ptrOutput[i + Offset_output] = Operation(ptrInput[i + Offset_input]);
        }
    }

    return true;
}

// ------------------------ MatrixColOperation on InputMatrixA at InputColIndexA with InputMatrixB

template<typename ElementType>
inline 
mdkDenseMatrix<ElementType> 
MatrixColNamedOperation(const char OperationName,
                        const mdkDenseMatrix<ElementType>& InputMatrixA, int64 InputColIndexA,
                        const mdkDenseMatrix<ElementType>& InputMatrixB,
                        const bool Enable_BoundCheck)
{
    mdkDenseMatrix<ElementType> OutputMatrix(InputMatrixA.GetRowNumber(), 1);

    MatrixColNamedOperation(OutputMatrix, 0, OperationName, InputMatrixA, InputColIndexA, InputMatrixB, Enable_BoundCheck);

    return OutputMatrix;
}


template<typename ElementType>
inline
bool
MatrixColNamedOperation(mdkDenseMatrix<ElementType>& OutputMatrixC, int64 OutputColIndexC,
                        const char OperationName,
                        const mdkDenseMatrix<ElementType>& InputMatrixA, int64 InputColIndexA,
                        const mdkDenseMatrix<ElementType>& InputMatrixB,
                        const bool Enable_BoundCheck)
{
    //note:
    // "*" is {.*} in Matlab 

    if (OperationName == '+')
    {
        return MatrixColOperation<ElementType>(OutputMatrixC, OutputColIndexC, [](const ElementType& a, const ElementType& b){return a + b; }, InputMatrixA, InputColIndexA, InputMatrixB, Enable_BoundCheck);
    }
    else if (OperationName == '-')
    {
        return MatrixColOperation<ElementType>(OutputMatrixC, OutputColIndexC, [](const ElementType& a, const ElementType& b){return a - b; }, InputMatrixA, InputColIndexA, InputMatrixB, Enable_BoundCheck);
    }
    else if (OperationName == '*')
    {
        return MatrixColOperation<ElementType>(OutputMatrixC, OutputColIndexC, [](const ElementType& a, const ElementType& b){return a * b; }, InputMatrixA, InputColIndexA, InputMatrixB, Enable_BoundCheck);
    }
    else if (OperationName == '/')
    {
        return MatrixColOperation<ElementType>(OutputMatrixC, OutputColIndexC, [](const ElementType& a, const ElementType& b){return a / b; }, InputMatrixA, InputColIndexA, InputMatrixB, Enable_BoundCheck);
    }
    else if (OperationName == '^')
    {
        return MatrixColOperation<ElementType>(OutputMatrixC, OutputColIndexC, [](const ElementType& a, const ElementType& b){return std::pow(a, b); }, InputMatrixA, InputColIndexA, InputMatrixB, Enable_BoundCheck);
    }
    else
    {
        mdkError << " unknown Operation @ mdkLinearAlgebra_DenseMatrix MatrixColNamedOperation(OutputMatrixC, OutputColIndexC, OperationName, InputMatrixA, InputColIndexA, InputMatrixB, Enable_BoundCheck)" << '\n';

        return false;
    }
}


template<typename ElementType>
inline 
mdkDenseMatrix<ElementType> 
MatrixColNamedOperation(const std::string& OperationName,
                        const mdkDenseMatrix<ElementType>& InputMatrixA, int64 InputColIndexA,
                        const mdkDenseMatrix<ElementType>& InputMatrixB,
                        const bool Enable_BoundCheck)
{
    mdkDenseMatrix<ElementType> OutputMatrix(InputMatrixA.GetRowNumber(), 1);

    MatrixColNamedOperation(OutputMatrix, 0, OperationName, InputMatrixA, InputColIndexA, InputMatrixB, Enable_BoundCheck);

    return OutputMatrix;
}


template<typename ElementType>
inline
bool
MatrixColNamedOperation(mdkDenseMatrix<ElementType>& OutputMatrixC, int64 OutputColIndexC,
                        const std::string& OperationName,
                        const mdkDenseMatrix<ElementType>& InputMatrixA, int64 InputColIndexA,
                        const mdkDenseMatrix<ElementType>& InputMatrixB,
                        const bool Enable_BoundCheck)
{
    //note:
    // "*" is {.*} in Matlab 

    if (OperationName == "+")
    {
        return MatrixColOperation<ElementType>(OutputMatrixC, OutputColIndexC, [](const ElementType& a, const ElementType& b){return a + b; }, InputMatrixA, InputColIndexA, InputMatrixB, Enable_BoundCheck);
    }
    else if (OperationName == "-")
    {
        return MatrixColOperation<ElementType>(OutputMatrixC, OutputColIndexC, [](const ElementType& a, const ElementType& b){return a - b; }, InputMatrixA, InputColIndexA, InputMatrixB, Enable_BoundCheck);
    }
    else if (OperationName == "*")
    {
        return MatrixColOperation<ElementType>(OutputMatrixC, OutputColIndexC, [](const ElementType& a, const ElementType& b){return a * b; }, InputMatrixA, InputColIndexA, InputMatrixB, Enable_BoundCheck);    
    }
    else if (OperationName == "/")
    {
        return MatrixColOperation<ElementType>(OutputMatrixC, OutputColIndexC, [](const ElementType& a, const ElementType& b){return a / b; }, InputMatrixA, InputColIndexA, InputMatrixB, Enable_BoundCheck);
    }
    else if (OperationName == "^")
    {
        return MatrixColOperation<ElementType>(OutputMatrixC, OutputColIndexC, [](const ElementType& a, const ElementType& b){return std::pow(a, b); }, InputMatrixA, InputColIndexA, InputMatrixB, Enable_BoundCheck);
    }
    else
    {
        mdkError << " unknown Operation @ mdkLinearAlgebra_DenseMatrix MatrixColNamedOperation(OutputMatrixC, OutputColIndexC, OperationName, InputMatrixA, InputColIndexA, InputMatrixB, Enable_BoundCheck)" << '\n';

        return false;
    }
}


template<typename ElementType, typename OperationType>
inline 
mdkDenseMatrix<ElementType>
MatrixColOperation(OperationType Operation,
                   const mdkDenseMatrix<ElementType>& InputMatrixA, int64 InputColIndexA,
                   const mdkDenseMatrix<ElementType>& InputMatrixB,
                   const bool Enable_BoundCheck)
{
    mdkDenseMatrix<ElementType> OutputMatrix(InputMatrixA.GetRowNumber(), 1);

    MatrixColOperation(OutputMatrix, 0, Operation, InputMatrixA, InputColIndexA, InputMatrixB, Enable_BoundCheck);

    return OutputMatrix;
}


template<typename ElementType, typename OperationType>
inline 
bool
MatrixColOperation(mdkDenseMatrix<ElementType>& OutputMatrixC, int64 OutputColIndexC,
                   OperationType Operation,
                   const mdkDenseMatrix<ElementType>& InputMatrixA, int64 InputColIndexA,
                   const mdkDenseMatrix<ElementType>& InputMatrixB,
                   const bool Enable_BoundCheck)
{
    if (InputMatrixB.GetRowNumber() == 1 && InputMatrixB.GetColNumber() == 1)
    {
        return MatrixColOperation(OutputMatrixC, OutputColIndexC, Operation, InputMatrixA, InputColIndexA, InputMatrixB(0), Enable_BoundCheck);
    }

    //-------------------------------------------------------------------

    if (Enable_BoundCheck)
    {
        auto SizeA = InputMatrixA.GetSize();

        auto SizeB = InputMatrixB.GetSize();

        auto SizeC = OutputMatrixC.GetSize();

        if (SizeA.RowNumber == 0 || SizeB.RowNumber == 0)
        {
            mdkError << "InputMatrixA or InputMatrixB is empty @ mdkLinearAlgebra_DenseMatrix MatrixColOperation(OutputMatrixC, OutputColIndexC, Operation, InputMatrixA, InputColIndexA, InputMatrixB, Enable_BoundCheck)" << '\n';

            return false;
        }

        if (OutputColIndexC >= SizeC.ColNumber || SizeC.RowNumber != SizeA.RowNumber || InputColIndexA >= SizeA.ColNumber)
        {
            mdkError << "Size does not match-a @ mdkLinearAlgebra_DenseMatrix MatrixColOperation(OutputMatrixC, OutputColIndexC, Operation, InputMatrixA, InputColIndexA, InputMatrixB, Enable_BoundCheck)" << '\n';
            return false;
        }

        if (SizeB.ColNumber == 1 && SizeB.RowNumber == SizeA.RowNumber)
        {
        }
        else if (SizeB.RowNumber == 1 && SizeB.ColNumber == SizeA.RowNumber)
        {
        }
        else
        {
            mdkError << "Size does not match-b @ mdkLinearAlgebra_DenseMatrix MatrixColOperation(OutputMatrixC, OutputColIndexC, Operation, InputMatrixA, InputColIndexA, InputMatrixB, Enable_BoundCheck)" << '\n';

            return false;
        }
    }

    //-------------------------------------------------------------------

    auto ptrC = OutputMatrixC.GetElementPointer();

    auto ptrA = InputMatrixA.GetElementPointer();

    auto ptrB = InputMatrixB.GetElementPointer();

    auto RowNumber = OutputMatrix.GetRowNumber();

    if (ptrC == ptrA && OutputColIndex == InputColIndex) // in place operation
    {
        auto Offset = OutputColIndexC * RowNumber

        ptrC += Offset;

        for (int64 i = 0; i < RowNumber; ++i)
        {
            ptrC[0] = Operation(ptrC[0], ptrB[i]);

            ++ptrC;
        }
    }
    else
    {
        auto OffsetC = OutputColIndexC * RowNumber;

        auto OffsetA = InputColIndexA * RowNumber;

        for (int64 i = 0; i < RowNumber; ++i)
        {
            ptrC[i + OffsetC] = Operation(ptrA[i + OffsetA], ptrB[i]);
        }
    }
    
    return true;
}

// ------------------------ MatrixColOperation on InputMatrixA at InputColIndexA with InputElementB

template<typename ElementType>
inline 
mdkDenseMatrix<ElementType> 
MatrixColNamedOperation(const char OperationName,
                        const mdkDenseMatrix<ElementType>& InputMatrixA, int64 InputColIndexA,
                        const ElementType& InputElementB,
                        const bool Enable_BoundCheck)
{
    mdkDenseMatrix<ElementType> OutputMatrix(InputMatrixA.GetRowNumber(), 1);

    MatrixColNamedOperation(OutputMatrix, 0, OperationName, InputMatrixA, InputColIndexA, InputElementB, Enable_BoundCheck);

    return OutputMatrix;
}


template<typename ElementType>
inline 
bool
MatrixColNamedOperation(mdkDenseMatrix<ElementType>& OutputMatrixC, int64 OutputColIndexC,
                        const char OperationName,
                        const mdkDenseMatrix<ElementType>& InputMatrixA, int64 InputColIndexA,
                        const ElementType& InputElementB,
                        const bool Enable_BoundCheck)
{
    if (OperationName == '+')
    {
        return MatrixColOperation<ElementType>(OutputMatrixC, OutputColIndexC, [](const ElementType& a, const ElementType& b){return a + b; }, InputMatrixA, InputColIndexA, InputElementB, Enable_BoundCheck);
    }
    else if (OperationName == '-')
    {
        return MatrixColOperation<ElementType>(OutputMatrixC, OutputColIndexC, [](const ElementType& a, const ElementType& b){return a - b; }, InputMatrixA, InputColIndexA, InputElementB, Enable_BoundCheck);
    }
    else if (OperationName == '*')
    {
        return MatrixColOperation<ElementType>(OutputMatrixC, OutputColIndexC, [](const ElementType& a, const ElementType& b){return a * b; }, InputMatrixA, InputColIndexA, InputElementB, Enable_BoundCheck);
    }
    else if (OperationName == '/')
    {
        return MatrixColOperation<ElementType>(OutputMatrixC, OutputColIndexC, [](const ElementType& a, const ElementType& b){return a / b; }, InputMatrixA, InputColIndexA, InputElementB, Enable_BoundCheck);
    }
    else if (OperationName == '^')
    {
        return MatrixColOperation<ElementType>(OutputMatrixC, OutputColIndexC, [](const ElementType& a, const ElementType& b){return std::pow(a, b); }, InputMatrixA, InputColIndexA, InputElementB, Enable_BoundCheck);
    }
    else
    {
        mdkError << " unknown operator @ mdkLinearAlgebra_DenseMatrix MatrixColOperation(OutputMatrixC, OutputColIndexC, OperationName, InputMatrixA, InputColIndexA, InputElementB)" << '\n';

        return false;
    }
}


template<typename ElementType>
inline
mdkDenseMatrix<ElementType> 
MatrixColNamedOperation(const std::string& OperationName,
                        const mdkDenseMatrix<ElementType>& InputMatrixA, int64 InputColIndexA,
                        const ElementType& InputElementB,
                        const bool Enable_BoundCheck)
{
    mdkDenseMatrix<ElementType> OutputMatrix(InputMatrixA.GetRowNumber(), 1);

    MatrixColNamedOperation(OutputMatrix, 0, OperationName, InputMatrixA, InputColIndexA, InputElementB, Enable_BoundCheck);

    return OutputMatrix;
}


template<typename ElementType>
inline 
bool
MatrixColNamedOperation(mdkDenseMatrix<ElementType>& OutputMatrixC, int64 OutputColIndexC,
                        const std::string& OperationName,
                        const mdkDenseMatrix<ElementType>& InputMatrixA, int64 InputColIndexA,
                        const ElementType& InputElementB,
                        const bool Enable_BoundCheck)
{
    if (OperationName == "+")
    {
        return MatrixColOperation<ElementType>(OutputMatrixC, OutputColIndexC, [](const ElementType& a, const ElementType& b){return a + b; }, InputMatrixA, InputColIndexA, InputElementB, Enable_BoundCheck);
    }
    else if (OperationName == "-")
    {
        return MatrixColOperation<ElementType>(OutputMatrixC, OutputColIndexC, [](const ElementType& a, const ElementType& b){return a - b; }, InputMatrixA, InputColIndexA, InputElementB, Enable_BoundCheck);
    }
    else if (OperationName == "*")
    {
        return MatrixColOperation<ElementType>(OutputMatrixC, OutputColIndexC, [](const ElementType& a, const ElementType& b){return a * b; }, InputMatrixA, InputColIndexA, InputElementB, Enable_BoundCheck);
    }
    else if (OperationName == "/")
    {
        return MatrixColOperation<ElementType>(OutputMatrixC, OutputColIndexC, [](const ElementType& a, const ElementType& b){return a / b; }, InputMatrixA, InputColIndexA, InputElementB, Enable_BoundCheck);
    }
    else if (OperationName == "^")
    {
        return MatrixColOperation<ElementType>(OutputMatrixC, OutputColIndexC, [](const ElementType& a, const ElementType& b){return std::pow(a, b); }, InputMatrixA, InputColIndexA, InputElementB, Enable_BoundCheck);
    }
    else
    {
        mdkError << " unknown operator @ mdkLinearAlgebra_DenseMatrix MatrixColNamedOperation(OutputMatrixC, OutputColIndexC, OperationName, InputMatrixA, InputColIndexA, InputElementB)" << '\n';

        return false;
    }
}


template<typename ElementType, typename OperationType>
inline 
mdkDenseMatrix<ElementType> 
MatrixColOperation(OperationType Operation,
                   const mdkDenseMatrix<ElementType>& InputMatrixA, int64 InputColIndexA,
                   const ElementType& InputElementB,
                   const bool Enable_BoundCheck)
{
    mdkDenseMatrix<ElementType> OutputMatrix(InputMatrixA.GetRowNumber(), 1);

    MatrixColOperation(OutputMatrix, 0, Operation, InputMatrixA, InputColIndexA, InputElementB, Enable_BoundCheck);

    return OutputMatrix;
}


template<typename ElementType, typename OperationType>
inline
bool
MatrixColOperation(mdkDenseMatrix<ElementType>& OutputMatrixC, int64 OutputColIndexC,
                   OperationType Operation,
                   const mdkDenseMatrix<ElementType>& InputMatrixA, int64 InputColIndexA,
                   const ElementType& InputElementB,
                   const bool Enable_BoundCheck)
{
    if (Enable_BoundCheck)
    {
        auto SizeA = InputMatrixA.GetSize();

        auto SizeC = OutputMatrixC.GetSize();

        if (SizeA.RowNumber == 0)
        {
            mdkError << "InputMatrixA is empty @ mdkLinearAlgebra_DenseMatrix MatrixColOperation(OutputMatrixC, OutputColIndexC, Operation, InputMatrixA, InputColIndexA, InputElementB, Enable_BoundCheck)" << '\n';

            return false;
        }

        if (OutputColIndexC >= SizeC.ColNumber || SizeC.RowNumber != SizeA.RowNumber || InputColIndexA >= SizeA.ColNumber)
        {
            mdkError << "Size does not match-a @ mdkLinearAlgebra_DenseMatrix MatrixColOperation(OutputMatrixC, OutputColIndexC, Operation, InputMatrixA, InputColIndexA, InputElementB, Enable_BoundCheck)" << '\n';
            return false;
        }
    }

    //-------------------------------------------------------------------

    auto ptrC = OutputMatrixC.GetElementPointer();

    auto ptrA = InputMatrixA.GetElementPointer();

    auto RowNumber = OutputMatrixC.GetRowNumber();

    if (ptrC == ptrA && OutputColIndexC == InputColIndexA) // in place operation
    {
        auto Offset = OutputColIndexC * RowNumber;

        ptrC += Offset;

        for (auto tempPtr = ptrC; tempPtr < ptrC + RowNumber; ++tempPtr)
        {
            tempPtr[0] = Operation(tempPtr[0], InputElementB);
        }
    }
    else
    {
        auto Offset_C = OutputColIndexC * RowNumber;

        auto Offset_A = InputColIndexA * RowNumber;

        for (int64 i = 0; i < RowNumber; ++i)
        {
            ptrC[i + Offset_C] = Operation(ptrA[i + Offset_A], InputElementB);
        }
    }

    return true;
}

//=========================================================================================================================================//
//------------------------------------------ MatrixRowOperation ----------------------------------------------------------------------//
//==========================================================================================================================================//

// ------------------------ MatrixRowOperation on InputMatrix at InputRowIndex

template<typename ElementType>
inline 
mdkDenseMatrix<ElementType>
MatrixRowNamedOperation(const std::string& OperationName,
                        const mdkDenseMatrix<ElementType>& InputMatrix, int64 InputRowIndex,
                        const bool Enable_BoundCheck)
{
    mdkDenseMatrix<ElementType> OutputMatrix(1, InputMatrix.GetColNumber());

    MatrixRowNamedOperation(OutputMatrix, 0, OperationName, InputMatrix, InputRowIndex, Enable_BoundCheck);

    return OutputMatrix;
}


template<typename ElementType>
inline 
bool
MatrixRowNamedOperation(mdkDenseMatrix<ElementType>& OutputMatrix, int64 OutputRowIndex,
                        const std::string& OperationName,
                        const mdkDenseMatrix<ElementType>& InputMatrix, int64 InputRowIndex,
                        const bool Enable_BoundCheck)
{
    if (OperationName == "abs")
    {
        return MatrixRowOperation<ElementType>(OutputMatrix, OutputRowIndex, [](const ElementType& a){return std::abs(a); }, InputMatrix, InputRowIndex, Enable_BoundCheck);
    }
    else if (OperationName == "sqrt")
    {
        return  MatrixRowOperation<ElementType>(OutputMatrix, OutputRowIndex, [](const ElementType& a){return std::sqrt(a); }, InputMatrix, InputRowIndex, Enable_BoundCheck);
    }
    else if (OperationName == "sin")
    {
        return MatrixRowOperation<ElementType>(OutputMatrix, OutputRowIndex, [](const ElementType& a){return std::sin(a); }, InputMatrix, InputRowIndex, Enable_BoundCheck);
    }
    else if (OperationName == "cos")
    {
        return MatrixRowOperation<ElementType>(OutputMatrix, OutputRowIndex, [](const ElementType& a){return std::cos(a); }, InputMatrix, InputRowIndex, Enable_BoundCheck);
    }
    else if (OperationName == "tan")
    {
        return MatrixRowOperation<ElementType>(OutputMatrix, OutputRowIndex, [](const ElementType& a){return std::tan(a); }, InputMatrix, InputRowIndex, Enable_BoundCheck);
    }
    else
    {
        mdkError << " unknown Operation @ mdkLinearAlgebra_DenseMatrix MatrixRowNamedOperation(OutputMatrix, OutputRowIndex, OperationName, InputMatrix, InputRowIndex, Enable_BoundCheck)" << '\n';

        return false;
    }
}


template<typename ElementType, typename OperationType>
inline
mdkDenseMatrix<ElementType> 
MatrixRowOperation(OperationType Operation,
                   const mdkDenseMatrix<ElementType>& InputMatrix, int64 InputRowIndex,
                   const bool Enable_BoundCheck)
{
    mdkDenseMatrix<ElementType> OutputMatrix(1, InputMatrix.GetColNumber());

    MatrixRowOperation(OutputMatrix, 0, Operation, InputMatrix, InputRowIndex, Enable_BoundCheck);

    return OutputMatrix;
}


template<typename ElementType, typename OperationType>
inline
bool
MatrixRowOperation(mdkDenseMatrix<ElementType>& OutputMatrix, int64 OutputRowIndex,
                   OperationType Operation,
                   const mdkDenseMatrix<ElementType>& InputMatrix, int64 InputRowIndex,
                   const bool Enable_BoundCheck)
{
    if (Enable_BoundCheck)
    {
        auto InputSize = InputMatrix.GetSize();

        if (InputSize.RowNumber == 0)
        {
            mdkError << "InputMatrix is empty @ mdkLinearAlgebra_DenseMatrix MatrixRowOperation(OutputMatrix, OutputRowIndex, Operation, InputMatrix, InputRowIndex, Enable_BoundCheck)" << '\n';

            return false;
        }

        auto OutputSize = OutputMatrix.GetSize();

        if (InputSize.ColNumber != OutputSize.ColNumber || InputRowIndex >= InputSize.RowNumber || OutputRowIndex >= OutputSize.RowNumber)
        {
            mdkError << "Size does not match @ mdkLinearAlgebra_DenseMatrix MatrixRowOperation(OutputMatrix, OutputRowIndex, Operation, InputMatrix, InputRowIndex, Enable_BoundCheck)" << '\n';
            return false;
        }
    }

    //-------------------------------------------------------------------

    auto ptrOutput = OutputMatrix.GetElementPointer();

    auto ptrInput = InputMatrix.GetElementPointer();

    auto ColNumber = OutputMatrix.GetColNumber();

    if (ptrOutput == ptrInput) // the same matrix
    {       
        auto RowNumber = OutputMatrix.GetRowNumber();

        if (OutputRowIndex == InputRowIndex) // in place operation
        {
            for (int64 j = 0; j < ColNumber; ++j)
            {
                auto temp = j*RowNumber + OutputRowIndex;

                ptrOutput[temp] = Operation(ptrOutput[temp]);
            }
        }
        else
        {
            for (int64 j = 0; j < ColNumber; ++j)
            {
                auto temp = j*RowNumber;

                ptrOutput[temp + OutputRowIndex] = Operation(ptrInput[temp + InputRowIndex]);
            }
        }
    }
    else
    {
        auto RowNumber_output = OutputMatrix.GetRowNumber();

        auto RowNumber_input = InputMatrix.GetRowNumber();

        for (int64 j = 0; j < ColNumber; ++j)
        {
            ptrOutput[j*RowNumber_output + OutputRowIndex] = Operation(ptrInput[j*RowNumber_input + InputRowIndex]);
        }
    }

    return true;
}

// ------------------------ MatrixRowOperation on InputMatrixA at InputRowIndexA with InputMatrixB

template<typename ElementType>
inline 
mdkDenseMatrix<ElementType> 
MatrixRowNamedOperation(const char OperationName,
                        const mdkDenseMatrix<ElementType>& InputMatrixA, int64 InputRowIndexA,
                        const mdkDenseMatrix<ElementType>& InputMatrixB,
                        const bool Enable_BoundCheck)
{
    mdkDenseMatrix<ElementType> OutputMatrixC(1, InputMatrixA.GetColNumber());

    MatrixRowNamedOperation(OutputMatrixC, 0, OperationName, InputMatrixA, InputRowIndexA, InputMatrixB, Enable_BoundCheck);

    return OutputMatrixC;
}


template<typename ElementType>
inline 
bool
MatrixRowNamedOperation(mdkDenseMatrix<ElementType>& OutputMatrixC, int64 OutputRowIndexC,
                        const char OperationName,
                        const mdkDenseMatrix<ElementType>& InputMatrixA, int64 InputRowIndexA,
                        const mdkDenseMatrix<ElementType>& InputMatrixB,
                        const bool Enable_BoundCheck)
{
    //note:
    // "*" is {.*} in Matlab 

    if (OperationName == '+')
    {
        return MatrixRowOperation<ElementType>(OutputMatrixC, OutputRowIndexC, [](const ElementType& a, const ElementType& b){return a + b; }, InputMatrixA, InputRowIndexA, InputMatrixB, Enable_BoundCheck);
    }
    else if (OperationName == '-')
    {
        return MatrixRowOperation<ElementType>(OutputMatrixC, OutputRowIndexC, [](const ElementType& a, const ElementType& b){return a - b; }, InputMatrixA, InputRowIndexA, InputMatrixB, Enable_BoundCheck);
    }
    else if (OperationName == '*')
    {
        return MatrixRowOperation<ElementType>(OutputMatrixC, OutputRowIndexC, [](const ElementType& a, const ElementType& b){return a * b; }, InputMatrixA, InputRowIndexA, InputMatrixB, Enable_BoundCheck);
    }
    else if (OperationName == '/')
    {
        return MatrixRowOperation<ElementType>(OutputMatrixC, OutputRowIndexC, [](const ElementType& a, const ElementType& b){return a / b; }, InputMatrixA, InputRowIndexA, InputMatrixB, Enable_BoundCheck);
    }
    else if (OperationName == '^')
    {
        return MatrixRowOperation<ElementType>(OutputMatrixC, OutputRowIndexC, [](const ElementType& a, const ElementType& b){return std::pow(a, b); }, InputMatrixA, InputRowIndexA, InputMatrixB, Enable_BoundCheck);
    }
    else
    {
        mdkError << " unknown Operation @ mdkLinearAlgebra_DenseMatrix MatrixRowNamedOperation(OutputMatrixC, OutputRowIndexC, OperationName, InputMatrixA, InputRowIndexA, InputMatrixB, Enable_BoundCheck)" << '\n';

        return false;
    }
}


template<typename ElementType>
inline 
mdkDenseMatrix<ElementType> 
MatrixRowNamedOperation(const std::string& OperationName,
                        const mdkDenseMatrix<ElementType>& InputMatrixA, int64 InputRowIndexA,
                        const mdkDenseMatrix<ElementType>& InputMatrixB,
                        const bool Enable_BoundCheck)
{
    mdkDenseMatrix<ElementType> OutputMatrixC(1, InputMatrixA.GetColNumber());

    MatrixRowNamedOperation(OutputMatrixC, 0, OperationName, InputMatrixA, InputRowIndexA, InputMatrixB, Enable_BoundCheck);

    return OutputMatrixC;
}


template<typename ElementType>
inline 
bool
MatrixRowNamedOperation(mdkDenseMatrix<ElementType>& OutputMatrixC, int64 OutputRowIndexC,
                        const std::string& OperationName,
                        const mdkDenseMatrix<ElementType>& InputMatrixA, int64 InputRowIndexA,
                        const mdkDenseMatrix<ElementType>& InputMatrixB,
                        const bool Enable_BoundCheck)
{
    //note:
    // "*" is {.*} in Matlab 

    if (OperationName == "+")
    {
        return MatrixRowOperation<ElementType>(OutputMatrixC, OutputRowIndexC, [](const ElementType& a, const ElementType& b){return a + b; }, InputMatrixA, InputRowIndexA, InputMatrixB, Enable_BoundCheck);
    }
    else if (OperationName == "-")
    {
        return MatrixRowOperation<ElementType>(OutputMatrixC, OutputRowIndexC, [](const ElementType& a, const ElementType& b){return a - b; }, InputMatrixA, InputRowIndexA, InputMatrixB, Enable_BoundCheck);
    }
    else if (OperationName == "*")
    {
        return MatrixRowOperation<ElementType>(OutputMatrixC, OutputRowIndexC, [](const ElementType& a, const ElementType& b){return a * b; }, InputMatrixA, InputRowIndexA, InputMatrixB, Enable_BoundCheck);
    }
    else if (OperationName == "/")
    {
        return MatrixRowOperation<ElementType>(OutputMatrixC, OutputRowIndexC, [](const ElementType& a, const ElementType& b){return a / b; }, InputMatrixA, InputRowIndexA, InputMatrixB, Enable_BoundCheck);
    }
    else if (OperationName == "^")
    {
        return MatrixRowOperation<ElementType>(OutputMatrixC, OutputRowIndexC, [](const ElementType& a, const ElementType& b){return std::pow(a, b); }, InputMatrixA, InputRowIndexA, InputMatrixB, Enable_BoundCheck);
    }
    else
    {
        mdkError << " unknown Operation @ mdkLinearAlgebra_DenseMatrix MatrixRowOperation(OutputMatrixC, OutputRowIndexC, OperationName, InputMatrixA, InputRowIndexA, InputMatrixB, Enable_BoundCheck)" << '\n';

        return false;
    }
}


template<typename ElementType, typename OperationType>
inline 
mdkDenseMatrix<ElementType> 
MatrixRowOperation(OperationType Operation,
                   const mdkDenseMatrix<ElementType>& InputMatrixA, int64 InputRowIndexA,
                   const mdkDenseMatrix<ElementType>& InputMatrixB,
                   const bool Enable_BoundCheck)
{
    mdkDenseMatrix<ElementType> OutputMatrixC(1, InputMatrixA.GetColNumber());

    MatrixRowOperation(OutputMatrixC, 0, InputMatrixA, Operation, InputRowIndexA, InputMatrixB, Enable_BoundCheck);

    return OutputMatrixC;
}


template<typename ElementType, typename OperationType>
inline 
bool
MatrixRowOperation(mdkDenseMatrix<ElementType>& OutputMatrixC, int64 OutputRowIndexC,
                   OperationType Operation,
                   const mdkDenseMatrix<ElementType>& InputMatrixA, int64 InputRowIndexA,
                   const mdkDenseMatrix<ElementType>& InputMatrixB,
                   const bool Enable_BoundCheck)
{
    if (InputMatrixB.GetRowNumber() == 1 && InputMatrixB.GetColNumber() == 1)
    {
        return MatrixRowOperation(OutputMatrixC, OutputRowIndexC, Operation, InputMatrixA, InputRowIndexA, InputMatrixB(0), Enable_BoundCheck);
    }

    //-------------------------------------------------------------------

    if (Enable_BoundCheck)
    {
        auto SizeA = InputMatrixA.GetSize();

        auto SizeB = InputMatrixB.GetSize();

        auto SizeC = OutputMatrixC.GetSize();

        if (SizeA.RowNumber == 0 || SizeB.RowNumber == 0)
        {
            mdkError << "InputMatrixA or InputMatrixB is empty @ mdkLinearAlgebra_DenseMatrix MatrixRowOperation(OutputMatrixC, OutputRowIndexC, Operation, InputMatrixA, InputRowIndexA, InputMatrixB, Enable_BoundCheck)" << '\n';

            return false;
        }

        if (OutputRowIndexC >= SizeC.RowNumber || SizeC.ColNumber != SizeA.ColNumber || InputRowIndexA >= SizeA.RowNumber)
        {
            mdkError << "Size does not match-a @ mdkLinearAlgebra_DenseMatrix MatrixRowOperation(OutputMatrixC, OutputRowIndexC, Operation, InputMatrixA, InputRowIndexA, InputMatrixB, Enable_BoundCheck)" << '\n';
            return false;
        }

        if (SizeB.ColNumber == 1 && SizeB.RowNumber == SizeA.ColNumber)
        {
        }
        else if (SizeB.RowNumber == 1 && SizeB.ColNumber == SizeA.ColNumber)
        {
        }
        else
        {
            mdkError << "Size does not match-b @ mdkLinearAlgebra_DenseMatrix MatrixRowOperation(OutputMatrixC, OutputRowIndexC, Operation, InputMatrixA, InputRowIndexA, InputMatrixB, Enable_BoundCheck)" << '\n';

            return false;
        }
    }

    //-------------------------------------------------------------------

    auto ptrC = OutputMatrixC.GetElementPointer();

    auto ptrA = InputMatrixA.GetElementPointer();

    auto ptrB = InputMatrixB.GetElementPointer();

    auto ColNumber = OutputMatrixC.GetColNumber();

    if (ptrC == ptrA ) // the same matrix
    {
        auto RowNumber = OutputMatrixC.GetRowNumber();

        if (OutputRowIndexC == InputRowIndexA) // in place operation
        {
            for (int64 j = 0; j < ColNumber; ++j)
            {
                auto temp = j*RowNumber + OutputRowIndexC;

                ptrC[temp] = Operation(ptrC[temp], ptrB[j]);
            }
        }
        else
        {
            for (int64 j = 0; j < ColNumber; ++j)
            {
                auto temp = j*RowNumber;

                ptrC[temp + OutputRowIndexC] = Operation(ptrA[temp + InputRowIndexA], ptrB[j]);
            }
        }
    }
    else
    {
        auto RowNumberC = OutputMatrixC.GetRowNumber();

        auto RowNumberA = InputMatrixA.GetRowNumber();

        for (int64 j = 0; j < ColNumber; ++j)
        {
            ptrC[j*RowNumberC + OutputRowIndexC] = Operation(ptrA[j*RowNumberA + InputRowIndexA], ptrB[j]);
        }
    }

    return true;
}

// ------------------------ MatrixRowOperation on InputMatrixA at InputColIndexA with InputElementB

template<typename ElementType>
inline 
mdkDenseMatrix<ElementType> 
MatrixRowNamedOperation(const char OperationName,
                        const mdkDenseMatrix<ElementType>& InputMatrixA, int64 InputRowIndexA,
                        const ElementType& InputElementB,
                        const bool Enable_BoundCheck)
{
    mdkDenseMatrix<ElementType> OutputMatrixC(1, InputMatrixA.GetColNumber());

    MatrixRowNamedOperation(OutputMatrixC, 0, OperationName, InputMatrixA, InputRowIndexA, InputElementB, Enable_BoundCheck);

    return OutputMatrixC;
}


template<typename ElementType>
inline
bool
MatrixRowNamedOperation(mdkDenseMatrix<ElementType>& OutputMatrixC, int64 OutputRowIndexC,
                        const char OperationName,
                        const mdkDenseMatrix<ElementType>& InputMatrixA, int64 InputRowIndexA,
                        const ElementType& InputElementB,
                        const bool Enable_BoundCheck)
{
    //note:
    // "*" is {.*} in Matlab 

    if (OperationName == '+')
    {
        return MatrixRowOperation<ElementType>(OutputMatrixC, OutputRowIndexC, [](const ElementType& a, const ElementType& b){return a + b; }, InputMatrixA, InputRowIndexA, InputElementB, Enable_BoundCheck);
    }
    else if (OperationName == '-')
    {
        return MatrixRowOperation<ElementType>(OutputMatrixC, OutputRowIndexC, [](const ElementType& a, const ElementType& b){return a - b; }, InputMatrixA, InputRowIndexA, InputElementB, Enable_BoundCheck);
    }
    else if (OperationName == '*')
    {
        return MatrixRowOperation<ElementType>(OutputMatrixC, OutputRowIndexC, [](const ElementType& a, const ElementType& b){return a * b; }, InputMatrixA, InputRowIndexA, InputElementB, Enable_BoundCheck);
    }
    else if (OperationName == '/')
    {
        return MatrixRowOperation<ElementType>(OutputMatrixC, OutputRowIndexC, [](const ElementType& a, const ElementType& b){return a / b; }, InputMatrixA, InputRowIndexA, InputElementB, Enable_BoundCheck);
    }
    else if (OperationName == '^')
    {
        return MatrixRowOperation<ElementType>(OutputMatrixC, OutputRowIndexC, [](const ElementType& a, const ElementType& b){return std::pow(a, b); }, InputMatrixA, InputRowIndexA, InputElementB, Enable_BoundCheck);
    }
    else
    {
        mdkError << " unknown Operation @ mdkLinearAlgebra_DenseMatrix MatrixRowNamedOperation(OutputMatrixC, OutputRowIndexC, OperationName, InputMatrixA, InputRowIndexA, InputElementB, Enable_BoundCheck)" << '\n';

        return false;
    }
}


template<typename ElementType>
inline
mdkDenseMatrix<ElementType>
MatrixRowNamedOperation(const std::string& OperationName,
                        const mdkDenseMatrix<ElementType>& InputMatrixA, int64 InputRowIndexA,
                        const ElementType& InputElementB,
                        const bool Enable_BoundCheck)
{
    mdkDenseMatrix<ElementType> OutputMatrixC(1, InputMatrixA.GetColNumber());

    MatrixRowNamedOperation(OutputMatrixC, 0, OperationName, InputMatrixA, InputRowIndexA, InputElementB, Enable_BoundCheck);

    return OutputMatrixC;
}


template<typename ElementType>
inline
bool
MatrixRowNamedOperation(mdkDenseMatrix<ElementType>& OutputMatrixC, int64 OutputRowIndexC,
                        const std::string& OperationName,
                        const mdkDenseMatrix<ElementType>& InputMatrixA, int64 InputRowIndexA,
                        const ElementType& InputElementB,
                        const bool Enable_BoundCheck)
{
    //note:
    // "*" is {.*} in Matlab 

    if (OperationName == "+")
    {
        return MatrixRowOperation<ElementType>(OutputMatrixC, OutputRowIndexC, [](const ElementType& a, const ElementType& b){return a + b; }, InputMatrixA, InputRowIndexA, InputElementB, Enable_BoundCheck);
    }
    else if (OperationName == "-")
    {
        return MatrixRowOperation<ElementType>(OutputMatrixC, OutputRowIndexC, [](const ElementType& a, const ElementType& b){return a - b; }, InputMatrixA, InputRowIndexA, InputElementB, Enable_BoundCheck);
    }
    else if (OperationName == "*")
    {
        return MatrixRowOperation<ElementType>(OutputMatrixC, OutputRowIndexC, [](const ElementType& a, const ElementType& b){return a * b; }, InputMatrixA, InputRowIndexA, InputElementB, Enable_BoundCheck);
    }
    else if (OperationName == "/")
    {
        return MatrixRowOperation<ElementType>(OutputMatrixC, OutputRowIndexC, [](const ElementType& a, const ElementType& b){return a / b; }, InputMatrixA, InputRowIndexA, InputElementB, Enable_BoundCheck);
    }
    else if (OperationName == "^")
    {
        return MatrixRowOperation<ElementType>(OutputMatrixC, OutputRowIndexC, [](const ElementType& a, const ElementType& b){return std::pow(a, b); }, InputMatrixA, InputRowIndexA, InputElementB, Enable_BoundCheck);
    }
    else
    {
        mdkError << " unknown Operation @ mdkLinearAlgebra_DenseMatrix MatrixRowNamedOperation(OutputMatrixC, OutputRowIndexC, OperationName, InputMatrixA, InputRowIndexA, InputElementB, Enable_BoundCheck)" << '\n';

        return false;
    }
}


template<typename ElementType, typename OperationType>
inline 
mdkDenseMatrix<ElementType> 
MatrixRowOperation(OperationType Operation,
                   const mdkDenseMatrix<ElementType>& InputMatrixA, int64 InputRowIndexA,
                   const ElementType& InputElementB,
                   const bool Enable_BoundCheck)
{
    mdkDenseMatrix<ElementType> OutputMatrixC(1, InputMatrixA.GetColNumber());

    MatrixRowOperation(OutputMatrixC, 0, OperationName, InputMatrixA, InputRowIndexA, InputElementB, Enable_BoundCheck);

    return OutputMatrixC;
}


template<typename ElementType, typename OperationType>
inline
bool
MatrixRowOperation(mdkDenseMatrix<ElementType>& OutputMatrixC, int64 OutputRowIndexC,
                   OperationType Operation,
                   const mdkDenseMatrix<ElementType>& InputMatrixA, int64 InputRowIndexA,
                   const ElementType& InputElementB,
                   const bool Enable_BoundCheck)
{
    if (Enable_BoundCheck)
    {
        auto SizeA = InputMatrixA.GetSize();

        auto SizeC = OutputMatrixC.GetSize();

        if (SizeA.RowNumber == 0)
        {
            mdkError << "InputMatrixA is empty @ mdkLinearAlgebra_DenseMatrix MatrixRowOperation(OutputMatrixC, OutputRowIndexC, Operation, InputMatrixA, InputRowIndexA, InputElementB, Enable_BoundCheck)" << '\n';

            return false;
        }

        if (OutputRowIndexC >= SizeC.RowNumber || SizeC.ColNumber != SizeA.ColNumber || InputRowIndexA >= SizeA.RowNumber)
        {
            mdkError << "Size does not match-a @ mdkLinearAlgebra_DenseMatrix MatrixRowOperation(OutputMatrixC, OutputRowIndexC, Operation, InputMatrixA, InputRowIndexA, InputElementB, Enable_BoundCheck)" << '\n';
            return false;
        }       
    }

    //-------------------------------------------------------------------

    auto ptrC = OutputMatrixC.GetElementPointer();

    auto ptrA = InputMatrixA.GetElementPointer();

    auto ColNumber = OutputMatrixC.GetColNumber();

    if (ptrC == ptrA) // the same matrix
    {
        auto RowNumber = OutputMatrixC.GetRowNumber();

        if (OutputRowIndexC == InputRowIndexA) // in place operation
        {
            for (int64 j = 0; j < ColNumber; ++j)
            {
                auto temp = j*RowNumber + OutputRowIndexC;

                ptrC[temp] = Operation(ptrC[temp], InputElementB);
            }
        }
        else
        {
            for (int64 j = 0; j < ColNumber; ++j)
            {
                auto temp = j*RowNumber;

                ptrC[temp + OutputRowIndexC] = Operation(ptrA[temp + InputRowIndexA], InputElementB);
            }
        }
    }
    else
    {
        auto RowNumberC = OutputMatrixC.GetRowNumber();

        auto RowNumberA = InputMatrixA.GetRowNumber();

        for (int64 j = 0; j < ColNumber; ++j)
        {
            ptrC[j*RowNumberC + OutputRowIndexC] = Operation(ptrA[j*RowNumberA + InputRowIndexA], InputElementB);
        }
    }

    return true;
}



//======================================================================================================================================//
//------------------------------------------ MatrixLinearCombine ----------------------------------------------------------------------//
//======================================================================================================================================//

template<typename ElementType>
inline
mdkDenseMatrix<ElementType> MatrixLinearCombine(const std::vector<ElementType>& CoefList, 
                                                const std::vector<const mdkDenseMatrix<ElementType>*>& MatrixPtrList,
                                                const ElementType& IndependentElement)
{
    mdkDenseMatrix<ElementType> tempMatrix;

    MatrixLinearCombine(tempMatrix, CoefList, MatrixPtrList, IndependentElement);

    return tempMatrix;
}


template<typename ElementType>
inline 
bool MatrixLinearCombine(mdkDenseMatrix<ElementType>& OutputMatrix, 
                         const std::vector<ElementType>& CoefList, 
                         const std::vector<const mdkDenseMatrix<ElementType>*>& MatrixPtrList,
                         const ElementType& IndependentElement)
{
    auto MatrixNumber = int64(MatrixPtrList.size());

    auto CoefNumber = int64(CoefList.size());

    if (MatrixNumber != CoefNumber)
    {
        mdkError << "MatrixNumber != CoefNumber @ mdkLinearAlgebra_DenseMatrix MatrixLinearCombine(OutputMatrix, CoefList, MatrixList)" << '\n';
        return false;
    }

    if (MatrixNumber == 0)
    {
        mdkWarning << "MatrixList is empty @ mdkLinearAlgebra_DenseMatrix MatrixLinearCombine(OutputMatrix, CoefList, MatrixList)" << '\n';
        return false;
    }

    auto Size = MatrixPtrList[0]->GetSize();

    for (int64 k = 1; k < int64(MatrixPtrList.size()); ++k)
    {
        if (Size.RowNumber != MatrixPtrList[k]->GetRowNumber() || Size.ColNumber != MatrixPtrList[k]->GetColNumber())
        {
            mdkError << "Size is not the same in MatrixPtrList @ mdkLinearAlgebra_DenseMatrix MatrixLinearCombine(OutputMatrix, CoefList, MatrixList)" << '\n';

            return false;
        }
    }


    if (Size.RowNumber != OutputMatrix.GetRowNumber() || Size.ColNumber != OutputMatrix.GetColNumber())
    {
        if (OutputMatrix.IsSizeFixed() == false)
        {
            OutputMatrix.Clear();

            OutputMatrix.Resize(Size.RowNumber, Size.ColNumber);
        }
        else
        {
            mdkError << "OutputMatrix Size does not match @ mdkLinearAlgebra_DenseMatrix MatrixLinearCombine(OutputMatrix, AlphaList, MatrixList)" << '\n';
            return false;
        }
    }


    auto PointerToOutput = OutputMatrix.GetElementPointer();

    std::vector<const ElementType*> MatrixElementDataRawPtrList(MatrixNumber);

    for (int64 k = 0; k < MatrixNumber; ++k)
    {
        MatrixElementDataRawPtrList[k] = MatrixPtrList[k]->GetElementPointer();
    }

    auto ElementNumber = Size.ColNumber*Size.RowNumber;

    switch (MatrixNumber)
    {
    case 1:
        MatrixLinearCombine_MatrixNumber_1(PointerToOutput, ElementNumber, CoefList, MatrixElementDataRawPtrList, IndependentElement);
        break;
        
    case 2:
        MatrixLinearCombine_MatrixNumber_2(PointerToOutput, ElementNumber, CoefList, MatrixElementDataRawPtrList, IndependentElement);
        break;

    case 3:
        MatrixLinearCombine_MatrixNumber_3(PointerToOutput, ElementNumber, CoefList, MatrixElementDataRawPtrList, IndependentElement);
        break;

    case 4:
        MatrixLinearCombine_MatrixNumber_4(PointerToOutput, ElementNumber, CoefList, MatrixElementDataRawPtrList, IndependentElement);
        break;

    case 5:
        MatrixLinearCombine_MatrixNumber_5(PointerToOutput, ElementNumber, CoefList, MatrixElementDataRawPtrList, IndependentElement);
        break;

    case 6:       
        MatrixLinearCombine_MatrixNumber_6(PointerToOutput, ElementNumber, CoefList, MatrixElementDataRawPtrList, IndependentElement);
        break;

    case 7:
        MatrixLinearCombine_MatrixNumber_7(PointerToOutput, ElementNumber, CoefList, MatrixElementDataRawPtrList, IndependentElement);
        break;

    case 8:
        MatrixLinearCombine_MatrixNumber_8(PointerToOutput, ElementNumber, CoefList, MatrixElementDataRawPtrList, IndependentElement);
        break;
        
    case 9:
        MatrixLinearCombine_MatrixNumber_9(PointerToOutput, ElementNumber, CoefList, MatrixElementDataRawPtrList, IndependentElement);
        break;

    case 10:
        MatrixLinearCombine_MatrixNumber_10(PointerToOutput, ElementNumber, CoefList, MatrixElementDataRawPtrList, IndependentElement);
        break;
        
    default:
            
        // for-loop #1: faster than #2
        
        auto Coef_0 = CoefList[0];

        auto RawPtr_0 = MatrixElementDataRawPtrList[0];

        for (int64 LinearIndex = 0; LinearIndex < ElementNumber; ++LinearIndex)
        {
            PointerToOutput[LinearIndex] = IndependentElement + Coef_0 * RawPtr_0[LinearIndex];
        }

        for (int64 k = 1; k < MatrixNumber; ++k)
        {
            auto Coef_k = CoefList[k];

            auto RawPtr_k = MatrixElementDataRawPtrList[k];

            for (int64 LinearIndex = 0; LinearIndex < ElementNumber; ++LinearIndex)
            {
                PointerToOutput[LinearIndex] += Coef_k * RawPtr_k[LinearIndex];
            }

        }
        

        // for-loop #2:
        /*
        for (int64 LinearIndex = 0; LinearIndex < ElementNumber; ++LinearIndex)
        {
            ElementType tempElement = IndependentElement;

            for (int64 k = 0; k < MatrixNumber; ++k)
            {
                tempElement += CoefRawPtr[k] * MatrixElementDataRawPtrList[k][LinearIndex];
            }

            PointerToOutput[LinearIndex] = tempElement;
        }
        */


    }

    return true;
}


template<typename ElementType>
inline
void MatrixLinearCombine_MatrixNumber_1(ElementType* Output, int64 ElementNumber,
                                        const std::vector<ElementType>& CoefList,  
                                        const std::vector<const ElementType*>& MatrixElementDataRawPtrList,
                                        const ElementType& IndependentElement)
{
    for (int64 LinearIndex = 0; LinearIndex < ElementNumber; ++LinearIndex)
    {
        Output[LinearIndex] = IndependentElement + CoefList[0] * MatrixElementDataRawPtrList[0][LinearIndex];
    }
}


template<typename ElementType>
inline
void MatrixLinearCombine_MatrixNumber_2(ElementType* Output, int64 ElementNumber,
                                        const std::vector<ElementType>& CoefList,  
                                        const std::vector<const ElementType*>& MatrixElementDataRawPtrList,
                                        const ElementType& IndependentElement)
{
    for (int64 LinearIndex = 0; LinearIndex < ElementNumber; ++LinearIndex)
    {
        Output[LinearIndex] = IndependentElement
                             + CoefList[0] * MatrixElementDataRawPtrList[0][LinearIndex]
                             + CoefList[1] * MatrixElementDataRawPtrList[1][LinearIndex];
    }
}


template<typename ElementType>
inline
void MatrixLinearCombine_MatrixNumber_3(ElementType* Output, int64 ElementNumber,
                                        const std::vector<ElementType>& CoefList,  
                                        const std::vector<const ElementType*>& MatrixElementDataRawPtrList,
                                        const ElementType& IndependentElement)
{
    for (int64 LinearIndex = 0; LinearIndex < ElementNumber; ++LinearIndex)
    {
        Output[LinearIndex] = IndependentElement
                             + CoefList[0] * MatrixElementDataRawPtrList[0][LinearIndex]
                             + CoefList[1] * MatrixElementDataRawPtrList[1][LinearIndex]
                             + CoefList[2] * MatrixElementDataRawPtrList[2][LinearIndex];
    }
}



template<typename ElementType>
inline
void MatrixLinearCombine_MatrixNumber_4(ElementType* Output, int64 ElementNumber,
                                        const std::vector<ElementType>& CoefList,  
                                        const std::vector<const ElementType*>& MatrixElementDataRawPtrList,
                                        const ElementType& IndependentElement)
{
    for (int64 LinearIndex = 0; LinearIndex < ElementNumber; ++LinearIndex)
    {
        Output[LinearIndex] = IndependentElement 
                             + CoefList[0] * MatrixElementDataRawPtrList[0][LinearIndex]
                             + CoefList[1] * MatrixElementDataRawPtrList[1][LinearIndex]
                             + CoefList[2] * MatrixElementDataRawPtrList[2][LinearIndex]
                             + CoefList[3] * MatrixElementDataRawPtrList[3][LinearIndex];
    }
}


template<typename ElementType>
inline
void MatrixLinearCombine_MatrixNumber_5(ElementType* Output, int64 ElementNumber,
                                        const std::vector<ElementType>& CoefList,  
                                        const std::vector<const ElementType*>& MatrixElementDataRawPtrList,
                                        const ElementType& IndependentElement)
{
    for (int64 LinearIndex = 0; LinearIndex < ElementNumber; ++LinearIndex)
    {
        Output[LinearIndex] = IndependentElement
                             + CoefList[0] * MatrixElementDataRawPtrList[0][LinearIndex]
                             + CoefList[1] * MatrixElementDataRawPtrList[1][LinearIndex]
                             + CoefList[2] * MatrixElementDataRawPtrList[2][LinearIndex]
                             + CoefList[3] * MatrixElementDataRawPtrList[3][LinearIndex]
                             + CoefList[4] * MatrixElementDataRawPtrList[4][LinearIndex];
    }
}


template<typename ElementType>
inline
void MatrixLinearCombine_MatrixNumber_6(ElementType* Output, int64 ElementNumber,
                                        const std::vector<ElementType>& CoefList,  
                                        const std::vector<const ElementType*>& MatrixElementDataRawPtrList,
                                        const ElementType& IndependentElement)
{
    for (int64 LinearIndex = 0; LinearIndex < ElementNumber; ++LinearIndex)
    {
        Output[LinearIndex] = IndependentElement
                             + CoefList[0] * MatrixElementDataRawPtrList[0][LinearIndex]
                             + CoefList[1] * MatrixElementDataRawPtrList[1][LinearIndex]
                             + CoefList[2] * MatrixElementDataRawPtrList[2][LinearIndex]
                             + CoefList[3] * MatrixElementDataRawPtrList[3][LinearIndex]
                             + CoefList[4] * MatrixElementDataRawPtrList[4][LinearIndex]
                             + CoefList[5] * MatrixElementDataRawPtrList[5][LinearIndex];
    }
}


template<typename ElementType>
inline
void MatrixLinearCombine_MatrixNumber_7(ElementType* Output, int64 ElementNumber,
                                        const std::vector<ElementType>& CoefList,  
                                        const std::vector<const ElementType*>& MatrixElementDataRawPtrList,
                                        const ElementType& IndependentElement)
{
    for (int64 LinearIndex = 0; LinearIndex < ElementNumber; ++LinearIndex)
    {
        Output[LinearIndex] = IndependentElement
                             + CoefList[0] * MatrixElementDataRawPtrList[0][LinearIndex]
                             + CoefList[1] * MatrixElementDataRawPtrList[1][LinearIndex]
                             + CoefList[2] * MatrixElementDataRawPtrList[2][LinearIndex]
                             + CoefList[3] * MatrixElementDataRawPtrList[3][LinearIndex]
                             + CoefList[4] * MatrixElementDataRawPtrList[4][LinearIndex]
                             + CoefList[5] * MatrixElementDataRawPtrList[5][LinearIndex]
                             + CoefList[6] * MatrixElementDataRawPtrList[6][LinearIndex];
    }
}


template<typename ElementType>
inline
void MatrixLinearCombine_MatrixNumber_8(ElementType* Output, int64 ElementNumber,
                                        const std::vector<ElementType>& CoefList,  
                                        const std::vector<const ElementType*>& MatrixElementDataRawPtrList,
                                        const ElementType& IndependentElement)
{
    for (int64 LinearIndex = 0; LinearIndex < ElementNumber; ++LinearIndex)
    {
        Output[LinearIndex] = IndependentElement
                             + CoefList[0] * MatrixElementDataRawPtrList[0][LinearIndex]
                             + CoefList[1] * MatrixElementDataRawPtrList[1][LinearIndex]
                             + CoefList[2] * MatrixElementDataRawPtrList[2][LinearIndex]
                             + CoefList[3] * MatrixElementDataRawPtrList[3][LinearIndex]
                             + CoefList[4] * MatrixElementDataRawPtrList[4][LinearIndex]
                             + CoefList[5] * MatrixElementDataRawPtrList[5][LinearIndex]
                             + CoefList[6] * MatrixElementDataRawPtrList[6][LinearIndex]
                             + CoefList[7] * MatrixElementDataRawPtrList[7][LinearIndex];
    }
}


template<typename ElementType>
inline
void MatrixLinearCombine_MatrixNumber_9(ElementType* Output, int64 ElementNumber,
                                        const std::vector<ElementType>& CoefList,  
                                        const std::vector<const ElementType*>& MatrixElementDataRawPtrList,
                                        const ElementType& IndependentElement)
{
    for (int64 LinearIndex = 0; LinearIndex < ElementNumber; ++LinearIndex)
    {
        Output[LinearIndex] = IndependentElement
                             + CoefList[0] * MatrixElementDataRawPtrList[0][LinearIndex]
                             + CoefList[1] * MatrixElementDataRawPtrList[1][LinearIndex]
                             + CoefList[2] * MatrixElementDataRawPtrList[2][LinearIndex]
                             + CoefList[3] * MatrixElementDataRawPtrList[3][LinearIndex]
                             + CoefList[4] * MatrixElementDataRawPtrList[4][LinearIndex]
                             + CoefList[5] * MatrixElementDataRawPtrList[5][LinearIndex]
                             + CoefList[6] * MatrixElementDataRawPtrList[6][LinearIndex]
                             + CoefList[7] * MatrixElementDataRawPtrList[7][LinearIndex]
                             + CoefList[8] * MatrixElementDataRawPtrList[8][LinearIndex];
    }
}


template<typename ElementType>
inline
void MatrixLinearCombine_MatrixNumber_10(ElementType* Output, int64 ElementNumber,
                                        const std::vector<ElementType>& CoefList,  
                                        const std::vector<const ElementType*>& MatrixElementDataRawPtrList,
                                        const ElementType& IndependentElement)
{
    for (int64 LinearIndex = 0; LinearIndex < ElementNumber; ++LinearIndex)
    {
        Output[LinearIndex] = IndependentElement
                             + CoefList[0] * MatrixElementDataRawPtrList[0][LinearIndex]
                             + CoefList[1] * MatrixElementDataRawPtrList[1][LinearIndex]
                             + CoefList[2] * MatrixElementDataRawPtrList[2][LinearIndex]
                             + CoefList[3] * MatrixElementDataRawPtrList[3][LinearIndex]
                             + CoefList[4] * MatrixElementDataRawPtrList[4][LinearIndex]
                             + CoefList[5] * MatrixElementDataRawPtrList[5][LinearIndex]
                             + CoefList[6] * MatrixElementDataRawPtrList[6][LinearIndex]
                             + CoefList[7] * MatrixElementDataRawPtrList[7][LinearIndex]
                             + CoefList[8] * MatrixElementDataRawPtrList[8][LinearIndex]
                             + CoefList[9] * MatrixElementDataRawPtrList[9][LinearIndex];
    }
}

//======================================================================================================================================================//



}//end namespace mdk

#endif