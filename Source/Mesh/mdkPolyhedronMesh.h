#pragma once

#include "mdkMesh.h"

namespace mdk
{

template<typename Scalar_Type>
class PolyhedronMesh : public Mesh<Scalar_Type>
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
	PolyhedronMesh();
	PolyhedronMesh(const MDK_Symbol_PureEmpty&);
	PolyhedronMesh(const PolyhedronMesh& InputMesh);
	PolyhedronMesh(PolyhedronMesh&& InputMesh);
	PolyhedronMesh(const Mesh& InputMesh);
	PolyhedronMesh(Mesh&& InputMesh);
    ~PolyhedronMesh();
    
	inline void operator=(const PolyhedronMesh& InputMesh);
	inline void operator=(PolyhedronMesh&& InputMesh);
	inline void operator=(const Mesh& InputMesh);
	inline void operator=(Mesh&& InputMesh);
};

}// namespace mdk

#include "mdkPolyhedronMesh.hpp"
#include "mdkPolyhedronMesh_FileIO.h"

