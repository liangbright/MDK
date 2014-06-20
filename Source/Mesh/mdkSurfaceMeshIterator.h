#ifndef __mdkSurfaceMeshIterator_h
#define __mdkSurfaceMeshIterator_h

#include "mdkSurfaceMeshItem.h"

namespace mdk
{

//================================================= Iterator_Of_Point_Of_SurfaceMesh ==================================//

template<typename MeshType>
struct Iterator_Of_Point_Of_SurfaceMesh
{
private:
    SurfaceMesh<MeshType> m_Mesh;
    Handle_Of_Point_Of_SurfaceMesh m_PointHandle;

//----------------------------------
public:
    Iterator_Of_Point_Of_SurfaceMesh(SurfaceMesh<MeshType>& ParentMesh) 
    {
        m_Mesh.Share(ParentMesh);
        this->SetToBegin();
    }

    ~Iterator_Of_Point_Of_SurfaceMesh() {}

    Handle_Of_Point_Of_SurfaceMesh GetPointHandle() const
    {
        return m_PointHandle;
    }

    void operator+=(int_max Offset)
    {
        if (Offset == 0)
        {
            return;
        }
        else if (Offset > 0)
        {
            auto PointIndex = m_PointHandle.GetIndex();
            if (PointIndex + Offset < m_Mesh.m_MeshData->PointList.GetLength())
            {
                int_max Counter = 0;
                int_max PointIndex_next = -1;
                for (int_max k = PointIndex + 1; k < m_Mesh.m_MeshData->PointList.GetLength(); ++k)
                {
                    if (m_Mesh.m_MeshData->PointList[k].IsValid() == true)
                    {
                        Counter += 1;    
                    }

                    if (Counter == Offset)
                    {
                        PointIndex_next = k;
                        break;
                    }
                }

                m_PointHandle.SetIndex(PointIndex_next);
            }
            else
            {
                m_PointHandle.SetIndex(-1);
            }
        }
        else
        {
            auto PointIndex = m_PointHandle.GetIndex();
            if (PointIndex + Offset >= 0)
            {
                int_max Counter = 0;
                int_max PointIndex_next = -1;
                for (int_max k = PointIndex-1; k >= 0; --k)
                {
                    if (m_Mesh.m_MeshData->PointList[k].IsValid() == true)
                    {
                        Counter += 1;
                    }

                    if ((Counter + Offset) == 0)
                    {
                        PointIndex_next = k;
                        break;
                    }
                }

                m_PointHandle.SetIndex(PointIndex_next);
            }
            else
            {
                m_PointHandle.SetIndex(-1);
            }
        }
    }

    void operator-=(int_max Offset)
    {
        this->operator+=(-Offset);
    }

    void operator++()
    {
        (*this) += 1;
    }

    void operator--()
    {
        (*this) -= 1;
    }

    bool IsInRange() const
    {
        auto PointIndex = m_PointHandle.GetIndex();
        return (PointIndex >= 0 && PointIndex < m_Mesh.m_MeshData->PointList.GetLength());
    }

    void SetToBegin()
    {
        int_max PointIndex_begin = -1;
        for (int_max k = 0; k < m_Mesh.m_MeshData->PointList.GetLength(); ++k)
        {
            if (m_Mesh.m_MeshData->PointList[k].IsValid() == true)
            {
                PointIndex_begin = k;
                break;
            }
        }

        m_PointHandle.SetIndex(PointIndex_begin);
    }
};

//================================================= Iterator_Of_Edge_Of_SurfaceMesh ==================================//

template<typename MeshType>
struct Iterator_Of_Edge_Of_SurfaceMesh
{
private:
    SurfaceMesh<MeshType> m_Mesh;
    Handle_Of_Edge_Of_SurfaceMesh m_EdgeHandle;

    //----------------------------------
public:
    Iterator_Of_Edge_Of_SurfaceMesh(SurfaceMesh<MeshType>& ParentMesh)
    {
        m_Mesh.Share(ParentMesh);
        this->SetToBegin();
    }

    ~Iterator_Of_Edge_Of_SurfaceMesh() {}

    Handle_Of_Edge_Of_SurfaceMesh GetEdgeHandle() const
    {
        return m_EdgeHandle;
    }

    void operator+=(int_max Offset)
    {
        if (Offset == 0)
        {
            return;
        }
        else if (Offset > 0)
        {
            auto EdgeIndex = m_EdgeHandle.GetIndex();
            if (EdgeIndex + Offset < m_Mesh.m_MeshData->EdgeList.GetLength())
            {
                int_max Counter = 0;
                int_max EdgeIndex_next = -1;
                for (int_max k = EdgeIndex + 1; k < m_Mesh.m_MeshData->EdgeList.GetLength(); ++k)
                {
                    if (m_Mesh.m_MeshData->EdgeList[k].IsValid() == true)
                    {
                        Counter += 1;
                    }

                    if (Counter == Offset)
                    {
                        EdgeIndex_next = k;
                        break;
                    }
                }

                m_EdgeHandle.SetIndex(EdgeIndex_next);
            }
            else
            {
                m_EdgeHandle.SetIndex(-1);
            }
        }
        else
        {
            auto EdgeIndex = m_EdgeHandle.GetIndex();
            if (EdgeIndex + Offset >= 0)
            {
                int_max Counter = 0;
                int_max EdgeIndex_next = -1;
                for (int_max k = EdgeIndex - 1; k >= 0; --k)
                {
                    if (m_Mesh.m_MeshData->EdgeList[k].IsValid() == true)
                    {
                        Counter += 1;
                    }

                    if ((Counter + Offset) == 0)
                    {
                        EdgeIndex_next = k;
                        break;
                    }
                }

                m_EdgeHandle.SetIndex(EdgeIndex_next);
            }
            else
            {
                m_EdgeHandle.SetIndex(-1);
            }
        }
    }

    void operator-=(int_max Offset)
    {
        this->operator+=(-Offset);
    }

    void operator++()
    {
        (*this) += 1;
    }

    void operator--()
    {
        (*this) -= 1;
    }

    bool IsInRange() const
    {
        auto EdgeIndex = m_EdgeHandle.GetIndex();
        return (EdgeIndex >= 0 && EdgeIndex < m_m_Mesh.m_MeshData->EdgeList.GetLength());
    }

    void SetToBegin()
    {
        int_max EdgeIndex_begin = -1;
        for (int_max k = 0; k < m_Mesh.m_MeshData->EdgeList.GetLength(); ++k)
        {
            if (m_Mesh.m_MeshData->EdgeList[k].IsValid() == true)
            {
                EdgeIndex_begin = k;
                break;
            }
        }

        m_EdgeHandle.SetIndex(EdgeIndex_begin);
    }
};

//================================================= Iterator_Of_Cell_Of_SurfaceMesh ==================================//

template<typename MeshType>
struct Iterator_Of_Cell_Of_SurfaceMesh
{
private:
    SurfaceMesh<MeshType> m_Mesh;
    Handle_Of_Cell_Of_SurfaceMesh m_CellHandle;

    //----------------------------------
public:
    Iterator_Of_Cell_Of_SurfaceMesh(SurfaceMesh<MeshType>& ParentMesh)
    {
        m_Mesh.Share(ParentMesh);
        this->SetToBegin();
    }

    ~Iterator_Of_Cell_Of_SurfaceMesh() {}

    Handle_Of_Cell_Of_SurfaceMesh GetCellHandle() const
    {
        return m_CellHandle;
    }

    void operator+=(int_max Offset)
    {
        if (Offset == 0)
        {
            return;
        }
        else if (Offset > 0)
        {
            auto CellIndex = m_CellHandle.GetIndex();
            if (CellIndex + Offset < m_Mesh.m_MeshData->CellList.GetLength())
            {
                int_max Counter = 0;
                int_max CellIndex_next = -1;
                for (int_max k = CellIndex + 1; k < m_Mesh.m_MeshData->CellList.GetLength(); ++k)
                {
                    if (m_Mesh.m_MeshData->CellList[k].IsValid() == true)
                    {
                        Counter += 1;
                    }

                    if (Counter == Offset)
                    {
                        CellIndex_next = k;
                        break;
                    }
                }

                m_CellHandle.SetIndex(CellIndex_next);
            }
            else
            {
                m_CellHandle.SetIndex(-1);
            }
        }
        else
        {
            auto CellIndex = m_CellHandle.GetIndex();
            if (CellIndex + Offset >= 0)
            {
                int_max Counter = 0;
                int_max CellIndex_next = -1;
                for (int_max k = CellIndex - 1; k >= 0; --k)
                {
                    if (m_Mesh.m_MeshData->CellList[k].IsValid() == true)
                    {
                        Counter += 1;
                    }

                    if ((Counter + Offset) == 0)
                    {
                        CellIndex_next = k;
                        break;
                    }
                }

                m_CellHandle.SetIndex(CellIndex_next);
            }
            else
            {
                m_CellHandle.SetIndex(-1);
            }
        }
    }

    void operator-=(int_max Offset)
    {
        this->operator+=(-Offset);
    }

    void operator++()
    {
        (*this) += 1;
    }

    void operator--()
    {
        (*this) -= 1;
    }

    bool IsInRange() const
    {
        auto CellIndex = m_CellHandle.GetIndex();
        return (CellIndex >= 0 && CellIndex < m_Mesh.m_MeshData->CellList.GetLength());
    }

    void SetToBegin()
    {
        int_max CellIndex_begin = -1;
        for (int_max k = 0; k < m_Mesh.m_MeshData->CellList.GetLength(); ++k)
        {
            if (m_Mesh.m_MeshData->CellList[k].IsValid() == true)
            {
                CellIndex_begin = k;
                break;
            }
        }

        m_CellHandle.SetIndex(CellIndex_begin);
    }
};

}// namespace mdk

#include "mdkSurfaceMeshIterator.hpp"

#endif
