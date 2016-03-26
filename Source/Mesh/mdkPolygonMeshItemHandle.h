#pragma once

#include "mdkCommonType.h"

namespace mdk
{
//================================================ Declaration ============================================================//

//====================================== Handle_Of_Point_Of_PolygonMesh ==============================================================//

struct Handle_Of_Point_Of_PolygonMesh
{
private:
    int_max m_Index; // PointIndex in PolygonMesh::m_MeshData->PointList

public:
    inline  Handle_Of_Point_Of_PolygonMesh();
    inline  Handle_Of_Point_Of_PolygonMesh(const Handle_Of_Point_Of_PolygonMesh& InputHandle);    
    inline  ~Handle_Of_Point_Of_PolygonMesh();

    inline void operator=(const Handle_Of_Point_Of_PolygonMesh& InputHandle);

    inline void SetIndex(int_max PointIndex); 
    inline int_max GetIndex() const;

    inline void SetToInvalid();
 
    inline bool operator==(const Handle_Of_Point_Of_PolygonMesh& InputHandle) const;
    inline bool operator!=(const Handle_Of_Point_Of_PolygonMesh& InputHandle) const;
    inline bool operator>(const Handle_Of_Point_Of_PolygonMesh& InputHandle) const;
    inline bool operator>=(const Handle_Of_Point_Of_PolygonMesh& InputHandle) const;
    inline bool operator<(const Handle_Of_Point_Of_PolygonMesh& InputHandle) const;
    inline bool operator<=(const Handle_Of_Point_Of_PolygonMesh& InputHandle) const;

};

//====================================== Handle_Of_Edge_Of_PolygonMesh ==============================================================//

struct Handle_Of_Edge_Of_PolygonMesh
{
private:
    int_max m_Index; // EdgeIndex in PolygonMesh::m_MeshData->EdgeList

public:
    inline Handle_Of_Edge_Of_PolygonMesh();
    inline Handle_Of_Edge_Of_PolygonMesh(const Handle_Of_Edge_Of_PolygonMesh& InputHandle);
    inline ~Handle_Of_Edge_Of_PolygonMesh();

    inline void operator=(const Handle_Of_Edge_Of_PolygonMesh& InputHandle);

    inline void SetIndex(int_max EdgeIndex);
    inline int_max GetIndex() const;

    inline void SetToInvalid();

    inline bool operator==(const Handle_Of_Edge_Of_PolygonMesh& InputHandle)  const;
    inline bool operator!=(const Handle_Of_Edge_Of_PolygonMesh& InputHandle)  const;
    inline bool operator>(const Handle_Of_Edge_Of_PolygonMesh& InputHandle) const;
    inline bool operator>=(const Handle_Of_Edge_Of_PolygonMesh& InputHandle) const;
    inline bool operator<(const Handle_Of_Edge_Of_PolygonMesh& InputHandle) const;
    inline bool operator<=(const Handle_Of_Edge_Of_PolygonMesh& InputHandle) const;

};

//====================================== DirectedEdge_Of_PolygonMesh (Face Boundary) ===================================================//

struct DirectedEdgeIndex_Of_PolygonMesh
{
    int_max FaceIndex = -1;
    int_max RelativeIndex = -1;
//------------------------------------------
    DirectedEdgeIndex_Of_PolygonMesh() {}
    
    DirectedEdgeIndex_Of_PolygonMesh(const DirectedEdgeIndex_Of_PolygonMesh& InputIndex)
    {
        (*this) = InputIndex;
    }
    
    ~DirectedEdgeIndex_Of_PolygonMesh() {}

    void operator=(const DirectedEdgeIndex_Of_PolygonMesh& InputIndex)
    {
		FaceIndex = InputIndex.FaceIndex;
        RelativeIndex = InputIndex.RelativeIndex;
    }

    bool operator==(const DirectedEdgeIndex_Of_PolygonMesh& InputIndex) const
    {
		return (FaceIndex == InputIndex.FaceIndex) && (RelativeIndex == InputIndex.RelativeIndex);
    }

    bool operator!=(const DirectedEdgeIndex_Of_PolygonMesh& InputIndex) const
    {
		return (FaceIndex != InputIndex.FaceIndex) || (RelativeIndex == InputIndex.RelativeIndex);
    }

    bool operator>(const DirectedEdgeIndex_Of_PolygonMesh& InputIndex) const
    {
		if (FaceIndex != InputIndex.FaceIndex)
        {
			return FaceIndex > InputIndex.FaceIndex;
        }
        else
        {
            return RelativeIndex > InputIndex.RelativeIndex;
        }
    }

    bool operator>=(const DirectedEdgeIndex_Of_PolygonMesh& InputIndex) const
    {
		if (FaceIndex != InputIndex.FaceIndex)
        {
			return FaceIndex >= InputIndex.FaceIndex;
        }
        else
        {
            return RelativeIndex >= InputIndex.RelativeIndex;
        }
    }

    bool operator<(const DirectedEdgeIndex_Of_PolygonMesh& InputIndex) const
    {
		if (FaceIndex != InputIndex.FaceIndex)
        {
			return FaceIndex < InputIndex.FaceIndex;
        }
        else
        {
            return RelativeIndex < InputIndex.RelativeIndex;
        }
    }

    bool operator<=(const DirectedEdgeIndex_Of_PolygonMesh& InputIndex) const
    {
		if (FaceIndex != InputIndex.FaceIndex)
        {
			return FaceIndex <= InputIndex.FaceIndex;
        }
        else
        {
            return RelativeIndex <= InputIndex.RelativeIndex;
        }
    }
};

struct Handle_Of_DirectedEdge_Of_PolygonMesh
{
private:
    DirectedEdgeIndex_Of_PolygonMesh m_Index;  // DirectedEdgeIndex in PolygonMesh::m_MeshData->DirectedEdgePairList

public:
    inline Handle_Of_DirectedEdge_Of_PolygonMesh();
    inline Handle_Of_DirectedEdge_Of_PolygonMesh(const Handle_Of_DirectedEdge_Of_PolygonMesh& InputHandle);
    inline ~Handle_Of_DirectedEdge_Of_PolygonMesh();

    inline void operator=(const Handle_Of_DirectedEdge_Of_PolygonMesh& InputHandle);

    inline void SetIndex(DirectedEdgeIndex_Of_PolygonMesh DirectedEdgeIndex);
	inline void SetIndex(int_max FaceIndex, int_max RelativeIndex);

    inline DirectedEdgeIndex_Of_PolygonMesh GetIndex() const;
    inline int_max GetFaceIndex() const;
    inline int_max GetRelativeIndex() const;

    inline void SetToInvalid();

    inline bool operator==(const Handle_Of_DirectedEdge_Of_PolygonMesh& InputHandle) const;
    inline bool operator!=(const Handle_Of_DirectedEdge_Of_PolygonMesh& InputHandle) const;
    inline bool operator>(const Handle_Of_DirectedEdge_Of_PolygonMesh& InputHandle) const;
    inline bool operator>=(const Handle_Of_DirectedEdge_Of_PolygonMesh& InputHandle) const;
    inline bool operator<(const Handle_Of_DirectedEdge_Of_PolygonMesh& InputHandle) const;
    inline bool operator<=(const Handle_Of_DirectedEdge_Of_PolygonMesh& InputHandle) const;
    
};

//====================================== Face_Of_PolygonMesh ==============================================================//

struct Handle_Of_Face_Of_PolygonMesh
{
private:
    int_max m_Index;   // FaceIndex in PolygonMesh::m_MeshData->FaceList

public:
    inline Handle_Of_Face_Of_PolygonMesh();
    inline Handle_Of_Face_Of_PolygonMesh(const Handle_Of_Face_Of_PolygonMesh& InputHandle);
    inline ~Handle_Of_Face_Of_PolygonMesh();

    inline void operator=(const Handle_Of_Face_Of_PolygonMesh& InputHandle);

    inline void SetIndex(int_max FaceIndex);
    inline int_max GetIndex() const;

    inline void SetToInvalid();

    inline bool operator==(const Handle_Of_Face_Of_PolygonMesh& InputHandle) const;
    inline bool operator!=(const Handle_Of_Face_Of_PolygonMesh& InputHandle) const;
    inline bool operator>(const Handle_Of_Face_Of_PolygonMesh& InputHandle) const;
    inline bool operator>=(const Handle_Of_Face_Of_PolygonMesh& InputHandle) const;
    inline bool operator<(const Handle_Of_Face_Of_PolygonMesh& InputHandle) const;
    inline bool operator<=(const Handle_Of_Face_Of_PolygonMesh& InputHandle) const;

};

//================================================ Implementation ============================================================//

//====================================== Handle_Of_Point_Of_PolygonMesh ==============================================================//

inline Handle_Of_Point_Of_PolygonMesh::Handle_Of_Point_Of_PolygonMesh()
{
    this->SetToInvalid();
}

inline Handle_Of_Point_Of_PolygonMesh::Handle_Of_Point_Of_PolygonMesh(const Handle_Of_Point_Of_PolygonMesh& InputHandle)
{
    m_Index = InputHandle.m_Index;
}

inline Handle_Of_Point_Of_PolygonMesh::~Handle_Of_Point_Of_PolygonMesh()
{
}

inline void Handle_Of_Point_Of_PolygonMesh::operator=(const Handle_Of_Point_Of_PolygonMesh& InputHandle)
{
    m_Index = InputHandle.m_Index;
}

inline void Handle_Of_Point_Of_PolygonMesh::SetIndex(int_max PointIndex)
{
    m_Index = PointIndex;
}

inline int_max Handle_Of_Point_Of_PolygonMesh::GetIndex() const
{
    return m_Index;
}

inline void Handle_Of_Point_Of_PolygonMesh::SetToInvalid()
{
    m_Index = -1;
}

inline bool Handle_Of_Point_Of_PolygonMesh::operator==(const Handle_Of_Point_Of_PolygonMesh& InputHandle) const
{
    return m_Index == InputHandle.m_Index;
}

inline bool Handle_Of_Point_Of_PolygonMesh::operator!=(const Handle_Of_Point_Of_PolygonMesh& InputHandle) const
{
    return m_Index != InputHandle.m_Index;
}

inline bool Handle_Of_Point_Of_PolygonMesh::operator>(const Handle_Of_Point_Of_PolygonMesh& InputHandle) const
{
    return m_Index > InputHandle.m_Index;
}

inline bool Handle_Of_Point_Of_PolygonMesh::operator>=(const Handle_Of_Point_Of_PolygonMesh& InputHandle) const
{
    return m_Index >= InputHandle.m_Index;
}

inline bool Handle_Of_Point_Of_PolygonMesh::operator<(const Handle_Of_Point_Of_PolygonMesh& InputHandle) const
{
    return m_Index < InputHandle.m_Index;
}

inline bool Handle_Of_Point_Of_PolygonMesh::operator<=(const Handle_Of_Point_Of_PolygonMesh& InputHandle) const
{
    return m_Index <= InputHandle.m_Index;
}

//====================================== Handle_Of_Edge_Of_PolygonMesh ==============================================================//

inline Handle_Of_Edge_Of_PolygonMesh::Handle_Of_Edge_Of_PolygonMesh()
{
    this->SetToInvalid();
}

inline Handle_Of_Edge_Of_PolygonMesh::Handle_Of_Edge_Of_PolygonMesh(const Handle_Of_Edge_Of_PolygonMesh& InputHandle)
{
    m_Index = InputHandle.m_Index;
}

inline Handle_Of_Edge_Of_PolygonMesh::~Handle_Of_Edge_Of_PolygonMesh()
{
}

inline void Handle_Of_Edge_Of_PolygonMesh::operator=(const Handle_Of_Edge_Of_PolygonMesh& InputHandle)
{
    m_Index = InputHandle.m_Index;
}

inline void Handle_Of_Edge_Of_PolygonMesh::SetIndex(int_max EdgeIndex)
{
    m_Index = EdgeIndex;
}

inline int_max Handle_Of_Edge_Of_PolygonMesh::GetIndex() const
{
    return m_Index;
}

inline void Handle_Of_Edge_Of_PolygonMesh::SetToInvalid()
{
    m_Index = -1;
}

inline bool Handle_Of_Edge_Of_PolygonMesh::operator==(const Handle_Of_Edge_Of_PolygonMesh& InputHandle) const
{
    return m_Index == InputHandle.m_Index;
}

inline bool Handle_Of_Edge_Of_PolygonMesh::operator!=(const Handle_Of_Edge_Of_PolygonMesh& InputHandle) const
{
    return m_Index != InputHandle.m_Index;
}

inline bool Handle_Of_Edge_Of_PolygonMesh::operator>(const Handle_Of_Edge_Of_PolygonMesh& InputHandle) const
{
    return m_Index > InputHandle.m_Index;
}

inline bool Handle_Of_Edge_Of_PolygonMesh::operator>=(const Handle_Of_Edge_Of_PolygonMesh& InputHandle) const
{
    return m_Index >= InputHandle.m_Index;
}

inline bool Handle_Of_Edge_Of_PolygonMesh::operator<(const Handle_Of_Edge_Of_PolygonMesh& InputHandle) const
{
    return m_Index < InputHandle.m_Index;
}

inline bool Handle_Of_Edge_Of_PolygonMesh::operator<=(const Handle_Of_Edge_Of_PolygonMesh& InputHandle) const
{
    return m_Index <= InputHandle.m_Index;
}

//====================================== DirectedEdge_Of_PolygonMesh (Face Plasma Polygon) ===================================================//

inline Handle_Of_DirectedEdge_Of_PolygonMesh::Handle_Of_DirectedEdge_Of_PolygonMesh()
{
    this->SetToInvalid();
}

inline Handle_Of_DirectedEdge_Of_PolygonMesh::Handle_Of_DirectedEdge_Of_PolygonMesh(const Handle_Of_DirectedEdge_Of_PolygonMesh& InputHandle)
{
	m_Index.FaceIndex = InputHandle.m_Index.FaceIndex;
    m_Index.RelativeIndex = InputHandle.m_Index.RelativeIndex;
}

inline Handle_Of_DirectedEdge_Of_PolygonMesh::~Handle_Of_DirectedEdge_Of_PolygonMesh()
{
}

inline void Handle_Of_DirectedEdge_Of_PolygonMesh::operator=(const Handle_Of_DirectedEdge_Of_PolygonMesh& InputHandle)
{
	m_Index.FaceIndex = InputHandle.m_Index.FaceIndex;
    m_Index.RelativeIndex = InputHandle.m_Index.RelativeIndex;
}

inline void Handle_Of_DirectedEdge_Of_PolygonMesh::SetIndex(DirectedEdgeIndex_Of_PolygonMesh DirectedEdgeIndex)
{
	m_Index.FaceIndex = DirectedEdgeIndex.FaceIndex;
    m_Index.RelativeIndex = DirectedEdgeIndex.RelativeIndex;
}

inline void Handle_Of_DirectedEdge_Of_PolygonMesh::SetIndex(int_max FaceIndex, int_max RelativeIndex)
{
	m_Index.FaceIndex = FaceIndex;
    m_Index.RelativeIndex = RelativeIndex;
}

inline void Handle_Of_DirectedEdge_Of_PolygonMesh::SetToInvalid()
{
	m_Index.FaceIndex = -1;
    m_Index.RelativeIndex = -1;
}

inline DirectedEdgeIndex_Of_PolygonMesh Handle_Of_DirectedEdge_Of_PolygonMesh::GetIndex() const
{
    return m_Index;
}

inline int_max Handle_Of_DirectedEdge_Of_PolygonMesh::GetFaceIndex() const
{
	return m_Index.FaceIndex;
}

inline int_max Handle_Of_DirectedEdge_Of_PolygonMesh::GetRelativeIndex() const
{
    return m_Index.RelativeIndex;
}

inline bool Handle_Of_DirectedEdge_Of_PolygonMesh::operator==(const Handle_Of_DirectedEdge_Of_PolygonMesh& InputHandle)  const
{
    return m_Index == InputHandle.m_Index;
}

inline bool Handle_Of_DirectedEdge_Of_PolygonMesh::operator!=(const Handle_Of_DirectedEdge_Of_PolygonMesh& InputHandle)  const
{
    return m_Index != InputHandle.m_Index;
}

inline bool Handle_Of_DirectedEdge_Of_PolygonMesh::operator>(const Handle_Of_DirectedEdge_Of_PolygonMesh& InputHandle) const
{
    return m_Index > InputHandle.m_Index;
}

inline bool Handle_Of_DirectedEdge_Of_PolygonMesh::operator>=(const Handle_Of_DirectedEdge_Of_PolygonMesh& InputHandle) const
{
    return m_Index >= InputHandle.m_Index;
}

inline bool Handle_Of_DirectedEdge_Of_PolygonMesh::operator<(const Handle_Of_DirectedEdge_Of_PolygonMesh& InputHandle) const
{
    return m_Index < InputHandle.m_Index;
}

inline bool Handle_Of_DirectedEdge_Of_PolygonMesh::operator<=(const Handle_Of_DirectedEdge_Of_PolygonMesh& InputHandle) const
{
    return m_Index <= InputHandle.m_Index;
}

//====================================== Face_Of_PolygonMesh ==============================================================//

inline Handle_Of_Face_Of_PolygonMesh::Handle_Of_Face_Of_PolygonMesh()
{
    this->SetToInvalid();
}

inline Handle_Of_Face_Of_PolygonMesh::Handle_Of_Face_Of_PolygonMesh(const Handle_Of_Face_Of_PolygonMesh& InputHandle)
{
    m_Index = InputHandle.m_Index;
}

inline Handle_Of_Face_Of_PolygonMesh::~Handle_Of_Face_Of_PolygonMesh()
{
}

inline void Handle_Of_Face_Of_PolygonMesh::operator=(const Handle_Of_Face_Of_PolygonMesh& InputHandle)
{
    m_Index = InputHandle.m_Index;
}

inline void Handle_Of_Face_Of_PolygonMesh::SetIndex(int_max FaceIndex)
{
    m_Index = FaceIndex;
}

inline int_max Handle_Of_Face_Of_PolygonMesh::GetIndex() const
{
    return m_Index;
}

inline void Handle_Of_Face_Of_PolygonMesh::SetToInvalid()
{
    m_Index = -1;
}

inline bool Handle_Of_Face_Of_PolygonMesh::operator==(const Handle_Of_Face_Of_PolygonMesh& InputHandle)  const
{
    return m_Index == InputHandle.m_Index;
}

inline bool Handle_Of_Face_Of_PolygonMesh::operator!=(const Handle_Of_Face_Of_PolygonMesh& InputHandle)  const
{
    return m_Index != InputHandle.m_Index;
}

inline bool Handle_Of_Face_Of_PolygonMesh::operator>(const Handle_Of_Face_Of_PolygonMesh& InputHandle) const
{
    return m_Index > InputHandle.m_Index;
}

inline bool Handle_Of_Face_Of_PolygonMesh::operator>=(const Handle_Of_Face_Of_PolygonMesh& InputHandle) const
{
    return m_Index >= InputHandle.m_Index;
}

inline bool Handle_Of_Face_Of_PolygonMesh::operator<(const Handle_Of_Face_Of_PolygonMesh& InputHandle) const
{
    return m_Index < InputHandle.m_Index;
}

inline bool Handle_Of_Face_Of_PolygonMesh::operator<=(const Handle_Of_Face_Of_PolygonMesh& InputHandle) const
{
    return m_Index <= InputHandle.m_Index;
}


}// namespace mdk
