#pragma once

#include "mdkPolyhedronMesh.h"
#Include "mdkTriangleMesh.h"

namespace mdk
{

template<typename Scalar_Type>
class TetrahedronMesh : public PolyhedronMesh<Scalar_Type>
{
public:
	typedef Scalar_Type ScalarType;
	//------------------------------------------------------------------------//
	typedef StandardAttribute_Of_Mesh<ScalarType>          MeshAttribute;
	typedef StandardAttribute_Of_Point_Of_Mesh<ScalarType> PointAttributeType;
	typedef StandardAttribute_Of_Edge_Of_Mesh<ScalarType>  EdgeAttributeType;
	typedef StandardAttribute_Of_Face_Of_Mesh<ScalarType>  FaceAttributeType;
	typedef StandardAttribute_Of_Cell_Of_Mesh<ScalarType>  CellAttributeType;
	//------------------------------------------------------------------------//
	typedef Point_Of_Mesh<ScalarType>  PointType;
	typedef Edge_Of_Mesh<ScalarType>   EdgeType;
	typedef Face_Of_Mesh<ScalarType>   FaceType;
	typedef Cell_Of_Mesh<ScalarType>   CellType;
	//------------------------------------------------------------------------//
	typedef StdObjectVector<PointType> PointListType;
	typedef StdObjectVector<EdgeType>  EdgeListType;
	typedef StdObjectVector<FaceType>  FaceListType;
	typedef StdObjectVector<CellType>  CellListType;
	//------------------------------------------------------------------------//

public:
	TetrahedronMesh();
	TetrahedronMesh(const MDK_Symbol_PureEmpty&);
	TetrahedronMesh(const TetrahedronMesh& InputMesh);
	TetrahedronMesh(TetrahedronMesh&& InputMesh);
	TetrahedronMesh(const PolyhedronMesh& InputMesh);
	TetrahedronMesh(PolyhedronMesh&& InputMesh);
	TetrahedronMesh(const Mesh& InputMesh);
	TetrahedronMesh(Mesh&& InputMesh);
    ~TetrahedronMesh();
    
	inline void operator=(const TetrahedronMesh& InputMesh);
	inline void operator=(TetrahedronMesh&& InputMesh);
	inline void operator=(const PolyhedronMesh& InputMesh);
	inline void operator=(PolyhedronMesh&& InputMesh);
	inline void operator=(const Mesh& InputMesh);
	inline void operator=(Mesh&& InputMesh);

	TriangleMesh<ScalarType> GetSubMeshByFace(const DenseVector<int_max>& FaceIndexList) const;
	TetrahedronMesh<ScalarType> GetSubMeshByCell(const DenseVector<int_max>& CellIndexList) const;
};

}// namespace mdk

#include "mdkTetrahedronMesh.hpp"

