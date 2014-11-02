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

template<typename ScalarType>
struct OMP_Paramter_Of_SPAMSSparseEncoder
{
    int_max L;
    ScalarType eps;
    ScalarType lambda;

    OMP_Paramter_Of_SPAMSSparseEncoder() { this->Clear(); }
    ~OMP_Paramter_Of_SPAMSSparseEncoder() {}

    void Clear()
    {
        L = -1;
        eps = 0;
        lambda = 0;
    }
};


template<typename ScalarType>
struct Lasso_Paramter_Of_SPAMSSparseEncoder
{
    int_max mode;
    ScalarType lambda;
    ScalarType lambda2;
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


template<typename Scalar_Type>
class SPAMSSparseEncoder : public FeatureDictionaryBasedSparseEncoder<Scalar_Type>
{
public:
	typedef Scalar_Type ScalarType;

public:
    std::string m_MethodName; // "OMP" or "Lasso"

    OMP_Paramter_Of_SPAMSSparseEncoder<ScalarType> m_Parameter_OMP;

    Lasso_Paramter_Of_SPAMSSparseEncoder<ScalarType> m_Parameter_Lasso;

public:

    SPAMSSparseEncoder();
    ~SPAMSSparseEncoder();

    //--------------------------------------------------------------------------------

    void Clear();
    bool CheckInput();
	inline SparseVector<ScalarType> EncodeSingleDataVector(const DenseMatrix<ScalarType>& DataColVector);
    //---------------------------------------------------------------------------------

protected:
    void GenerateCode_in_a_Thread(int_max IndexOfDataVector_start, int_max IndexOfDataVector_end, int_max ThreadIndex);
	inline SparseVector<ScalarType> EncodeSingleDataVector(int_max DataIndex, const DenseMatrix<ScalarType>& DataColVector, int_max ThreadIndex);

private:
//deleted:
    SPAMSSparseEncoder(const SPAMSSparseEncoder&) = delete;
    void operator=(const SPAMSSparseEncoder&) = delete;
};

}

#include "mdkSPAMSSparseEncoder.hpp"

#endif
