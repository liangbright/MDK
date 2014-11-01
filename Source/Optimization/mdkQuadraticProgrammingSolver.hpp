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

    m_H_dense    = nullptr;
    m_g_dense    = nullptr;
    m_lb_x_dense = nullptr;
    m_ub_x_dense = nullptr;
    m_A_dense    = nullptr;
    m_lb_A_dense = nullptr;
    m_ub_A_dense = nullptr;
    m_x0_dense   = nullptr;

    m_H_sparse    = nullptr;
    m_g_sparse    = nullptr;
    m_lb_x_sparse = nullptr;
    m_ub_x_sparse = nullptr;
    m_A_sparse    = nullptr;
    m_lb_A_sparse = nullptr;
    m_ub_A_sparse = nullptr;
    m_x0_sparse   = nullptr;

    m_DenseInput = false;
    m_SparseInput = false;
    m_Only_A_Sparse = false;

    m_Counter_In_Mode_Online_Varying_H_A = 0;
    m_VaribleNumber_In_Mode_Online_Varying_H_A = 0;
    m_ConstraintNumber_In_Mode_Online_Varying_H_A = 0;

    m_QProblem_Online.reset();
    m_QProblemB_Online.reset();
    m_SQProblem_Online.reset();

    m_EmptyDenseMatrix.Clear();
    m_EmptyDenseMatrix.FixSize();

    m_EmptySparseMatrix.Clear();
    m_EmptySparseMatrix.FixSize();

	m_Solution.Clear();
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
    m_H_dense    = H;
    m_g_dense    = g;
    m_lb_x_dense = lb_x;
    m_ub_x_dense = ub_x;
    m_A_dense    = A;
    m_lb_A_dense = lb_A;
    m_ub_A_dense = ub_A;
    m_x0_dense   = x0;

    m_H_sparse    = nullptr;
    m_g_sparse    = nullptr;
    m_lb_x_sparse = nullptr;
    m_ub_x_sparse = nullptr;
    m_A_sparse    = nullptr;
    m_lb_A_sparse = nullptr;
    m_ub_A_sparse = nullptr;
    m_x0_sparse   = nullptr;

    m_DenseInput = true;
}


template<typename ElementType>
void QuadraticProgrammingSolver<ElementType>::SetInputData(const SparseMatrix<ElementType>*  H,                                                           
                                                           const SparseMatrix<ElementType>*  g,
                                                           const SparseMatrix<ElementType>*  lb_x,
                                                           const SparseMatrix<ElementType>*  ub_x,
                                                           const SparseMatrix<ElementType>*  A,
                                                           const SparseMatrix<ElementType>*  lb_A,
                                                           const SparseMatrix<ElementType>*  ub_A,
                                                           const SparseMatrix<ElementType>*  x0)
{
    m_H_dense    = nullptr;
    m_g_dense    = nullptr;
    m_lb_x_dense = nullptr;
    m_ub_x_dense = nullptr;
    m_A_dense    = nullptr;
    m_lb_A_dense = nullptr;
    m_ub_A_dense = nullptr;
    m_x0_dense   = nullptr;

    m_H_sparse    = H;
    m_g_sparse    = g;
    m_lb_x_sparse = lb_x;
    m_ub_x_sparse = ub_x;
    m_A_sparse    = A;
    m_lb_A_sparse = lb_A;
    m_ub_A_sparse = ub_A;
    m_x0_sparse   = x0;

    m_SparseInput = true;
}


template<typename ElementType>
void QuadraticProgrammingSolver<ElementType>::SetInputData(const DenseMatrix<ElementType>*  H,                                                           
                                                           const DenseMatrix<ElementType>*  g,
                                                           const DenseMatrix<ElementType>*  lb_x,
                                                           const DenseMatrix<ElementType>*  ub_x,
                                                           const SparseMatrix<ElementType>* A,
                                                           const DenseMatrix<ElementType>*  lb_A,
                                                           const DenseMatrix<ElementType>*  ub_A,
                                                           const DenseMatrix<ElementType>*  x0)
{
    m_H_dense    = H;
    m_g_dense    = g;
    m_lb_x_dense = lb_x;
    m_ub_x_dense = ub_x;
    m_A_dense    = nullptr;
    m_lb_A_dense = lb_A;
    m_ub_A_dense = ub_A;
    m_x0_dense   = x0;

    m_H_sparse    = nullptr;
    m_g_sparse    = nullptr;
    m_lb_x_sparse = nullptr;
    m_ub_x_sparse = nullptr;
    m_A_sparse    = A;
    m_lb_A_sparse = nullptr;
    m_ub_A_sparse = nullptr;
    m_x0_sparse   = nullptr;

    m_Only_A_Sparse = true;
}


template<typename ElementType>
bool QuadraticProgrammingSolver<ElementType>::Update()
{
    bool IsOK = false;

    if (m_DenseInput == true)
    {
        if (m_Option.Mode == "OneTimeOnly")
        {
            IsOK = Update_Mode_OneTimeOnly_Input_ALLDense();
        }
        else if (m_Option.Mode == "Online_Varying_H_A")
        {
            IsOK = Update_Mode_Online_Varying_H_A_Input_ALLDense();
        }
        else
        {
            MDK_Error("Uknown Mode @ QuadraticProgrammingSolver::Update()")
            return false;
        }
    }
    else if (m_SparseInput == true)
    {
        if (m_Option.Mode == "OneTimeOnly")
        {
            IsOK = Update_Mode_OneTimeOnly_Input_ALLSparse();
        }
        else if (m_Option.Mode == "Online_Varying_H_A")
        {
            IsOK = Update_Mode_Online_Varying_H_A_Input_ALLSparse();
        }
        else
        {
            MDK_Error("Uknown Mode @ QuadraticProgrammingSolver::Update()")
            return false;
        }
    }
    else if (m_Only_A_Sparse == true)
    {
        if (m_Option.Mode == "OneTimeOnly")
        {
            IsOK = Update_Mode_OneTimeOnly_Input_Only_A_sparse();
        }
    }

    if (IsOK = true)
    {
        return true;
    }
    else
    {
        return false;
    }
}


template<typename ElementType>
bool QuadraticProgrammingSolver<ElementType>::CheckInput_ALLDense()
{
    int_max VaribleNumber = 0;

    if (m_H_dense == nullptr)
    {
        MDK_Error("Input H is empty (nullptr) @ QuadraticProgrammingSolver::CheckInput_ALLDense()")
        return false;
    }

    auto SizeH = m_H_dense->GetSize();
    if (SizeH.RowNumber == 0)
    {
        MDK_Error("Input H is empty @ QuadraticProgrammingSolver::CheckInput_ALLDense()")
        return false;
    }

    if (SizeH.RowNumber != SizeH.ColNumber)
    {
        MDK_Error("Input H is not symetric @ QuadraticProgrammingSolver::CheckInput_ALLDense()")
        return false;
    }

    VaribleNumber = SizeH.RowNumber;

    if (m_g_dense != nullptr)
    {
        if (m_g_dense->IsEmpty() == false)
        {
            if (m_g_dense->IsColVector() == false)
            {
                MDK_Error("Input g is not a vector @ QuadraticProgrammingSolver::CheckInput_ALLDense()")
                return false;
            }

            if (m_g_dense->GetElementNumber() != VaribleNumber)
            {
                MDK_Error("Input g Size is wrong @ QuadraticProgrammingSolver::CheckInput_ALLDense()")
                return false;
            }
        }
    }
    else
    {
        m_g_dense = &m_EmptyDenseMatrix;
    }

    if (m_lb_x_dense != nullptr)
    {
        if (m_lb_x_dense->IsEmpty() == false)
        {
            if (m_lb_x_dense->IsColVector() == false)
            {
                MDK_Error("Input lb_x is not a vector @ QuadraticProgrammingSolver::CheckInput_ALLDense()")
                return false;
            }

            if (m_lb_x_dense->GetElementNumber() != VaribleNumber)
            {
                MDK_Error("Input lb_x Size is wrong @ QuadraticProgrammingSolver::CheckInput_ALLDense()")
                return false;
            }
        }
    }
    else
    {
        m_lb_x_dense = &m_EmptyDenseMatrix;
    }

    if (m_ub_x_dense != nullptr)
    {
        if (m_ub_x_dense->IsEmpty() == false)
        {
            if (m_ub_x_dense->IsColVector() == false)
            {
                MDK_Error("Input ub_x is not a vector @ QuadraticProgrammingSolver::CheckInput_ALLDense()")
                return false;
            }

            if (m_ub_x_dense->GetElementNumber() != VaribleNumber)
            {
                MDK_Error("Input ub_x Size is wrong @ QuadraticProgrammingSolver::CheckInput_ALLDense()")
                return false;
            }
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
                MDK_Error("lb_x and ub_x are infeasible @ QuadraticProgrammingSolver::CheckInput_ALLDense()")
                return false;
            }
        }
    }

    int_max ConstraintNumber = 0;

    if (m_A_dense != nullptr)
    {
        if (m_A_dense->IsEmpty() == false)
        {
            auto SizeA = m_A_dense->GetSize();
            if (SizeA.ColNumber != VaribleNumber)
            {
                MDK_Error("Input A size is wrong @ QuadraticProgrammingSolver::CheckInput_ALLDense()")
                return false;
            }

            ConstraintNumber = m_A_dense->GetRowNumber();
        }
    }
    else
    {
        m_A_dense = &m_EmptyDenseMatrix;
    }

    if (m_lb_A_dense != nullptr)
    {
        if (m_lb_A_dense->IsEmpty() == false)
        {
            if (m_lb_A_dense->IsColVector() == false)
            {
                MDK_Error("Input lb_A is not a column vector @ QuadraticProgrammingSolver::CheckInput_ALLDense()")
                return false;
            }

            if (m_lb_A_dense->GetElementNumber() != ConstraintNumber)
            {
                MDK_Error("Input lb_A Size is wrong @ QuadraticProgrammingSolver::CheckInput_ALLDense()")
                return false;
            }
        }
    }
    else
    {
        m_lb_A_dense = &m_EmptyDenseMatrix;
    }

    if (m_ub_A_dense != nullptr)
    {
        if (m_ub_A_dense->IsEmpty() == false)
        {
            if (m_ub_A_dense->IsColVector() == false)
            {
                MDK_Error("Input ub_A is not a column vector @ QuadraticProgrammingSolver::CheckInput_ALLDense()")
                return false;
            }

            if (m_ub_A_dense->GetElementNumber() != ConstraintNumber)
            {
                MDK_Error("Input ub_A Size is wrong @ QuadraticProgrammingSolver::CheckInput_ALLDense()")
                return false;
            }
        }
    }
    else
    {
        m_ub_A_dense = &m_EmptyDenseMatrix;
    }

    if (m_A_dense->IsEmpty() == false && m_lb_A_dense->IsEmpty() == true && m_ub_A_dense->IsEmpty() == true)
    {
        MDK_Error("Input lb_A and ub_A Size are empty but A is not empty @ QuadraticProgrammingSolver::CheckInput_ALLDense()")
        return false;
    }

    if (m_lb_A_dense->IsColVector() == true && m_ub_A_dense->IsColVector() == true)
    {
        for (int_max i = 0; i < ConstraintNumber; ++i)
        {
            if ((*m_lb_A_dense)[i] >(*m_ub_A_dense)[i])
            {
                MDK_Error("lb_A and ub_A are infeasible @ QuadraticProgrammingSolver::CheckInput_ALLDense()")
                return false;
            }
        }
    }

    if (m_x0_dense != nullptr)
    {
        if (m_x0_dense->IsEmpty() == false)
        {
            if (m_x0_dense->IsColVector() == false)
            {
                MDK_Error("Input x0 is not a column vector @ QuadraticProgrammingSolver::CheckInput_ALLDense()")
                return false;
            }

            if (m_x0_dense->GetElementNumber() != VaribleNumber)
            {
                MDK_Error("Input x0 size is wrong @ QuadraticProgrammingSolver::CheckInput_ALLDense()")
                return false;
            }
        }
    }
    else
    {
        m_x0_dense = &m_EmptyDenseMatrix;
    }

    if (m_Option.MaxIterNumber <= 0)
    {
        m_Option.MaxIterNumber = 5 * (VaribleNumber + ConstraintNumber);
    }

    return true;
}


template<typename ElementType>
bool QuadraticProgrammingSolver<ElementType>::Update_Mode_OneTimeOnly_Input_ALLDense()
{
    if (this->CheckInput_ALLDense() == false)
    {
        return false;
    }

    //---------------------------------------------------------

    switch (m_Option.MessageOutputOption)
    {
    case MessageOutputOption_Of_QuadraticProgramming::Nothing:

        m_Option_qpOASES.printLevel = qpOASES::PL_NONE;
        break;

    case MessageOutputOption_Of_QuadraticProgramming::ErrorMessageOnly:

        m_Option_qpOASES.printLevel = qpOASES::PL_LOW;
        break;

    case MessageOutputOption_Of_QuadraticProgramming::ALLMessage:

        m_Option_qpOASES.printLevel = qpOASES::PL_HIGH;
        break;
    }

    //---------------------------------------------------------

    bool SimpleBound = false;
    if (m_A_dense->IsEmpty() == true)
    {
        SimpleBound = true;
    }

    if (SimpleBound == true)
    {
        auto VaribleNumber = m_H_dense->GetColNumber();

        qpOASES::QProblemB<ElementType> tempQProblemB(VaribleNumber);

        tempQProblemB.setOptions(m_Option_qpOASES);

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

        qpOASES::SymDenseMat<ElementType> temp_H(H_Copy.GetRowNumber(), H_Copy.GetColNumber(), H_Copy.GetColNumber(), H_Copy.GetElementPointer());

        //----------------------------------------------------------------------------------------
        
        int_max WSR = m_Option.MaxIterNumber;

        ElementType CPUTime = 0;
        ElementType* ptrCPUTime = nullptr;
        if (m_Option.CPUTime > 0.0)
        {
            CPUTime = ElementType(m_Option.CPUTime);
            ptrCPUTime = &CPUTime;
        }

        if (m_x0_dense->IsEmpty() == true)
        {
            tempQProblemB.init(&temp_H, m_g_dense->GetElementPointer(), m_lb_x_dense->GetElementPointer(), m_ub_x_dense->GetElementPointer(), WSR, ptrCPUTime);
        }
        else
        {
            tempQProblemB.init(&temp_H, m_g_dense->GetElementPointer(), m_lb_x_dense->GetElementPointer(), m_ub_x_dense->GetElementPointer(), WSR, ptrCPUTime,
                               m_x0_dense->GetElementPointer(), nullptr, nullptr);
        }
        //-------------------------------------------------------------------------------------------------

        if (m_Option.InputDataWillNotbeUsedByAnyOtherProcessDuringOptimization == true)
        {
            H_Copy.TransposeInPlace();
        }

        //------------------------------------------------------------------------------------------

        m_Solution.WSR = WSR;
        m_Solution.CPUTime = CPUTime;

        m_Solution.X.FastResize(VaribleNumber, 1);      
        tempQProblemB.getPrimalSolution(m_Solution.X.GetElementPointer());  

        m_Solution.ObjectiveFunctionValue = tempQProblemB.getObjVal();
    }
    else
    {
        auto VaribleNumber = m_H_dense->GetRowNumber();

        auto ConstraintNumber = m_A_dense->GetRowNumber();

        qpOASES::QProblem<ElementType> tempQProblem(VaribleNumber, ConstraintNumber);

        tempQProblem.setOptions(m_Option_qpOASES);

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

        qpOASES::SymDenseMat<ElementType> temp_H(H_Copy.GetRowNumber(), H_Copy.GetColNumber(), H_Copy.GetColNumber(), H_Copy.GetElementPointer());

        qpOASES::DenseMatrix<ElementType> temp_A(A_Copy.GetRowNumber(), A_Copy.GetColNumber(), A_Copy.GetColNumber(), A_Copy.GetElementPointer());
        //------------------------------------------------------------------------------------------

        int_max WSR = m_Option.MaxIterNumber;
       
        ElementType CPUTime = 0;
        ElementType* ptrCPUTime = nullptr;
        if (m_Option.CPUTime > 0.0)
        {
            CPUTime = ElementType(m_Option.CPUTime);
            ptrCPUTime = &CPUTime;
        }       

        if (m_x0_dense->IsEmpty() == true)
        {
            tempQProblem.init(&temp_H, m_g_dense->GetElementPointer(), &temp_A,
                              m_lb_x_dense->GetElementPointer(), m_ub_x_dense->GetElementPointer(),
                              m_lb_A_dense->GetElementPointer(), m_ub_A_dense->GetElementPointer(),
                              WSR, ptrCPUTime);
        }
        else
        {
            tempQProblem.init(&temp_H, m_g_dense->GetElementPointer(), &temp_A,
                              m_lb_x_dense->GetElementPointer(), m_ub_x_dense->GetElementPointer(),
                              m_lb_A_dense->GetElementPointer(), m_ub_A_dense->GetElementPointer(),
                              WSR, ptrCPUTime,
                              m_x0_dense->GetElementPointer(), nullptr, nullptr, nullptr);
        }
        //----------------------------------------------------------------------------------------------

        if (m_Option.InputDataWillNotbeUsedByAnyOtherProcessDuringOptimization == true)
        {
            H_Copy.TransposeInPlace();
            A_Copy.TransposeInPlace();
        }

        //------------------------------------------------------------------------------------------
        m_Solution.WSR = WSR;
        m_Solution.CPUTime = CPUTime;

        m_Solution.X.FastResize(VaribleNumber, 1);
        tempQProblem.getPrimalSolution(m_Solution.X.GetElementPointer());

        m_Solution.ObjectiveFunctionValue = tempQProblem.getObjVal();
    }

    return true;
}


template<typename ElementType>
bool QuadraticProgrammingSolver<ElementType>::CheckInput_ALLSparse()
{
    int_max VaribleNumber = 0;

    if (m_H_sparse == nullptr)
    {
        MDK_Error("Input H is empty (nullptr) @ QuadraticProgrammingSolver::CheckInput_ALLSparse()")
        return false;
    }

    auto SizeH = m_H_sparse->GetSize();
    if (SizeH.RowNumber == 0)
    {
        MDK_Error("Input H is empty @ QuadraticProgrammingSolver::CheckInput_ALLSparse()")
        return false;
    }

    if (SizeH.RowNumber != SizeH.ColNumber)
    {
        MDK_Error("Input H is not symetric @ QuadraticProgrammingSolver::CheckInput_ALLSparse()")
        return false;
    }

    VaribleNumber = SizeH.RowNumber;

    if (m_g_sparse != nullptr)
    {
        if (m_g_sparse->IsEmpty() == false)
        {
            if (m_g_sparse->IsColVector() == false)
            {
                MDK_Error("Input g is not a vector @ QuadraticProgrammingSolver::CheckInput_ALLSparse()")
                return false;
            }

            if (m_g_sparse->GetElementNumber() != VaribleNumber)
            {
                MDK_Error("Input g Size is wrong @ QuadraticProgrammingSolver::CheckInput_ALLSparse()")
                return false;
            }
        }
    }
    else
    {
        m_g_sparse = &m_EmptySparseMatrix;
    }

    if (m_lb_x_sparse != nullptr)
    {
        if (m_lb_x_sparse->IsEmpty() == false)
        {
            if (m_lb_x_sparse->IsColVector() == false)
            {
                MDK_Error("Input lb_x is not a vector @ QuadraticProgrammingSolver::CheckInput_ALLSparse()")
                return false;
            }

            if (m_lb_x_sparse->GetElementNumber() != VaribleNumber)
            {
                MDK_Error("Input lb_x Size is wrong @ QuadraticProgrammingSolver::CheckInput_ALLSparse()")
                return false;
            }
        }
    }
    else
    {
        m_lb_x_sparse = &m_EmptySparseMatrix;
    }

    if (m_ub_x_sparse != nullptr)
    {
        if (m_ub_x_sparse->IsEmpty() == false)
        {
            if (m_ub_x_sparse->IsColVector() == false)
            {
                MDK_Error("Input ub_x is not a vector @ QuadraticProgrammingSolver::CheckInput_ALLSparse()")
                return false;
            }

            if (m_ub_x_sparse->GetElementNumber() != VaribleNumber)
            {
                MDK_Error("Input ub_x Size is wrong @ QuadraticProgrammingSolver::CheckInput_ALLSparse()")
                return false;
            }
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
                MDK_Error("lb_x and ub_x are infeasible @ QuadraticProgrammingSolver::CheckInput_ALLSparse()")
                return false;
            }
        }
    }

    int_max ConstraintNumber = 0;

    if (m_A_sparse != nullptr)
    {
        if (m_A_sparse->IsEmpty() == false)
        {
            auto SizeA = m_A_sparse->GetSize();
            if (SizeA.ColNumber != VaribleNumber)
            {
                MDK_Error("Input A size is wrong @ QuadraticProgrammingSolver::CheckInput_ALLSparse()")
                return false;
            }

            ConstraintNumber = m_A_sparse->GetRowNumber();
        }
    }
    else
    {
        m_A_sparse = &m_EmptySparseMatrix;
    }

    if (m_lb_A_sparse != nullptr)
    {
        if (m_lb_A_sparse->IsEmpty() == false)
        {
            if (m_lb_A_sparse->IsColVector() == false)
            {
                MDK_Error("Input lb_x is not a vector @ QuadraticProgrammingSolver::CheckInput_ALLSparse()")
                return false;
            }

            if (m_lb_A_sparse->GetElementNumber() != ConstraintNumber)
            {
                MDK_Error("Input lb_A Size is wrong @ QuadraticProgrammingSolver::CheckInput_ALLSparse()")
                return false;
            }
        }
    }
    else
    {
        m_lb_A_sparse = &m_EmptySparseMatrix;
    }

    if (m_ub_A_sparse != nullptr)
    {
        if (m_ub_A_sparse->IsEmpty() == false)
        {
            if (m_ub_A_sparse->IsColVector() == false)
            {
                MDK_Error("Input ub_A is not a vector @ QuadraticProgrammingSolver::CheckInput_ALLSparse()")
                return false;
            }

            if (m_ub_A_sparse->GetElementNumber() != ConstraintNumber)
            {
                MDK_Error("Input ub_A Size is wrong @ QuadraticProgrammingSolver::CheckInput_ALLSparse()")
                return false;
            }
        }
    }
    else
    {
        m_ub_A_sparse = &m_EmptySparseMatrix;
    }

    if (m_A_sparse->IsEmpty() == false && m_lb_A_sparse->IsEmpty() == true && m_ub_A_sparse->IsEmpty() == true)
    {
        MDK_Error("Input lb_A and ub_A Size are empty but A is not empty @ QuadraticProgrammingSolver::CheckInput_ALLSparse()")
        return false;
    }

    if (m_lb_A_sparse->IsColVector() == true && m_ub_A_sparse->IsColVector() == true)
    {
        for (int_max i = 0; i < ConstraintNumber; ++i)
        {
            if (m_lb_A_sparse->GetElement(i) > m_ub_A_sparse->GetElement(i))
            {
                MDK_Error("lb_A and ub_A are infeasible @ QuadraticProgrammingSolver::CheckInput_ALLSparse()")
                return false;
            }
        }
    }

    if (m_x0_sparse != nullptr)
    {
        if (m_x0_sparse->IsEmpty() == false)
        {
            if (m_x0_sparse->IsColVector() == false)
            {
                MDK_Error("Input x0 is not a vector @ QuadraticProgrammingSolver::CheckInput_ALLSparse()")
                return false;
            }

            if (m_x0_sparse->GetElementNumber() != VaribleNumber)
            {
                MDK_Error("Input x0 size is wrong @ QuadraticProgrammingSolver::CheckInput_ALLSparse()")
                return false;
            }
        }
    }
    else
    {
        m_x0_sparse = &m_EmptySparseMatrix;
    }

    if (m_Option.MaxIterNumber < 0)
    {
        m_Option.MaxIterNumber = 5 * (VaribleNumber + ConstraintNumber);
    }

    return true;
}


template<typename ElementType>
bool QuadraticProgrammingSolver<ElementType>::Update_Mode_OneTimeOnly_Input_ALLSparse()
{
    if (this->CheckInput_ALLSparse() == false)
    {
        return false;
    }

    bool SimpleBound = false;
    if (m_A_dense->IsEmpty() == true)
    {
        SimpleBound = true;
    }

    if (SimpleBound == true)
    {
        auto VaribleNumber = m_H_dense->GetColNumber();

        qpOASES::QProblemB<ElementType> tempQProblemB(VaribleNumber);

        //----------------------------------------------------------------------------------------

        qpOASES::SymSparseMat<ElementType> temp_H(m_H_sparse->GetRowNumber(),
                                                  m_H_sparse->GetColNumber(),
                                                  const_cast<qpOASES::sparse_int_t*>(m_H_sparse->GetPointerOfRowIndexList()),
                                                  const_cast<qpOASES::sparse_int_t*>(m_H_sparse->GetPointerOfColBeginElementIndexInElementList()),
                                                  const_cast<ElementType*>(m_H_sparse->GetPointerOfElementList())
                                                  );

        DenseMatrix<ElementType> g;
        ConvertSparseMatrixToDenseMatrix(*m_g_sparse, g);

        DenseMatrix<ElementType> lb_x;
        ConvertSparseMatrixToDenseMatrix(*m_lb_x_sparse, lb_x);

        DenseMatrix<ElementType> ub_x;
        ConvertSparseMatrixToDenseMatrix(*m_ub_x_sparse, ub_x);

        //----------------------------------------------------------------------------------------

        int_max WSR = m_Option.MaxIterNumber;

        ElementType CPUTime = 0;
        ElementType* ptrCPUTime = nullptr;
        if (m_Option.CPUTime > 0.0)
        {
            CPUTime = ElementType(m_Option.CPUTime);
            ptrCPUTime = &CPUTime;
        }

        tempQProblemB.setOptions(m_Option_qpOASES);

        if (m_x0_sparse->IsEmpty() == true)
        {           
            tempQProblemB.init(&temp_H, g.GetElementPointer(), lb_x.GetElementPointer(), ub_x.GetElementPointer(), WSR, ptrCPUTime);
        }
        else
        {
            DenseMatrix<ElementType> x0;
            ConvertSparseMatrixToDenseMatrix(*m_x0_sparse, x0);

            tempQProblemB.init(&temp_H, g.GetElementPointer(), lb_x.GetElementPointer(), ub_x.GetElementPointer(), WSR, ptrCPUTime,
                               x0.GetElementPointer(), nullptr, nullptr);
        }

        //------------------------------------------------------------------------------------------

        m_Solution.WSR = WSR;
        m_Solution.CPUTime = CPUTime;

        m_Solution.X.FastResize(VaribleNumber, 1);
        tempQProblemB.getPrimalSolution(m_Solution.X.GetElementPointer());

        m_Solution.ObjectiveFunctionValue = tempQProblemB.getObjVal();
    }
    else
    {
        auto VaribleNumber = m_H_dense->GetRowNumber();

        auto ConstraintNumber = m_A_dense->GetRowNumber();

        qpOASES::QProblem<ElementType> tempQProblem(VaribleNumber, ConstraintNumber);

        //----------------------------------------------------------------------------------------

        qpOASES::SymSparseMat<ElementType> temp_H(m_H_sparse->GetRowNumber(),
                                                  m_H_sparse->GetColNumber(),
                                                  const_cast<qpOASES::sparse_int_t*>(m_H_sparse->GetPointerOfRowIndexList()),
                                                  const_cast<qpOASES::sparse_int_t*>(m_H_sparse->GetPointerOfColBeginElementIndexInElementList()),
                                                  const_cast<ElementType*>(m_H_sparse->GetPointerOfElementList())
                                                  );

        qpOASES::SymSparseMat<ElementType> temp_A(m_A_sparse->GetRowNumber(),
                                                  m_A_sparse->GetColNumber(),
                                                  const_cast<qpOASES::sparse_int_t*>(m_A_sparse->GetPointerOfRowIndexList()),
												  const_cast<qpOASES::sparse_int_t*>(m_A_sparse->GetPointerOfColBeginElementIndexInElementList()),
												  const_cast<ElementType*>(m_A_sparse->GetPointerOfElementList())
                                                  );

        DenseMatrix<ElementType> g;
        ConvertSparseMatrixToDenseMatrix(*m_g_sparse, g);

        DenseMatrix<ElementType> lb_x;
        ConvertSparseMatrixToDenseMatrix(*m_lb_x_sparse, lb_x);

        DenseMatrix<ElementType> ub_x;
        ConvertSparseMatrixToDenseMatrix(*m_ub_x_sparse, ub_x);

        DenseMatrix<ElementType> lb_A;
        ConvertSparseMatrixToDenseMatrix(*m_lb_A_sparse, lb_A);

        DenseMatrix<ElementType> ub_A;
        ConvertSparseMatrixToDenseMatrix(*m_ub_A_sparse, ub_A);
        //------------------------------------------------------------------------------------------

        int_max WSR = m_Option.MaxIterNumber;

        ElementType CPUTime = 0;
        ElementType* ptrCPUTime = nullptr;
        if (m_Option.CPUTime > 0.0)
        {
            CPUTime = ElementType(m_Option.CPUTime);
            ptrCPUTime = &CPUTime;
        }

        tempQProblem.setOptions(m_Option_qpOASES);

        if (m_x0_sparse->IsEmpty() == true)
        {
            tempQProblem.init(&temp_H, g.GetElementPointer(), &temp_A,
                              lb_x.GetElementPointer(), ub_x.GetElementPointer(),
                              lb_A.GetElementPointer(), ub_A.GetElementPointer(),
                              WSR, ptrCPUTime);
        }
        else
        {
            DenseMatrix<ElementType> x0;
            ConvertSparseMatrixToDenseMatrix(*m_x0_sparse, x0);

            tempQProblem.init(&temp_H, g.GetElementPointer(), &temp_A,
                              lb_x.GetElementPointer(), ub_x.GetElementPointer(),
                              lb_A.GetElementPointer(), ub_A.GetElementPointer(),
                              WSR, ptrCPUTime,
                              x0.GetElementPointer(), nullptr, nullptr, nullptr);
        }
        //------------------------------------------------------------------------------------------
        m_Solution.WSR = WSR;
        m_Solution.CPUTime = CPUTime;

        m_Solution.X.FastResize(VaribleNumber, 1);
        tempQProblem.getPrimalSolution(m_Solution.X.GetElementPointer());

        m_Solution.ObjectiveFunctionValue = tempQProblem.getObjVal();
    }

    return true;
}


template<typename ElementType>
bool QuadraticProgrammingSolver<ElementType>::CheckInput_Only_A_sparse()
{
    if (this->CheckInput_ALLDense() == false)
    {
        return false;
    }

    auto VaribleNumber = m_H_dense->GetColNumber();

    if (m_A_sparse != nullptr)
    {
        if (m_A_sparse->IsEmpty() == false)
        {
            auto SizeA = m_A_sparse->GetSize();
            if (SizeA.ColNumber != VaribleNumber)
            {
                MDK_Error("Input A size is wrong @ QuadraticProgrammingSolver::CheckInput_Only_A_sparse()")
                return false;
            }
        }
    }
    else
    {
        m_A_sparse = &m_EmptySparseMatrix;
    }

    if (m_A_sparse->IsEmpty() == false && m_lb_A_dense->IsEmpty() == true && m_ub_A_dense->IsEmpty() == true)
    {
        MDK_Error("Input lb_A and ub_A Size are empty but A is not empty @ QuadraticProgrammingSolver::CheckInput_Only_A_sparse()")
        return false;
    }

    return true;
}


template<typename ElementType>
bool QuadraticProgrammingSolver<ElementType>::Update_Mode_OneTimeOnly_Input_Only_A_sparse()
{
    if (this->CheckInput_Only_A_sparse() == false)
    {
        return false;
    }

    bool SimpleBound = false;
    if (m_A_sparse->IsEmpty() == true)
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

        qpOASES::SymDenseMat<ElementType> temp_H(H_Copy.GetRowNumber(), H_Copy.GetColNumber(), H_Copy.GetColNumber(), H_Copy.GetElementPointer());

        //----------------------------------------------------------------------------------------

        int_max WSR = m_Option.MaxIterNumber;

        ElementType CPUTime = 0;
        ElementType* ptrCPUTime = nullptr;
        if (m_Option.CPUTime > 0.0)
        {
            CPUTime = ElementType(m_Option.CPUTime);
            ptrCPUTime = &CPUTime;
        }

        tempQProblemB.setOptions(m_Option_qpOASES);

        if (m_x0_dense->IsEmpty() == true)
        {
            tempQProblemB.init(&temp_H, m_g_dense->GetElementPointer(), m_lb_x_dense->GetElementPointer(), m_ub_x_dense->GetElementPointer(), WSR, ptrCPUTime);
        }
        else
        {
            tempQProblemB.init(&temp_H, m_g_dense->GetElementPointer(), m_lb_x_dense->GetElementPointer(), m_ub_x_dense->GetElementPointer(), WSR, ptrCPUTime,
                               m_x0_dense->GetElementPointer(), nullptr, nullptr);
        }
        //-------------------------------------------------------------------------------------------------

        if (m_Option.InputDataWillNotbeUsedByAnyOtherProcessDuringOptimization == true)
        {
            H_Copy.TransposeInPlace();
        }

        //------------------------------------------------------------------------------------------

        m_Solution.WSR = WSR;
        m_Solution.CPUTime = CPUTime;

        m_Solution.X.FastResize(VaribleNumber, 1);
        tempQProblemB.getPrimalSolution(m_Solution.X.GetElementPointer());

        m_Solution.ObjectiveFunctionValue = tempQProblemB.getObjVal();
    }
    else
    {
        auto VaribleNumber = m_H_dense->GetRowNumber();

        auto ConstraintNumber = m_A_dense->GetRowNumber();

        qpOASES::QProblem<ElementType> tempQProblem(VaribleNumber, ConstraintNumber);

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

        H_Copy.TransposeInPlace();
        H_Copy.Reshape(H_Copy.GetColNumber(), H_Copy.GetRowNumber());

        qpOASES::SymDenseMat<ElementType> temp_H(H_Copy.GetRowNumber(), H_Copy.GetColNumber(), H_Copy.GetColNumber(), H_Copy.GetElementPointer());

        qpOASES::SymSparseMat<ElementType> temp_A(m_A_sparse->GetRowNumber(),
                                                  m_A_sparse->GetColNumber(),
                                                  const_cast<qpOASES::sparse_int_t*>(m_A_sparse->GetPointerOfRowIndexList()),
                                                  const_cast<qpOASES::sparse_int_t*>(m_A_sparse->GetPointerOfColBeginElementIndexInElementList()),
                                                  const_cast<ElementType*>(m_A_sparse->GetPointerOfElementList())
                                                  );

        //------------------------------------------------------------------------------------------

        int_max WSR = m_Option.MaxIterNumber;

        ElementType CPUTime = 0;
        ElementType* ptrCPUTime = nullptr;
        if (m_Option.CPUTime > 0.0)
        {
            CPUTime = ElementType(m_Option.CPUTime);
            ptrCPUTime = &CPUTime;
        }

        tempQProblem.setOptions(m_Option_qpOASES);

        if (m_x0_dense->IsEmpty() == true)
        {
            tempQProblem.init(&temp_H, m_g_dense->GetElementPointer(), &temp_A,
                              m_lb_x_dense->GetElementPointer(), m_ub_x_dense->GetElementPointer(),
                              m_lb_A_dense->GetElementPointer(), m_ub_A_dense->GetElementPointer(),
                              WSR, ptrCPUTime);
        }
        else
        {
            tempQProblem.init(&temp_H, m_g_dense->GetElementPointer(), &temp_A,
                              m_lb_x_dense->GetElementPointer(), m_ub_x_dense->GetElementPointer(),
                              m_lb_A_dense->GetElementPointer(), m_ub_A_dense->GetElementPointer(),
                              WSR, ptrCPUTime,
                              m_x0_dense->GetElementPointer(), nullptr, nullptr, nullptr);
        }
        //----------------------------------------------------------------------------------------------

        if (m_Option.InputDataWillNotbeUsedByAnyOtherProcessDuringOptimization == true)
        {
            H_Copy.TransposeInPlace();
        }

        //------------------------------------------------------------------------------------------
        m_Solution.WSR = WSR;
        m_Solution.CPUTime = CPUTime;

        m_Solution.X.FastResize(VaribleNumber, 1);
        tempQProblem.getPrimalSolution(m_Solution.X.GetElementPointer());

        m_Solution.ObjectiveFunctionValue = tempQProblem.getObjVal();
    }

    return true;
}


template<typename ElementType>
bool QuadraticProgrammingSolver<ElementType>::CheckInput_ALLDense_Mode_Online_Varying_H_A()
{
    if (this->CheckInput_ALLDense() == false)
    {
        return false;
    }

    auto VaribleNumber = m_H_dense->GetRowNumber();

    if (m_Counter_In_Mode_Online_Varying_H_A > 0)
    {
        if (VaribleNumber != m_VaribleNumber_In_Mode_Online_Varying_H_A)
        {
            MDK_Error("H Size can not change in Mode Online_Varying_H_A @ QuadraticProgrammingSolver::CheckInput_ALLDense_Mode_Online_Varying_H_A()")
            return false;
        }
    }

    auto ConstraintNumber = m_A_dense->GetRowNumber();

    if (m_Counter_In_Mode_Online_Varying_H_A > 0)
    {
        if (ConstraintNumber != m_ConstraintNumber_In_Mode_Online_Varying_H_A)
        {
            MDK_Error("A Size can not change in Mode Online_Varying_H_A @ QuadraticProgrammingSolver::CheckInput_ALLDense_Mode_Online_Varying_H_A()")
            return false;
        }
    }

    return true;
}


template<typename ElementType>
bool QuadraticProgrammingSolver<ElementType>::Update_Mode_Online_Varying_H_A_Input_ALLDense()
{
    if (this->CheckInput_ALLDense_Mode_Online_Varying_H_A() == false)
    {
        return false;
    }

    //----------------------------------------------------------------------------------------
    m_Counter_In_Mode_Online_Varying_H_A += 1;
    //----------------------------------------------------------------------------------------

    if (m_Counter_In_Mode_Online_Varying_H_A == 1)
    {
        m_VaribleNumber_In_Mode_Online_Varying_H_A = m_H_dense->GetRowNumber();

        m_ConstraintNumber_In_Mode_Online_Varying_H_A = m_A_dense->GetRowNumber();

        m_SQProblem_Online = std::move(std::make_unique<qpOASES::SQProblem<ElementType>>(m_VaribleNumber_In_Mode_Online_Varying_H_A,
                                                                                         m_ConstraintNumber_In_Mode_Online_Varying_H_A));

        m_SQProblem_Online->setOptions(m_Option_qpOASES);
    }
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

    qpOASES::SymDenseMat<ElementType> temp_H(H_Copy.GetRowNumber(), H_Copy.GetColNumber(), H_Copy.GetColNumber(), H_Copy.GetElementPointer());

    qpOASES::DenseMatrix<ElementType> temp_A(A_Copy.GetRowNumber(), A_Copy.GetColNumber(), A_Copy.GetColNumber(), A_Copy.GetElementPointer());
    //------------------------------------------------------------------------------------------

    int_max WSR = m_Option.MaxIterNumber;

    ElementType CPUTime = 0;
    ElementType* ptrCPUTime = nullptr;
    if (m_Option.CPUTime > 0.0)
    {
        CPUTime = ElementType(m_Option.CPUTime);
        ptrCPUTime = &CPUTime;
    }

    if (m_Counter_In_Mode_Online_Varying_H_A == 1)
    {
        m_SQProblem_Online->init(&temp_H, m_g_dense->GetElementPointer(), &temp_A,
                                 m_lb_x_dense->GetElementPointer(), m_ub_x_dense->GetElementPointer(),
                                 m_lb_A_dense->GetElementPointer(), m_ub_A_dense->GetElementPointer(),
                                 WSR, ptrCPUTime);
    }
    else
    {
        m_SQProblem_Online->hotstart(&temp_H, m_g_dense->GetElementPointer(), &temp_A,
                                     m_lb_x_dense->GetElementPointer(), m_ub_x_dense->GetElementPointer(),
                                     m_lb_A_dense->GetElementPointer(), m_ub_A_dense->GetElementPointer(),
                                     WSR, ptrCPUTime);
    }
    //----------------------------------------------------------------------------------------------

    if (m_Option.InputDataWillNotbeUsedByAnyOtherProcessDuringOptimization == true)
    {
        H_Copy.TransposeInPlace();
        A_Copy.TransposeInPlace();
    }

    //------------------------------------------------------------------------------------------
    m_Solution.WSR = WSR;
    m_Solution.CPUTime = CPUTime;

    m_Solution.X.FastResize(m_VaribleNumber_In_Mode_Online_Varying_H_A, 1);
    m_SQProblem_Online->getPrimalSolution(m_Solution.X.GetElementPointer());

    m_Solution.ObjectiveFunctionValue = m_SQProblem_Online->getObjVal();

    return true;
}


template<typename ElementType>
bool QuadraticProgrammingSolver<ElementType>::CheckInput_ALLSparse_Mode_Online_Varying_H_A()
{
    if (this->CheckInput_ALLSparse() == false)
    {
        return false;
    }

    auto VaribleNumber = m_H_sparse->GetRowNumber();

    if (m_Counter_In_Mode_Online_Varying_H_A > 0)
    {
        if (VaribleNumber != m_VaribleNumber_In_Mode_Online_Varying_H_A)
        {
            MDK_Error("H Size can not change in Mode Online_Varying_H_A @ QuadraticProgrammingSolver::CheckInput_ALLSparse_Mode_Online_Varying_H_A()")
            return false;
        }
    }

    auto ConstraintNumber = m_A_sparse->GetRowNumber();

    if (m_Counter_In_Mode_Online_Varying_H_A > 0)
    {
        if (ConstraintNumber != m_ConstraintNumber_In_Mode_Online_Varying_H_A)
        {
            MDK_Error("A Size can not change in Mode Online_Varying_H_A @ QuadraticProgrammingSolver::CheckInput_ALLSparse_Mode_Online_Varying_H_A()")
            return false;
        }
    }

    return true;
}


template<typename ElementType>
bool QuadraticProgrammingSolver<ElementType>::Update_Mode_Online_Varying_H_A_Input_ALLSparse()
{
    if (this->CheckInput_ALLSparse_Mode_Online_Varying_H_A() == false)
    {
        return false;
    }

    //----------------------------------------------------------------------------------------
    m_Counter_In_Mode_Online_Varying_H_A += 1;
    //----------------------------------------------------------------------------------------

    if (m_Counter_In_Mode_Online_Varying_H_A == 1)
    {
        m_VaribleNumber_In_Mode_Online_Varying_H_A = m_H_sparse->GetRowNumber();

        m_ConstraintNumber_In_Mode_Online_Varying_H_A = m_A_sparse->GetRowNumber();

        m_SQProblem_Online = std::move(std::make_unique<qpOASES::SQProblem<ElementType>>(m_VaribleNumber_In_Mode_Online_Varying_H_A,
                                                                                         m_ConstraintNumber_In_Mode_Online_Varying_H_A));

        m_SQProblem_Online->setOptions(m_Option_qpOASES);
    }
    //----------------------------------------------------------------------------------------

    qpOASES::SymSparseMat<ElementType> temp_H(m_H_sparse->GetRowNumber(),
                                              m_H_sparse->GetColNumber(),
                                              const_cast<qpOASES::sparse_int_t*>(m_H_sparse->GetPointerOfRowIndexList()),
                                              const_cast<qpOASES::sparse_int_t*>(m_H_sparse->GetPointerOfColBeginElementIndexInElementList()),
                                              const_cast<ElementType*>(m_H_sparse->GetPointerOfElementList())
                                              );

    qpOASES::SymSparseMat<ElementType> temp_A(m_A_sparse->GetRowNumber(),
                                              m_A_sparse->GetColNumber(),
                                              const_cast<qpOASES::sparse_int_t*>(m_A_sparse->GetPointerOfRowIndexList()),
											  const_cast<qpOASES::sparse_int_t*>(m_A_sparse->GetPointerOfColBeginElementIndexInElementList()),
											  const_cast<ElementType*>(m_A_sparse->GetPointerOfElementList())
                                              );

    DenseMatrix<ElementType> g;
    ConvertSparseMatrixToDenseMatrix(*m_g_sparse, g);

    DenseMatrix<ElementType> lb_x;
    ConvertSparseMatrixToDenseMatrix(*m_lb_x_sparse, lb_x);

    DenseMatrix<ElementType> ub_x;
    ConvertSparseMatrixToDenseMatrix(*m_ub_x_sparse, ub_x);

    DenseMatrix<ElementType> lb_A;
    ConvertSparseMatrixToDenseMatrix(*m_lb_A_sparse, lb_A);

    DenseMatrix<ElementType> ub_A;
    ConvertSparseMatrixToDenseMatrix(*m_ub_A_sparse, ub_A);
    //------------------------------------------------------------------------------------------

    int_max WSR = m_Option.MaxIterNumber;

    ElementType CPUTime = 0;
    ElementType* ptrCPUTime = nullptr;
    if (m_Option.CPUTime > 0.0)
    {
        CPUTime = ElementType(m_Option.CPUTime);
        ptrCPUTime = &CPUTime;
    }

    if (m_Counter_In_Mode_Online_Varying_H_A == 1)
    {
        m_SQProblem_Online->init(&temp_H, g.GetElementPointer(), &temp_A,
                                 lb_x.GetElementPointer(), ub_x.GetElementPointer(),
                                 lb_A.GetElementPointer(), ub_A.GetElementPointer(),
                                 WSR, ptrCPUTime);
    }
    else
    {
        m_SQProblem_Online->hotstart(&temp_H, g.GetElementPointer(), &temp_A,
                                     lb_x.GetElementPointer(), ub_x.GetElementPointer(),
                                     lb_A.GetElementPointer(), ub_A.GetElementPointer(),
                                     WSR, ptrCPUTime);
    }
    //------------------------------------------------------------------------------------------
    m_Solution.WSR = WSR;
    m_Solution.CPUTime = CPUTime;

    m_Solution.X.FastResize(m_VaribleNumber_In_Mode_Online_Varying_H_A, 1);
    m_SQProblem_Online->getPrimalSolution(m_Solution.X.GetElementPointer());

    m_Solution.ObjectiveFunctionValue = m_SQProblem_Online->getObjVal();

    return true;
}


template<typename ElementType>
Solution_Of_QuadraticProgramming<ElementType>* QuadraticProgrammingSolver<ElementType>::GetSolution()
{
    return &m_Solution;
}


template<typename ElementType>
Solution_Of_QuadraticProgramming<ElementType>& QuadraticProgrammingSolver<ElementType>::Solution()
{
	return m_Solution;
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

        if (Solver->m_Option.Mode != "OneTimeOnly")
        {
            MDK_Error("Wrong Mode in input Option @ QuadraticProgrammingSolver::Apply(...)")
			Solution_Of_QuadraticProgramming<ElementType> EmptySolution;
			return EmptySolution;
        }
    }

    Solver->SetInputData(H, g, lb_x, ub_x, A, lb_A, ub_A, x0);

    Solver->Update();

	return Solver->Solution();
}


template<typename ElementType>
Solution_Of_QuadraticProgramming<ElementType> QuadraticProgrammingSolver<ElementType>::Apply(const SparseMatrix<ElementType>* H,
                                                                                             const SparseMatrix<ElementType>* g,
                                                                                             const SparseMatrix<ElementType>* lb_x,
                                                                                             const SparseMatrix<ElementType>* ub_x,
                                                                                             const SparseMatrix<ElementType>* A,
                                                                                             const SparseMatrix<ElementType>* lb_A,
                                                                                             const SparseMatrix<ElementType>* ub_A,
                                                                                             const SparseMatrix<ElementType>* x0,
                                                                                             const Option_Of_QuadraticProgramming* Option)
{
    auto Solver = std::make_unique<QuadraticProgrammingSolver<ElementType>>();

    if (Option != nullptr)
    {
        Solver->m_Option = *Option;

        if (Solver->m_Option.Mode != "OneTimeOnly")
        {
            MDK_Error("Wrong Mode in input Option @ QuadraticProgrammingSolver::Apply(...)")
			Solution_Of_QuadraticProgramming<ElementType> EmptySolution;
			return EmptySolution;
        }
    }

    Solver->SetInputData(H, g, lb_x, ub_x, A, lb_A, ub_A, x0);

    Solver->Update();

	return Solver->Solution();
}


template<typename ElementType>
Solution_Of_QuadraticProgramming<ElementType> QuadraticProgrammingSolver<ElementType>::Apply(const DenseMatrix<ElementType>*  H,
                                                                                             const DenseMatrix<ElementType>*  g,
                                                                                             const DenseMatrix<ElementType>*  lb_x,
                                                                                             const DenseMatrix<ElementType>*  ub_x,
                                                                                             const SparseMatrix<ElementType>* A,
                                                                                             const DenseMatrix<ElementType>*  lb_A,
                                                                                             const DenseMatrix<ElementType>*  ub_A,
                                                                                             const DenseMatrix<ElementType>*  x0,
                                                                                             const Option_Of_QuadraticProgramming* Option)
{
    auto Solver = std::make_unique<QuadraticProgrammingSolver<ElementType>>();

    if (Option != nullptr)
    {
        Solver->m_Option = *Option;

        if (Solver->m_Option.Mode != "OneTimeOnly")
        {
            MDK_Error("Wrong Mode in input Option @ QuadraticProgrammingSolver::Apply(...)")
            Solution_Of_QuadraticProgramming<ElementType> EmptySolution;
			return EmptySolution;
        }
    }

    Solver->SetInputData(H, g, lb_x, ub_x, A, lb_A, ub_A, x0);

    Solver->Update();

	return Solver->Solution();
}

}//namespace mdk

#endif