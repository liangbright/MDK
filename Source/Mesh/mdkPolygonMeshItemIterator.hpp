#pragma once

namespace mdk
{
//================================================= Iterator_Of_Point_Of_PolygonMesh ==================================//

template<typename MeshAttribute>
inline
Iterator_Of_Point_Of_PolygonMesh<MeshAttribute>::Iterator_Of_Point_Of_PolygonMesh(const PolygonMesh<MeshAttribute>& ParentMesh)
: m_Mesh(MDK_PURE_EMPTY)
{
    m_Mesh.ForceShare(ParentMesh);
    this->SetToBegin();
}

template<typename MeshAttribute>
inline
Iterator_Of_Point_Of_PolygonMesh<MeshAttribute>::Iterator_Of_Point_Of_PolygonMesh(const Iterator_Of_Point_Of_PolygonMesh<MeshAttribute>& InputIterator)
: m_Mesh(MDK_PURE_EMPTY)
{
    (*this) = InputIterator;
}

template<typename MeshAttribute>
inline
Iterator_Of_Point_Of_PolygonMesh<MeshAttribute>::~Iterator_Of_Point_Of_PolygonMesh() 
{
}

template<typename MeshAttribute>
inline
void Iterator_Of_Point_Of_PolygonMesh<MeshAttribute>::operator=(const Iterator_Of_Point_Of_PolygonMesh<MeshAttribute>& InputIterator) const
{
    m_Mesh.ForceShare(InputIterator.m_Mesh);
    m_PointHandle = InputIterator.m_PointHandle;
    this->SetToBegin();
}

template<typename MeshAttribute>
inline  
Handle_Of_Point_Of_PolygonMesh Iterator_Of_Point_Of_PolygonMesh<MeshAttribute>::GetPointHandle() const
{
     return m_PointHandle;
}

template<typename MeshAttribute>
inline
int_max Iterator_Of_Point_Of_PolygonMesh<MeshAttribute>::GetPointID() const
{
    return m_Mesh.GetPointID(m_PointHandle);
}

template<typename MeshAttribute>
inline
Point_Of_PolygonMesh<MeshAttribute>& Iterator_Of_Point_Of_PolygonMesh<MeshAttribute>::Point()
{
    return m_Mesh.Point(m_PointHandle);
}

template<typename MeshAttribute>
inline
const Point_Of_PolygonMesh<MeshAttribute>& Iterator_Of_Point_Of_PolygonMesh<MeshAttribute>::Point() const
{
    return m_Mesh.Point(m_PointHandle);
}

template<typename MeshAttribute>
inline
void Iterator_Of_Point_Of_PolygonMesh<MeshAttribute>::operator+=(int_max Offset) const
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
				if (Counter == Offset)
				{
					m_PointHandle.SetIndex(k);
					return;
				}
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
				if (Counter == -Offset)
				{
					m_PointHandle.SetIndex(k);
					return;
				}
			}
        }
        m_PointHandle.SetIndex(-1);
    }
}

template<typename MeshAttribute>
inline
void Iterator_Of_Point_Of_PolygonMesh<MeshAttribute>::operator-=(int_max Offset) const
{
    (*this) += -Offset;
}

template<typename MeshAttribute>
inline
void Iterator_Of_Point_Of_PolygonMesh<MeshAttribute>::operator++() const
{
    (*this) += 1;
}

template<typename MeshAttribute>
inline
void Iterator_Of_Point_Of_PolygonMesh<MeshAttribute>::operator--() const
{
    (*this) -= 1;
}

template<typename MeshAttribute>
inline
bool Iterator_Of_Point_Of_PolygonMesh<MeshAttribute>::IsNotEnd() const
{
    auto PointIndex = m_PointHandle.GetIndex();
    return (PointIndex >= 0 && PointIndex < m_Mesh.m_MeshData->PointList.GetLength());
}

template<typename MeshAttribute>
inline
void Iterator_Of_Point_Of_PolygonMesh<MeshAttribute>::SetToBegin() const
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

//================================================= Iterator_Of_Edge_Of_PolygonMesh ==================================//

template<typename MeshAttribute>
inline Iterator_Of_Edge_Of_PolygonMesh<MeshAttribute>::Iterator_Of_Edge_Of_PolygonMesh(const PolygonMesh<MeshAttribute>& ParentMesh)
: m_Mesh(MDK_PURE_EMPTY)
{
    m_Mesh.ForceShare(ParentMesh);
    this->SetToBegin();
}

template<typename MeshAttribute>
inline
Iterator_Of_Edge_Of_PolygonMesh<MeshAttribute>::Iterator_Of_Edge_Of_PolygonMesh(const Iterator_Of_Edge_Of_PolygonMesh<MeshAttribute>& InputIterator)
: m_Mesh(MDK_PURE_EMPTY)
{
    (*this) = InputIterator;
}

template<typename MeshAttribute>
inline
Iterator_Of_Edge_Of_PolygonMesh<MeshAttribute>::~Iterator_Of_Edge_Of_PolygonMesh()
{
}

template<typename MeshAttribute>
inline
void Iterator_Of_Edge_Of_PolygonMesh<MeshAttribute>::operator=(const Iterator_Of_Edge_Of_PolygonMesh<MeshAttribute>& InputIterator) const
{
    m_Mesh.ForceShare(InputIterator.m_Mesh);
    m_EdgeHandle = InputIterator.m_EdgeHandle;
    this->SetToBegin();
}

template<typename MeshAttribute>
inline
Handle_Of_Edge_Of_PolygonMesh Iterator_Of_Edge_Of_PolygonMesh<MeshAttribute>::GetEdgeHandle() const
{
    return m_EdgeHandle;
}

template<typename MeshAttribute>
inline
int_max Iterator_Of_Edge_Of_PolygonMesh<MeshAttribute>::GetEdgeID() const
{
    return m_Mesh.GetEdgeID(m_EdgeHandle);
}

template<typename MeshAttribute>
inline
Edge_Of_PolygonMesh<MeshAttribute>& Iterator_Of_Edge_Of_PolygonMesh<MeshAttribute>::Edge()
{
    return m_Mesh.Edge(m_EdgeHandle);
}

template<typename MeshAttribute>
inline
const Edge_Of_PolygonMesh<MeshAttribute>& Iterator_Of_Edge_Of_PolygonMesh<MeshAttribute>::Edge() const
{
    return m_Mesh.Edge(m_EdgeHandle);
}

template<typename MeshAttribute>
inline
void Iterator_Of_Edge_Of_PolygonMesh<MeshAttribute>::operator+=(int_max Offset) const
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
				if (Counter == Offset)
				{
					m_EdgeHandle.SetIndex(k);
					return;
				}
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
				if (Counter == -Offset)
				{
					m_EdgeHandle.SetIndex(k);
					return;
				}
			}
        }
        m_EdgeHandle.SetIndex(-1);
    }
}

template<typename MeshAttribute>
inline
void Iterator_Of_Edge_Of_PolygonMesh<MeshAttribute>::operator-=(int_max Offset) const
{
    this->operator+=(-Offset);
}

template<typename MeshAttribute>
inline
void Iterator_Of_Edge_Of_PolygonMesh<MeshAttribute>::operator++() const
{
    (*this) += 1;
}

template<typename MeshAttribute>
inline
void Iterator_Of_Edge_Of_PolygonMesh<MeshAttribute>::operator--() const
{
    (*this) -= 1;
}

template<typename MeshAttribute>
inline
bool Iterator_Of_Edge_Of_PolygonMesh<MeshAttribute>::IsNotEnd() const
{
    auto EdgeIndex = m_EdgeHandle.GetIndex();
    return (EdgeIndex >= 0 && EdgeIndex < m_Mesh.m_MeshData->EdgeList.GetLength());
}

template<typename MeshAttribute>
inline
void Iterator_Of_Edge_Of_PolygonMesh<MeshAttribute>::SetToBegin() const
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

//================================================= Iterator_Of_DirectedEdge_Of_PolygonMesh ==================================//

template<typename MeshAttribute>
inline Iterator_Of_DirectedEdge_Of_PolygonMesh<MeshAttribute>::Iterator_Of_DirectedEdge_Of_PolygonMesh(const PolygonMesh<MeshAttribute>& ParentMesh)
: m_Mesh(MDK_PURE_EMPTY)
{
    m_Mesh.ForceShare(ParentMesh);
    this->SetToBegin();
}

template<typename MeshAttribute>
inline
Iterator_Of_DirectedEdge_Of_PolygonMesh<MeshAttribute>::
Iterator_Of_DirectedEdge_Of_PolygonMesh(const Iterator_Of_DirectedEdge_Of_PolygonMesh<MeshAttribute>& InputIterator)
: m_Mesh(MDK_PURE_EMPTY)
{
    (*this) = InputIterator;
}

template<typename MeshAttribute>
inline
Iterator_Of_DirectedEdge_Of_PolygonMesh<MeshAttribute>::~Iterator_Of_DirectedEdge_Of_PolygonMesh()
{
}

template<typename MeshAttribute>
inline
void Iterator_Of_DirectedEdge_Of_PolygonMesh<MeshAttribute>::operator=(const Iterator_Of_DirectedEdge_Of_PolygonMesh<MeshAttribute>& InputIterator) const
{
    m_Mesh.ForceShare(InputIterator.m_Mesh);
    m_DirectedEdgeHandle = InputIterator.m_DirectedEdgeHandle;
    this->SetToBegin();
}

template<typename MeshAttribute>
inline
Handle_Of_DirectedEdge_Of_PolygonMesh Iterator_Of_DirectedEdge_Of_PolygonMesh<MeshAttribute>::GetDirectedEdgeHandle() const
{
    return m_DirectedEdgeHandle;
}

template<typename MeshAttribute>
inline
int_max Iterator_Of_DirectedEdge_Of_PolygonMesh<MeshAttribute>::GetDirectedEdgeID() const
{
    return m_Mesh.GetDirectedEdgeID(m_DirectedEdgeHandle);
}

template<typename MeshAttribute>
inline
DirectedEdge_Of_PolygonMesh<MeshAttribute>& Iterator_Of_DirectedEdge_Of_PolygonMesh<MeshAttribute>::DirectedEdge()
{
    return m_Mesh.DirectedEdge(m_DirectedEdgeHandle);
}

template<typename MeshAttribute>
inline
const DirectedEdge_Of_PolygonMesh<MeshAttribute>& Iterator_Of_DirectedEdge_Of_PolygonMesh<MeshAttribute>::DirectedEdge() const
{
    return m_Mesh.DirectedEdge(m_DirectedEdgeHandle);
}

template<typename MeshAttribute>
inline
void Iterator_Of_DirectedEdge_Of_PolygonMesh<MeshAttribute>::operator+=(int_max Offset) const
{
    if (Offset == 0)
    {
        return;
    }
    else if (Offset > 0)
    {
        auto FaceIndex = m_DirectedEdgeHandle.GetFaceIndex();
        auto RelativeIndex = m_DirectedEdgeHandle.GetRelativeIndex();

		int_max Counter = 0;
		for (int_max k = FaceIndex; k < m_Mesh.m_MeshData->FaceList.GetLength(); ++k)
		{
			if (m_Mesh.m_MeshData->FaceValidityFlagList[k] == 1)
			{
				auto DirectedEdgeCount = m_Mesh.m_MeshData->FaceList[k].GetDirectedEdgeCount();
				int_max Index_s = -1;
				if (k == FaceIndex)
				{
					Index_s = RelativeIndex + 1;
				}
				else
				{
					Index_s = 0;
				}
				for (int_max n = Index_s; n < DirectedEdgeCount; ++n)
				{
					Counter += 1;
					if (Counter == Offset)
					{
						m_DirectedEdgeHandle.SetIndex(k, n);
						return;
					}
				}
			}
		}
		m_DirectedEdgeHandle.SetIndex(-1, -1);
    }
    else // Offset < 0
    {
		auto FaceIndex = m_DirectedEdgeHandle.GetFaceIndex();
		auto RelativeIndex = m_DirectedEdgeHandle.GetRelativeIndex();

		int_max Counter = 0;
		for (int_max k = FaceIndex; k >= 0; --k)
		{
			if (m_Mesh.m_MeshData->FaceValidityFlagList[k] == 1)
			{
				auto DirectedEdgeCount = m_Mesh.m_MeshData->FaceList[k].GetDirectedEdgeCount();
				int_max Index_s = -1;
				if (k == FaceIndex)
				{
					Index_s = RelativeIndex - 1;
				}
				else
				{
					Index_s = DirectedEdgeCount-1;
				}
				for (int_max n = Index_s; n >= 0; --n)
				{
					Counter += 1;
					if (Counter == -Offset)
					{
						m_DirectedEdgeHandle.SetIndex(k, n);
						return;
					}
				}
			}
		}
		m_DirectedEdgeHandle.SetIndex(-1, -1);
    }
}

template<typename MeshAttribute>
inline
void Iterator_Of_DirectedEdge_Of_PolygonMesh<MeshAttribute>::operator-=(int_max Offset) const
{
    this->operator+=(-Offset);
}

template<typename MeshAttribute>
inline
void Iterator_Of_DirectedEdge_Of_PolygonMesh<MeshAttribute>::operator++() const
{
    (*this) += 1;
}

template<typename MeshAttribute>
inline
void Iterator_Of_DirectedEdge_Of_PolygonMesh<MeshAttribute>::operator--() const
{
    (*this) -= 1;
}

template<typename MeshAttribute>
inline
bool Iterator_Of_DirectedEdge_Of_PolygonMesh<MeshAttribute>::IsNotEnd() const
{
    auto FaceIndex = m_DirectedEdgeHandle.GetFaceIndex();
    auto RelativeIndex = m_DirectedEdgeHandle.GetRelativeIndex();
	if (FaceIndex < 0 || FaceIndex >= m_Mesh.m_MeshData->FaceList.GetLength() || RelativeIndex < 0)
	{
		return false;
	}

	if (FaceIndex == m_Mesh.m_MeshData->FaceList.GetLength() - 1)
	{
		if (m_Mesh.m_MeshData->FaceValidityFlagList[FaceIndex] == 1)
		{
			auto DirectedEdgeCount = m_Mesh.m_MeshData->FaceList[FaceIndex].GetDirectedEdgeCount();
			if (RelativeIndex >= DirectedEdgeCount)
			{
				return false;
			}
		}
		else
		{
			return false;
		}
	}

	return true;
}

template<typename MeshAttribute>
inline
void Iterator_Of_DirectedEdge_Of_PolygonMesh<MeshAttribute>::SetToBegin() const
{
	for (int_max k = 0; k < m_Mesh.m_MeshData->FaceList.GetLength(); ++k)
    {
        if (m_Mesh.m_MeshData->FaceValidityFlagList[k] == 1)
        {
            m_DirectedEdgeHandle.SetIndex(k, 0);
            return;
        }
    }

    m_DirectedEdgeHandle.SetIndex(-1, -1);
}

//================================================= Iterator_Of_Face_Of_PolygonMesh ==================================//

template<typename MeshAttribute>
inline 
Iterator_Of_Face_Of_PolygonMesh<MeshAttribute>::Iterator_Of_Face_Of_PolygonMesh(const PolygonMesh<MeshAttribute>& ParentMesh)
: m_Mesh(MDK_PURE_EMPTY)
{
    m_Mesh.ForceShare(ParentMesh);
    this->SetToBegin();
}

template<typename MeshAttribute>
inline
Iterator_Of_Face_Of_PolygonMesh<MeshAttribute>::Iterator_Of_Face_Of_PolygonMesh(const Iterator_Of_Face_Of_PolygonMesh<MeshAttribute>& InputIterator)
: m_Mesh(MDK_PURE_EMPTY)
{
    (*this) = InputIterator;
}

template<typename MeshAttribute>
inline
Iterator_Of_Face_Of_PolygonMesh<MeshAttribute>::~Iterator_Of_Face_Of_PolygonMesh() 
{
}

template<typename MeshAttribute>
inline
void Iterator_Of_Face_Of_PolygonMesh<MeshAttribute>::operator=(const Iterator_Of_Face_Of_PolygonMesh<MeshAttribute>& InputIterator) const
{
    m_Mesh.ForceShare(InputIterator.m_Mesh);
    m_FaceHandle = InputIterator.m_FaceHandle;
    this->SetToBegin();
}

template<typename MeshAttribute>
inline
Handle_Of_Face_Of_PolygonMesh Iterator_Of_Face_Of_PolygonMesh<MeshAttribute>::GetFaceHandle() const
{
    return m_FaceHandle;
}

template<typename MeshAttribute>
inline
int_max Iterator_Of_Face_Of_PolygonMesh<MeshAttribute>::GetFaceID() const
{
    return m_Mesh.GetFaceID(m_FaceHandle);
}

template<typename MeshAttribute>
inline
Face_Of_PolygonMesh<MeshAttribute>& Iterator_Of_Face_Of_PolygonMesh<MeshAttribute>::Face()
{
    return m_Mesh.Face(m_FaceHandle);
}

template<typename MeshAttribute>
inline
const Face_Of_PolygonMesh<MeshAttribute>& Iterator_Of_Face_Of_PolygonMesh<MeshAttribute>::Face() const
{
    return m_Mesh.Face(m_FaceHandle);
}

template<typename MeshAttribute>
inline
void Iterator_Of_Face_Of_PolygonMesh<MeshAttribute>::operator+=(int_max Offset) const
{
    if (Offset == 0)
    {
        return;
    }
    else if (Offset > 0)
    {
        auto FaceIndex = m_FaceHandle.GetIndex();

        if (FaceIndex + Offset >= m_Mesh.m_MeshData->FaceList.GetLength())
        {
            m_FaceHandle.SetIndex(-1);
            return;
        }

        int_max Counter = 0;
        for (int_max k = FaceIndex + 1; k < m_Mesh.m_MeshData->FaceList.GetLength(); ++k)
        {
			if (m_Mesh.m_MeshData->FaceList[k].IsValid() == true)
			{
				Counter += 1;
				if (Counter == Offset)
				{
					m_FaceHandle.SetIndex(k);
					return;
				}
			}
        }

        m_FaceHandle.SetIndex(-1);
    }
    else // Offset < 0
    {
        auto FaceIndex = m_FaceHandle.GetIndex();

        if (FaceIndex + Offset < 0)
        {
            m_FaceHandle.SetIndex(-1);
            return;
        }

        int_max Counter = 0;
        for (int_max k = FaceIndex - 1; k >= 0; --k)
        {
			if (m_Mesh.m_MeshData->FaceList[k].IsValid() == true)
			{
				Counter += 1;
				if (Counter == -Offset)
				{
					m_FaceHandle.SetIndex(k);
					return;
				}
			}
        }
        m_FaceHandle.SetIndex(-1);
    }
}

template<typename MeshAttribute>
inline
void Iterator_Of_Face_Of_PolygonMesh<MeshAttribute>::operator-=(int_max Offset) const
{
    this->operator+=(-Offset);
}

template<typename MeshAttribute>
inline
void Iterator_Of_Face_Of_PolygonMesh<MeshAttribute>::operator++() const
{
    (*this) += 1;
}

template<typename MeshAttribute>
inline
void Iterator_Of_Face_Of_PolygonMesh<MeshAttribute>::operator--() const
{
    (*this) -= 1;
}

template<typename MeshAttribute>
inline
bool Iterator_Of_Face_Of_PolygonMesh<MeshAttribute>::IsNotEnd() const
{
    auto FaceIndex = m_FaceHandle.GetIndex();
    return (FaceIndex >= 0 && FaceIndex < m_Mesh.m_MeshData->FaceList.GetLength());
}

template<typename MeshAttribute>
inline
void Iterator_Of_Face_Of_PolygonMesh<MeshAttribute>::SetToBegin() const
{
    for (int_max k = 0; k < m_Mesh.m_MeshData->FaceList.GetLength(); ++k)
    {
        if (m_Mesh.m_MeshData->FaceList[k].IsValid() == true)
        {
            m_FaceHandle.SetIndex(k);
            return;
        }
    }
    m_FaceHandle.SetIndex(-1);
}

}// namespace mdk
