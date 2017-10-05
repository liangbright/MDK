#include "mdkPolygonMeshProcessing.h"

using namespace mdk;

template<typename ScalarType>
TriangleMesh<ScalarType> HandleSpecialCase1_Before_CollapseEdge(const TriangleMesh<ScalarType>& InputMesh)
{
	auto OutputMesh = InputMesh;
	auto PointIndex_max = OutputMesh.GetMaxValueOfPointIndex();
	for (int_max k = 0; k < PointIndex_max; ++k)
	{
		if (OutputMesh.IsValidPointIndex(k) == true)
		{
			if (OutputMesh.Point(k).IsOnPolygonMeshBoundary() == false)
			{
				auto AdjPointIndexList = OutputMesh.Point(k).GetAdjacentPointIndexList();
				auto AdjEdgeIndexList = OutputMesh.Point(k).GetAdjacentEdgeIndexList();
				auto AdjFaceIndexList = OutputMesh.Point(k).GetAdjacentFaceIndexList();
				bool Flag = false;
				if (AdjPointIndexList.GetLength() == 3 && AdjEdgeIndexList.GetLength() == 3 && AdjFaceIndexList.GetLength() == 3)
				{//case-1 detected
					Flag = true;
				}
				else if (AdjPointIndexList.GetLength() == 4 && AdjEdgeIndexList.GetLength() == 4 && AdjFaceIndexList.GetLength() == 4)
				{
					Flag = true;
				}

				int_max H0 = -1;
				{
					auto PointIndexList = OutputMesh.Edge(AdjEdgeIndexList[0]).GetPointIndexList();
					if (PointIndexList[0] == k)
					{
						H0 = PointIndexList[1];
					}
					else
					{
						H0 = PointIndexList[0];
					}
				}
				if (Flag == true)
				{
					OutputMesh.CollapseEdge(AdjEdgeIndexList[0], H0);
					break;
				}
			}
		}
	}
	OutputMesh.CleanDataStructure();
	return OutputMesh;
}

void Test1()
{	
	TriangleMesh<double> InputMesh;
	LoadPolygonMeshFromVTKFile(InputMesh, "C:/Research/MDK/MDK_Build/Test/Test_MeshProcessing/Test_ResampleMeshBoundary/TestData/Leaflet.vtk");
	/*
	auto BounaryPointIndexList_all = TraceMeshBoundaryCurve(InputMesh, 0, 34);
	DenseVector<int_max> BounaryPointIndexList;	
	for (int_max k = 0; k < BounaryPointIndexList_all.GetLength(); ++k)
	{
		BounaryPointIndexList.Append(BounaryPointIndexList_all[k]);
		if (BounaryPointIndexList_all[k] == 1)
		{
			break;
		}
	}
	*/
	auto BounaryPointIndexList_all = TraceMeshBoundaryCurve(InputMesh, 1, 19);
	DenseVector<int_max> BounaryPointIndexList;
	for (int_max k = 0; k < BounaryPointIndexList_all.GetLength(); ++k)
	{
		BounaryPointIndexList.Append(BounaryPointIndexList_all[k]);
		if (BounaryPointIndexList_all[k] == 0)
		{
			break;
		}
	}

	// not easy to implement ResampleMeshClosedBoundary
	//BounaryPointIndexList = BounaryPointIndexList_all;
	//BounaryPointIndexList.Append(BounaryPointIndexList[0]);

	// this will lead to error
	/*
	for (int_max k = 0; k < BounaryPointIndexList_all.GetLength(); ++k)
	{
		BounaryPointIndexList.Append(BounaryPointIndexList_all[k]);
		if (BounaryPointIndexList_all[k] == 1)
		{
			BounaryPointIndexList.Append(BounaryPointIndexList_all[k+1]);
			break;
		}
	}
	*/
	int_max PointCountOfBounary_output = 1.5*BounaryPointIndexList.GetLength()+1;
	auto OutputMesh = ResampleOpenCurveOfSurface(InputMesh, BounaryPointIndexList, PointCountOfBounary_output);
	SavePolygonMeshAsVTKFile(OutputMesh, "C:/Research/MDK/MDK_Build/Test/Test_MeshProcessing/Test_ResampleMeshBoundary/TestData/Leaflet_out.vtk");
}

void Test2()
{
	TriangleMesh<double> InputMesh, OutputMesh;
	LoadPolygonMeshFromVTKFile(InputMesh, "C:/Research/MDK/MDK_Build/Test/Test_MeshProcessing/Test_ResampleMeshBoundary/TestData/LeafletTriangle.vtk");
	{
		auto BounaryPointIndexList_all = TraceMeshBoundaryCurve(InputMesh, 91, 85);
		DenseVector<int_max> BounaryPointIndexList;
		for (int_max k = 0; k < BounaryPointIndexList_all.GetLength(); ++k)
		{
			BounaryPointIndexList.Append(BounaryPointIndexList_all[k]);
			if (BounaryPointIndexList_all[k] == 16)
			{
				break;
			}
		}
		int_max PointCountOfBounary_output = 1.5*BounaryPointIndexList.GetLength() + 1;
		OutputMesh = ResampleOpenCurveOfSurface(InputMesh, BounaryPointIndexList, PointCountOfBounary_output);
		SavePolygonMeshAsVTKFile(OutputMesh, "C:/Research/MDK/MDK_Build/Test/Test_MeshProcessing/Test_ResampleMeshBoundary/TestData/LeafletTriangle_out1.vtk");
	}
	{
		auto BounaryPointIndexList_all = TraceMeshBoundaryCurve(OutputMesh, 77, 73);
		DenseVector<int_max> BounaryPointIndexList;
		for (int_max k = 0; k < BounaryPointIndexList_all.GetLength(); ++k)
		{
			BounaryPointIndexList.Append(BounaryPointIndexList_all[k]);
			if (BounaryPointIndexList_all[k] == 26)
			{
				break;
			}
		}
		int_max PointCountOfBounary_output = 1.5*BounaryPointIndexList.GetLength() + 1;
		OutputMesh = ResampleOpenCurveOfSurface(OutputMesh, BounaryPointIndexList, PointCountOfBounary_output);
		SavePolygonMeshAsVTKFile(OutputMesh, "C:/Research/MDK/MDK_Build/Test/Test_MeshProcessing/Test_ResampleMeshBoundary/TestData/LeafletTriangle_out2.vtk");
	}
}

void Test3()
{
	TriangleMesh<double> InputMesh, OutputMesh;
	LoadPolygonMeshFromVTKFile(InputMesh, "C:/Research/MDK/MDK_Build/Test/Test_MeshProcessing/Test_ResampleMeshBoundary/TestData/LeafletTriangle2.vtk");
	InputMesh = HandleSpecialCase1_Before_CollapseEdge(InputMesh);
	SavePolygonMeshAsVTKFile(InputMesh, "C:/Research/MDK/MDK_Build/Test/Test_MeshProcessing/Test_ResampleMeshBoundary/TestData/LeafletTriangle2_ok.vtk");
	return;
	{
		auto BounaryPointIndexList_all = TraceMeshBoundaryCurve(InputMesh, 30, 39);
		DenseVector<int_max> BounaryPointIndexList;
		for (int_max k = 0; k < BounaryPointIndexList_all.GetLength(); ++k)
		{
			BounaryPointIndexList.Append(BounaryPointIndexList_all[k]);
			if (BounaryPointIndexList_all[k] == 20)
			{
				break;
			}
		}
		int_max PointCountOfBounary_output = 5;// 1.5*BounaryPointIndexList.GetLength() + 1;
		OutputMesh = ResampleOpenCurveOfSurface(InputMesh, BounaryPointIndexList, PointCountOfBounary_output);
		SavePolygonMeshAsVTKFile(OutputMesh, "C:/Research/MDK/MDK_Build/Test/Test_MeshProcessing/Test_ResampleMeshBoundary/TestData/LeafletTriangle2_out1.vtk");
	}
	/*
	{
		auto BounaryPointIndexList_all = TraceMeshBoundaryCurve(OutputMesh, 28, 31);
		DenseVector<int_max> BounaryPointIndexList;
		for (int_max k = 0; k < BounaryPointIndexList_all.GetLength(); ++k)
		{
			BounaryPointIndexList.Append(BounaryPointIndexList_all[k]);
			if (BounaryPointIndexList_all[k] == 7)
			{
				break;
			}
		}
		int_max PointCountOfBounary_output = 1.5*BounaryPointIndexList.GetLength() + 1;
		OutputMesh = ResampleOpenCurveOfSurface(OutputMesh, BounaryPointIndexList, PointCountOfBounary_output);
		SavePolygonMeshAsVTKFile(OutputMesh, "C:/Research/MDK/MDK_Build/Test/Test_MeshProcessing/Test_ResampleMeshBoundary/TestData/LeafletTriangle2_out2.vtk");
	}
	*/
}

void Test3a()
{
	TriangleMesh<double> InputMesh, OutputMesh;
	LoadPolygonMeshFromVTKFile(InputMesh, "C:/Research/MDK/MDK_Build/Test/Test_MeshProcessing/Test_ResampleMeshBoundary/TestData/LeafletTriangle3.vtk");
	{
		auto BounaryPointIndexList_all = TraceMeshBoundaryCurve(InputMesh, 7743, 7744);
		DenseVector<int_max> BounaryPointIndexList;
		for (int_max k = 0; k < BounaryPointIndexList_all.GetLength(); ++k)
		{
			BounaryPointIndexList.Append(BounaryPointIndexList_all[k]);
			if (BounaryPointIndexList_all[k] == 7437)
			{
				break;
			}
		}
		int_max PointCountOfBounary_output = 10;// 1.5*BounaryPointIndexList.GetLength() + 1;
		OutputMesh = ResampleOpenCurveOfSurface(InputMesh, BounaryPointIndexList, PointCountOfBounary_output);
		SavePolygonMeshAsVTKFile(OutputMesh, "C:/Research/MDK/MDK_Build/Test/Test_MeshProcessing/Test_ResampleMeshBoundary/TestData/LeafletTriangle3_out1.vtk");
	}
	/*
	{
		auto BounaryPointIndexList_all = TraceMeshBoundaryCurve(OutputMesh, 7723, 7856);
		DenseVector<int_max> BounaryPointIndexList;
		for (int_max k = 0; k < BounaryPointIndexList_all.GetLength(); ++k)
		{
			BounaryPointIndexList.Append(BounaryPointIndexList_all[k]);
			if (BounaryPointIndexList_all[k] == 7862)
			{
				break;
			}
		}
		int_max PointCountOfBounary_output = 1.5*BounaryPointIndexList.GetLength() + 1;
		OutputMesh = ResampleOpenCurveOfSurface(OutputMesh, BounaryPointIndexList, PointCountOfBounary_output);
		SavePolygonMeshAsVTKFile(OutputMesh, "C:/Research/MDK/MDK_Build/Test/Test_MeshProcessing/Test_ResampleMeshBoundary/TestData/LeafletTriangle3_out2.vtk");
	}
	*/
}


void Test3b()
{
	String DataPath = "C:/Research/MDK/MDK_Build/Test/Test_MeshProcessing/Test_ResampleMeshBoundary/TestData/";
	TriangleMesh<double> InputMesh, OutputMesh;
	LoadPolygonMeshFromVTKFile(InputMesh, "C:/Research/MDK/MDK_Build/Test/Test_MeshProcessing/Test_ResampleMeshBoundary/TestData/LeafletTriangle3.vtk");
	{
		DenseVector<int_max> BounaryPointIndexList;
		DenseMatrix<int_max> temp;
		LoadDenseMatrixFromJsonDataFile(temp, DataPath + "LeafletTriangle3_BLC_to_H_on_LT_short.json");
		BounaryPointIndexList = temp;
		
		DenseMatrix<double> BounaryPos;
		LoadDenseMatrixFromJsonDataFile(BounaryPos, DataPath + "LeafletTriangle3_Pos_BLC_to_H.json");
		
		//BounaryPointIndexList=BounaryPointIndexList.GetSubSet(0, 10);
		//BounaryPos = BounaryPos.GetSubMatrix(ALL, span(0, 10));

		OutputMesh = ResampleOpenCurveOfSurface(InputMesh, BounaryPointIndexList, 30);
		SavePolygonMeshAsVTKFile(OutputMesh, "C:/Research/MDK/MDK_Build/Test/Test_MeshProcessing/Test_ResampleMeshBoundary/TestData/LeafletTriangle3_BLC_out1.vtk");
	}
}


void main()
{
	//Test1();
	//Test2();
	Test3();
	//Test3a();
	//Test3b();
}
