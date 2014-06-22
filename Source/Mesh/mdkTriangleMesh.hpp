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
: PolygonMesh(std::forward<TriangleMesh<MeshAttributeType>&&>(InputMesh))
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
    this->PolygonMesh::operator=(std::forward<TriangleMesh<MeshAttributeType>&&>(InputMesh));
}

template<typename MeshAttributeType>
inline 
typename MeshAttributeType::CellHandleType
TriangleMesh<MeshAttributeType>::AddCellByEdge(typename MeshAttributeType::EdgeHandleType EdgeHandle0, 
                                               typename MeshAttributeType::EdgeHandleType EdgeHandle1, 
                                               typename MeshAttributeType::EdgeHandleType EdgeHandle2)
{
    DenseVector<EdgeHandleType> EdgeHandleList = { EdgeHandle0, EdgeHandle1, EdgeHandle2 };
    return this->PolygonMesh::AddCellByEdge(EdgeHandleList);
}

template<typename MeshAttributeType>
inline
typename MeshAttributeType::CellHandleType
TriangleMesh<MeshAttributeType>::AddCellByEdge(int_max EdgeID0, int_max EdgeID1, int_max EdgeID2)
{
    DenseVector<int_max> EdgeIDList = { EdgeID0, EdgeID1, EdgeID2 };
    return this->PolygonMesh::AddCellByEdge(EdgeIDList);
}

template<typename MeshAttributeType>
inline
typename MeshAttributeType::CellHandleType
TriangleMesh<MeshAttributeType>::AddCellByPoint(typename MeshAttributeType::PointHandleType PointHandle0, 
                                                typename MeshAttributeType::PointHandleType PointHandle1, 
                                                typename MeshAttributeType::PointHandleType PointHandle2)
{
    DenseVector<PointHandleType> PointHandleList = { PointHandle0, PointHandle1, PointHandle2 };
    return this->PolygonMesh::AddCellByPoint(PointHandleList);
}

template<typename MeshAttributeType>
inline
typename MeshAttributeType::CellHandleType
TriangleMesh<MeshAttributeType>::AddCellByPoint(int_max PointID0, int_max PointID1, int_max PointID2)
{
    DenseVector<int_max> PointIDList = { PointID0, PointID1, PointID2 };
    return this->PolygonMesh::AddCellByPoint(PointIDList);
}

template<typename MeshAttributeType>
void TriangleMesh<MeshAttributeType>::Construct(PolygonMesh<MeshAttributeType> InputPolygonMesh)
{
    (*this) = std::move(InputPolygonMesh);
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