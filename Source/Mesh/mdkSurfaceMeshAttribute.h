#ifndef __mdkSurfaceMeshAttribute_h
#define __mdkSurfaceMeshAttribute_h

namespace mdk
{
//============================================== GlobalAttribute_Of_SurfaceMesh ===========================================//

template<typename ScalarType>
struct GlobalAttribute_Of_SurfaceMesh
{
    GlobalAttribute_Of_SurfaceMesh() {}
    GlobalAttribute_Of_SurfaceMesh(const GlobalAttribute_Of_SurfaceMesh&) {}
    ~GlobalAttribute_Of_SurfaceMesh() {}
    void operator=(const GlobalAttribute_Of_SurfaceMesh&) {}
    void Clear() {}
};

//============================================== PointAttribute_Of_SurfaceMesh ===========================================//

template<typename ScalarType>
struct PointAttribute_Of_SurfaceMesh
{
    PointAttribute_Of_SurfaceMesh() {}
    PointAttribute_Of_SurfaceMesh(const PointAttribute_Of_SurfaceMesh&) {}
    ~PointAttribute_Of_SurfaceMesh() {}
    void operator=(const PointAttribute_Of_SurfaceMesh&) {}
    void Clear() {}
};

//============================================== EdgeAttribute_Of_SurfaceMesh ===========================================//

template<typename ScalarType>
struct EdgeAttribute_Of_SurfaceMesh
{
    EdgeAttribute_Of_SurfaceMesh() {}
    EdgeAttribute_Of_SurfaceMesh(const EdgeAttribute_Of_SurfaceMesh&) {}
    ~EdgeAttribute_Of_SurfaceMesh() {}
    void operator=(const EdgeAttribute_Of_SurfaceMesh) {}
    virtual void Clear(){}
};

//============================================== DirectedEdgeAttribute_Of_SurfaceMesh ===========================================//
template<typename ScalarType>
struct DirectedEdgeAttribute_Of_SurfaceMesh
{
    DirectedEdgeAttribute_Of_SurfaceMesh() {}
    DirectedEdgeAttribute_Of_SurfaceMesh(const DirectedEdgeAttribute_Of_SurfaceMesh&) {}
    ~DirectedEdgeAttribute_Of_SurfaceMesh() {}
    void operator=(const DirectedEdgeAttribute_Of_SurfaceMesh&) {}
    void Clear() {}
};

//============================================== CellAttribute_Of_SurfaceMesh ===========================================//

template<typename ScalarType>
struct CellAttribute_Of_SurfaceMesh
{
    CellAttribute_Of_SurfaceMesh() {}
    CellAttribute_Of_SurfaceMesh(const CellAttribute_Of_SurfaceMesh& InputAttribute) {}
    ~CellAttribute_Of_SurfaceMesh() {}
    void operator=(const CellAttribute_Of_SurfaceMesh&) {}
    void Clear() {}
};

}// namespace mdk


#endif