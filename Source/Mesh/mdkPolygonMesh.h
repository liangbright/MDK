#pragma once

#include <tuple>
#include <unordered_map>

#include "mdkMesh.h"

namespace mdk
{

template<typename Scalar_Type>
class PolygonMesh : public Mesh<Scalar_Type>
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

protected:
	template<typename T>
	friend class Point_Of_Mesh;

	template<typename T>
	friend class Edge_Of_Mesh;

	template<typename T>
	friend class Face_Of_Mesh;

	template<typename T>
	friend class Cell_Of_Mesh;

public:
    PolygonMesh();
    PolygonMesh(const MDK_Symbol_PureEmpty&);
    PolygonMesh(const PolygonMesh& InputMesh);
    PolygonMesh(PolygonMesh&& InputMesh);
	PolygonMesh(const Mesh& InputMesh);
	PolygonMesh(Mesh&& InputMesh);
    ~PolygonMesh();

	inline void operator=(const PolygonMesh& InputMesh);
	inline void operator=(PolygonMesh&& InputMesh);
	inline void operator=(const Mesh& InputMesh);
	inline void operator=(Mesh&& InputMesh);

	PolygonMesh<ScalarType> GetSubMeshByFace(const DenseVector<int_max>& FaceIndexList) const;
};

}// namespace mdk

#include "mdkPolygonMesh.hpp"
#include "mdkPolygonMesh_FileIO.h"
