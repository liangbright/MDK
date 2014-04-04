#ifndef __mdkLinearAlgebra_Function_DenseMatrix_Part_3_h
#define __mdkLinearAlgebra_Function_DenseMatrix_Part_3_h

#include <string>

#include <Eigen/Dense>

#include "mdkType.h"

namespace mdk
{

//forward-declare ----------------//
template<typename ElementType>
class DenseMatrix;
//-------------------------------------//

// SingleVector is a column vector
// each column of VectorSet is a vector
// L2DistanceList[j] = distance between SingleVector and VectorSet(ALL, j)
template<typename ElementType>
DenseMatrix<ElementType> ComputeL2DistanceListFromSingleVectorToVectorSet(const DenseMatrix<ElementType>& SingleVector,
                                                                          const DenseMatrix<ElementType>& VectorSet);


//--------------------SolveLinearLeastSquaresProblem (lsqlin in Matlab) --------------------------------------------------------//
// similar to lsqlin in Matlab, use the same notation
//
// find X that minimizes ||C*X - d||^2 such that A*X <= b, Aeq*X = beq, lb <= X <= ub
//
// X, d, b, lb, ub : col vector


template<typename ElementType>
struct DenseLsqlinOption
{
    std::string MethodName;

    DenseMatrix<ElementType> GramianMatrix_CtC; // C'*C

//-------------------------------

    DenseLsqlinOption()
    {

    }

    ~DenseLsqlinOption() {}
};


template<typename ElementType>
struct DenseLsqlinResult
{
    DenseMatrix<ElementType> X;

    DenseMatrix<ElementType> Residual;

    std::string MethodName;

//--------------------------------------------------
    DenseLsqlinResult() {};
    ~DenseLsqlinResult() {};

    DenseLsqlinResult(DenseLsqlinResult&& InputResult)
    {
        X = std::move(InputResult.X);
        Residual = std::move(InputResult.Residual);
        MethodName = std::move(InputResult.MethodName);
    }

    void operator=(DenseLsqlinResult&& InputResult)
    {
        X = std::move(InputResult.X);
        Residual = std::move(InputResult.Residual);
        MethodName = std::move(InputResult.MethodName);
    }

private:
    DenseLsqlinResult(const DenseLsqlinResult&) = delete;
    void operator=(const DenseLsqlinResult&) = delete;
};

template<typename ElementType>
DenseLsqlinResult<ElementType> SolveLinearLeastSquaresProblem(const DenseMatrix<ElementType>& C,
                                                              const DenseMatrix<ElementType>& d,
                                                              const DenseLsqlinOption<ElementType>& Option);

template<typename ElementType>
DenseLsqlinResult<ElementType> SolveLinearLeastSquaresProblem(const DenseMatrix<ElementType>& X0,
                                                              const DenseMatrix<ElementType>& C,
                                                              const DenseMatrix<ElementType>& d);

template<typename ElementType>
DenseLsqlinResult<ElementType> SolveLinearLeastSquaresProblem(const DenseMatrix<ElementType>& X0,
                                                              const DenseMatrix<ElementType>& C,
                                                              const DenseMatrix<ElementType>& d,
                                                              const DenseMatrix<ElementType>& A, 
                                                              const DenseMatrix<ElementType>& b);

template<typename ElementType>
DenseLsqlinResult<ElementType> SolveLinearLeastSquaresProblem(const DenseMatrix<ElementType>& X0,
                                                              const DenseMatrix<ElementType>& C,
                                                              const DenseMatrix<ElementType>& d,
                                                              const DenseMatrix<ElementType>& A, 
                                                              const DenseMatrix<ElementType>& b, 
                                                              const DenseMatrix<ElementType>& Aeq,
                                                              const DenseMatrix<ElementType>& beq);

template<typename ElementType>
DenseLsqlinResult<ElementType> SolveLinearLeastSquaresProblem(const DenseMatrix<ElementType>& X0,
                                                              const DenseMatrix<ElementType>& C,
                                                              const DenseMatrix<ElementType>& d,
                                                              const DenseMatrix<ElementType>& A, 
                                                              const DenseMatrix<ElementType>& b, 
                                                              const DenseMatrix<ElementType>& Aeq,
                                                              const DenseMatrix<ElementType>& beq,
                                                              const DenseMatrix<ElementType>& lb,
                                                              const DenseMatrix<ElementType>& ub);

template<typename ElementType>
DenseLsqlinResult<ElementType> SolveLinearLeastSquaresProblem(const DenseMatrix<ElementType>& X0, 
                                                              const DenseMatrix<ElementType>& C,
                                                              const DenseMatrix<ElementType>& d,
                                                              const DenseMatrix<ElementType>& A, 
                                                              const DenseMatrix<ElementType>& b, 
                                                              const DenseMatrix<ElementType>& Aeq,
                                                              const DenseMatrix<ElementType>& beq,
                                                              const DenseMatrix<ElementType>& lb,
                                                              const DenseMatrix<ElementType>& ub);

template<typename ElementType>
DenseLsqlinResult<ElementType> SolveLinearLeastSquaresProblem(const DenseMatrix<ElementType>& X0,
                                                              const DenseMatrix<ElementType>& C,
                                                              const DenseMatrix<ElementType>& d,
                                                              const DenseMatrix<ElementType>& A, 
                                                              const DenseMatrix<ElementType>& b, 
                                                              const DenseMatrix<ElementType>& Aeq,
                                                              const DenseMatrix<ElementType>& beq,
                                                              const DenseMatrix<ElementType>& lb,
                                                              const DenseMatrix<ElementType>& ub,                                                              
                                                              const DenseLsqlinOption<ElementType>& Option);


}

#include "mdkLinearAlgebra_Function_DenseMatrix_Part_3.hpp"

#endif