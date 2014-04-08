#ifndef __mdkQuadraticProgrammingSolver_h
#define __mdkQuadraticProgrammingSolver_h

#include <string>
#include <memory>


#include <qpOASES.h>


#include "mdkObject.h"
#include "mdkDenseMatrix.h"
#include "mdkSparseMatrix.h"


// find x that minimizes 0.5 * x'*H*x + x'*g
//
// subject to
//
// lb_A <= A*x <= ub_A
// lb_x <= x <= ub_x


namespace mdk
{

enum struct HessianType_Of_QuadraticProgramming
{
    PositiveSemidefinite,

    Unknown,

//special type:
    Zero,
    Indentity,
};

struct Option_Of_QuadraticProgramming
{
    std::string Mode;
    // OneTimeOnly          (default mode)
    // Online_Varying_H_A   (for dictionary learning)
    // Online_Fixed_H_A     (for some control problems)
    
    double CPUTime;

    int_max MaxIterNumber;

    HessianType_Of_QuadraticProgramming HessianType;

    bool InputDataWillNotbeUsedByAnyOtherProcessDuringOptimization;
    // qpOASES use row major dense matrix, so input H and A need to be transposed
    // if it is true, then perform in place transpose, and transpose back after computation 
    // if it is false, then copy H and A, and perform in place transpose
    //                 clear copy of H and A after computation
    
    Option_Of_QuadraticProgramming()
    {
        this->SetToDefault();
    }

    ~Option_Of_QuadraticProgramming() {}

    void operator=(const Option_Of_QuadraticProgramming& InputOption)
    {
        Mode = InputOption.Mode;
        CPUTime = InputOption.CPUTime;
        MaxIterNumber = InputOption.MaxIterNumber;
        HessianType = InputOption.HessianType;
        InputDataWillNotbeUsedByAnyOtherProcessDuringOptimization = InputOption.InputDataWillNotbeUsedByAnyOtherProcessDuringOptimization;
    }

    void SetToDefault()
    {
        Mode = "OneTimeOnly";
        CPUTime = false;
        MaxIterNumber = -1;
        HessianType = HessianType_Of_QuadraticProgramming::Unknown;
        InputDataWillNotbeUsedByAnyOtherProcessDuringOptimization = false;
    }


private:
    Option_Of_QuadraticProgramming(const Option_Of_QuadraticProgramming&) = delete;
};


template<typename ElementType>
struct Solution_Of_QuadraticProgramming
{
    DenseMatrix<ElementType> X;

    ElementType ObjectiveFunctionValue;

    //----------------------

    std::string MethodName;

    double CPUTime;

    int_max WSR;
//---------------------------------------------
    Solution_Of_QuadraticProgramming() 
    {
        MethodName = "ActiveSet_qpOASES";
        this->Clear();
    }

    ~Solution_Of_QuadraticProgramming() {}

    Solution_Of_QuadraticProgramming(Solution_Of_QuadraticProgramming&& InputSolution)
    {
        this->Take(std::forward<Solution_Of_QuadraticProgramming&>(InputSolution));
    }

    void Clear()
    {
        X.Clear();
        CPUTime = 0;
        WSR = 0;
        ObjectiveFunctionValue = 0;
    }

    void ShallowCopy(Solution_Of_QuadraticProgramming& InputSolution)
    {
        X.ForceShare(InputSolution.X);
        MethodName = InputSolution.MethodName;
    }

    void Take(Solution_Of_QuadraticProgramming& InputSolution)
    {
        X = std::move(InputSolution.X);
        ObjectiveFunctionValue = InputSolution.ObjectiveFunctionValue;

        MethodName = std::move(InputSolution.MethodName);
        CPUTime = InputSolution.CPUTime;
        WSR = InputSolution.WSR;
    }

private:
    Solution_Of_QuadraticProgramming(const Solution_Of_QuadraticProgramming&) = delete;
    void operator=(const Solution_Of_QuadraticProgramming&) = delete;
};


template<typename ElementType>
class QuadraticProgrammingSolver : public Object
{
public:
    Option_Of_QuadraticProgramming m_Option;

private:

    qpOASES::Options<ElementType> m_Option_qpOASES;

    const DenseMatrix<ElementType>*  m_H_dense;
    const DenseMatrix<ElementType>*  m_g_dense;
    const DenseMatrix<ElementType>*  m_lb_x_dense;
    const DenseMatrix<ElementType>*  m_ub_x_dense;
    const DenseMatrix<ElementType>*  m_A_dense;
    const DenseMatrix<ElementType>*  m_lb_A_dense;
    const DenseMatrix<ElementType>*  m_ub_A_dense;
    const DenseMatrix<ElementType>*  m_x0_dense;

    const SparseMatrix<ElementType>*  m_H_sparse;
    const SparseMatrix<ElementType>*  m_g_sparse;
    const SparseMatrix<ElementType>*  m_lb_x_sparse;
    const SparseMatrix<ElementType>*  m_ub_x_sparse;
    const SparseMatrix<ElementType>*  m_A_sparse;
    const SparseMatrix<ElementType>*  m_lb_A_sparse;
    const SparseMatrix<ElementType>*  m_ub_A_sparse;
    const SparseMatrix<ElementType>*  m_x0_sparse;

    bool m_IsInputDense;

    Solution_Of_QuadraticProgramming<ElementType>* m_Solution;

    Solution_Of_QuadraticProgramming<ElementType> m_Solution_SharedCopy;

    //-------------------------------------------------------------------//
    std::unique_ptr<qpOASES::QProblem<ElementType>>  m_QProblem_Online;
    std::unique_ptr<qpOASES::QProblemB<ElementType>> m_QProblemB_Online;
    std::unique_ptr<qpOASES::SQProblem<ElementType>> m_SQProblem_Online;

    int_max m_Counter_In_Mode_Online_Varying_H_A;
    int_max m_VaribleNumber_In_Mode_Online_Varying_H_A;
    int_max m_ConstraintNumber_In_Mode_Online_Varying_H_A;

    //----empty matrix
    DenseMatrix<ElementType>  m_EmptyDenseMatrix;
    SparseMatrix<ElementType> m_EmptySparseMatrix;

public:

    QuadraticProgrammingSolver();
    ~QuadraticProgrammingSolver();

    void Clear();

    void SetInputData(const DenseMatrix<ElementType>* H,                      
                      const DenseMatrix<ElementType>* g    = nullptr,
                      const DenseMatrix<ElementType>* lb_x = nullptr,
                      const DenseMatrix<ElementType>* ub_x = nullptr,
                      const DenseMatrix<ElementType>* A    = nullptr,
                      const DenseMatrix<ElementType>* lb_A = nullptr,
                      const DenseMatrix<ElementType>* ub_A = nullptr,
                      const DenseMatrix<ElementType>* x0   = nullptr);

    void SetInputData(const SparseMatrix<ElementType>*  H,                     
                      const SparseMatrix<ElementType>*  g    = nullptr,
                      const SparseMatrix<ElementType>*  lb_x = nullptr,
                      const SparseMatrix<ElementType>*  ub_x = nullptr,
                      const SparseMatrix<ElementType>*  A    = nullptr,
                      const SparseMatrix<ElementType>*  lb_A = nullptr,
                      const SparseMatrix<ElementType>*  ub_A = nullptr,
                      const SparseMatrix<ElementType>*  x0   = nullptr);

    bool SetOutputSolution(Solution_Of_QuadraticProgramming<ElementType>* Solution);

    bool Update();

    Solution_Of_QuadraticProgramming<ElementType>* GetSolution();

    //--------------------------------------------------------------------------------------------------------//
    static Solution_Of_QuadraticProgramming<ElementType> Apply(const DenseMatrix<ElementType>* H,                                                               
                                                               const DenseMatrix<ElementType>* g    = nullptr,
                                                               const DenseMatrix<ElementType>* lb_x = nullptr,
                                                               const DenseMatrix<ElementType>* ub_x = nullptr,
                                                               const DenseMatrix<ElementType>* A    = nullptr,
                                                               const DenseMatrix<ElementType>* lb_A = nullptr,
                                                               const DenseMatrix<ElementType>* ub_A = nullptr,
                                                               const DenseMatrix<ElementType>* x0   = nullptr,
                                                               const Option_Of_QuadraticProgramming* Option = nullptr);

    static Solution_Of_QuadraticProgramming<ElementType> Apply(const SparseMatrix<ElementType>*  H,
                                                               const SparseMatrix<ElementType>*  g    = nullptr,
                                                               const SparseMatrix<ElementType>*  lb_x = nullptr,
                                                               const SparseMatrix<ElementType>*  ub_x = nullptr,
                                                               const SparseMatrix<ElementType>*  A    = nullptr,
                                                               const SparseMatrix<ElementType>*  lb_A = nullptr,
                                                               const SparseMatrix<ElementType>*  ub_A = nullptr,
                                                               const SparseMatrix<ElementType>*  x0   = nullptr,
                                                               const Option_Of_QuadraticProgramming* Option = nullptr);
private:

    bool CheckInput_dense_OneTimeOnly();
    bool Update_dense_OneTimeOnly();

    bool CheckInput_sparse_OneTimeOnly();
    bool Update_sparse_OneTimeOnly();

    bool CheckInput_dense_Online_Varying_H_A();
    bool Update_dense_Online_Varying_H_A();

    bool CheckInput_sparse_Online_Varying_H_A();
    bool Update_sparse_Online_Varying_H_A();

    bool CheckInput_dense_Online_Fixed_H_A();
    bool Update_dense_Online_Fixed_H_A();

    bool CheckInput_sparse_Online_Fixed_H_A();
    bool Update_sparse_Online_Fixed_H_A();

private:
    QuadraticProgrammingSolver(const QuadraticProgrammingSolver&) = delete;
    void operator=(const QuadraticProgrammingSolver&) = delete;
};

}//namespace mdk

#include "mdkQuadraticProgrammingSolver.hpp"

#endif