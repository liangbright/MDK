#ifndef __mdkLinearLeastSquaresProblemSolver_h
#define __mdkLinearLeastSquaresProblemSolver_h

#include <string>
#include <memory>

#include <Eigen/Dense>

#include "mdkProcessObject.h"
#include "mdkString.h"
#include "mdkQuadraticProgrammingSolver.h"


//--------------------LinearLeastSquaresProblemSolver (lsqlin in Matlab) --------------------------------------------------------//
// similar to lsqlin in Matlab
//
// find x that minimizes 0.5*||D*x - c||^2  
//                  <=>  0.5*x'*(D'*D)*x - x'*(D'*c) + 0.5*c'*c  
//                  <=>  0.5 * x'*H*x + x'*g with H = D'*D, and g = -D'*c;
//
// subject to:
//
// lb_A <= A*x <= ub_A
// lb_x <= x <= ub_x


namespace mdk
{

enum struct MDK_MethodType_Of_LinearLeastSquaresProblemSolver
{
    QuadraticProgramming,
    NormalEquation,
    SVD,
    QR,
    Unknown
};


struct Option_Of_LinearLeastSquaresProblemSolver
{
    MDK_MethodType_Of_LinearLeastSquaresProblemSolver MethodType;

//-------------------------------

    Option_Of_LinearLeastSquaresProblemSolver()
    {
        this->SetToDefault();
    }

    ~Option_Of_LinearLeastSquaresProblemSolver() {}

    void operator=(const Option_Of_LinearLeastSquaresProblemSolver& InputOption)
    {
        MethodType = InputOption.MethodType;
    }

    void SetToDefault()
    {
        MethodType = MDK_MethodType_Of_LinearLeastSquaresProblemSolver::QuadraticProgramming;
    }
//
private:
    Option_Of_LinearLeastSquaresProblemSolver(const Option_Of_LinearLeastSquaresProblemSolver&) = delete;    
};


template<typename ElementType>
struct Solution_Of_LinearLeastSquaresProblem
{
    DenseMatrix<ElementType> X;

    MDK_MethodType_Of_LinearLeastSquaresProblemSolver MethodType;

//--------------------------------------------------
    Solution_Of_LinearLeastSquaresProblem() {};
    
    Solution_Of_LinearLeastSquaresProblem(Solution_Of_LinearLeastSquaresProblem&& InputSolution)
    {
        X = std::move(InputSolution.X);
        MethodType = InputSolution.MethodType;
    }

    ~Solution_Of_LinearLeastSquaresProblem() {};

    void operator=(Solution_Of_LinearLeastSquaresProblem&& InputSolution)
    {
        X = std::move(InputSolution.X);
        MethodType = InputSolution.MethodType;
    }

    void Clear()
    {
        X.Clear();
        MethodType = MDK_MethodType_Of_LinearLeastSquaresProblemSolver::Unknown;
    }

    void Share(Solution_Of_LinearLeastSquaresProblem& InputSolution)
    {
        X.Share(InputSolution.X);
        MethodType = InputSolution.MethodType; // this is not shared, but it does not matter because Solution is output
    }

private:
    Solution_Of_LinearLeastSquaresProblem(const Solution_Of_LinearLeastSquaresProblem&) = delete;
    void operator=(const Solution_Of_LinearLeastSquaresProblem&) = delete;
};


template<typename ElementType>
class LinearLeastSquaresProblemSolver : public ProcessObject
{
public:
	typedef MDK_MethodType_Of_LinearLeastSquaresProblemSolver MethodTypeEnum;

public:
    Option_Of_LinearLeastSquaresProblemSolver m_Option;

private:

    const DenseMatrix<ElementType>*  m_D_dense;
    const DenseMatrix<ElementType>*  m_c_dense;
    const DenseMatrix<ElementType>*  m_lb_x_dense;
    const DenseMatrix<ElementType>*  m_ub_x_dense;
    const DenseMatrix<ElementType>*  m_A_dense;
    const DenseMatrix<ElementType>*  m_lb_A_dense;
    const DenseMatrix<ElementType>*  m_ub_A_dense;
    const DenseMatrix<ElementType>*  m_x0_dense;
    const DenseMatrix<ElementType>*  m_H_dense;

    const SparseMatrix<ElementType>*  m_D_sparse;
    const SparseMatrix<ElementType>*  m_c_sparse;
    const SparseMatrix<ElementType>*  m_lb_x_sparse;
    const SparseMatrix<ElementType>*  m_ub_x_sparse;
    const SparseMatrix<ElementType>*  m_A_sparse;
    const SparseMatrix<ElementType>*  m_lb_A_sparse;
    const SparseMatrix<ElementType>*  m_ub_A_sparse;
    const SparseMatrix<ElementType>*  m_x0_sparse;
    const SparseMatrix<ElementType>*  m_H_sparse;

    bool m_IsInputDense;

    Solution_Of_LinearLeastSquaresProblem<ElementType>* m_Solution;

    Solution_Of_LinearLeastSquaresProblem<ElementType> m_Solution_SharedCopy;

    //----empty matrix
    DenseMatrix<ElementType>  m_EmptyDenseMatrix;
    SparseMatrix<ElementType> m_EmptySparseMatrix;

public:
    LinearLeastSquaresProblemSolver();
    ~LinearLeastSquaresProblemSolver();

    void Clear();

    void SetInputData(const DenseMatrix<ElementType>* D,                      
                      const DenseMatrix<ElementType>* c    = nullptr,
                      const DenseMatrix<ElementType>* lb_x = nullptr,
                      const DenseMatrix<ElementType>* ub_x = nullptr,
                      const DenseMatrix<ElementType>* A    = nullptr,
                      const DenseMatrix<ElementType>* lb_A = nullptr,
                      const DenseMatrix<ElementType>* ub_A = nullptr,
                      const DenseMatrix<ElementType>* x0   = nullptr,
                      const DenseMatrix<ElementType>* H    = nullptr);

    void SetInputData(const SparseMatrix<ElementType>*  D,                     
                      const SparseMatrix<ElementType>*  c    = nullptr,
                      const SparseMatrix<ElementType>*  lb_x = nullptr,
                      const SparseMatrix<ElementType>*  ub_x = nullptr,
                      const SparseMatrix<ElementType>*  A    = nullptr,
                      const SparseMatrix<ElementType>*  lb_A = nullptr,
                      const SparseMatrix<ElementType>*  ub_A = nullptr,
                      const SparseMatrix<ElementType>*  x0   = nullptr,
                      const SparseMatrix<ElementType>*  H    = nullptr);

    bool SetOutputSolution(Solution_Of_LinearLeastSquaresProblem<ElementType>* Solution);

    bool Update();

    Solution_Of_LinearLeastSquaresProblem<ElementType>* GetSolution();

    //--------------------------------------------------------------------------------------------------------//
    static Solution_Of_LinearLeastSquaresProblem<ElementType> Apply(const DenseMatrix<ElementType>* D,
                                                                    const DenseMatrix<ElementType>* c    = nullptr,
                                                                    const DenseMatrix<ElementType>* lb_x = nullptr,
                                                                    const DenseMatrix<ElementType>* ub_x = nullptr,
                                                                    const DenseMatrix<ElementType>* A    = nullptr,
                                                                    const DenseMatrix<ElementType>* lb_A = nullptr,
                                                                    const DenseMatrix<ElementType>* ub_A = nullptr,
                                                                    const DenseMatrix<ElementType>* x0   = nullptr,
                                                                    const DenseMatrix<ElementType>* H    = nullptr,
                                                                    const Option_Of_LinearLeastSquaresProblemSolver* Option = nullptr);
        
    static Solution_Of_LinearLeastSquaresProblem<ElementType> Apply(const SparseMatrix<ElementType>*  D,
                                                                    const SparseMatrix<ElementType>*  c    = nullptr,
                                                                    const SparseMatrix<ElementType>*  lb_x = nullptr,
                                                                    const SparseMatrix<ElementType>*  ub_x = nullptr,
                                                                    const SparseMatrix<ElementType>*  A    = nullptr,
                                                                    const SparseMatrix<ElementType>*  lb_A = nullptr,
                                                                    const SparseMatrix<ElementType>*  ub_A = nullptr,
                                                                    const SparseMatrix<ElementType>*  x0   = nullptr,
                                                                    const SparseMatrix<ElementType>*  H    = nullptr,
                                                                    const Option_Of_LinearLeastSquaresProblemSolver* Option = nullptr);

    static Solution_Of_LinearLeastSquaresProblem<ElementType> Apply(const DenseMatrix<ElementType>*  D,
                                                                    const DenseMatrix<ElementType>*  c,
                                                                    const DenseMatrix<ElementType>*  lb_x,
                                                                    const DenseMatrix<ElementType>*  ub_x,
                                                                    const SparseMatrix<ElementType>* A,
                                                                    const DenseMatrix<ElementType>*  lb_A,
                                                                    const DenseMatrix<ElementType>*  ub_A,
                                                                    const DenseMatrix<ElementType>*  x0,
                                                                    const DenseMatrix<ElementType>*  H );

private:

    void ClearPipelineOutput();
    void UpdatePipelineOutput();

    bool CheckInput_dense();
    bool Update_dense_unconstrained();
    bool Update_dense_QuadraticProgramming();

    bool CheckInput_sparse();
    bool Update_sparse_unconstrained();
    bool Update_sparse_QuadraticProgramming();

private:
    LinearLeastSquaresProblemSolver(const LinearLeastSquaresProblemSolver&) = delete;
    void operator=(const LinearLeastSquaresProblemSolver&) = delete;
};

}//namespace mdk

#include "mdkLinearLeastSquaresProblemSolver.hpp"

#endif