#ifndef __mdkLinearAlgebra_Function_DenseMatrix_Part_3_hpp
#define __mdkLinearAlgebra_Function_DenseMatrix_Part_3_hpp

//#include "mdkLinearAlgebra_Function_DenseMatrix_Part_3.h"

namespace mdk
{

// SingleVector is a column vector
// each column of VectorSet is a vector
// L2DistanceList[j] = distance between SingleVector and VectorSet(ALL, j)
template<typename ElementType>
DenseMatrix<ElementType> ComputeL2DistanceListFromSingleVectorToVectorSet(const DenseMatrix<ElementType>& SingleVector,
                                                                          const DenseMatrix<ElementType>& VectorSet)
{
    DenseMatrix<ElementType> L2DistanceList;

    auto Size = VectorSet.GetSize();

    if (Size.ColNumber == 0)
    {
        MDK_Error("Empty Dictionary @ mdkLinearAlgebra_DenseMatrix ComputeL2DistanceListFromSingleVectorToVectorSet(...)")
        return L2DistanceList;
    }

    if (SingleVector.GetElementNumber() != Size.RowNumber)
    {
        MDK_Error("Size does not match @ mdkLinearAlgebra_DenseMatrix ComputeL2DistanceListFromSingleVectorToVectorSet(...)")
        return L2DistanceList;
    }

    L2DistanceList.Resize(1, Size.ColNumber);

    auto ElementPointer = VectorSet.GetElementPointer();

    for (int_max j = 0; j < Size.ColNumber; ++j)
    {
        auto BeginPointer_j = ElementPointer + j*Size.RowNumber;

        ElementType Distance_j = ElementType(0);

        for (int_max i = 0; i < Size.RowNumber; ++i)
        {
            auto temp = SingleVector[i] - BeginPointer_j[i];
            Distance_j += temp*temp;
        }

        L2DistanceList[j] = std::sqrt(Distance_j);
    }

    return L2DistanceList;
}


//--------------------SolveLinearLeastSquaresProblem (lsqlin in Matlab) --------------------------------------------------------//
// similar to lsqlin in Matlab, use the same notation
//
// find X that minimizes ||C*X - d||^2 such that A*X <= b, Aeq*X = beq, lb <= X <= ub

template<typename ElementType>
DenseLsqlinResult<ElementType> SolveLinearLeastSquaresProblem(const DenseMatrix<ElementType>& C,
                                                              const DenseMatrix<ElementType>& d,
                                                              const std::string& MethodName = "SVD")
{
    DenseLsqlinResult<ElementType> Result;

    Result.X.FastResize(C.GetColNumber(), 1);

    Result.Residual.FastResize(C.GetRowNumber(), 1);

    if (MethodName == "SVD")
    {
        Result.MethodName = "SVD";

        typedef Eigen::Map<Eigen::Matrix<ElementType, Eigen::Dynamic, Eigen::Dynamic>> EigenMapDynamicMatrix;

        EigenMapDynamicMatrix Input_C(const_cast<ElementType*>(C.GetElementPointer()), C.GetRowNumber(), C.GetColNumber());

        EigenMapDynamicMatrix Input_d(const_cast<ElementType*>(d.GetElementPointer()), d.GetElementNumber(), 1);

        EigenMapDynamicMatrix Output_X(Result.X.GetElementPointer(), Result.X.GetElementNumber(), 1);

        EigenMapDynamicMatrix Output_Residual(Result.Residual.GetElementPointer(), Result.Residual.GetElementNumber(), 1);

        Output_X = Input_C.jacobiSvd(Eigen::ComputeThinU | Eigen::ComputeThinV).solve(Input_d);

        Output_Residual = Input_C*Output_X - Input_d;
    }
    else
    {
        MDK_Error("Not supported yet @ SolveLinearLeastSquaresProblem(...)")
    }

    return Result;
}


template<typename ElementType>
DenseLsqlinResult<ElementType> SolveLinearLeastSquaresProblem(const DenseMatrix<ElementType>& X0,
                                                              const DenseMatrix<ElementType>& C,
                                                              const DenseMatrix<ElementType>& d);


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
                                                              const DenseLsqlinOption& Option);
}

#endif