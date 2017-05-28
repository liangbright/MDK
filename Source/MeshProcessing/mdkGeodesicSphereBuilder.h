#pragma once

#include "mdkTriangleMesh.h"

namespace mdk
{

struct Input_of_GeodesicSphereBuilder
{
	int_max MaxDepth;//Length-1 of SphereList
};

template<typename MeshType>
struct Output_of_GeodesicSphereBuilder
{
	ObjectArray<MeshType> SphereList; // L2Norm of each vector ((0,0,0) to Point) is 1
   // Depth:              1,  2,   3,   4,    5,     5,     7
   // SphereResolution:  20, 42, 162, 642, 2562, 10242, 40962
};

template<typename Scalar_Type>
class GeodesicSphereBuilder : public Object
{
public:
	typedef Scalar_Type ScalarType; // double or float
	typedef TriangleMesh<TriangleMeshEmptyAttributeType<ScalarType>> MeshType;   	
	Input_of_GeodesicSphereBuilder Input;
	Output_of_GeodesicSphereBuilder<MeshType> Output;	
public:
	GeodesicSphereBuilder();
	~GeodesicSphereBuilder();
	void Clear();
	void Update();

private:
	void BuildInitialSphere();
	void BuildSphereAtDepth(int_max Depth);// triangle subdivision

private:
	GeodesicSphereBuilder(const GeodesicSphereBuilder&) = delete;
	void operator=(const GeodesicSphereBuilder&) = delete;
};

}//namespace mdk

#include "mdkGeodesicSphereBuilder.hpp"
