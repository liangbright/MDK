#include "mdkPolygonMeshProcessing.h"

using namespace mdk;

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
	auto OutputMesh = ResampleOpenBoundaryCurveOfSurface(InputMesh, BounaryPointIndexList, PointCountOfBounary_output);
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
		OutputMesh = ResampleOpenBoundaryCurveOfSurface(InputMesh, BounaryPointIndexList, PointCountOfBounary_output);
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
		OutputMesh = ResampleOpenBoundaryCurveOfSurface(OutputMesh, BounaryPointIndexList, PointCountOfBounary_output);
		SavePolygonMeshAsVTKFile(OutputMesh, "C:/Research/MDK/MDK_Build/Test/Test_MeshProcessing/Test_ResampleMeshBoundary/TestData/LeafletTriangle_out2.vtk");
	}
}

void Test3()
{
	TriangleMesh<double> InputMesh, OutputMesh;
	LoadPolygonMeshFromVTKFile(InputMesh, "C:/Research/MDK/MDK_Build/Test/Test_MeshProcessing/Test_ResampleMeshBoundary/TestData/LeafletTriangle2.vtk");
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
		int_max PointCountOfBounary_output = 1.5*BounaryPointIndexList.GetLength() + 1;
		OutputMesh = ResampleOpenBoundaryCurveOfSurface(InputMesh, BounaryPointIndexList, PointCountOfBounary_output);
		SavePolygonMeshAsVTKFile(OutputMesh, "C:/Research/MDK/MDK_Build/Test/Test_MeshProcessing/Test_ResampleMeshBoundary/TestData/LeafletTriangle2_out1.vtk");
	}
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
		OutputMesh = ResampleOpenBoundaryCurveOfSurface(OutputMesh, BounaryPointIndexList, PointCountOfBounary_output);
		SavePolygonMeshAsVTKFile(OutputMesh, "C:/Research/MDK/MDK_Build/Test/Test_MeshProcessing/Test_ResampleMeshBoundary/TestData/LeafletTriangle2_out2.vtk");
	}
}

void main()
{
	//Test1();
	//Test2();
	Test3();
}
