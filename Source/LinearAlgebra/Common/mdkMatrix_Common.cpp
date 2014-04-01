#include "mdkMatrix_Common.h"

namespace mdk
{

std::vector<int_max> span(int_max Index_A, int_max Index_B)
{
    std::vector<int_max> IndexList;

    if (Index_A == Index_B)
    {
        IndexList.push_back(Index_A);
    }
    else if (Index_A < Index_B)
    {
        IndexList.reserve(Index_B - Index_A + 1);

        for (int_max i = Index_A; i <= Index_B; ++i)
        {
            IndexList.push_back(i);
        }
    }
    else //if (Index_A > Index_B)
    {
        IndexList.reserve(Index_A - Index_B + 1);

        for (int_max i = Index_A; i >= Index_B; --i)
        {
            IndexList.push_back(i);
        }
    }

    return IndexList;
}


std::vector<int_max> span(int_max Index_A, int_max Step, int_max Index_B)
{
    std::vector<int_max> IndexList;

    if (Index_A == Index_B && Step == 0)
    {
        IndexList.push_back(Index_A);
    }
    else if (Index_A < Index_B && Step > 0)
    {
        IndexList.reserve(Index_B - Index_A + 1);

        for (int_max i = Index_A; i <= Index_B; i += Step)
        {
            IndexList.push_back(i);
        }
    }
    else if (Index_A > Index_B && Step < 0)
    {
        IndexList.reserve(Index_A - Index_B + 1);

        for (int_max i = Index_A; i >= Index_B; i += Step)
        {
            IndexList.push_back(i);
        }
    }

    return IndexList;
}

}// namespace mdk
