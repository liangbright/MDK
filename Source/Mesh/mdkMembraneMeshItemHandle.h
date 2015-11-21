#ifndef mdk_MembraneMeshItemHandle_h
#define mdk_MembraneMeshItemHandle_h

#include "mdkCommonType.h"

namespace mdk
{
//================================================ Declaration ============================================================//

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

//====================================== DirectedEdge_Of_MembraneMesh (Face Boundary) ===================================================//

struct DirectedEdgeIndex_Of_MembraneMesh
{
    int_max FaceIndex = -1;
    int_max RelativeIndex = -1;
//------------------------------------------
    DirectedEdgeIndex_Of_MembraneMesh() {}
    
    DirectedEdgeIndex_Of_MembraneMesh(const DirectedEdgeIndex_Of_MembraneMesh& InputIndex)
    {
        (*this) = InputIndex;
    }
    
    ~DirectedEdgeIndex_Of_MembraneMesh() {}

    void operator=(const DirectedEdgeIndex_Of_MembraneMesh& InputIndex)
    {
		FaceIndex = InputIndex.FaceIndex;
        RelativeIndex = InputIndex.RelativeIndex;
    }

    bool operator==(const DirectedEdgeIndex_Of_MembraneMesh& InputIndex) const
    {
		return (FaceIndex == InputIndex.FaceIndex) && (RelativeIndex == InputIndex.RelativeIndex);
    }

    bool operator!=(const DirectedEdgeIndex_Of_MembraneMesh& InputIndex) const
    {
		return (FaceIndex != InputIndex.FaceIndex) || (RelativeIndex == InputIndex.RelativeIndex);
    }

    bool operator>(const DirectedEdgeIndex_Of_MembraneMesh& InputIndex) const
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

    bool operator>=(const DirectedEdgeIndex_Of_MembraneMesh& InputIndex) const
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

    bool operator<(const DirectedEdgeIndex_Of_MembraneMesh& InputIndex) const
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

    bool operator<=(const DirectedEdgeIndex_Of_MembraneMesh& InputIndex) const
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
	inline void SetIndex(int_max FaceIndex, int_max RelativeIndex);

    inline DirectedEdgeIndex_Of_MembraneMesh GetIndex() const;
    inline int_max GetFaceIndex() const;
    inline int_max GetRelativeIndex() const;

    inline void SetToInvalid();

    inline bool operator==(const Handle_Of_DirectedEdge_Of_MembraneMesh& InputHandle) const;
    inline bool operator!=(const Handle_Of_DirectedEdge_Of_MembraneMesh& InputHandle) const;
    inline bool operator>(const Handle_Of_DirectedEdge_Of_MembraneMesh& InputHandle) const;
    inline bool operator>=(const Handle_Of_DirectedEdge_Of_MembraneMesh& InputHandle) const;
    inline bool operator<(const Handle_Of_DirectedEdge_Of_MembraneMesh& InputHandle) const;
    inline bool operator<=(const Handle_Of_DirectedEdge_Of_MembraneMesh& InputHandle) const;
    
};

//====================================== Face_Of_MembraneMesh ==============================================================//

struct Handle_Of_Face_Of_MembraneMesh
{
private:
    int_max m_Index;   // FaceIndex in MembraneMesh::m_MeshData->FaceList

public:
    inline Handle_Of_Face_Of_MembraneMesh();
    inline Handle_Of_Face_Of_MembraneMesh(const Handle_Of_Face_Of_MembraneMesh& InputHandle);
    inline ~Handle_Of_Face_Of_MembraneMesh();

    inline void operator=(const Handle_Of_Face_Of_MembraneMesh& InputHandle);

    inline void SetIndex(int_max FaceIndex);
    inline int_max GetIndex() const;

    inline void SetToInvalid();

    inline bool operator==(const Handle_Of_Face_Of_MembraneMesh& InputHandle) const;
    inline bool operator!=(const Handle_Of_Face_Of_MembraneMesh& InputHandle) const;
    inline bool operator>(const Handle_Of_Face_Of_MembraneMesh& InputHandle) const;
    inline bool operator>=(const Handle_Of_Face_Of_MembraneMesh& InputHandle) const;
    inline bool operator<(const Handle_Of_Face_Of_MembraneMesh& InputHandle) const;
    inline bool operator<=(const Handle_Of_Face_Of_MembraneMesh& InputHandle) const;

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

//====================================== DirectedEdge_Of_MembraneMesh (Face Plasma Membrane) ===================================================//

inline Handle_Of_DirectedEdge_Of_MembraneMesh::Handle_Of_DirectedEdge_Of_MembraneMesh()
{
    this->SetToInvalid();
}

inline Handle_Of_DirectedEdge_Of_MembraneMesh::Handle_Of_DirectedEdge_Of_MembraneMesh(const Handle_Of_DirectedEdge_Of_MembraneMesh& InputHandle)
{
	m_Index.FaceIndex = InputHandle.m_Index.FaceIndex;
    m_Index.RelativeIndex = InputHandle.m_Index.RelativeIndex;
}

inline Handle_Of_DirectedEdge_Of_MembraneMesh::~Handle_Of_DirectedEdge_Of_MembraneMesh()
{
}

inline void Handle_Of_DirectedEdge_Of_MembraneMesh::operator=(const Handle_Of_DirectedEdge_Of_MembraneMesh& InputHandle)
{
	m_Index.FaceIndex = InputHandle.m_Index.FaceIndex;
    m_Index.RelativeIndex = InputHandle.m_Index.RelativeIndex;
}

inline void Handle_Of_DirectedEdge_Of_MembraneMesh::SetIndex(DirectedEdgeIndex_Of_MembraneMesh DirectedEdgeIndex)
{
	m_Index.FaceIndex = DirectedEdgeIndex.FaceIndex;
    m_Index.RelativeIndex = DirectedEdgeIndex.RelativeIndex;
}

inline void Handle_Of_DirectedEdge_Of_MembraneMesh::SetIndex(int_max FaceIndex, int_max RelativeIndex)
{
	m_Index.FaceIndex = FaceIndex;
    m_Index.RelativeIndex = RelativeIndex;
}

inline void Handle_Of_DirectedEdge_Of_MembraneMesh::SetToInvalid()
{
	m_Index.FaceIndex = -1;
    m_Index.RelativeIndex = -1;
}

inline DirectedEdgeIndex_Of_MembraneMesh Handle_Of_DirectedEdge_Of_MembraneMesh::GetIndex() const
{
    return m_Index;
}

inline int_max Handle_Of_DirectedEdge_Of_MembraneMesh::GetFaceIndex() const
{
	return m_Index.FaceIndex;
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

//====================================== Face_Of_MembraneMesh ==============================================================//

inline Handle_Of_Face_Of_MembraneMesh::Handle_Of_Face_Of_MembraneMesh()
{
    this->SetToInvalid();
}

inline Handle_Of_Face_Of_MembraneMesh::Handle_Of_Face_Of_MembraneMesh(const Handle_Of_Face_Of_MembraneMesh& InputHandle)
{
    m_Index = InputHandle.m_Index;
}

inline Handle_Of_Face_Of_MembraneMesh::~Handle_Of_Face_Of_MembraneMesh()
{
}

inline void Handle_Of_Face_Of_MembraneMesh::operator=(const Handle_Of_Face_Of_MembraneMesh& InputHandle)
{
    m_Index = InputHandle.m_Index;
}

inline void Handle_Of_Face_Of_MembraneMesh::SetIndex(int_max FaceIndex)
{
    m_Index = FaceIndex;
}

inline int_max Handle_Of_Face_Of_MembraneMesh::GetIndex() const
{
    return m_Index;
}

inline void Handle_Of_Face_Of_MembraneMesh::SetToInvalid()
{
    m_Index = -1;
}

inline bool Handle_Of_Face_Of_MembraneMesh::operator==(const Handle_Of_Face_Of_MembraneMesh& InputHandle)  const
{
    return m_Index == InputHandle.m_Index;
}

inline bool Handle_Of_Face_Of_MembraneMesh::operator!=(const Handle_Of_Face_Of_MembraneMesh& InputHandle)  const
{
    return m_Index != InputHandle.m_Index;
}

inline bool Handle_Of_Face_Of_MembraneMesh::operator>(const Handle_Of_Face_Of_MembraneMesh& InputHandle) const
{
    return m_Index > InputHandle.m_Index;
}

inline bool Handle_Of_Face_Of_MembraneMesh::operator>=(const Handle_Of_Face_Of_MembraneMesh& InputHandle) const
{
    return m_Index >= InputHandle.m_Index;
}

inline bool Handle_Of_Face_Of_MembraneMesh::operator<(const Handle_Of_Face_Of_MembraneMesh& InputHandle) const
{
    return m_Index < InputHandle.m_Index;
}

inline bool Handle_Of_Face_Of_MembraneMesh::operator<=(const Handle_Of_Face_Of_MembraneMesh& InputHandle) const
{
    return m_Index <= InputHandle.m_Index;
}


}// namespace mdk

#endif