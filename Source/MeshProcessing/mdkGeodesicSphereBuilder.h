#pragma once

#include "mdkTriangleMesh.h"

namespace mdk
{

template<typename Scalar_Type>
class GeodesicSphereBuilder : public Object
{
public:
	typedef Scalar_Type ScalarType; // double or float
	typedef TriangleMesh<TriangleMeshEmptyAttributeType<ScalarType>> MeshType;

	ObjectArray<MeshType> m_SphereList; // L2Norm of each vector ((0,0,0) to Point) is 1
	int_max m_MaxDepth;//Length-1 of m_SphereList

	// Depth:              1,  2,   3,   4,    5,     5,     7
	// SphereResolution:  20, 42, 162, 642, 2562, 10242, 40962

public:
	GeodesicSphereBuilder();
	~GeodesicSphereBuilder();
	void Clear();
	void SetMaxDepth(int_max MaxDepth);
	void Update();
	int_max GetMaxDepth() {	return m_MaxDepth;	}
	ObjectArray<MeshType>& OutputSphereList() { return m_SphereList;}

private:
	void BuildInitialSphere();
	void BuildSphereAtDepth(int_max Depth);// triangle subdivision

private:
	GeodesicSphereBuilder(const GeodesicSphereBuilder&) = delete;
	void operator=(const GeodesicSphereBuilder&) = delete;
};

}//namespace mdk

#include "mdkGeodesicSphereBuilder.hpp"
