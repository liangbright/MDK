#ifndef __mdkSPAMSSparseEncoder_hpp
#define __mdkSPAMSSparseEncoder_hpp

//#include "mdkSPAMSSparseEncoder.h"

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
bool SPAMSSparseEncoder<ElementType>::CheckInputAndOutput()
{
    if (this->FeatureDictionaryBasedSparseEncoder::CheckInputAndOutput() == false)
    {
        return false;
    }

    if (m_MethodName == "OMP")
    {

    }
    else if (m_MethodName == "Lasso")
    {

    }
    else
    {
        MDK_Error("Not support @ SPAMSSparseEncoder::CheckInputAndOutput()")
        return false;
    }

    return true;
}


template<typename ElementType>
void SPAMSSparseEncoder<ElementType>::GenerateCode_in_a_Thread(int_max IndexOfFeatureVector_start, int_max IndexOfFeatureVector_end)
{
    auto tempFeatureData = m_FeatureData->GetSubMatrix(ALL, span(IndexOfFeatureVector_start, IndexOfFeatureVector_end));

    auto CodeDimension = m_Dictionary->m_Record.GetColNumber();

    spams::Matrix<ElementType> X(const_cast<ElementType*>(tempFeatureData.GetElementPointer()),
                                 tempFeatureData.GetRowNumber(), 
                                 tempFeatureData.GetColNumber());

    spams::Matrix<ElementType> D(const_cast<ElementType*>(m_Dictionary->m_Record.GetElementPointer()),
                                 m_Dictionary->m_Record.GetRowNumber(),
                                 m_Dictionary->m_Record.GetColNumber());

    spams::SpMatrix<ElementType> alpha;

    int L = m_Parameter_OMP.L;

    ElementType Eps = m_Parameter_OMP.eps;

    ElementType Lambda = m_Parameter_OMP.lambda;

    spams::omp<ElementType>(X, D, alpha, &L, &Eps, &Lambda, false, false, false, 1, nullptr);

    for (int_max i = IndexOfFeatureVector_start; i <= IndexOfFeatureVector_end; ++i)
    {
        spams::SpVector<ElementType> tempS;

        alpha.refCol(i - IndexOfFeatureVector_start, tempS);

        std::vector<int_max> RowIndexList(int_max(tempS.nzmax()));
        for (int_max k = 0; k < int_max(tempS.nzmax()); ++k)
        {
            RowIndexList[k] = tempS.r(k);
        }

        std::vector<ElementType> DataArray(tempS.rawX(), tempS.rawX() + int_max(tempS.nzmax()));

        (*m_CodeInSparseVectorList)[i].ConstructColVectorWithOrder(std::move(RowIndexList), std::move(DataArray), CodeDimension);
    }
}


template<typename ElementType>
inline
void SPAMSSparseEncoder<ElementType>::EncodingFunction(const DenseMatrix<ElementType>& SingleFeatureDataVector,
                                                       SparseMatrix<ElementType>& CodeInSparseVector)
{
   
}


}// namespace mdk


#endif