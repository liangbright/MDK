#ifndef __mdkTriangleMesh_h
#define __mdkTriangleMesh_h

#include "mdkPolygonMesh.h"
#include "mdkTriangleMeshAttribute.h"

namespace mdk
{

template<typename ScalarType>
struct TriangleMeshAttributeType
{
    typedef ScalarType  ScalarType;
    typedef PointAttribute_Of_TriangleMesh<ScalarType>          PointAttributeType;
    typedef EdgeAttribute_Of_TriangleMesh<ScalarType>           EdgeAttributeType;
    typedef DirectedEdgeAttribute_Of_TriangleMesh<ScalarType>   DirectedEdgeAttributeType;
    typedef CellAttribute_Of_TriangleMesh<ScalarType>           CellAttributeType;
};


template<typename MeshAttributeType>
class TriangleMesh : public PolygonMesh<MeshAttributeType>
{
public:
    typedef TriangleMesh<MeshAttributeType> MeshType;

    typedef MeshAttributeType MeshAttributeType;

    typedef int_max IndexType;

    typedef typename MeshAttributeType::ScalarType ScalarType;

    typedef typename MeshAttributeType::PointAttributeType          PointAttributeType;
    typedef typename MeshAttributeType::EdgeAttributeType           EdgeAttributeType;
    typedef typename MeshAttributeType::DirectedEdgeAttributeType   DirectedEdgeAttributeType;
    typedef typename MeshAttributeType::CellAttributeType           CellAttributeType;

    typedef Point_Of_SurfaceMesh<MeshAttributeType>           PointType;
    typedef Edge_Of_SurfaceMesh<MeshAttributeType>            EdgeType;
    typedef DirectedEdge_Of_SurfaceMesh<MeshAttributeType>    DirectedEdgeType;
    typedef Cell_Of_SurfaceMesh<MeshAttributeType>            CellType;

    typedef Handle_Of_Point_Of_SurfaceMesh          PointHandleType;
    typedef Handle_Of_Edge_Of_SurfaceMesh           EdgeHandleType;
    typedef Handle_Of_DirectedEdge_Of_SurfaceMesh   DirectedEdgeHandleType;
    typedef Handle_Of_Cell_Of_SurfaceMesh           CellHandleType;

    typedef Iterator_Of_Point_Of_SurfaceMesh<MeshAttributeType>         PointIteratorType;
    typedef Iterator_Of_Edge_Of_SurfaceMesh<MeshAttributeType>          EdgeIteratorType;
    typedef Iterator_Of_DirectedEdge_Of_SurfaceMesh<MeshAttributeType>  DirectedEdgeIteratorType;
    typedef Iterator_Of_Cell_Of_SurfaceMesh<MeshAttributeType>          CellIteratorType;

public:
    TriangleMesh();
    TriangleMesh(const TriangleMesh& InputMesh);
    TriangleMesh(TriangleMesh&& InputMesh);
    ~TriangleMesh();

    inline void operator=(const TriangleMesh& InputMesh);
    inline void operator=(TriangleMesh&& InputMesh);

    inline void Clear();

    inline void Copy(const TriangleMesh& InputMesh);
    inline bool Copy(const TriangleMesh* InputMesh);

    inline void Share(TriangleMesh& InputMesh);
    inline bool Share(TriangleMesh* InputMesh);

    inline void ForceShare(const TriangleMesh& InputMesh);
    inline bool ForceShare(const TriangleMesh* InputMesh);

    inline void Take(TriangleMesh&& InputMesh);
    inline bool Take(TriangleMesh& InputMesh);
    inline bool Take(TriangleMesh* InputMesh);

    //------------- Function optimized For TriangleMesh --------------------------------------------------//

    void UpdateNormalAtPoint();
    void UpdateNormalAtCell();
};

}// namespace mdk

#include "mdkTriangleMesh.hpp"

#endif