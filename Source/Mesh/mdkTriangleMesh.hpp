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

    auto it = this->GetIteratorOfCell();
    it.SetToBegin();
    while (it.IsNotEnd())
    {
        auto CellHandle = it.GetCellHandle();
        auto PointNumber = this->Cell(CellHandle).GetPointNumber();
        if (PointNumber != 3)
        {
            return false;
        }
        ++it;
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