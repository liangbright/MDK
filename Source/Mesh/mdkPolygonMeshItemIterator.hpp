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
    m_PointIndex = InputIterator.m_PointIndex;
    this->SetToBegin();
}

template<typename MeshAttribute>
inline  
int_max Iterator_Of_Point_Of_PolygonMesh<MeshAttribute>::GetPointIndex() const
{
     return m_PointIndex;
}

template<typename MeshAttribute>
inline
Point_Of_PolygonMesh<MeshAttribute>& Iterator_Of_Point_Of_PolygonMesh<MeshAttribute>::Point()
{
    return m_Mesh.Point(m_PointIndex);
}

template<typename MeshAttribute>
inline
const Point_Of_PolygonMesh<MeshAttribute>& Iterator_Of_Point_Of_PolygonMesh<MeshAttribute>::Point() const
{
    return m_Mesh.Point(m_PointIndex);
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
        auto PointIndex = m_PointIndex;

        if (PointIndex + Offset >= m_Mesh.m_MeshData->PointList.GetLength())
        {
            m_PointIndex = -1;
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
					m_PointIndex = k;
					return;
				}
			}
        }

        m_PointIndex = -1;
    }
    else // Offset < 0
    {
        auto PointIndex = m_PointIndex;

        if (PointIndex + Offset < 0)
        {
            m_PointIndex = -1;
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
					m_PointIndex = k;
					return;
				}
			}
        }
        m_PointIndex = -1;
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
    return (m_PointIndex >= 0 && m_PointIndex < m_Mesh.m_MeshData->PointList.GetLength());
}

template<typename MeshAttribute>
inline
void Iterator_Of_Point_Of_PolygonMesh<MeshAttribute>::SetToBegin() const
{
    for (int_max k = 0; k < m_Mesh.m_MeshData->PointList.GetLength(); ++k)
    {
        if (m_Mesh.m_MeshData->PointList[k].IsValid() == true)
        {
            m_PointIndex = k;
            return;
        }
    }
    m_PointIndex = -1;
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
    m_EdgeIndex = InputIterator.m_EdgeIndex;
    this->SetToBegin();
}

template<typename MeshAttribute>
inline
int_max Iterator_Of_Edge_Of_PolygonMesh<MeshAttribute>::GetEdgeIndex() const
{
    return m_EdgeIndex;
}

template<typename MeshAttribute>
inline
Edge_Of_PolygonMesh<MeshAttribute>& Iterator_Of_Edge_Of_PolygonMesh<MeshAttribute>::Edge()
{
    return m_Mesh.Edge(m_EdgeIndex);
}

template<typename MeshAttribute>
inline
const Edge_Of_PolygonMesh<MeshAttribute>& Iterator_Of_Edge_Of_PolygonMesh<MeshAttribute>::Edge() const
{
    return m_Mesh.Edge(m_EdgeIndex);
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
        auto EdgeIndex = m_EdgeIndex;

        if (EdgeIndex + Offset >= m_Mesh.m_MeshData->EdgeList.GetLength())
        {
            m_EdgeIndex = -1;
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
					m_EdgeIndex = k;
					return;
				}
			}
        }

        m_EdgeIndex = -1;
    }
    else // Offset < 0
    {
        auto EdgeIndex = m_EdgeIndex;

        if (EdgeIndex + Offset < 0)
        {
            m_EdgeIndex = -1;
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
					m_EdgeIndex = k;
					return;
				}
			}
        }
        m_EdgeIndex = -1;
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
    return (m_EdgeIndex >= 0 && m_EdgeIndex < m_Mesh.m_MeshData->EdgeList.GetLength());
}

template<typename MeshAttribute>
inline
void Iterator_Of_Edge_Of_PolygonMesh<MeshAttribute>::SetToBegin() const
{
    for (int_max k = 0; k < m_Mesh.m_MeshData->EdgeList.GetLength(); ++k)
    {
        if (m_Mesh.m_MeshData->EdgeList[k].IsValid() == true)
        {
            m_EdgeIndex = k;
            return;
        }
    }
    m_EdgeIndex = -1;
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
    m_FaceIndex = InputIterator.m_FaceIndex;
    this->SetToBegin();
}

template<typename MeshAttribute>
inline
int_max Iterator_Of_Face_Of_PolygonMesh<MeshAttribute>::GetFaceIndex() const
{
    return m_FaceIndex;
}

template<typename MeshAttribute>
inline
Face_Of_PolygonMesh<MeshAttribute>& Iterator_Of_Face_Of_PolygonMesh<MeshAttribute>::Face()
{
    return m_Mesh.Face(m_FaceIndex);
}

template<typename MeshAttribute>
inline
const Face_Of_PolygonMesh<MeshAttribute>& Iterator_Of_Face_Of_PolygonMesh<MeshAttribute>::Face() const
{
    return m_Mesh.Face(m_FaceIndex);
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
        auto FaceIndex = m_FaceIndex;

        if (FaceIndex + Offset >= m_Mesh.m_MeshData->FaceList.GetLength())
        {
            m_FaceIndex = -1;
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
					m_FaceIndex = k;
					return;
				}
			}
        }

        m_FaceIndex = -1;
    }
    else // Offset < 0
    {
        auto FaceIndex = m_FaceIndex;

        if (FaceIndex + Offset < 0)
        {
            m_FaceIndex = -1;
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
					m_FaceIndex = k;
					return;
				}
			}
        }
        m_FaceIndex = -1;
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
    return (m_FaceIndex >= 0 && m_FaceIndex < m_Mesh.m_MeshData->FaceList.GetLength());
}

template<typename MeshAttribute>
inline
void Iterator_Of_Face_Of_PolygonMesh<MeshAttribute>::SetToBegin() const
{
    for (int_max k = 0; k < m_Mesh.m_MeshData->FaceList.GetLength(); ++k)
    {
        if (m_Mesh.m_MeshData->FaceList[k].IsValid() == true)
        {
            m_FaceIndex = k;
            return;
        }
    }
    m_FaceIndex = -1;
}

}// namespace mdk
