#ifndef __mdkMembraneMeshAttribute_h
#define __mdkMembraneMeshAttribute_h

namespace mdk
{
//============================================== GlobalAttribute_Of_MembraneMesh ===========================================//

template<typename ScalarType>
struct GlobalAttribute_Of_MembraneMesh
{
    GlobalAttribute_Of_MembraneMesh() {}
    GlobalAttribute_Of_MembraneMesh(const GlobalAttribute_Of_MembraneMesh&) {}
    ~GlobalAttribute_Of_MembraneMesh() {}
    void operator=(const GlobalAttribute_Of_MembraneMesh&) {}
    void Clear() {}
};

//============================================== PointAttribute_Of_MembraneMesh ===========================================//

template<typename ScalarType>
struct PointAttribute_Of_MembraneMesh
{
    PointAttribute_Of_MembraneMesh() {}
    PointAttribute_Of_MembraneMesh(const PointAttribute_Of_MembraneMesh&) {}
    ~PointAttribute_Of_MembraneMesh() {}
    void operator=(const PointAttribute_Of_MembraneMesh&) {}
    void Clear() {}
};

//============================================== EdgeAttribute_Of_MembraneMesh ===========================================//

template<typename ScalarType>
struct EdgeAttribute_Of_MembraneMesh
{
    EdgeAttribute_Of_MembraneMesh() {}
    EdgeAttribute_Of_MembraneMesh(const EdgeAttribute_Of_MembraneMesh&) {}
    ~EdgeAttribute_Of_MembraneMesh() {}
    void operator=(const EdgeAttribute_Of_MembraneMesh) {}
    virtual void Clear(){}
};

//============================================== DirectedEdgeAttribute_Of_MembraneMesh ===========================================//
template<typename ScalarType>
struct DirectedEdgeAttribute_Of_MembraneMesh
{
    DirectedEdgeAttribute_Of_MembraneMesh() {}
    DirectedEdgeAttribute_Of_MembraneMesh(const DirectedEdgeAttribute_Of_MembraneMesh&) {}
    ~DirectedEdgeAttribute_Of_MembraneMesh() {}
    void operator=(const DirectedEdgeAttribute_Of_MembraneMesh&) {}
    void Clear() {}
};

//============================================== CellAttribute_Of_MembraneMesh ===========================================//

template<typename ScalarType>
struct CellAttribute_Of_MembraneMesh
{
    CellAttribute_Of_MembraneMesh() {}
    CellAttribute_Of_MembraneMesh(const CellAttribute_Of_MembraneMesh& InputAttribute) {}
    ~CellAttribute_Of_MembraneMesh() {}
    void operator=(const CellAttribute_Of_MembraneMesh&) {}
    void Clear() {}
};

}// namespace mdk


#endif