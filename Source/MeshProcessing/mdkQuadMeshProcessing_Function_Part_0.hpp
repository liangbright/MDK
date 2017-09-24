#pragma once

namespace mdk
{

template<typename ScalarType>
PolygonMesh<ScalarType> SubdivideQuadMesh_Linear(const PolygonMesh<ScalarType>& InputMesh)
{
	PolygonMesh<ScalarType> OutputMesh;
	
	auto PointCount_input = InputMesh.GetPointCount();
	auto EdgeCount_input = InputMesh.GetEdgeCount();
	auto FaceCount_input = InputMesh.GetFaceCount();
	auto PointCount = PointCount_input + EdgeCount_input + FaceCount_input;
	auto EdgeCount = EdgeCount_input * 2 + FaceCount_input * 4;
	auto FaceCount = FaceCount_input * 4;
	OutputMesh.SetCapacity(PointCount, EdgeCount, FaceCount);

	//------- add initial point by copying all point of InputMesh ----------------//
	DenseVector<int_max> PointIndexMap_init;//Map Input Point to Output Point
	PointIndexMap_init.Resize(PointCount_input + InputMesh.GetDeletedPointCount());
	PointIndexMap_init.Fill(-1);
	for (int_max k = 0; k <= InputMesh.GetMaxValueOfPointIndex(); ++k)
	{
		if (InputMesh.IsValidPointIndex(k) == true)
		{
			auto Pos = InputMesh.GetPointPosition(k);
			PointIndexMap_init[k] = OutputMesh.AddPoint(Pos);			 
		}
	}
	//------- add new point by splitting each edge of InputMesh -----------------//   
	DenseVector<int_max> PointIndexMap_new;//Map Input Edge (new point location) to Output Point
	PointIndexMap_new.Resize(EdgeCount_input + InputMesh.GetDeletedEdgeCount());
	PointIndexMap_new.Fill(-1);	
	for (int_max k = 0; k <= InputMesh.GetMaxValueOfEdgeIndex(); ++k)
	{
		if (InputMesh.IsValidEdgeIndex(k) == true)
		{
			auto TempList = InputMesh.Edge(k).GetPointIndexList();
			auto P0 = InputMesh.GetPointPosition(TempList[0]);
			auto P1 = InputMesh.GetPointPosition(TempList[1]);
			auto P3 = P0 + P1;
			P3 /= ScalarType(2);
			PointIndexMap_new[k] = OutputMesh.AddPoint(P3);			
		}
	}

	//------- add new cell by splitting each cell of InputMesh and add center point ----------------//   
	for (int_max k = 0; k <= InputMesh.GetMaxValueOfFaceIndex(); ++k)
	{
		if (InputMesh.IsValidFaceIndex(k) == true)
		{
			auto PointIndexList_input = InputMesh.Face(k).GetPointIndexList(); // P0, P1, P2, P3
			auto EdgeIndexList_input = InputMesh.Face(k).GetEdgeIndexList();   // P0-P1, P1-P2, P2-P3, P3-P0
			if (PointIndexList_input.GetLength() != 4)
			{
				MDK_Error("Input is NOT QuadMesh @ SubdivideQuadMesh_Linear(...)")
				OutputMesh.Clear();
				return OutputMesh;
			}
			//-----------------
			// 3    6     2 
			// 7    8     5   
			// 0    4     1
			//-----------------		
			auto H0 = PointIndexMap_init[PointIndexList_input[0]];
			auto H1 = PointIndexMap_init[PointIndexList_input[1]];
			auto H2 = PointIndexMap_init[PointIndexList_input[2]];
			auto H3 = PointIndexMap_init[PointIndexList_input[3]];
			auto H4 = PointIndexMap_new[EdgeIndexList_input[0]];
			auto H5 = PointIndexMap_new[EdgeIndexList_input[1]];
			auto H6 = PointIndexMap_new[EdgeIndexList_input[2]];
			auto H7 = PointIndexMap_new[EdgeIndexList_input[3]];

			DenseVector<ScalarType, 3> Point8;
			{
				auto Point0 = OutputMesh.GetPointPosition(H0);
				auto Point1 = OutputMesh.GetPointPosition(H1);
				auto Point2 = OutputMesh.GetPointPosition(H2);
				auto Point3 = OutputMesh.GetPointPosition(H3);
				auto L02 = (Point2 - Point0).L2Norm();
				auto L13 = (Point3 - Point1).L2Norm();
				if (L02 < L13)
				{
					Point8 = (Point2 + Point0) / ScalarType(2);
				}
				else
				{
					Point8 = (Point3 + Point1) / ScalarType(2);
				}
			}
			auto H8 = OutputMesh.AddPoint(Point8);

			OutputMesh.AddFaceByPoint({ H8, H6, H3, H7 });
			OutputMesh.AddFaceByPoint({ H8, H7, H0, H4 });
			OutputMesh.AddFaceByPoint({ H8, H4, H1, H5 });
			OutputMesh.AddFaceByPoint({ H8, H5, H2, H6 });
		}
	}
	//--------------------------------------------------------------------------//
	return OutputMesh;
}


template<typename ScalarType>
PolygonMesh<ScalarType> SubdivideQuadMesh_Linear(const PolygonMesh<ScalarType>& InputMesh, int_max SubdivisionCount)
{
	PolygonMesh<ScalarType> OutputMesh;
	if (SubdivisionCount <= 0)
	{
		MDK_Error(" SubdivisionCount <=0 @ SubdivideQuadMesh_Linear(...,...)")
	}
	else
	{
		OutputMesh = SubdivideQuadMesh_Linear(InputMesh);
		for (int_max k = 1; k < SubdivisionCount; ++k)//k from 1
		{
			OutputMesh = SubdivideQuadMesh_Linear(OutputMesh);
		}
	}
	return OutputMesh;
}

template<typename ScalarType>
TriangleMesh<ScalarType> ConvertQuadMeshToTriangleMesh(const PolygonMesh<ScalarType>& InputMesh, const std::string& Method)
{
	return ConvertMixedTriangleQuadMeshToTriangleMesh(InputMesh);
}

template<typename ScalarType>
TriangleMesh<ScalarType> ConvertMixedTriangleQuadMeshToTriangleMesh(const PolygonMesh<ScalarType>& InputMesh, const std::string& Method)
{
	if (Method == "1to2")
	{
		return ConvertMixedTriangleQuadMeshToTriangleMesh_1to2(InputMesh);
	}
	else if (Method == "1to8")
	{
		return ConvertMixedTriangleQuadMeshToTriangleMesh_1to8(InputMesh);
	}
	else
	{
		MDK_Error("Method is unknown @ ConvertMixedTriangleQuadMeshToTriangleMesh(...)")
		TriangleMesh<ScalarType> EmptyMesh;
		return EmptyMesh;
	}
}


template<typename ScalarType>
TriangleMesh<ScalarType> ConvertMixedTriangleQuadMeshToTriangleMesh_1to2(const PolygonMesh<ScalarType>& InputMesh)
{//Input: Quad or Mixed-Quad-Triangle

	TriangleMesh<ScalarType> OutputMesh;

	auto PointCount_input = InputMesh.GetPointCount();
	auto EdgeCount_input = InputMesh.GetEdgeCount();
	auto FaceCount_input = InputMesh.GetFaceCount();
	auto PointCount = PointCount_input;
	auto EdgeCount = EdgeCount_input + FaceCount_input;
	auto FaceCount = FaceCount_input * 2;
	OutputMesh.SetCapacity(PointCount, EdgeCount, FaceCount);

	//------- copy all point of InputMesh ----------------//
	DenseVector<int_max> PointIndexMap;//Map Input Point to Output Point
	PointIndexMap.Resize(PointCount_input + InputMesh.GetDeletedPointCount());
	PointIndexMap.Fill(-1);
	for (int_max k = 0; k <= InputMesh.GetMaxValueOfPointIndex(); ++k)
	{
		if (InputMesh.IsValidPointIndex(k) == true)
		{
			auto Pos = InputMesh.GetPointPosition(k);
			PointIndexMap[k] = OutputMesh.AddPoint(Pos);
		}
	}

	//------- add new face by splitting each face of InputMesh ----------------//   
	for (int_max k = 0; k <= InputMesh.GetMaxValueOfFaceIndex(); ++k)
	{
		if (InputMesh.IsValidFaceIndex(k) == true)
		{
			auto PointIndexList_input = InputMesh.Face(k).GetPointIndexList(); // P0, P1, P2, P3
			auto EdgeIndexList_input = InputMesh.Face(k).GetEdgeIndexList();   // P0-P1, P1-P2, P2-P3, P3-P0
			if (PointIndexList_input.GetLength() > 4)
			{
				MDK_Error("Input is NOT QuadMesh or Mixed_Triangle_Quad @ ConvertMixedTriangleQuadMeshToTriangleMesh_1to2(...)")
				return OutputMesh;
			}
			else if (PointIndexList_input.GetLength() == 4)
			{
				//-----------------
				// 3    2 
				//  
				// 0    1
				//-----------------
				auto H0 = PointIndexMap[PointIndexList_input[0]];
				auto H1 = PointIndexMap[PointIndexList_input[1]];
				auto H2 = PointIndexMap[PointIndexList_input[2]];
				auto H3 = PointIndexMap[PointIndexList_input[3]];
				auto Point0 = OutputMesh.GetPointPosition(H0);
				auto Point1 = OutputMesh.GetPointPosition(H1);
				auto Point2 = OutputMesh.GetPointPosition(H2);
				auto Point3 = OutputMesh.GetPointPosition(H3);

				auto L02 = (Point2 - Point0).L2Norm();
				auto L13 = (Point3 - Point1).L2Norm();
				if (L02 < L13)// this is good
				{
					OutputMesh.AddFaceByPoint(H0, H1, H2);
					OutputMesh.AddFaceByPoint(H0, H2, H3);
				}
				else
				{
					OutputMesh.AddFaceByPoint(H0, H1, H3);
					OutputMesh.AddFaceByPoint(H1, H2, H3);
				}

			}
			else if (PointIndexList_input.GetLength() == 3)
			{
				auto H0 = PointIndexMap[PointIndexList_input[0]];
				auto H1 = PointIndexMap[PointIndexList_input[1]];
				auto H2 = PointIndexMap[PointIndexList_input[2]];
				OutputMesh.AddFaceByPoint(H0, H1, H2);
			}
			else
			{
				MDK_Error("Input is NOT QuadMesh or Mixed_Triangle_Quad @ ConvertMixedTriangleQuadMeshToTriangleMesh_1to2(...)")
				return OutputMesh;
			}
		}
	}

	//-----------------------------------------------------------
	int_max NamedPointCount = InputMesh.GetNamedPointCount();
	if (NamedPointCount > 0)
	{
		auto PointNameList = InputMesh.GetValidPointNameList();
		for (int_max k = 0; k < PointNameList.GetLength(); ++k)
		{
			auto PointIndex = InputMesh.GetPointIndexByName(PointNameList[k]);
			OutputMesh.Point(PointIndexMap[PointIndex]).SetName(PointNameList[k]);
		}
	}
	//----------------------------------------------------------
	auto PointSetCount = InputMesh.GetPointSetCount();
	if (PointSetCount > 0)
	{		
		for (int_max PointSetIndex = 0; PointSetIndex < PointSetCount; ++PointSetIndex)
		{
			auto PointSet = InputMesh.GetPointSet(PointSetIndex);
			auto Name = InputMesh.GetPointSetName(PointSetIndex);
			for (int_max k = 0; k < PointSet.GetLength(); ++k)
			{
				PointSet[k] = PointIndexMap[PointSet[k]];
			}
			OutputMesh.SetPointSet(Name, PointSet);
		}
	}
	//----------------------------------------------------------	
	auto PointDataSetCount = InputMesh.GetPointDataSetCount();
	if (PointDataSetCount > 0)
	{
		DenseVector<int_max> PointIndexMap_inverse;//out->in
		PointIndexMap_inverse.Resize(OutputMesh.GetPointCount());
		for (int_max k = 0; k < PointIndexMap.GetLength(); ++k)
		{
			if (PointIndexMap[k] >= 0)
			{
				PointIndexMap_inverse[PointIndexMap[k]] = PointIndexMap[k];
			}
		}

		for (int_max PointDataSetIndex = 0; PointDataSetIndex < PointDataSetCount; ++PointDataSetIndex)
		{
			auto Name = InputMesh.GetPointDataSetName(PointDataSetIndex);
			auto DataSet = InputMesh.GetPointDataSet(PointDataSetIndex);//input data structure may not be clean
			DataSet = DataSet.GetSubMatrix(ALL, PointIndexMap_inverse);
			OutputMesh.SetPointDataSet(Name, DataSet);			
		}
	}

	return OutputMesh;
}


template<typename ScalarType>
TriangleMesh<ScalarType> ConvertMixedTriangleQuadMeshToTriangleMesh_1to8(const PolygonMesh<ScalarType>& InputMesh)
{//Input: Quad or Mixed-Quad-Triangle

	TriangleMesh<ScalarType> OutputMesh;

	auto PointCount_input = InputMesh.GetPointCount();
	auto EdgeCount_input = InputMesh.GetEdgeCount();
	auto FaceCount_input = InputMesh.GetFaceCount();
	auto PointCount = PointCount_input + EdgeCount_input + FaceCount_input;
	auto EdgeCount = EdgeCount_input*2 + FaceCount_input*8;
	auto FaceCount = FaceCount_input*8;
	OutputMesh.SetCapacity(PointCount, EdgeCount, FaceCount);

	//------- add initial point by copying all point of InputMesh ----------------//
	DenseVector<int_max> PointIndexMap_init;//Map Input Point to Output Point
	PointIndexMap_init.Resize(PointCount_input + InputMesh.GetDeletedPointCount());
	PointIndexMap_init.Fill(-1);
	for (int_max k = 0; k <= InputMesh.GetMaxValueOfPointIndex(); ++k)
	{
		if (InputMesh.IsValidPointIndex(k) == true)
		{
			auto Pos = InputMesh.GetPointPosition(k);
			PointIndexMap_init[k] = OutputMesh.AddPoint(Pos);
		}
	}

	//------- add new point by splitting each edge of InputMesh -----------------//   
	DenseVector<int_max> PointIndexMap_new;//Map Input Edge(new point location) to Output Point
	PointIndexMap_new.Resize(EdgeCount_input + InputMesh.GetDeletedEdgeCount());
	PointIndexMap_new.Fill(-1);
	for (int_max k = 0; k <= InputMesh.GetMaxValueOfEdgeIndex(); ++k)
	{
		if (InputMesh.IsValidEdgeIndex(k) == true)
		{
			auto TempList = InputMesh.Edge(k).GetPointIndexList();
			auto P0 = InputMesh.GetPointPosition(TempList[0]);
			auto P1 = InputMesh.GetPointPosition(TempList[1]);
			auto P3 = P0 + P1;
			P3 /= ScalarType(2);
			PointIndexMap_new[k] = OutputMesh.AddPoint(P3);
		}
	}
	//------- add new face by splitting each face of InputMesh ----------------//   
	for (int_max k = 0; k <= InputMesh.GetMaxValueOfFaceIndex(); ++k)
	{
		if (InputMesh.IsValidFaceIndex(k) == true)
		{
			auto PointIndexList_input = InputMesh.Face(k).GetPointIndexList(); // P0, P1, P2, P3
			auto EdgeIndexList_input = InputMesh.Face(k).GetEdgeIndexList();   // P0-P1, P1-P2, P2-P3, P3-P0
			if (PointIndexList_input.GetLength() > 4)
			{
				MDK_Error("Input is NOT QuadMesh or Mixed Quad-Triangle @ ConvertMixedTriangleQuadMeshToTriangleMesh_1to8(...)")
				return OutputMesh;
			}
			else if (PointIndexList_input.GetLength() == 4)
			{
				//-----------------
				// 3    6     2 
				// 7    8     5   
				// 0    4     1
				//-----------------		
				auto H0 = PointIndexMap_init[PointIndexList_input[0]];
				auto H1 = PointIndexMap_init[PointIndexList_input[1]];
				auto H2 = PointIndexMap_init[PointIndexList_input[2]];
				auto H3 = PointIndexMap_init[PointIndexList_input[3]];
				auto H4 = PointIndexMap_new[EdgeIndexList_input[0]];
				auto H5 = PointIndexMap_new[EdgeIndexList_input[1]];
				auto H6 = PointIndexMap_new[EdgeIndexList_input[2]];
				auto H7 = PointIndexMap_new[EdgeIndexList_input[3]];

				DenseVector<ScalarType, 3> Point8;
				{
					auto Point0 = OutputMesh.GetPointPosition(H0);
					auto Point1 = OutputMesh.GetPointPosition(H1);
					auto Point2 = OutputMesh.GetPointPosition(H2);
					auto Point3 = OutputMesh.GetPointPosition(H3);
					auto L02 = (Point2 - Point0).L2Norm();
					auto L13 = (Point3 - Point1).L2Norm();
					if (L02 < L13)
					{
						Point8 = (Point2 + Point0) / ScalarType(2);
					}
					else
					{
						Point8 = (Point3 + Point1) / ScalarType(2);
					}
				}
				auto H8 = OutputMesh.AddPoint(Point8);

				OutputMesh.AddFaceByPoint(H8, H6, H3);
				OutputMesh.AddFaceByPoint(H8, H3, H7);
				OutputMesh.AddFaceByPoint(H8, H7, H0);
				OutputMesh.AddFaceByPoint(H8, H0, H4);
				OutputMesh.AddFaceByPoint(H8, H4, H1);
				OutputMesh.AddFaceByPoint(H8, H1, H5);
				OutputMesh.AddFaceByPoint(H8, H5, H2);
				OutputMesh.AddFaceByPoint(H8, H2, H6);
			}
			else if (PointIndexList_input.GetLength() == 3)
			{
				//-----------------
				//            2 
				//      5     4
				// 0    3     1
				//-----------------		

				auto H0 = PointIndexMap_init[PointIndexList_input[0]];
				auto H1 = PointIndexMap_init[PointIndexList_input[1]];
				auto H2 = PointIndexMap_init[PointIndexList_input[2]];
				auto H3 = PointIndexMap_new[EdgeIndexList_input[0]];
				auto H4 = PointIndexMap_new[EdgeIndexList_input[1]];
				auto H5 = PointIndexMap_new[EdgeIndexList_input[2]];

				OutputMesh.AddFaceByPoint(H5, H0, H3);
				OutputMesh.AddFaceByPoint(H5, H3, H1);
				OutputMesh.AddFaceByPoint(H5, H1, H4);
				OutputMesh.AddFaceByPoint(H5, H4, H2);
			}
			else
			{
				MDK_Error("Input is NOT QuadMesh or Mixed Quad-Triangle @ ConvertMixedTriangleQuadMeshToTriangleMesh_1to8(...)")
				return OutputMesh;
			}
		}
	}
	//-----------------------------------------------------------
	int_max NamedPointCount = InputMesh.GetNamedPointCount();
	if (NamedPointCount > 0)
	{
		auto PointNameList = InputMesh.GetValidPointNameList();
		for (int_max k = 0; k < PointNameList.GetLength(); ++k)
		{
			auto PointIndex = InputMesh.GetPointIndexByName(PointNameList[k]);
			OutputMesh.Point(PointIndex).SetName(PointNameList[k]);
		}
	}
	//-----------------------------------------------------------
	return OutputMesh;
}


template<typename ScalarType>
PolygonMesh<ScalarType> SubdivideMixedTriangleQuadMeshToQuadMesh_Linear(const PolygonMesh<ScalarType>& InputMesh)
{
	PolygonMesh<ScalarType> OutputMesh;

	auto PointCount_input = InputMesh.GetPointCount();
	auto EdgeCount_input = InputMesh.GetEdgeCount();
	auto FaceCount_input = InputMesh.GetFaceCount();
	auto PointCount = PointCount_input + EdgeCount_input + FaceCount_input;
	auto EdgeCount = EdgeCount_input * 2 + FaceCount_input * 4;
	auto FaceCount = FaceCount_input * 4;
	OutputMesh.SetCapacity(PointCount, EdgeCount, FaceCount);

	//------- add initial point by copying all point of InputMesh ----------------//
	DenseVector<int_max> PointIndexMap_init;//Map Input Point to Output Point
	PointIndexMap_init.Resize(PointCount_input + InputMesh.GetDeletedPointCount());
	PointIndexMap_init.Fill(-1);
	for (int_max k = 0; k <= InputMesh.GetMaxValueOfPointIndex(); ++k)
	{
		if (InputMesh.IsValidPointIndex(k) == true)
		{
			auto Pos = InputMesh.GetPointPosition(k);
			PointIndexMap_init[k] = OutputMesh.AddPoint(Pos);
		}
	}
	//------- add new point by splitting each edge of InputMesh -----------------//   
	DenseVector<int_max> PointIndexMap_new;//Map Input Edge(new point location) to Output Point
	PointIndexMap_new.Resize(EdgeCount_input + InputMesh.GetDeletedEdgeCount());
	PointIndexMap_new.Fill(-1);
	for (int_max k = 0; k <= InputMesh.GetMaxValueOfEdgeIndex(); ++k)
	{
		if (InputMesh.IsValidEdgeIndex(k) == true)
		{
			auto TempList = InputMesh.Edge(k).GetPointIndexList();
			auto P0 = InputMesh.GetPointPosition(TempList[0]);
			auto P1 = InputMesh.GetPointPosition(TempList[1]);
			auto P3 = P0 + P1;
			P3 /= ScalarType(2);
			PointIndexMap_new[k] = OutputMesh.AddPoint(P3);			
		}
	}
	//------- add new cell by splitting each cell of InputMesh and add center point ----------------//   
	for (int_max k = 0; k <= InputMesh.GetMaxValueOfFaceIndex(); ++k)
	{
		if (InputMesh.IsValidFaceIndex(k) == true)
		{
			auto PointIndexList_input = InputMesh.Face(k).GetPointIndexList(); // P0, P1, P2, P3
			auto EdgeIndexList_input = InputMesh.Face(k).GetEdgeIndexList();   // P0-P1, P1-P2, P2-P3, P3-P0

			if (PointIndexList_input.GetLength() == 4)
			{
				//-----------------
				// 3    6     2 
				// 7    8     5   
				// 0    4     1
				//-----------------		
				auto H0 = PointIndexMap_init[PointIndexList_input[0]];
				auto H1 = PointIndexMap_init[PointIndexList_input[1]];
				auto H2 = PointIndexMap_init[PointIndexList_input[2]];
				auto H3 = PointIndexMap_init[PointIndexList_input[3]];
				auto H4 = PointIndexMap_new[EdgeIndexList_input[0]];
				auto H5 = PointIndexMap_new[EdgeIndexList_input[1]];
				auto H6 = PointIndexMap_new[EdgeIndexList_input[2]];
				auto H7 = PointIndexMap_new[EdgeIndexList_input[3]];

				DenseVector<ScalarType, 3> Point8;
				{
					auto Point0 = OutputMesh.GetPointPosition(H0);
					auto Point1 = OutputMesh.GetPointPosition(H1);
					auto Point2 = OutputMesh.GetPointPosition(H2);
					auto Point3 = OutputMesh.GetPointPosition(H3);
					auto L02 = (Point2 - Point0).L2Norm();
					auto L13 = (Point3 - Point1).L2Norm();
					if (L02 < L13)// this is good
					{
						Point8 = (Point2 + Point0) / ScalarType(2);
					}
					else
					{
						Point8 = (Point3 + Point1) / ScalarType(2);
					}
				}
				auto H8 = OutputMesh.AddPoint(Point8);

				OutputMesh.AddFaceByPoint({ H8, H6, H3, H7 });
				OutputMesh.AddFaceByPoint({ H8, H7, H0, H4 });
				OutputMesh.AddFaceByPoint({ H8, H4, H1, H5 });
				OutputMesh.AddFaceByPoint({ H8, H5, H2, H6 });
			}
			else if (PointIndexList_input.GetLength() == 3)
			{
				//-----------------
				//      0
				//   3     5			
				//      6
				// 1    4     2
				//-----------------		
				auto H0 = PointIndexMap_init[PointIndexList_input[0]];
				auto H1 = PointIndexMap_init[PointIndexList_input[1]];
				auto H2 = PointIndexMap_init[PointIndexList_input[2]];
				auto H3 = PointIndexMap_new[EdgeIndexList_input[0]];
				auto H4 = PointIndexMap_new[EdgeIndexList_input[1]];
				auto H5 = PointIndexMap_new[EdgeIndexList_input[2]];

				auto Point0 = OutputMesh.GetPointPosition(H0);
				auto Point1 = OutputMesh.GetPointPosition(H1);
				auto Point2 = OutputMesh.GetPointPosition(H2);
				auto Point6 = (Point0 + Point1 + Point2) / ScalarType(3);
				auto H6 = OutputMesh.AddPoint(Point6);

				OutputMesh.AddFaceByPoint({ H6, H5, H0, H3 });
				OutputMesh.AddFaceByPoint({ H6, H3, H1, H4 });
				OutputMesh.AddFaceByPoint({ H6, H4, H2, H5 });
			}
			else
			{
				MDK_Error("Input is NOT MixedTriangleQuadMesh @ SubdivideMixedTriangleQuadMeshToQuadMesh(...)")
				OutputMesh.Clear();
				return OutputMesh;
			}
		}
	}
	//--------------------------------------------------------------------------//
	return OutputMesh;
}


template<typename ScalarType>
PolygonMesh<ScalarType> SubdivideMixedTriangleQuadMeshToQuadMesh_Linear(const PolygonMesh<ScalarType>& InputMesh, int_max SubdivisionCount)
{
	PolygonMesh<ScalarType> OutputMesh;
	if (SubdivisionCount <= 0)
	{
		MDK_Error(" SubdivisionCount <=0 @ SubdivideMixedTriangleQuadMeshToQuadMesh_Linear(...,...)")
	}
	else
	{
		OutputMesh = SubdivideMixedTriangleQuadMeshToQuadMesh_Linear(InputMesh);
		for (int_max k = 1; k < SubdivisionCount; ++k)//k from 1
		{
			OutputMesh = SubdivideMixedTriangleQuadMeshToQuadMesh_Linear(OutputMesh);
		}
	}
	return OutputMesh;
}


template<typename ScalarType>
PolygonMesh<ScalarType> CreateQuadMeshOfRectangularFlatSurface(int_max PointCount_x, int_max PointCount_y, double Spacing_x, double Spacing_y)
{
	PolygonMesh<ScalarType> OutputMesh;

	if (PointCount_x <= 0 || PointCount_y <= 0)
	{
		MDK_Error("invalid input @ CreateQuadMeshOfRectangularFlatSurface(...)")
		return OutputMesh;
	}
	int_max PointCount = PointCount_x*PointCount_y;
	OutputMesh.SetCapacity(PointCount, PointCount / 2, PointCount / 4 + 4);

	DenseVector<int_max> PointIndexList_a, PointIndexList_b;
	PointIndexList_a.Resize(PointCount_x);
	PointIndexList_b.Resize(PointCount_x);
	for (int_max x = 0; x < PointCount_x; ++x)
	{
		PointIndexList_a[x] = OutputMesh.AddPoint(ScalarType(x)*Spacing_x, ScalarType(0), ScalarType(0));
	}
	PointIndexList_b = PointIndexList_a;

	for (int_max y = 1; y < PointCount_y; ++y)
	{
		for (int_max x = 0; x < PointCount_x; ++x)
		{
			PointIndexList_a[x] = OutputMesh.AddPoint(ScalarType(x)*Spacing_x, ScalarType(y)*Spacing_y, ScalarType(0));
		}

		for (int_max x = 0; x < PointCount_x - 1; ++x)
		{
			auto P0 = PointIndexList_b[x];
			auto P1 = PointIndexList_b[x+1];
			auto P2 = PointIndexList_a[x+1];
			auto P3 = PointIndexList_a[x];
			OutputMesh.AddFaceByPoint({ P0, P1, P2, P3 });
		}

		PointIndexList_b = PointIndexList_a;
	}

	return OutputMesh;
}


template<typename ScalarType>
PolygonMesh<ScalarType> CreateQuadMeshOfCylinderSurface(int_max PointCountPerRing, int_max RingCount, double Radius, double Height)
{
	PolygonMesh<ScalarType> OutputMesh;

	if (PointCountPerRing <= 2 || RingCount <= 0)
	{
		MDK_Error("invalid input @ CreateQuadMeshOfCylinderSurface(...)")
		return OutputMesh;
	}

	auto PointCount = PointCountPerRing*RingCount;
	OutputMesh.SetCapacity(PointCount, PointCount / 2, PointCount / 4 + 4);

	DenseVector<int_max> PointIndexList_a, PointIndexList_b;
	PointIndexList_a.Resize(PointCountPerRing+1);
	PointIndexList_b.Resize(PointCountPerRing+1);
	
	double theta = 2*3.141592654 / double(PointCountPerRing);

	for (int_max k = 0; k < PointCountPerRing; ++k)
	{
		ScalarType x = Radius*std::cos(theta*k);
		ScalarType y = Radius*std::sin(theta*k);
		PointIndexList_a[k] = OutputMesh.AddPoint(x, y, ScalarType(0));
	}
	PointIndexList_a[PointCountPerRing] = PointIndexList_a[0];
	PointIndexList_b = PointIndexList_a;

	for (int_max n = 1; n < RingCount; ++n)
	{
		for (int_max k = 0; k < PointCountPerRing; ++k)
		{
			ScalarType x = Radius*std::cos(theta*k);
			ScalarType y = Radius*std::sin(theta*k);
			ScalarType z = (Height / ScalarType(RingCount-1))*ScalarType(n);
			PointIndexList_a[k] = OutputMesh.AddPoint(x, y, z);
		}
		PointIndexList_a[PointCountPerRing] = PointIndexList_a[0];

		for (int_max k = 0; k < PointCountPerRing; ++k)
		{
			auto P0 = PointIndexList_b[k];
			auto P1 = PointIndexList_b[k + 1];
			auto P2 = PointIndexList_a[k + 1];
			auto P3 = PointIndexList_a[k];
			OutputMesh.AddFaceByPoint({ P0, P1, P2, P3 });
		}

		PointIndexList_b = PointIndexList_a;
	}

	return OutputMesh;
}


}//namespace mdk
