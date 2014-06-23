#ifndef __mdkTriangleMesh_hpp
#define __mdkTriangleMesh_hpp


namespace mdk
{

template<typename MeshAttributeType>
TriangleMesh<MeshAttributeType>::TriangleMesh()
: PolygonMesh()
{
}


template<typename MeshAttributeType>
TriangleMesh<MeshAttributeType>::TriangleMesh(const TriangleMesh<MeshAttributeType>& InputMesh)
: PolygonMesh(InputMesh)
{
}


template<typename MeshAttributeType>
TriangleMesh<MeshAttributeType>::TriangleMesh(TriangleMesh<MeshAttributeType>&& InputMesh)
: PolygonMesh(std::forward<PolygonMesh<MeshAttributeType>&&>(InputMesh))
{
}


template<typename MeshAttributeType>
TriangleMesh<MeshAttributeType>::~TriangleMesh()
{
}


template<typename MeshAttributeType>
inline
void TriangleMesh<MeshAttributeType>::operator=(const TriangleMesh<MeshAttributeType>& InputMesh)
{
    this->PolygonMesh::operator=(InputMesh);
}


template<typename MeshAttributeType>
inline
void TriangleMesh<MeshAttributeType>::operator=(TriangleMesh<MeshAttributeType>&& InputMesh)
{
    this->PolygonMesh::operator=(std::forward<PolygonMesh<MeshAttributeType>&&>(InputMesh));
}

template<typename MeshAttributeType>
inline 
Handle_Of_Cell_Of_SurfaceMesh
TriangleMesh<MeshAttributeType>::AddCellByEdge(Handle_Of_Edge_Of_SurfaceMesh EdgeHandle0,
                                               Handle_Of_Edge_Of_SurfaceMesh EdgeHandle1, 
                                               Handle_Of_Edge_Of_SurfaceMesh EdgeHandle2)
{
    DenseVector<EdgeHandleType> EdgeHandleList = { EdgeHandle0, EdgeHandle1, EdgeHandle2 };
    return this->PolygonMesh::AddCellByEdge(EdgeHandleList);
}

template<typename MeshAttributeType>
inline
Handle_Of_Cell_Of_SurfaceMesh
TriangleMesh<MeshAttributeType>::AddCellByEdge(int_max EdgeID0, int_max EdgeID1, int_max EdgeID2)
{
    DenseVector<int_max> EdgeIDList = { EdgeID0, EdgeID1, EdgeID2 };
    return this->PolygonMesh::AddCellByEdge(EdgeIDList);
}

template<typename MeshAttributeType>
inline
Handle_Of_Cell_Of_SurfaceMesh
TriangleMesh<MeshAttributeType>::AddCellByPoint(Handle_Of_Point_Of_SurfaceMesh PointHandle0,
                                                Handle_Of_Point_Of_SurfaceMesh PointHandle1, 
                                                Handle_Of_Point_Of_SurfaceMesh PointHandle2)
{
    DenseVector<PointHandleType> PointHandleList = { PointHandle0, PointHandle1, PointHandle2 };
    return this->PolygonMesh::AddCellByPoint(PointHandleList);
}

template<typename MeshAttributeType>
inline
Handle_Of_Cell_Of_SurfaceMesh
TriangleMesh<MeshAttributeType>::AddCellByPoint(int_max PointID0, int_max PointID1, int_max PointID2)
{
    DenseVector<int_max> PointIDList = { PointID0, PointID1, PointID2 };
    return this->PolygonMesh::AddCellByPoint(PointIDList);
}

template<typename MeshAttributeType>
void TriangleMesh<MeshAttributeType>::Construct(PolygonMesh<MeshAttributeType> InputPolygonMesh)
{
    auto InputMeshPtr = static_cast<TriangleMesh<MeshAttributeType>*>(&InputPolygonMesh);
    m_MeshData = std::move(InputMeshPtr->m_MeshData);
}


template<typename MeshAttributeType>
bool TriangleMesh<MeshAttributeType>::CheckIfTriangleMesh() const
{
    if (this->IsEmpty() == true)
    {
        return false;
    }

    for (auto it = this->GetIteratorOfCell(); it.IsNotEnd(); ++it)
    {
        auto CellHandle = it.GetCellHandle();
        auto PointNumber = this->Cell(CellHandle).GetPointNumber();
        if (PointNumber != 3)
        {
            return false;
        }
    }

    return true;
}

// get a sub mesh by CellHandleList or CellIDList----------------------------//

template<typename MeshAttributeType>
TriangleMesh<MeshAttributeType> TriangleMesh<MeshAttributeType>::GetSubMeshByCell(const DenseVector<CellHandleType>& CellHandleList) const
{
    TriangleMesh<MeshAttributeType> OutputMesh;
    OutputMesh.Construct(this->PolygonMesh<MeshAttributeType>::GetSubMeshByCell(CellHandleList));
    return OutputMesh;
}

template<typename MeshAttributeType>
TriangleMesh<MeshAttributeType> TriangleMesh<MeshAttributeType>::GetSubMeshByCell(const DenseVector<int_max>& CellIDList) const
{
    TriangleMesh<MeshAttributeType> OutputMesh;
    OutputMesh.Construct(this->PolygonMesh<MeshAttributeType>::GetSubMeshByCell(CellIDList));
    return OutputMesh;
}

//------------- Function optimized TriangleMesh --------------------------------------------------//

template<typename MeshAttributeType>
void TriangleMesh<MeshAttributeType>::UpdateNormalAtPoint()
{
}


template<typename MeshAttributeType>
void TriangleMesh<MeshAttributeType>::UpdateNormalAtCell()
{
}

}// namespace mdk

#endif