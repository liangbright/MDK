#ifndef __mdkSurfaceMeshItemIterator_h
#define __mdkSurfaceMeshItemIterator_h

#include "mdkSurfaceMeshItem.h"

namespace mdk
{

//================================================= Iterator_Of_Point_Of_SurfaceMesh ==================================//

template<typename MeshAttribute>
struct Iterator_Of_Point_Of_SurfaceMesh
{
private:
    mutable SurfaceMesh<MeshAttribute> m_Mesh;
    mutable Handle_Of_Point_Of_SurfaceMesh m_PointHandle;

//----------------------------------
private:
    Iterator_Of_Point_Of_SurfaceMesh() = delete;
public:
    inline Iterator_Of_Point_Of_SurfaceMesh(const SurfaceMesh<MeshAttribute>& ParentMesh);
    inline Iterator_Of_Point_Of_SurfaceMesh(const Iterator_Of_Point_Of_SurfaceMesh& InputIterator);
    inline ~Iterator_Of_Point_Of_SurfaceMesh();

    inline void operator=(const Iterator_Of_Point_Of_SurfaceMesh& InputIterator) const;

    inline Handle_Of_Point_Of_SurfaceMesh GetPointHandle() const;

    inline int_max GetPointID() const;

    inline Point_Of_SurfaceMesh<MeshAttribute>& Point();
    inline const Point_Of_SurfaceMesh<MeshAttribute>& Point() const;

    inline void operator+=(int_max Offset) const;
    inline void operator-=(int_max Offset) const;
    inline void operator++() const;
    inline void operator--() const;

    inline void SetToBegin() const;

    inline bool IsNotEnd() const;
};

//================================================= Iterator_Of_Edge_Of_SurfaceMesh ==================================//

template<typename MeshAttribute>
struct Iterator_Of_Edge_Of_SurfaceMesh
{
private:
    mutable SurfaceMesh<MeshAttribute> m_Mesh;
    mutable Handle_Of_Edge_Of_SurfaceMesh m_EdgeHandle;

    //----------------------------------
private:
    Iterator_Of_Edge_Of_SurfaceMesh() = delete;
public:
    inline Iterator_Of_Edge_Of_SurfaceMesh(const SurfaceMesh<MeshAttribute>& ParentMesh);
    inline Iterator_Of_Edge_Of_SurfaceMesh(const Iterator_Of_Edge_Of_SurfaceMesh& InputIterator);
    inline ~Iterator_Of_Edge_Of_SurfaceMesh();

    inline void operator=(const Iterator_Of_Edge_Of_SurfaceMesh& InputIterator) const;

    inline Handle_Of_Edge_Of_SurfaceMesh GetEdgeHandle() const;

    inline int_max GetEdgeID() const;

    inline Edge_Of_SurfaceMesh<MeshAttribute>& Edge();
    inline const Edge_Of_SurfaceMesh<MeshAttribute>& Edge() const;

    inline void operator+=(int_max Offset) const;
    inline void operator-=(int_max Offset) const;
    inline void operator++() const;
    inline void operator--() const;

    inline void SetToBegin() const;

    inline bool IsNotEnd() const;
};

//================================================= Iterator_Of_DirectedEdge_Of_SurfaceMesh ==================================//

template<typename MeshAttribute>
struct Iterator_Of_DirectedEdge_Of_SurfaceMesh
{
private:
    mutable SurfaceMesh<MeshAttribute> m_Mesh;
    mutable Handle_Of_DirectedEdge_Of_SurfaceMesh m_DirectedEdgeHandle;

    //----------------------------------
private:
    Iterator_Of_DirectedEdge_Of_SurfaceMesh() = delete;
public:
    inline Iterator_Of_DirectedEdge_Of_SurfaceMesh(const SurfaceMesh<MeshAttribute>& ParentMesh);
    inline Iterator_Of_DirectedEdge_Of_SurfaceMesh(const Iterator_Of_DirectedEdge_Of_SurfaceMesh& InputIterator);
    inline ~Iterator_Of_DirectedEdge_Of_SurfaceMesh();

    inline void operator=(const Iterator_Of_DirectedEdge_Of_SurfaceMesh& InputIterator) const;

    inline Handle_Of_DirectedEdge_Of_SurfaceMesh GetDirectedEdgeHandle() const;

    inline int_max GetDirectedEdgeID() const;

    inline DirectedEdge_Of_SurfaceMesh<MeshAttribute>& DirectedEdge();
    inline const DirectedEdge_Of_SurfaceMesh<MeshAttribute>& DirectedEdge() const;

    inline void operator+=(int_max Offset) const;
    inline void operator-=(int_max Offset) const;
    inline void operator++() const;
    inline void operator--() const;

    inline void SetToBegin() const;

    inline bool IsNotEnd() const;
};

//================================================= Iterator_Of_Cell_Of_SurfaceMesh ==================================//

template<typename MeshAttribute>
struct Iterator_Of_Cell_Of_SurfaceMesh
{
private:
    mutable SurfaceMesh<MeshAttribute> m_Mesh;
    mutable Handle_Of_Cell_Of_SurfaceMesh m_CellHandle;

    //----------------------------------
private:
    Iterator_Of_Cell_Of_SurfaceMesh() = delete;
public:
    inline Iterator_Of_Cell_Of_SurfaceMesh(const SurfaceMesh<MeshAttribute>& ParentMesh);
    inline Iterator_Of_Cell_Of_SurfaceMesh(const Iterator_Of_Cell_Of_SurfaceMesh& InputIterator);
    inline ~Iterator_Of_Cell_Of_SurfaceMesh();

    inline void operator=(const Iterator_Of_Cell_Of_SurfaceMesh& InputIterator) const;

    inline Handle_Of_Cell_Of_SurfaceMesh GetCellHandle() const;

    inline int_max GetCellID() const;

    inline Cell_Of_SurfaceMesh<MeshAttribute>& Cell();
    inline const Cell_Of_SurfaceMesh<MeshAttribute>& Cell() const;

    inline void operator+=(int_max Offset) const;
    inline void operator-=(int_max Offset) const;
    inline void operator++() const;
    inline void operator--() const;

    inline void SetToBegin() const;

    inline bool IsNotEnd() const;
};

}// namespace mdk

#include "mdkSurfaceMeshItemIterator.hpp"

#endif
