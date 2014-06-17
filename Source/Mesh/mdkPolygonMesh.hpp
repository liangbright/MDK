#ifndef __mdkPolygonMesh_hpp
#define __mdkPolygonMesh_hpp


namespace mdk
{

template<typename ScalarType>
PolygonMesh<ScalarType>::PolygonMesh()
{
    m_MeshData = std::make_shared<PolygonMeshData<ScalarType>>();
    m_MeshData->IsTriangleMesh = false;
}

template<typename ScalarType>
PolygonMesh<ScalarType>::PolygonMesh(const Pure_Empty_PolygonMesh_Symbol&)
{
}

template<typename ScalarType>
PolygonMesh<ScalarType>::PolygonMesh(const PolygonMesh<ScalarType>& InputMesh)
{
    this->Copy(InputMesh);
}


template<typename ScalarType>
PolygonMesh<ScalarType>::PolygonMesh(PolygonMesh<ScalarType>&& InputMesh)
{
    m_MeshData = std::move(InputMesh.m_MeshData);
}


template<typename ScalarType>
PolygonMesh<ScalarType>::~PolygonMesh()
{
}


template<typename ScalarType>
inline
void PolygonMesh<ScalarType>::operator=(const PolygonMesh<ScalarType>& InputMesh)
{
    this->Copy(InputMesh);
}


template<typename ScalarType>
inline
void PolygonMesh<ScalarType>::operator=(PolygonMesh<ScalarType>&& InputMesh)
{
    if (!m_MeshData)
    {
        m_MeshData = std::make_shared<PolygonMeshData<ScalarType>>();
    }

    this->Take(std::forward<PolygonMesh<ScalarType>&>(InputMesh));
}


template<typename ScalarType>
bool PolygonMesh<ScalarType>::Construct(DenseMatrix<ScalarType> InputVertexPositionTable, DataArray<DenseVector<int_max>> InputVertexIndexTable)
{
    if (InputVertexPositionTable.IsEmpty() == true || InputVertexIndexTable.IsEmpty() == true)
    {
        MDK_Error("InputVertexPositionTable or InputVertexIndexTable is empty @ PolygonMesh::Construct(...)")
        return false;
    }

    if (InputVertexPositionTable.GetRowNumber() != 3 || 3 * InputVertexIndexTable.GetElementNumber() < InputVertexPositionTable.GetColNumber())
    {
        MDK_Error("InputVertexPositionTable or InputVertexIndexTable is invalid @ PolygonMesh::Construct(...)")
        return false;
    }

    //---------------------------------------------------------------------------------------------------------------//

    m_MeshData->VertexPositionTable = std::move(InputVertexPositionTable);
    m_MeshData->VertexIndexTable = std::move(InputVertexIndexTable);

    auto TempHalfEdgeList = this->ConstructTempHalfEdgeList();
    if (TempHalfEdgeList.IsEmpty() == true)
    {
        return false;
    }

    if (this->ConstructPolygonList(TempHalfEdgeList) == false)
    {
        return false;
    }

    if (this->ConstructEdgeList(TempHalfEdgeList) == false)
    {
        return false;
    }

    if (this->ConstructHalfEdgeList(TempHalfEdgeList) == false)
    {
        return false;
    }

    if (this->ConstructVertexList() == false)
    {
        return false;
    }

    this->CheckIfTriangleMesh();

    return true;
}


template<typename ScalarType>
DataArray<DenseVector<int_max>> PolygonMesh<ScalarType>::ConstructTempHalfEdgeList()
{
    //---------------------------------------------------------------------------------------//
    // Output: TempHalfEdgeList
    // TempHalfEdgeList[k] is a half edge, and k is HalfEdgeIndex
    // TempHalfEdgeList[k][0] : PolygonIndex
    // TempHalfEdgeList[k][1] : VertexIndex_0
    // TempHalfEdgeList[k][2] : VertexIndex_1
    // VertexIndex_0 < VertexIndex_1 always
    //----------------------------------------------------------------------------------------//

    const DataArray<DenseVector<int_max>>& TempPolygonList = m_MeshData->VertexIndexTable;
    auto PolygonNumber = TempPolygonList.GetElementNumber();

    DataArray<DenseVector<int_max>> TempHalfEdgeList;
    TempHalfEdgeList.ReserveCapacity(PolygonNumber * 3);

    int_max HalfEdgeIndex = 0;

    for (int_max k = 0; k < PolygonNumber; ++k)
    {
        auto VertexNumber_k = TempPolygonList[k].GetElementNumber();

        const DenseVector<int_max>& Polygon_k = TempPolygonList[k];

        for (int_max n = 0; n < VertexNumber_k - 1; ++n)
        {
            int_max VertexIndex_a = Polygon_k[n];
            int_max VertexIndex_b = Polygon_k[n + 1];

            if (VertexIndex_a < VertexIndex_b)
            {
                TempHalfEdgeList.Append({ k, VertexIndex_a, VertexIndex_b });
            }
            else
            {
                TempHalfEdgeList.Append({ k, VertexIndex_b, VertexIndex_a });
            }

            HalfEdgeIndex += 1;
        }

        // HalfEdge between VertexIndex_0 and VertexIndex_end
        int_max VertexIndex_0 = Polygon_k[0];
        int_max VertexIndex_end = Polygon_k[VertexNumber_k - 1];

        if (VertexIndex_0 < VertexIndex_end)
        {
            TempHalfEdgeList.Append({ k, VertexIndex_0, VertexIndex_end });
        }
        else
        {
            TempHalfEdgeList.Append({ k, VertexIndex_end, VertexIndex_0 });
        }

        HalfEdgeIndex += 1;
    }

    TempHalfEdgeList.SortInPlace([](const DenseVector<int_max>& A, const DenseVector<int_max>& B)
    {
        if (A[1] == B[1]) // VertexIndex_0 of A == VertexIndex_0 of B
        {
            return A[2] < B[2];
        }
        else
        {
            return A[1] < B[1];
        }
    });

    return TempHalfEdgeList;
}


template<typename ScalarType>
bool PolygonMesh<ScalarType>::
ConstructPolygonList(const DataArray<DenseVector<int_max>>& TempHalfEdgeList)
{
    const DataArray<DenseVector<int_max>>& TempPolygonList = m_MeshData->VertexIndexTable;

    auto PolygonNumber = TempPolygonList.GetLength();

    DataArray<DenseVector<int_max>> HalfEdgeIndexListOfEachPolygon;
    HalfEdgeIndexListOfEachPolygon.FastResize(PolygonNumber);
    for (int_max k = 0; k < TempHalfEdgeList.GetLength(); ++k)
    {
        auto tempPlygonIndex_k = TempHalfEdgeList[k][0];
        HalfEdgeIndexListOfEachPolygon[tempPlygonIndex_k].Append(k);
    }

    m_MeshData->PolygonList.FastResize(PolygonNumber);

    for (int_max PolygonIndex = 0; PolygonIndex < PolygonNumber; ++PolygonIndex)
    {      
        const DenseVector<int_max>& HalfEdgeIndexList = HalfEdgeIndexListOfEachPolygon[PolygonIndex];

        DenseVector<int_max> HalfEdgeIndexList_reorder;
        HalfEdgeIndexList_reorder.FastResize(HalfEdgeIndexList.GetLength());
        HalfEdgeIndexList_reorder.Fill(-1);

        const DenseVector<int_max>& VertexIndexList = TempPolygonList[PolygonIndex];

        DenseVector<int_max> FlagList;

        for (int_max k = 0; k < VertexIndexList.GetLength(); ++k)
        {
            auto VertexIndex_0 = VertexIndexList[k];

            int_max VertexIndex_1;
            if (k < VertexIndexList.GetLength() - 1)
            {
                VertexIndex_1 = VertexIndexList[k + 1];
            }
            else
            {
                VertexIndex_1 = VertexIndexList[0];
            }

            FlagList.FastResize(HalfEdgeIndexList.GetLength());
            FlagList.Fill(1);

            int_max HalfEdgeIndex_k = -1;

            for (int_max m = 0; m < HalfEdgeIndexList.GetLength(); ++m)
            {
                auto tempVertexIndex_0 = TempHalfEdgeList[HalfEdgeIndexList[m]][1];
                auto tempVertexIndex_1 = TempHalfEdgeList[HalfEdgeIndexList[m]][2];

                if (FlagList[m] > 0)
                {
                    if ((tempVertexIndex_0 == VertexIndex_0 && tempVertexIndex_1 == VertexIndex_1)
                        || (tempVertexIndex_0 == VertexIndex_1 && tempVertexIndex_1 == VertexIndex_0))
                    {
                        FlagList[m] = 0;
                        HalfEdgeIndex_k = HalfEdgeIndexList[m];
                        break;
                    }
                }
            }

            if (HalfEdgeIndex_k < 0)
            {
                MDK_Error("HalfEdgeIndex_k < 0 @ PolygonMesh<ScalarType>::ConstructPolygonList(...)")
                return false;
            }

            HalfEdgeIndexList_reorder[k] = HalfEdgeIndex_k;
        }

        m_MeshData->PolygonList[PolygonIndex].Create();
        m_MeshData->PolygonList[PolygonIndex].SetParentMesh(*this);
        m_MeshData->PolygonList[PolygonIndex].SetIndex(PolygonIndex);
        m_MeshData->PolygonList[PolygonIndex].SetHalfEdgeIndexList(std::move(HalfEdgeIndexList_reorder));
    }

    m_MeshData->PolygonValidityFlagList.FastResize(PolygonNumber);
    m_MeshData->PolygonValidityFlagList.Fill(1);

    return true;
}


template<typename ScalarType>
bool PolygonMesh<ScalarType>::ConstructEdgeList(const DataArray<DenseVector<int_max>>& TempHalfEdgeList)
{
    auto TempFunction_SameEdge = [](const DenseVector<int_max>& A, const DenseVector<int_max>& B)
    {
        if (A[1] == B[1])
        {
            if (A[2] == B[2])
            {
                return true;
            }
        }
        return false;
    };
    //-----------------------------------------------------------------------------------

    auto HalfEdgeNumber = TempHalfEdgeList.GetElementNumber();

    DenseVector<int_max> HalfEdgeFlagList;
    HalfEdgeFlagList.FastResize(HalfEdgeNumber);
    HalfEdgeFlagList.Fill(0);

    HalfEdgeFlagList[0] = 1;

    for (int_max k = 1; k < HalfEdgeNumber; ++k)
    {
        if (TempFunction_SameEdge(TempHalfEdgeList[k - 1], TempHalfEdgeList[k]) == false)
        {
            HalfEdgeFlagList[k] = 1;
        }
        else
        {
            if (k < HalfEdgeNumber - 1)
            {
                if (TempFunction_SameEdge(TempHalfEdgeList[k], TempHalfEdgeList[k + 1]) == true)
                {
                    MDK_Error("Polygon Data is invalid @ PolygonMesh::ConstructEdgeList()")
                    return false;
                }
            }
        }
    }

    int_max EdgeNumber = HalfEdgeFlagList.Sum();

    m_MeshData->EdgeList.FastResize(EdgeNumber);

    int_max EdgeIndex_current = 0;

    for (int_max HalfEdgeIndex = 0; HalfEdgeIndex < HalfEdgeNumber; ++HalfEdgeIndex)
    {
        if (HalfEdgeFlagList[HalfEdgeIndex] == 1)
        {
            int_max HalfEdgeIndex_opposite = -1;

            if (HalfEdgeIndex < HalfEdgeNumber - 1)
            {
                if (TempFunction_SameEdge(TempHalfEdgeList[HalfEdgeIndex], TempHalfEdgeList[HalfEdgeIndex + 1]) == true)
                {
                    HalfEdgeIndex_opposite = HalfEdgeIndex + 1;
                }
            }

            m_MeshData->EdgeList[EdgeIndex_current].Create();
            m_MeshData->EdgeList[EdgeIndex_current].SetParentMesh(*this);
            m_MeshData->EdgeList[EdgeIndex_current].SetIndex(EdgeIndex_current);
            m_MeshData->EdgeList[EdgeIndex_current].SetHalfEdgeIndexList(HalfEdgeIndex, HalfEdgeIndex_opposite);
            EdgeIndex_current += 1;
        }
    }

    m_MeshData->EdgeValidityFlagList.FastResize(EdgeNumber);
    m_MeshData->EdgeValidityFlagList.Fill(1);

    return true;
}


template<typename ScalarType>
bool PolygonMesh<ScalarType>::
ConstructHalfEdgeList(const DataArray<DenseVector<int_max>>& TempHalfEdgeList)
{
    auto TempFunction_SameEdge = [](const DenseVector<int_max>& A, const DenseVector<int_max>& B)
    {
        if (A[1] == B[1])
        {
            if (A[2] == B[2])
            {
                return true;
            }
        }
        return false;
    };
    //--------------------------------------------------------------------------------------------

    const DataArray<DenseVector<int_max>>& TempPolygonList = m_MeshData->VertexIndexTable;
    auto HalfEdgeNumber = TempHalfEdgeList.GetElementNumber();

    DenseVector<int_max> Map_HalfEdgeIndexToEdgeIndex;
    Map_HalfEdgeIndexToEdgeIndex.FastResize(HalfEdgeNumber);
    Map_HalfEdgeIndexToEdgeIndex.Fill(-1);
    for (int_max k = 0; k < m_MeshData->EdgeList.GetLength(); ++k)
    {
        int_max tempHalfEdgeIndex0 = -1;
        int_max tempHalfEdgeIndex1 = -1;
        m_MeshData->EdgeList[k].GetHalfEdgeIndexList(tempHalfEdgeIndex0, tempHalfEdgeIndex1);
        if (tempHalfEdgeIndex0 > 0) { Map_HalfEdgeIndexToEdgeIndex[tempHalfEdgeIndex0] = k; }
        if (tempHalfEdgeIndex1 > 0) { Map_HalfEdgeIndexToEdgeIndex[tempHalfEdgeIndex1] = k; }
    }

    m_MeshData->HalfEdgeList.FastResize(HalfEdgeNumber);
    for (int_max HalfEdgeIndex = 0; HalfEdgeIndex < HalfEdgeNumber; ++HalfEdgeIndex)
    {
        const DenseVector<int_max>& tempHalfEdge = TempHalfEdgeList[HalfEdgeIndex];

        int_max HalfEdgeIndex_opposite = -1;
        if (HalfEdgeIndex == 0)
        {
            if (TempFunction_SameEdge(TempHalfEdgeList[HalfEdgeIndex], TempHalfEdgeList[HalfEdgeIndex + 1]) == true)
            {
                HalfEdgeIndex_opposite = HalfEdgeIndex + 1;
            }
        }
        else if (HalfEdgeIndex == HalfEdgeNumber - 1)
        {
            if (TempFunction_SameEdge(TempHalfEdgeList[HalfEdgeIndex], TempHalfEdgeList[HalfEdgeIndex - 1]) == true)
            {
                HalfEdgeIndex_opposite = HalfEdgeIndex - 1;
            }
        }
        else // if (HalfEdgeIndex > 0 && HalfEdgeIndex < HalfEdgeNumber - 1)
        {
            if (TempFunction_SameEdge(TempHalfEdgeList[HalfEdgeIndex], TempHalfEdgeList[HalfEdgeIndex + 1]) == true)
            {
                HalfEdgeIndex_opposite = HalfEdgeIndex + 1;
            }
            else if (TempFunction_SameEdge(TempHalfEdgeList[HalfEdgeIndex], TempHalfEdgeList[HalfEdgeIndex - 1]) == true)
            {
                HalfEdgeIndex_opposite = HalfEdgeIndex - 1;
            }
        }

        auto PolygonIndex = tempHalfEdge[0];

        const DenseVector<int_max>& VertexIndexListOfPolygon = TempPolygonList[PolygonIndex];

        const DenseVector<int_max>& HalfEdgeIndexListOfPolygon = m_MeshData->PolygonList[PolygonIndex].GetHalfEdgeIndexList();

        int_max tempRelativeIndex = -1;
        for (int_max k = 0; k < HalfEdgeIndexListOfPolygon.GetLength(); ++k)
        {
            if (HalfEdgeIndexListOfPolygon[k] == HalfEdgeIndex)
            {
                tempRelativeIndex = k;
                break;
            }
        }

        auto VertexIndex_start_of_halfedge = VertexIndexListOfPolygon[tempRelativeIndex];

        int_max VertexIndex_end_of_halfedge = -1;
        int_max HalfEdgeIndex_next = -1;
        if (tempRelativeIndex < HalfEdgeIndexListOfPolygon.GetLength() - 1)
        {
            HalfEdgeIndex_next = HalfEdgeIndexListOfPolygon[tempRelativeIndex + 1];
            VertexIndex_end_of_halfedge = VertexIndexListOfPolygon[tempRelativeIndex + 1];
        }
        else
        {
            HalfEdgeIndex_next = HalfEdgeIndexListOfPolygon[0];
            VertexIndex_end_of_halfedge = VertexIndexListOfPolygon[0];
        }

        int_max HalfEdgeIndex_prev = -1;
        if (tempRelativeIndex >= 1)
        {
            HalfEdgeIndex_prev = HalfEdgeIndexListOfPolygon[tempRelativeIndex - 1];
        }
        else
        {
            HalfEdgeIndex_prev = HalfEdgeIndexListOfPolygon[HalfEdgeIndexListOfPolygon.GetLength() - 1];
        }

        m_MeshData->HalfEdgeList[HalfEdgeIndex].Create();
        m_MeshData->HalfEdgeList[HalfEdgeIndex].SetParentMesh(*this);
        m_MeshData->HalfEdgeList[HalfEdgeIndex].SetIndex(HalfEdgeIndex);
        m_MeshData->HalfEdgeList[HalfEdgeIndex].SetStartVertexIndex(VertexIndex_start_of_halfedge);
        m_MeshData->HalfEdgeList[HalfEdgeIndex].SetEndVertexIndex(VertexIndex_end_of_halfedge);
        m_MeshData->HalfEdgeList[HalfEdgeIndex].SetEdgeIndex(Map_HalfEdgeIndexToEdgeIndex[HalfEdgeIndex]);
        m_MeshData->HalfEdgeList[HalfEdgeIndex].SetPolygonIndex(PolygonIndex);
        m_MeshData->HalfEdgeList[HalfEdgeIndex].SetOppositeHalfEdgeIndex(HalfEdgeIndex_opposite);
        m_MeshData->HalfEdgeList[HalfEdgeIndex].SetNextHalfEdgeIndex(HalfEdgeIndex_next);
        m_MeshData->HalfEdgeList[HalfEdgeIndex].SetPreviousHalfEdgeIndex(HalfEdgeIndex_prev);
    }

    m_MeshData->HalfEdgeValidityFlagList.FastResize(HalfEdgeNumber);
    m_MeshData->HalfEdgeValidityFlagList.Fill(1);

    return true;
}


template<typename ScalarType>
bool PolygonMesh<ScalarType>::ConstructVertexList()
{
    auto VertexNumber   = m_MeshData->VertexPositionTable.GetColNumber();
    auto EdgeNumber     = m_MeshData->EdgeList.GetLength();
    auto HalfEdgeNumber = m_MeshData->HalfEdgeList.GetLength();
    auto PolygonNumber  = m_MeshData->PolygonList.GetLength();

    m_MeshData->VertexList.FastResize(VertexNumber);

    for (int_max VertexIndex = 0; VertexIndex < VertexNumber; ++VertexIndex)
    {
        m_MeshData->VertexList[VertexIndex].Create();
        m_MeshData->VertexList[VertexIndex].SetParentMesh(*this);
        m_MeshData->VertexList[VertexIndex].SetIndex(VertexIndex);
    }
   
    //------------------------ Adjacency_VertexToVertex --------------------------------//

    DataArray<DenseVector<int_max>> Adjacency_VertexToVertex;
    Adjacency_VertexToVertex.FastResize(VertexNumber);
    for (int_max k = 0; k < VertexNumber; ++k)
    {
        Adjacency_VertexToVertex[k].ReserveCapacity(10);
    }

    for (int_max k = 0; k < EdgeNumber; ++k)
    {
        int_max tempVertexIndexList[2];
        m_MeshData->EdgeList[k].GetVertexIndexList(tempVertexIndexList);
        Adjacency_VertexToVertex[tempVertexIndexList[0]].Append(tempVertexIndexList[1]);
        Adjacency_VertexToVertex[tempVertexIndexList[1]].Append(tempVertexIndexList[0]);
    }

    for (int_max k = 0; k < VertexNumber; ++k)
    {
        DenseVector<int_max>& VertexIndexList = Adjacency_VertexToVertex[k];
        VertexIndexList.ReleaseUnusedCapacity();
        m_MeshData->VertexList[k].SetAdjacentVertexIndexList(std::move(VertexIndexList));
    }

    //------------------------ Adjacency_VertexToOutgoingHalfEdge --------------------------------//

    DataArray<DenseVector<int_max>> Adjacency_VertexToOutgoingHalfEdge;
    Adjacency_VertexToOutgoingHalfEdge.FastResize(VertexNumber);
    for (int_max k = 0; k < VertexNumber; ++k)
    {
        Adjacency_VertexToOutgoingHalfEdge[k].ReserveCapacity(10);
    }

    for (int_max k = 0; k < HalfEdgeNumber; ++k)
    {
        auto VertexIndex_k_start = m_MeshData->HalfEdgeList[k].GetStartVertexIndex();
        Adjacency_VertexToOutgoingHalfEdge[VertexIndex_k_start].Append(k);
    }

    for (int_max k = 0; k < VertexNumber; ++k)
    {
        DenseVector<int_max>& OutgoingHalfEdgeIndexList = Adjacency_VertexToOutgoingHalfEdge[k];
        OutgoingHalfEdgeIndexList.ReleaseUnusedCapacity();
        m_MeshData->VertexList[k].SetOutgoingHalfEdgeIndexList(std::move(OutgoingHalfEdgeIndexList));
    }

    //------------------------ Adjacency_VertexToIncomingHalfEdge --------------------------------//

    DataArray<DenseVector<int_max>> Adjacency_VertexToIncomingHalfEdge;
    Adjacency_VertexToIncomingHalfEdge.FastResize(VertexNumber);
    for (int_max k = 0; k < VertexNumber; ++k)
    {
        Adjacency_VertexToIncomingHalfEdge[k].ReserveCapacity(10);
    }

    for (int_max k = 0; k < HalfEdgeNumber; ++k)
    {
        auto VertexIndex_k_end = m_MeshData->HalfEdgeList[k].GetEndVertexIndex();
        Adjacency_VertexToIncomingHalfEdge[VertexIndex_k_end].Append(k);
    }

    for (int_max k = 0; k < VertexNumber; ++k)
    {
        DenseVector<int_max>& IncomingHalfEdgeIndexList = Adjacency_VertexToIncomingHalfEdge[k];
        IncomingHalfEdgeIndexList.ReleaseUnusedCapacity();
        m_MeshData->VertexList[k].SetOutgoingHalfEdgeIndexList(std::move(IncomingHalfEdgeIndexList));
    }

    //--------------------------- Adjacency_VertexToEdge -------------------------------------//

    DataArray<DenseVector<int_max>> Adjacency_VertexToEdge;
    Adjacency_VertexToEdge.FastResize(VertexNumber);
    for (int_max k = 0; k < VertexNumber; ++k)
    {
        Adjacency_VertexToEdge[k].ReserveCapacity(10);
    }

    for (int_max k = 0; k < EdgeNumber; ++k)
    {
        int_max tempIndexList[2];
        m_MeshData->EdgeList[k].GetVertexIndexList(tempIndexList);
        Adjacency_VertexToEdge[tempIndexList[0]].Append(k);
        Adjacency_VertexToEdge[tempIndexList[1]].Append(k);
    }

    for (int_max k = 0; k < VertexNumber; ++k)
    {
        DenseVector<int_max>& EdgeIndexList = Adjacency_VertexToEdge[k];
        EdgeIndexList.ReleaseUnusedCapacity();
        m_MeshData->VertexList[k].SetAdjacentEdgeIndexList(std::move(EdgeIndexList));
    }

    //--------------------------- Adjacency_VertexToPolygon -------------------------------------//

    DataArray<DenseVector<int_max>> Adjacency_VertexToPolygon;
    Adjacency_VertexToPolygon.FastResize(VertexNumber);
    for (int_max k = 0; k < VertexNumber; ++k)
    {
        Adjacency_VertexToPolygon[k].ReserveCapacity(10);
    }

    for (int_max k = 0; k < PolygonNumber; ++k)
    {
        auto tempIndexList = m_MeshData->PolygonList[k].GetVertexIndexList();
        for (int_max m = 0; m < tempIndexList.GetLength(); ++m)
        {
            Adjacency_VertexToPolygon[tempIndexList[m]].Append(k);
        }
    }

    for (int_max k = 0; k < VertexNumber; ++k)
    {
        DenseVector<int_max>& PolygonIndexList = Adjacency_VertexToPolygon[k];
        PolygonIndexList.ReleaseUnusedCapacity();
        m_MeshData->VertexList[k].SetAdjacentPolygonIndexList(std::move(PolygonIndexList));
    }

    //---------------------------------------------------------------------------------//

    m_MeshData->VertexValidityFlagList.FastResize(VertexNumber);
    m_MeshData->VertexValidityFlagList.Fill(1);

    return true;
}


template<typename ScalarType>
inline
bool PolygonMesh<ScalarType>::CheckIfTriangleMesh() const
{
    bool SelfIsTriangleMesh = true;

    for (int_max k = 0; k < m_MeshData->PolygonList.GetLength(); ++k)
    {
        if (m_MeshData->PolygonValidityFlagList[k] == 1)
        {
            if (m_MeshData->PolygonList[k].IsTriangle() == false)
            {
                SelfIsTriangleMesh = false;
                break;
            }
        }
    }

    m_MeshData->IsTriangleMesh = SelfIsTriangleMesh;
     
    return SelfIsTriangleMesh;
}


template<typename ScalarType>
inline
void PolygonMesh<ScalarType>::CleanDataStructure()
{

}


template<typename ScalarType>
inline 
void PolygonMesh<ScalarType>::Clear()
{
    if (!m_MeshData)
    {
        return;
    }

    m_MeshData->IsTriangleMesh = false;

    m_MeshData->VertexGlobalIDList.Clear();
    m_MeshData->PolygonGlobalIDList.Clear();
    
    m_MeshData->VertexPositionTable.Clear();

    m_MeshData->VertexList.Clear();
    m_MeshData->PolygonList.Clear();
    m_MeshData->EdgeList.Clear();
    m_MeshData->HalfEdgeList.Clear();

    m_MeshData->MeshAttribute.NormalAtVertex.Clear();
    m_MeshData->MeshAttribute.NormalAtPolygon.Clear();
}


template<typename ScalarType>
template<typename ScalarType_Input>
inline 
void PolygonMesh<ScalarType>::Copy(const PolygonMesh<ScalarType_Input>& InputMesh)
{
    m_MeshData->IsTriangleMesh = InputMesh.m_MeshData->IsTriangleMesh;

    m_MeshData->VertexGlobalIDList  = InputMesh.m_MeshData->VertexGlobalIDList;
    m_MeshData->PolygonGlobalIDList = InputMesh.m_MeshData->PolygonGlobalIDList;

    m_MeshData->VertexPositionTable = InputMesh.m_MeshData->VertexPositionTable;
    m_MeshData->VertexIndexTable    = InputMesh.m_MeshData->VertexIndexTable;

    m_MeshData->VertexList = InputMesh.m_MeshData->VertexList;
    m_MeshData->VertexValidityFlagList = InputMesh.m_MeshData->VertexValidityFlagList;

    m_MeshData->PolygonList = InputMesh.m_MeshData->PolygonList;
    m_MeshData->PolygonValidityFlagList = InputMesh.m_MeshData->PolygonValidityFlagList;

    m_MeshData->EdgeList = InputMesh.m_MeshData->EdgeList;
    m_MeshData->EdgeValidityFlagList = InputMesh.m_MeshData->EdgeValidityFlagList;

    m_MeshData->HalfEdgeList = InputMesh.m_MeshData->HalfEdgeList;
    m_MeshData->HalfEdgeValidityFlagList = InputMesh.m_MeshData->HalfEdgeValidityFlagList;
    
    this->UpdateParentMeshPointerInMeshItem();

    m_MeshData->MeshAttribute.NormalAtVertex  = InputMesh.m_MeshData->MeshAttribute.NormalAtVertex;
    m_MeshData->MeshAttribute.NormalAtPolygon = InputMesh.m_MeshData->MeshAttribute.NormalAtPolygon;
}


template<typename ScalarType>
template<typename ScalarType_Input>
inline 
bool PolygonMesh<ScalarType>::Copy(const PolygonMesh<ScalarType_Input>* InputMesh)
{
    if (InputMesh == nullptr)
    {
        MDK_Error("Input is nullptr @ PolygonMesh::Copy(...)")
        return false;
    }

    this->Copy(*InputMesh);

    return true;
}


template<typename ScalarType>
inline 
void PolygonMesh<ScalarType>::Share(PolygonMesh& InputMesh)
{
    m_MeshData = InputMesh.m_MeshData;
}


template<typename ScalarType>
inline
bool PolygonMesh<ScalarType>::Share(PolygonMesh* InputMesh)
{
    if (InputMesh == nullptr)
    {
        MDK_Error("Input is nullptr @ PolygonMesh::Share(...)")
        return false;
    }

    this->Share(*InputMesh);

    return true;
}


template<typename ScalarType>
inline 
void PolygonMesh<ScalarType>::ForceShare(const PolygonMesh<ScalarType>& InputMesh)
{
    m_MeshData = InputMesh.m_MeshData;
}


template<typename ScalarType>
inline 
bool PolygonMesh<ScalarType>::ForceShare(const PolygonMesh<ScalarType>* InputMesh)
{
    if (InputMesh == nullptr)
    {
        MDK_Error("Input is nullptr @ PolygonMesh::ForceShare(...)")
        return false;
    }

    this->ForceShare(*InputMesh);

    return true;
}


template<typename ScalarType>
inline
void PolygonMesh<ScalarType>::Take(PolygonMesh<ScalarType>&& InputMesh)
{
    Take(std::forward<PolygonMesh<ScalarType>&>(InputMesh));
}


template<typename ScalarType>
inline 
bool PolygonMesh<ScalarType>::Take(PolygonMesh<ScalarType>& InputMesh)
{
    m_MeshData->IsTriangleMesh = InputMesh.m_MeshData->IsTriangleMesh;

    m_MeshData->VertexGlobalIDList  = std::move(InputMesh.m_MeshData->VertexGlobalIDList);
    m_MeshData->PolygonGlobalIDList = std::move(InputMesh.m_MeshData->PolygonGlobalIDList);

    m_MeshData->VertexPositionTable = std::move(InputMesh.m_MeshData->VertexPositionTable);
    m_MeshData->VertexIndexTable    = std::move(InputMesh.m_MeshData->VertexIndexTable);

    m_MeshData->VertexList = std::move(InputMesh.m_MeshData->VertexList);
    m_MeshData->VertexValidityFlagList = std::move(InputMesh.m_MeshData->VertexValidityFlagList);
    for (int_max k = 0; k < m_MeshData->VertexList.GetLength(); ++k)
    {
        m_MeshData->VertexList[k].SetParentMesh(*this);
    }

    m_MeshData->EdgeList = std::move(InputMesh.m_MeshData->EdgeList);
    m_MeshData->EdgeValidityFlagList = std::move(InputMesh.m_MeshData->EdgeValidityFlagList);
    for (int_max k = 0; k < m_MeshData->EdgeList.GetLength(); ++k)
    {
        m_MeshData->EdgeList[k].SetParentMesh(*this);
    }

    m_MeshData->HalfEdgeList = std::move(InputMesh.m_MeshData->HalfEdgeList);
    m_MeshData->HalfEdgeValidityFlagList = std::move(InputMesh.m_MeshData->HalfEdgeValidityFlagList);
    for (int_max k = 0; k < m_MeshData->HalfEdgeList.GetLength(); ++k)
    {
        m_MeshData->HalfEdgeList[k].SetParentMesh(*this);
    }

    m_MeshData->PolygonList = std::move(InputMesh.m_MeshData->PolygonList);
    m_MeshData->PolygonValidityFlagList = std::move(InputMesh.m_MeshData->PolygonValidityFlagList);
    for (int_max k = 0; k < m_MeshData->PolygonList.GetLength(); ++k)
    {
        m_MeshData->PolygonList[k].SetParentMesh(*this);
    }

    m_MeshData->MeshAttribute.NormalAtVertex  = std::move(InputMesh.m_MeshData->MeshAttribute.NormalAtVertex);
    m_MeshData->MeshAttribute.NormalAtPolygon = std::move(InputMesh.m_MeshData->MeshAttribute.NormalAtPolygon);

    return true;
}


template<typename ScalarType>
inline 
bool PolygonMesh<ScalarType>::Take(PolygonMesh<ScalarType>* InputMesh)
{
    if (InputMesh == nullptr)
    {
        MDK_Error("Input is nullptr @ PolygonMesh::Take(...)")
        return false;
    }

    return this->Take(*InputMesh);
}

//-------------------------------------------------------------------

template<typename ScalarType>
inline bool PolygonMesh<ScalarType>::IsEmpty() const
{
    return m_MeshData->VertexPositionTable.IsEmpty();
}

template<typename ScalarType>
inline 
int_max PolygonMesh<ScalarType>::GetVertexNumber() const
{
    return m_MeshData->VertexList.GetLength();
}

template<typename ScalarType>
inline
int_max PolygonMesh<ScalarType>::GetPolygonNumber() const
{
    return m_MeshData->PolygonList.GetLength();
}

template<typename ScalarType>
inline 
int_max PolygonMesh<ScalarType>::GetEdgeNumber() const
{
    return m_MeshData->EdgeList.GetLength();
}

//------------------------- Vertex and Polygon GlobalID ------------------------

template<typename ScalarType>
inline
DenseVector<int_max>& PolygonMesh<ScalarType>::VertexGlobalIDList()
{
    return m_MeshData->VertexGlobalIDList;
}

template<typename ScalarType>
inline
const DenseVector<int_max>& PolygonMesh<ScalarType>::VertexGlobalIDList() const
{
    return m_MeshData->VertexGlobalIDList;
}

template<typename ScalarType>
inline
DenseVector<int_max>& PolygonMesh<ScalarType>::PolygonGlobalIDList()
{
    return m_MeshData->PolygonGlobalIDList;
}

template<typename ScalarType>
inline
const DenseVector<int_max>& PolygonMesh<ScalarType>::PolygonGlobalIDList() const
{
    return m_MeshData->PolygonGlobalIDList;
}

//------------------------- Mesh 3D Position --------------------------------------

template<typename ScalarType>
inline
DenseMatrix<ScalarType>& PolygonMesh<ScalarType>::VertexPositionTable()
{
    return m_MeshData->VertexPositionTable;
}

template<typename ScalarType>
inline 
const DenseMatrix<ScalarType>& PolygonMesh<ScalarType>::VertexPositionTable() const
{
    return m_MeshData->VertexPositionTable;
}

//------------------------- Mesh Element ----------------------------------------------

template<typename ScalarType>
inline
DataArray<DenseVector<int_max>>& PolygonMesh<ScalarType>::VertexIndexTable()
{
    return m_MeshData->VertexIndexTable;
}

template<typename ScalarType>
inline 
const DataArray<DenseVector<int_max>>& PolygonMesh<ScalarType>::VertexIndexTable() const
{
    return m_MeshData->VertexIndexTable;
}

template<typename ScalarType>
inline
DataArray<Vertex_Of_PolygonMesh<ScalarType>>& PolygonMesh<ScalarType>::VertexList()
{
    return m_MeshData->VertexList;
}

template<typename ScalarType>
inline
const DataArray<Vertex_Of_PolygonMesh<ScalarType>>& PolygonMesh<ScalarType>::VertexList() const
{
    return m_MeshData->VertexList;
}

template<typename ScalarType>
inline
DenseVector<int_max>& PolygonMesh<ScalarType>::VertexValidityFlagList()
{
    return m_MeshData->VertexValidityFlagList;
}

template<typename ScalarType>
inline
const DenseVector<int_max>& PolygonMesh<ScalarType>::VertexValidityFlagList() const
{
    return m_MeshData->VertexValidityFlagList;
}

template<typename ScalarType>
inline
DataArray<Edge_Of_PolygonMesh<ScalarType>>& PolygonMesh<ScalarType>::EdgeList()
{
    return m_MeshData->EdgeList;
}

template<typename ScalarType>
inline
const DataArray<Edge_Of_PolygonMesh<ScalarType>>& PolygonMesh<ScalarType>::EdgeList() const
{
    return m_MeshData->EdgeList;
}

template<typename ScalarType>
inline
DenseVector<int_max>& PolygonMesh<ScalarType>::EdgeValidityFlagList()
{
    return m_MeshData->EdgeValidityFlagList;
}

template<typename ScalarType>
inline
const DenseVector<int_max>& PolygonMesh<ScalarType>::EdgeValidityFlagList() const
{
    return m_MeshData->EdgeValidityFlagList;
}

template<typename ScalarType>
inline
DataArray<HalfEdge_Of_PolygonMesh<ScalarType>>& PolygonMesh<ScalarType>::HalfEdgeList()
{
    return m_MeshData->HalfEdgeList;
}

template<typename ScalarType>
inline
const DataArray<HalfEdge_Of_PolygonMesh<ScalarType>>& PolygonMesh<ScalarType>::HalfEdgeList() const
{
    return m_MeshData->HalfEdgeList;
}

template<typename ScalarType>
inline
DenseVector<int_max>& PolygonMesh<ScalarType>::HalfEdgeValidityFlagList()
{
    return m_MeshData->HalfEdgeValidityFlagList;
}

template<typename ScalarType>
inline
const DenseVector<int_max>& PolygonMesh<ScalarType>::HalfEdgeValidityFlagList() const
{
    return m_MeshData->HalfEdgeValidityFlagList;
}

template<typename ScalarType>
inline
DataArray<Polygon_Of_PolygonMesh<ScalarType>>& PolygonMesh<ScalarType>::PolygonList()
{
    return m_MeshData->PolygonList;
}

template<typename ScalarType>
inline
const DataArray<Polygon_Of_PolygonMesh<ScalarType>>& PolygonMesh<ScalarType>::PolygonList() const
{
    return m_MeshData->PolygonList;
}

template<typename ScalarType>
inline 
DenseVector<int_max>& PolygonMesh<ScalarType>::PolygonValidityFlagList()
{
    return m_MeshData->PolygonValidityFlagList;
}

template<typename ScalarType>
inline
const DenseVector<int_max>& PolygonMesh<ScalarType>::PolygonValidityFlagList() const
{
    return m_MeshData->PolygonValidityFlagList;
}

//----------------------------- Mesh Attribute -----------------------------------------

template<typename ScalarType>
inline 
DenseMatrix<ScalarType>& PolygonMesh<ScalarType>::NormalAtVertex()
{
    return m_MeshData->MeshAttribute.NormalAtVertex;
}


template<typename ScalarType>
inline 
const DenseMatrix<ScalarType>& PolygonMesh<ScalarType>::NormalAtVertex() const
{
    return m_MeshData->MeshAttribute.NormalAtVertex;
}


template<typename ScalarType>
inline 
DenseMatrix<ScalarType>& PolygonMesh<ScalarType>::NormalAtPolygon()
{
    return m_MeshData->MeshAttribute.NormalAtPolygon;
}


template<typename ScalarType>
inline
const DenseMatrix<ScalarType>& PolygonMesh<ScalarType>::NormalAtPolygon() const
{
    return m_MeshData->MeshAttribute.NormalAtPolygon;
}

//----------------------------------------------------------------------------

template<typename ScalarType>
void PolygonMesh<ScalarType>::UpdateNormalAtVertex()
{
}


template<typename ScalarType>
void PolygonMesh<ScalarType>::UpdateNormalAtPolygon()
{
}

//----------------------------------------------------------------------------

template<typename ScalarType>
void PolygonMesh<ScalarType>::UpdateAttribute()
{
}




}// namespace mdk

#endif