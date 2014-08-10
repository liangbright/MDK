#ifndef __mdkSPAMSOnlineDictionaryBuilder_h
#define __mdkSPAMSOnlineDictionaryBuilder_h

// based on : Online Dictionary Learning for Sparse Coding. Julien Mairal
//            SPAMS (SPArse Modeling Software) http://spams-devel.gforge.inria.fr/

// [D model] = mexTrainDL(X,param, model);

#include <vector>
#include <string>
#include <memory>

#include <spams/dictLearn/dicts.h>

#include "mdkFeatureDictionaryBuilder.h"
#include "mdkFeatureDictionaryBasedSparseEncoder.h"
#include "mdkFeatureDictionaryForSparseCoding.h"

namespace mdk
{

template<typename ElementType>
struct Parameter_Of_SPAMSOnlineDictionaryBuilder  // param in [D model] = mexTrainDL(X,param,model);
{
    int_max mode; // -1 means use m_SparseEncoder instead of the methods in SPAMS software

    int_max modeD; // select method to normalize Dictionary

    DenseMatrix<ElementType> D; // initial Dictionary

    int_max K; // length of dictionary, not not useful if D is not empty

    ElementType lambda;

    ElementType lambda2;

    int_max iter; //(number of iterations)

    bool posAlpha; // positivity constraints on the coefficients

    bool posD;     // positivity constraints on the dictionary

    ElementType gamma1;

    ElementType gamma2;

    int_max batchsize;

    int_max iter_updateD = 1;

    int_max modeParam;

    ElementType rho;

    ElementType t0;

    bool clean;

    bool verbose;

    int_max numThreads;

    /*
    ElementType mu;    
    ElementType lambda3 = 0;
    ElementType lambda4 = 0;
    bool whiten;
    bool expand;
    bool isConstant;
    bool updateConstant;
    bool ThetaDiag;
    bool ThetaDiagPlus;
    bool ThetaId;
    bool DequalsW;
    bool weightClasses;
    bool balanceClasses;
    bool extend;
    bool pattern;
    bool stochastic;
    bool scaleW;
    bool batch;
       
    bool log;
    bool updateD;
    bool updateW;
    bool updateTheta;
    char* logName;
    */

    Parameter_Of_SPAMSOnlineDictionaryBuilder() { this->Clear(); };

    ~Parameter_Of_SPAMSOnlineDictionaryBuilder() {};

    void Clear()
    {
        mode = -1;

        modeD = 0;

        D.Clear();

        K = 1;

        lambda = 0;

        lambda2 = 0;

        iter = 1000;

        posAlpha = false;

        posD = false;

        gamma1 = 0;

        gamma2 = 0;

        batchsize = 512;

        iter_updateD = 1;

        modeParam = 0;

        rho = 1;

        t0 = 0.00001;

        clean = true;

        verbose = true;

        numThreads = 1;
    }

private:
    Parameter_Of_SPAMSOnlineDictionaryBuilder(const Parameter_Of_SPAMSOnlineDictionaryBuilder&) = delete;
    void operator=(const Parameter_Of_SPAMSOnlineDictionaryBuilder&) = delete;
};


template<typename ElementType>
struct State_Of_SPAMSOnlineDictionaryBuilder  // D and model in [D model] = mexTrainDL(X,param,model);
{
    DenseMatrix<ElementType> A;
    DenseMatrix<ElementType> B;
    DenseMatrix<ElementType> D;

//-----------------------------------------------------------------------------------------------
    State_Of_SPAMSOnlineDictionaryBuilder(){};

    State_Of_SPAMSOnlineDictionaryBuilder(State_Of_SPAMSOnlineDictionaryBuilder&& InputState)
    {
        A = std::move(InputState.A);
        B = std::move(InputState.B);
        D = std::move(InputState.D);
    }

    ~State_Of_SPAMSOnlineDictionaryBuilder(){};

    void Take(State_Of_SPAMSOnlineDictionaryBuilder& InputState)
    {
        A.Take(InputState.A);
        B.Take(InputState.B);
        D.Take(InputState.D);
    }

    void Clear()
    {
        A.Clear();
        B.Clear();
        D.Clear();
    }

    bool IsEmpty()
    {
        return A.IsEmpty();
    }

private:
    State_Of_SPAMSOnlineDictionaryBuilder(const State_Of_SPAMSOnlineDictionaryBuilder&) = delete;
    void operator=(const State_Of_SPAMSOnlineDictionaryBuilder&) = delete;
    void operator=(State_Of_SPAMSOnlineDictionaryBuilder&&) = delete;
};


template<typename Element_Type>
class SPAMSOnlineDictionaryBuilder : public FeatureDictionaryBuilder<Element_Type>
{
public:
	typedef Element_Type ElementType;

public:
    Parameter_Of_SPAMSOnlineDictionaryBuilder<ElementType> m_Parameter;

protected:

    const DenseMatrix<ElementType>* m_FeatureData;

    FeatureDictionaryBasedSparseEncoder<ElementType>* m_SparseEncoder;

    State_Of_SPAMSOnlineDictionaryBuilder<ElementType> m_State;

    const FeatureDictionaryForSparseCoding<ElementType>* m_InitialDictionary;

    FeatureDictionaryForSparseCoding<ElementType>* m_Dictionary;

    FeatureDictionaryForSparseCoding<ElementType> m_Dictionary_SharedCopy;

public:

    SPAMSOnlineDictionaryBuilder();

    ~SPAMSOnlineDictionaryBuilder();

    void Clear();

    void SetInputFeatureData(const DenseMatrix<ElementType>* InputFeatureData);

    void SetOutputDictionary(FeatureDictionaryForSparseCoding<ElementType>* Dictionary);

    void SetInitialDictionary(const FeatureDictionaryForSparseCoding<ElementType>* Dictionary);

    void SetInitialState(State_Of_SPAMSOnlineDictionaryBuilder<ElementType> InitialState); //copy value

    State_Of_SPAMSOnlineDictionaryBuilder<ElementType>* GetCurrentState();

    void SetSparseEncoder(FeatureDictionaryBasedSparseEncoder<ElementType>* Encoder);

    bool LoadStateAndParameter(const std::string& FilePathAndName);

    bool SaveStateAndParameter(const std::string& FilePathAndName);

    bool CheckInput();

    FeatureDictionaryForSparseCoding<ElementType>* GetOutputDictionary();

protected:

    void GenerateDictionary();

    void ClearPipelineOutput();

    void UpdatePipelineOutput();

private:
    SPAMSOnlineDictionaryBuilder(const SPAMSOnlineDictionaryBuilder&) = delete;

    void operator=(const SPAMSOnlineDictionaryBuilder&) = delete;

    SPAMSOnlineDictionaryBuilder(SPAMSOnlineDictionaryBuilder&&) = delete;

    void operator=(SPAMSOnlineDictionaryBuilder&&) = delete;
};

}// namespace mdk


#include "mdkSPAMSOnlineDictionaryBuilder.hpp"

#endif