#ifndef __mdkTriangleMesh_h
#define __mdkTriangleMesh_h


#include "mdkPolygonMesh.h"

namespace mdk
{

template<typename ScalarType = double>
class TriangleMesh : private PolygonMesh<ScalarType>
{
public:
    typedef ScalarType ScalarType;

    typedef int_max IndexType;

public:
    TriangleMesh();

    TriangleMesh(const TriangleMesh& InputMesh);

    TriangleMesh(TriangleMesh&& InputMesh);

    ~TriangleMesh();

    inline void operator=(const TriangleMesh& InputMesh);

    inline void operator=(TriangleMesh&& InputMesh);

    //---------------------------------------------------------------------------

    bool Construct(DenseMatrix<ScalarType> InputVertexPositionTable, DataArray<DenseVector<int_max>> InputVertexIndexTable);

    //---------------------------------------------------------------------------

    inline void Clear();

    template<typename ScalarType_Input>
    inline void Copy(const TriangleMesh<ScalarType_Input>& InputMesh);

    template<typename ScalarType_Input>
    inline bool Copy(const TriangleMesh<ScalarType_Input>* InputMesh);

    inline void Share(TriangleMesh& InputMesh);

    inline bool Share(TriangleMesh* InputMesh);

    inline void ForceShare(const TriangleMesh& InputMesh);

    inline bool ForceShare(const TriangleMesh* InputMesh);

    inline void Take(TriangleMesh&& InputMesh);

    inline bool Take(TriangleMesh& InputMesh);

    inline bool Take(TriangleMesh* InputMesh);
};

}// namespace mdk

#include "mdkTriangleMesh.hpp"

#endif