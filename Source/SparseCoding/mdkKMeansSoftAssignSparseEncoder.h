#ifndef __mdkKMeansSoftAssignSparseEncoder_h
#define __mdkKMeansSoftAssignSparseEncoder_h

#include <string>

#include "mdkFeatureSparseEncoder.h"


namespace mdk
{

template<typename ElementType>
class mdkKMeansSoftAssignSparseEncoder : public mdkFeatureSparseEncoder<ElementType>
{

private: 

    int64 m_MaxNumberOfNeighbours;

public:

    mdkKMeansSoftAssignSparseEncoder();

    ~mdkKMeansSoftAssignSparseEncoder();

    //-----------------------------------------

    void Reset();

    void Clear();

    bool SetMaxNumberOfNeighbours(int64 MaxNumberOfNeighbours);

protected:

    bool GenerateCode();

private:
//deleted:
    mdkKMeansSoftAssignSparseEncoder(const mdkKMeansSoftAssignSparseEncoder&) = delete;

    void operator=(const mdkKMeansSoftAssignSparseEncoder&) = delete;

};

}

#include "mdkKMeansSoftAssignEncoder.hpp"

#endif