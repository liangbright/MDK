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

template<typename ScalarType>
struct Parameter_Of_SPAMSOnlineDictionaryBuilder  // param in [D model] = mexTrainDL(X,param,model);
{
    int_max mode; // -1 means use m_SparseEncoder instead of the methods in SPAMS software

    int_max modeD; // select method to normalize Dictionary

    DenseMatrix<ScalarType> D; // initial Dictionary

    int_max K; // length of dictionary, not useful if D is not empty

    ScalarType lambda;

    ScalarType lambda2;

    int_max iter; //(number of iterations)

    bool posAlpha; // positivity constraints on the coefficients

    bool posD;     // positivity constraints on the dictionary

    ScalarType gamma1;

    ScalarType gamma2;

    int_max batchsize;

    int_max iter_updateD = 1;

    int_max modeParam;

    ScalarType rho;

    ScalarType t0;

    bool clean;

    bool verbose;

    int_max numThreads;

    /*
    ScalarType mu;    
    ScalarType lambda3 = 0;
    ScalarType lambda4 = 0;
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


template<typename ScalarType>
struct State_Of_SPAMSOnlineDictionaryBuilder  // D and model in [D model] = mexTrainDL(X,param,model);
{
    DenseMatrix<ScalarType> A;
    DenseMatrix<ScalarType> B;
    DenseMatrix<ScalarType> D;

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
        return D.IsEmpty();
    }

private:
    State_Of_SPAMSOnlineDictionaryBuilder(const State_Of_SPAMSOnlineDictionaryBuilder&) = delete;
    void operator=(const State_Of_SPAMSOnlineDictionaryBuilder&) = delete;
    void operator=(State_Of_SPAMSOnlineDictionaryBuilder&&) = delete;
};


template<typename Scalar_Type>
class SPAMSOnlineDictionaryBuilder : public FeatureDictionaryBuilder<FeatureDictionaryForSparseCoding<Scalar_Type>>
{
public:
	typedef Scalar_Type ScalarType;
	typedef FeatureDictionaryForSparseCoding<ScalarType> DictionaryType;

public:
    Parameter_Of_SPAMSOnlineDictionaryBuilder<ScalarType> m_Parameter;

protected:
    const DenseMatrix<ScalarType>* m_FeatureData;

    FeatureDictionaryBasedSparseEncoder<ScalarType>* m_SparseEncoder;

    State_Of_SPAMSOnlineDictionaryBuilder<ScalarType> m_State;

	const DictionaryType* m_InitialDictionary;

	DictionaryType m_Dictionary;

public:

    SPAMSOnlineDictionaryBuilder();

    ~SPAMSOnlineDictionaryBuilder();

    void Clear();

    void SetInputFeatureData(const DenseMatrix<ScalarType>* InputFeatureData);

	void SetInitialDictionary(const DictionaryType* Dictionary);

    void SetInitialState(State_Of_SPAMSOnlineDictionaryBuilder<ScalarType> InitialState); //copy value

    State_Of_SPAMSOnlineDictionaryBuilder<ScalarType>* GetCurrentState();

    void SetSparseEncoder(FeatureDictionaryBasedSparseEncoder<ScalarType>* Encoder);

    bool LoadStateAndParameter(const std::string& FilePathAndName);

    bool SaveStateAndParameter(const std::string& FilePathAndName);

    bool CheckInput();

	bool Update();

	DictionaryType* GetOutputDictionary();

protected:
    void GenerateDictionary();

private:
    SPAMSOnlineDictionaryBuilder(const SPAMSOnlineDictionaryBuilder&) = delete;
    void operator=(const SPAMSOnlineDictionaryBuilder&) = delete;
};

}// namespace mdk


#include "mdkSPAMSOnlineDictionaryBuilder.hpp"

#endif