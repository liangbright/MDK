#ifndef mdk_GeodesicSphereBuilder_h
#define mdk_GeodesicSphereBuilder_h

#include "mdkTriangleMesh.h"

namespace mdk
{

template<typename Scalar_Type>
class GeodesicSphereBuilder : public Object
{
public:
	typedef Scalar_Type ScalarType; // double or float
	typedef TriangleMesh<TriangleMeshEmptyAttributeType<ScalarType>> MeshType;

	ObjectArray<MeshType> m_SphereList;
	int_max m_MaxDepth;

public:
	GeodesicSphereBuilder();
	~GeodesicSphereBuilder();
	void Clear();
	void SetMaxDepth(int_max MaxDepth);
	void Update();
	MeshType* GetSphereAtDepth(int_max Depth);
	MeshType* GetSphereAtMaxDepth();
	ObjectArray<MeshType>* GetSphereList();

private:
	void BuildInitialSphere();
	void BuildSphereAtDepth(int_max Depth);// triangle subdivision

private:
	GeodesicSphereBuilder(const GeodesicSphereBuilder&) = delete;
	void operator=(const GeodesicSphereBuilder&) = delete;
};

}//namespace mdk

#include "mdkGeodesicSphereBuilder.hpp"

#endif