#ifndef __mdkSPAMSOnlineDictionaryBuilder_h
#define __mdkSPAMSOnlineDictionaryBuilder_h

// based on : Online Dictionary Learning for Sparse Coding. Julien Mairal
//            SPAMS (SPArse Modeling Software) http://spams-devel.gforge.inria.fr/

// [D model] = mexTrainDL(X,param,model);

#include <vector>
#include <string>

#include "mdkFeatureDictionaryBasedSparseEncoder.h"
#include "mdkFeatureDictionaryBuilder.h"

namespace mdk
{

template<typename ElementType>
struct Parameter_Of_SPAMSOnlineDictionaryBuilder  // param in [D model] = mexTrainDL(X,param,model);
{
    int64 mode = -1; // -1 means use m_SparseEncoder instead of the methods in SPAMS software

    int64 modeD = 0; // select method to normalize Dictionary

    DenseMatrix<ElementType> D; // initial Dictionary

    int64 K = 0; // length of dictionary, not not useful if D is not empty

    double lambda;

    double lambda2 = 0;

    int64 iter; //(number of iterations)

    bool posAlpha = false;

    bool posD = false;

    double gamma1;

    int64 batchsize = 512;

    int64 iter_updateD = 1;

    int64 modeParam;

    double rho;

    double t0;

    bool clean = true;

    bool verbose;

    int64 numThreads = 1;

//-----parameter not mentioned in online document
    bool whiten = false;
};


template<typename ElementType>
struct State_Of_SPAMSOnlineDictionaryBuilder  // model in [D model] = mexTrainDL(X,param,model);
{
    DenseMatrix<ElementType> A;
    DenseMatrix<ElementType> B;
};


template<typename ElementType>
class SPAMSOnlineDictionaryBuilder : public FeatureDictionaryBuilder<ElementType>
{
protected:

    FeatureDictionaryBasedSparseEncoder<ElementType>* m_SparseEncoder;

    std::string m_SparseEncoderName;

    Parameter_Of_SPAMSOnlineDictionaryBuilder<ElementType> m_Parameter;

    State_Of_SPAMSOnlineDictionaryBuilder<ElementType> m_State;

    const FeatureDictionary<ElementType>* m_InitialDictionary;

public:

    SPAMSOnlineDictionaryBuilder();

    ~SPAMSOnlineDictionaryBuilder();

    void Clear();
  
    bool SetInitialState(const State_Of_SPAMSOnlineDictionaryBuilder<ElementType>& InitialState);

    bool SetOutputDictionaryLength(int64 DictionaryLength);

    State_Of_SPAMSOnlineDictionaryBuilder<ElementType> GetCurrentState();

    //----------------------------------------------------//

protected:

    bool GenerateDictionary();

private:

    bool FirstTimeBuild();

    bool OnlineUpdate();

private:
//deleted
    SPAMSOnlineDictionaryBuilder(const SPAMSOnlineDictionaryBuilder&) = delete;

    void operator=(const SPAMSOnlineDictionaryBuilder&) = delete;

    SPAMSOnlineDictionaryBuilder(SPAMSOnlineDictionaryBuilder&&) = delete;

    void operator=(SPAMSOnlineDictionaryBuilder&&) = delete;
};

}// namespace mdk


#include "mdkSPAMSOnlineDictionaryBuilder.hpp"

#endif