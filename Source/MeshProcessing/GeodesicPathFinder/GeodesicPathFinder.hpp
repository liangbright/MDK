#pragma once

namespace aortic_valve
{

using namespace mdk;

template<typename ScalarType>
GeodesicPathFinder<ScalarType>::GeodesicPathFinder()
{
    this->Clear();
}


template<typename ScalarType>
GeodesicPathFinder<ScalarType>::~GeodesicPathFinder()
{
}
 
template<typename ScalarType>
void GeodesicPathFinder<ScalarType>::Clear()
{
	m_InputMesh = nullptr;
    m_InputMesh_copy.Clear();
	m_InputMeshForGeodesicPathAlgorithm.reset();
}

template<typename ScalarType>
void GeodesicPathFinder<ScalarType>::SetInputMesh(const TriangleMesh<ScalarType>* InputTriangleMesh)
{
	m_InputMesh = InputTriangleMesh;
}

template<typename ScalarType>
bool GeodesicPathFinder<ScalarType>::CheckInput()
{
	if (m_InputMesh == nullptr)
	{
		MDK_Error("Empty input mesh (nullptr) @ GeodesicPathFinder::CheckInput()")
		return false;
	}

    if (m_InputMesh->IsEmpty() == true)
    {
        MDK_Error("Empty input mesh @ GeodesicPathFinder::CheckInput()")
        return false;
    }

	if (m_InputMesh->CheckIfTriangleMesh() == false)
	{
		MDK_Error("input mesh is NOT triangle mesh @ GeodesicPathFinder::CheckInput()")
		return false;
	}

    return true;
}


template<typename ScalarType>
bool GeodesicPathFinder<ScalarType>::UpdateParameter()
{
    if (this->CheckInput() == false)
    {
        return false;
    }
	this->ConvertMeshForGeodesicPathAlgorithm();
    return true;
}


template<typename ScalarType>
void GeodesicPathFinder<ScalarType>::ConvertMeshForGeodesicPathAlgorithm()
{
    if (m_InputMesh->Check_If_DataStructure_is_Clean() == false)
    {
        m_InputMesh_copy.Construct(m_InputMesh->GetPointPositionMatrixAndFaceTable());
        m_InputMesh = &m_InputMesh_copy;
    }

	m_InputMeshForGeodesicPathAlgorithm = std::make_unique<geodesic::Mesh>();

    std::vector<double> points;
    std::vector<int_max> faces;

	auto PointCount = m_InputMesh->GetPointCount();
	auto TriangleCount = m_InputMesh->GetFaceCount();

	auto TablePair = m_InputMesh->GetPointPositionMatrixAndFaceTable();
    const auto& PointPositionMatrix = TablePair.first;
    const auto& TriangleTable = TablePair.second;

    points.resize(PointCount * 3);
    for (int_max k = 0; k < PointCount; ++k)
    {
        points[3 * k]     = PointPositionMatrix(0, k);
        points[3 * k + 1] = PointPositionMatrix(1, k);
        points[3 * k + 2] = PointPositionMatrix(2, k);
    }

    faces.resize(TriangleCount * 3);
    for (int_max k = 0; k < TriangleCount; ++k)
    {
        faces[3 * k]     = TriangleTable[k][0];
        faces[3 * k + 1] = TriangleTable[k][1];
        faces[3 * k + 2] = TriangleTable[k][2];
    }

	m_InputMeshForGeodesicPathAlgorithm->initialize_mesh_data(points, faces);
}

template<typename ScalarType>
DenseMatrix<ScalarType> GeodesicPathFinder<ScalarType>::
FindPath(const DenseVector<ScalarType, 3>& Position_start, const DenseVector<ScalarType, 3>& Position_end, const String& Method)
{
    GeodesicAlgorithmTypeEnum AlgorithmType;
    if (Method == "Exact")
    {
        AlgorithmType = GeodesicAlgorithmTypeEnum::Exact;
    }
    else if (Method == "Dijkstra")
    {
        AlgorithmType = GeodesicAlgorithmTypeEnum::Dijkstra;
    }
    else
    {
        MDK_Error("unkown method: " << Method)
    }
	auto PointIndex_start = this->FindNearestPointOnMesh(*m_InputMeshForGeodesicPathAlgorithm, Position_start);
	auto PointIndex_end = this->FindNearestPointOnMesh(*m_InputMeshForGeodesicPathAlgorithm, Position_end);
    auto Path = this->ComputeGeodesicPathOnMesh(PointIndex_start, PointIndex_end, AlgorithmType);
	if (Path.GetColCount() <= 0)
	{
		Path.FastResize(3, 2);
	}
	Path.SetCol(0, Position_start);
	Path.SetCol(Path.GetColCount() - 1, Position_end);
	return Path;
}

/*
template<typename ScalarType>
DenseMatrix<ScalarType> GeodesicPathFinder<ScalarType>::
InterpolateCurve(const DenseMatrix<ScalarType>& InputCurve, int_max LowerBound_PointCount)
{
    DenseMatrix<ScalarType> OutputCurve;

    if (InputCurve.IsEmpty() == true)
    {
        return OutputCurve;
    }

    OutputCurve = InputCurve;
    DenseMatrix<ScalarType> tempCurve;
    while (OutputCurve.GetColCount() < LowerBound_PointCount)
    {
        auto ColNumber_new = 2 * OutputCurve.GetColCount() - 1;
        tempCurve.FastResize(3, ColNumber_new);

        tempCurve.SetCol(0, OutputCurve.GetPointerOfCol(0));

        for (int_max k = 1; k < OutputCurve.GetColCount(); ++k)
        {
            auto Pos_prev = OutputCurve.GetPointerOfCol(k - 1);
            auto Pos_k = OutputCurve.GetPointerOfCol(k);

            ScalarType Pos[3];
            Pos[0] = (Pos_prev[0] + Pos_k[0]) / ScalarType(2);
            Pos[1] = (Pos_prev[1] + Pos_k[1]) / ScalarType(2);
            Pos[2] = (Pos_prev[2] + Pos_k[2]) / ScalarType(2);

            tempCurve.SetCol(2 * k - 1, Pos);
            tempCurve.SetCol(2 * k, Pos_k);
        }
        OutputCurve = std::move(tempCurve);
    }

    return OutputCurve;
}
*/

template<typename ScalarType>
int_max GeodesicPathFinder<ScalarType>::
FindNearestPointOnMesh(geodesic::Mesh& TargetMesh, const DenseVector<ScalarType, 3>& Position)
{
    std::vector<geodesic::Vertex>& PointList = TargetMesh.vertices();

    int_max OutputPointIndex = 0;

    ScalarType Distance_sq_min = 0;
    bool IsFirstPoint = true;
    for (int_max k = 0; k < int_max(PointList.size()); ++k)
    {
        auto x = ScalarType(PointList[k].x());
        auto y = ScalarType(PointList[k].y());
        auto z = ScalarType(PointList[k].z());

        auto Distance_sq = (x - Position[0])*(x - Position[0]) + (y - Position[1])*(y - Position[1]) + (z - Position[2])*(z - Position[2]);

        if (k == 0)
        {
            Distance_sq_min = Distance_sq;
            OutputPointIndex = 0;
        }
        else
        {
            if (Distance_sq < Distance_sq_min)
            {
                Distance_sq_min = Distance_sq;
                OutputPointIndex = k;
            }
        }
    }

    return OutputPointIndex;
}


template<typename ScalarType>
DenseMatrix<ScalarType> GeodesicPathFinder<ScalarType>::
ComputeGeodesicPathOnMesh(int_max PointIndex_start, int_max PointIndex_end, GeodesicAlgorithmTypeEnum AlgorithmType)
{
    // path : track back from target to source
	auto& TargetMesh = *m_InputMeshForGeodesicPathAlgorithm;

    std::vector<geodesic::SurfacePoint> path;
    geodesic::SurfacePoint source(&TargetMesh.vertices()[PointIndex_end]);
    geodesic::SurfacePoint target(&TargetMesh.vertices()[PointIndex_start]);

    if (AlgorithmType == GeodesicAlgorithmTypeEnum::Exact)
    {
        geodesic::GeodesicAlgorithmExact algorithm(&TargetMesh);
        algorithm.geodesic(source, target, path);
    }
    else if (AlgorithmType == GeodesicAlgorithmTypeEnum::Dijkstra)
    {
        geodesic::GeodesicAlgorithmDijkstra algorithm(&TargetMesh);
        algorithm.geodesic(source, target, path);
    }

    DenseMatrix<ScalarType> GeodesicPath(3, path.size());

    for (int_max i = 0; i < int_max(path.size()); ++i)
    {
        GeodesicPath(0, i) = ScalarType(path[i].x());
        GeodesicPath(1, i) = ScalarType(path[i].y());
        GeodesicPath(2, i) = ScalarType(path[i].z());
    }

    return GeodesicPath;
}

}// namespace aortic_valve
