#ifndef __mdkLinearAlgebra_Function_DenseMatrix_Part_3_hpp
#define __mdkLinearAlgebra_Function_DenseMatrix_Part_3_hpp

//#include "mdkLinearAlgebra_Function_DenseMatrix_Part_3.h"

namespace mdk
{

//--------------------SolveLinearLeastSquaresProblem (lsqlin in Matlab) --------------------------------------------------------//
// similar to lsqlin in Matlab, use the same notation
//
// find X that minimizes ||C*X - d||^2 such that A*X <= b, Aeq*X = beq, lb <= X <= ub

template<typename ElementType>
DenseLsqlinResult<ElementType> SolveLinearLeastSquaresProblem(const DenseMatrix<ElementType>& C,
                                                              const DenseMatrix<ElementType>& d)
{
    DenseLsqlinResult<ElementType> Result;

    Result.X.FastResize(C.GetColNumber(), 1);

    Result.Residual.FastResize(C.GetRowNumber(), 1);

    Result.MethodName = "SVD";

    typedef Eigen::Map<Eigen::Matrix<ElementType, Eigen::Dynamic, Eigen::Dynamic>> EigenMapDynamicMatrix;

    EigenMapDynamicMatrix Input_C(const_cast<ElementType*>(C.GetElementPointer()), C.GetRowNumber(), C.GetColNumber());

    EigenMapDynamicMatrix Input_d(const_cast<ElementType*>(d.GetElementPointer()), d.GetElementNumber(), 1);

    EigenMapDynamicMatrix Output_X(Result.X.GetElementPointer(), Result.X.GetElementNumber(), 1);

    EigenMapDynamicMatrix Output_Residual(Result.Residual.GetElementPointer(), Result.Residual.GetElementNumber(), 1);

    Output_X = Input_C.jacobiSvd(Eigen::ComputeThinU | Eigen::ComputeThinV).solve(Input_d);

    Output_Residual = Input_C*Output_X - Input_d;

    return Result;
}


template<typename ElementType>
DenseLsqlinResult<ElementType> SolveLinearLeastSquaresProblem(const DenseMatrix<ElementType>& C,
                                                              const DenseMatrix<ElementType>& d,
                                                              const DenseMatrix<ElementType>& A, 
                                                              const DenseMatrix<ElementType>& b, 
                                                              const DenseMatrix<ElementType>& Aeq,
                                                              const DenseMatrix<ElementType>& beq);

template<typename ElementType>
DenseLsqlinResult<ElementType> SolveLinearLeastSquaresProblem(const DenseMatrix<ElementType>& C,
                                                              const DenseMatrix<ElementType>& d,
                                                              const DenseMatrix<ElementType>& A, 
                                                              const DenseMatrix<ElementType>& b, 
                                                              const DenseMatrix<ElementType>& Aeq,
                                                              const DenseMatrix<ElementType>& beq,
                                                              const DenseMatrix<ElementType>& lb,
                                                              const DenseMatrix<ElementType>& ub);

template<typename ElementType>
DenseLsqlinResult<ElementType> SolveLinearLeastSquaresProblem(const DenseMatrix<ElementType>& C,
                                                              const DenseMatrix<ElementType>& d,
                                                              const DenseMatrix<ElementType>& A, 
                                                              const DenseMatrix<ElementType>& b, 
                                                              const DenseMatrix<ElementType>& Aeq,
                                                              const DenseMatrix<ElementType>& beq,
                                                              const DenseMatrix<ElementType>& lb,
                                                              const DenseMatrix<ElementType>& ub,
                                                              const DenseMatrix<ElementType>& x0);

template<typename ElementType>
DenseLsqlinResult<ElementType> SolveLinearLeastSquaresProblem(const DenseMatrix<ElementType>& C,
                                                              const DenseMatrix<ElementType>& d,
                                                              const DenseMatrix<ElementType>& A, 
                                                              const DenseMatrix<ElementType>& b, 
                                                              const DenseMatrix<ElementType>& Aeq,
                                                              const DenseMatrix<ElementType>& beq,
                                                              const DenseMatrix<ElementType>& lb,
                                                              const DenseMatrix<ElementType>& ub,
                                                              const DenseMatrix<ElementType>& x0,
                                                              const DenseLsqlinOption& Option);
}

#endif