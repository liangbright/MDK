#ifndef __mdkLinearAlgebra_Function_DenseMatrix_Part_3_h
#define __mdkLinearAlgebra_Function_DenseMatrix_Part_3_h


#include "mdkType.h"

namespace mdk
{

//forward-declare ----------------//
template<typename ElementType>
class DenseMatrix;
//-------------------------------------//


//--------------------SolveLinearEquationByLeastSqures = lsqlin in Matlab --------------------------------------------------------//
// similar to lsqlin in Matlab, use the same notation

struct DenseLsqlinOption
{

    DenseLsqlinOption()
    {

    }

    ~DenseLsqlinOption()
    {}
};


template<typename ElementType>
struct DenseLsqlinResult
{
    DenseMatrix<ElementType> X;

    DenseMatrix<ElementType> Residual;

//--------------------------------------------------
    LsqlinResult() {};
    ~LsqlinResult() {};

    LsqlinResult(LsqlinResult&& InputResult)
    {
        X = std::move(InputResult.X);
        Residual = std::move(InputResult.Residual);
    }

    operator=(LsqlinResult&& InputResult)
    {
        X = std::move(InputResult.X);
        Residual = std::move(InputResult.Residual);
    }

private:
    LsqlinResult(const LsqlinResult&) = delete;
    void operator(const LsqlinResult&) = delete;
};

template<typename ElementType>
DenseLsqlinResult<ElementType> SolveLinearEquationByLeastSqures(const DenseMatrix<ElementType>& C,
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

#include "mdkLinearAlgebra_Function_DenseMatrix_Part_3_hpp"

#endif