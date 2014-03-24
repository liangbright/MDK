#ifndef __mdkKNNSoftAssignSparseEncoder_h
#define __mdkKNNSoftAssignSparseEncoder_h

#include <string>

#include "mdkFeatureSparseEncoder.h"


namespace mdk
{

template<typename ElementType>
class KNNSoftAssignSparseEncoder : public FeatureSparseEncoder<ElementType>
{

private: 

    int64 m_MaxNumberOfNeighbours;

public:

    KNNSoftAssignSparseEncoder();

    ~KNNSoftAssignSparseEncoder();

    //-----------------------------------------

    void Clear();

    bool SetMaxNumberOfNeighbours(int64 MaxNumberOfNeighbours);

protected:

    bool GenerateCode();

private:
//deleted:
    KNNSoftAssignSparseEncoder(const KNNSoftAssignSparseEncoder&) = delete;

    void operator=(const KNNSoftAssignSparseEncoder&) = delete;

};

}

#include "mdkKNNSoftAssignEncoder.hpp"

#endif