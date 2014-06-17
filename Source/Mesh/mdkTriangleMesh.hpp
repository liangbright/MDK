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
    return this->PolygonMesh::Take(InputMesh);
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
inline 
bool TriangleMesh<ScalarType>::IsEmpty() const
{
    return this->PolygonMesh::IsEmpty();
}


template<typename ScalarType>
inline 
int_max TriangleMesh<ScalarType>::GetVertexNumber() const
{
    return this->PolygonMesh::GetVertexNumber();
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
DenseVector<int_max>& TriangleMesh<ScalarType>::VertexGlobalIDList()
{
    return this->PolygonMesh::VertexGlobalIDList();
}


template<typename ScalarType>
inline
const DenseVector<int_max>& TriangleMesh<ScalarType>::VertexGlobalIDList() const
{
    return this->PolygonMesh::VertexGlobalIDList();
}


template<typename ScalarType>
inline
DenseVector<int_max>& TriangleMesh<ScalarType>::TriangleGlobalIDList()
{
    return this->PolygonMesh::PolygonGlobalIDList();
}


template<typename ScalarType>
inline
const DenseVector<int_max>& TriangleMesh<ScalarType>::TriangleGlobalIDList() const
{
    return this->PolygonMesh::PolygonGlobalIDList();
}

//------------------------- Mesh 3D Position --------------------------------------

template<typename ScalarType>
inline
DenseMatrix<ScalarType>& TriangleMesh<ScalarType>::VertexPositionTable()
{
    return this->PolygonMesh::VertexPositionTable();
}

template<typename ScalarType>
inline
const DenseMatrix<ScalarType>& TriangleMesh<ScalarType>::VertexPositionTable() const
{
    return this->PolygonMesh::VertexPositionTable();
}

//------------------------- Mesh Element ----------------------------------------------

template<typename ScalarType>
inline
DataArray<DenseVector<int_max>>& TriangleMesh<ScalarType>::VertexIndexTable()
{
    return this->PolygonMesh::VertexIndexTable();
}

template<typename ScalarType>
inline
const DataArray<DenseVector<int_max>>& TriangleMesh<ScalarType>::VertexIndexTable() const
{
    return this->PolygonMesh::VertexIndexTable();
}

template<typename ScalarType>
inline
DataArray<Vertex_Of_PolygonMesh<ScalarType>>& TriangleMesh<ScalarType>::VertexList()
{
    return this->PolygonMesh::VertexList();
}

template<typename ScalarType>
inline
const DataArray<Vertex_Of_PolygonMesh<ScalarType>>& TriangleMesh<ScalarType>::VertexList() const
{
    return this->PolygonMesh::VertexList();
}

template<typename ScalarType>
inline
DenseVector<int_max>& TriangleMesh<ScalarType>::VertexValidityFlagList()
{
    return this->PolygonMesh::VertexValidityFlagList();
}

template<typename ScalarType>
inline
const DenseVector<int_max>& TriangleMesh<ScalarType>::VertexValidityFlagList() const
{
    return this->PolygonMesh::VertexValidityFlagList();
}

template<typename ScalarType>
inline
DataArray<Edge_Of_PolygonMesh<ScalarType>>& TriangleMesh<ScalarType>::EdgeList()
{
    return this->PolygonMesh::EdgeList();
}

template<typename ScalarType>
inline
const DataArray<Edge_Of_PolygonMesh<ScalarType>>& TriangleMesh<ScalarType>::EdgeList() const
{
    return this->PolygonMesh::EdgeList();
}

template<typename ScalarType>
inline
DenseVector<int_max>& TriangleMesh<ScalarType>::EdgeValidityFlagList()
{
    return this->PolygonMesh::EdgeValidityFlagList();
}

template<typename ScalarType>
inline
const DenseVector<int_max>& TriangleMesh<ScalarType>::EdgeValidityFlagList() const
{
    return this->PolygonMesh::EdgeValidityFlagList();
}

template<typename ScalarType>
inline
DataArray<HalfEdge_Of_PolygonMesh<ScalarType>>& TriangleMesh<ScalarType>::HalfEdgeList()
{
    return this->PolygonMesh::HalfEdgeList();
}

template<typename ScalarType>
inline
const DataArray<HalfEdge_Of_PolygonMesh<ScalarType>>& TriangleMesh<ScalarType>::HalfEdgeList() const
{
    return this->PolygonMesh::HalfEdgeList();
}

template<typename ScalarType>
inline
DenseVector<int_max>& TriangleMesh<ScalarType>::HalfEdgeValidityFlagList()
{
    return this->PolygonMesh::HalfEdgeValidityFlagList();
}

template<typename ScalarType>
inline
const DenseVector<int_max>& TriangleMesh<ScalarType>::HalfEdgeValidityFlagList() const
{
    return this->PolygonMesh::HalfEdgeValidityFlagList();
}

template<typename ScalarType>
inline
DataArray<Polygon_Of_PolygonMesh<ScalarType>>& TriangleMesh<ScalarType>::TriangleList()
{
    return this->PolygonMesh::PolygonList();
}

template<typename ScalarType>
inline
const DataArray<Polygon_Of_PolygonMesh<ScalarType>>& TriangleMesh<ScalarType>::TriangleList() const
{
    return this->PolygonMesh::PolygonList();
}

template<typename ScalarType>
inline
DenseVector<int_max>& TriangleMesh<ScalarType>::PolygonValidityFlagList()
{
    return this->PolygonMesh::PolygonValidityFlagList();
}

template<typename ScalarType>
inline
const DenseVector<int_max>& TriangleMesh<ScalarType>::PolygonValidityFlagList() const
{
    return this->PolygonMesh::PolygonValidityFlagList();
}

//----------------------------- Mesh Attribute -----------------------------------------

template<typename ScalarType>
inline 
DenseMatrix<ScalarType>& TriangleMesh<ScalarType>::NormalAtVertex()
{
    return this->PolygonMesh::NormalAtVertex();
}


template<typename ScalarType>
inline 
const DenseMatrix<ScalarType>& TriangleMesh<ScalarType>::NormalAtVertex() const
{
    return this->PolygonMesh::NormalAtVertex();
}


template<typename ScalarType>
inline 
DenseMatrix<ScalarType>& TriangleMesh<ScalarType>::NormalAtTriangle()
{
    return this->PolygonMesh::NormalAtPolygon();
}


template<typename ScalarType>
inline
const DenseMatrix<ScalarType>& TriangleMesh<ScalarType>::NormalAtTriangle() const
{
    return this->PolygonMesh::NormalAtPolygon();
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