#ifndef __mdkSPAMSOnlineDictionaryBuilder_hpp
#define __mdkSPAMSOnlineDictionaryBuilder_hpp

//#include <opencv2/core/core.hpp>

namespace mdk
{

template<typename ScalarType>
SPAMSOnlineDictionaryBuilder<ScalarType>::SPAMSOnlineDictionaryBuilder()
{
    this->Clear();
}


template<typename ScalarType>
SPAMSOnlineDictionaryBuilder<ScalarType>::~SPAMSOnlineDictionaryBuilder()
{
}

//---------------------------------------------------------------------------------------------------------------//

template<typename ScalarType>
void SPAMSOnlineDictionaryBuilder<ScalarType>::Clear()
{
    m_Parameter.Clear();
    m_State.Clear();
	m_SparseEncoder = nullptr;
	m_InitialDictionary = nullptr;
	m_Dictionary.Clear();
}


template<typename ScalarType>
void SPAMSOnlineDictionaryBuilder<ScalarType>::SetInputFeatureData(const DenseMatrix<ScalarType>* InputFeatureData)
{
    m_FeatureData = InputFeatureData;
}


template<typename ScalarType>
void SPAMSOnlineDictionaryBuilder<ScalarType>::SetInitialDictionary(const FeatureDictionaryForSparseCoding<ScalarType>* Dictionary)
{
    m_InitialDictionary = Dictionary;
}


template<typename ScalarType>
void SPAMSOnlineDictionaryBuilder<ScalarType>::SetInitialState(State_Of_SPAMSOnlineDictionaryBuilder<ScalarType> InitialState)
{
    m_State = std::move(InitialState);
}


template<typename ScalarType>
void SPAMSOnlineDictionaryBuilder<ScalarType>::SetSparseEncoder(FeatureDictionaryBasedSparseEncoder<ScalarType>* Encoder)
{
    m_SparseEncoder = Encoder;
}


template<typename ScalarType>
State_Of_SPAMSOnlineDictionaryBuilder<ScalarType>*
SPAMSOnlineDictionaryBuilder<ScalarType>::GetCurrentState()
{
    return &m_State;
}


template<typename ScalarType>
bool SPAMSOnlineDictionaryBuilder<ScalarType>::LoadStateAndParameter(const std::string& FilePathAndName)
{
    return true;
}


template<typename ScalarType>
bool SPAMSOnlineDictionaryBuilder<ScalarType>::SaveStateAndParameter(const std::string& FilePathAndName)
{
    return true;
}


template<typename ScalarType>
bool SPAMSOnlineDictionaryBuilder<ScalarType>::CheckInput()
{
    if (m_FeatureData == nullptr)
    {
        MDK_Error("m_FeatureData is nullptr @ SPAMSOnlineDictionaryBuilder::CheckInput()")
        return false;
    }

    auto DataSize = m_FeatureData->GetSize();

    if (DataSize.RowNumber == 0)
    {
        MDK_Error("InputFeatureData is empty @ SPAMSOnlineDictionaryBuilder::CheckInput()")
        return false;
    }

    if (m_SparseEncoder == nullptr)
    {
        MDK_Error("m_SparseEncoder is nullptr @ SPAMSOnlineDictionaryBuilder::CheckInput()")
        return false;
    }

    if (m_Parameter.mode < 0) // use MDK sparse encoder (e.g., KNNReconstructionSparseEncoder)
    {
        if (m_InitialDictionary == nullptr)
        {
            MDK_Error("m_InitialDictionary is empty (nullptr) and m_Parameter.mode < 0 @ SPAMSOnlineDictionaryBuilder::CheckInput()")
            return false;
        }

        if (m_InitialDictionary->IsEmpty() == true)
        {
            MDK_Error("m_InitialDictionary is empty and m_Parameter.mode < 0 @ SPAMSOnlineDictionaryBuilder::CheckInput()")
            return false;
        }

        if (m_Parameter.modeD == 3 && m_Parameter.gamma1 == ScalarType(1))
        {
            if (m_InitialDictionary->GetProperty_BasisNormalizedWithL1Norm() == false)
            {
                MDK_Error("Basis is NOT Normalized With L1 Norm in m_InitialDictionary" << '\n'
                          << "and m_Parameter.mode < 0 and m_Parameter.modeD == 3 and m_Parameter.gamma1 == 1" << '\n'
                          << "@ SPAMSOnlineDictionaryBuilder::CheckInput()")
                return false;
            }
        }

        if (m_Parameter.posD != m_InitialDictionary->GetProperty_BasisPositive())
        {
            MDK_Error("m_Parameter.posD != m_InitialDictionary->GetProperty_BasisPositive()" << '\n'               
                      << "@ SPAMSOnlineDictionaryBuilder::CheckInput()")
            return false;
        }
    }

    return true;
}


template<typename ScalarType>
bool SPAMSOnlineDictionaryBuilder<ScalarType>::Update()
{
	if (this->CheckInput() == false)
	{
		return false;
	}
	this->GenerateDictionary();
	return true;
}


template<typename ScalarType>
void SPAMSOnlineDictionaryBuilder<ScalarType>::GenerateDictionary()
{
    auto X_spt = std::make_unique<spams::Matrix<ScalarType>>(const_cast<ScalarType*>(m_FeatureData->GetElementPointer()),
                                                             m_FeatureData->GetRowNumber(), m_FeatureData->GetColNumber());
    auto X = X_spt.get();

    std::unique_ptr<spams::Trainer<ScalarType>> trainer_spt;
    spams::Trainer<ScalarType>* trainer;

    if (m_State.D.IsEmpty() == true)
    {
        trainer_spt.reset(new spams::Trainer<ScalarType>(m_Parameter.K, m_Parameter.batchsize, m_Parameter.numThreads));
        trainer = trainer_spt.get();
    }
    else
    {
        spams::Matrix<ScalarType> D1(m_State.D.GetElementPointer(), m_State.D.GetRowNumber(), m_State.D.GetColNumber());

        if (m_State.A.IsEmpty() == true)
        {
            trainer_spt.reset(new spams::Trainer<ScalarType>(D1, m_Parameter.batchsize, m_Parameter.numThreads));
            trainer = trainer_spt.get();
        }
        else
        {
            spams::Matrix<ScalarType> A(m_State.A.GetElementPointer(), m_State.A.GetRowNumber(), m_State.A.GetColNumber());
            spams::Matrix<ScalarType> B(m_State.B.GetElementPointer(), m_State.B.GetRowNumber(), m_State.B.GetColNumber());
            trainer_spt.reset(new spams::Trainer<ScalarType>(A, B, D1, m_Parameter.iter, m_Parameter.batchsize, m_Parameter.numThreads));
            trainer = trainer_spt.get();
        }
    }

    //---------------------------------------------------------

    spams::ParamDictLearn<ScalarType> param;

    param.lambda = m_Parameter.lambda;
    param.lambda2 = m_Parameter.lambda2;
    param.iter = m_Parameter.iter;
    param.t0 = m_Parameter.t0;
    param.mode = static_cast<spams::constraint_type>(m_Parameter.mode);
    param.posAlpha = m_Parameter.posAlpha;
    param.posD = m_Parameter.posD;
    param.expand = false; // what is this ?
    param.modeD = static_cast<spams::constraint_type_D>(m_Parameter.modeD);
    param.whiten = false;
    param.clean = m_Parameter.clean;
    param.verbose = m_Parameter.verbose;
    param.gamma1 = m_Parameter.gamma1;
    param.gamma2 = m_Parameter.gamma2;
    param.rho = m_Parameter.rho;
    param.stochastic = false; // what is this ?
    param.modeParam = static_cast<spams::mode_compute>(m_Parameter.modeParam);
    param.batch = false;
    param.iter_updateD = m_Parameter.iter_updateD;
    param.log = false;

    //---------------------------------------------------------

    if (param.mode >= 0)
    {
        trainer->train(*X, param);
    }
    else
    {
        trainer->train_extended(*X, param, m_SparseEncoder, *m_InitialDictionary);
    }

    //---------------------------------------------------------

    spams::Matrix<ScalarType>& D = trainer->getD_ref();
    m_State.D.Copy(D.rawX(), D.m(), D.n());

    spams::Matrix<ScalarType>& A = trainer->getA_ref();
    m_State.A.Copy(A.rawX(), A.m(), A.n());

    spams::Matrix<ScalarType>& B = trainer->getB_ref();
    m_State.B.Copy(B.rawX(), B.m(), B.n());
}


template<typename ScalarType>
FeatureDictionaryForSparseCoding<ScalarType>* SPAMSOnlineDictionaryBuilder<ScalarType>::GetOutputDictionary()
{
	return &m_Dictionary;
}


}// namespace mdk


#endif