#ifndef __mdkTriangleMeshAttribute_hpp
#define __mdkTriangleMeshAttribute_hpp

namespace mdk
{

//============================================== VertexAttribute_Of_TriangleMesh ===========================================//

template<typename ScalarType>
VertexAttribute_Of_TriangleMesh<ScalarType>::VertexAttribute_Of_TriangleMesh()
{
    m_Data = std::make_unique<Data_Of_VertexAttribute_Of_TriangleMesh<ScalarType>>();
}

template<typename ScalarType>
VertexAttribute_Of_TriangleMesh<ScalarType>::VertexAttribute_Of_TriangleMesh(const VertexAttribute_Of_TriangleMesh& InputAttribute)
{
    m_Data = std::make_unique<Data_Of_VertexAttribute_Of_TriangleMesh<ScalarType>>();
    (*this) = InputAttribute;
}

template<typename ScalarType>
VertexAttribute_Of_TriangleMesh<ScalarType>::VertexAttribute_Of_TriangleMesh(VertexAttribute_Of_TriangleMesh&& InputAttribute)
{
    m_Data = std::move(InputAttribute.m_Data);
}

template<typename ScalarType>
VertexAttribute_Of_TriangleMesh<ScalarType>::~VertexAttribute_Of_TriangleMesh()
{
}

template<typename ScalarType>
void VertexAttribute_Of_TriangleMesh<ScalarType>::operator=(const VertexAttribute_Of_TriangleMesh<ScalarType>& InputAttribute)
{
    if (!m_Data)
    {
        m_Data = std::make_unique<Data_Of_VertexAttribute_Of_TriangleMesh<ScalarType>>();
    }

    m_Data->MeanCurvature = InputAttribute.m_Data->MeanCurvature;
    m_Data->Normal = InputAttribute.m_Data->Normal;
}

template<typename ScalarType>
void VertexAttribute_Of_TriangleMesh<ScalarType>::operator=(VertexAttribute_Of_TriangleMesh<ScalarType>&& InputAttribute)
{
    m_Data = std::move(InputAttribute.m_Data);
}

//============================================== EdgeAttribute_Of_TriangleMesh ===========================================//

template<typename ScalarType>
EdgeAttribute_Of_TriangleMesh<ScalarType>::EdgeAttribute_Of_TriangleMesh()
{
    m_Data = std::make_unique<Data_Of_EdgeAttribute_Of_TriangleMesh<ScalarType>>();
}

template<typename ScalarType>
EdgeAttribute_Of_TriangleMesh<ScalarType>::EdgeAttribute_Of_TriangleMesh(const EdgeAttribute_Of_TriangleMesh<ScalarType>& InputAttribute)
{
    m_Data = std::make_unique<Data_Of_EdgeAttribute_Of_TriangleMesh<ScalarType>>();
    (*this) = InputAttribute;
}

template<typename ScalarType>
EdgeAttribute_Of_TriangleMesh<ScalarType>::EdgeAttribute_Of_TriangleMesh(EdgeAttribute_Of_TriangleMesh<ScalarType>&& InputAttribute)
{
    m_Data = std::move(InputAttribute.m_Data);
}

template<typename ScalarType>
EdgeAttribute_Of_TriangleMesh<ScalarType>::~EdgeAttribute_Of_TriangleMesh()
{
}

template<typename ScalarType>
void EdgeAttribute_Of_TriangleMesh<ScalarType>::operator=(const EdgeAttribute_Of_TriangleMesh<ScalarType>& InputAttribute)
{
    if (!m_Data)
    {
        m_Data = std::make_unique<Data_Of_EdgeAttribute_Of_TriangleMesh<ScalarType>>();
    }

    m_Data->PhysicalLength = InputAttribute.m_Data->PhysicalLength;
}

template<typename ScalarType>
void EdgeAttribute_Of_TriangleMesh<ScalarType>::operator=(EdgeAttribute_Of_TriangleMesh<ScalarType>&& InputAttribute)
{
    m_Data = std::move(InputAttribute.m_Data);
}

//============================================== DirectedEdgeAttribute_Of_TriangleMesh ===========================================//

template<typename ScalarType>
DirectedEdgeAttribute_Of_TriangleMesh<ScalarType>::DirectedEdgeAttribute_Of_TriangleMesh()
{
    m_Data = std::make_unique<Data_Of_DirectedEdgeAttribute_Of_TriangleMesh<ScalarType>>();
}

template<typename ScalarType>
DirectedEdgeAttribute_Of_TriangleMesh<ScalarType>::DirectedEdgeAttribute_Of_TriangleMesh(const DirectedEdgeAttribute_Of_TriangleMesh<ScalarType>& InputAttribute)
{
    m_Data = std::make_unique<Data_Of_DirectedEdgeAttribute_Of_TriangleMesh<ScalarType>>();
    (*this) = InputAttribute;
}

template<typename ScalarType>
DirectedEdgeAttribute_Of_TriangleMesh<ScalarType>::DirectedEdgeAttribute_Of_TriangleMesh(DirectedEdgeAttribute_Of_TriangleMesh<ScalarType>&& InputAttribute)
{
    m_Data = std::move(InputAttribute.m_Data);
}

template<typename ScalarType>
DirectedEdgeAttribute_Of_TriangleMesh<ScalarType>::~DirectedEdgeAttribute_Of_TriangleMesh()
{
}

template<typename ScalarType>
void DirectedEdgeAttribute_Of_TriangleMesh<ScalarType>::operator=(const DirectedEdgeAttribute_Of_TriangleMesh<ScalarType>& InputAttribute)
{
    if (!m_Data)
    {
        m_Data = std::make_unique<Data_Of_DirectedEdgeAttribute_Of_TriangleMesh<ScalarType>>();
    }

    m_Data->Orientation = InputAttribute.m_Data->Orientation;
}

template<typename ScalarType>
void DirectedEdgeAttribute_Of_TriangleMesh<ScalarType>::operator=(DirectedEdgeAttribute_Of_TriangleMesh<ScalarType>&& InputAttribute)
{
    m_Data = std::move(InputAttribute.m_Data);
}

//============================================== CellAttribute_Of_TriangleMesh ===========================================//

template<typename ScalarType>
CellAttribute_Of_TriangleMesh<ScalarType>::CellAttribute_Of_TriangleMesh()
{
    m_Data = std::make_unique<Data_Of_CellAttribute_Of_TriangleMesh<ScalarType>>();
}

template<typename ScalarType>
CellAttribute_Of_TriangleMesh<ScalarType>::CellAttribute_Of_TriangleMesh(const CellAttribute_Of_TriangleMesh<ScalarType>& InputAttribute)
{
    m_Data = std::make_unique<Data_Of_CellAttribute_Of_TriangleMesh<ScalarType>>();
    (*this) = InputAttribute;
}

template<typename ScalarType>
CellAttribute_Of_TriangleMesh<ScalarType>::CellAttribute_Of_TriangleMesh(CellAttribute_Of_TriangleMesh<ScalarType>&& InputAttribute)
{
    m_Data = std::move(InputAttribute.m_Data);
}

template<typename ScalarType>
CellAttribute_Of_TriangleMesh<ScalarType>::~CellAttribute_Of_TriangleMesh()
{
}

template<typename ScalarType>
void CellAttribute_Of_TriangleMesh<ScalarType>::operator=(const CellAttribute_Of_TriangleMesh<ScalarType>& InputAttribute)
{
    if (!m_Data)
    {
        m_Data = std::make_unique<Data_Of_CellAttribute_Of_TriangleMesh<ScalarType>>();
    }

    m_Data->Area = InputAttribute.m_Data->Area;
}

template<typename ScalarType>
void CellAttribute_Of_TriangleMesh<ScalarType>::operator=(CellAttribute_Of_TriangleMesh<ScalarType>&& InputAttribute)
{
    m_Data = std::move(InputAttribute.m_Data);
}

}// namespace mdk

#endif