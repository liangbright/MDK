#ifndef __mdkFeatureCoding_Common_Type_h
#define __mdkFeatureCoding_Common_Type_h


namespace mdk
{

enum struct VectorSimilarityTypeEnum
{
    L1Distance,

    L2Distance,
    
    Correlation,
    AbsoluteValueOfCorrelation,
    
    UncenteredCorrelation,
    AbsoluteValueOfUncenteredCorrelation,

    UnnormalizedCorrelation,    
    AbsoluteValueOfUnnormalizedCorrelation,

    KLDivergence,

    JSDivergence,

    Unknown
};


}// namespace mdk


#endif