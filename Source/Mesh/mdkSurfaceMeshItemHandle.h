#ifndef __mdkSurfaceMeshItemHandle_h
#define __mdkSurfaceMeshItemHandle_h

#include "mdkType.h"

namespace mdk
{
//====================================== Handle_Of_Point_Of_SurfaceMesh ==============================================================//

struct Handle_Of_Point_Of_SurfaceMesh
{
private:
    int_max m_Index; // PointIndex in SurfaceMesh::m_MeshData->PointList

public:
    inline  Handle_Of_Point_Of_SurfaceMesh();
    inline  Handle_Of_Point_Of_SurfaceMesh(const Handle_Of_Point_Of_SurfaceMesh& InputHandle);
    inline  ~Handle_Of_Point_Of_SurfaceMesh();

    inline void operator=(const Handle_Of_Point_Of_SurfaceMesh& InputHandle);

    inline void SetIndex(int_max PointIndex);
    inline int_max GetIndex() const;

    inline void SetToInvalid();

    inline bool operator==(const Handle_Of_Point_Of_SurfaceMesh& InputHandle) const;
    inline bool operator!=(const Handle_Of_Point_Of_SurfaceMesh& InputHandle) const;
};

//====================================== Handle_Of_Edge_Of_SurfaceMesh ==============================================================//

struct Handle_Of_Edge_Of_SurfaceMesh
{
private:
    int_max m_Index; // EdgeIndex in SurfaceMesh::m_MeshData->EdgeList

public:
    inline Handle_Of_Edge_Of_SurfaceMesh();
    inline Handle_Of_Edge_Of_SurfaceMesh(const Handle_Of_Edge_Of_SurfaceMesh& InputHandle);
    inline ~Handle_Of_Edge_Of_SurfaceMesh();

    inline void operator=(const Handle_Of_Edge_Of_SurfaceMesh& InputHandle);

    inline void SetIndex(int_max EdgeIndex);
    inline int_max GetIndex() const;

    inline void SetToInvalid();

    inline bool operator==(const Handle_Of_Edge_Of_SurfaceMesh& InputHandle)  const;
    inline bool operator!=(const Handle_Of_Edge_Of_SurfaceMesh& InputHandle)  const;
};

//====================================== DirectedEdge_Of_SurfaceMesh (Cell Plasma Membrane) ===================================================//

struct DirectedEdgeIndex_Of_SurfaceMesh
{
    int_max EdgeIndex = -1;
    int_max RelativeIndex = -1; // 0 or 1
//------------------------------------------
    DirectedEdgeIndex_Of_SurfaceMesh() {}
    DirectedEdgeIndex_Of_SurfaceMesh(const DirectedEdgeIndex_Of_SurfaceMesh& InputIndex)
    {
        (*this) = InputIndex;
    }
    ~DirectedEdgeIndex_Of_SurfaceMesh() {}
    void operator=(const DirectedEdgeIndex_Of_SurfaceMesh& InputIndex)
    {
        EdgeIndex = InputIndex.EdgeIndex;
        RelativeIndex = InputIndex.RelativeIndex;
    }

    bool operator==(const DirectedEdgeIndex_Of_SurfaceMesh& InputHandle) const
    {
        return (EdgeIndex == InputHandle.EdgeIndex) && (RelativeIndex == InputHandle.RelativeIndex);
    }

    bool operator!=(const DirectedEdgeIndex_Of_SurfaceMesh& InputHandle) const
    {
        return (EdgeIndex != InputHandle.EdgeIndex) || (RelativeIndex == InputHandle.RelativeIndex);
    }
};

struct Handle_Of_DirectedEdge_Of_SurfaceMesh
{
private:
    DirectedEdgeIndex_Of_SurfaceMesh m_Index;  // DirectedEdgeIndex in SurfaceMesh::m_MeshData->DirectedEdgePairList

public:
    inline Handle_Of_DirectedEdge_Of_SurfaceMesh();
    inline Handle_Of_DirectedEdge_Of_SurfaceMesh(const Handle_Of_DirectedEdge_Of_SurfaceMesh& InputHandle);
    inline ~Handle_Of_DirectedEdge_Of_SurfaceMesh();

    inline void operator=(const Handle_Of_DirectedEdge_Of_SurfaceMesh& InputHandle);

    inline void SetIndex(DirectedEdgeIndex_Of_SurfaceMesh DirectedEdgeIndex);
    inline void SetIndex(int_max EdgeIndex, int_max RelativeIndex);

    inline DirectedEdgeIndex_Of_SurfaceMesh GetIndex() const;
    inline int_max GetEdgeIndex() const;
    inline int_max GetRelativeIndex() const;

    inline void SetToInvalid();

    inline bool operator==(const Handle_Of_DirectedEdge_Of_SurfaceMesh& InputHandle)  const;
    inline bool operator!=(const Handle_Of_DirectedEdge_Of_SurfaceMesh& InputHandle)  const;
};

//====================================== Cell_Of_SurfaceMesh ==============================================================//

struct Handle_Of_Cell_Of_SurfaceMesh
{
private:
    int_max m_Index;   // CellIndex in SurfaceMesh::m_MeshData->CellList

public:
    inline Handle_Of_Cell_Of_SurfaceMesh();
    inline Handle_Of_Cell_Of_SurfaceMesh(const Handle_Of_Cell_Of_SurfaceMesh& InputHandle);
    inline ~Handle_Of_Cell_Of_SurfaceMesh();

    inline void operator=(const Handle_Of_Cell_Of_SurfaceMesh& InputHandle);

    inline void SetIndex(int_max CellIndex);
    inline int_max GetIndex() const;

    inline void SetToInvalid();

    inline bool operator==(const Handle_Of_Cell_Of_SurfaceMesh& InputHandle) const;
    inline bool operator!=(const Handle_Of_Cell_Of_SurfaceMesh& InputHandle) const;

};

//================================================ Implementation ============================================================//

//====================================== Handle_Of_Point_Of_SurfaceMesh ==============================================================//

inline Handle_Of_Point_Of_SurfaceMesh::Handle_Of_Point_Of_SurfaceMesh()
{
    this->SetToInvalid();
}

inline Handle_Of_Point_Of_SurfaceMesh::Handle_Of_Point_Of_SurfaceMesh(const Handle_Of_Point_Of_SurfaceMesh& InputHandle)
{
    m_Index = InputHandle.m_Index;
}

inline Handle_Of_Point_Of_SurfaceMesh::~Handle_Of_Point_Of_SurfaceMesh()
{
}

inline void Handle_Of_Point_Of_SurfaceMesh::operator=(const Handle_Of_Point_Of_SurfaceMesh& InputHandle)
{
    m_Index = InputHandle.m_Index;
}

inline void Handle_Of_Point_Of_SurfaceMesh::SetIndex(int_max PointIndex)
{
    m_Index = PointIndex;
}

inline int_max Handle_Of_Point_Of_SurfaceMesh::GetIndex() const
{
    return m_Index;
}

inline void Handle_Of_Point_Of_SurfaceMesh::SetToInvalid()
{
    m_Index = -1;
}

inline bool Handle_Of_Point_Of_SurfaceMesh::operator==(const Handle_Of_Point_Of_SurfaceMesh& InputHandle) const
{
    return m_Index == InputHandle.m_Index;
}

inline bool Handle_Of_Point_Of_SurfaceMesh::operator!=(const Handle_Of_Point_Of_SurfaceMesh& InputHandle) const
{
    return m_Index != InputHandle.m_Index;
}

//====================================== Handle_Of_Edge_Of_SurfaceMesh ==============================================================//

inline Handle_Of_Edge_Of_SurfaceMesh::Handle_Of_Edge_Of_SurfaceMesh()
{
    this->SetToInvalid();
}

inline Handle_Of_Edge_Of_SurfaceMesh::Handle_Of_Edge_Of_SurfaceMesh(const Handle_Of_Edge_Of_SurfaceMesh& InputHandle)
{
    m_Index = InputHandle.m_Index;
}

inline Handle_Of_Edge_Of_SurfaceMesh::~Handle_Of_Edge_Of_SurfaceMesh()
{
}

inline void Handle_Of_Edge_Of_SurfaceMesh::operator=(const Handle_Of_Edge_Of_SurfaceMesh& InputHandle)
{
    m_Index = InputHandle.m_Index;
}

inline void Handle_Of_Edge_Of_SurfaceMesh::SetIndex(int_max EdgeIndex)
{
    m_Index = EdgeIndex;
}

inline int_max Handle_Of_Edge_Of_SurfaceMesh::GetIndex() const
{
    return m_Index;
}

inline void Handle_Of_Edge_Of_SurfaceMesh::SetToInvalid()
{
    m_Index = -1;
}

inline bool Handle_Of_Edge_Of_SurfaceMesh::operator==(const Handle_Of_Edge_Of_SurfaceMesh& InputHandle) const
{
    return m_Index == InputHandle.m_Index;
}

inline bool Handle_Of_Edge_Of_SurfaceMesh::operator!=(const Handle_Of_Edge_Of_SurfaceMesh& InputHandle) const
{
    return m_Index != InputHandle.m_Index;
}

//====================================== DirectedEdge_Of_SurfaceMesh (Cell Plasma Membrane) ===================================================//

inline Handle_Of_DirectedEdge_Of_SurfaceMesh::Handle_Of_DirectedEdge_Of_SurfaceMesh()
{
    this->SetToInvalid();
}

inline Handle_Of_DirectedEdge_Of_SurfaceMesh::Handle_Of_DirectedEdge_Of_SurfaceMesh(const Handle_Of_DirectedEdge_Of_SurfaceMesh& InputHandle)
{
    m_Index.EdgeIndex = InputHandle.m_Index.EdgeIndex;
    m_Index.RelativeIndex = InputHandle.m_Index.RelativeIndex;
}

inline Handle_Of_DirectedEdge_Of_SurfaceMesh::~Handle_Of_DirectedEdge_Of_SurfaceMesh()
{
}

inline void Handle_Of_DirectedEdge_Of_SurfaceMesh::operator=(const Handle_Of_DirectedEdge_Of_SurfaceMesh& InputHandle)
{
    m_Index.EdgeIndex = InputHandle.m_Index.EdgeIndex;
    m_Index.RelativeIndex = InputHandle.m_Index.RelativeIndex;
}

inline void Handle_Of_DirectedEdge_Of_SurfaceMesh::SetIndex(DirectedEdgeIndex_Of_SurfaceMesh DirectedEdgeIndex)
{
    m_Index.EdgeIndex = DirectedEdgeIndex.EdgeIndex;
    m_Index.RelativeIndex = DirectedEdgeIndex.RelativeIndex;
}

inline void Handle_Of_DirectedEdge_Of_SurfaceMesh::SetIndex(int_max EdgeIndex, int_max RelativeIndex)
{
    m_Index.EdgeIndex = EdgeIndex;
    m_Index.RelativeIndex = RelativeIndex;
}

inline void Handle_Of_DirectedEdge_Of_SurfaceMesh::SetToInvalid()
{
    m_Index.EdgeIndex = -1;
    m_Index.RelativeIndex = -1;
}

inline DirectedEdgeIndex_Of_SurfaceMesh Handle_Of_DirectedEdge_Of_SurfaceMesh::GetIndex() const
{
    return m_Index;
}

inline int_max Handle_Of_DirectedEdge_Of_SurfaceMesh::GetEdgeIndex() const
{
    return m_Index.EdgeIndex;
}

inline int_max Handle_Of_DirectedEdge_Of_SurfaceMesh::GetRelativeIndex() const
{
    return m_Index.RelativeIndex;
}

inline bool Handle_Of_DirectedEdge_Of_SurfaceMesh::operator==(const Handle_Of_DirectedEdge_Of_SurfaceMesh& InputHandle) const
{
    return m_Index.EdgeIndex == InputHandle.m_Index.EdgeIndex && m_Index.RelativeIndex == InputHandle.m_Index.RelativeIndex;
}

inline bool Handle_Of_DirectedEdge_Of_SurfaceMesh::operator!=(const Handle_Of_DirectedEdge_Of_SurfaceMesh& InputHandle) const
{
    return m_Index.EdgeIndex != InputHandle.m_Index.EdgeIndex || m_Index.RelativeIndex != InputHandle.m_Index.RelativeIndex;
}

//====================================== Cell_Of_SurfaceMesh ==============================================================//

inline Handle_Of_Cell_Of_SurfaceMesh::Handle_Of_Cell_Of_SurfaceMesh()
{
    this->SetToInvalid();
}

inline Handle_Of_Cell_Of_SurfaceMesh::Handle_Of_Cell_Of_SurfaceMesh(const Handle_Of_Cell_Of_SurfaceMesh& InputHandle)
{
    m_Index = InputHandle.m_Index;
}

inline Handle_Of_Cell_Of_SurfaceMesh::~Handle_Of_Cell_Of_SurfaceMesh()
{
}

inline void Handle_Of_Cell_Of_SurfaceMesh::operator=(const Handle_Of_Cell_Of_SurfaceMesh& InputHandle)
{
    m_Index = InputHandle.m_Index;
}

inline void Handle_Of_Cell_Of_SurfaceMesh::SetIndex(int_max CellIndex)
{
    m_Index = CellIndex;
}

inline int_max Handle_Of_Cell_Of_SurfaceMesh::GetIndex() const
{
    return m_Index;
}

inline void Handle_Of_Cell_Of_SurfaceMesh::SetToInvalid()
{
    m_Index = -1;
}

inline bool Handle_Of_Cell_Of_SurfaceMesh::operator==(const Handle_Of_Cell_Of_SurfaceMesh& InputHandle) const
{
    return m_Index == InputHandle.m_Index;
}

inline bool Handle_Of_Cell_Of_SurfaceMesh::operator!=(const Handle_Of_Cell_Of_SurfaceMesh& InputHandle) const
{
    return m_Index != InputHandle.m_Index;
}


}// namespace mdk

#endif