#ifndef __mdkTriangleMesh_hpp
#define __mdkTriangleMesh_hpp


namespace mdk
{

template<typename ScalarType>
TriangleMesh<ScalarType>::TriangleMesh()
: PolygonMesh()
{
}


template<typename ScalarType>
TriangleMesh<ScalarType>::TriangleMesh(const TriangleMesh<ScalarType>& InputMesh)
{
    this->Copy(InputMesh);
}


template<typename ScalarType>
TriangleMesh<ScalarType>::TriangleMesh(TriangleMesh<ScalarType>&& InputMesh)
{
    m_MeshData = std::move(InputMesh.m_MeshData);
}


template<typename ScalarType>
TriangleMesh<ScalarType>::~TriangleMesh()
{

}


template<typename ScalarType>
inline
void TriangleMesh<ScalarType>::operator=(const TriangleMesh<ScalarType>& InputMesh)
{
    this->Copy(InputMesh);
}


template<typename ScalarType>
inline
void TriangleMesh<ScalarType>::operator=(TriangleMesh<ScalarType>&& InputMesh)
{
    if (!m_MeshData)
    {
        m_MeshData = std::make_shared<PolygonMeshData<ScalarType>>();
    }

    this->Take(std::forward<TriangleMesh<ScalarType>&>(InputMesh));
}


template<typename ScalarType>
bool TriangleMesh<ScalarType>::Construct(DenseMatrix<ScalarType> InputVertexPositionTable, DataArray<DenseVector<int_max>> InputVertexIndexTable)
{
    return this->PolygonMesh::Construct(std::move(InputVertexPositionTable), std::move(InputVertexIndexTable));

    
}



template<typename ScalarType>
inline 
void TriangleMesh<ScalarType>::Clear()
{
    this->PolygonMesh::Clear();
}


template<typename ScalarType>
template<typename ScalarType_Input>
inline 
void TriangleMesh<ScalarType>::Copy(const TriangleMesh<ScalarType_Input>& InputMesh)
{
    this->PolygonMesh::Copy(InputMesh);
}


template<typename ScalarType>
template<typename ScalarType_Input>
inline 
bool TriangleMesh<ScalarType>::Copy(const TriangleMesh<ScalarType_Input>* InputMesh)
{
    if (InputMesh == nullptr)
    {
        MDK_Error("Input is nullptr @ TriangleMesh::Copy(...)")
        return false;
    }

    this->Copy(*InputMesh);

    return true;
}


template<typename ScalarType>
inline 
void TriangleMesh<ScalarType>::Share(TriangleMesh& InputMesh)
{
    m_MeshData = InputMesh.m_MeshData;
}


template<typename ScalarType>
inline
bool TriangleMesh<ScalarType>::Share(TriangleMesh* InputMesh)
{
    if (InputMesh == nullptr)
    {
        MDK_Error("Input is nullptr @ TriangleMesh::Share(...)")
        return false;
    }

    this->Share(*InputMesh);

    return true;
}


template<typename ScalarType>
inline 
void TriangleMesh<ScalarType>::ForceShare(const TriangleMesh<ScalarType>& InputMesh)
{
    m_MeshData = InputMesh.m_MeshData;
}


template<typename ScalarType>
inline 
bool TriangleMesh<ScalarType>::ForceShare(const TriangleMesh<ScalarType>* InputMesh)
{
    if (InputMesh == nullptr)
    {
        MDK_Error("Input is nullptr @ TriangleMesh::ForceShare(...)")
        return false;
    }

    this->ForceShare(*InputMesh);

    return true;
}


template<typename ScalarType>
inline
void TriangleMesh<ScalarType>::Take(TriangleMesh<ScalarType>&& InputMesh)
{
    this->Take(std::forward<TriangleMesh<ScalarType>&>(InputMesh));
}


template<typename ScalarType>
inline 
bool TriangleMesh<ScalarType>::Take(TriangleMesh<ScalarType>& InputMesh)
{
    this->PolygonMesh::Take(InputMesh);
}


template<typename ScalarType>
inline 
bool TriangleMesh<ScalarType>::Take(TriangleMesh<ScalarType>* InputMesh)
{
    if (InputMesh == nullptr)
    {
        MDK_Error("Input is nullptr @ TriangleMesh::Take(...)")
        return false;
    }

    return this->Take(*InputMesh);
}

//-------------------------------------------------------------------

template<typename ScalarType>
inline bool TriangleMesh<ScalarType>::IsEmpty() const
{
    return this->PolygonMesh::IsEmpty();
}


template<typename ScalarType>
inline 
int_max TriangleMesh<ScalarType>::GetVertexNumber() const
{
    return this->PolygonMesh::GetColNumber();
}


template<typename ScalarType>
inline
int_max TriangleMesh<ScalarType>::GetTriangleNumber() const
{
    return this->PolygonMesh::GetPolygonNumber();
}


template<typename ScalarType>
inline 
int_max TriangleMesh<ScalarType>::GetEdgeNumber() const
{
    return this->PolygonMesh::GetEdgeNumber();
}

//-------------------------------------------------------------------

template<typename ScalarType>
inline
DenseMatrix<int_max>& TriangleMesh<ScalarType>::VertexGlobalIndexList()
{
    return m_MeshData->VertexGlobalIndexList;
}


template<typename ScalarType>
inline
const DenseMatrix<int_max>& TriangleMesh<ScalarType>::VertexGlobalIndexList() const
{
    return m_MeshData->VertexGlobalIndexList;
}


template<typename ScalarType>
inline
DenseMatrix<int_max>& TriangleMesh<ScalarType>::TriangleGlobalIndexList()
{
    return m_MeshData->PolygonGlobalIndexList;
}


template<typename ScalarType>
inline
const DenseMatrix<int_max>& TriangleMesh<ScalarType>::TriangleGlobalIndexList() const
{
    return m_MeshData->PolygonGlobalIndexList;
}

//----------------------------------------------------------------------------

template<typename ScalarType>
inline 
DenseMatrix<ScalarType>& TriangleMesh<ScalarType>::NormalAtVertex()
{
    return m_MeshData->NormalAtVertex;
}


template<typename ScalarType>
inline 
const DenseMatrix<ScalarType>& TriangleMesh<ScalarType>::NormalAtVertex() const
{
    return m_MeshData->NormalAtVertex;
}


template<typename ScalarType>
inline 
DenseMatrix<ScalarType>& TriangleMesh<ScalarType>::NormalAtTriangle()
{
    return m_MeshData->NormalAtPolygon;
}


template<typename ScalarType>
inline
const DenseMatrix<ScalarType>& TriangleMesh<ScalarType>::NormalAtTriangle() const
{
    return m_MeshData->NormalAtPolygon;
}

//----------------------------------------------------------------------------

template<typename ScalarType>
void TriangleMesh<ScalarType>::UpdateNormalAtVertex()
{
}


template<typename ScalarType>
void TriangleMesh<ScalarType>::UpdateNormalAtTriangle()
{
    this->PolygonMesh::UpdateNormalAtPolygon();
}

//----------------------------------------------------------------------------

template<typename ScalarType>
void TriangleMesh<ScalarType>::UpdateAttribute()
{
}




}// namespace mdk

#endif