#pragma once

#include "vtkLinearSubdivisionFilter.h"

#include "geodesic_algorithm_dijkstra.h"
#include "geodesic_algorithm_subdivision.h"
#include "geodesic_algorithm_exact.h"

#include "mdkPolygonMeshProcessing.h"

namespace aortic_valve
{

using namespace mdk;

template<typename Scalar_Type>
class GeodesicPathFinder
{
public:
	typedef Scalar_Type ScalarType;

	enum GeodesicAlgorithm_TypeEnum
	{
		Exact,
		Dijkstra
	};

public:
	typedef GeodesicAlgorithm_TypeEnum GeodesicAlgorithmTypeEnum;

protected:
	const TriangleMesh<ScalarType>* m_InputMesh;
    std::unique_ptr<geodesic::Mesh> m_InputMeshForGeodesicPathAlgorithm;
	TriangleMesh<ScalarType> m_InputMesh_copy;//If InputMesh DataStructure is not Clean, then create a copy

public:
    GeodesicPathFinder();
    ~GeodesicPathFinder();

    void Clear();

	void SetInputMesh(const TriangleMesh<ScalarType>* InputMesh);

    bool UpdateParameter();

	DenseMatrix<ScalarType> FindPath(const DenseVector<ScalarType, 3>& Position_start, 
									 const DenseVector<ScalarType, 3>& Position_end,
									 const String& Method);//Dijkstra or Exact

	//DenseMatrix<ScalarType> InterpolateCurve(const DenseMatrix<ScalarType>& PointPositionMatrixOfCurve, int_max LowerBound_PointNumber);

private:
	bool CheckInput();
    void ConvertMeshForGeodesicPathAlgorithm();

    int_max FindNearestPointOnMesh(geodesic::Mesh& LeafletMesh, const DenseVector<ScalarType, 3>& Position);

    DenseMatrix<ScalarType> ComputeGeodesicPathOnMesh(int_max PointIndex_start,
                                                      int_max PointIndex_end,
                                                      GeodesicAlgorithmTypeEnum AlgorithmType = GeodesicAlgorithmTypeEnum::Exact);

private:
    GeodesicPathFinder(const GeodesicPathFinder&) = delete;
    void operator=(const GeodesicPathFinder&) = delete;
};


}// namespace aortic_valve

#include "GeodesicPathFinder.hpp"
