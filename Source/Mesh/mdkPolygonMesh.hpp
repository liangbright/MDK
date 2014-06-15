#ifndef __mdkPolygonMesh_hpp
#define __mdkPolygonMesh_hpp


namespace mdk
{

template<typename ScalarType>
PolygonMesh<ScalarType>::PolygonMesh()
{
    m_MeshData = std::make_shared<PolygonMeshData<ScalarType>>();
}


template<typename ScalarType>
PolygonMesh<ScalarType>::PolygonMesh(const PolygonMesh<ScalarType>& InputMesh)
{
    Copy(InputMesh);
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
    Copy(InputMesh);
}


template<typename ScalarType>
inline
void PolygonMesh<ScalarType>::operator=(PolygonMesh<ScalarType>&& InputMesh)
{
    if (!m_MeshData)
    {
        m_MeshData = std::make_shared<PolygonMeshData<ScalarType>>();
    }

    Take(std::forward<PolygonMesh<ScalarType>&>(InputMesh));
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
        MDK_Error("InputVertex or InputVertexIndexTable is invalid @ PolygonMesh::Construct(...)")
        return false;
    }

    //---------------------------------------------------------------------------------------------------------------//

    m_MeshData->VertexPositionTable = std::move(InputVertexPositionTable);
    m_MeshData->VertexIndexTable = std::move(InputVertexIndexTable);

    const DataArray<DenseVector<int_max>>& TempPolygonList = m_MeshData->VertexIndexTable;
    auto PolygonNumber = TempPolygonList.GetElementNumber();

    DataArray<DenseVector<int_max>> TempHalfEdgeList, HalfEdgeIndexListOfEachPolygon;
    this->ConstructTempHalfEdgeList(TempHalfEdgeList, HalfEdgeIndexListOfEachPolygon);
    if (TempHalfEdgeList.IsEmpty() == true)
    {
        return false;
    }

    if (this->ConstructPolygonList(TempHalfEdgeList, HalfEdgeIndexListOfEachPolygon) == false)
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

    return true;
}


template<typename ScalarType>
void PolygonMesh<ScalarType>::
ConstructTempHalfEdgeList(DataArray<DenseVector<int_max>>& TempHalfEdgeList, DataArray<DenseVector<int_max>>& HalfEdgeIndexListOfEachPolygon)
{
    //---------------------------------------------------------------------------------------//
    // Output: TempHalfEdgeList
    // TempHalfEdgeList[k] is a half edge, and k is HalfEdgeIndex
    // TempHalfEdgeList[k][0] : PolygonIndex
    // TempHalfEdgeList[k][1] : VertexIndex_0
    // TempHalfEdgeList[k][2] : VertexIndex_1
    // VertexIndex_0 < VertexIndex_1 always
    //----------------------------------------------------------------------------------------//
    // Output: HalfEdgeIndexListOfEachPolygon
    // HalfEdgeIndexListOfEachPolygon[k] is HalfEdgeIndexList of Polygon, and k is PolygonIndex
    //------------------------------------------------------------------------------------------//

    const DataArray<DenseVector<int_max>>& TempPolygonList = m_MeshData->VertexIndexTable;

    auto PolygonNumber = TempPolygonList.GetElementNumber();

    TempHalfEdgeList.FastResize(0);
    TempHalfEdgeList.ReserveCapacity(PolygonNumber * 3);

    DataArray<DenseVector<int_max>> HalfEdgeIndexListOfEachPolygon;
    HalfEdgeIndexListOfEachPolygon.FastResize(PolygonNumber);

    int_max HalfEdgeIndex = 0;

    for (int_max k = 0; k < PolygonNumber; ++k)
    {
        auto VertexNumber_k = TempPolygonList[k].GetElementNumber();

        DenseVector<int_max>& HalfEdgeIndexList_k = HalfEdgeIndexListOfEachPolygon[k];
        HalfEdgeIndexList_k.FastResize(VertexNumber_k);

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

            HalfEdgeIndexList_k[n] = HalfEdgeIndex;

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

        HalfEdgeIndexList_k[VertexNumber_k - 1] = HalfEdgeIndex;

        HalfEdgeIndex += 1;
    }

    auto tempRelativeIndexList = TempHalfEdgeList.Sort([](const DenseVector<int_max>& A, const DenseVector<int_max>& B)
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

    TempHalfEdgeList = TempHalfEdgeList.GetSubSet(tempRelativeIndexList);

    HalfEdgeIndexListOfEachPolygon = HalfEdgeIndexListOfEachPolygon.GetSubSet(tempRelativeIndexList);
}


template<typename ScalarType>
bool PolygonMesh<ScalarType>::
ConstructPolygonList(const DataArray<DenseVector<int_max>>& TempHalfEdgeList, const DataArray<DenseVector<int_max>>& HalfEdgeIndexListOfEachPolygon)
{
    const DataArray<DenseVector<int_max>>& TempPolygonList = m_MeshData->VertexIndexTable;

    auto PolygonNumber = TempPolygonList.GetLength();

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
            if (k < VertexIndexListOfPolygon.GetLength() - 1)
            {
                VertexIndex_1 = VertexIndexList[k + 1];
            }
            else
            {
                VertexIndex_1 = VertexIndexList[0];
            }

            FlagList.FastResize(HalfEdgeIndexList.GetLength())
            FlagList.Fill(1);

            int_max HalfEdgeIndex_k = -1;

            for (int_max m = 0; m < HalfEdgeIndexList.GetLength(); ++m)
            {
                if (FlagList[m] > 0)
                {
                    if ((TempHalfEdgeList[HalfEdgeIndexList[k]][1] == VertexIndex_0 && TempHalfEdgeList[HalfEdgeIndexList[k]][2] == VertexIndex_1)
                        || (TempHalfEdgeList[HalfEdgeIndexList[k]][1] == VertexIndex_1 && TempHalfEdgeList[HalfEdgeIndexList[k]][2] == VertexIndex_0))
                    {
                        FlagList[m] = 0;
                        HalfEdgeIndex_k = m;
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

        m_MeshData->PolygonList[PolygonIndex].SetParentMesh(this);
        m_MeshData->PolygonList[PolygonIndex].SetIndex(PolygonIndex);
        m_MeshData->PolygonList[PolygonIndex].SetHalfEdgeIndexList(std::move(HalfEdgeIndexList_reorder));
    }

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

    auto HalfEdgeNumber = TempHalfEdgeList.GetElementNumber();

    DenseMatrix<int_max> UniqueEdgeFlagList(1, HalfEdgeNumber);
    UniqueEdgeFlagList.Fill(0);

    UniqueEdgeFlagList[0] = 1;

    for (int_max k = 1; k < HalfEdgeNumber; ++k)
    {
        if (TempFunction_SameEdge(TempHalfEdgeList[k - 1], TempHalfEdgeList[k]) == false)
        {
            UniqueEdgeFlagList[k] = 1;
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

    int_max UniqueEdgeNumber = UniqueEdgeFlagList.Sum();

    m_MeshData->EdgeList.FastResize(4, UniqueEdgeNumber);

    DenseVector<int_max> Map_HalfEdgeIndexToEdgeIndex;
    Map_HalfEdgeIndexToEdgeIndex.FastResize(HalfEdgeNumber)

    int_max EdgeIndex_current = 0;

    for (int_max HalfEdgeIndex = 0; HalfEdgeIndex < HalfEdgeNumber; ++HalfEdgeIndex)
    {
        if (UniqueEdgeFlagList[HalfEdgeIndex] == 1)
        {
            int_max HalfEdgeIndex_opposite = -1;

            auto PolygonIndex_0 = TempHalfEdgeList[HalfEdgeIndex][0];

            int_max PolygonIndex_1 = -1;

            if (HalfEdgeIndex < HalfEdgeNumber - 1)
            {
                if (TempFunction_SameEdge(TempHalfEdgeList[HalfEdgeIndex], TempHalfEdgeList[HalfEdgeIndex + 1]) == true)
                {
                    PolygonIndex_1 = TempHalfEdgeList[HalfEdgeIndex + 1][0];

                    HalfEdgeIndex_opposite = HalfEdgeIndex + 1;
                }
            }

            if (PolygonIndex_0 > PolygonIndex_1)
            {
                auto temp_0 = PolygonIndex_0;
                PolygonIndex_0 = PolygonIndex_1;
                PolygonIndex_1 = temp_0;
            }

            m_MeshData->EdgeList[EdgeIndex_current].SetHalfEdgeIndexList(HalfEdgeIndex, HalfEdgeIndex_opposite);
            Map_HalfEdgeIndexToEdgeIndex[HalfEdgeIndex] = EdgeIndex_current;
            if (HalfEdgeIndex_opposite >= 0)
            {
                Map_HalfEdgeIndexToEdgeIndex[HalfEdgeIndex_opposite] = EdgeIndex_current;
            }

            EdgeIndex_current += 1;
        }
    }

    return true;
}


template<typename ScalarType>
bool PolygonMesh<ScalarType>::ConstructHalfEdgeList(const DataArray<DenseVector<int_max>>& TempHalfEdgeList)
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

    auto HalfEdgeNumber = TempHalfEdgeList.GetElementNumber();

    m_MeshData->HalfEdgeList.FastResize(HalfEdgeNumber);
    for (int_max HalfEdgeIndex = 0; HalfEdgeIndex < HalfEdgeNumber; ++HalfEdgeIndex)
    {
        auto DenseVector<int_max>& tempHalfEdge = TempHalfEdgeList[HalfEdgeIndex];

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

        auto tempPolygonIndexOfHalfEdge = tempHalfEdge[0];

        const DenseVector<int_max>& VertexIndexListOfPolygon = TempPolygonList[tempPolygonIndexOfHalfEdge];

        const DenseVector<int_max>& HalfEdgeIndexListOfPolygon = m_MeshData->PolygonList[tempPolygonIndexOfHalfEdge].GetHalfEdgeIndexList();

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

        m_MeshData->HalfEdgeList[k].SetParentMesh(this);
        m_MeshData->HalfEdgeList[k].SetIndex(HalfEdgeIndex);
        m_MeshData->HalfEdgeList[k].SetStartVertexIndex(VertexIndex_start_of_halfedge);
        m_MeshData->HalfEdgeList[k].SetEndVertexIndex(VertexIndex_end_of_halfedge);
        m_MeshData->HalfEdgeList[k].SetEdgeIndex(Map_HalfEdgeIndexToEdgeIndex[HalfEdgeIndex]);
        m_MeshData->HalfEdgeList[k].SetPolygonIndex(tempHalfEdge[0]);
        m_MeshData->HalfEdgeList[k].SetOppositeHalfEdgeIndex(HalfEdgeIndex_opposite);
        m_MeshData->HalfEdgeList[k].SetNextHalfEdgeIndex(HalfEdgeIndex_next);
        m_MeshData->HalfEdgeList[k].SetPreviousHalfEdgeIndex(HalfEdgeIndex_prev);
    }

    return true;
}


template<typename ScalarType>
bool PolygonMesh<ScalarType>::ConstructVertexList()
{
    auto VertexNumber = m_MeshData->VertexPositionTable.GetColNumber();

    int_max EdgeNumber = m_MeshData->EdgeList.GetLength();

    int_max HalfEdgeNumber = m_MeshData->HalfEdgeList.GetLength();

    int_max PolygonNumber = m_MeshData->PolygonList.GetLength();

    m_MeshData->VertexList.FastResize(VertexNumber);

    for (int_max VertexIndex = 0; VertexIndex < VertexNumber; ++VertexIndex)
    {
        m_MeshData->VertexList[k].SetParentMesh(this);
        m_MeshData->VertexList[k].SetIndex(VertexIndex);
    }
   
    DataArray<DenseVector<int_max>> Adjacency_VertexToHalfEdge;
    Adjacency_VertexToHalfEdge.FastResize(VertexNumber);
    for (int_max k = 0; k < VertexNumber; ++k)
    {
        Adjacency_VertexToHalfEdge[k].ReserveCapacity(10);
    }

    for (int_max k = 0; k < HalfEdgeNumber; ++k)
    {
        auto VertexIndex_k_start = m_MeshData->HalfEdgeList[k].GetStartVertexIndex();
        Adjacency_VertexToHalfEdge[VertexIndex_k_start].Append(k);
    }

    for (int_max k = 0; k < VertexNumber; ++k)
    {
        const DenseVector<int_max>& AdjacentHalfEdgeIndexList = Adjacency_VertexToHalfEdge[k];
        AdjacentHalfEdgeIndexList.Squeeze();

        DenseVector<int_max> AdjacenctVertexIndexList;
        AdjacenctVertexIndexList.FastResize(AdjacentHalfEdgeIndexList.GetLength());

        DenseVector<int_max> AdjacenctEdgeIndexList;
        AdjacenctEdgeIndexList.FastResize(AdjacentHalfEdgeIndexList.GetLength());

        DenseVector<int_max> AdjacenctPolygonIndexList;
        AdjacenctPolygonIndexList.FastResize(AdjacentHalfEdgeIndexList.GetLength());

        for (int_max m = 0; m < AdjacentHalfEdgeIndexList.GetLength(); ++m)
        {
            AdjacenctVertexIndexList[m] = m_MeshData->HalfEdgeList[AdjacentHalfEdgeIndexList[m]].GetEndVertexIndex();
            AdjacenctEdgeIndexList[m] = m_MeshData->HalfEdgeList[AdjacentHalfEdgeIndexList[m]].GetEdgeIndex();
            AdjacenctPolygonIndexList[m] = m_MeshData->HalfEdgeList[AdjacentHalfEdgeIndexList[m]].GetPolygonIndex();
        }

        AdjacenctPolygonIndexList = AdjacenctPolygonIndexList.GetSubSet(AdjacenctPolygonIndexList.FindUnique());

        m_MeshData->VertexList[k].SetAdjacentHalfEdgeIndexList(std::move(AdjacentHalfEdgeIndexList));
        m_MeshData->VertexList[k].SetAdjacentVertexIndexList(std::move(AdjacenctVertexIndexList));
        m_MeshData->VertexList[k].SetAdjacentEdgeIndexList(std::move(AdjacenctEdgeIndexList));
        m_MeshData->VertexList[k].SetAdjacentPolygonIndexList(std::move(AdjacenctPolygonIndexList));
    }
}


template<typename ScalarType>
inline 
void PolygonMesh<ScalarType>::Clear()
{
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

    m_MeshData->VertexGlobalIndexList = InputMesh.m_MeshData->VertexGlobalIndexList;
    m_MeshData->PolygonGlobalIndexList = InputMesh.m_MeshData->PolygonGlobalIndexList;

    m_MeshData->VertexPositionTable = InputMesh.m_MeshData->VertexPositionTable;

    m_MeshData->VertexList = InputMesh.m_MeshData->VertexList;
    m_MeshData->PolygonList = InputMesh.m_MeshData->PolygonList;
    m_MeshData->EdgeList = InputMesh.m_MeshData->EdgeList;
    m_MeshData->HalfEdgeList = InputMesh.m_MeshData->HalfEdgeList;

    m_MeshData->MeshAttribute.NormalAtVertex = InputMesh.m_MeshData->MeshAttribute.NormalAtVertex;
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

    Copy(*InputMesh);

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

    Share(*InputMesh);

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

    ForceShare(*InputMesh);

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

    m_MeshData->VertexGlobalIndexList = std::move(InputMesh.m_MeshData->VertexGlobalIndexList);
    m_MeshData->PolygonGlobalIndexList = std::move(InputMesh.m_MeshData->PolygonGlobalIndexList);

    m_MeshData->VertexPositionTable = std::move(InputMesh.m_MeshData->VertexPositionTable);

    m_MeshData->VertexList = std::move(InputMesh.m_MeshData->VertexList);
    m_MeshData->PolygonList = std::move(InputMesh.m_MeshData->PolygonList);
    m_MeshData->EdgeList = std::move(InputMesh.m_MeshData->EdgeList);
    m_MeshData->HalfEdgeList = std::move(InputMesh.m_MeshData->HalfEdgeList);

    m_MeshData->MeshAttribute.NormalAtVertex = std::move(InputMesh.m_MeshData->MeshAttribute.NormalAtVertex);
    m_MeshData->MeshAttribute.NormalAtPolygon = std::move(InputMesh.m_MeshData->MeshAttribute.NormalAtPolygon);
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

    return Take(*InputMesh);
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

//-------------------------------------------------------------------

template<typename ScalarType>
inline
const DenseVector<int_max>& PolygonMesh<ScalarType>::GetVertexGlobalIDList() const
{
    return m_MeshData->VertexGlobalIDList;
}


template<typename ScalarType>
inline
const DenseVector<int_max>& PolygonMesh<ScalarType>::GetPolygonGlobalIDList() const
{
    return m_MeshData->PolygonGlobalIDList;
}


template<typename ScalarType>
inline 
const DenseMatrix<ScalarType>& PolygonMesh<ScalarType>::GetVertexPositionTable() const
{
    return m_MeshData->VertexPositionTable;
}


template<typename ScalarType>
inline 
const DataArray<DenseVector<int_max>>& PolygonMesh<ScalarType>::GetVertexIndexTable() const
{
    return m_MeshData->VertexIndexTable;
}

//----------------------------------------------------------------------------

template<typename ScalarType>
inline 
DenseMatrix<ScalarType>& PolygonMesh<ScalarType>::NormalAtVertex()
{
    return m_MeshData->NormalAtVertex;
}


template<typename ScalarType>
inline 
const DenseMatrix<ScalarType>& PolygonMesh<ScalarType>::NormalAtVertex() const
{
    return m_MeshData->NormalAtVertex;
}


template<typename ScalarType>
inline 
DenseMatrix<ScalarType>& PolygonMesh<ScalarType>::NormalAtPolygon()
{
    return m_MeshData->NormalAtPolygon;
}


template<typename ScalarType>
inline
const DenseMatrix<ScalarType>& PolygonMesh<ScalarType>::NormalAtPolygon() const
{
    return m_MeshData->NormalAtPolygon;
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