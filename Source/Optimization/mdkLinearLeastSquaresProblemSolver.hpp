#ifndef __mdkLinearLeastSquaresProblemSolver_hpp
#define __mdkLinearLeastSquaresProblemSolver_hpp

namespace mdk
{
template<typename ElementType>
LinearLeastSquaresProblemSolver<ElementType>::LinearLeastSquaresProblemSolver()
{
    this->Clear();
}


template<typename ElementType>
LinearLeastSquaresProblemSolver<ElementType>::~LinearLeastSquaresProblemSolver()
{
}


template<typename ElementType>
void LinearLeastSquaresProblemSolver<ElementType>::Clear()
{
    m_Option.SetToDefault();

    m_D_dense = nullptr;

    m_D_sparse = nullptr;

    m_H_dense = nullptr;

    m_H_sparse = nullptr;

    m_c = nullptr;

    m_lb_x = nullptr;
    m_ub_x = nullptr;

    m_lb_A = nullptr;
    m_ub_A = nullptr;

    m_x0 = nullptr;

    m_IsInputDense = true;

    m_Solution_SharedCopy.Clear();
    m_Solution = &m_Solution_SharedCopy;

    m_EmptyDenseMatrix.Clear();
    m_EmptyDenseMatrix.FixSize();

    m_EmptySparseMatrix.Clear();
    m_EmptySparseMatrix.FixSize();
}


template<typename ElementType>
void LinearLeastSquaresProblemSolver<ElementType>::SetInputData(const DenseMatrix<ElementType>* D,                                                           
                                                                const DenseMatrix<ElementType>* c,
                                                                const DenseMatrix<ElementType>* lb_x,
                                                                const DenseMatrix<ElementType>* ub_x,
                                                                const DenseMatrix<ElementType>* A,
                                                                const DenseMatrix<ElementType>* lb_A,
                                                                const DenseMatrix<ElementType>* ub_A,
                                                                const DenseMatrix<ElementType>* x0,
                                                                const DenseMatrix<ElementType>* H)
{
    m_D_dense = D;
    m_D_sparse = nullptr;

    m_H_dense = H;
    m_H_sparse = nullptr;

    m_c = c;

    m_lb_x = lb_x;
    m_ub_x = ub_x;

    m_A_dense = A;
    m_A_sparse = nullptr;

    m_lb_A = lb_A;
    m_ub_A = ub_A;

    m_x0 = x0;

    m_IsInputDense = true;
}


template<typename ElementType>
void LinearLeastSquaresProblemSolver<ElementType>::SetInputData(const SparseMatrix<ElementType>* D,                                                           
                                                                const DenseMatrix<ElementType>*  c,
                                                                const DenseMatrix<ElementType>*  lb_x,
                                                                const DenseMatrix<ElementType>*  ub_x,
                                                                const SparseMatrix<ElementType>* A,
                                                                const DenseMatrix<ElementType>*  lb_A,
                                                                const DenseMatrix<ElementType>*  ub_A,
                                                                const DenseMatrix<ElementType>*  x0,
                                                                const SparseMatrix<ElementType>* H)
{
    m_D_sparse = H;
    m_D_dense = nullptr;

    m_H_sparse = H;
    m_H_dense = nullptr;

    m_g = g;

    m_lb_x = lb_x;
    m_ub_x = ub_x;

    m_A_sparse = A;
    m_A_dense = nullptr;

    m_lb_A = lb_A;
    m_ub_A = ub_A;

    m_x0 = x0;

    m_IsInputDense = false;
}


template<typename ElementType>
bool LinearLeastSquaresProblemSolver<ElementType>::SetOutputSolution(Solution_Of_LinearLeastSquaresProblem<ElementType>* Solution)
{
    if (Solution == nullptr)
    {
        MDK_Error("Invalid input @ LinearLeastSquaresProblemSolver::SetOutputSolution(...)")
        return false;
    }

    m_Solution = Solution;

    m_Solution_SharedCopy.ShallowCopy(Solution);

    return true;
}


template<typename ElementType>
bool LinearLeastSquaresProblemSolver<ElementType>::Update()
{
    if (m_IsInputDense == true)
    {
        if (this->CheckInput_dense() == false)
        {
            return false;
        }

        if (m_lb_x_dense->IsEmpty() == true && m_ub_x_dense->IsEmpty() == true
            && m_lb_A_dense->IsEmpty() == true && m_ub_A_dense->IsEmpty() == true)
        {
            return this->Update_dense_unconstrained();
        }
        else
        {
            return this->Update_dense_constrained();
        }
    }
    else
    {
        if (this->CheckInput_sparse() == false)
        {
            return false;
        }

        if (m_lb_x_sparse->IsEmpty() == true && m_ub_x_sparse->IsEmpty() == true
            && m_lb_A_sparse->IsEmpty() == true && m_ub_A_sparse->IsEmpty() == true)
        {
            return this->Update_sparse_unconstrained();
        }
        else
        {
            return this->Update_sparse_constrained();
        }
    }
}


template<typename ElementType>
bool LinearLeastSquaresProblemSolver<ElementType>::CheckInput_dense()
{
    int_max VaribleNumber = 0;

    if (m_D_dense == nullptr)
    {
        MDK_Error("Input D is nullptr @ LinearLeastSquaresProblemSolver::CheckInput_dense()")
        return false;
    }

    auto SizeD = m_D_dense->GetSize();
    if (SizeD.RowNumber == 0)
    {
        MDK_Error("Input D is empty @ LinearLeastSquaresProblemSolver::CheckInput_dense()")
        return false;
    }

    VaribleNumber = SizeD.ColNumber;

    if (m_H_dense != nullptr)
    {
        auto SizeH = m_H_dense->GetSize();
        if (SizeH.RowNumber > 0 && SizeH.RowNumber != VaribleNumber)
        {
            MDK_Error("Input H size is wrong @ LinearLeastSquaresProblemSolver::CheckInput_dense()")
            return false;
        }
    }
    else
    {
        m_H_dense = &m_EmptyDenseMatrix;
    }

    if (m_c_dense != nullptr)
    {
        if (m_c_dense->IsColVector() == false)
        {
            MDK_Error("Input c is not a column vector @ LinearLeastSquaresProblemSolver::CheckInput_dense()")
            return false;
        }

        if (m_c_dense->GetElementNumber() != VaribleNumber)
        {
            MDK_Error("Input c Size is wrong @ LinearLeastSquaresProblemSolver::CheckInput_dense()")
            return false;
        }
    }
    else
    {
        m_c_dense = &m_EmptyDenseMatrix;
    }

    if (m_lb_x_dense != nullptr)
    {
        if (m_lb_x_dense->IsColVector() == false)
        {
            MDK_Error("Input lb_x is not a column vector @ LinearLeastSquaresProblemSolver::CheckInput_dense()")
            return false;
        }

        if (m_lb_x_dense->GetElementNumber() != VaribleNumber)
        {
            MDK_Error("Input lb_x Size is wrong @ LinearLeastSquaresProblemSolver::CheckInput_dense()")
            return false;
        }
    }
    else
    {
        m_lb_x_dense = &m_EmptyDenseMatrix;
    }

    if (m_ub_x_dense != nullptr)
    {
        if (m_ub_x_dense->IsColVector() == false)
        {
            MDK_Error("Input ub_x is not a column vector @ LinearLeastSquaresProblemSolver::CheckInput_dense()")
            return false;
        }

        if (m_ub_x_dense->GetElementNumber() != VaribleNumber)
        {
            MDK_Error("Input ub_x Size is wrong @ LinearLeastSquaresProblemSolver::CheckInput_dense()")
            return false;
        }
    }
    else
    {
        m_ub_x_dense = &m_EmptyDenseMatrix;
    }

    if (m_lb_x_dense->IsColVector() == true && m_ub_x_dense->IsColVector() == true)
    {
        for (int_max i = 0; i < VaribleNumber; ++i)
        {
            if ((*m_lb_x_dense)[i] >(*m_ub_x_dense)[i])
            {
                MDK_Error("lb_x and ub_x are infeasible @ LinearLeastSquaresProblemSolver::CheckInput_dense()")
                return false;
            }
        }
    }

    int_max ConstraintNumber = 0;

    if (m_A_dense != nullptr)
    {
        auto SizeA = m_A_dense->GetSize();
        if (SizeA.ColNumber != VaribleNumber)
        {
            MDK_Error("Input A size is wrong @ LinearLeastSquaresProblemSolver::CheckInput_dense()")
            return false;
        }

        ConstraintNumber = m_A_dense->GetRowNumber();

        if (m_lb_A_dense == nullptr && m_ub_A_dense == nullptr)
        {
            MDK_Error("Input lb_A and ub_A are empty (nullptr) @ LinearLeastSquaresProblemSolver::CheckInput_dense()")
            return false;
        }
    }
    else
    {
        m_A_dense = &m_EmptyDenseMatrix;
    }

    if (m_lb_A_dense != nullptr)
    {
        if (m_lb_A_dense->IsColVector() == false)
        {
            MDK_Error("Input lb_x is not a column vector @ LinearLeastSquaresProblemSolver::CheckInput_dense()")
            return false;
        }

        if (m_lb_A_dense->GetElementNumber() != ConstraintNumber)
        {
            MDK_Error("Input lb_A Size is wrong @ LinearLeastSquaresProblemSolver::CheckInput_dense()")
            return false;
        }
    }
    else
    {
        m_lb_A_dense = &m_EmptyDenseMatrix;
    }

    if (m_ub_A_dense != nullptr)
    {
        if (m_ub_A_dense->IsColVector() == false)
        {
            MDK_Error("Input ub_A is not a column vector @ LinearLeastSquaresProblemSolver::CheckInput_dense()")
            return false;
        }

        if (m_ub_A_dense->GetElementNumber() != ConstraintNumber)
        {
            MDK_Error("Input ub_A Size is wrong @ LinearLeastSquaresProblemSolver::CheckInput_dense()")
            return false;
        }
    }
    else
    {
        m_ub_A_dense = &m_EmptyDenseMatrix;
    }

    if (m_lb_A_dense->IsColVector() == true && m_ub_A_dense->IsColVector() == true)
    {
        for (int_max i = 0; i < ConstraintNumber; ++i)
        {
            if ((*m_lb_A_dense)[i] >(*m_ub_A_dense)[i])
            {
                MDK_Error("lb_A and ub_A are infeasible @ LinearLeastSquaresProblemSolver::CheckInput_dense()")
                return false;
            }
        }
    }

    if (m_x0_dense != nullptr)
    {
        if (m_x0_dense->IsColVector() == false)
        {
            MDK_Error("Input x0 is not a column vector @ LinearLeastSquaresProblemSolver::CheckInput_dense()")
            return false;
        }

        if (m_x0_dense->GetElementNumber() != VaribleNumber)
        {
            MDK_Error("Input x0 size is wrong @ LinearLeastSquaresProblemSolver::CheckInput_dense()")
            return false;
        }
    }
    else
    {
        m_x0_dense = &m_EmptyDenseMatrix;
    }

    return true;
}


template<typename ElementType>
bool LinearLeastSquaresProblemSolver<ElementType>::Update_dense_unconstrained()
{
    auto VariableNumber = m_D_dense->GetColNumber();

    m_Solution->X.FastResize(VariableNumber, 1);

    typedef Eigen::Map<Eigen::Matrix<ElementType, Eigen::Dynamic, Eigen::Dynamic>> EigenMapDynamicMatrix;

    EigenMapDynamicMatrix Input_D(const_cast<ElementType*>(m_D_dense->.GetElementPointer()), m_D_dense->.GetRowNumber(), m_D_dense->.GetColNumber());

    EigenMapDynamicMatrix Input_c(const_cast<ElementType*>(m_c_dense->GetElementPointer()), m_c_dense->GetElementNumber(), 1);

    EigenMapDynamicMatrix Output_X(m_Solution->X.GetElementPointer(), m_Solution->X.GetElementNumber(), 1);

    if (m_Option.MethodName == "SVD")
    {
        m_Solution->MethodName = "SVD";

        Output_X = Input_D.jacobiSvd(Eigen::ComputeThinU | Eigen::ComputeThinV).solve(Input_c);

    }
    else if (m_Option.MethodName == "QR")
    {
        m_Solution->MethodName = "QR";

        Output_X = Input_D.colPivHouseholderQr().solve(Input_c);

    }
    else if (m_Option.MethodName == "Normal")
    {
        m_Solution->MethodName = "Normal";

        if (m_H_dense->IsEmpty() == false)
        {
            EigenMapDynamicMatrix Input_H(const_cast<ElementType*>(m_H_dense->GetElementPointer()),
                                          m_H_dense->GetRowNumber(), m_H_dense->GetColNumber());

            Output_X = Input_H.ldlt().solve(Input_D.transpose()*Input_c);
        }
        else
        {
            Output_X = (Input_D.transpose()*Input_D).ldlt().solve(Input_D.transpose()*Input_c);
        }
    }
    else
    {
        MDK_Error("Unknonw method @ LinearLeastSquaresProblemSolver::Update_dense_unconstrained()")
        return false;
    }

    
    DenseMatrix<ElementType> Residual = (*m_D_dense) * m_Solution->X - (*m_c_dense);
    m_Solution->ObjectiveFunctionValue = 0.5*Residual.L2Norm();

    return true;
}


template<typename ElementType>
bool LinearLeastSquaresProblemSolver<ElementType>::Update_dense_constrained()
{
    if (m_H_dense->IsEmpty() == false)
    {
        DenseMatrix<ElementType> g = (*m_c_dense).Transpose() * (*m_D_dense);
        g.TransposeInPlace();

        auto qpSolution = QuadraticProgrammingSolver<ElementType>::Apply(m_H_dense, &g, m_lb_x_dense, m_ub_x_dense, m_A_dense, m_lb_A_dense, m_ub_A_dense);

        m_Solution->X = std::move(qpSolution.X);
    }
    else
    {
        DenseMatrix<ElementType> H = (*m_D_dense).Transpose() * (*m_D_dense);

        DenseMatrix<ElementType> g = (*m_c_dense).Transpose() * (*m_D_dense);
        g.TransposeInPlace();

        auto qpSolution = QuadraticProgrammingSolver<ElementType>::Apply(&H, &g, m_lb_x_dense, m_ub_x_dense, m_A_dense, m_lb_A_dense, m_ub_A_dense);

        m_Solution->X = std::move(qpSolution.X);
    }

    m_Solution->MethodName = "QuadraticProgramming";

    return true;
}



template<typename ElementType>
bool LinearLeastSquaresProblemSolver<ElementType>::CheckInput_sparse()
{
    int_max VaribleNumber = 0;

    if (m_D_sparse == nullptr)
    {
        MDK_Error("Input D is nullptr @ LinearLeastSquaresProblemSolver::CheckInput_sparse()")
        return false;
    }

    auto SizeD = m_D_sparse->GetSize();
    if (SizeD.RowNumber == 0)
    {
        MDK_Error("Input D is nempty @ LinearLeastSquaresProblemSolver::CheckInput_sparse()")
        return false;
    }

    VaribleNumber = SizeD.RowNumber;

    if (m_H_sparse != nullptr)
    {
        auto SizeH = m_H_sparse->GetSize();
        if (SizeH.RowNumber > 0 && SizeH.RowNumber != VaribleNumber)
        {
            MDK_Error("Input H size is wrong @ LinearLeastSquaresProblemSolver::CheckInput_sparse()")
            return false;
        }
    }
    else
    {
        m_H_sparse = &m_EmptySparseMatrix;
    }

    if (m_c_sparse != nullptr)
    {
        if (m_c_sparse->IsColVector() == false)
        {
            MDK_Error("Input c is not a column vector @ LinearLeastSquaresProblemSolver::CheckInput_sparse()")
            return false;
        }

        if (m_c_sparse->GetElementNumber() != VaribleNumber)
        {
            MDK_Error("Input c Size is wrong @ LinearLeastSquaresProblemSolver::CheckInput_sparse()")
            return false;
        }
    }
    else
    {
        m_c_sparse = &m_EmptySparseMatrix;
    }

    if (m_lb_x_sparse != nullptr)
    {
        if (m_lb_x_sparse->IsColVector() == false)
        {
            MDK_Error("Input lb_x is not a column vector @ LinearLeastSquaresProblemSolver::CheckInput_sparse()")
            return false;
        }

        if (m_lb_x_sparse->GetElementNumber() != VaribleNumber)
        {
            MDK_Error("Input lb_x Size is wrong @ LinearLeastSquaresProblemSolver::CheckInput_sparse()")
            return false;
        }
    }
    else
    {
        m_lb_x_sparse = &m_EmptySparseMatrix;
    }

    if (m_ub_x_sparse != nullptr)
    {
        if (m_ub_x_sparse->IsColVector() == false)
        {
            MDK_Error("Input ub_x is not a column vector @ LinearLeastSquaresProblemSolver::CheckInput_sparse()")
            return false;
        }

        if (m_ub_x_sparse->GetElementNumber() != VaribleNumber)
        {
            MDK_Error("Input ub_x Size is wrong @ LinearLeastSquaresProblemSolver::CheckInput_sparse()")
            return false;
        }
    }
    else
    {
        m_ub_x_sparse = &m_EmptySparseMatrix;
    }

    if (m_lb_x_sparse->IsColVector() == true && m_ub_x_sparse->IsColVector() == true)
    {
        for (int_max i = 0; i < VaribleNumber; ++i)
        {
            if (m_lb_x_sparse->GetElement(i) > m_ub_x_sparse->GetElement(i))
            {
                MDK_Error("lb_x and ub_x are infeasible @ LinearLeastSquaresProblemSolver::CheckInput_sparse()")
                return false;
            }
        }
    }

    int_max ConstraintNumber = 0;

    if (m_A_sparse != nullptr)
    {
        auto SizeA = m_A_sparse->GetSize();
        if (SizeA.ColNumber != VaribleNumber)
        {
            MDK_Error("Input A size is wrong @ LinearLeastSquaresProblemSolver::CheckInput_sparse()")
            return false;
        }

        ConstraintNumber = m_A_sparse->GetRowNumber();

        if (m_lb_A_sparse == nullptr && m_ub_A_sparse == nullptr)
        {
            MDK_Error("Input lb_A and ub_A are empty (nullptr) @ LinearLeastSquaresProblemSolver::CheckInput_sparse()")
            return false;
        }
    }
    else
    {
        m_A_sparse = &m_EmptySparseMatrix;
    }

    if (m_lb_A_sparse != nullptr)
    {
        if (m_lb_A_sparse->IsColVector() == false)
        {
            MDK_Error("Input lb_x is not a column vector @ LinearLeastSquaresProblemSolver::CheckInput_sparse()")
            return false;
        }

        if (m_lb_A_sparse->GetElementNumber() != ConstraintNumber)
        {
            MDK_Error("Input lb_A Size is wrong @ LinearLeastSquaresProblemSolver::CheckInput_sparse()")
            return false;
        }
    }
    else
    {
        m_lb_A_sparse = &m_EmptySparseMatrix;
    }

    if (m_ub_A_sparse != nullptr)
    {
        if (m_ub_A_sparse->IsColVector() == false)
        {
            MDK_Error("Input ub_A is not a column vector @ LinearLeastSquaresProblemSolver::CheckInput_sparse()")
            return false;
        }

        if (m_ub_A_sparse->GetElementNumber() != ConstraintNumber)
        {
            MDK_Error("Input ub_A Size is wrong @ LinearLeastSquaresProblemSolver::CheckInput_sparse()")
            return false;
        }
    }
    else
    {
        m_ub_A_sparse = &m_EmptySparseMatrix;
    }

    if (m_lb_A_sparse->IsColVector() == true && m_ub_A_sparse->IsColVector() == true)
    {
        for (int_max i = 0; i < ConstraintNumber; ++i)
        {
            if (m_lb_A_sparse->GetElement(i) > m_ub_A_sparse->GetElement(i))
            {
                MDK_Error("lb_A and ub_A are infeasible @ LinearLeastSquaresProblemSolver::CheckInput_sparse()")
                return false;
            }
        }
    }

    if (m_x0_sparse != nullptr)
    {
        if (m_x0_sparse->IsColVector() == false)
        {
            MDK_Error("Input x0 is not a column vector @ LinearLeastSquaresProblemSolver::CheckInput_sparse()")
            return false;
        }

        if (m_x0_sparse->GetElementNumber() != VaribleNumber)
        {
            MDK_Error("Input x0 size is wrong @ LinearLeastSquaresProblemSolver::CheckInput_sparse()")
            return false;
        }
    }
    else
    {
        m_x0_sparse = &m_EmptySparseMatrix;
    }

    return true;
}


template<typename ElementType>
bool LinearLeastSquaresProblemSolver<ElementType>::Update_sparse_unconstrained()
{
    return true;
}


template<typename ElementType>
bool LinearLeastSquaresProblemSolver<ElementType>::Update_sparse_constrained()
{
    if (m_H_sparse->IsEmpty() == false)
    {
        SparseMatrix<ElementType> g = (*m_c_sparse).Transpose() * (*m_D_sparse);
        g.TransposeInPlace();

        auto qpSolution = QuadraticProgrammingSolver<ElementType>::Apply(m_H_sparse, &g, m_lb_x_sparse, m_ub_x_sparse, m_A_sparse, m_lb_A_sparse, m_ub_A_sparse);

        m_Solution->X = std::move(qpSolution.X);
    }
    else
    {
        SparseMatrix<ElementType> H = (*m_D_sparse).Transpose() * (*m_D_sparse);

        SparseMatrix<ElementType> g = (*m_c_sparse).Transpose() * (*m_D_sparse);
        g.TransposeInPlace();

        auto qpSolution = QuadraticProgrammingSolver<ElementType>::Apply(&H, &g, m_lb_x_sparse, m_ub_x_sparse, m_A_sparse, m_lb_A_sparse, m_ub_A_sparse);

        m_Solution->X = std::move(qpSolution.X);
    }

    m_Solution->MethodName = "QuadraticProgramming";

    return true;
}



}//namespace mdk

#endif