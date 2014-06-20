#ifndef __mdkPolygonMeshAttribute_hpp
#define __smdkPolygonMeshAttribute_hpp

namespace mdk
{

//============================================== PointAttributeType_Of_PolygonMesh ===========================================//

template<typename ScalarType>
PointAttributeType_Of_PolygonMesh<ScalarType>::PointAttributeType_Of_PolygonMesh()
{
    m_Data = std::make_unique<Data_Of_PointAttributeType_Of_PolygonMesh<ScalarType>>();
}

template<typename ScalarType>
PointAttributeType_Of_PolygonMesh<ScalarType>::PointAttributeType_Of_PolygonMesh(const PointAttributeType_Of_PolygonMesh& InputAttribute)
{
    m_Data = std::make_unique<Data_Of_PointAttributeType_Of_PolygonMesh<ScalarType>>();
    (*this) = InputAttribute;
}

template<typename ScalarType>
PointAttributeType_Of_PolygonMesh<ScalarType>::PointAttributeType_Of_PolygonMesh(PointAttributeType_Of_PolygonMesh&& InputAttribute)
{
    m_Data = std::move(InputAttribute.m_Data);
}

template<typename ScalarType>
PointAttributeType_Of_PolygonMesh<ScalarType>::~PointAttributeType_Of_PolygonMesh()
{
}

template<typename ScalarType>
void PointAttributeType_Of_PolygonMesh<ScalarType>::operator=(const PointAttributeType_Of_PolygonMesh<ScalarType>& InputAttribute)
{
    if (!m_Data)
    {
        m_Data = std::make_unique<Data_Of_PointAttributeType_Of_PolygonMesh<ScalarType>>();
    }

    m_Data->MeanCurvature = InputAttribute.m_Data->MeanCurvature;
    m_Data->Normal = InputAttribute.m_Data->Normal;
}

template<typename ScalarType>
void PointAttributeType_Of_PolygonMesh<ScalarType>::operator=(PointAttributeType_Of_PolygonMesh<ScalarType>&& InputAttribute)
{
    m_Data = std::move(InputAttribute.m_Data);
}

//============================================== EdgeAttribute_Of_PolygonMesh ===========================================//

template<typename ScalarType>
EdgeAttribute_Of_PolygonMesh<ScalarType>::EdgeAttribute_Of_PolygonMesh()
{
    m_Data = std::make_unique<Data_Of_EdgeAttribute_Of_PolygonMesh<ScalarType>>();
}

template<typename ScalarType>
EdgeAttribute_Of_PolygonMesh<ScalarType>::EdgeAttribute_Of_PolygonMesh(const EdgeAttribute_Of_PolygonMesh<ScalarType>& InputAttribute)
{
    m_Data = std::make_unique<Data_Of_EdgeAttribute_Of_PolygonMesh<ScalarType>>();
    (*this) = InputAttribute;
}

template<typename ScalarType>
EdgeAttribute_Of_PolygonMesh<ScalarType>::EdgeAttribute_Of_PolygonMesh(EdgeAttribute_Of_PolygonMesh<ScalarType>&& InputAttribute)
{
    m_Data = std::move(InputAttribute.m_Data);
}

template<typename ScalarType>
EdgeAttribute_Of_PolygonMesh<ScalarType>::~EdgeAttribute_Of_PolygonMesh()
{
}

template<typename ScalarType>
void EdgeAttribute_Of_PolygonMesh<ScalarType>::operator=(const EdgeAttribute_Of_PolygonMesh<ScalarType>& InputAttribute)
{
    if (!m_Data)
    {
        m_Data = std::make_unique<Data_Of_EdgeAttribute_Of_PolygonMesh<ScalarType>>();
    }

    m_Data->PhysicalLength = InputAttribute.m_Data->PhysicalLength;
}

template<typename ScalarType>
void EdgeAttribute_Of_PolygonMesh<ScalarType>::operator=(EdgeAttribute_Of_PolygonMesh<ScalarType>&& InputAttribute)
{
    m_Data = std::move(InputAttribute.m_Data);
}

//============================================== DirectedEdgeAttribute_Of_PolygonMesh ===========================================//

template<typename ScalarType>
DirectedEdgeAttribute_Of_PolygonMesh<ScalarType>::DirectedEdgeAttribute_Of_PolygonMesh()
{
    m_Data = std::make_unique<Data_Of_DirectedEdgeAttribute_Of_PolygonMesh<ScalarType>>();
}

template<typename ScalarType>
DirectedEdgeAttribute_Of_PolygonMesh<ScalarType>::DirectedEdgeAttribute_Of_PolygonMesh(const DirectedEdgeAttribute_Of_PolygonMesh<ScalarType>& InputAttribute)
{
    m_Data = std::make_unique<Data_Of_DirectedEdgeAttribute_Of_PolygonMesh<ScalarType>>();
    (*this) = InputAttribute;
}

template<typename ScalarType>
DirectedEdgeAttribute_Of_PolygonMesh<ScalarType>::DirectedEdgeAttribute_Of_PolygonMesh(DirectedEdgeAttribute_Of_PolygonMesh<ScalarType>&& InputAttribute)
{
    m_Data = std::move(InputAttribute.m_Data);
}

template<typename ScalarType>
DirectedEdgeAttribute_Of_PolygonMesh<ScalarType>::~DirectedEdgeAttribute_Of_PolygonMesh()
{
}

template<typename ScalarType>
void DirectedEdgeAttribute_Of_PolygonMesh<ScalarType>::operator=(const DirectedEdgeAttribute_Of_PolygonMesh<ScalarType>& InputAttribute)
{
    if (!m_Data)
    {
        m_Data = std::make_unique<Data_Of_DirectedEdgeAttribute_Of_PolygonMesh<ScalarType>>();
    }

    m_Data->Orientation = InputAttribute.m_Data->Orientation;
}

template<typename ScalarType>
void DirectedEdgeAttribute_Of_PolygonMesh<ScalarType>::operator=(DirectedEdgeAttribute_Of_PolygonMesh<ScalarType>&& InputAttribute)
{
    m_Data = std::move(InputAttribute.m_Data);
}

//============================================== CellAttribute_Of_PolygonMesh ===========================================//

template<typename ScalarType>
CellAttribute_Of_PolygonMesh<ScalarType>::CellAttribute_Of_PolygonMesh()
{
    m_Data = std::make_unique<Data_Of_CellAttribute_Of_PolygonMesh<ScalarType>>();
}

template<typename ScalarType>
CellAttribute_Of_PolygonMesh<ScalarType>::CellAttribute_Of_PolygonMesh(const CellAttribute_Of_PolygonMesh<ScalarType>& InputAttribute)
{
    m_Data = std::make_unique<Data_Of_CellAttribute_Of_PolygonMesh<ScalarType>>();
    (*this) = InputAttribute;
}

template<typename ScalarType>
CellAttribute_Of_PolygonMesh<ScalarType>::CellAttribute_Of_PolygonMesh(CellAttribute_Of_PolygonMesh<ScalarType>&& InputAttribute)
{
    m_Data = std::move(InputAttribute.m_Data);
}

template<typename ScalarType>
CellAttribute_Of_PolygonMesh<ScalarType>::~CellAttribute_Of_PolygonMesh()
{
}

template<typename ScalarType>
void CellAttribute_Of_PolygonMesh<ScalarType>::operator=(const CellAttribute_Of_PolygonMesh<ScalarType>& InputAttribute)
{
    if (!m_Data)
    {
        m_Data = std::make_unique<Data_Of_CellAttribute_Of_PolygonMesh<ScalarType>>();
    }

    m_Data->Area = InputAttribute.m_Data->Area;
}

template<typename ScalarType>
void CellAttribute_Of_PolygonMesh<ScalarType>::operator=(CellAttribute_Of_PolygonMesh<ScalarType>&& InputAttribute)
{
    m_Data = std::move(InputAttribute.m_Data);
}

}// namespace mdk

#endif