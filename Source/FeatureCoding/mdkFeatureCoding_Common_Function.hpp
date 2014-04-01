#ifndef __mdkFeatureCoding_Common_Function_hpp
#define __mdkFeatureCoding_Common_Function_hpp


//#include "mdkFeatureCoding_Common_Function.h"

namespace mdk
{

template<typename ElementType>
DenseMatrix<int_max> FindKNNByDistanceList(int_max K_NeighbourNumber, const DenseMatrix<ElementType>& DistanceList)
{
    DenseMatrix<int_max> NeighbourIndexList;

    int_max ElementNumber = DistanceList.GetElementNumber();

    if (K_NeighbourNumber > ElementNumber || K_NeighbourNumber <= 0)
    {
        MDK_Error("Invalid input @ mdkFeatureCoding FindKNNByDistanceList(...)")
        return NeighbourIndexList;
    }

    //---------------------------------------------------------------------------------------------

    NeighbourIndexList.Resize(1, K_NeighbourNumber);

    //---------------------------------------------------------------------------------------------

    if (K_NeighbourNumber == ElementNumber)
    {
        for (int_max i = 0; i < K_NeighbourNumber; ++i)
        {
            NeighbourIndexList[i] = i;
        }

        return NeighbourIndexList;
    }
    
    //---------------------------------------------------------------------------------------------

    if (K_NeighbourNumber == 1)
    {
        int_max Index_min = 0;

        ElementType Distance_min = DistanceList[0];

        for (int_max i = 1; i < ElementNumber; ++i)
        {
            if (DistanceList[i] < Distance_min)
            {
                Distance_min = DistanceList[i];

                Index_min = i;
            }
        }

        NeighbourIndexList[0] = Index_min;

        return NeighbourIndexList;
    }

    // K_NeighbourNumber >= 2 -------------------------------------------------------------------

    for (int_max i = 0; i < K_NeighbourNumber; ++i)
    {
        NeighbourIndexList[i] = i;
    }

    for (int_max i = K_NeighbourNumber; i < ElementNumber; ++i)
    {

        //------------------------------------------------------------
        int_max Index_max_in_NeighbourIndexList;

        ElementType Distance_max_in_NeighbourIndexList = DistanceList[NeighbourIndexList[0]];

        for (int_max j = 1; j < K_NeighbourNumber; ++j)
        {
            if (DistanceList[NeighbourIndexList[j]] > Distance_max_in_NeighbourIndexList)
            {
                Distance_max_in_NeighbourIndexList = DistanceList[NeighbourIndexList[j]];

                Index_max_in_NeighbourIndexList = j;
            }
        }
        //------------------------------------------------------------

        if (DistanceList[i] < Distance_max_in_NeighbourIndexList)
        {
            NeighbourIndexList[Index_max_in_NeighbourIndexList] = i;
        }
    }

    return NeighbourIndexList;
}

}


#endif