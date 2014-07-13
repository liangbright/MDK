#ifndef __mdkMembraneMeshItemHandle_h
#define __mdkMembraneMeshItemHandle_h

#include "mdkType.h"

namespace mdk
{
//====================================== Handle_Of_Point_Of_MembraneMesh ==============================================================//

struct Handle_Of_Point_Of_MembraneMesh
{
private:
    int_max m_Index; // PointIndex in MembraneMesh::m_MeshData->PointList

public:
    inline  Handle_Of_Point_Of_MembraneMesh();
    inline  Handle_Of_Point_Of_MembraneMesh(const Handle_Of_Point_Of_MembraneMesh& InputHandle);    
    inline  ~Handle_Of_Point_Of_MembraneMesh();

    inline void operator=(const Handle_Of_Point_Of_MembraneMesh& InputHandle);

    inline void SetIndex(int_max PointIndex); 
    inline int_max GetIndex() const;

    inline void SetToInvalid();
 
    inline bool operator==(const Handle_Of_Point_Of_MembraneMesh& InputHandle) const;
    inline bool operator!=(const Handle_Of_Point_Of_MembraneMesh& InputHandle) const;
    inline bool operator>(const Handle_Of_Point_Of_MembraneMesh& InputHandle) const;
    inline bool operator>=(const Handle_Of_Point_Of_MembraneMesh& InputHandle) const;
    inline bool operator<(const Handle_Of_Point_Of_MembraneMesh& InputHandle) const;
    inline bool operator<=(const Handle_Of_Point_Of_MembraneMesh& InputHandle) const;

};

//====================================== Handle_Of_Edge_Of_MembraneMesh ==============================================================//

struct Handle_Of_Edge_Of_MembraneMesh
{
private:
    int_max m_Index; // EdgeIndex in MembraneMesh::m_MeshData->EdgeList

public:
    inline Handle_Of_Edge_Of_MembraneMesh();
    inline Handle_Of_Edge_Of_MembraneMesh(const Handle_Of_Edge_Of_MembraneMesh& InputHandle);
    inline ~Handle_Of_Edge_Of_MembraneMesh();

    inline void operator=(const Handle_Of_Edge_Of_MembraneMesh& InputHandle);

    inline void SetIndex(int_max EdgeIndex);
    inline int_max GetIndex() const;

    inline void SetToInvalid();

    inline bool operator==(const Handle_Of_Edge_Of_MembraneMesh& InputHandle)  const;
    inline bool operator!=(const Handle_Of_Edge_Of_MembraneMesh& InputHandle)  const;
    inline bool operator>(const Handle_Of_Edge_Of_MembraneMesh& InputHandle) const;
    inline bool operator>=(const Handle_Of_Edge_Of_MembraneMesh& InputHandle) const;
    inline bool operator<(const Handle_Of_Edge_Of_MembraneMesh& InputHandle) const;
    inline bool operator<=(const Handle_Of_Edge_Of_MembraneMesh& InputHandle) const;

};

//====================================== DirectedEdge_Of_MembraneMesh (Cell Plasma Membrane) ===================================================//

struct DirectedEdgeIndex_Of_MembraneMesh
{
    int_max EdgeIndex = -1;
    int_max RelativeIndex = -1; // 0 or 1
//------------------------------------------
    DirectedEdgeIndex_Of_MembraneMesh() {}
    
    DirectedEdgeIndex_Of_MembraneMesh(const DirectedEdgeIndex_Of_MembraneMesh& InputIndex)
    {
        (*this) = InputIndex;
    }
    
    ~DirectedEdgeIndex_Of_MembraneMesh() {}

    void operator=(const DirectedEdgeIndex_Of_MembraneMesh& InputIndex)
    {
        EdgeIndex = InputIndex.EdgeIndex;
        RelativeIndex = InputIndex.RelativeIndex;
    }

    bool operator==(const DirectedEdgeIndex_Of_MembraneMesh& InputIndex) const
    {
        return (EdgeIndex == InputIndex.EdgeIndex) && (RelativeIndex == InputIndex.RelativeIndex);
    }

    bool operator!=(const DirectedEdgeIndex_Of_MembraneMesh& InputIndex) const
    {
        return (EdgeIndex != InputIndex.EdgeIndex) || (RelativeIndex == InputIndex.RelativeIndex);
    }

    bool operator>(const DirectedEdgeIndex_Of_MembraneMesh& InputIndex) const
    {
        if (EdgeIndex != InputIndex.EdgeIndex)
        {
            return EdgeIndex > InputIndex.EdgeIndex;
        }
        else
        {
            return RelativeIndex > InputIndex.RelativeIndex;
        }
    }

    bool operator>=(const DirectedEdgeIndex_Of_MembraneMesh& InputIndex) const
    {
        if (EdgeIndex != InputIndex.EdgeIndex)
        {
            return EdgeIndex >= InputIndex.EdgeIndex;
        }
        else
        {
            return RelativeIndex >= InputIndex.RelativeIndex;
        }
    }

    bool operator<(const DirectedEdgeIndex_Of_MembraneMesh& InputIndex) const
    {
        if (EdgeIndex != InputIndex.EdgeIndex)
        {
            return EdgeIndex < InputIndex.EdgeIndex;
        }
        else
        {
            return RelativeIndex < InputIndex.RelativeIndex;
        }
    }

    bool operator<=(const DirectedEdgeIndex_Of_MembraneMesh& InputIndex) const
    {
        if (EdgeIndex != InputIndex.EdgeIndex)
        {
            return EdgeIndex <= InputIndex.EdgeIndex;
        }
        else
        {
            return RelativeIndex <= InputIndex.RelativeIndex;
        }
    }
};

struct Handle_Of_DirectedEdge_Of_MembraneMesh
{
private:
    DirectedEdgeIndex_Of_MembraneMesh m_Index;  // DirectedEdgeIndex in MembraneMesh::m_MeshData->DirectedEdgePairList

public:
    inline Handle_Of_DirectedEdge_Of_MembraneMesh();
    inline Handle_Of_DirectedEdge_Of_MembraneMesh(const Handle_Of_DirectedEdge_Of_MembraneMesh& InputHandle);
    inline ~Handle_Of_DirectedEdge_Of_MembraneMesh();

    inline void operator=(const Handle_Of_DirectedEdge_Of_MembraneMesh& InputHandle);

    inline void SetIndex(DirectedEdgeIndex_Of_MembraneMesh DirectedEdgeIndex);
    inline void SetIndex(int_max EdgeIndex, int_max RelativeIndex);

    inline DirectedEdgeIndex_Of_MembraneMesh GetIndex() const;
    inline int_max GetEdgeIndex() const;
    inline int_max GetRelativeIndex() const;

    inline void SetToInvalid();

    inline bool operator==(const Handle_Of_DirectedEdge_Of_MembraneMesh& InputHandle)  const;
    inline bool operator!=(const Handle_Of_DirectedEdge_Of_MembraneMesh& InputHandle)  const;
    inline bool operator>(const Handle_Of_DirectedEdge_Of_MembraneMesh& InputHandle) const;
    inline bool operator>=(const Handle_Of_DirectedEdge_Of_MembraneMesh& InputHandle) const;
    inline bool operator<(const Handle_Of_DirectedEdge_Of_MembraneMesh& InputHandle) const;
    inline bool operator<=(const Handle_Of_DirectedEdge_Of_MembraneMesh& InputHandle) const;
    
};

//====================================== Cell_Of_MembraneMesh ==============================================================//

struct Handle_Of_Cell_Of_MembraneMesh
{
private:
    int_max m_Index;   // CellIndex in MembraneMesh::m_MeshData->CellList

public:
    inline Handle_Of_Cell_Of_MembraneMesh();
    inline Handle_Of_Cell_Of_MembraneMesh(const Handle_Of_Cell_Of_MembraneMesh& InputHandle);
    inline ~Handle_Of_Cell_Of_MembraneMesh();

    inline void operator=(const Handle_Of_Cell_Of_MembraneMesh& InputHandle);

    inline void SetIndex(int_max CellIndex);
    inline int_max GetIndex() const;

    inline void SetToInvalid();

    inline bool operator==(const Handle_Of_Cell_Of_MembraneMesh& InputHandle)  const;
    inline bool operator!=(const Handle_Of_Cell_Of_MembraneMesh& InputHandle)  const;
    inline bool operator>(const Handle_Of_Cell_Of_MembraneMesh& InputHandle) const;
    inline bool operator>=(const Handle_Of_Cell_Of_MembraneMesh& InputHandle) const;
    inline bool operator<(const Handle_Of_Cell_Of_MembraneMesh& InputHandle) const;
    inline bool operator<=(const Handle_Of_Cell_Of_MembraneMesh& InputHandle) const;

};

//================================================ Implementation ============================================================//

//====================================== Handle_Of_Point_Of_MembraneMesh ==============================================================//

inline Handle_Of_Point_Of_MembraneMesh::Handle_Of_Point_Of_MembraneMesh()
{
    this->SetToInvalid();
}

inline Handle_Of_Point_Of_MembraneMesh::Handle_Of_Point_Of_MembraneMesh(const Handle_Of_Point_Of_MembraneMesh& InputHandle)
{
    m_Index = InputHandle.m_Index;
}

inline Handle_Of_Point_Of_MembraneMesh::~Handle_Of_Point_Of_MembraneMesh()
{
}

inline void Handle_Of_Point_Of_MembraneMesh::operator=(const Handle_Of_Point_Of_MembraneMesh& InputHandle)
{
    m_Index = InputHandle.m_Index;
}

inline void Handle_Of_Point_Of_MembraneMesh::SetIndex(int_max PointIndex)
{
    m_Index = PointIndex;
}

inline int_max Handle_Of_Point_Of_MembraneMesh::GetIndex() const
{
    return m_Index;
}

inline void Handle_Of_Point_Of_MembraneMesh::SetToInvalid()
{
    m_Index = -1;
}

inline bool Handle_Of_Point_Of_MembraneMesh::operator==(const Handle_Of_Point_Of_MembraneMesh& InputHandle) const
{
    return m_Index == InputHandle.m_Index;
}

inline bool Handle_Of_Point_Of_MembraneMesh::operator!=(const Handle_Of_Point_Of_MembraneMesh& InputHandle) const
{
    return m_Index != InputHandle.m_Index;
}

inline bool Handle_Of_Point_Of_MembraneMesh::operator>(const Handle_Of_Point_Of_MembraneMesh& InputHandle) const
{
    return m_Index > InputHandle.m_Index;
}

inline bool Handle_Of_Point_Of_MembraneMesh::operator>=(const Handle_Of_Point_Of_MembraneMesh& InputHandle) const
{
    return m_Index >= InputHandle.m_Index;
}

inline bool Handle_Of_Point_Of_MembraneMesh::operator<(const Handle_Of_Point_Of_MembraneMesh& InputHandle) const
{
    return m_Index < InputHandle.m_Index;
}

inline bool Handle_Of_Point_Of_MembraneMesh::operator<=(const Handle_Of_Point_Of_MembraneMesh& InputHandle) const
{
    return m_Index <= InputHandle.m_Index;
}

//====================================== Handle_Of_Edge_Of_MembraneMesh ==============================================================//

inline Handle_Of_Edge_Of_MembraneMesh::Handle_Of_Edge_Of_MembraneMesh()
{
    this->SetToInvalid();
}

inline Handle_Of_Edge_Of_MembraneMesh::Handle_Of_Edge_Of_MembraneMesh(const Handle_Of_Edge_Of_MembraneMesh& InputHandle)
{
    m_Index = InputHandle.m_Index;
}

inline Handle_Of_Edge_Of_MembraneMesh::~Handle_Of_Edge_Of_MembraneMesh()
{
}

inline void Handle_Of_Edge_Of_MembraneMesh::operator=(const Handle_Of_Edge_Of_MembraneMesh& InputHandle)
{
    m_Index = InputHandle.m_Index;
}

inline void Handle_Of_Edge_Of_MembraneMesh::SetIndex(int_max EdgeIndex)
{
    m_Index = EdgeIndex;
}

inline int_max Handle_Of_Edge_Of_MembraneMesh::GetIndex() const
{
    return m_Index;
}

inline void Handle_Of_Edge_Of_MembraneMesh::SetToInvalid()
{
    m_Index = -1;
}

inline bool Handle_Of_Edge_Of_MembraneMesh::operator==(const Handle_Of_Edge_Of_MembraneMesh& InputHandle) const
{
    return m_Index == InputHandle.m_Index;
}

inline bool Handle_Of_Edge_Of_MembraneMesh::operator!=(const Handle_Of_Edge_Of_MembraneMesh& InputHandle) const
{
    return m_Index != InputHandle.m_Index;
}

inline bool Handle_Of_Edge_Of_MembraneMesh::operator>(const Handle_Of_Edge_Of_MembraneMesh& InputHandle) const
{
    return m_Index > InputHandle.m_Index;
}

inline bool Handle_Of_Edge_Of_MembraneMesh::operator>=(const Handle_Of_Edge_Of_MembraneMesh& InputHandle) const
{
    return m_Index >= InputHandle.m_Index;
}

inline bool Handle_Of_Edge_Of_MembraneMesh::operator<(const Handle_Of_Edge_Of_MembraneMesh& InputHandle) const
{
    return m_Index < InputHandle.m_Index;
}

inline bool Handle_Of_Edge_Of_MembraneMesh::operator<=(const Handle_Of_Edge_Of_MembraneMesh& InputHandle) const
{
    return m_Index <= InputHandle.m_Index;
}

//====================================== DirectedEdge_Of_MembraneMesh (Cell Plasma Membrane) ===================================================//

inline Handle_Of_DirectedEdge_Of_MembraneMesh::Handle_Of_DirectedEdge_Of_MembraneMesh()
{
    this->SetToInvalid();
}

inline Handle_Of_DirectedEdge_Of_MembraneMesh::Handle_Of_DirectedEdge_Of_MembraneMesh(const Handle_Of_DirectedEdge_Of_MembraneMesh& InputHandle)
{
    m_Index.EdgeIndex = InputHandle.m_Index.EdgeIndex;
    m_Index.RelativeIndex = InputHandle.m_Index.RelativeIndex;
}

inline Handle_Of_DirectedEdge_Of_MembraneMesh::~Handle_Of_DirectedEdge_Of_MembraneMesh()
{
}

inline void Handle_Of_DirectedEdge_Of_MembraneMesh::operator=(const Handle_Of_DirectedEdge_Of_MembraneMesh& InputHandle)
{
    m_Index.EdgeIndex = InputHandle.m_Index.EdgeIndex;
    m_Index.RelativeIndex = InputHandle.m_Index.RelativeIndex;
}

inline void Handle_Of_DirectedEdge_Of_MembraneMesh::SetIndex(DirectedEdgeIndex_Of_MembraneMesh DirectedEdgeIndex)
{
    m_Index.EdgeIndex = DirectedEdgeIndex.EdgeIndex;
    m_Index.RelativeIndex = DirectedEdgeIndex.RelativeIndex;
}

inline void Handle_Of_DirectedEdge_Of_MembraneMesh::SetIndex(int_max EdgeIndex, int_max RelativeIndex)
{
    m_Index.EdgeIndex = EdgeIndex;
    m_Index.RelativeIndex = RelativeIndex;
}

inline void Handle_Of_DirectedEdge_Of_MembraneMesh::SetToInvalid()
{
    m_Index.EdgeIndex = -1;
    m_Index.RelativeIndex = -1;
}

inline DirectedEdgeIndex_Of_MembraneMesh Handle_Of_DirectedEdge_Of_MembraneMesh::GetIndex() const
{
    return m_Index;
}

inline int_max Handle_Of_DirectedEdge_Of_MembraneMesh::GetEdgeIndex() const
{
    return m_Index.EdgeIndex;
}

inline int_max Handle_Of_DirectedEdge_Of_MembraneMesh::GetRelativeIndex() const
{
    return m_Index.RelativeIndex;
}

inline bool Handle_Of_DirectedEdge_Of_MembraneMesh::operator==(const Handle_Of_DirectedEdge_Of_MembraneMesh& InputHandle)  const
{
    return m_Index == InputHandle.m_Index;
}

inline bool Handle_Of_DirectedEdge_Of_MembraneMesh::operator!=(const Handle_Of_DirectedEdge_Of_MembraneMesh& InputHandle)  const
{
    return m_Index != InputHandle.m_Index;
}

inline bool Handle_Of_DirectedEdge_Of_MembraneMesh::operator>(const Handle_Of_DirectedEdge_Of_MembraneMesh& InputHandle) const
{
    return m_Index > InputHandle.m_Index;
}

inline bool Handle_Of_DirectedEdge_Of_MembraneMesh::operator>=(const Handle_Of_DirectedEdge_Of_MembraneMesh& InputHandle) const
{
    return m_Index >= InputHandle.m_Index;
}

inline bool Handle_Of_DirectedEdge_Of_MembraneMesh::operator<(const Handle_Of_DirectedEdge_Of_MembraneMesh& InputHandle) const
{
    return m_Index < InputHandle.m_Index;
}

inline bool Handle_Of_DirectedEdge_Of_MembraneMesh::operator<=(const Handle_Of_DirectedEdge_Of_MembraneMesh& InputHandle) const
{
    return m_Index <= InputHandle.m_Index;
}

//====================================== Cell_Of_MembraneMesh ==============================================================//

inline Handle_Of_Cell_Of_MembraneMesh::Handle_Of_Cell_Of_MembraneMesh()
{
    this->SetToInvalid();
}

inline Handle_Of_Cell_Of_MembraneMesh::Handle_Of_Cell_Of_MembraneMesh(const Handle_Of_Cell_Of_MembraneMesh& InputHandle)
{
    m_Index = InputHandle.m_Index;
}

inline Handle_Of_Cell_Of_MembraneMesh::~Handle_Of_Cell_Of_MembraneMesh()
{
}

inline void Handle_Of_Cell_Of_MembraneMesh::operator=(const Handle_Of_Cell_Of_MembraneMesh& InputHandle)
{
    m_Index = InputHandle.m_Index;
}

inline void Handle_Of_Cell_Of_MembraneMesh::SetIndex(int_max CellIndex)
{
    m_Index = CellIndex;
}

inline int_max Handle_Of_Cell_Of_MembraneMesh::GetIndex() const
{
    return m_Index;
}

inline void Handle_Of_Cell_Of_MembraneMesh::SetToInvalid()
{
    m_Index = -1;
}

inline bool Handle_Of_Cell_Of_MembraneMesh::operator==(const Handle_Of_Cell_Of_MembraneMesh& InputHandle)  const
{
    return m_Index == InputHandle.m_Index;
}

inline bool Handle_Of_Cell_Of_MembraneMesh::operator!=(const Handle_Of_Cell_Of_MembraneMesh& InputHandle)  const
{
    return m_Index != InputHandle.m_Index;
}

inline bool Handle_Of_Cell_Of_MembraneMesh::operator>(const Handle_Of_Cell_Of_MembraneMesh& InputHandle) const
{
    return m_Index > InputHandle.m_Index;
}

inline bool Handle_Of_Cell_Of_MembraneMesh::operator>=(const Handle_Of_Cell_Of_MembraneMesh& InputHandle) const
{
    return m_Index >= InputHandle.m_Index;
}

inline bool Handle_Of_Cell_Of_MembraneMesh::operator<(const Handle_Of_Cell_Of_MembraneMesh& InputHandle) const
{
    return m_Index < InputHandle.m_Index;
}

inline bool Handle_Of_Cell_Of_MembraneMesh::operator<=(const Handle_Of_Cell_Of_MembraneMesh& InputHandle) const
{
    return m_Index <= InputHandle.m_Index;
}


}// namespace mdk

#endif