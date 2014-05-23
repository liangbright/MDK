#ifndef __mdkSPAMSSparseEncoder_hpp
#define __mdkSPAMSSparseEncoder_hpp


namespace mdk
{

template<typename ElementType>
SPAMSSparseEncoder<ElementType>::SPAMSSparseEncoder()
{
    this->Clear();
}


template<typename ElementType>
SPAMSSparseEncoder<ElementType>::~SPAMSSparseEncoder()
{

}


template<typename ElementType>
void SPAMSSparseEncoder<ElementType>::Clear()
{
    this->FeatureDictionaryBasedSparseEncoder::Clear();

    m_Parameter_OMP.Clear();

    m_Parameter_Lasso.Clear();

    m_MethodName = "OMP";
}


template<typename ElementType>
bool SPAMSSparseEncoder<ElementType>::CheckInput()
{
    if (this->FeatureDictionaryBasedSparseEncoder::CheckInput() == false)
    {
        return false;
    }

    if (m_MethodName != "OMP" && m_MethodName != "Lasso")
    {
        MDK_Error("Method Not support @ SPAMSSparseEncoder::CheckInput()")
        return false;
    }

    return true;
}


template<typename ElementType>
void SPAMSSparseEncoder<ElementType>::GenerateCode_in_a_Thread(int_max IndexOfDataVector_start, int_max IndexOfDataVector_end, int_max ThreadIndex)
{
    //-------------------------------------//
    m_ThreadStatus[ThreadIndex] = 1;
    //-------------------------------------//

    auto tempFeatureData = m_FeatureData->GetSubMatrix(ALL, span(IndexOfDataVector_start, IndexOfDataVector_end));

    auto CodeLength = m_Dictionary->BasisMatrix().GetColNumber();

    spams::Matrix<ElementType> X(const_cast<ElementType*>(tempFeatureData.GetElementPointer()),
                                 tempFeatureData.GetRowNumber(), 
                                 tempFeatureData.GetColNumber());

    spams::Matrix<ElementType> D(const_cast<ElementType*>(m_Dictionary->BasisMatrix().GetElementPointer()),
                                 m_Dictionary->BasisMatrix().GetRowNumber(),
                                 m_Dictionary->BasisMatrix().GetColNumber());

    spams::SpMatrix<ElementType> alpha;

    if (m_MethodName == "OMP")
    {
        int L = m_Parameter_OMP.L;

        ElementType Eps = m_Parameter_OMP.eps;

        ElementType Lambda = m_Parameter_OMP.lambda;

        spams::omp<ElementType>(X, D, alpha, &L, &Eps, &Lambda, false, false, false, 1, nullptr);
    }
    else if (m_MethodName == "Lasso")
    {
        auto mode = static_cast<spams::constraint_type>(m_Parameter_Lasso.mode);
        bool pos = m_Parameter_Lasso.pos;
        bool ols = m_Parameter_Lasso.ols;
        auto lambda = m_Parameter_Lasso.lambda;
        auto lambda2 = m_Parameter_Lasso.lambda2;

        int n = m_Dictionary->BasisMatrix().GetRowNumber();
        int K = m_Dictionary->BasisMatrix().GetColNumber();

        int L;
        if (m_Parameter_Lasso.L > 0)
        {
            L = m_Parameter_Lasso.L;
        }
        else
        {
            L = K;
        }

        if (L > n && !(mode == spams::PENALTY && (std::abs(lambda) < 0.000001) && !pos && lambda2 > 0))
        {           
            L = n;
        }
        if (L > K)
        {        
            L = K;
        }

        bool cholesky = m_Parameter_Lasso.cholesky;
        int length_path = 4 * L;

        if (cholesky)
        {
            spams::lasso<ElementType>(X, D, alpha, L, lambda, lambda2, mode, pos, ols, 1, nullptr, length_path);
        }
        else 
        {
            spams::lasso2<ElementType>(X, D, alpha, L, lambda, lambda2, mode, pos, 1, nullptr, length_path);
        }
    }
    
    for (int_max i = IndexOfDataVector_start; i <= IndexOfDataVector_end; ++i)
    {
        spams::SpVector<ElementType> tempS;

        alpha.refCol(i - IndexOfDataVector_start, tempS);

        std::vector<int_max> RowIndexList(int_max(tempS.nzmax()));
        for (int_max k = 0; k < int_max(tempS.nzmax()); ++k)
        {
            RowIndexList[k] = tempS.r(k);
        }

        std::vector<ElementType> DataArray(tempS.rawX(), tempS.rawX() + int_max(tempS.nzmax()));

        (*m_CodeInSparseColVectorSet)[i].ConstructFromSortedData(std::move(RowIndexList), std::move(DataArray), CodeLength);
    }

    //-------------------------------------//
    m_ThreadStatus[ThreadIndex] = 2;
    //-------------------------------------//
}


template<typename ElementType>
inline
void SPAMSSparseEncoder<ElementType>::EncodingFunction(int_max DataIndex, int_max ThreadIndex)
{
    MDK_Error("This function is not implemented @ SPAMSSparseEncoder::EncodingFunction(...)")
}


}// namespace mdk


#endif