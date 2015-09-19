#ifndef mdk_DirectionGenerator3D_h
#define mdk_DirectionGenerator3D_h

#include <queue>

#include "mdkGeodesicSphereBuilder.h"

namespace mdk
{

template<typename ScalarType>
struct Mask_Of_DirectionGenerator3D
{
	int_max Level;
	int_max Index;
	DenseVector<ScalarType, 3> Direction;
	DenseVector<int_max> MaskIndexListAtNextLevel;

	bool Flag;
	// true: have been processed
	// false: not yet
};

template<typename Scalar_Type>
class DirectionGenerator3D :public Object
{
public:
	typedef Scalar_Type ScalarType;

private:
	ObjectArray<ObjectArray<Mask_Of_DirectionGenerator3D<ScalarType>>> m_MaskList;  // m_MaskList[k] is MaskList at Level k
	GeodesicSphereBuilder<ScalarType> m_SphereBuilder;
	int_max m_SphereResolution;	// PointCount of SphereMesh from m_SphereBuilder
	// 20, 42, 162, 642, 2562, 10242, 40962

public:
	DirectionGenerator3D();
	~DirectionGenerator3D();
	void Clear();
	void SetSphereResolution(ScalarType Resolution);
	void BuildMask();
	
	// Output will not include Direction_mean
	// DirectionDotProduct_min is the min dot product between candidate direction and mean direction
	DenseMatrix<ScalarType> FindDirectionCandidate(const DenseVector<ScalarType, 3> Direction_mean, ScalarType DirectionDotProduct_min, int_max Resolution);

private:
	bool CheckInput();
	void InitializeMaskList();
	void BuildMaskLink(int_max Level, int_max MaskIndex);
	int_max GetDepthByResolution(int_max Resolution);

private:
	DirectionGenerator3D(const DirectionGenerator3D&) = delete;
	void operator=(const DirectionGenerator3D&) = delete;
};

}//namespace mdk

#include "mdkDirectionGenerator3D.hpp"

#endif