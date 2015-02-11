#ifndef mdk_MembraneMeshItemIterator_h
#define mdk_MembraneMeshItemIterator_h

#include "mdkMembraneMeshItem.h"

namespace mdk
{

//================================================= Iterator_Of_Point_Of_MembraneMesh ==================================//

template<typename MeshAttribute>
struct Iterator_Of_Point_Of_MembraneMesh
{
private:
    mutable MembraneMesh<MeshAttribute> m_Mesh;
    mutable Handle_Of_Point_Of_MembraneMesh m_PointHandle;

//----------------------------------
private:
    Iterator_Of_Point_Of_MembraneMesh() = delete;
public:
    inline Iterator_Of_Point_Of_MembraneMesh(const MembraneMesh<MeshAttribute>& ParentMesh);
    inline Iterator_Of_Point_Of_MembraneMesh(const Iterator_Of_Point_Of_MembraneMesh& InputIterator);
    inline ~Iterator_Of_Point_Of_MembraneMesh();

    inline void operator=(const Iterator_Of_Point_Of_MembraneMesh& InputIterator) const;

    inline Handle_Of_Point_Of_MembraneMesh GetPointHandle() const;

    inline int_max GetPointID() const;

    inline Point_Of_MembraneMesh<MeshAttribute>& Point();
    inline const Point_Of_MembraneMesh<MeshAttribute>& Point() const;

    inline void operator+=(int_max Offset) const;
    inline void operator-=(int_max Offset) const;
    inline void operator++() const;
    inline void operator--() const;

    inline void SetToBegin() const;

    inline bool IsNotEnd() const;
};

//================================================= Iterator_Of_Edge_Of_MembraneMesh ==================================//

template<typename MeshAttribute>
struct Iterator_Of_Edge_Of_MembraneMesh
{
private:
    mutable MembraneMesh<MeshAttribute> m_Mesh;
    mutable Handle_Of_Edge_Of_MembraneMesh m_EdgeHandle;

    //----------------------------------
private:
    Iterator_Of_Edge_Of_MembraneMesh() = delete;
public:
    inline Iterator_Of_Edge_Of_MembraneMesh(const MembraneMesh<MeshAttribute>& ParentMesh);
    inline Iterator_Of_Edge_Of_MembraneMesh(const Iterator_Of_Edge_Of_MembraneMesh& InputIterator);
    inline ~Iterator_Of_Edge_Of_MembraneMesh();

    inline void operator=(const Iterator_Of_Edge_Of_MembraneMesh& InputIterator) const;

    inline Handle_Of_Edge_Of_MembraneMesh GetEdgeHandle() const;

    inline int_max GetEdgeID() const;

    inline Edge_Of_MembraneMesh<MeshAttribute>& Edge();
    inline const Edge_Of_MembraneMesh<MeshAttribute>& Edge() const;

    inline void operator+=(int_max Offset) const;
    inline void operator-=(int_max Offset) const;
    inline void operator++() const;
    inline void operator--() const;

    inline void SetToBegin() const;

    inline bool IsNotEnd() const;
};

//================================================= Iterator_Of_DirectedEdge_Of_MembraneMesh ==================================//

template<typename MeshAttribute>
struct Iterator_Of_DirectedEdge_Of_MembraneMesh
{
private:
    mutable MembraneMesh<MeshAttribute> m_Mesh;
    mutable Handle_Of_DirectedEdge_Of_MembraneMesh m_DirectedEdgeHandle;

    //----------------------------------
private:
    Iterator_Of_DirectedEdge_Of_MembraneMesh() = delete;
public:
    inline Iterator_Of_DirectedEdge_Of_MembraneMesh(const MembraneMesh<MeshAttribute>& ParentMesh);
    inline Iterator_Of_DirectedEdge_Of_MembraneMesh(const Iterator_Of_DirectedEdge_Of_MembraneMesh& InputIterator);
    inline ~Iterator_Of_DirectedEdge_Of_MembraneMesh();

    inline void operator=(const Iterator_Of_DirectedEdge_Of_MembraneMesh& InputIterator) const;

    inline Handle_Of_DirectedEdge_Of_MembraneMesh GetDirectedEdgeHandle() const;

    inline int_max GetDirectedEdgeID() const;

    inline DirectedEdge_Of_MembraneMesh<MeshAttribute>& DirectedEdge();
    inline const DirectedEdge_Of_MembraneMesh<MeshAttribute>& DirectedEdge() const;

    inline void operator+=(int_max Offset) const;
    inline void operator-=(int_max Offset) const;
    inline void operator++() const;
    inline void operator--() const;

    inline void SetToBegin() const;

    inline bool IsNotEnd() const;
};

//================================================= Iterator_Of_Cell_Of_MembraneMesh ==================================//

template<typename MeshAttribute>
struct Iterator_Of_Cell_Of_MembraneMesh
{
private:
    mutable MembraneMesh<MeshAttribute> m_Mesh;
    mutable Handle_Of_Cell_Of_MembraneMesh m_CellHandle;

    //----------------------------------
private:
    Iterator_Of_Cell_Of_MembraneMesh() = delete;
public:
    inline Iterator_Of_Cell_Of_MembraneMesh(const MembraneMesh<MeshAttribute>& ParentMesh);
    inline Iterator_Of_Cell_Of_MembraneMesh(const Iterator_Of_Cell_Of_MembraneMesh& InputIterator);
    inline ~Iterator_Of_Cell_Of_MembraneMesh();

    inline void operator=(const Iterator_Of_Cell_Of_MembraneMesh& InputIterator) const;

    inline Handle_Of_Cell_Of_MembraneMesh GetCellHandle() const;

    inline int_max GetCellID() const;

    inline Cell_Of_MembraneMesh<MeshAttribute>& Cell();
    inline const Cell_Of_MembraneMesh<MeshAttribute>& Cell() const;

    inline void operator+=(int_max Offset) const;
    inline void operator-=(int_max Offset) const;
    inline void operator++() const;
    inline void operator--() const;

    inline void SetToBegin() const;

    inline bool IsNotEnd() const;
};

}// namespace mdk

#include "mdkMembraneMeshItemIterator.hpp"

#endif
