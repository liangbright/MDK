#pragma

namespace mdk
{

template<typename ElementType>
DenseMatrix<int_max> FindKNNByDistanceList(const DenseMatrix<ElementType>& DistanceList, int_max K_MaxNeighbourCount)
{
    DenseMatrix<int_max> NeighbourIndexList;
	if (K_MaxNeighbourCount <= 0)
	{
		return NeighbourIndexList;
	}

    int_max ElementCount = DistanceList.GetElementCount();

    //---------------------------------------------------------------------------------------------

    if (K_MaxNeighbourCount >= ElementCount)
    {
		NeighbourIndexList.Resize(1, ElementCount);
        for (int_max i = 0; i < ElementCount; ++i)
        {
            NeighbourIndexList[i] = i;
        }
        return NeighbourIndexList;
    }
    
    //---------------------------------------------------------------------------------------------

    if (K_MaxNeighbourCount == 1)
    {	
        int_max Index_min = 0;
        ElementType Distance_min = DistanceList[0];
        for (int_max i = 1; i < ElementCount; ++i)
        {
            if (DistanceList[i] < Distance_min)
            {
                Distance_min = DistanceList[i];
                Index_min = i;
            }
        }
		NeighbourIndexList.Resize(1, 1);
        NeighbourIndexList[0] = Index_min;
        return NeighbourIndexList;
    }

    // K_MaxNeighbourCount >= 2 and K_MaxNeighbourCount < ElementCount ---------------------------
	NeighbourIndexList.Resize(1, K_MaxNeighbourCount);
    for (int_max i = 0; i < K_MaxNeighbourCount; ++i)
    {
        NeighbourIndexList[i] = i;
    }

    for (int_max i = K_MaxNeighbourCount; i < ElementCount; ++i)
    {
        //------------------------------------------------------------
        int_max Index_max_in_NeighbourIndexList = 0;

        ElementType Distance_max_in_NeighbourIndexList = DistanceList[NeighbourIndexList[0]];

        for (int_max j = 1; j < K_MaxNeighbourCount; ++j)
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


template<typename ElementType>
DenseMatrix<int_max> FindKNNByDistanceList(const DenseMatrix<ElementType>& DistanceList, int_max K_MaxNeighbourCount, ElementType DistanceThreshold)
{
    DenseMatrix<int_max> tempNeighbourIndexList = FindKNNByDistanceList(DistanceList, K_MaxNeighbourCount);
    DenseMatrix<int_max> NeighbourIndexList;
    NeighbourIndexList.SetCapacity(tempNeighbourIndexList.GetElementCount());
    for (int_max i = 0; i < tempNeighbourIndexList.GetElementCount(); ++i)
    {
        if (DistanceList[tempNeighbourIndexList[i]] <= DistanceThreshold)
        {
            NeighbourIndexList.AppendCol(tempNeighbourIndexList[i]);
        }
    }    
    return NeighbourIndexList;
}


template<typename ElementType>
DenseMatrix<int_max> FindKNNBySimilarityList(const DenseMatrix<ElementType>& SimilarityList, int_max K_MaxNeighbourCount)
{
    DenseMatrix<ElementType> DistanceList = SimilarityList;
    DistanceList *= ElementType(-1);
    return FindKNNByDistanceList(DistanceList, K_MaxNeighbourCount);
}


template<typename ElementType>
DenseMatrix<int_max> FindKNNBySimilarityList(const DenseMatrix<ElementType>& SimilarityList, int_max K_MaxNeighbourCount, ElementType SimilarityThreshold)
{ 
    DenseMatrix<int_max> tempNeighbourIndexList = FindKNNBySimilarityList(SimilarityList, K_MaxNeighbourCount);
	DenseMatrix<int_max> NeighbourIndexList;
    NeighbourIndexList.SetCapacity(tempNeighbourIndexList.GetElementCount());
    for (int_max i = 0; i < tempNeighbourIndexList.GetElementCount(); ++i)
    {
        if (SimilarityList[tempNeighbourIndexList[i]] >= SimilarityThreshold)
        {
            NeighbourIndexList.AppendCol({ tempNeighbourIndexList[i] });
        }
    }
    return NeighbourIndexList;
}


}//namespace mdk

