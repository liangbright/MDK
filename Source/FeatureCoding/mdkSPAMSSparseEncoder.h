#ifndef __mdkSPAMSSparseEncoder_h
#define __mdkSPAMSSparseEncoder_h

// based on : Online Dictionary Learning for Sparse Coding. Julien Mairal
//            SPAMS (SPArse Modeling Software) http://spams-devel.gforge.inria.fr/
//
// A=mexOMP(X,D,param);
// A=mexLasso(X,D,param);

#include <string>

#include <spams/decomp/decomp.h>

#include "mdkFeatureDictionaryBasedSparseEncoder.h"
#include "mdkFeatureCoding_Common_Function.h"


namespace mdk
{

template<typename ElementType>
struct OMP_Paramter_Of_SPAMSSparseEncoder
{
    int_max L;
    ElementType eps;
    ElementType lambda;

    OMP_Paramter_Of_SPAMSSparseEncoder() { this->Clear(); }
    ~OMP_Paramter_Of_SPAMSSparseEncoder() {}

    void Clear()
    {
        L = -1;
        eps = 0;
        lambda = 0;
    }
};


template<typename ElementType>
struct Lasso_Paramter_Of_SPAMSSparseEncoder
{
    int_max mode;
    ElementType lambda;
    ElementType lambda2;
    int_max L;
    bool pos;
    bool cholesky;
    bool ols;

    Lasso_Paramter_Of_SPAMSSparseEncoder() { this->Clear(); }
    ~Lasso_Paramter_Of_SPAMSSparseEncoder() {}

    void Clear()
    {
        mode = 0;
        lambda = 0;
        lambda2 = 0;
        L = -1;
        pos = false;
        cholesky = false;
        ols = false;
    }
};


template<typename ElementType>
class SPAMSSparseEncoder : public FeatureDictionaryBasedSparseEncoder<ElementType>
{
public:

    std::string m_MethodName; // "OMP" or "Lasso"

    OMP_Paramter_Of_SPAMSSparseEncoder<ElementType> m_Parameter_OMP;

    Lasso_Paramter_Of_SPAMSSparseEncoder<ElementType> m_Parameter_Lasso;

public:

    SPAMSSparseEncoder();

    ~SPAMSSparseEncoder();

    //--------------------------------------------------------------------------------

    void Clear();

    bool CheckInput();

    //---------------------------------------------------------------------------------

protected:
    void GenerateCode_in_a_Thread(int_max IndexOfDataVector_start, int_max IndexOfDataVector_end, int_max ThreadIndex);

    inline void EncodingFunction(int_max DataIndex, int_max ThreadIndex);

private:
//deleted:
    SPAMSSparseEncoder(const SPAMSSparseEncoder&) = delete;

    void operator=(const SPAMSSparseEncoder&) = delete;

};

}

#include "mdkSPAMSSparseEncoder.hpp"

#endif