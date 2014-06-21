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
{
    this->Copy(InputMesh);
}


template<typename MeshAttributeType>
TriangleMesh<MeshAttributeType>::TriangleMesh(TriangleMesh<MeshAttributeType>&& InputMesh)
{
    m_MeshData = std::move(InputMesh.m_MeshData);
}


template<typename MeshAttributeType>
TriangleMesh<MeshAttributeType>::~TriangleMesh()
{
}


template<typename MeshAttributeType>
inline
void TriangleMesh<MeshAttributeType>::operator=(const TriangleMesh<MeshAttributeType>& InputMesh)
{
    this->Copy(InputMesh);
}


template<typename MeshAttributeType>
inline
void TriangleMesh<MeshAttributeType>::operator=(TriangleMesh<MeshAttributeType>&& InputMesh)
{
    if (!m_MeshData)
    {
        m_MeshData = std::make_shared<PolygonMeshData<MeshAttributeType>>();
    }

    this->Take(std::forward<TriangleMesh<MeshAttributeType>&>(InputMesh));
}


template<typename MeshAttributeType>
inline 
void TriangleMesh<MeshAttributeType>::Clear()
{
    this->PolygonMesh::Clear();
}


template<typename MeshAttributeType>
inline 
void TriangleMesh<MeshAttributeType>::Copy(const TriangleMesh<MeshAttributeType>& InputMesh)
{
    this->PolygonMesh::Copy(InputMesh);
}


template<typename MeshAttributeType>
inline 
bool TriangleMesh<MeshAttributeType>::Copy(const TriangleMesh<MeshAttributeType>* InputMesh)
{
    if (InputMesh == nullptr)
    {
        MDK_Error("Input is nullptr @ TriangleMesh::Copy(...)")
        return false;
    }

    this->Copy(*InputMesh);

    return true;
}


template<typename MeshAttributeType>
inline 
void TriangleMesh<MeshAttributeType>::Share(TriangleMesh<MeshAttributeType>& InputMesh)
{
    m_MeshData = InputMesh.m_MeshData;
}


template<typename MeshAttributeType>
inline
bool TriangleMesh<MeshAttributeType>::Share(TriangleMesh<MeshAttributeType>* InputMesh)
{
    if (InputMesh == nullptr)
    {
        MDK_Error("Input is nullptr @ TriangleMesh::Share(...)")
        return false;
    }

    this->Share(*InputMesh);

    return true;
}


template<typename MeshAttributeType>
inline 
void TriangleMesh<MeshAttributeType>::ForceShare(const TriangleMesh<MeshAttributeType>& InputMesh)
{
    m_MeshData = InputMesh.m_MeshData;
}


template<typename MeshAttributeType>
inline 
bool TriangleMesh<MeshAttributeType>::ForceShare(const TriangleMesh<MeshAttributeType>* InputMesh)
{
    if (InputMesh == nullptr)
    {
        MDK_Error("Input is nullptr @ TriangleMesh::ForceShare(...)")
        return false;
    }

    this->ForceShare(*InputMesh);

    return true;
}


template<typename MeshAttributeType>
inline
void TriangleMesh<MeshAttributeType>::Take(TriangleMesh<MeshAttributeType>&& InputMesh)
{
    this->Take(std::forward<TriangleMesh<MeshAttributeType>&>(InputMesh));
}


template<typename MeshAttributeType>
inline 
bool TriangleMesh<MeshAttributeType>::Take(TriangleMesh<MeshAttributeType>& InputMesh)
{
    return this->PolygonMesh::Take(InputMesh);
}


template<typename MeshAttributeType>
inline 
bool TriangleMesh<MeshAttributeType>::Take(TriangleMesh<MeshAttributeType>* InputMesh)
{
    if (InputMesh == nullptr)
    {
        MDK_Error("Input is nullptr @ TriangleMesh::Take(...)")
        return false;
    }

    return this->Take(*InputMesh);
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