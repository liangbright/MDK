#include "mdkMatrixCommon.h"


namespace mdk
{

std::vector<int64> span(int64 Index_A, int64 Index_B)
{
    std::vector<int64> IndexList;

    if (Index_A == Index_B)
    {
        IndexList.push_back(Index_A);
    }
    else if (Index_A < Index_B)
    {
        IndexList.reserve(Index_B - Index_A + 1);

        for (int64 i = Index_A; i <= Index_B; ++i)
        {
            IndexList.push_back(i);
        }
    }
    else //if (Index_A > Index_B)
    {
        IndexList.reserve(Index_A - Index_B + 1);

        for (int64 i = Index_A; i >= Index_B; --i)
        {
            IndexList.push_back(i);
        }
    }

    return IndexList;
}


std::vector<int64> span(int64 Index_A, int64 Step, int64 Index_B)
{
    std::vector<int64> IndexList;

    if (Index_A == Index_B && Step == 0)
    {
        IndexList.push_back(Index_A);
    }
    else if (Index_A < Index_B && Step > 0)
    {
        IndexList.reserve(Index_B - Index_A + 1);

        for (int64 i = Index_A; i <= Index_B; i += Step)
        {
            IndexList.push_back(i);
        }
    }
    else if (Index_A > Index_B && Step < 0)
    {
        IndexList.reserve(Index_A - Index_B + 1);

        for (int64 i = Index_A; i >= Index_B; i += Step)
        {
            IndexList.push_back(i);
        }
    }

    return IndexList;
}

}// namespace mdk
