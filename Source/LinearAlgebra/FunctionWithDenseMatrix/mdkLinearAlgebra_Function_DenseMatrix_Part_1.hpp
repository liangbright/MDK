#pragma once

namespace mdk
{

//=========================================================================================================================================//
//------------ MatrixAdd, MatrixSubtract, MatrixMultiply, MatrixElementMultiply, MatrixElementDivide ---------------------------------------//
//=========================================================================================================================================//


//------------------- OutputMatrixC = MatrixA operator MatrixB --------------------------------------------//

template<typename ElementType>
inline 
DenseMatrix<ElementType> MatrixAdd(const DenseMatrix<ElementType>& MatrixA, const DenseMatrix<ElementType>& MatrixB)
{
    DenseMatrix<ElementType> tempMatrix;
    MatrixAdd(tempMatrix, MatrixA, MatrixB, CheckInput);
    return  tempMatrix;
}


template<typename ElementType>
inline
bool MatrixAdd(DenseMatrix<ElementType>& OutputMatrixC, const DenseMatrix<ElementType>& MatrixA, const DenseMatrix<ElementType>& MatrixB)
{
    auto SizeA = MatrixA.GetSize();
    auto SizeB = MatrixB.GetSize();

    if (SizeA.RowCount == 0 || SizeB.RowCount == 0)
    {
        MDK_Error("MatrixA or MatrixB is empty @ mdkLinearAlgebra_DenseMatrix MatrixAdd(OutputMatrixC, MatrixA, MatrixB)")
        return false;
    }

    if (SizeA.ColCount == 1 && SizeA.RowCount == 1)
    {
        return MatrixAdd(OutputMatrixC, MatrixA(0), MatrixB);
    }

    if (SizeB.ColCount == 1 && SizeB.RowCount == 1)
    {
        return MatrixAdd(OutputMatrixC, MatrixA, MatrixB(0));
    }

    if (SizeA.RowCount != SizeB.RowCount || SizeA.ColCount != SizeB.ColCount)
    {
        MDK_Error("MatrixA Size does not match MatrixB Size @ mdkLinearAlgebra_DenseMatrix MatrixAdd(OutputMatrixC, MatrixA, MatrixB)")
        return false;
    }

	OutputMatrixC.FastResize(SizeA.RowCount, SizeA.ColCount);

    auto ptrC = OutputMatrixC.GetElementPointer();
    auto ptrA = MatrixA.GetElementPointer();
    auto ptrB = MatrixB.GetElementPointer();
    auto ElementCount = SizeA.RowCount*SizeA.ColCount;

    return MatrixAdd(ptrC, ptrA, ptrB, ElementCount, false);
}


template<typename ElementType>
inline 
bool MatrixAdd(ElementType* OutputMatrixC, const ElementType* MatrixA, const ElementType* MatrixB, int_max ElementCount, bool CheckInput)
{
    if (CheckInput == true)
    {
        if (OutputMatrixC == nullptr || MatrixA == nullptr || MatrixB == nullptr || ElementCount <= 0)
        {
            MDK_Error("Invlaid input @ mdkLinearAlgebra_DenseMatrix MatrixAdd(OutputMatrixC, MatrixA, MatrixB)")
            return false;
        }
    }

	if (ElementCount < 128)
	{
		for (int_max i = 0; i < ElementCount; ++i)
		{
			OutputMatrixC[i] = MatrixA[i] + MatrixB[i];
		}
	}
	else
	{
		//call Eigen
		typedef Eigen::Matrix<ElementType, Eigen::Dynamic, Eigen::Dynamic> EigenMatrixType;
		Eigen::Map<EigenMatrixType> A(const_cast<ElementType*>(MatrixA), ElementCount, 1);
		Eigen::Map<EigenMatrixType> B(const_cast<ElementType*>(MatrixB), ElementCount, 1);
		Eigen::Map<EigenMatrixType> C(OutputMatrixC, ElementCount, 1);
		C = A + B;
	}

    return true;
}


template<typename ElementType>
inline 
DenseMatrix<ElementType> MatrixSubtract(const DenseMatrix<ElementType>& MatrixA, const DenseMatrix<ElementType>& MatrixB)
{
    DenseMatrix<ElementType> tempMatrix;
    MatrixSubtract(tempMatrix, MatrixA, MatrixB);
    return  tempMatrix;
}


template<typename ElementType>
inline
bool MatrixSubtract(DenseMatrix<ElementType>& OutputMatrixC, const DenseMatrix<ElementType>& MatrixA, const DenseMatrix<ElementType>& MatrixB)
{
    auto SizeA = MatrixA.GetSize();
    auto SizeB = MatrixB.GetSize();

    if (SizeA.RowCount == 0 || SizeA.ColCount == 0)
    {
        MDK_Error("MatrixA or MatrixB is empty @ mdkLinearAlgebra_DenseMatrix MatrixSubtract(OutputMatrixC, MatrixA, MatrixB)")
        return false;
    }

    if (SizeA.ColCount == 1 && SizeA.RowCount == 1)
    {
        return MatrixSubtract(OutputMatrixC, MatrixA(0), MatrixB);
    }

    if (SizeB.ColCount == 1 && SizeB.RowCount == 1)
    {
        return MatrixSubtract(OutputMatrixC, MatrixA, MatrixB(0));
    }

    if (SizeA.RowCount != SizeB.RowCount || SizeA.ColCount != SizeB.ColCount)
    {
        MDK_Error("MatrixA Size does not match MatrixB Size @ mdkLinearAlgebra_DenseMatrix MatrixSubtract(OutputMatrixC, MatrixA, MatrixB)")
        return false;
    }

	OutputMatrixC.FastResize(SizeA.RowCount, SizeA.ColCount);

    auto ptrC = OutputMatrixC.GetElementPointer();
    auto ptrA = MatrixA.GetElementPointer();
    auto ptrB = MatrixB.GetElementPointer();
    auto ElementCount = SizeA.RowCount*SizeA.ColCount;

    return MatrixSubtract(ptrC, ptrA, ptrB, ElementCount, false);
}


template<typename ElementType>
inline bool MatrixSubtract(ElementType* OutputMatrixC, const ElementType* MatrixA, const ElementType* MatrixB, int_max ElementCount, bool CheckInput)
{
    if (CheckInput == true)
    {
        if (OutputMatrixC == nullptr || MatrixA == nullptr || MatrixB == nullptr || ElementCount <= 0)
        {
            MDK_Error("Invalid input @ mdkLinearAlgebra_DenseMatrix MatrixSubtract(OutputMatrixC, MatrixA, MatrixB)")
            return false;
        }
    }

	if (ElementCount < 128)
	{
		for (int_max i = 0; i < ElementCount; ++i)
		{
			OutputMatrixC[i] = MatrixA[i] - MatrixB[i];
		}
	}
	else
	{
		//call Eigen
		typedef Eigen::Matrix<ElementType, Eigen::Dynamic, Eigen::Dynamic> EigenMatrixType;
		Eigen::Map<EigenMatrixType> A(const_cast<ElementType*>(MatrixA), ElementCount, 1);
		Eigen::Map<EigenMatrixType> B(const_cast<ElementType*>(MatrixB), ElementCount, 1);
		Eigen::Map<EigenMatrixType> C(OutputMatrixC, ElementCount, 1);
		C = A - B;
	}

    return true;
}



template<typename ElementType>
inline 
DenseMatrix<ElementType> MatrixMultiply(const DenseMatrix<ElementType>& MatrixA, const DenseMatrix<ElementType>& MatrixB)
{
    DenseMatrix<ElementType> tempMatrix;
    MatrixMultiply(tempMatrix, MatrixA, MatrixB);
    return  tempMatrix;
}


template<typename ElementType>
inline
bool MatrixMultiply_slow(DenseMatrix<ElementType>& OutputMatrixC, const DenseMatrix<ElementType>& MatrixA, const DenseMatrix<ElementType>& MatrixB)
{
    auto SizeA = MatrixA.GetSize();
    auto SizeB = MatrixB.GetSize();

    if (SizeA.RowCount == 0 || SizeB.RowCount == 0)
    {
        MDK_Error("MatrixA or MatrixB is empty @ mdkLinearAlgebra_DenseMatrix MatrixMultiply(OutputMatrixC, MatrixA, MatrixB)")
        return false;
    }

    if (SizeA.ColCount == 1 && SizeA.RowCount == 1)
    {
        return MatrixMultiply(OutputMatrixC, MatrixA(0), MatrixB);
    }

    if (SizeB.ColCount == 1 && SizeB.RowCount == 1)
    {
        return MatrixMultiply(OutputMatrixC, MatrixA, MatrixB(0));
    }

    if (SizeA.ColCount != SizeB.RowCount)
    {
        MDK_Error("MatrixA Size does not match MatrixB Size @ mdkLinearAlgebra_DenseMatrix MatrixMultiply(OutputMatrixC, MatrixA, MatrixB)")
        return false;
    }

    auto SizeC = OutputMatrixC.GetSize();

    if (SizeC.RowCount > 0)
    {
        auto ptrA = MatrixA.GetElementPointer();
        auto ptrB = MatrixB.GetElementPointer();
        auto ptrC = OutputMatrixC.GetElementPointer();

        if (ptrC == ptrA || ptrC == ptrB)
        {
            // OutputMatrixC is MatrixA or MatrixB
            // create a temp matrix and call this function again

            DenseMatrix<ElementType> tempMatrix;
            MatrixMultiply(tempMatrix, MatrixA, MatrixB);
            OutputMatrixC.Copy(std::move(tempMatrix));// do NOT use OutputMatrixC = std::move(tempMatrix)
            return true;
        }
    }

	OutputMatrixC.FastResize(SizeA.RowCount, SizeB.ColCount);

    auto C_begin = OutputMatrixC.GetElementPointer();
    auto A_begin = MatrixA.GetElementPointer();
    auto B_begin = MatrixB.GetElementPointer();

    // C : M x K ,   A: M x N,   B: N x K

    int_max M = SizeA.RowCount;
    int_max N = SizeA.ColCount;
    int_max K = SizeB.ColCount;

    bool TheFirstMethod = true; // the second method is very slow

    if (TheFirstMethod == true)
    {        
        for (auto pC = C_begin; pC < C_begin + M*K; ++pC)
        {
            pC[0] = ElementType(0);
        }

        auto pA = A_begin;
        auto pB = B_begin;
        auto pC_Col_k_begin = C_begin;

        for (int_max k = 0; k < K; ++k)
        {
            for (int_max n = 0; n < N; ++n)
            {
                //--------------------------------------------
                //for (int_max m = 0; m < M; ++m)
                //{
                //    C(m, k) += A(m, n)*B(n, k);
                //}
                //----------------------------------------------                
                auto pA_prev = pA;                
                for (auto pC = pC_Col_k_begin; pA < pA_prev + M; ++pA, ++pC)
                {                
                    pC[0] += pA[0] * pB[0];
                }                
                //----------------------------------------------
                pB+=1;
            }
            pA = A_begin;
            pC_Col_k_begin += M;
        }

        return true;
    }
    else
    {
        auto pC = C_begin;
        auto pA = A_begin;
        auto pB = B_begin;

        int_max IndexB = 0;

        for (int_max k = 0; k < K; ++k)
        {
            for (int_max m = 0; m < M; ++m)
            {
                pC[0] = ElementType(0);

                int_max IndexA = 0;

                for (int_max n = 0; n < N; ++n)
                {
                    // C(m, k) += A(m, n)*B(n, k);

                    pC[0] += pA[IndexA + m] * pB[IndexB + n];
                    IndexA += M;
                }
                pC+=1;
            }
            IndexB += N;
        }

        return true;
    }
}


template<typename ElementType>
inline
bool MatrixMultiply(DenseMatrix<ElementType>& OutputMatrixC, const DenseMatrix<ElementType>& MatrixA, const DenseMatrix<ElementType>& MatrixB)
{
    auto SizeA = MatrixA.GetSize();
    auto SizeB = MatrixB.GetSize();

    if (SizeA.RowCount == 0 || SizeB.RowCount == 0)
    {
        MDK_Error("MatrixA or MatrixB is empty @ mdkLinearAlgebra_DenseMatrix MatrixMultiply(OutputMatrixC, MatrixA, MatrixB)")
        return false;
    }

    if (SizeA.ColCount == 1 && SizeA.RowCount == 1)
    {
        return MatrixMultiply(OutputMatrixC, MatrixA(0), MatrixB);
    }

    if (SizeB.ColCount == 1 && SizeB.RowCount == 1)
    {
        return MatrixMultiply(OutputMatrixC, MatrixA, MatrixB(0));
    }

    if (SizeA.ColCount != SizeB.RowCount)
    {
        MDK_Error("MatrixA Size does not match MatrixB Size @ mdkLinearAlgebra_DenseMatrix MatrixMultiply(OutputMatrixC, MatrixA, MatrixB)")
        return false;
    }

    auto SizeC = OutputMatrixC.GetSize();

    if (SizeC.RowCount > 0)
    {
        auto ptrA = MatrixA.GetElementPointer();
        auto ptrB = MatrixB.GetElementPointer();
        auto ptrC = OutputMatrixC.GetElementPointer();

        if (ptrC == ptrA || ptrC == ptrB)
        {
            // OutputMatrixC is MatrixA or MatrixB
            // create a temp matrix and call this function again  
            DenseMatrix<ElementType> tempMatrix;
            MatrixMultiply(tempMatrix, MatrixA, MatrixB);
            OutputMatrixC.Copy(std::move(tempMatrix));// do NOT use OutputMatrixC = std::move(tempMatrix)
            return true;
        }
    }

	OutputMatrixC.FastResize(SizeA.RowCount, SizeB.ColCount);

    // get non-const pointer
    auto ptrA = const_cast<ElementType*>(MatrixA.GetElementPointer());
    auto ptrB = const_cast<ElementType*>(MatrixB.GetElementPointer());
    auto ptrC = OutputMatrixC.GetElementPointer();

    //call armadillo
    //arma::Mat<ElementType> A(ptrA, arma::uword(MatrixA.GetRowCount()), arma::uword(MatrixA.GetColCount()), false);
    //arma::Mat<ElementType> B(ptrB, arma::uword(MatrixB.GetRowCount()), arma::uword(MatrixB.GetColCount()), false);
    //arma::Mat<ElementType> C(ptrC, arma::uword(OutputMatrixC.GetRowCount()), arma::uword(OutputMatrixC.GetColCount()), false);
    //C = A*B;

	//call Eigen
	Eigen::Map<const Eigen::Matrix<ElementType, Eigen::Dynamic, Eigen::Dynamic>> A(ptrA, MatrixA.GetRowCount(), MatrixA.GetColCount());
	Eigen::Map<const Eigen::Matrix<ElementType, Eigen::Dynamic, Eigen::Dynamic>> B(ptrB, MatrixB.GetRowCount(), MatrixB.GetColCount());
	Eigen::Map<Eigen::Matrix<ElementType, Eigen::Dynamic, Eigen::Dynamic>> C(ptrC, OutputMatrixC.GetRowCount(), OutputMatrixC.GetColCount());
	C = A*B;

    return true;   
}


template<typename ElementType>
inline 
DenseMatrix<ElementType> MatrixElementMultiply(const DenseMatrix<ElementType>& MatrixA, const DenseMatrix<ElementType>& MatrixB)
{
    DenseMatrix<ElementType> tempMatrix;
    MatrixElementMultiply(tempMatrix, MatrixA, MatrixB);
    return  tempMatrix;
}


template<typename ElementType>
inline 
bool MatrixElementMultiply(DenseMatrix<ElementType>& OutputMatrixC, const DenseMatrix<ElementType>& MatrixA, const DenseMatrix<ElementType>& MatrixB)
{
    auto SizeA = MatrixA.GetSize();
    auto SizeB = MatrixB.GetSize();

    if (SizeA.RowCount == 0 || SizeA.ColCount == 0)
    {
        MDK_Error("MatrixA or MatrixB is empty @ mdkLinearAlgebra_DenseMatrix MatrixElementMultiply(OutputMatrixC, MatrixA, MatrixB)")
        return false;
    }

    if (SizeA.ColCount == 1 && SizeA.RowCount == 1)
    {
        return MatrixElementMultiply(OutputMatrixC, MatrixA(0), MatrixB);
    }

    if (SizeB.ColCount == 1 && SizeB.RowCount == 1)
    {
        return MatrixElementMultiply(OutputMatrixC, MatrixA, MatrixB(0));
    }

    if (SizeA.RowCount != SizeB.RowCount || SizeA.ColCount != SizeB.ColCount)
    {
        MDK_Error("MatrixA Size does not match MatrixB Size @ mdkLinearAlgebra_DenseMatrix MatrixElementMultiply(OutputMatrixC, MatrixA, MatrixB)")
        return false;
    }
    
	OutputMatrixC.FastResize(SizeA.RowCount, SizeB.ColCount);

    auto ptrC = OutputMatrixC.GetElementPointer();
    auto ptrA = MatrixA.GetElementPointer();
    auto ptrB = MatrixB.GetElementPointer();
    auto ElementCount = SizeA.RowCount*SizeA.ColCount;

    return MatrixElementMultiply(ptrC, ptrA, ptrB, ElementCount, false);
}


template<typename ElementType>
inline 
bool MatrixElementMultiply(ElementType* OutputMatrixC, const ElementType* MatrixA, const ElementType* MatrixB, int_max ElementCount, bool CheckInput)
{
    if (CheckInput == true)
    {
        if (OutputMatrixC == nullptr || MatrixA == nullptr || MatrixB == nullptr || ElementCount <= 0)
        {
            MDK_Error("Invalid input @ mdkLinearAlgebra_DenseMatrix MatrixElementMultiply(OutputMatrixC, MatrixA, MatrixB)")
            return false;
        }
    }

    for (int_max i = 0; i < ElementCount; ++i)
    {
        OutputMatrixC[i] = MatrixA[i] * MatrixB[i];
    }

    return true;
}


template<typename ElementType>
inline 
DenseMatrix<ElementType> MatrixElementDivide(const DenseMatrix<ElementType>& MatrixA, const DenseMatrix<ElementType>& MatrixB)
{
    DenseMatrix<ElementType> tempMatrix;
    MatrixElementDivide(tempMatrix, MatrixA, MatrixB);
    return  tempMatrix;
}


template<typename ElementType>
inline 
bool MatrixElementDivide(DenseMatrix<ElementType>& OutputMatrixC, const DenseMatrix<ElementType>& MatrixA, const DenseMatrix<ElementType>& MatrixB)
{
    auto SizeA = MatrixA.GetSize();
    auto SizeB = MatrixB.GetSize();

    if (SizeA.RowCount == 0 || SizeA.ColCount == 0)
    {
        MDK_Error("MatrixA or MatrixB is empty @ mdkLinearAlgebra_DenseMatrix MatrixElementDivide(OutputMatrixC, MatrixA, MatrixB)")
        return false;
    }

    if (SizeA.ColCount == 1 && SizeA.RowCount == 1)
    {
        return MatrixElementDivide(OutputMatrixC, MatrixA(0), MatrixB);
    }

    if (SizeB.ColCount == 1 && SizeB.RowCount == 1)
    {
        return MatrixElementDivide(OutputMatrixC, MatrixA, MatrixB(0));
    }

    if (SizeA.RowCount != SizeB.RowCount || SizeA.ColCount != SizeB.ColCount)
    {
        MDK_Error("MatrixA Size does not match MatrixB Size @ mdkLinearAlgebra_DenseMatrix MatrixElementDivide(OutputMatrixC, MatrixA, MatrixB)")
        return false;
    }

	OutputMatrixC.FastResize(SizeA.RowCount, SizeA.ColCount);

    auto ptrC = OutputMatrixC.GetElementPointer();
    auto ptrA = MatrixA.GetElementPointer();
    auto ptrB = MatrixB.GetElementPointer();
    auto ElementCount = SizeA.RowCount*SizeA.ColCount;

    return MatrixElementDivide(ptrC, ptrA, ptrB, ElementCount, false);
}


template<typename ElementType>
inline 
bool MatrixElementDivide(ElementType* OutputMatrixC, const ElementType* MatrixA, const ElementType* MatrixB, int_max ElementCount, bool CheckInput)
{
    if (CheckInput == true)
    {
        if (OutputMatrixC == nullptr || MatrixA == nullptr || MatrixB == nullptr || ElementCount <= 0)
        {
            MDK_Error("Invalid input @ mdkLinearAlgebra_DenseMatrix MatrixElementDivide(OutputMatrixC, MatrixA, MatrixB)")
            return false;
        }
    }

    for (int_max i = 0; i < ElementCount; ++i)
    {
        OutputMatrixC[i] = MatrixA[i] / MatrixB[i];
    }

    return true;
}

//------------------- OutputMatrixC =  ElementA operator MatrixB --------------------------------------------//

template<typename ElementType>
inline 
DenseMatrix<ElementType> MatrixAdd(const ElementType& ElementA, const DenseMatrix<ElementType>& MatrixB)
{
    DenseMatrix<ElementType> tempMatrix;
    MatrixAdd(tempMatrix, ElementA, MatrixB);
    return tempMatrix;
}


template<typename ElementType>
inline
bool MatrixAdd(DenseMatrix<ElementType>& OutputMatrixC, const ElementType& ElementA, const DenseMatrix<ElementType>& MatrixB)
{
    auto SizeB = MatrixB.GetSize();

    if (SizeB.RowCount == 0)
    {
        MDK_Error("MatrixB is empty @ mdkLinearAlgebra_DenseMatrix MatrixAdd(OutputMatrixC, ElementA, MatrixB)")
        return false;
    }

	OutputMatrixC.FastResize(SizeB.RowCount, SizeB.ColCount);

    auto ptrC = OutputMatrixC.GetElementPointer();
    auto ptrB = MatrixB.GetElementPointer();
    auto ElementCount = SizeB.RowCount*SizeB.ColCount;

    if (ptrC == ptrB) // in place
    {
        for (auto tempPtr = ptrC; tempPtr < ptrC + ElementCount; ++tempPtr)
        {
            tempPtr[0] += ElementA;
        }
    }
    else
    {
        for (int_max i = 0; i < ElementCount; ++i)
        {
            ptrC[i] = ElementA + ptrB[i];
        }
    }

    return true;
}


template<typename ElementType>
inline
DenseMatrix<ElementType> MatrixSubtract(const ElementType& ElementA, const DenseMatrix<ElementType>& MatrixB)
{
    DenseMatrix<ElementType> tempMatrix;
    MatrixSubtract(tempMatrix, ElementA, MatrixB);
    return tempMatrix;
}


template<typename ElementType>
inline
bool MatrixSubtract(DenseMatrix<ElementType>& OutputMatrixC, const ElementType& ElementA, const DenseMatrix<ElementType>& MatrixB)
{
    auto SizeB = MatrixB.GetSize();

    if (SizeB.RowCount == 0)
    {
        MDK_Error("MatrixB is empty @ mdkLinearAlgebra_DenseMatrix MatrixSubtract(OutputMatrixC, ElementA, MatrixB)")
        return false;
    }

	OutputMatrixC.FastResize(SizeB.RowCount, SizeB.ColCount);

    auto ptrC = OutputMatrixC.GetElementPointer();
    auto ptrB = MatrixB.GetElementPointer();
    auto ElementCount = SizeB.RowCount*SizeB.ColCount;

    if (ptrC == ptrB) // in place
    {
        for (auto tempPtr = ptrC; tempPtr < ptrC + ElementCount; ++tempPtr)
        {
            tempPtr[0] -= ElementA;
        }
    }
    else
    {
        for (int_max i = 0; i < ElementCount; ++i)
        {
            ptrC[i] = ElementA - ptrB[i];
        }
    }

    return true;
}


template<typename ElementType>
inline 
DenseMatrix<ElementType> MatrixMultiply(const ElementType& ElementA, DenseMatrix<ElementType>& MatrixB)
{
    DenseMatrix<ElementType> tempMatrix;
    MatrixMultiply(tempMatrix, ElementA, MatrixB);
    return tempMatrix;
}


template<typename ElementType>
inline
bool MatrixMultiply(DenseMatrix<ElementType>& OutputMatrixC, const ElementType& ElementA, const DenseMatrix<ElementType>& MatrixB)
{
    auto SizeB = MatrixB.GetSize();

    if (SizeB.RowCount == 0)
    {
        MDK_Error("MatrixB is empty @ mdkLinearAlgebra_DenseMatrix MatrixMultiply(OutputMatrixC, ElementA, MatrixB)")
        return false;
    }

	OutputMatrixC.FastResize(SizeB.RowCount, SizeB.ColCount);

    auto ptrC = OutputMatrixC.GetElementPointer();
    auto ptrB = MatrixB.GetElementPointer();
    auto ElementCount = SizeB.RowCount*SizeB.ColCount;

    if (ptrC == ptrB) // in place
    {
        for (auto tempPtr = ptrC; tempPtr < ptrC + ElementCount; ++tempPtr)
        {
            tempPtr[0] *= ElementA;
        }
    }
    else
    {
        for (int_max i = 0; i < ElementCount; ++i)
        {
            ptrC[i] = ElementA * ptrB[i];
        }
    }

    return true;
}


template<typename ElementType>
inline 
DenseMatrix<ElementType> MatrixElementMultiply(const ElementType& ElementA, const DenseMatrix<ElementType>& MatrixB)
{
    DenseMatrix<ElementType> tempMatrix;
    MatrixElementMultiply(tempMatrix, ElementA, MatrixB);
    return tempMatrix;
}


template<typename ElementType>
inline 
bool MatrixElementMultiply(DenseMatrix<ElementType>& OutputMatrixC, const ElementType& ElementA, const DenseMatrix<ElementType>& MatrixB)
{
    return MatrixMultiply(OutputMatrixC, ElementA, MatrixB);
}


template<typename ElementType>
inline 
DenseMatrix<ElementType> MatrixElementDivide(const ElementType& ElementA, const DenseMatrix<ElementType>& MatrixB)
{
    DenseMatrix<ElementType> tempMatrix;
    MatrixElementDivide(tempMatrix, ElementA, MatrixB);
    return tempMatrix;
}


template<typename ElementType>
inline 
bool MatrixElementDivide(DenseMatrix<ElementType>& OutputMatrixC, const ElementType& ElementA, const DenseMatrix<ElementType>& MatrixB)
{
    auto SizeB = MatrixB.GetSize();

    if (SizeB.RowCount == 0)
    {
        MDK_Error("MatrixB is empty @ mdkLinearAlgebra_DenseMatrix MatrixElementDivide(OutputMatrixC, ElementA, MatrixB)")
        return false;
    }

	OutputMatrixC.FastResize(SizeB.RowCount, SizeB.ColCount);

    auto ptrC = OutputMatrixC.GetElementPointer();
    auto ptrB = MatrixB.GetElementPointer();
    auto ElementCount = SizeB.RowCount*SizeB.ColCount;

    if (ptrC == ptrB) // in place
    {
        for (auto tempPtr = ptrC; tempPtr < ptrC + ElementCount; ++tempPtr)
        {
            tempPtr[0] = ElementA / tempPtr[0];
        }
    }
    else
    {
        for (int_max i = 0; i < ElementCount; ++i)
        {
            ptrC[i] = ElementA / ptrB[i];
        }
    }

    return true;
}

//-----------------------------------OutputMatrixC = MatrixA operator ElementB ----------------------------------------------------//

template<typename ElementType>
inline
DenseMatrix<ElementType> MatrixAdd(const DenseMatrix<ElementType>& MatrixA, const ElementType& ElementB)
{
    DenseMatrix<ElementType> tempMatrix;
    MatrixAdd(tempMatrix, MatrixA, ElementB);
    return tempMatrix;
}


template<typename ElementType>
inline
bool MatrixAdd(DenseMatrix<ElementType>& OutputMatrixC, const DenseMatrix<ElementType>& MatrixA, const ElementType& ElementB)
{
    auto SizeA = MatrixA.GetSize();

    if (SizeA.RowCount == 0)
    {
        MDK_Error("MatrixA is empty @ mdkLinearAlgebra_DenseMatrix MatrixAdd(OutputMatrixC, MatrixA, ElementB)")
        return false;
    }

	OutputMatrixC.FastResize(SizeA.RowCount, SizeA.ColCount);

    auto ptrC = OutputMatrixC.GetElementPointer();
    auto ptrA = MatrixA.GetElementPointer();
    auto ElementCount = SizeA.RowCount*SizeA.ColCount;

    if (ptrC == ptrA) // in place
    {
        for (auto tempPtr = ptrC; tempPtr < ptrC + ElementCount; ++tempPtr)
        {
            tempPtr[0] += ElementB;
        }
    }
    else
    {
        for (int_max i = 0; i < ElementCount; ++i)
        {
            ptrC[i] = ptrA[i] + ElementB;
        }
    }

    return true;
}


template<typename ElementType>
inline
DenseMatrix<ElementType> MatrixSubtract(const DenseMatrix<ElementType>& MatrixA, const ElementType& ElementB)
{
    DenseMatrix<ElementType> tempMatrix;
    MatrixSubtract(tempMatrix, MatrixA, ElementB);
    return tempMatrix;
}


template<typename ElementType>
inline
bool MatrixSubtract(DenseMatrix<ElementType>& OutputMatrixC, const DenseMatrix<ElementType>& MatrixA, const ElementType& ElementB)
{
    auto SizeA = MatrixA.GetSize();

    if (SizeA.RowCount == 0)
    {
        MDK_Error("MatrixA is empty @ mdkLinearAlgebra_DenseMatrix MatrixSubtract(OutputMatrixC, MatrixA, ElementB)")
        return false;
    }

	OutputMatrixC.FastResize(SizeA.RowCount, SizeA.ColCount);

    auto ptrC = OutputMatrixC.GetElementPointer();
    auto ptrA = MatrixA.GetElementPointer();
    auto ElementCount = SizeA.RowCount*SizeA.ColCount;

    if (ptrC == ptrA) // in place
    {
        for (auto tempPtr = ptrC; tempPtr < ptrC + ElementCount; ++tempPtr)
        {
            tempPtr[0] -= ElementB;
        }
    }
    else
    {
        for (int_max i = 0; i < ElementCount; ++i)
        {
            ptrC[i] = ptrA[i] - ElementB;
        }
    }

    return true;
}


template<typename ElementType>
inline
DenseMatrix<ElementType> MatrixMultiply(const DenseMatrix<ElementType>& MatrixA, const ElementType& ElementB)
{
    DenseMatrix<ElementType> tempMatrix;
    MatrixMultiply(tempMatrix, MatrixA, ElementB);
    return tempMatrix;
}


template<typename ElementType>
inline
bool MatrixMultiply(DenseMatrix<ElementType>& OutputMatrixC, const DenseMatrix<ElementType>& MatrixA, const ElementType& ElementB)
{
    auto SizeA = MatrixA.GetSize();

    if (SizeA.RowCount == 0)
    {
        MDK_Error("MatrixA is empty @ mdkLinearAlgebra_DenseMatrix MatrixMultiply(OutputMatrixC, MatrixA, ElementB)")
        return false;
    }

	OutputMatrixC.FastResize(SizeA.RowCount, SizeA.ColCount);

    //---------------------------------------------------------------------------------------------------------------------------------------

    auto ptrC = OutputMatrixC.GetElementPointer();
    auto ptrA = MatrixA.GetElementPointer();
    auto ElementCount = SizeA.RowCount*SizeA.ColCount;

    if (ptrC == ptrA) // in place
    {
        for (auto tempPtr = ptrC; tempPtr < ptrC + ElementCount; ++tempPtr)
        {
            tempPtr[0] *= ElementB;
        }
    }
    else
    {
        for (int_max i = 0; i < ElementCount; ++i)
        {
            ptrC[i] = ptrA[i] * ElementB;
        }
    }
   
    return true;
}


template<typename ElementType>
inline
DenseMatrix<ElementType> MatrixElementMultiply(const DenseMatrix<ElementType>& MatrixA, const ElementType& ElementB)
{
    DenseMatrix<ElementType> tempMatrix;
    MatrixElementMultiply(tempMatrix, MatrixA, ElementB);
    return tempMatrix;
}


template<typename ElementType>
inline
bool MatrixElementMultiply(DenseMatrix<ElementType>& OutputMatrixC, const DenseMatrix<ElementType>& MatrixA, const ElementType& ElementB)
{
    return MatrixMultiply(OutputMatrixC, MatrixA, ElementB);
}


template<typename ElementType>
inline
DenseMatrix<ElementType> MatrixElementDivide(const DenseMatrix<ElementType>& MatrixA, const ElementType& ElementB)
{
    DenseMatrix<ElementType> tempMatrix;
    MatrixElementDivide(tempMatrix, MatrixA, ElementB);
    return tempMatrix;
}


template<typename ElementType>
inline
bool MatrixElementDivide(DenseMatrix<ElementType>& OutputMatrixC, const DenseMatrix<ElementType>& MatrixA, const ElementType& ElementB)
{
    auto SizeA = MatrixA.GetSize();

    if (SizeA.RowCount == 0)
    {
        MDK_Error("MatrixA is empty @ mdkLinearAlgebra_DenseMatrix MatrixElementDivide(OutputMatrixC, MatrixA, ElementB)")
        return false;
    }

	OutputMatrixC.FastResize(SizeA.RowCount, SizeA.ColCount);

    auto ptrC = OutputMatrixC.GetElementPointer();
    auto ptrA = MatrixA.GetElementPointer();
    auto ElementCount = SizeA.RowCount*SizeA.ColCount;

    if (ptrC == ptrA) // in place
    {
        for (auto tempPtr = ptrC; tempPtr < ptrC + ElementCount; ++tempPtr)
        {
            tempPtr[0] /= ElementB;
        }
    }
    else
    {
        for (int_max i = 0; i < ElementCount; ++i)
        {
            ptrC[i] = ptrA[i] / ElementB;
        }
    }

    return true;
}

//=========================================================================================================================================//
//------------------------------------------ MatrixElementOperation ----------------------------------------------------------------------//
//=========================================================================================================================================//

// ------------------------ MatrixElementOperation on InputMatrix

template<typename ElementType>
inline 
DenseMatrix<ElementType> MatrixElementNamedOperation(const std::string& OperationName, const DenseMatrix<ElementType>& InputMatrix)
{
    DenseMatrix<ElementType> OutputMatrix;
    MatrixElementNamedOperation(OutputMatrix, OperationName, InputMatrix);
    return OutputMatrix;
}


template<typename ElementType>
inline 
bool MatrixElementNamedOperation(DenseMatrix<ElementType>& OutputMatrix, const std::string& OperationName, const DenseMatrix<ElementType>& InputMatrix)
{
    // note: must use  MatrixElementOperation<ElementType>, not MatrixElementOperation
    // otherwise compiler error (vs2013)

    if (OperationName == "abs")
    {
        return MatrixElementOperation<ElementType>(OutputMatrix, [](const ElementType& a){return std::abs(a); }, InputMatrix);
    }
    else if (OperationName == "exp")
    {
        return  MatrixElementOperation<ElementType>(OutputMatrix, [](const ElementType& a){return std::exp(a); }, InputMatrix);
    }
    else if (OperationName == "log")
    {
        return  MatrixElementOperation<ElementType>(OutputMatrix, [](const ElementType& a){return std::log(a); }, InputMatrix);
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
        MDK_Error("unknown Operation @ mdkLinearAlgebra_DenseMatrix MatrixElementNamedOperation(OutputMatrix, OperationName, InputMatrix)")
        return false;
    }
}


template<typename ElementType, typename OperationType>
inline 
DenseMatrix<ElementType> MatrixElementOperation(OperationType Operation, const DenseMatrix<ElementType>& InputMatrix)
{
	DenseMatrix<ElementType> OutputMatrix(InputMatrix.GetRowCount(), InputMatrix.GetColCount());
    MatrixElementOperation(OutputMatrix, Operation, InputMatrix);
    return OutputMatrix;
}


template<typename ElementType, typename OperationType>
inline 
bool MatrixElementOperation(DenseMatrix<ElementType>& OutputMatrix, OperationType Operation, const DenseMatrix<ElementType>& InputMatrix)
{
    auto InputSize = InputMatrix.GetSize();

    if (InputSize.RowCount == 0)
    {
        MDK_Error("InputMatrix is empty @ mdkLinearAlgebra_DenseMatrix MatrixElementOperation(OutputMatrix, Operation, InputMatrix)")
        return false;
    }

	OutputMatrix.FastResize(InputSize.RowCount, InputSize.ColCount);

    auto ptrOutput = OutputMatrix.GetElementPointer();
    auto ptrInput = InputMatrix.GetElementPointer();
    auto ElementCount = InputSize.ColCount * InputSize.RowCount;

    if (ptrOutput == ptrInput) // in place operation
    {
        for (auto Ptr = ptrOutput; Ptr < ptrOutput + ElementCount; ++Ptr)
        {
            Ptr[0] = Operation(Ptr[0]);
        }
    }
    else
    {
        for (int_max i = 0; i < ElementCount; ++i)
        {
            ptrOutput[i] = Operation(ptrInput[i]);
        }
    }

    return true;
}


// ------------------------ MatrixElementOperation on InputMatrixA with InputMatrixB

template<typename ElementType>
inline 
DenseMatrix<ElementType> MatrixElementNamedOperation(const char OperationName,
                                                     const DenseMatrix<ElementType>& InputMatrixA,
                                                     const DenseMatrix<ElementType>& InputMatrixB)
{                                              
    DenseMatrix<ElementType> OutputMatrix;
    MatrixElementNamedOperation(OutputMatrix, OperationName, InputMatrixA, InputMatrixB);
    return OutputMatrix;
}


template<typename ElementType>
inline 
bool MatrixElementNamedOperation(DenseMatrix<ElementType>& OutputMatrixC,
                                 const char OperationName,
                                 const DenseMatrix<ElementType>& InputMatrixA,
                                 const DenseMatrix<ElementType>& InputMatrixB)
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
        MDK_Error("unknown Operation @ MatrixElementNamedOperation(OutputMatrixC, OperationName, InputMatrixA, InputMatrixB)")
        return false;
    }

}


template<typename ElementType>
inline 
DenseMatrix<ElementType> MatrixElementNamedOperation(const std::string& OperationName,
                                                     const DenseMatrix<ElementType>& InputMatrixA,
                                                     const DenseMatrix<ElementType>& InputMatrixB)
{                                              
    DenseMatrix<ElementType> OutputMatrix;
    MatrixElementNamedOperation(OutputMatrix, OperationName, InputMatrixA, InputMatrixB);
    return OutputMatrix;
}


template<typename ElementType>
inline 
bool MatrixElementNamedOperation(DenseMatrix<ElementType>& OutputMatrixC,
                                 const std::string& OperationName,
                                 const DenseMatrix<ElementType>& InputMatrixA,
                                 const DenseMatrix<ElementType>& InputMatrixB)
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
        MDK_Error("unknown Operation @ MatrixElementNamedOperation(OutputMatrixC, OperationName, InputMatrixA, InputMatrixB)")
        return false;
    }

}


template<typename ElementType, typename OperationType>
inline 
DenseMatrix<ElementType> MatrixElementOperation(OperationType Operation,
                                                const DenseMatrix<ElementType>& InputMatrixA,
                                                const DenseMatrix<ElementType>& InputMatrixB)
{
    DenseMatrix<ElementType> OutputMatrix;
    MatrixElementOperation<ElementType>(OutputMatrix, Operation, InputMatrixA, InputMatrixB);
    return OutputMatrix;
}


template<typename ElementType, typename OperationType>
inline 
bool MatrixElementOperation(DenseMatrix<ElementType>& OutputMatrixC,
                            OperationType Operation,
                            const DenseMatrix<ElementType>& InputMatrixA,
                            const DenseMatrix<ElementType>& InputMatrixB)
{
    auto SizeA = InputMatrixA.GetSize();
    auto SizeB = InputMatrixB.GetSize();

    if (SizeA.RowCount == 0 || SizeB.RowCount ==0)
    {
        MDK_Error("InputMatrixA or InputMatrixB is empty @ mdkLinearAlgebra_DenseMatrix MatrixElementOperation(OutputMatrix, Operation, InputMatrixA, InputMatrixB)")

        return false;
    }

    if (SizeB.RowCount == 1 && SizeB.ColCount == 1)
    {
        return MatrixElementOperation(OutputMatrixC, Operation, InputMatrixA, InputMatrixB(0));
    }

    auto Flag_row = 0;
    auto Flag_col = 0;
    auto Flag_full = 0;

    if (SizeB.ColCount == SizeA.ColCount && SizeB.RowCount == SizeA.RowCount)
    {
        Flag_full = 1;
    }
    else if (SizeB.ColCount == 1 && SizeB.RowCount == SizeA.RowCount)
    {
        Flag_col = 1;
    }
    else if (SizeB.ColCount == SizeA.ColCount && SizeB.RowCount == 1)
    {
        Flag_row = 1;
    }
    else
    {
        MDK_Error("Size does not match @ mdkLinearAlgebra_DenseMatrix MatrixElementOperation(OutputMatrix, Operation, InputMatrixA, InputMatrixB)")
        return false;
    }

	OutputMatrixC.FastResize(SizeA.RowCount, SizeA.ColCount);

    auto ptrC = OutputMatrixC.GetElementPointer();
    auto ptrA = InputMatrixA.GetElementPointer();
    auto ptrB = InputMatrixB.GetElementPointer();

    if (Flag_full == 1)
    {
        auto ElementCount = SizeA.RowCount * SizeA.ColCount;

        for (int_max i = 0; i < ElementCount; ++i)
        {
            ptrC[i] = Operation(ptrA[i], ptrB[i]);
        }
    }
    else if (Flag_col == 1)
    {
        for (int_max j = 0; j < SizeA.ColCount; ++j)
        {
            for (int_max i = 0; i < SizeA.RowCount; ++i)
            {
                ptrC[i] = Operation(ptrA[i], ptrB[i]);
            }
        }
    }
    else if (Flag_row == 1)
    {
        for (int_max i = 0; i < SizeA.RowCount; ++i)
        {
            int_max tempIndex = 0;

            for (int_max j = 0; j < SizeA.ColCount; ++j)
            {
                ptrC[tempIndex + i] = Operation(ptrA[tempIndex + i], ptrB[i]);

                tempIndex += SizeA.RowCount;
            }
        }
    }

    return true;
}

// ------------------------ MatrixElementOperation on InputMatrixA with InputElementB

template<typename ElementType>
inline 
DenseMatrix<ElementType> MatrixElementNamedOperation(const char OperationName,
                                                     const DenseMatrix<ElementType>& InputMatrixA,
                                                     const ElementType& InputElementB)
{
    DenseMatrix<ElementType> OutputMatrix;
    MatrixElementNamedOperation(OutputMatrix, OperationName, InputMatrixA, InputElementB);
    return OutputMatrix;
}


template<typename ElementType>
inline 
bool MatrixElementNamedOperation(DenseMatrix<ElementType>& OutputMatrixC,
                                 const char OperationName,
                                 const DenseMatrix<ElementType>& InputMatrixA,
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
        MDK_Error("unknown operator @ mdkLinearAlgebra_DenseMatrix MatrixElementNamedOperation(OutputMatrixC, OperationName, InputMatrixA, InputElementB)")
        return false;
    }
}


template<typename ElementType>
inline 
DenseMatrix<ElementType> MatrixElementNamedOperation(const std::string& OperationName,
                                                     const DenseMatrix<ElementType>& InputMatrixA,
                                                     const ElementType& InputElementB)
{
    DenseMatrix<ElementType> OutputMatrix;
    MatrixElementNamedOperation(OutputMatrix, OperationName, InputMatrixA, InputElementB);
    return OutputMatrix;
}


template<typename ElementType>
inline 
bool MatrixElementNamedOperation(DenseMatrix<ElementType>& OutputMatrixC,
                                 const std::string& OperationName,
                                 const DenseMatrix<ElementType>& InputMatrixA,
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
        MDK_Error("unknown operator @ mdkLinearAlgebra_DenseMatrix MatrixElementOperation(OutputMatrixC, OperationName, InputMatrixA, InputElementB)")
        return false;
    }
}


template<typename ElementType, typename OperationType>
inline 
DenseMatrix<ElementType> MatrixElementOperation(OperationType Operation,
                                                const DenseMatrix<ElementType>& InputMatrixA,
                                                const ElementType& InputElementB)
{
    DenseMatrix<ElementType> OutputMatrix;
    MatrixElementOperation<ElementType>(OutputMatrix, Operation, InputMatrixA, InputElementB);
    return OutputMatrix;
}



template<typename ElementType, typename OperationType>
inline
bool MatrixElementOperation(DenseMatrix<ElementType>& OutputMatrixC, 
                            OperationType Operation,
                            const DenseMatrix<ElementType>& InputMatrixA,
                            const ElementType& InputElementB)
{
    auto SizeA = InputMatrixA.GetSize();

    if (SizeA.RowCount == 0)
    {
        MDK_Error("InputMatrixA is empty @ MatrixElementOperation(OutputMatrixC, Operation, InputMatrixA, InputElementB)")

        return false;
    }

	OutputMatrixC.FastResize(SizeA.RowCount, SizeA.ColCount);

    auto ptrC = OutputMatrixC.GetElementPointer();
    auto ptrA = InputMatrixA.GetElementPointer();
    auto ElementCount = SizeA.RowCount * SizeA.ColCount;

    if (ptrC == ptrA) // in place MatrixElementOperation
    {
        for (auto Ptr = ptrC; Ptr < ptrC + ElementCount; ++Ptr)
        {
            Ptr[0] = Operation(Ptr[0], InputElementB);
        }
    }
    else
    {
        for (int_max i = 0; i < ElementCount; ++i)
        {
            ptrC[i] = Operation(ptrA[i], InputElementB);
        }
    }

    return true;
}

//---------------------- convenient function ----------------------------------//

template<typename ElementType>
inline
DenseMatrix<ElementType> MatrixAbs(const DenseMatrix<ElementType>& InputMatrix)
{
    return MatrixElementNamedOperation("abs", InputMatrix);
}


template<typename ElementType>
inline
DenseMatrix<ElementType> MatrixExp(const DenseMatrix<ElementType>& InputMatrix)
{
    return MatrixElementNamedOperation("exp", InputMatrix);
}


template<typename ElementType>
inline
DenseMatrix<ElementType> MatrixLog(const DenseMatrix<ElementType>& InputMatrix)
{
    return MatrixElementNamedOperation("log", InputMatrix);
}


template<typename ElementType>
inline
DenseMatrix<ElementType> MatrixSqrt(const DenseMatrix<ElementType>& InputMatrix)
{
    return MatrixElementNamedOperation("sqrt", InputMatrix);
}


//=========================================================================================================================================//
//------------------------------------------ MatrixColOperation ----------------------------------------------------------------------//
//==========================================================================================================================================//

// ------------------------ MatrixColOperation on InputMatrix at InputColIndex

template<typename ElementType>
inline 
DenseMatrix<ElementType> 
MatrixColNamedOperation(const std::string& OperationName, const DenseMatrix<ElementType>& InputMatrix, int_max InputColIndex, const bool Enable_BoundCheck)
{
    DenseMatrix<ElementType> OutputMatrix(InputMatrix.GetRowCount(), 1);
    MatrixColNamedOperation(OutputMatrix, 0, OperationName, InputMatrix, InputColIndex, Enable_BoundCheck);
    return OutputMatrix;
}


template<typename ElementType>
inline
bool 
MatrixColNamedOperation(DenseMatrix<ElementType>& OutputMatrix, int_max OutputColIndex,
                        const std::string& OperationName, 
                        const DenseMatrix<ElementType>& InputMatrix, int_max InputColIndex,
                        const bool Enable_BoundCheck)
{
    if (OperationName == "abs")
    {
        return MatrixColOperation<ElementType>(OutputMatrix, OutputColIndex, [](const ElementType& a){return std::abs(a); }, InputMatrix, InputColIndex, Enable_BoundCheck);
    }
	else if (OperationName == "exp")
	{
		return  MatrixColOperation<ElementType>(OutputMatrix, OutputColIndex, [](const ElementType& a){return std::exp(a); }, InputMatrix, InputColIndex, Enable_BoundCheck);
	}
	else if (OperationName == "log")
	{
		return  MatrixColOperation<ElementType>(OutputMatrix, OutputColIndex, [](const ElementType& a){return std::log(a); }, InputMatrix, InputColIndex, Enable_BoundCheck);
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
        MDK_Error("unknown Operation @ mdkLinearAlgebra_DenseMatrix MatrixColNamedOperation(OutputMatrix, OutputColIndex, OperationName, InputMatrix, InputColIndex, Enable_BoundCheck)")
        return false;
    }
}


template<typename ElementType, typename OperationType>
inline 
DenseMatrix<ElementType>
MatrixColOperation(OperationType Operation,
                   const DenseMatrix<ElementType>& InputMatrix, int_max InputColIndex,
                   const bool Enable_BoundCheck)
{
    DenseMatrix<ElementType> OutputMatrix(InputMatrix.GetRowCount(), 1);
    MatrixColOperation(OutputMatrix, 0, Operation, InputMatrix, InputColIndex, Enable_BoundCheck);
    return OutputMatrix;
}


template<typename ElementType, typename OperationType>
inline 
bool 
MatrixColOperation(DenseMatrix<ElementType>& OutputMatrix, int_max OutputColIndex,
                   OperationType Operation,
                   const DenseMatrix<ElementType>& InputMatrix, int_max InputColIndex, 
                   const bool Enable_BoundCheck)
{
    if (Enable_BoundCheck)
    {
        auto InputSize = InputMatrix.GetSize();

        if (InputSize.RowCount == 0)
        {
            MDK_Error("InputMatrix is empty @ mdkLinearAlgebra_DenseMatrix MatrixColOperation(OutputMatrix, OutputColIndex, Operation, InputMatrix, InputColIndex, Enable_BoundCheck)")

            return false;
        }

        auto OutputSize = OutputMatrix.GetSize();

        if (InputSize.RowCount != OutputSize.RowCount || InputColIndex >= InputSize.ColCount || OutputColIndex >= OutputSize.ColCount)
        {
            MDK_Error("Size does not match @ mdkLinearAlgebra_DenseMatrix MatrixColOperation(OutputMatrix, OutputColIndex, Operation, InputMatrix, InputColIndex, Enable_BoundCheck)")
            return false;
        }
    }

    //-------------------------------------------------------------------

    auto ptrOutput = OutputMatrix.GetElementPointer();

    auto ptrInput = InputMatrix.GetElementPointer();

    auto RowCount = OutputMatrix.GetRowCount();

    if (ptrOutput == ptrInput && OutputColIndex == InputColIndex) // in place operation
    {
        auto Offset = OutputColIndex*RowCount;

        for (auto Ptr = ptrOutput + Offset; Ptr < ptrOutput + Offset + RowCount; ++Ptr)
        {
            Ptr[0] = Operation(Ptr[0]);
        }
    }
    else
    {
        auto Offset_output = OutputColIndex*RowCount;

        auto Offset_input = InputColIndex*RowCount;

        for (int_max i = 0; i < RowCount; ++i)
        {
            ptrOutput[i + Offset_output] = Operation(ptrInput[i + Offset_input]);
        }
    }

    return true;
}

// ------------------------ MatrixColOperation on InputMatrixA at InputColIndexA with InputMatrixB

template<typename ElementType>
inline 
DenseMatrix<ElementType> 
MatrixColNamedOperation(const char OperationName,
                        const DenseMatrix<ElementType>& InputMatrixA, int_max InputColIndexA,
                        const DenseMatrix<ElementType>& InputMatrixB,
                        const bool Enable_BoundCheck)
{
    DenseMatrix<ElementType> OutputMatrix(InputMatrixA.GetRowCount(), 1);
    MatrixColNamedOperation(OutputMatrix, 0, OperationName, InputMatrixA, InputColIndexA, InputMatrixB, Enable_BoundCheck);
    return OutputMatrix;
}


template<typename ElementType>
inline
bool
MatrixColNamedOperation(DenseMatrix<ElementType>& OutputMatrixC, int_max OutputColIndexC,
                        const char OperationName,
                        const DenseMatrix<ElementType>& InputMatrixA, int_max InputColIndexA,
                        const DenseMatrix<ElementType>& InputMatrixB,
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
        MDK_Error("unknown Operation @ mdkLinearAlgebra_DenseMatrix MatrixColNamedOperation(OutputMatrixC, OutputColIndexC, OperationName, InputMatrixA, InputColIndexA, InputMatrixB, Enable_BoundCheck)")
        return false;
    }
}


template<typename ElementType>
inline 
DenseMatrix<ElementType> 
MatrixColNamedOperation(const std::string& OperationName,
                        const DenseMatrix<ElementType>& InputMatrixA, int_max InputColIndexA,
                        const DenseMatrix<ElementType>& InputMatrixB,
                        const bool Enable_BoundCheck)
{
    DenseMatrix<ElementType> OutputMatrix(InputMatrixA.GetRowCount(), 1);
    MatrixColNamedOperation(OutputMatrix, 0, OperationName, InputMatrixA, InputColIndexA, InputMatrixB, Enable_BoundCheck);
    return OutputMatrix;
}


template<typename ElementType>
inline
bool
MatrixColNamedOperation(DenseMatrix<ElementType>& OutputMatrixC, int_max OutputColIndexC,
                        const std::string& OperationName,
                        const DenseMatrix<ElementType>& InputMatrixA, int_max InputColIndexA,
                        const DenseMatrix<ElementType>& InputMatrixB,
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
        MDK_Error("unknown Operation @ mdkLinearAlgebra_DenseMatrix MatrixColNamedOperation(OutputMatrixC, OutputColIndexC, OperationName, InputMatrixA, InputColIndexA, InputMatrixB, Enable_BoundCheck)")
        return false;
    }
}


template<typename ElementType, typename OperationType>
inline 
DenseMatrix<ElementType>
MatrixColOperation(OperationType Operation,
                   const DenseMatrix<ElementType>& InputMatrixA, int_max InputColIndexA,
                   const DenseMatrix<ElementType>& InputMatrixB,
                   const bool Enable_BoundCheck)
{
    DenseMatrix<ElementType> OutputMatrix(InputMatrixA.GetRowCount(), 1);
    MatrixColOperation(OutputMatrix, 0, Operation, InputMatrixA, InputColIndexA, InputMatrixB, Enable_BoundCheck);
    return OutputMatrix;
}


template<typename ElementType, typename OperationType>
inline 
bool
MatrixColOperation(DenseMatrix<ElementType>& OutputMatrixC, int_max OutputColIndexC,
                   OperationType Operation,
                   const DenseMatrix<ElementType>& InputMatrixA, int_max InputColIndexA,
                   const DenseMatrix<ElementType>& InputMatrixB,
                   const bool Enable_BoundCheck)
{
    if (InputMatrixB.GetRowCount() == 1 && InputMatrixB.GetColCount() == 1)
    {
        return MatrixColOperation(OutputMatrixC, OutputColIndexC, Operation, InputMatrixA, InputColIndexA, InputMatrixB(0), Enable_BoundCheck);
    }

    //-------------------------------------------------------------------

    if (Enable_BoundCheck)
    {
        auto SizeA = InputMatrixA.GetSize();
        auto SizeB = InputMatrixB.GetSize();
        auto SizeC = OutputMatrixC.GetSize();

        if (SizeA.RowCount == 0 || SizeB.RowCount == 0)
        {
            MDK_Error("InputMatrixA or InputMatrixB is empty @ mdkLinearAlgebra_DenseMatrix MatrixColOperation(OutputMatrixC, OutputColIndexC, Operation, InputMatrixA, InputColIndexA, InputMatrixB, Enable_BoundCheck)")

            return false;
        }

        if (OutputColIndexC >= SizeC.ColCount || SizeC.RowCount != SizeA.RowCount || InputColIndexA >= SizeA.ColCount)
        {
            MDK_Error("Size does not match-a @ mdkLinearAlgebra_DenseMatrix MatrixColOperation(OutputMatrixC, OutputColIndexC, Operation, InputMatrixA, InputColIndexA, InputMatrixB, Enable_BoundCheck)")
            return false;
        }

        if (SizeB.ColCount == 1 && SizeB.RowCount == SizeA.RowCount)
        {
        }
        else if (SizeB.RowCount == 1 && SizeB.ColCount == SizeA.RowCount)
        {
        }
        else
        {
            MDK_Error("Size does not match-b @ mdkLinearAlgebra_DenseMatrix MatrixColOperation(OutputMatrixC, OutputColIndexC, Operation, InputMatrixA, InputColIndexA, InputMatrixB, Enable_BoundCheck)")
            return false;
        }
    }

    //-------------------------------------------------------------------

    auto ptrC = OutputMatrixC.GetElementPointer();
    auto ptrA = InputMatrixA.GetElementPointer();
    auto ptrB = InputMatrixB.GetElementPointer();
    auto RowCount = OutputMatrix.GetRowCount();

    if (ptrC == ptrA && OutputColIndex == InputColIndex) // in place operation
    {
        auto Offset = OutputColIndexC * RowCount

        ptrC += Offset;

        for (int_max i = 0; i < RowCount; ++i)
        {
            ptrC[0] = Operation(ptrC[0], ptrB[i]);
            ++ptrC;
        }
    }
    else
    {
        auto OffsetC = OutputColIndexC * RowCount;
        auto OffsetA = InputColIndexA * RowCount;

        for (int_max i = 0; i < RowCount; ++i)
        {
            ptrC[i + OffsetC] = Operation(ptrA[i + OffsetA], ptrB[i]);
        }
    }
    
    return true;
}

// ------------------------ MatrixColOperation on InputMatrixA at InputColIndexA with InputElementB

template<typename ElementType>
inline 
DenseMatrix<ElementType> 
MatrixColNamedOperation(const char OperationName,
                        const DenseMatrix<ElementType>& InputMatrixA, int_max InputColIndexA,
                        const ElementType& InputElementB,
                        const bool Enable_BoundCheck)
{
    DenseMatrix<ElementType> OutputMatrix(InputMatrixA.GetRowCount(), 1);
    MatrixColNamedOperation(OutputMatrix, 0, OperationName, InputMatrixA, InputColIndexA, InputElementB, Enable_BoundCheck);
    return OutputMatrix;
}


template<typename ElementType>
inline 
bool
MatrixColNamedOperation(DenseMatrix<ElementType>& OutputMatrixC, int_max OutputColIndexC,
                        const char OperationName,
                        const DenseMatrix<ElementType>& InputMatrixA, int_max InputColIndexA,
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
        MDK_Error("unknown operator @ mdkLinearAlgebra_DenseMatrix MatrixColOperation(OutputMatrixC, OutputColIndexC, OperationName, InputMatrixA, InputColIndexA, InputElementB)")
        return false;
    }
}


template<typename ElementType>
inline
DenseMatrix<ElementType> 
MatrixColNamedOperation(const std::string& OperationName,
                        const DenseMatrix<ElementType>& InputMatrixA, int_max InputColIndexA,
                        const ElementType& InputElementB,
                        const bool Enable_BoundCheck)
{
    DenseMatrix<ElementType> OutputMatrix(InputMatrixA.GetRowCount(), 1);
    MatrixColNamedOperation(OutputMatrix, 0, OperationName, InputMatrixA, InputColIndexA, InputElementB, Enable_BoundCheck);
    return OutputMatrix;
}


template<typename ElementType>
inline 
bool
MatrixColNamedOperation(DenseMatrix<ElementType>& OutputMatrixC, int_max OutputColIndexC,
                        const std::string& OperationName,
                        const DenseMatrix<ElementType>& InputMatrixA, int_max InputColIndexA,
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
        MDK_Error(" unknown operator @ mdkLinearAlgebra_DenseMatrix MatrixColNamedOperation(OutputMatrixC, OutputColIndexC, OperationName, InputMatrixA, InputColIndexA, InputElementB)")
        return false;
    }
}


template<typename ElementType, typename OperationType>
inline 
DenseMatrix<ElementType> 
MatrixColOperation(OperationType Operation,
                   const DenseMatrix<ElementType>& InputMatrixA, int_max InputColIndexA,
                   const ElementType& InputElementB,
                   const bool Enable_BoundCheck)
{
    DenseMatrix<ElementType> OutputMatrix(InputMatrixA.GetRowCount(), 1);
    MatrixColOperation(OutputMatrix, 0, Operation, InputMatrixA, InputColIndexA, InputElementB, Enable_BoundCheck);
    return OutputMatrix;
}


template<typename ElementType, typename OperationType>
inline
bool
MatrixColOperation(DenseMatrix<ElementType>& OutputMatrixC, int_max OutputColIndexC,
                   OperationType Operation,
                   const DenseMatrix<ElementType>& InputMatrixA, int_max InputColIndexA,
                   const ElementType& InputElementB,
                   const bool Enable_BoundCheck)
{
    if (Enable_BoundCheck)
    {
        auto SizeA = InputMatrixA.GetSize();
        auto SizeC = OutputMatrixC.GetSize();

        if (SizeA.RowCount == 0)
        {
            MDK_Error("InputMatrixA is empty @ mdkLinearAlgebra_DenseMatrix MatrixColOperation(OutputMatrixC, OutputColIndexC, Operation, InputMatrixA, InputColIndexA, InputElementB, Enable_BoundCheck)")

            return false;
        }

        if (OutputColIndexC >= SizeC.ColCount || SizeC.RowCount != SizeA.RowCount || InputColIndexA >= SizeA.ColCount)
        {
            MDK_Error("Size does not match-a @ mdkLinearAlgebra_DenseMatrix MatrixColOperation(OutputMatrixC, OutputColIndexC, Operation, InputMatrixA, InputColIndexA, InputElementB, Enable_BoundCheck)")
            return false;
        }
    }

    //-------------------------------------------------------------------

    auto ptrC = OutputMatrixC.GetElementPointer();
    auto ptrA = InputMatrixA.GetElementPointer();
    auto RowCount = OutputMatrixC.GetRowCount();

    if (ptrC == ptrA && OutputColIndexC == InputColIndexA) // in place operation
    {
        auto Offset = OutputColIndexC * RowCount;

        ptrC += Offset;

        for (auto tempPtr = ptrC; tempPtr < ptrC + RowCount; ++tempPtr)
        {
            tempPtr[0] = Operation(tempPtr[0], InputElementB);
        }
    }
    else
    {
        auto Offset_C = OutputColIndexC * RowCount;
        auto Offset_A = InputColIndexA * RowCount;

        for (int_max i = 0; i < RowCount; ++i)
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
DenseMatrix<ElementType>
MatrixRowNamedOperation(const std::string& OperationName,
                        const DenseMatrix<ElementType>& InputMatrix, int_max InputRowIndex,
                        const bool Enable_BoundCheck)
{
    DenseMatrix<ElementType> OutputMatrix(1, InputMatrix.GetColCount());
    MatrixRowNamedOperation(OutputMatrix, 0, OperationName, InputMatrix, InputRowIndex, Enable_BoundCheck);
    return OutputMatrix;
}


template<typename ElementType>
inline 
bool
MatrixRowNamedOperation(DenseMatrix<ElementType>& OutputMatrix, int_max OutputRowIndex,
                        const std::string& OperationName,
                        const DenseMatrix<ElementType>& InputMatrix, int_max InputRowIndex,
                        const bool Enable_BoundCheck)
{
    if (OperationName == "abs")
    {
		return MatrixRowOperation<ElementType>(OutputMatrix, OutputRowIndex, [](const ElementType& a){return std::abs(a); }, InputMatrix, InputRowIndex, Enable_BoundCheck);
    }
	else if (OperationName == "exp")
	{
		return  MatrixRowOperation<ElementType>(OutputMatrix, OutputRowIndex, [](const ElementType& a){return std::exp(a); }, InputMatrix, InputRowIndex, Enable_BoundCheck);
	}
	else if (OperationName == "log")
	{
		return  MatrixRowOperation<ElementType>(OutputMatrix, OutputRowIndex, [](const ElementType& a){return std::log(a); }, InputMatrix, InputRowIndex, Enable_BoundCheck);
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
        MDK_Error("unknown Operation @ mdkLinearAlgebra_DenseMatrix MatrixRowNamedOperation(OutputMatrix, OutputRowIndex, OperationName, InputMatrix, InputRowIndex, Enable_BoundCheck)")
        return false;
    }
}


template<typename ElementType, typename OperationType>
inline
DenseMatrix<ElementType> 
MatrixRowOperation(OperationType Operation,
                   const DenseMatrix<ElementType>& InputMatrix, int_max InputRowIndex,
                   const bool Enable_BoundCheck)
{
    DenseMatrix<ElementType> OutputMatrix(1, InputMatrix.GetColCount());
    MatrixRowOperation(OutputMatrix, 0, Operation, InputMatrix, InputRowIndex, Enable_BoundCheck);
    return OutputMatrix;
}


template<typename ElementType, typename OperationType>
inline
bool
MatrixRowOperation(DenseMatrix<ElementType>& OutputMatrix, int_max OutputRowIndex,
                   OperationType Operation,
                   const DenseMatrix<ElementType>& InputMatrix, int_max InputRowIndex,
                   const bool Enable_BoundCheck)
{
    if (Enable_BoundCheck)
    {
        auto InputSize = InputMatrix.GetSize();

        if (InputSize.RowCount == 0)
        {
            MDK_Error("InputMatrix is empty @ mdkLinearAlgebra_DenseMatrix MatrixRowOperation(OutputMatrix, OutputRowIndex, Operation, InputMatrix, InputRowIndex, Enable_BoundCheck)")

            return false;
        }

        auto OutputSize = OutputMatrix.GetSize();

        if (InputSize.ColCount != OutputSize.ColCount || InputRowIndex >= InputSize.RowCount || OutputRowIndex >= OutputSize.RowCount)
        {
            MDK_Error("Size does not match @ mdkLinearAlgebra_DenseMatrix MatrixRowOperation(OutputMatrix, OutputRowIndex, Operation, InputMatrix, InputRowIndex, Enable_BoundCheck)")
            return false;
        }
    }

    //-------------------------------------------------------------------

    auto ptrOutput = OutputMatrix.GetElementPointer();

    auto ptrInput = InputMatrix.GetElementPointer();

    auto ColCount = OutputMatrix.GetColCount();

    if (ptrOutput == ptrInput) // the same matrix
    {       
        auto RowCount = OutputMatrix.GetRowCount();

        if (OutputRowIndex == InputRowIndex) // in place operation
        {
            for (int_max j = 0; j < ColCount; ++j)
            {
                auto temp = j*RowCount + OutputRowIndex;

                ptrOutput[temp] = Operation(ptrOutput[temp]);
            }
        }
        else
        {
            for (int_max j = 0; j < ColCount; ++j)
            {
                auto temp = j*RowCount;

                ptrOutput[temp + OutputRowIndex] = Operation(ptrInput[temp + InputRowIndex]);
            }
        }
    }
    else
    {
        auto RowCount_output = OutputMatrix.GetRowCount();

        auto RowCount_input = InputMatrix.GetRowCount();

        for (int_max j = 0; j < ColCount; ++j)
        {
            ptrOutput[j*RowCount_output + OutputRowIndex] = Operation(ptrInput[j*RowCount_input + InputRowIndex]);
        }
    }

    return true;
}

// ------------------------ MatrixRowOperation on InputMatrixA at InputRowIndexA with InputMatrixB

template<typename ElementType>
inline 
DenseMatrix<ElementType> 
MatrixRowNamedOperation(const char OperationName,
                        const DenseMatrix<ElementType>& InputMatrixA, int_max InputRowIndexA,
                        const DenseMatrix<ElementType>& InputMatrixB,
                        const bool Enable_BoundCheck)
{
    DenseMatrix<ElementType> OutputMatrixC(1, InputMatrixA.GetColCount());
    MatrixRowNamedOperation(OutputMatrixC, 0, OperationName, InputMatrixA, InputRowIndexA, InputMatrixB, Enable_BoundCheck);
    return OutputMatrixC;
}


template<typename ElementType>
inline 
bool
MatrixRowNamedOperation(DenseMatrix<ElementType>& OutputMatrixC, int_max OutputRowIndexC,
                        const char OperationName,
                        const DenseMatrix<ElementType>& InputMatrixA, int_max InputRowIndexA,
                        const DenseMatrix<ElementType>& InputMatrixB,
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
        MDK_Error("unknown Operation @ mdkLinearAlgebra_DenseMatrix MatrixRowNamedOperation(OutputMatrixC, OutputRowIndexC, OperationName, InputMatrixA, InputRowIndexA, InputMatrixB, Enable_BoundCheck)")
        return false;
    }
}


template<typename ElementType>
inline 
DenseMatrix<ElementType> 
MatrixRowNamedOperation(const std::string& OperationName,
                        const DenseMatrix<ElementType>& InputMatrixA, int_max InputRowIndexA,
                        const DenseMatrix<ElementType>& InputMatrixB,
                        const bool Enable_BoundCheck)
{
    DenseMatrix<ElementType> OutputMatrixC(1, InputMatrixA.GetColCount());
    MatrixRowNamedOperation(OutputMatrixC, 0, OperationName, InputMatrixA, InputRowIndexA, InputMatrixB, Enable_BoundCheck);
    return OutputMatrixC;
}


template<typename ElementType>
inline 
bool
MatrixRowNamedOperation(DenseMatrix<ElementType>& OutputMatrixC, int_max OutputRowIndexC,
                        const std::string& OperationName,
                        const DenseMatrix<ElementType>& InputMatrixA, int_max InputRowIndexA,
                        const DenseMatrix<ElementType>& InputMatrixB,
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
        MDK_Error("unknown Operation @ mdkLinearAlgebra_DenseMatrix MatrixRowOperation(OutputMatrixC, OutputRowIndexC, OperationName, InputMatrixA, InputRowIndexA, InputMatrixB, Enable_BoundCheck)")
        return false;
    }
}


template<typename ElementType, typename OperationType>
inline 
DenseMatrix<ElementType> 
MatrixRowOperation(OperationType Operation,
                   const DenseMatrix<ElementType>& InputMatrixA, int_max InputRowIndexA,
                   const DenseMatrix<ElementType>& InputMatrixB,
                   const bool Enable_BoundCheck)
{
    DenseMatrix<ElementType> OutputMatrixC(1, InputMatrixA.GetColCount());
    MatrixRowOperation(OutputMatrixC, 0, InputMatrixA, Operation, InputRowIndexA, InputMatrixB, Enable_BoundCheck);
    return OutputMatrixC;
}


template<typename ElementType, typename OperationType>
inline 
bool
MatrixRowOperation(DenseMatrix<ElementType>& OutputMatrixC, int_max OutputRowIndexC,
                   OperationType Operation,
                   const DenseMatrix<ElementType>& InputMatrixA, int_max InputRowIndexA,
                   const DenseMatrix<ElementType>& InputMatrixB,
                   const bool Enable_BoundCheck)
{
    if (InputMatrixB.GetRowCount() == 1 && InputMatrixB.GetColCount() == 1)
    {
        return MatrixRowOperation(OutputMatrixC, OutputRowIndexC, Operation, InputMatrixA, InputRowIndexA, InputMatrixB(0), Enable_BoundCheck);
    }

    //-------------------------------------------------------------------

    if (Enable_BoundCheck)
    {
        auto SizeA = InputMatrixA.GetSize();
        auto SizeB = InputMatrixB.GetSize();
        auto SizeC = OutputMatrixC.GetSize();

        if (SizeA.RowCount == 0 || SizeB.RowCount == 0)
        {
            MDK_Error("InputMatrixA or InputMatrixB is empty @ mdkLinearAlgebra_DenseMatrix MatrixRowOperation(OutputMatrixC, OutputRowIndexC, Operation, InputMatrixA, InputRowIndexA, InputMatrixB, Enable_BoundCheck)")

            return false;
        }

        if (OutputRowIndexC >= SizeC.RowCount || SizeC.ColCount != SizeA.ColCount || InputRowIndexA >= SizeA.RowCount)
        {
            MDK_Error("Size does not match-a @ mdkLinearAlgebra_DenseMatrix MatrixRowOperation(OutputMatrixC, OutputRowIndexC, Operation, InputMatrixA, InputRowIndexA, InputMatrixB, Enable_BoundCheck)")
            return false;
        }

        if (SizeB.ColCount == 1 && SizeB.RowCount == SizeA.ColCount)
        {
        }
        else if (SizeB.RowCount == 1 && SizeB.ColCount == SizeA.ColCount)
        {
        }
        else
        {
            MDK_Error("Size does not match-b @ mdkLinearAlgebra_DenseMatrix MatrixRowOperation(OutputMatrixC, OutputRowIndexC, Operation, InputMatrixA, InputRowIndexA, InputMatrixB, Enable_BoundCheck)")

            return false;
        }
    }

    //-------------------------------------------------------------------

    auto ptrC = OutputMatrixC.GetElementPointer();
    auto ptrA = InputMatrixA.GetElementPointer();
    auto ptrB = InputMatrixB.GetElementPointer();

    auto ColCount = OutputMatrixC.GetColCount();

    if (ptrC == ptrA ) // the same matrix
    {
        auto RowCount = OutputMatrixC.GetRowCount();

        if (OutputRowIndexC == InputRowIndexA) // in place operation
        {
            for (int_max j = 0; j < ColCount; ++j)
            {
                auto temp = j*RowCount + OutputRowIndexC;

                ptrC[temp] = Operation(ptrC[temp], ptrB[j]);
            }
        }
        else
        {
            for (int_max j = 0; j < ColCount; ++j)
            {
                auto temp = j*RowCount;

                ptrC[temp + OutputRowIndexC] = Operation(ptrA[temp + InputRowIndexA], ptrB[j]);
            }
        }
    }
    else
    {
        auto RowCountC = OutputMatrixC.GetRowCount();
        auto RowCountA = InputMatrixA.GetRowCount();

        for (int_max j = 0; j < ColCount; ++j)
        {
            ptrC[j*RowCountC + OutputRowIndexC] = Operation(ptrA[j*RowCountA + InputRowIndexA], ptrB[j]);
        }
    }

    return true;
}

// ------------------------ MatrixRowOperation on InputMatrixA at InputColIndexA with InputElementB

template<typename ElementType>
inline 
DenseMatrix<ElementType> 
MatrixRowNamedOperation(const char OperationName,
                        const DenseMatrix<ElementType>& InputMatrixA, int_max InputRowIndexA,
                        const ElementType& InputElementB,
                        const bool Enable_BoundCheck)
{
    DenseMatrix<ElementType> OutputMatrixC(1, InputMatrixA.GetColCount());
    MatrixRowNamedOperation(OutputMatrixC, 0, OperationName, InputMatrixA, InputRowIndexA, InputElementB, Enable_BoundCheck);
    return OutputMatrixC;
}


template<typename ElementType>
inline
bool
MatrixRowNamedOperation(DenseMatrix<ElementType>& OutputMatrixC, int_max OutputRowIndexC,
                        const char OperationName,
                        const DenseMatrix<ElementType>& InputMatrixA, int_max InputRowIndexA,
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
        MDK_Error("unknown Operation @ mdkLinearAlgebra_DenseMatrix MatrixRowNamedOperation(OutputMatrixC, OutputRowIndexC, OperationName, InputMatrixA, InputRowIndexA, InputElementB, Enable_BoundCheck)")
        return false;
    }
}


template<typename ElementType>
inline
DenseMatrix<ElementType>
MatrixRowNamedOperation(const std::string& OperationName,
                        const DenseMatrix<ElementType>& InputMatrixA, int_max InputRowIndexA,
                        const ElementType& InputElementB,
                        const bool Enable_BoundCheck)
{
    DenseMatrix<ElementType> OutputMatrixC(1, InputMatrixA.GetColCount());
    MatrixRowNamedOperation(OutputMatrixC, 0, OperationName, InputMatrixA, InputRowIndexA, InputElementB, Enable_BoundCheck);
    return OutputMatrixC;
}


template<typename ElementType>
inline
bool
MatrixRowNamedOperation(DenseMatrix<ElementType>& OutputMatrixC, int_max OutputRowIndexC,
                        const std::string& OperationName,
                        const DenseMatrix<ElementType>& InputMatrixA, int_max InputRowIndexA,
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
        MDK_Error("unknown Operation @ mdkLinearAlgebra_DenseMatrix MatrixRowNamedOperation(OutputMatrixC, OutputRowIndexC, OperationName, InputMatrixA, InputRowIndexA, InputElementB, Enable_BoundCheck)")
        return false;
    }
}


template<typename ElementType, typename OperationType>
inline 
DenseMatrix<ElementType> 
MatrixRowOperation(OperationType Operation,
                   const DenseMatrix<ElementType>& InputMatrixA, int_max InputRowIndexA,
                   const ElementType& InputElementB,
                   const bool Enable_BoundCheck)
{
    DenseMatrix<ElementType> OutputMatrixC(1, InputMatrixA.GetColCount());
    MatrixRowOperation(OutputMatrixC, 0, OperationName, InputMatrixA, InputRowIndexA, InputElementB, Enable_BoundCheck);
    return OutputMatrixC;
}


template<typename ElementType, typename OperationType>
inline
bool
MatrixRowOperation(DenseMatrix<ElementType>& OutputMatrixC, int_max OutputRowIndexC,
                   OperationType Operation,
                   const DenseMatrix<ElementType>& InputMatrixA, int_max InputRowIndexA,
                   const ElementType& InputElementB,
                   const bool Enable_BoundCheck)
{
    if (Enable_BoundCheck)
    {
        auto SizeA = InputMatrixA.GetSize();
        auto SizeC = OutputMatrixC.GetSize();

        if (SizeA.RowCount == 0)
        {
            MDK_Error("InputMatrixA is empty @ mdkLinearAlgebra_DenseMatrix MatrixRowOperation(OutputMatrixC, OutputRowIndexC, Operation, InputMatrixA, InputRowIndexA, InputElementB, Enable_BoundCheck)")

            return false;
        }

        if (OutputRowIndexC >= SizeC.RowCount || SizeC.ColCount != SizeA.ColCount || InputRowIndexA >= SizeA.RowCount)
        {
            MDK_Error("Size does not match-a @ mdkLinearAlgebra_DenseMatrix MatrixRowOperation(OutputMatrixC, OutputRowIndexC, Operation, InputMatrixA, InputRowIndexA, InputElementB, Enable_BoundCheck)")
            return false;
        }       
    }

    //-------------------------------------------------------------------

    auto ptrC = OutputMatrixC.GetElementPointer();
    auto ptrA = InputMatrixA.GetElementPointer();

    auto ColCount = OutputMatrixC.GetColCount();

    if (ptrC == ptrA) // the same matrix
    {
        auto RowCount = OutputMatrixC.GetRowCount();

        if (OutputRowIndexC == InputRowIndexA) // in place operation
        {
            for (int_max j = 0; j < ColCount; ++j)
            {
                auto temp = j*RowCount + OutputRowIndexC;

                ptrC[temp] = Operation(ptrC[temp], InputElementB);
            }
        }
        else
        {
            for (int_max j = 0; j < ColCount; ++j)
            {
                auto temp = j*RowCount;

                ptrC[temp + OutputRowIndexC] = Operation(ptrA[temp + InputRowIndexA], InputElementB);
            }
        }
    }
    else
    {
        auto RowCountC = OutputMatrixC.GetRowCount();
        auto RowCountA = InputMatrixA.GetRowCount();

        for (int_max j = 0; j < ColCount; ++j)
        {
            ptrC[j*RowCountC + OutputRowIndexC] = Operation(ptrA[j*RowCountA + InputRowIndexA], InputElementB);
        }
    }

    return true;
}


//======================================================================================================================================//
//------------------------------------------ MatrixLinearCombine ----------------------------------------------------------------------//
//======================================================================================================================================//

template<typename ElementType>
inline
DenseMatrix<ElementType> MatrixLinearCombine(const std::vector<ElementType>& CoefList,
									         const std::vector<DenseMatrix<ElementType>>& MatrixList,
									         const ElementType& IndependentElement)
{
	DenseMatrix<ElementType> tempMatrix;
	MatrixLinearCombine(tempMatrix, CoefList, MatrixList, IndependentElement);
	return tempMatrix;
}


template<typename ElementType>
inline 
void MatrixLinearCombine(DenseMatrix<ElementType>& OutputMatrix,
                         const std::vector<ElementType>& CoefList, 
                         const std::vector<DenseMatrix<ElementType>>& MatrixList,
                         const ElementType& IndependentElement)
{
	auto MatrixNumber = int_max(MatrixList.size());

    auto CoefNumber = int_max(CoefList.size());

    if (MatrixNumber != CoefNumber)
    {
        MDK_Error("MatrixNumber != CoefNumber @ mdkLinearAlgebra_DenseMatrix MatrixLinearCombine(...)")
        return;
    }

    if (MatrixNumber == 0)
    {
        MDK_Error("MatrixList is empty @ mdkLinearAlgebra_DenseMatrix MatrixLinearCombine(...)")
        return;
    }

    auto Size = MatrixList[0].GetSize();

    for (int_max k = 1; k < int_max(MatrixList.size()); ++k)
    {
        if (Size.RowCount != MatrixList[k].GetRowCount() || Size.ColCount != MatrixList[k].GetColCount())
        {
            MDK_Error("Size is not the same in MatrixList @ mdkLinearAlgebra_DenseMatrix MatrixLinearCombine(...)")
            return;
        }
    }

    if (Size.RowCount != OutputMatrix.GetRowCount() || Size.ColCount != OutputMatrix.GetColCount())
    {
        if (OutputMatrix.IsSizeFixed() == true)
        {
            MDK_Error("OutputMatrix Size can not change @ mdkLinearAlgebra_DenseMatrix MatrixLinearCombine(...)")
            return;
        }
		OutputMatrix.FastResize(Size.RowCount, Size.ColCount);
    }

	auto OutputMatrixElementPointer = OutputMatrix.GetElementPointer();

	auto ElementCount = Size.ColCount*Size.RowCount;

	std::vector<const ElementType*> MatrixElementPointerList(MatrixNumber);
	for (int_max k = 0; k < MatrixNumber; ++k)
	{
		MatrixElementPointerList[k] = MatrixList[k].GetElementPointer();
	}

	MatrixLinearCombine(OutputMatrixElementPointer, ElementCount, CoefList, MatrixElementPointerList, IndependentElement);
}


template<typename ElementType>
inline
DenseMatrix<ElementType> MatrixLinearCombine(const std::vector<ElementType>& CoefList,
									         const std::vector<const DenseMatrix<ElementType>*>& MatrixList,
									         const ElementType& IndependentElement)
{
	DenseMatrix<ElementType> tempMatrix;
	MatrixLinearCombine(tempMatrix, CoefList, MatrixList, IndependentElement);
	return tempMatrix;
}


template<typename ElementType>
inline 
void MatrixLinearCombine(DenseMatrix<ElementType>& OutputMatrix,
						 const std::vector<ElementType>& CoefList,
						 const std::vector<const DenseMatrix<ElementType>*>& MatrixList,
						 const ElementType& IndependentElement)
{
	auto MatrixNumber = int_max(MatrixList.size());

	auto CoefNumber = int_max(CoefList.size());

	if (MatrixNumber != CoefNumber)
	{
		MDK_Error("MatrixNumber != CoefNumber @ mdkLinearAlgebra_DenseMatrix MatrixLinearCombine(OutputMatrix, CoefList, MatrixList, IndependentElement)")
		return;
	}

	if (MatrixNumber == 0)
	{
		MDK_Error("MatrixList is empty @ mdkLinearAlgebra_DenseMatrix MatrixLinearCombine(OutputMatrix, CoefList, MatrixList, IndependentElement)")
		return;
	}

	auto Size = MatrixList[0]->GetSize();

	for (int_max k = 1; k < int_max(MatrixList.size()); ++k)
	{
		if (Size.RowCount != MatrixList[k]->GetRowCount() || Size.ColCount != MatrixList[k]->GetColCount())
		{
			MDK_Error("Size is not the same in MatrixList @ mdkLinearAlgebra_DenseMatrix MatrixLinearCombine(OutputMatrix, CoefList, MatrixList, IndependentElement)")
			return;
		}
	}

	if (Size.RowCount != OutputMatrix.GetRowCount() || Size.ColCount != OutputMatrix.GetColCount())
	{
		if (OutputMatrix.IsSizeFixed() == true)
		{
			MDK_Error("OutputMatrix Size can not change @ mdkLinearAlgebra_DenseMatrix MatrixLinearCombine(OutputMatrix, AlphaList, MatrixList, IndependentElement)")
			return;
		}
		OutputMatrix.FastResize(Size.RowCount, Size.ColCount);
	}

	auto OutputMatrixElementPointer = OutputMatrix.GetElementPointer();

	auto ElementCount = Size.ColCount*Size.RowCount;

	std::vector<const ElementType*> MatrixElementPointerList(MatrixNumber);
	for (int_max k = 0; k < MatrixNumber; ++k)
	{
		MatrixElementPointerList[k] = MatrixList[k]->GetElementPointer();
	}

	MatrixLinearCombine(OutputMatrixElementPointer, ElementCount, CoefList, MatrixElementPointerList, IndependentElement);
}


template<typename ElementType>
inline 
void MatrixLinearCombine(ElementType* OutputMatrixElementPointer,
						 const int_max ElementCount,
                         const std::vector<ElementType>& CoefList, 
						 const std::vector<const ElementType*>& MatrixElementPointerList,
                         const ElementType& IndependentElement)
{
	auto MatrixNumber = int_max(MatrixElementPointerList.size());
    switch (MatrixNumber)
    {
    case 1:
		MatrixLinearCombine_MatrixNumber_1(OutputMatrixElementPointer, ElementCount, CoefList, MatrixElementPointerList, IndependentElement);
        break;
        
    case 2:
		MatrixLinearCombine_MatrixNumber_2(OutputMatrixElementPointer, ElementCount, CoefList, MatrixElementPointerList, IndependentElement);
        break;

    case 3:
		MatrixLinearCombine_MatrixNumber_3(OutputMatrixElementPointer, ElementCount, CoefList, MatrixElementPointerList, IndependentElement);
        break;

    case 4:
		MatrixLinearCombine_MatrixNumber_4(OutputMatrixElementPointer, ElementCount, CoefList, MatrixElementPointerList, IndependentElement);
        break;

    case 5:
		MatrixLinearCombine_MatrixNumber_5(OutputMatrixElementPointer, ElementCount, CoefList, MatrixElementPointerList, IndependentElement);
        break;

    case 6:       
		MatrixLinearCombine_MatrixNumber_6(OutputMatrixElementPointer, ElementCount, CoefList, MatrixElementPointerList, IndependentElement);
        break;

    case 7:
		MatrixLinearCombine_MatrixNumber_7(OutputMatrixElementPointer, ElementCount, CoefList, MatrixElementPointerList, IndependentElement);
        break;

    case 8:
		MatrixLinearCombine_MatrixNumber_8(OutputMatrixElementPointer, ElementCount, CoefList, MatrixElementPointerList, IndependentElement);
        break;
        
    case 9:
		MatrixLinearCombine_MatrixNumber_9(OutputMatrixElementPointer, ElementCount, CoefList, MatrixElementPointerList, IndependentElement);
        break;

    case 10:
		MatrixLinearCombine_MatrixNumber_10(OutputMatrixElementPointer, ElementCount, CoefList, MatrixElementPointerList, IndependentElement);
        break;
        
    default:
            
        // for-loop #1: faster than #2
        
        auto Coef_0 = CoefList[0];

        auto RawPtr_0 = MatrixElementPointerList[0];

        for (int_max LinearIndex = 0; LinearIndex < ElementCount; ++LinearIndex)
        {
			OutputMatrixElementPointer[LinearIndex] = IndependentElement + Coef_0 * RawPtr_0[LinearIndex];
        }

        for (int_max k = 1; k < MatrixNumber; ++k)
        {
            auto Coef_k = CoefList[k];

            auto RawPtr_k = MatrixElementPointerList[k];

            for (int_max LinearIndex = 0; LinearIndex < ElementCount; ++LinearIndex)
            {
				OutputMatrixElementPointer[LinearIndex] += Coef_k * RawPtr_k[LinearIndex];
            }

        }
        

        // for-loop #2:
        /*
        for (int_max LinearIndex = 0; LinearIndex < ElementCount; ++LinearIndex)
        {
            ElementType tempElement = IndependentElement;

            for (int_max k = 0; k < MatrixNumber; ++k)
            {
                tempElement += CoefRawPtr[k] * MatrixElementPointerList[k][LinearIndex];
            }

            PointerToOutput[LinearIndex] = tempElement;
        }
        */
    }
}


template<typename ElementType>
inline
void MatrixLinearCombine_MatrixNumber_1(ElementType* Output,
						                const int_max ElementCount,
                                        const std::vector<ElementType>& CoefList,  
                                        const std::vector<const ElementType*>& MatrixElementPointerList,
                                        const ElementType& IndependentElement)
{
	if (CoefList[0] != ElementType(1))
	{
		for (int_max LinearIndex = 0; LinearIndex < ElementCount; ++LinearIndex)
		{
			Output[LinearIndex] = IndependentElement + CoefList[0] * MatrixElementPointerList[0][LinearIndex];
		}
	}
	else
	{
		for (int_max LinearIndex = 0; LinearIndex < ElementCount; ++LinearIndex)
		{
			Output[LinearIndex] = IndependentElement + MatrixElementPointerList[0][LinearIndex];
		}
	}
}


template<typename ElementType>
inline
void MatrixLinearCombine_MatrixNumber_2(ElementType* Output,
						                const int_max ElementCount,
                                        const std::vector<ElementType>& CoefList,  
                                        const std::vector<const ElementType*>& MatrixElementPointerList,
                                        const ElementType& IndependentElement)
{
	if (CoefList[0] != ElementType(1) || CoefList[1] != ElementType(1))
	{
		for (int_max LinearIndex = 0; LinearIndex < ElementCount; ++LinearIndex)
		{
			Output[LinearIndex] = IndependentElement
								  + CoefList[0] * MatrixElementPointerList[0][LinearIndex]
				                  + CoefList[1] * MatrixElementPointerList[1][LinearIndex];
		}
	}
	else
	{
		for (int_max LinearIndex = 0; LinearIndex < ElementCount; ++LinearIndex)
		{
			Output[LinearIndex] = IndependentElement + MatrixElementPointerList[0][LinearIndex] + MatrixElementPointerList[1][LinearIndex];
		}
	}
}


template<typename ElementType>
inline
void MatrixLinearCombine_MatrixNumber_3(ElementType* Output,
						                const int_max ElementCount,
                                        const std::vector<ElementType>& CoefList,  
                                        const std::vector<const ElementType*>& MatrixElementPointerList,
                                        const ElementType& IndependentElement)
{
	if (CoefList[0] != ElementType(1) || CoefList[1] != ElementType(1) || CoefList[2] != ElementType(1))
	{
		for (int_max LinearIndex = 0; LinearIndex < ElementCount; ++LinearIndex)
		{
			Output[LinearIndex] = IndependentElement
								  + CoefList[0] * MatrixElementPointerList[0][LinearIndex]
								  + CoefList[1] * MatrixElementPointerList[1][LinearIndex]
								  + CoefList[2] * MatrixElementPointerList[2][LinearIndex];
		}
	}
	else
	{
		for (int_max LinearIndex = 0; LinearIndex < ElementCount; ++LinearIndex)
		{
			Output[LinearIndex] = IndependentElement 
								  + MatrixElementPointerList[0][LinearIndex] 
								  + MatrixElementPointerList[1][LinearIndex]
								  + MatrixElementPointerList[2][LinearIndex];
		}
	}
}


template<typename ElementType>
inline
void MatrixLinearCombine_MatrixNumber_4(ElementType* Output,
						                const int_max ElementCount,
                                        const std::vector<ElementType>& CoefList,  
                                        const std::vector<const ElementType*>& MatrixElementPointerList,
                                        const ElementType& IndependentElement)
{
	if (CoefList[0] != ElementType(1) || CoefList[1] != ElementType(1) || CoefList[2] != ElementType(1) || CoefList[3] != ElementType(1))
	{
		for (int_max LinearIndex = 0; LinearIndex < ElementCount; ++LinearIndex)
		{
			Output[LinearIndex] = IndependentElement
								  + CoefList[0] * MatrixElementPointerList[0][LinearIndex]
								  + CoefList[1] * MatrixElementPointerList[1][LinearIndex]
								  + CoefList[2] * MatrixElementPointerList[2][LinearIndex]
								  + CoefList[3] * MatrixElementPointerList[3][LinearIndex];
		}
	}
	else
	{
		for (int_max LinearIndex = 0; LinearIndex < ElementCount; ++LinearIndex)
		{
			Output[LinearIndex] = IndependentElement
								  + MatrixElementPointerList[0][LinearIndex]
								  + MatrixElementPointerList[1][LinearIndex]
								  + MatrixElementPointerList[2][LinearIndex]
								  + MatrixElementPointerList[3][LinearIndex];
		}
	}
}


template<typename ElementType>
inline
void MatrixLinearCombine_MatrixNumber_5(ElementType* Output,
						                const int_max ElementCount,
                                        const std::vector<ElementType>& CoefList,  
                                        const std::vector<const ElementType*>& MatrixElementPointerList,
                                        const ElementType& IndependentElement)
{
	if (CoefList[0] != ElementType(1) || CoefList[1] != ElementType(1) || CoefList[2] != ElementType(1)
		|| CoefList[3] != ElementType(1) || CoefList[4] != ElementType(1))
	{
		for (int_max LinearIndex = 0; LinearIndex < ElementCount; ++LinearIndex)
		{
			Output[LinearIndex] = IndependentElement
								  + CoefList[0] * MatrixElementPointerList[0][LinearIndex]
								  + CoefList[1] * MatrixElementPointerList[1][LinearIndex]
								  + CoefList[2] * MatrixElementPointerList[2][LinearIndex]
								  + CoefList[3] * MatrixElementPointerList[3][LinearIndex]
								  + CoefList[4] * MatrixElementPointerList[4][LinearIndex];
		}
	}
	else
	{
		for (int_max LinearIndex = 0; LinearIndex < ElementCount; ++LinearIndex)
		{
			Output[LinearIndex] = IndependentElement
								  + MatrixElementPointerList[0][LinearIndex]
								  + MatrixElementPointerList[1][LinearIndex]
								  + MatrixElementPointerList[2][LinearIndex]
								  + MatrixElementPointerList[3][LinearIndex]
								  + MatrixElementPointerList[4][LinearIndex];
		}
	}
}


template<typename ElementType>
inline
void MatrixLinearCombine_MatrixNumber_6(ElementType* Output,
						                const int_max ElementCount,
                                        const std::vector<ElementType>& CoefList,  
                                        const std::vector<const ElementType*>& MatrixElementPointerList,
                                        const ElementType& IndependentElement)
{
    for (int_max LinearIndex = 0; LinearIndex < ElementCount; ++LinearIndex)
    {
        Output[LinearIndex] = IndependentElement
                             + CoefList[0] * MatrixElementPointerList[0][LinearIndex]
                             + CoefList[1] * MatrixElementPointerList[1][LinearIndex]
                             + CoefList[2] * MatrixElementPointerList[2][LinearIndex]
                             + CoefList[3] * MatrixElementPointerList[3][LinearIndex]
                             + CoefList[4] * MatrixElementPointerList[4][LinearIndex]
                             + CoefList[5] * MatrixElementPointerList[5][LinearIndex];
    }
}


template<typename ElementType>
inline
void MatrixLinearCombine_MatrixNumber_7(ElementType* Output,
						                const int_max ElementCount,
                                        const std::vector<ElementType>& CoefList,  
                                        const std::vector<const ElementType*>& MatrixElementPointerList,
                                        const ElementType& IndependentElement)
{
    for (int_max LinearIndex = 0; LinearIndex < ElementCount; ++LinearIndex)
    {
        Output[LinearIndex] = IndependentElement
                             + CoefList[0] * MatrixElementPointerList[0][LinearIndex]
                             + CoefList[1] * MatrixElementPointerList[1][LinearIndex]
                             + CoefList[2] * MatrixElementPointerList[2][LinearIndex]
                             + CoefList[3] * MatrixElementPointerList[3][LinearIndex]
                             + CoefList[4] * MatrixElementPointerList[4][LinearIndex]
                             + CoefList[5] * MatrixElementPointerList[5][LinearIndex]
                             + CoefList[6] * MatrixElementPointerList[6][LinearIndex];
    }
}


template<typename ElementType>
inline
void MatrixLinearCombine_MatrixNumber_8(ElementType* Output,
						                const int_max ElementCount,
                                        const std::vector<ElementType>& CoefList,  
                                        const std::vector<const ElementType*>& MatrixElementPointerList,
                                        const ElementType& IndependentElement)
{
    for (int_max LinearIndex = 0; LinearIndex < ElementCount; ++LinearIndex)
    {
        Output[LinearIndex] = IndependentElement
                             + CoefList[0] * MatrixElementPointerList[0][LinearIndex]
                             + CoefList[1] * MatrixElementPointerList[1][LinearIndex]
                             + CoefList[2] * MatrixElementPointerList[2][LinearIndex]
                             + CoefList[3] * MatrixElementPointerList[3][LinearIndex]
                             + CoefList[4] * MatrixElementPointerList[4][LinearIndex]
                             + CoefList[5] * MatrixElementPointerList[5][LinearIndex]
                             + CoefList[6] * MatrixElementPointerList[6][LinearIndex]
                             + CoefList[7] * MatrixElementPointerList[7][LinearIndex];
    }
}


template<typename ElementType>
inline
void MatrixLinearCombine_MatrixNumber_9(ElementType* Output,
						                const int_max ElementCount,
                                        const std::vector<ElementType>& CoefList,  
                                        const std::vector<const ElementType*>& MatrixElementPointerList,
                                        const ElementType& IndependentElement)
{
    for (int_max LinearIndex = 0; LinearIndex < ElementCount; ++LinearIndex)
    {
        Output[LinearIndex] = IndependentElement
                             + CoefList[0] * MatrixElementPointerList[0][LinearIndex]
                             + CoefList[1] * MatrixElementPointerList[1][LinearIndex]
                             + CoefList[2] * MatrixElementPointerList[2][LinearIndex]
                             + CoefList[3] * MatrixElementPointerList[3][LinearIndex]
                             + CoefList[4] * MatrixElementPointerList[4][LinearIndex]
                             + CoefList[5] * MatrixElementPointerList[5][LinearIndex]
                             + CoefList[6] * MatrixElementPointerList[6][LinearIndex]
                             + CoefList[7] * MatrixElementPointerList[7][LinearIndex]
                             + CoefList[8] * MatrixElementPointerList[8][LinearIndex];
    }
}


template<typename ElementType>
inline
void MatrixLinearCombine_MatrixNumber_10(ElementType* Output,
						                 const int_max ElementCount,
                                         const std::vector<ElementType>& CoefList,  
                                         const std::vector<const ElementType*>& MatrixElementPointerList,
                                         const ElementType& IndependentElement)
{
    for (int_max LinearIndex = 0; LinearIndex < ElementCount; ++LinearIndex)
    {
        Output[LinearIndex] = IndependentElement
                             + CoefList[0] * MatrixElementPointerList[0][LinearIndex]
                             + CoefList[1] * MatrixElementPointerList[1][LinearIndex]
                             + CoefList[2] * MatrixElementPointerList[2][LinearIndex]
                             + CoefList[3] * MatrixElementPointerList[3][LinearIndex]
                             + CoefList[4] * MatrixElementPointerList[4][LinearIndex]
                             + CoefList[5] * MatrixElementPointerList[5][LinearIndex]
                             + CoefList[6] * MatrixElementPointerList[6][LinearIndex]
                             + CoefList[7] * MatrixElementPointerList[7][LinearIndex]
                             + CoefList[8] * MatrixElementPointerList[8][LinearIndex]
                             + CoefList[9] * MatrixElementPointerList[9][LinearIndex];
    }
}

//======================================================================================================================================================//

}//end namespace mdk
