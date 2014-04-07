#ifndef __mdkQuadraticProgrammingSolver_hpp
#define __mdkQuadraticProgrammingSolver_hpp

namespace mdk
{

template<typename ElementType>
QuadraticProgrammingSolver<ElementType>::QuadraticProgrammingSolver()
{
    this->Clear();
}


template<typename ElementType>
QuadraticProgrammingSolver<ElementType>::~QuadraticProgrammingSolver()
{
}


template<typename ElementType>
void QuadraticProgrammingSolver<ElementType>::Clear()
{
    m_Option.SetToDefault();

    m_H_dense = nullptr;
    m_A_dense = nullptr;

    m_H_sparse = nullptr;
    m_A_sparse = nullptr;

    m_g = nullptr;

    m_lb_x = nullptr;
    m_ub_x = nullptr;

    m_lb_A = nullptr;
    m_ub_A = nullptr;

    m_x0 = nullptr;

    m_IsInputDense = true;

    m_Solution_SharedCopy.Clear();
    m_Solution = &m_Solution_SharedCopy;

    m_QProblem_Online.reset();
    m_QProblemB_Online.reset();
    m_SQProblem_Online.reset();
}


template<typename ElementType>
void QuadraticProgrammingSolver<ElementType>::SetInputData(const DenseMatrix<ElementType>* H,                                                           
                                                           const DenseMatrix<ElementType>* g,
                                                           const DenseMatrix<ElementType>* lb_x,
                                                           const DenseMatrix<ElementType>* ub_x,
                                                           const DenseMatrix<ElementType>* A,
                                                           const DenseMatrix<ElementType>* lb_A,
                                                           const DenseMatrix<ElementType>* ub_A,
                                                           const DenseMatrix<ElementType>* x0)
{
    m_H_dense = H;
    m_H_sparse = nullptr;

    m_g = g;

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
void QuadraticProgrammingSolver<ElementType>::SetInputData(const SparseMatrix<ElementType>* H,                                                           
                                                           const DenseMatrix<ElementType>*  g,
                                                           const DenseMatrix<ElementType>*  lb_x,
                                                           const DenseMatrix<ElementType>*  ub_x,
                                                           const SparseMatrix<ElementType>* A,
                                                           const DenseMatrix<ElementType>*  lb_A,
                                                           const DenseMatrix<ElementType>*  ub_A,
                                                           const DenseMatrix<ElementType>*  x0)
{
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
bool QuadraticProgrammingSolver<ElementType>::SetOutputSolution(Solution_Of_QuadraticProgramming<ElementType>* Solution)
{
    if (Solution == nullptr)
    {
        MDK_Error("Invalid input @ QuadraticProgrammingSolver::SetOutputSolution(...)")
        return false;
    }

    m_Solution = Solution;

    m_Solution_SharedCopy.ShallowCopy(Solution);

    return true;
}


template<typename ElementType>
bool QuadraticProgrammingSolver<ElementType>::CheckInput_dense_OneTimeOnly()
{
    if (m_H_dense == nullptr)
    {
        MDK_Error("Input H is nullptr @ QuadraticProgrammingSolver::CheckInput_dense_OneTimeOnly()")
        return false;
    }

    auto SizeH = m_H_dense->GetSize();
    if (SizeH.RowNumber != SizeH.ColNumber)
    {
        MDK_Error("Input H is not symetric @ QuadraticProgrammingSolver::CheckInput_dense_OneTimeOnly()")
        return false;
    }

    auto VaribleNumber = SizeH.RowNumber;

    int_max ConstraintNumber = 0;
    if (m_A_dense != nullptr)
    {
        auto SizeA = m_A_dense->GetSize();
        if (SizeA.ColNumber != VaribleNumber)
        {
            MDK_Error("Input A size is wrong @ QuadraticProgrammingSolver::CheckInput_dense_OneTimeOnly()")
            return false;
        }

        ConstraintNumber = m_A_dense->GetRowNumber();
    }

    if (m_x0 != nullptr)
    {
        if (m_x0->IsVector() == false)
        {
            MDK_Error("Input x0 is not a vector @ QuadraticProgrammingSolver::CheckInput_dense_OneTimeOnly()")
            return false;
        }

        if (m_x0->GetElementNumber() != VaribleNumber)
        {
            MDK_Error("Input x0 size is wrong @ QuadraticProgrammingSolver::CheckInput_dense_OneTimeOnly()")
            return false;
        }
    }

    if (m_Option.MaxWSR < 0)
    {
        m_Option.MaxWSR = 5 * (VaribleNumber + ConstraintNumber);
    }

    return true;
}


template<typename ElementType>
bool QuadraticProgrammingSolver<ElementType>::Update()
{
    if (m_IsInputDense == true)
    {
        if (m_Option.IsOnlineMode == false)
        {
            return Update_dense_OneTimeOnly();
        }
        else
        {
            return Update_dense_Online();
        }
    }
    else
    {
        if (m_Option.IsOnlineMode == false)
        {
            return Update_sparse_OneTimeOnly();
        }
        else
        {
            return Update_sparse_Online();
        }
    }
}


template<typename ElementType>
bool QuadraticProgrammingSolver<ElementType>::Update_dense_OneTimeOnly()
{
    if (this->CheckInput_dense_OneTimeOnly() == false)
    {
        return false;
    }

    bool SimpleBound = false;
    if (m_A_dense == nullptr)
    {
        SimpleBound = true;
    }
    else if (m_A_dense->IsEmpty() == true)
    {
        SimpleBound = true;
    }

    if (SimpleBound == true)
    {
        auto VaribleNumber = m_H_dense->GetColNumber();

        qpOASES::QProblemB<ElementType> tempQProblemB(VaribleNumber);

        //----------------------------------------------------------------------------------------

        DenseMatrix<ElementType> H_Copy;

        if (m_Option.InputDataWillNotbeUsedByAnyOtherProcessDuringOptimization == true)
        {
            H_Copy.ForceShare(m_H_dense);
        }
        else
        {
            H_Copy.Copy(m_H_dense);
        }

        // change m_H_dense_Copy to be row major
        H_Copy.TransposeInPlace();
        H_Copy.Reshape(H_Copy.GetColNumber(), H_Copy.GetRowNumber());

        qpOASES::SymDenseMat<ElementType> temp_H(H_Copy.GetRowNumber(),
                                                 H_Copy.GetColNumber(),
                                                 H_Copy.GetColNumber(),
                                                 H_Copy.GetElementPointer());

        temp_H.print();
        //----------------------------------------------------------------------------------------
        
        int_max WSR = m_Option.MaxWSR;

        ElementType CPUTime = 0;
        ElementType* ptrCPUTime = nullptr;
        if (m_Option.CPUTime > 0.0)
        {
            CPUTime = m_Option.CPUTime;
            ptrCPUTime = &CPUTime;
        }

        tempQProblemB.setOptions(m_Option_qpOASES);

        if (m_x0 == nullptr)
        {
            tempQProblemB.init(&temp_H, m_g->GetElementPointer(), m_lb_x->GetElementPointer(), m_ub_x->GetElementPointer(), WSR, ptrCPUTime);
        }
        else
        {
            tempQProblemB.init(&temp_H, m_g->GetElementPointer(), m_lb_x->GetElementPointer(), m_ub_x->GetElementPointer(), WSR, ptrCPUTime,
                               m_x0->GetElementPointer(), nullptr, nullptr);
        }
        //-------------------------------------------------------------------------------------------------

        if (m_Option.InputDataWillNotbeUsedByAnyOtherProcessDuringOptimization == true)
        {
            H_Copy.TransposeInPlace();
        }

        //------------------------------------------------------------------------------------------

        m_Solution->WSR = WSR;
        m_Solution->CPUTime = CPUTime;

        m_Solution->X.FastResize(VaribleNumber, 1);      
        tempQProblemB.getPrimalSolution(m_Solution->X.GetElementPointer());  

        m_Solution->ObjectiveFunctionValue = tempQProblemB.getObjVal();
    }
    else
    {
        auto VaribleNumber = m_H_dense->GetRowNumber();

        auto ConstraintNumber = m_A_dense->GetRowNumber();

        qpOASES::QProblem<ElementType> tempQProblem(VaribleNumber, ConstraintNumber);

        //----------------------------------------------------------------------------------------

        DenseMatrix<ElementType> H_Copy, A_Copy;

        if (m_Option.InputDataWillNotbeUsedByAnyOtherProcessDuringOptimization == true)
        {
            H_Copy.ForceShare(m_H_dense);
            A_Copy.ForceShare(m_A_dense);
        }
        else
        {
            H_Copy.Copy(m_H_dense);
            A_Copy.Copy(m_A_dense);
        }

        H_Copy.TransposeInPlace();
        H_Copy.Reshape(H_Copy.GetColNumber(), H_Copy.GetRowNumber());

        A_Copy.TransposeInPlace();
        A_Copy.Reshape(A_Copy.GetColNumber(), A_Copy.GetRowNumber());

        qpOASES::SymDenseMat<ElementType> temp_H(H_Copy.GetRowNumber(),
                                                 H_Copy.GetColNumber(),
                                                 H_Copy.GetColNumber(),
                                                 H_Copy.GetElementPointer());

        qpOASES::DenseMatrix<ElementType> temp_A(A_Copy.GetRowNumber(),
                                                 A_Copy.GetColNumber(),
                                                 A_Copy.GetColNumber(),
                                                 A_Copy.GetElementPointer());

        temp_H.print();

        temp_A.print();
        //------------------------------------------------------------------------------------------

        int_max WSR = m_Option.MaxWSR;
       
        ElementType CPUTime = 0;
        ElementType* ptrCPUTime = nullptr;
        if (m_Option.CPUTime > 0.0)
        {
            CPUTime = m_Option.CPUTime;
            ptrCPUTime = &CPUTime;
        }

        tempQProblem.setOptions(m_Option_qpOASES);

        if (m_x0 == nullptr)
        {
            tempQProblem.init(&temp_H, m_g->GetElementPointer(), &temp_A,
                              m_lb_x->GetElementPointer(), m_ub_x->GetElementPointer(),
                              m_lb_A->GetElementPointer(), m_ub_A->GetElementPointer(),
                              WSR, ptrCPUTime);
        }
        else
        {
            tempQProblem.init(&temp_H, m_g->GetElementPointer(), &temp_A,
                              m_lb_x->GetElementPointer(), m_ub_x->GetElementPointer(),
                              m_lb_A->GetElementPointer(), m_ub_A->GetElementPointer(),
                              WSR, ptrCPUTime,
                              m_x0->GetElementPointer(), nullptr, nullptr, nullptr);
        }
        //----------------------------------------------------------------------------------------------

        if (m_Option.InputDataWillNotbeUsedByAnyOtherProcessDuringOptimization == true)
        {
            H_Copy.TransposeInPlace();
            A_Copy.TransposeInPlace();
        }

        //------------------------------------------------------------------------------------------
        m_Solution->WSR = WSR;
        m_Solution->CPUTime = CPUTime;

        m_Solution->X.FastResize(VaribleNumber, 1);
        tempQProblem.getPrimalSolution(m_Solution->X.GetElementPointer());

        m_Solution->ObjectiveFunctionValue = tempQProblem.getObjVal();
    }


    if (m_Solution != &m_Solution_SharedCopy)
    {
        m_Solution_SharedCopy.ShallowCopy(*m_Solution);
    }

    return true;
}


template<typename ElementType>
bool QuadraticProgrammingSolver<ElementType>::Update_dense_Online()
{

    return true;
}


template<typename ElementType>
bool QuadraticProgrammingSolver<ElementType>::Update_sparse_OneTimeOnly()
{

    return true;
}


template<typename ElementType>
bool QuadraticProgrammingSolver<ElementType>::Update_sparse_Online()
{

    return true;
}


template<typename ElementType>
Solution_Of_QuadraticProgramming<ElementType>* QuadraticProgrammingSolver<ElementType>::GetSolution()
{
    return &m_Solution_SharedCopy;
}


template<typename ElementType>
Solution_Of_QuadraticProgramming<ElementType> QuadraticProgrammingSolver<ElementType>::Apply(const DenseMatrix<ElementType>* H,
                                                                                             const DenseMatrix<ElementType>* g,
                                                                                             const DenseMatrix<ElementType>* lb_x,
                                                                                             const DenseMatrix<ElementType>* ub_x,
                                                                                             const DenseMatrix<ElementType>* A,
                                                                                             const DenseMatrix<ElementType>* lb_A,
                                                                                             const DenseMatrix<ElementType>* ub_A,
                                                                                             const DenseMatrix<ElementType>* x0,
                                                                                             const Option_Of_QuadraticProgramming* Option)
{
    auto Solver = std::make_unique<QuadraticProgrammingSolver<ElementType>>();

    if (Option != nullptr)
    {
        Solver->m_Option = *Option;
    }

    Solver->SetInputData(H, g, lb_x, ub_x, A, lb_A, ub_A, x0);

    Solver->Update();

    auto ptrSolution = Solver->GetSolution();

    return std::move(*ptrSolution);
}

}//namespace mdk

#endif