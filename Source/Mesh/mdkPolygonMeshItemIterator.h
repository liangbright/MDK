#pragma once

#include "mdkPolygonMeshItem.h"

namespace mdk
{

//================================================= Iterator_Of_Point_Of_PolygonMesh ==================================//

template<typename MeshAttribute>
struct Iterator_Of_Point_Of_PolygonMesh
{
private:
    mutable PolygonMesh<MeshAttribute> m_Mesh;
    mutable Handle_Of_Point_Of_PolygonMesh m_PointHandle;

//----------------------------------
private:
    Iterator_Of_Point_Of_PolygonMesh() = delete;
public:
    inline Iterator_Of_Point_Of_PolygonMesh(const PolygonMesh<MeshAttribute>& ParentMesh);
    inline Iterator_Of_Point_Of_PolygonMesh(const Iterator_Of_Point_Of_PolygonMesh& InputIterator);
    inline ~Iterator_Of_Point_Of_PolygonMesh();

    inline void operator=(const Iterator_Of_Point_Of_PolygonMesh& InputIterator) const;

    inline Handle_Of_Point_Of_PolygonMesh GetPointHandle() const;

    inline int_max GetPointID() const;

    inline Point_Of_PolygonMesh<MeshAttribute>& Point();
    inline const Point_Of_PolygonMesh<MeshAttribute>& Point() const;

    inline void operator+=(int_max Offset) const;
    inline void operator-=(int_max Offset) const;
    inline void operator++() const;
    inline void operator--() const;

    inline void SetToBegin() const;

    inline bool IsNotEnd() const;
};

//================================================= Iterator_Of_Edge_Of_PolygonMesh ==================================//

template<typename MeshAttribute>
struct Iterator_Of_Edge_Of_PolygonMesh
{
private:
    mutable PolygonMesh<MeshAttribute> m_Mesh;
    mutable Handle_Of_Edge_Of_PolygonMesh m_EdgeHandle;

    //----------------------------------
private:
    Iterator_Of_Edge_Of_PolygonMesh() = delete;
public:
    inline Iterator_Of_Edge_Of_PolygonMesh(const PolygonMesh<MeshAttribute>& ParentMesh);
    inline Iterator_Of_Edge_Of_PolygonMesh(const Iterator_Of_Edge_Of_PolygonMesh& InputIterator);
    inline ~Iterator_Of_Edge_Of_PolygonMesh();

    inline void operator=(const Iterator_Of_Edge_Of_PolygonMesh& InputIterator) const;

    inline Handle_Of_Edge_Of_PolygonMesh GetEdgeHandle() const;

    inline int_max GetEdgeID() const;

    inline Edge_Of_PolygonMesh<MeshAttribute>& Edge();
    inline const Edge_Of_PolygonMesh<MeshAttribute>& Edge() const;

    inline void operator+=(int_max Offset) const;
    inline void operator-=(int_max Offset) const;
    inline void operator++() const;
    inline void operator--() const;

    inline void SetToBegin() const;

    inline bool IsNotEnd() const;
};

//================================================= Iterator_Of_DirectedEdge_Of_PolygonMesh ==================================//

template<typename MeshAttribute>
struct Iterator_Of_DirectedEdge_Of_PolygonMesh
{
private:
    mutable PolygonMesh<MeshAttribute> m_Mesh;
    mutable Handle_Of_DirectedEdge_Of_PolygonMesh m_DirectedEdgeHandle;

    //----------------------------------
private:
    Iterator_Of_DirectedEdge_Of_PolygonMesh() = delete;
public:
    inline Iterator_Of_DirectedEdge_Of_PolygonMesh(const PolygonMesh<MeshAttribute>& ParentMesh);
    inline Iterator_Of_DirectedEdge_Of_PolygonMesh(const Iterator_Of_DirectedEdge_Of_PolygonMesh& InputIterator);
    inline ~Iterator_Of_DirectedEdge_Of_PolygonMesh();

    inline void operator=(const Iterator_Of_DirectedEdge_Of_PolygonMesh& InputIterator) const;

    inline Handle_Of_DirectedEdge_Of_PolygonMesh GetDirectedEdgeHandle() const;

    inline int_max GetDirectedEdgeID() const;

    inline DirectedEdge_Of_PolygonMesh<MeshAttribute>& DirectedEdge();
    inline const DirectedEdge_Of_PolygonMesh<MeshAttribute>& DirectedEdge() const;

    inline void operator+=(int_max Offset) const;
    inline void operator-=(int_max Offset) const;
    inline void operator++() const;
    inline void operator--() const;

    inline void SetToBegin() const;

    inline bool IsNotEnd() const;
};

//================================================= Iterator_Of_Face_Of_PolygonMesh ==================================//

template<typename MeshAttribute>
struct Iterator_Of_Face_Of_PolygonMesh
{
private:
    mutable PolygonMesh<MeshAttribute> m_Mesh;
    mutable Handle_Of_Face_Of_PolygonMesh m_FaceHandle;

    //----------------------------------
private:
    Iterator_Of_Face_Of_PolygonMesh() = delete;
public:
    inline Iterator_Of_Face_Of_PolygonMesh(const PolygonMesh<MeshAttribute>& ParentMesh);
    inline Iterator_Of_Face_Of_PolygonMesh(const Iterator_Of_Face_Of_PolygonMesh& InputIterator);
    inline ~Iterator_Of_Face_Of_PolygonMesh();

    inline void operator=(const Iterator_Of_Face_Of_PolygonMesh& InputIterator) const;

    inline Handle_Of_Face_Of_PolygonMesh GetFaceHandle() const;

    inline int_max GetFaceID() const;

    inline Face_Of_PolygonMesh<MeshAttribute>& Face();
    inline const Face_Of_PolygonMesh<MeshAttribute>& Face() const;

    inline void operator+=(int_max Offset) const;
    inline void operator-=(int_max Offset) const;
    inline void operator++() const;
    inline void operator--() const;

    inline void SetToBegin() const;

    inline bool IsNotEnd() const;
};

}// namespace mdk

#include "mdkPolygonMeshItemIterator.hpp"

