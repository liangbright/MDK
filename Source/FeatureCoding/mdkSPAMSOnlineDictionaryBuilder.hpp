#ifndef __mdkSPAMSOnlineDictionaryBuilder_hpp
#define __mdkSPAMSOnlineDictionaryBuilder_hpp


#include <opencv2/core/core.hpp>

//#include "mdkSPAMSOnlineDictionaryBuilder.h"

namespace mdk
{

template<typename ElementType>
SPAMSOnlineDictionaryBuilder<ElementType>::SPAMSOnlineDictionaryBuilder()
{
    this->Clear();
}


template<typename ElementType>
SPAMSOnlineDictionaryBuilder<ElementType>::~SPAMSOnlineDictionaryBuilder()
{
}

//---------------------------------------------------------------------------------------------------------------//

template<typename ElementType>
void SPAMSOnlineDictionaryBuilder<ElementType>::Clear()
{
    this->FeatureDictionaryBuilder::Clear();

    m_Parameter.Clear();

    m_State.Clear();
}


template<typename ElementType>
bool SPAMSOnlineDictionaryBuilder<ElementType>::SetInitialState(State_Of_SPAMSOnlineDictionaryBuilder<ElementType> InitialState)
{
    //check InitialState

    m_State.Take(InitialState);

    return true;
}


template<typename ElementType>
State_Of_SPAMSOnlineDictionaryBuilder<ElementType>*
SPAMSOnlineDictionaryBuilder<ElementType>::GetCurrentState()
{
    return &m_State;
}


template<typename ElementType>
bool SPAMSOnlineDictionaryBuilder<ElementType>::SaveStateAndParameter(const std::string& FilePathAndName)
{
    return true;
}


template<typename ElementType>
bool SPAMSOnlineDictionaryBuilder<ElementType>::LoadStateAndParameter(const std::string& FilePathAndName)
{
    return true;
}



template<typename ElementType>
bool SPAMSOnlineDictionaryBuilder<ElementType>::CheckInputAndOutput()
{
    return true;
}


template<typename ElementType>
bool SPAMSOnlineDictionaryBuilder<ElementType>::GenerateDictionary()
{
    if (this->CheckInputAndOutput() == false)
    {
        return false;
    }

    //---------------------------------------------------------

    auto X_spt = std::make_unique<spams::Matrix<ElementType>>(m_FeatureData->GetElementPointer(), m_FeatureData->GetRowNumber(), m_FeatureData->GetColNumber());
    auto X = X_spt.get();

    std::unique_ptr<spams::Trainer<ElementType>> trainer_spt;
    spams::Trainer<ElementType>* trainer;

    if (m_State.D.IsEmpty() == true)
    {
        trainer_spt.reset(new spams::Trainer<ElementType>(m_Parameter.K, m_Parameter.batchsize, m_Parameter.numThreads));
        trainer = trainer_spt.get();
    }
    else
    {
        Matrix<T> D1(m_State.D.GetElementPointer(), m_State.D.GetRowNumber(), m_State.D.GetColNumber());

        if (m_State.A.IsEmpty() == true)
        {
            trainer_spt.reset(new spams::Trainer<ElementType>(D1, m_Parameter.batchsize, m_Parameter.numThreads));
            trainer = trainer_spt.get();
        }
        else
        {
            spams::Matrix<ElementType> A(m_State.A.GetElementPointer(), m_State.A.GetRowNumber(), m_State.A.GetColNumber());
            spams::Matrix<ElementType> B(m_State.B.GetElementPointer(), m_State.B.GetRowNumber(), m_State.B.GetColNumber());
            trainer_spt.reset(new spams::Trainer<ElementType>(A, B, D1, m_Parameter.batchsize, m_Parameter.numThreads));
            trainer = trainer_spt.get();
        }
    }

    //---------------------------------------------------------

    spams::ParamDictLearn<ElementType> param;

    param.lambda = m_Parameter.lambda;
    param.lambda2 = m_Parameter.lambda2;
    param.iter = m_Parameter.iter;
    param.t0 = m_Parameter.t0;
    param.mode = m_Parameter.mode;
    param.posAlpha = m_Parameter.posAlpha;
    param.posD = m_Parameter.posD;
    param.expand = false; // what is this ?
    param.modeD = m_Parameter.modeD;
    param.whiten = false;
    param.clean = m_Parameter.clean;
    param.verbose = m_Parameter.verbose;
    param.gamma1 = m_Parameter.gamma1;
    param.gamma2 = m_Parameter.gamma2;
    param.rho = m_Parameter.rho;
    param.stochastic = false; // what is this ?
    param.modeParam = m_Parameter.modeParam;
    param.batch = false;
    param.iter_updateD = m_Parameter.iter_updateD;
    param.log = false;

    //---------------------------------------------------------

    if (param.mode >= 0)
    {
        trainer->train_extended(*X, param);
    }
    else
    {
        trainer->train_extended(*X, param, m_SparseEncoder);
    }

    //---------------------------------------------------------

    spams::Matrix<ElementType>& D = trainer->getD_ref();
    m_State.D.Copy(D.rawX(), D.m(), D.n());

    spams::Matrix<ElementType>& A = trainer->getA_ref();
    m_State.A.Copy(A.rawX(), A.m(), A.n());

    spams::Matrix<ElementType>& B = trainer->getB_ref();
    m_State.B.Copy(B.rawX(), B.m(), B.n());

    return true;
}



}// namespace mdk


#endif