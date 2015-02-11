#ifndef mdk_LinearAlgebra_SparseMatrix_Part_1_hpp
#define mdk_LinearAlgebra_SparseMatrix_Part_1_hpp


namespace mdk
{

//=========================================================================================================================================//
//------------ MatrixAdd, MatrixSubtract, MatrixMultiply, MatrixElementMultiply, MatrixElementDivide ---------------------------------------//
//=========================================================================================================================================//


//------------------- OutputMatrixC = MatrixA operator MatrixB --------------------------------------------//

template<typename ElementType>
inline 
mdkSparseMatrix<ElementType> MatrixAdd(const mdkSparseMatrix<ElementType>& MatrixA, const mdkSparseMatrix<ElementType>& MatrixB)
{
    mdkSparseMatrix<ElementType> tempMatrix;

    MatrixAdd(tempMatrix, MatrixA, MatrixB);

    return  tempMatrix;
}


template<typename ElementType>
inline
bool MatrixAdd(mdkSparseMatrix<ElementType>& OutputMatrixC, const mdkSparseMatrix<ElementType>& MatrixA, const mdkSparseMatrix<ElementType>& MatrixB)
{
    auto SizeA = MatrixA.GetSize();

    auto SizeB = MatrixB.GetSize();

    if (SizeA.RowNumber == 0 || SizeB.RowNumber == 0)
    {
        mdkError << "MatrixA or MatrixB is empty @ mdkLinearAlgebra_SparseMatrix MatrixAdd(OutputMatrixC, MatrixA, MatrixB)" << '\n';
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
        mdkError << "MatrixA Size does not match MatrixB Size @ mdkLinearAlgebra_SparseMatrix MatrixAdd(OutputMatrixC, MatrixA, MatrixB)" << '\n';
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
            mdkError << "OutputMatrixC Size does not match @ mdkLinearAlgebra_SparseMatrix MatrixAdd(OutputMatrixC, MatrixA, MatrixB)" << '\n';
            return false;
        }
    }

    //-----------------------------------------------------------------------


    return true;
}


template<typename ElementType>
inline 
mdkSparseMatrix<ElementType> MatrixSubtract(const mdkSparseMatrix<ElementType>& MatrixA, const mdkSparseMatrix<ElementType>& MatrixB)
{
    mdkSparseMatrix<ElementType> tempMatrix;

    MatrixSubtract(tempMatrix, MatrixA, MatrixB);

    return  tempMatrix;
}


template<typename ElementType>
inline
bool MatrixSubtract(mdkSparseMatrix<ElementType>& OutputMatrixC, const mdkSparseMatrix<ElementType>& MatrixA, const mdkSparseMatrix<ElementType>& MatrixB)
{
    auto SizeA = MatrixA.GetSize();

    auto SizeB = MatrixB.GetSize();

    if (SizeA.RowNumber == 0 || SizeA.ColNumber == 0)
    {
        mdkError << "MatrixA or MatrixB is empty @ mdkLinearAlgebra_SparseMatrix MatrixSubtract(OutputMatrixC, MatrixA, MatrixB)" << '\n';
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
        mdkError << "MatrixA Size does not match MatrixB Size @ mdkLinearAlgebra_SparseMatrix MatrixSubtract(OutputMatrixC, MatrixA, MatrixB)" << '\n';
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
            mdkError << "OutputMatrixC Size does not match @ mdkLinearAlgebra_SparseMatrix MatrixSubtract(OutputMatrixC, MatrixA, MatrixB)" << '\n';
            return false;
        }
    }
    
    //-------------------------------------------------------------------------


    return true;
}


template<typename ElementType>
inline 
mdkSparseMatrix<ElementType> MatrixMultiply(const mdkSparseMatrix<ElementType>& MatrixA, const mdkSparseMatrix<ElementType>& MatrixB)
{
    mdkSparseMatrix<ElementType> tempMatrix;

    MatrixMultiply(tempMatrix, MatrixA, MatrixB);

    return  tempMatrix;
}


template<typename ElementType>
inline
bool MatrixMultiply(mdkSparseMatrix<ElementType>& OutputMatrixC, const mdkSparseMatrix<ElementType>& MatrixA, const mdkSparseMatrix<ElementType>& MatrixB)
{
    auto SizeA = MatrixA.GetSize();

    auto SizeB = MatrixB.GetSize();

    if (SizeA.RowNumber == 0 || SizeB.RowNumber == 0)
    {
        mdkError << "MatrixA or MatrixB is empty @ mdkLinearAlgebra_SparseMatrix MatrixMultiply(OutputMatrixC, MatrixA, MatrixB)" << '\n';
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
        mdkError << "MatrixA Size does not match MatrixB Size @ mdkLinearAlgebra_SparseMatrix MatrixMultiply(OutputMatrixC, MatrixA, MatrixB)" << '\n';
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

            mdkSparseMatrix<ElementType> tempMatrix;

            MatrixMultiply(tempMatrix, MatrixA, MatrixB);

            OutputMatrixC.Take(tempMatrix);

            return true;
        }
    }

    if (SizeC.RowNumber != SizeA.RowNumber || SizeC.ColNumber != SizeB.ColNumber)
    {
        auto IsOK = OutputMatrixC.FastResize(SizeA.RowNumber, SizeB.ColNumber);

        if (IsOK == false)
        {
            mdkError << "OutputMatrixC Size does not match @ mdkLinearAlgebra_SparseMatrix MatrixMultiply(OutputMatrixC, MatrixA, MatrixB)" << '\n';
            return false;
        }
    }

   //---------------------------------------------------------------------------------

    return true;
}


template<typename ElementType>
inline 
mdkSparseMatrix<ElementType> MatrixElementMultiply(const mdkSparseMatrix<ElementType>& MatrixA, const mdkSparseMatrix<ElementType>& MatrixB)
{
    mdkSparseMatrix<ElementType> tempMatrix;

    MatrixElementMultiply(tempMatrix, MatrixA, MatrixB);

    return  tempMatrix;
}


template<typename ElementType>
inline 
bool MatrixElementMultiply(mdkSparseMatrix<ElementType>& OutputMatrixC, const mdkSparseMatrix<ElementType>& MatrixA, const mdkSparseMatrix<ElementType>& MatrixB)
{
    auto SizeA = MatrixA.GetSize();

    auto SizeB = MatrixB.GetSize();

    if (SizeA.RowNumber == 0 || SizeA.ColNumber == 0)
    {
        mdkError << "MatrixA or MatrixB is empty @ mdkLinearAlgebra_SparseMatrix MatrixElementMultiply(OutputMatrixC, MatrixA, MatrixB)" << '\n';
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
        mdkError << "MatrixA Size does not match MatrixB Size @ mdkLinearAlgebra_SparseMatrix MatrixElementMultiply(OutputMatrixC, MatrixA, MatrixB)" << '\n';
        return false;
    }

    auto SizeC = OutputMatrixC.GetSize();

    if (SizeC.RowNumber != SizeA.RowNumber || SizeC.ColNumber != SizeB.ColNumber)
    {
        if (OutputMatrixC.IsSizeFixed() == false)
        {
            if (SizeC.RowNumber > 0)
            {
                mdkWarning << "OutputMatrixC Size is changed @ mdkLinearAlgebra_SparseMatrix MatrixElementMultiply(OutputMatrixC, MatrixA, MatrixB)" << '\n';
            }

            OutputMatrixC.Clear();

            OutputMatrixC.Resize(SizeA.RowNumber, SizeB.ColNumber);
        }
        else
        {
            mdkError << "OutputMatrixC Size does not match @ mdkLinearAlgebra_SparseMatrix MatrixElementMultiply(OutputMatrixC, MatrixA, MatrixB)" << '\n';
            return false;
        }
    }

    //-----------------------------------------------------------------
  
    return true;
}

//------------------- OutputMatrixC =  ElementA operator MatrixB --------------------------------------------//

template<typename ElementType>
inline 
mdkSparseMatrix<ElementType> MatrixAdd(const ElementType& ElementA, const mdkSparseMatrix<ElementType>& MatrixB)
{
    mdkSparseMatrix<ElementType> tempMatrix;

    MatrixAdd(tempMatrix, ElementA, MatrixB);

    return tempMatrix;
}


template<typename ElementType>
inline
bool MatrixAdd(mdkSparseMatrix<ElementType>& OutputMatrixC, const ElementType& ElementA, const mdkSparseMatrix<ElementType>& MatrixB)
{
    auto SizeB = MatrixB.GetSize();

    if (SizeB.RowNumber == 0)
    {
        mdkError << "MatrixB is empty @ mdkLinearAlgebra_SparseMatrix MatrixAdd(OutputMatrixC, ElementA, MatrixB)" << '\n';
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
            mdkError << "OutputMatrixC Size does not match @ mdkLinearAlgebra_SparseMatrix MatrixAdd(OutputMatrixC, ElementA, MatrixB)" << '\n';
            return false;
        }
    }

    //------------------------------------------------------------------------

    return true;
}


template<typename ElementType>
inline
mdkSparseMatrix<ElementType> MatrixSubtract(const ElementType& ElementA, const mdkSparseMatrix<ElementType>& MatrixB)
{
    mdkSparseMatrix<ElementType> tempMatrix;

    MatrixSubtract(tempMatrix, ElementA, MatrixB);

    return tempMatrix;
}


template<typename ElementType>
inline
void MatrixSubtract(mdkSparseMatrix<ElementType> OutputMatrixC, const ElementType& ElementA, const mdkSparseMatrix<ElementType>& MatrixB)
{
    auto SizeB = MatrixB.GetSize();

    if (SizeB.RowNumber == 0)
    {
        mdkError << "MatrixB is empty @ mdkLinearAlgebra_SparseMatrix MatrixSubtract(OutputMatrixC, ElementA, MatrixB)" << '\n';
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
            mdkError << "OutputMatrixC Size does not match @ mdkLinearAlgebra_SparseMatrix MatrixSubtract(OutputMatrixC, ElementA, MatrixB)" << '\n';
            return false;
        }
    }

    //-------------------------------------------------------------------

    

    return true;
}


template<typename ElementType>
inline 
mdkSparseMatrix<ElementType> MatrixMultiply(const ElementType& ElementA, mdkSparseMatrix<ElementType>& MatrixB)
{
    mdkSparseMatrix<ElementType> tempMatrix;

    MatrixMultiply(tempMatrix, ElementA, MatrixB);

    return tempMatrix;
}


template<typename ElementType>
inline
bool MatrixMultiply(mdkSparseMatrix<ElementType>& OutputMatrixC, const ElementType& ElementA, const mdkSparseMatrix<ElementType>& MatrixB)
{
    auto SizeB = MatrixB.GetSize();

    if (SizeB.RowNumber == 0)
    {
        mdkError << "MatrixB is empty @ mdkLinearAlgebra_SparseMatrix MatrixMultiply(OutputMatrixC, ElementA, MatrixB)" << '\n';
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
            mdkError << "OutputMatrixC Size does not match @ mdkLinearAlgebra_SparseMatrix MatrixMultiply(OutputMatrixC, ElementA, MatrixB)" << '\n';
            return false;
        }
    }

    //---------------------------------------------------------


    return true;
}


template<typename ElementType>
inline 
mdkSparseMatrix<ElementType> MatrixElementMultiply(const ElementType& ElementA, const mdkSparseMatrix<ElementType>& MatrixB)
{
    mdkSparseMatrix<ElementType> tempMatrix;

    MatrixElementMultiply(tempMatrix, ElementA, MatrixB);

    return tempMatrix;
}


template<typename ElementType>
inline 
bool MatrixElementMultiply(mdkSparseMatrix<ElementType>& OutputMatrixC, const ElementType& ElementA, const mdkSparseMatrix<ElementType>& MatrixB)
{
    return MatrixMultiply(OutputMatrixC, ElementA, MatrixB);
}

//-----------------------------------OutputMatrixC = MatrixA operator ElementB ----------------------------------------------------//

template<typename ElementType>
inline
mdkSparseMatrix<ElementType> MatrixAdd(const mdkSparseMatrix<ElementType>& MatrixA, const ElementType& ElementB)
{
    mdkSparseMatrix<ElementType> tempMatrix;

    MatrixAdd(tempMatrix, MatrixA, ElementB);

    return tempMatrix;
}


template<typename ElementType>
inline
bool MatrixAdd(mdkSparseMatrix<ElementType>& OutputMatrixC, const mdkSparseMatrix<ElementType>& MatrixA, const ElementType& ElementB)
{
    auto SizeA = MatrixA.GetSize();

    if (SizeA.RowNumber == 0)
    {
        mdkError << "MatrixA is empty @ mdkLinearAlgebra_SparseMatrix MatrixAdd(OutputMatrixC, MatrixA, ElementB)" << '\n';
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
            mdkError << "OutputMatrixC Size does not match @ mdkLinearAlgebra_SparseMatrix MatrixAdd(OutputMatrixC, MatrixA, ElementB)" << '\n';
            return false;
        }
    }

    //------------------------------------------------------------

    return true;
}


template<typename ElementType>
inline
mdkSparseMatrix<ElementType> MatrixSubtract(const mdkSparseMatrix<ElementType>& MatrixA, const ElementType& ElementB)
{
    mdkSparseMatrix<ElementType> tempMatrix;

    MatrixSubtract(tempMatrix, MatrixA, ElementB);

    return tempMatrix;
}


template<typename ElementType>
inline
void MatrixSubtract(mdkSparseMatrix<ElementType> OutputMatrixC, const mdkSparseMatrix<ElementType>& MatrixA, const ElementType& ElementB)
{
    auto SizeA = MatrixA.GetSize();

    if (SizeA.RowNumber == 0)
    {
        mdkError << "MatrixA is empty @ mdkLinearAlgebra_SparseMatrix MatrixSubtract(OutputMatrixC, MatrixA, ElementB)" << '\n';
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
            mdkError << "OutputMatrixC Size does not match @ mdkLinearAlgebra_SparseMatrix MatrixSubtract(OutputMatrixC, MatrixA, ElementB)" << '\n';
            return false;
        }
    }

    //------------------------------------------------------------------------


    return true;
}


template<typename ElementType>
inline
mdkSparseMatrix<ElementType> MatrixMultiply(const mdkSparseMatrix<ElementType>& MatrixA, const ElementType& ElementB)
{
    mdkSparseMatrix<ElementType> tempMatrix;

    MatrixMultiply(tempMatrix, MatrixA, ElementB);

    return tempMatrix;
}


template<typename ElementType>
inline
bool MatrixMultiply(mdkSparseMatrix<ElementType>& OutputMatrixC, const mdkSparseMatrix<ElementType>& MatrixA, const ElementType& ElementB)
{
    auto SizeA = MatrixA.GetSize();

    if (SizeA.RowNumber == 0)
    {
        mdkError << "MatrixA is empty @ mdkLinearAlgebra_SparseMatrix MatrixMultiply(OutputMatrixC, MatrixA, ElementB)" << '\n';
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
            mdkError << "OutputMatrixC Size does not match @ mdkLinearAlgebra_SparseMatrix MatrixMultiply(OutputMatrixC, MatrixA, ElementB)" << '\n';
            return false;
        }
    }

    //---------------------------------------------------------------------------------------------------------------------------------------

   
    return true;
}


template<typename ElementType>
inline
mdkSparseMatrix<ElementType> MatrixElementMultiply(const mdkSparseMatrix<ElementType>& MatrixA, const ElementType& ElementB)
{
    mdkSparseMatrix<ElementType> tempMatrix;

    MatrixElementMultiply(tempMatrix, MatrixA, ElementB);

    return tempMatrix;
}


template<typename ElementType>
inline
bool MatrixElementMultiply(mdkSparseMatrix<ElementType>& OutputMatrixC, const mdkSparseMatrix<ElementType>& MatrixA, const ElementType& ElementB)
{
    return MatrixMultiply(OutputMatrixC, MatrixA, ElementB);
}


template<typename ElementType>
inline
mdkSparseMatrix<ElementType> MatrixElementDivide(const mdkSparseMatrix<ElementType>& MatrixA, const ElementType& ElementB)
{
    mdkSparseMatrix<ElementType> tempMatrix;

    MatrixElementDivide(tempMatrix, MatrixA, ElementB);

    return tempMatrix;
}


template<typename ElementType>
inline
bool MatrixElementDivide(mdkSparseMatrix<ElementType>& OutputMatrixC, const mdkSparseMatrix<ElementType>& MatrixA, const ElementType& ElementB)
{
    auto SizeA = MatrixA.GetSize();

    if (SizeA.RowNumber == 0)
    {
        mdkError << "MatrixA is empty @ mdkLinearAlgebra_SparseMatrix MatrixElementDivide(OutputMatrixC, MatrixA, ElementB)" << '\n';
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
            mdkError << "OutputMatrixC Size does not match @ mdkLinearAlgebra_SparseMatrix MatrixElementDivide(OutputMatrixC, MatrixA, ElementB)" << '\n';
            return false;
        }
    }

    //------------------------------------------------------------------------

    return true;

}

//=========================================================================================================================================//
//------------------------------------------ MatrixElementOperation ----------------------------------------------------------------------//
//=========================================================================================================================================//

// ------------------------ MatrixElementOperation on InputMatrix

template<typename ElementType>
inline
mdkSparseMatrix<ElementType> MatrixElementOperation(const char* OperationName, const mdkSparseMatrix<ElementType>& InputMatrix)
{
    return MatrixElementOperation(std::string(OperationName), InputMatrix);
}


template<typename ElementType>
inline 
mdkSparseMatrix<ElementType> MatrixElementOperation(const std::string& OperationName, const mdkSparseMatrix<ElementType>& InputMatrix)
{
    mdkSparseMatrix<ElementType> tempMatrix;

    MatrixElementOperation(tempMatrix, OperationName, InputMatrix);

    return tempMatrix;
}


template<typename ElementType>
inline 
bool MatrixElementOperation(mdkSparseMatrix<ElementType>& OutputMatrix, const std::string& OperationName, const mdkSparseMatrix<ElementType>& InputMatrix)
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
        mdkError << " unknown Operation @ mdkLinearAlgebra_SparseMatrix MatrixElementOperation(OutputMatrix, OperationName, InputMatrix)" << '\n';

        return false;
    }
}


template<typename ElementType>
inline 
mdkSparseMatrix<ElementType> MatrixElementOperation(std::function<ElementType(const ElementType&)> Operation,
                                                    const mdkSparseMatrix<ElementType>& InputMatrix)
{
    auto InputSize = InputMatrix.GetSize();

    mdkSparseMatrix<ElementType> tempMatrix(InputSize.RowNumber, InputSize.ColNumber);

    MatrixElementOperation(tempMatrix, Operation, InputMatrix);

    return tempMatrix;
}


template<typename ElementType>
inline 
bool MatrixElementOperation(mdkSparseMatrix<ElementType>& OutputMatrix, 
                            std::function<ElementType(const ElementType&)> Operation, 
                            const mdkSparseMatrix<ElementType>& InputMatrix)
{
    auto InputSize = InputMatrix.GetSize();

    if (InputSize.RowNumber == 0)
    {
        mdkError << "InputMatrix is empty @ mdkLinearAlgebra_SparseMatrix MatrixElementOperation(OutputMatrix, Operation, InputMatrix)" << '\n';

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
            mdkError << "Size does not match @ mdkLinearAlgebra_SparseMatrix MatrixElementOperation(OutputMatrix, Operation, InputMatrix)" << '\n';
            return false;
        }
    }

    //--------------------------------------------------------------------------------
   

    return true;
}


// ------------------------ MatrixElementOperation on InputMatrixA with InputMatrixB

template<typename ElementType>
inline 
mdkSparseMatrix<ElementType> MatrixElementOperation(const char* OperationName,
                                                    const mdkSparseMatrix<ElementType>& InputMatrixA,
                                                    const mdkSparseMatrix<ElementType>& InputMatrixB)
{                                              
    return MatrixElementOperation(std::string(OperationName), InputMatrixA, InputMatrixB);
}


template<typename ElementType>
inline 
mdkSparseMatrix<ElementType> MatrixElementOperation(const std::string& OperationName,
                                                    const mdkSparseMatrix<ElementType>& InputMatrixA,
                                                    const mdkSparseMatrix<ElementType>& InputMatrixB)
{                                              
    mdkSparseMatrix<ElementType> tempMatrix;

    MatrixElementOperation(tempMatrix, OperationName, InputMatrixA, InputMatrixB);

    return tempMatrix;
}


template<typename ElementType>
inline 
bool MatrixElementOperation(mdkSparseMatrix<ElementType>& OutputMatrixC,
                            const std::string& OperationName,
                            const mdkSparseMatrix<ElementType>& InputMatrixA,
                            const mdkSparseMatrix<ElementType>& InputMatrixB)
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
mdkSparseMatrix<ElementType> MatrixElementOperation(std::function<ElementType(const ElementType&, const ElementType&)> Operation,
                                              const mdkSparseMatrix<ElementType>& InputMatrixA,
                                              const mdkSparseMatrix<ElementType>& InputMatrixB)
{
    mdkSparseMatrix<ElementType> tempMatrix;

    MatrixElementOperation<ElementType>(tempMatrix, Operation, InputMatrixA, InputMatrixB);

    return tempMatrix;
}


template<typename ElementType>
inline 
bool MatrixElementOperation(mdkSparseMatrix<ElementType>& OutputMatrixC,
                            std::function<ElementType(const ElementType&, const ElementType&)> Operation,
                            const mdkSparseMatrix<ElementType>& InputMatrixA,
                            const mdkSparseMatrix<ElementType>& InputMatrixB)
{
    auto SizeA = InputMatrixA.GetSize();

    auto SizeB = InputMatrixB.GetSize();

    if (SizeA.RowNumber == 0 || SizeB.RowNumber ==0)
    {
        mdkError << "InputMatrixA or InputMatrixB is empty @ mdkLinearAlgebra_SparseMatrix MatrixElementOperation(OutputMatrix, Operation, InputMatrixA, InputMatrixB)" << '\n';

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
        mdkError << "Size does not match @ mdkLinearAlgebra_SparseMatrix MatrixElementOperation(OutputMatrix, Operation, InputMatrixA, InputMatrixB)" << '\n';

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
            mdkError << "Size of OutputMatrixC does not match @ mdkLinearAlgebra_SparseMatrix MatrixElementOperation(OutputMatrixC, Operation, InputMatrixA, InputMatrixB)" << '\n';
            return false;
        }
    }

    //--------------------------------------------------------------------------------

    

    return true;
}

// ------------------------ MatrixElementOperation on InputMatrixA with InputElementB

template<typename ElementType>
inline 
mdkSparseMatrix<ElementType> MatrixElementOperation(const char* OperationName,
                                                    const mdkSparseMatrix<ElementType>& InputMatrixA,
                                                    const ElementType& InputElementB)
{
    return MatrixElementOperation(std::string(OperationName), InputMatrixA, InputElementB);
}


template<typename ElementType>
inline 
mdkSparseMatrix<ElementType> MatrixElementOperation(const std::string& OperationName,
                                                    const mdkSparseMatrix<ElementType>& InputMatrixA,
                                                    const ElementType& InputElementB)
{
    mdkSparseMatrix<ElementType> tempMatrix;

    MatrixElementOperation(tempMatrix, OperationName, InputMatrixA, InputElementB);

    return tempMatrix;
}


template<typename ElementType>
inline 
bool MatrixElementOperation(mdkSparseMatrix<ElementType>& OutputMatrixC,
                            const std::string& OperationName,
                            const mdkSparseMatrix<ElementType>& InputMatrixA,
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
        mdkError << " unknown operator @ mdkLinearAlgebra_SparseMatrix MatrixElementOperation(OutputMatrixC, OperationName, InputMatrixA, InputElementB)" << '\n';

        return false;
    }
}


template<typename ElementType>
inline 
mdkSparseMatrix<ElementType> MatrixElementOperation(std::function<ElementType(const ElementType&, const ElementType&)> Operation,
                                                    const mdkSparseMatrix<ElementType>& InputMatrixA,
                                                    const ElementType& InputElementB)
{
    mdkSparseMatrix<ElementType> tempMatrix;

    MatrixElementOperation<ElementType>(tempMatrix, Operation, InputMatrixA, InputElementB);

    return tempMatrix;
}



template<typename ElementType>
inline
bool MatrixElementOperation(mdkSparseMatrix<ElementType>& OutputMatrixC, 
                            std::function<ElementType(const ElementType&, const ElementType&)> Operation,
                            const mdkSparseMatrix<ElementType>& InputMatrixA,
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
            mdkError << "Size of OutputMatrixC does not match @ mdkLinearAlgebra_SparseMatrix MatrixElementOperation(OutputMatrixC, Operation, InputMatrixA, InputElementB)" << '\n';
            return false;
        }
    }

    //--------------------------------------------------------------------------------


    return true;
}


//======================================================================================================================================//
//------------------------------------------ MatrixLinearCombine ----------------------------------------------------------------------//
//======================================================================================================================================//

template<typename ElementType>
inline
mdkSparseMatrix<ElementType> MatrixLinearCombine(const std::vector<ElementType>& CoefList, 
                                                 const std::vector<const mdkSparseMatrix<ElementType>*>& MatrixPtrList,
                                                 const ElementType& IndependentElement)
{
    mdkSparseMatrix<ElementType> tempMatrix;

    MatrixLinearCombine(tempMatrix, CoefList, MatrixPtrList, IndependentElement);

    return tempMatrix;
}


template<typename ElementType>
inline 
bool MatrixLinearCombine(mdkSparseMatrix<ElementType>& OutputMatrix, 
                         const std::vector<ElementType>& CoefList, 
                         const std::vector<const mdkSparseMatrix<ElementType>*>& MatrixPtrList,
                         const ElementType& IndependentElement)
{
    auto MatrixNumber = int64(MatrixPtrList.size());

    auto CoefNumber = int64(CoefList.size());

    if (MatrixNumber != CoefNumber)
    {
        mdkError << "MatrixNumber != CoefNumber @ mdkLinearAlgebra_SparseMatrix MatrixLinearCombine(OutputMatrix, CoefList, MatrixList)" << '\n';
        return false;
    }

    if (MatrixNumber == 0)
    {
        mdkWarning << "MatrixList is empty @ mdkLinearAlgebra_SparseMatrix MatrixLinearCombine(OutputMatrix, CoefList, MatrixList)" << '\n';
        return false;
    }

    auto Size = MatrixPtrList[0]->GetSize();

    for (int64 k = 1; k < int64(MatrixPtrList.size()); ++k)
    {
        if (Size.RowNumber != MatrixPtrList[k]->GetRowNumber() || Size.ColNumber != MatrixPtrList[k]->GetColNumber())
        {
            mdkError << "Size is not the same in MatrixPtrList @ mdkLinearAlgebra_SparseMatrix MatrixLinearCombine(OutputMatrix, CoefList, MatrixList)" << '\n';

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
            mdkError << "OutputMatrix Size does not match @ mdkLinearAlgebra_SparseMatrix MatrixLinearCombine(OutputMatrix, AlphaList, MatrixList)" << '\n';
            return false;
        }
    }

    //--------------------------------------------------------------------------------

   
    return true;
}

//======================================================================================================================================================//



}//end namespace mdk

#endif