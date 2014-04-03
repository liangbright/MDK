#ifndef __mdkSPAMSSparseEncoder_h
#define __mdkSPAMSSparseEncoder_h

// based on : Online Dictionary Learning for Sparse Coding. Julien Mairal
//            SPAMS (SPArse Modeling Software) http://spams-devel.gforge.inria.fr/
//
// A=mexOMP(X,D,param);
// A=mexLasso(X,D,param);

#include <string>

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
        pos = false;
        cholesky = false;
        osl = false;
    }
};


template<typename ElementType>
class SPAMSSparseEncoder : public FeatureDictionaryBasedSparseEncoder<ElementType>
{
protected:
    std::string MethodName; // "OMP" or "Lasso"

public:
    OMP_Paramter_Of_SPAMSSparseEncoder<ElementType> m_Parameter_OMP;

    Lasso_Paramter_Of_SPAMSSparseEncoder<ElementType> m_Parameter_Lasso;

public:

    SPAMSSparseEncoder();

    ~SPAMSSparseEncoder();

    //--------------------------------------------------------------------------------

    void Clear();

    bool SelectMethod(const std::string& MethodName);

    bool CheckInputAndOutput();

    //--------------------------------------------------------------------------------

    using FeatureDictionaryBasedSparseEncoder::EncodingFunction;

    void inline EncodingFunction(const DenseMatrix<ElementType>& SingleFeatureDataVector,
                                 const FeatureDictionary<ElementType>& Dictionary,
                                 DenseMatrix<ElementType>& SingleFeatureCode,
                                 bool Flag_OutputCodeInCompactFormat);

    //---------------------------------------------------------------------------------

    static DenseMatrix<ElementType> Apply(const DenseMatrix<ElementType>* FeatureData, 
                                          const FeatureDictionary<ElementType>* Dictionary,
                                          int_max NeighbourNumber = 3,
                                          bool  Flag_OutputCodeInCompactFormat = false, // DenseFormat in default
                                          int_max MaxNumberOfThreads = 1);

    static bool Apply(DenseMatrix<ElementType>& OutputFeatureCode, 
                      const DenseMatrix<ElementType>* FeatureData,
                      const FeatureDictionary<ElementType>* Dictionary,
                      int_max NeighbourNumber = 3,
                      bool  Flag_OutputCodeInCompactFormat = false, // DenseFormat in default
                      int_max MaxNumberOfThreads = 1);

    static bool Apply(SparseMatrix<ElementType>& OutputFeatureCode,
                      const DenseMatrix<ElementType>* FeatureData,
                      const FeatureDictionary<ElementType>* Dictionary,
                      int_max NeighbourNumber = 3,
                      int_max MaxNumberOfThreads = 1);

private:
//deleted:
    SPAMSSparseEncoder(const SPAMSSparseEncoder&) = delete;

    void operator=(const SPAMSSparseEncoder&) = delete;

};

}

#include "mdkSPAMSSparseEncoder.hpp"

#endif