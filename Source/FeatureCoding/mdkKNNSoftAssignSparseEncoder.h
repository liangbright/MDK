#ifndef __mdkKNNSoftAssignSparseEncoder_h
#define __mdkKNNSoftAssignSparseEncoder_h

#include <string>

#include "mdkFeatureSparseEncoder.h"


namespace mdk
{

template<typename ElementType>
class mdkKNNSoftAssignSparseEncoder : public mdkFeatureSparseEncoder<ElementType>
{

private: 

    int64 m_MaxNumberOfNeighbours;

public:

    mdkKNNSoftAssignSparseEncoder();

    ~mdkKNNSoftAssignSparseEncoder();

    //-----------------------------------------

    void Reset();

    void Clear();

    bool SetMaxNumberOfNeighbours(int64 MaxNumberOfNeighbours);

protected:

    bool GenerateCode();

private:
//deleted:
    mdkKNNSoftAssignSparseEncoder(const mdkKNNSoftAssignSparseEncoder&) = delete;

    void operator=(const mdkKNNSoftAssignSparseEncoder&) = delete;

};

}

#include "mdkKNNSoftAssignEncoder.hpp"

#endif