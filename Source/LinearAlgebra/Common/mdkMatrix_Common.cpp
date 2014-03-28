#include "mdkMatrix_Common.h"

#include "mdkDenseMatrix.h"

namespace mdk
{

DenseMatrix<int64> span(int64 Index_A, int64 Index_B)
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

    DenseMatrix<int64> OutputIndexList;

    OutputIndexList.Take(IndexList);

    return OutputIndexList;
}


DenseMatrix<int64> span(int64 Index_A, int64 Step, int64 Index_B)
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

    DenseMatrix<int64> OutputIndexList;

    OutputIndexList.Take(IndexList);

    return OutputIndexList;
}

}// namespace mdk
