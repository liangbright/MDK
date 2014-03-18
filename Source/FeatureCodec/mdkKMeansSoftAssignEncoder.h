#ifndef __mdkKMeansSoftAssignEncoder_h
#define __mdkKMeansSoftAssignEncoder_h

#include <string>

#include "mdkFeatureEncoder.h"


namespace mdk
{

template<typename ElementType>
class mdkKMeansSoftAssignEncoder : public mdkFeatureEncoder<ElementType>
{

private: 

    int64 m_MaxNumberOfNeighbours;

public:

    mdkKMeansSoftAssignEncoder();

    ~mdkKMeansSoftAssignEncoder();

    //-----------------------------------------

    void Reset();

    void Clear();

    bool SetMaxNumberOfNeighbours(int64 MaxNumberOfNeighbours);

protected:

    bool GenerateCode();

private:
//deleted:
    mdkKMeansSoftAssignEncoder(const mdkKMeansSoftAssignEncoder&) = delete;

    void operator=(const mdkKMeansSoftAssignEncoder&) = delete;

};

}

#include "mdkKMeansSoftAssignEncoder.hpp"

#endif