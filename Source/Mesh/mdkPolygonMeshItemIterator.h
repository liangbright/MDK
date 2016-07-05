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
	mutable int_max m_PointIndex;

//----------------------------------
private:
    Iterator_Of_Point_Of_PolygonMesh() = delete;
public:
    inline Iterator_Of_Point_Of_PolygonMesh(const PolygonMesh<MeshAttribute>& ParentMesh);
    inline Iterator_Of_Point_Of_PolygonMesh(const Iterator_Of_Point_Of_PolygonMesh& InputIterator);
    inline ~Iterator_Of_Point_Of_PolygonMesh();

    inline void operator=(const Iterator_Of_Point_Of_PolygonMesh& InputIterator) const;

    inline int_max GetPointIndex() const;

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
	mutable int_max m_EdgeIndex;

    //----------------------------------
private:
    Iterator_Of_Edge_Of_PolygonMesh() = delete;
public:
    inline Iterator_Of_Edge_Of_PolygonMesh(const PolygonMesh<MeshAttribute>& ParentMesh);
    inline Iterator_Of_Edge_Of_PolygonMesh(const Iterator_Of_Edge_Of_PolygonMesh& InputIterator);
    inline ~Iterator_Of_Edge_Of_PolygonMesh();

    inline void operator=(const Iterator_Of_Edge_Of_PolygonMesh& InputIterator) const;

    inline int_max GetEdgeIndex() const;

    inline Edge_Of_PolygonMesh<MeshAttribute>& Edge();
    inline const Edge_Of_PolygonMesh<MeshAttribute>& Edge() const;

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
	mutable int_max m_FaceIndex;

    //----------------------------------
private:
    Iterator_Of_Face_Of_PolygonMesh() = delete;
public:
    inline Iterator_Of_Face_Of_PolygonMesh(const PolygonMesh<MeshAttribute>& ParentMesh);
    inline Iterator_Of_Face_Of_PolygonMesh(const Iterator_Of_Face_Of_PolygonMesh& InputIterator);
    inline ~Iterator_Of_Face_Of_PolygonMesh();

    inline void operator=(const Iterator_Of_Face_Of_PolygonMesh& InputIterator) const;

    inline int_max GetFaceIndex() const;

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

