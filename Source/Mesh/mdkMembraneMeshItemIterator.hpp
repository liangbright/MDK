#ifndef __mdkMembraneMeshItemIterator_hpp
#define __mdkMembraneMeshItemIterator_hpp

namespace mdk
{
//================================================= Iterator_Of_Point_Of_MembraneMesh ==================================//

template<typename MeshAttribute>
inline
Iterator_Of_Point_Of_MembraneMesh<MeshAttribute>::Iterator_Of_Point_Of_MembraneMesh(const MembraneMesh<MeshAttribute>& ParentMesh)
: m_Mesh(MDK_PURE_EMPTY_MembraneMesh)
{
    m_Mesh.ForceShare(ParentMesh);
    this->SetToBegin();
}

template<typename MeshAttribute>
inline
Iterator_Of_Point_Of_MembraneMesh<MeshAttribute>::Iterator_Of_Point_Of_MembraneMesh(const Iterator_Of_Point_Of_MembraneMesh<MeshAttribute>& InputIterator)
: m_Mesh(MDK_PURE_EMPTY_MembraneMesh)
{
    (*this) = InputIterator;
}

template<typename MeshAttribute>
inline
Iterator_Of_Point_Of_MembraneMesh<MeshAttribute>::~Iterator_Of_Point_Of_MembraneMesh() 
{
}

template<typename MeshAttribute>
inline
void Iterator_Of_Point_Of_MembraneMesh<MeshAttribute>::operator=(const Iterator_Of_Point_Of_MembraneMesh<MeshAttribute>& InputIterator) const
{
    m_Mesh.ForceShare(InputIterator.m_Mesh);
    m_PointHandle = InputIterator.m_PointHandle;
    this->SetToBegin();
}

template<typename MeshAttribute>
inline  
Handle_Of_Point_Of_MembraneMesh Iterator_Of_Point_Of_MembraneMesh<MeshAttribute>::GetPointHandle() const
{
     return m_PointHandle;
}

template<typename MeshAttribute>
inline
int_max Iterator_Of_Point_Of_MembraneMesh<MeshAttribute>::GetPointID() const
{
    return m_Mesh.GetPointID(m_PointHandle);
}

template<typename MeshAttribute>
inline
Point_Of_MembraneMesh<MeshAttribute>& Iterator_Of_Point_Of_MembraneMesh<MeshAttribute>::Point()
{
    return m_Mesh.Point(m_PointHandle);
}

template<typename MeshAttribute>
inline
const Point_Of_MembraneMesh<MeshAttribute>& Iterator_Of_Point_Of_MembraneMesh<MeshAttribute>::Point() const
{
    return m_Mesh.Point(m_PointHandle);
}

template<typename MeshAttribute>
inline
void Iterator_Of_Point_Of_MembraneMesh<MeshAttribute>::operator+=(int_max Offset) const
{
    if (Offset == 0)
    {
        return;
    }
    else if (Offset > 0)
    {
        auto PointIndex = m_PointHandle.GetIndex();

        if (PointIndex + Offset >= m_Mesh.m_MeshData->PointList.GetLength())
        {
            m_PointHandle.SetIndex(-1);
            return;
        }

        int_max Counter = 0;
        for (int_max k = PointIndex + 1; k < m_Mesh.m_MeshData->PointList.GetLength(); ++k)
        {
            if (m_Mesh.m_MeshData->PointList[k].IsValid() == true)
            {
                Counter += 1;
            }

            if (Counter == Offset)
            {
                m_PointHandle.SetIndex(k);
                return;
            }

            if (Counter > Offset)
            {
                m_PointHandle.SetIndex(-1);
                return;
            }
        }

        m_PointHandle.SetIndex(-1);
    }
    else // Offset < 0
    {
        auto PointIndex = m_PointHandle.GetIndex();

        if (PointIndex + Offset < 0)
        {
            m_PointHandle.SetIndex(-1);
            return;
        }

        int_max Counter = 0;
        for (int_max k = PointIndex - 1; k >= 0; --k)
        {
            if (m_Mesh.m_MeshData->PointList[k].IsValid() == true)
            {
                Counter += 1;
            }

            if (Counter == -Offset)
            {
                m_PointHandle.SetIndex(k);
                return;
            }

            if (Counter > -Offset)
            {
                m_PointHandle.SetIndex(-1);
                return;
            }
        }
        m_PointHandle.SetIndex(-1);
    }
}

template<typename MeshAttribute>
inline
void Iterator_Of_Point_Of_MembraneMesh<MeshAttribute>::operator-=(int_max Offset) const
{
    (*this) += -Offset;
}

template<typename MeshAttribute>
inline
void Iterator_Of_Point_Of_MembraneMesh<MeshAttribute>::operator++() const
{
    (*this) += 1;
}

template<typename MeshAttribute>
inline
void Iterator_Of_Point_Of_MembraneMesh<MeshAttribute>::operator--() const
{
    (*this) -= 1;
}

template<typename MeshAttribute>
inline
bool Iterator_Of_Point_Of_MembraneMesh<MeshAttribute>::IsNotEnd() const
{
    auto PointIndex = m_PointHandle.GetIndex();
    return (PointIndex >= 0 && PointIndex < m_Mesh.m_MeshData->PointList.GetLength());
}

template<typename MeshAttribute>
inline
void Iterator_Of_Point_Of_MembraneMesh<MeshAttribute>::SetToBegin() const
{
    for (int_max k = 0; k < m_Mesh.m_MeshData->PointList.GetLength(); ++k)
    {
        if (m_Mesh.m_MeshData->PointList[k].IsValid() == true)
        {
            m_PointHandle.SetIndex(k);
            return;
        }
    }
    m_PointHandle.SetIndex(-1);
}

//================================================= Iterator_Of_Edge_Of_MembraneMesh ==================================//

template<typename MeshAttribute>
inline Iterator_Of_Edge_Of_MembraneMesh<MeshAttribute>::Iterator_Of_Edge_Of_MembraneMesh(const MembraneMesh<MeshAttribute>& ParentMesh)
: m_Mesh(MDK_PURE_EMPTY_MembraneMesh)
{
    m_Mesh.ForceShare(ParentMesh);
    this->SetToBegin();
}

template<typename MeshAttribute>
inline
Iterator_Of_Edge_Of_MembraneMesh<MeshAttribute>::Iterator_Of_Edge_Of_MembraneMesh(const Iterator_Of_Edge_Of_MembraneMesh<MeshAttribute>& InputIterator)
: m_Mesh(MDK_PURE_EMPTY_MembraneMesh)
{
    (*this) = InputIterator;
}

template<typename MeshAttribute>
inline
Iterator_Of_Edge_Of_MembraneMesh<MeshAttribute>::~Iterator_Of_Edge_Of_MembraneMesh()
{
}

template<typename MeshAttribute>
inline
void Iterator_Of_Edge_Of_MembraneMesh<MeshAttribute>::operator=(const Iterator_Of_Edge_Of_MembraneMesh<MeshAttribute>& InputIterator) const
{
    m_Mesh.ForceShare(InputIterator.m_Mesh);
    m_EdgeHandle = InputIterator.m_EdgeHandle;
    this->SetToBegin();
}

template<typename MeshAttribute>
inline
Handle_Of_Edge_Of_MembraneMesh Iterator_Of_Edge_Of_MembraneMesh<MeshAttribute>::GetEdgeHandle() const
{
    return m_EdgeHandle;
}

template<typename MeshAttribute>
inline
int_max Iterator_Of_Edge_Of_MembraneMesh<MeshAttribute>::GetEdgeID() const
{
    return m_Mesh.GetEdgeID(m_EdgeHandle);
}

template<typename MeshAttribute>
inline
Edge_Of_MembraneMesh<MeshAttribute>& Iterator_Of_Edge_Of_MembraneMesh<MeshAttribute>::Edge()
{
    return m_Mesh.Edge(m_EdgeHandle);
}

template<typename MeshAttribute>
inline
const Edge_Of_MembraneMesh<MeshAttribute>& Iterator_Of_Edge_Of_MembraneMesh<MeshAttribute>::Edge() const
{
    return m_Mesh.Edge(m_EdgeHandle);
}

template<typename MeshAttribute>
inline
void Iterator_Of_Edge_Of_MembraneMesh<MeshAttribute>::operator+=(int_max Offset) const
{
    if (Offset == 0)
    {
        return;
    }
    else if (Offset > 0)
    {
        auto EdgeIndex = m_EdgeHandle.GetIndex();

        if (EdgeIndex + Offset >= m_Mesh.m_MeshData->EdgeList.GetLength())
        {
            m_EdgeHandle.SetIndex(-1);
            return;
        }

        int_max Counter = 0;
        for (int_max k = EdgeIndex + 1; k < m_Mesh.m_MeshData->EdgeList.GetLength(); ++k)
        {
            if (m_Mesh.m_MeshData->EdgeList[k].IsValid() == true)
            {
                Counter += 1;
            }

            if (Counter == Offset)
            {
                m_EdgeHandle.SetIndex(k);
                return;
            }

            if (Counter > Offset)
            {
                m_EdgeHandle.SetIndex(-1);
                return;
            }
        }

        m_EdgeHandle.SetIndex(-1);
    }
    else // Offset < 0
    {
        auto EdgeIndex = m_EdgeHandle.GetIndex();

        if (EdgeIndex + Offset < 0)
        {
            m_EdgeHandle.SetIndex(-1);
            return;
        }

        int_max Counter = 0;
        for (int_max k = EdgeIndex - 1; k >= 0; --k)
        {
            if (m_Mesh.m_MeshData->EdgeList[k].IsValid() == true)
            {
                Counter += 1;
            }

            if (Counter == -Offset)
            {
                m_EdgeHandle.SetIndex(k);
                return;
            }

            if (Counter > -Offset)
            {
                m_EdgeHandle.SetIndex(-1);
                return;
            }
        }
        m_EdgeHandle.SetIndex(-1);
    }
}

template<typename MeshAttribute>
inline
void Iterator_Of_Edge_Of_MembraneMesh<MeshAttribute>::operator-=(int_max Offset) const
{
    this->operator+=(-Offset);
}

template<typename MeshAttribute>
inline
void Iterator_Of_Edge_Of_MembraneMesh<MeshAttribute>::operator++() const
{
    (*this) += 1;
}

template<typename MeshAttribute>
inline
void Iterator_Of_Edge_Of_MembraneMesh<MeshAttribute>::operator--() const
{
    (*this) -= 1;
}

template<typename MeshAttribute>
inline
bool Iterator_Of_Edge_Of_MembraneMesh<MeshAttribute>::IsNotEnd() const
{
    auto EdgeIndex = m_EdgeHandle.GetIndex();
    return (EdgeIndex >= 0 && EdgeIndex < m_Mesh.m_MeshData->EdgeList.GetLength());
}

template<typename MeshAttribute>
inline
void Iterator_Of_Edge_Of_MembraneMesh<MeshAttribute>::SetToBegin() const
{
    for (int_max k = 0; k < m_Mesh.m_MeshData->EdgeList.GetLength(); ++k)
    {
        if (m_Mesh.m_MeshData->EdgeList[k].IsValid() == true)
        {
            m_EdgeHandle.SetIndex(k);
            return;
        }
    }
    m_EdgeHandle.SetIndex(-1);
}

//================================================= Iterator_Of_DirectedEdge_Of_MembraneMesh ==================================//

template<typename MeshAttribute>
inline Iterator_Of_DirectedEdge_Of_MembraneMesh<MeshAttribute>::Iterator_Of_DirectedEdge_Of_MembraneMesh(const MembraneMesh<MeshAttribute>& ParentMesh)
: m_Mesh(MDK_PURE_EMPTY_MembraneMesh)
{
    m_Mesh.ForceShare(ParentMesh);
    this->SetToBegin();
}

template<typename MeshAttribute>
inline
Iterator_Of_DirectedEdge_Of_MembraneMesh<MeshAttribute>::
Iterator_Of_DirectedEdge_Of_MembraneMesh(const Iterator_Of_DirectedEdge_Of_MembraneMesh<MeshAttribute>& InputIterator)
: m_Mesh(MDK_PURE_EMPTY_MembraneMesh)
{
    (*this) = InputIterator;
}

template<typename MeshAttribute>
inline
Iterator_Of_DirectedEdge_Of_MembraneMesh<MeshAttribute>::~Iterator_Of_DirectedEdge_Of_MembraneMesh()
{
}

template<typename MeshAttribute>
inline
void Iterator_Of_DirectedEdge_Of_MembraneMesh<MeshAttribute>::operator=(const Iterator_Of_DirectedEdge_Of_MembraneMesh<MeshAttribute>& InputIterator) const
{
    m_Mesh.ForceShare(InputIterator.m_Mesh);
    m_DirectedEdgeHandle = InputIterator.m_DirectedEdgeHandle;
    this->SetToBegin();
}

template<typename MeshAttribute>
inline
Handle_Of_DirectedEdge_Of_MembraneMesh Iterator_Of_DirectedEdge_Of_MembraneMesh<MeshAttribute>::GetDirectedEdgeHandle() const
{
    return m_DirectedEdgeHandle;
}

template<typename MeshAttribute>
inline
int_max Iterator_Of_DirectedEdge_Of_MembraneMesh<MeshAttribute>::GetDirectedEdgeID() const
{
    return m_Mesh.GetDirectedEdgeID(m_DirectedEdgeHandle);
}

template<typename MeshAttribute>
inline
DirectedEdge_Of_MembraneMesh<MeshAttribute>& Iterator_Of_DirectedEdge_Of_MembraneMesh<MeshAttribute>::DirectedEdge()
{
    return m_Mesh.DirectedEdge(m_DirectedEdgeHandle);
}

template<typename MeshAttribute>
inline
const DirectedEdge_Of_MembraneMesh<MeshAttribute>& Iterator_Of_DirectedEdge_Of_MembraneMesh<MeshAttribute>::DirectedEdge() const
{
    return m_Mesh.DirectedEdge(m_DirectedEdgeHandle);
}

template<typename MeshAttribute>
inline
void Iterator_Of_DirectedEdge_Of_MembraneMesh<MeshAttribute>::operator+=(int_max Offset) const
{
    if (Offset == 0)
    {
        return;
    }
    else if (Offset > 0)
    {
        auto EdgeIndex = m_DirectedEdgeHandle.GetEdgeIndex();
        auto RelativeIndex = m_DirectedEdgeHandle.GetRelativeIndex();

        if (EdgeIndex == m_Mesh.m_MeshData->DirectedEdgePairList.GetLength() - 1 && RelativeIndex == 1)
        {
            m_DirectedEdgeHandle.SetIndex(-1, -1);
            return;
        }

        int_max Counter = 0;

        if (RelativeIndex == 0)
        {
            if (m_Mesh.m_MeshData->EdgeValidityFlagList[EdgeIndex] == 1)
            {
                Counter += 1;
                if (Counter == Offset)
                {
                    m_DirectedEdgeHandle.SetIndex(EdgeIndex, 1);
                    return;
                }
            }
        }
        
        for (int_max k = EdgeIndex + 1; k < m_Mesh.m_MeshData->DirectedEdgePairList.GetLength(); ++k)
        {
            if (m_Mesh.m_MeshData->EdgeValidityFlagList[EdgeIndex] == 1)
            {
                Counter += 1;

                if (Counter == Offset)
                {
                    m_DirectedEdgeHandle.SetIndex(k, 0);
                    return;
                }
            }

            if (Counter > Offset)
            {
                m_DirectedEdgeHandle.SetIndex(-1, -1);
                return;
            }

            if (m_Mesh.m_MeshData->EdgeValidityFlagList[EdgeIndex] == 1)
            {
                Counter += 1;

                if (Counter == Offset)
                {
                    m_DirectedEdgeHandle.SetIndex(k, 1);
                    return;
                }
            }

            if (Counter > Offset)
            {
                m_DirectedEdgeHandle.SetIndex(-1, -1);
                return;
            }
        }

        m_DirectedEdgeHandle.SetIndex(-1, -1);
        return;
    }
    else // Offset < 0
    {
        auto EdgeIndex = m_DirectedEdgeHandle.GetEdgeIndex();
        auto RelativeIndex = m_DirectedEdgeHandle.GetRelativeIndex();

        if (EdgeIndex == 0 && RelativeIndex == 0)
        {
            m_DirectedEdgeHandle.SetIndex(-1, 1);
            return;
        }

        int_max Counter = 0;

        if (RelativeIndex == 1)
        {
            if (m_Mesh.m_MeshData->EdgeValidityFlagList[EdgeIndex] == 1)
            {
                Counter += 1;
                if (Counter == -Offset)
                {
                    m_DirectedEdgeHandle.SetIndex(EdgeIndex, 0);
                    return;
                }
            }
        }
        
        for (int_max k = EdgeIndex - 1; k >= 0; --k)
        {
            if (m_Mesh.m_MeshData->EdgeValidityFlagList[EdgeIndex] == 1)
            {
                Counter += 1;

                if (Counter == -Offset)
                {
                    m_DirectedEdgeHandle.SetIndex(k, 0);
                    return;
                }
            }

            if (m_Mesh.m_MeshData->EdgeValidityFlagList[EdgeIndex] == 1)
            {
                Counter += 1;

                if (Counter == -Offset)
                {
                    m_DirectedEdgeHandle.SetIndex(k, 1);
                    return;
                }
            }

            if (Counter > -Offset)
            {
                m_DirectedEdgeHandle.SetIndex(-1, 1);
                return;
            }
        }

        m_DirectedEdgeHandle.SetIndex(-1, 1);
        return;
    }
}

template<typename MeshAttribute>
inline
void Iterator_Of_DirectedEdge_Of_MembraneMesh<MeshAttribute>::operator-=(int_max Offset) const
{
    this->operator+=(-Offset);
}

template<typename MeshAttribute>
inline
void Iterator_Of_DirectedEdge_Of_MembraneMesh<MeshAttribute>::operator++() const
{
    (*this) += 1;
}

template<typename MeshAttribute>
inline
void Iterator_Of_DirectedEdge_Of_MembraneMesh<MeshAttribute>::operator--() const
{
    (*this) -= 1;
}

template<typename MeshAttribute>
inline
bool Iterator_Of_DirectedEdge_Of_MembraneMesh<MeshAttribute>::IsNotEnd() const
{
    auto EdgeIndex = m_DirectedEdgeHandle.GetEdgeIndex();
    auto RelativeIndex = m_DirectedEdgeHandle.GetRelativeIndex();
    return (EdgeIndex >= 0 && EdgeIndex < m_Mesh.m_MeshData->EdgeList.GetLength() && RelativeIndex >= 0 && RelativeIndex <= 1);
}

template<typename MeshAttribute>
inline
void Iterator_Of_DirectedEdge_Of_MembraneMesh<MeshAttribute>::SetToBegin() const
{
    for (int_max k = 0; k < m_Mesh.m_MeshData->DirectedEdgePairList.GetLength(); ++k)
    {
        if (m_Mesh.m_MeshData->EdgeValidityFlagList[EdgeIndex] == 1)
        {
            m_DirectedEdgeHandle.SetIndex(k, 0);
            return;
        }
    }

    m_DirectedEdgeHandle.SetIndex(-1, -1);
}

//================================================= Iterator_Of_Cell_Of_MembraneMesh ==================================//

template<typename MeshAttribute>
inline 
Iterator_Of_Cell_Of_MembraneMesh<MeshAttribute>::Iterator_Of_Cell_Of_MembraneMesh(const MembraneMesh<MeshAttribute>& ParentMesh)
: m_Mesh(MDK_PURE_EMPTY_MembraneMesh)
{
    m_Mesh.ForceShare(ParentMesh);
    this->SetToBegin();
}

template<typename MeshAttribute>
inline
Iterator_Of_Cell_Of_MembraneMesh<MeshAttribute>::Iterator_Of_Cell_Of_MembraneMesh(const Iterator_Of_Cell_Of_MembraneMesh<MeshAttribute>& InputIterator)
: m_Mesh(MDK_PURE_EMPTY_MembraneMesh)
{
    (*this) = InputIterator;
}

template<typename MeshAttribute>
inline
Iterator_Of_Cell_Of_MembraneMesh<MeshAttribute>::~Iterator_Of_Cell_Of_MembraneMesh() 
{
}

template<typename MeshAttribute>
inline
void Iterator_Of_Cell_Of_MembraneMesh<MeshAttribute>::operator=(const Iterator_Of_Cell_Of_MembraneMesh<MeshAttribute>& InputIterator) const
{
    m_Mesh.ForceShare(InputIterator.m_Mesh);
    m_CellHandle = InputIterator.m_CellHandle;
    this->SetToBegin();
}

template<typename MeshAttribute>
inline
Handle_Of_Cell_Of_MembraneMesh Iterator_Of_Cell_Of_MembraneMesh<MeshAttribute>::GetCellHandle() const
{
    return m_CellHandle;
}

template<typename MeshAttribute>
inline
int_max Iterator_Of_Cell_Of_MembraneMesh<MeshAttribute>::GetCellID() const
{
    return m_Mesh.GetCellID(m_CellHandle);
}

template<typename MeshAttribute>
inline
Cell_Of_MembraneMesh<MeshAttribute>& Iterator_Of_Cell_Of_MembraneMesh<MeshAttribute>::Cell()
{
    return m_Mesh.Cell(m_CellHandle);
}

template<typename MeshAttribute>
inline
const Cell_Of_MembraneMesh<MeshAttribute>& Iterator_Of_Cell_Of_MembraneMesh<MeshAttribute>::Cell() const
{
    return m_Mesh.Cell(m_CellHandle);
}

template<typename MeshAttribute>
inline
void Iterator_Of_Cell_Of_MembraneMesh<MeshAttribute>::operator+=(int_max Offset) const
{
    if (Offset == 0)
    {
        return;
    }
    else if (Offset > 0)
    {
        auto CellIndex = m_CellHandle.GetIndex();

        if (CellIndex + Offset >= m_Mesh.m_MeshData->CellList.GetLength())
        {
            m_CellHandle.SetIndex(-1);
            return;
        }

        int_max Counter = 0;
        for (int_max k = CellIndex + 1; k < m_Mesh.m_MeshData->CellList.GetLength(); ++k)
        {
            if (m_Mesh.m_MeshData->CellList[k].IsValid() == true)
            {
                Counter += 1;
            }

            if (Counter == Offset)
            {
                m_CellHandle.SetIndex(k);
                return;
            }

            if (Counter > Offset)
            {
                m_CellHandle.SetIndex(-1);
                return;
            }
        }

        m_CellHandle.SetIndex(-1);
    }
    else // Offset < 0
    {
        auto CellIndex = m_CellHandle.GetIndex();

        if (CellIndex + Offset < 0)
        {
            m_CellHandle.SetIndex(-1);
            return;
        }

        int_max Counter = 0;
        for (int_max k = CellIndex - 1; k >= 0; --k)
        {
            if (m_Mesh.m_MeshData->CellList[k].IsValid() == true)
            {
                Counter += 1;
            }

            if (Counter == -Offset)
            {
                m_CellHandle.SetIndex(k);
                return;
            }

            if (Counter > -Offset)
            {
                m_CellHandle.SetIndex(-1);
                return;
            }
        }
        m_CellHandle.SetIndex(-1);
    }
}

template<typename MeshAttribute>
inline
void Iterator_Of_Cell_Of_MembraneMesh<MeshAttribute>::operator-=(int_max Offset) const
{
    this->operator+=(-Offset);
}

template<typename MeshAttribute>
inline
void Iterator_Of_Cell_Of_MembraneMesh<MeshAttribute>::operator++() const
{
    (*this) += 1;
}

template<typename MeshAttribute>
inline
void Iterator_Of_Cell_Of_MembraneMesh<MeshAttribute>::operator--() const
{
    (*this) -= 1;
}

template<typename MeshAttribute>
inline
bool Iterator_Of_Cell_Of_MembraneMesh<MeshAttribute>::IsNotEnd() const
{
    auto CellIndex = m_CellHandle.GetIndex();
    return (CellIndex >= 0 && CellIndex < m_Mesh.m_MeshData->CellList.GetLength());
}

template<typename MeshAttribute>
inline
void Iterator_Of_Cell_Of_MembraneMesh<MeshAttribute>::SetToBegin() const
{
    for (int_max k = 0; k < m_Mesh.m_MeshData->CellList.GetLength(); ++k)
    {
        if (m_Mesh.m_MeshData->CellList[k].IsValid() == true)
        {
            m_CellHandle.SetIndex(k);
            return;
        }
    }
    m_CellHandle.SetIndex(-1);
}

}// namespace mdk

#endif