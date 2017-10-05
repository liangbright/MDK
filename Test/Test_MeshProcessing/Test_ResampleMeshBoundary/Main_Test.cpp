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
		int_max PointCountOfBounary_output = 5;// 1.5*BounaryPointIndexList.GetLength() + 1;
		OutputMesh = ResampleOpenBoundaryCurveOfSurface(InputMesh, BounaryPointIndexList, PointCountOfBounary_output);
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
		OutputMesh = ResampleOpenBoundaryCurveOfSurface(OutputMesh, BounaryPointIndexList, PointCountOfBounary_output);
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
		OutputMesh = ResampleOpenBoundaryCurveOfSurface(InputMesh, BounaryPointIndexList, PointCountOfBounary_output);
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
		OutputMesh = ResampleOpenBoundaryCurveOfSurface(OutputMesh, BounaryPointIndexList, PointCountOfBounary_output);
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

		OutputMesh = ResampleOpenBoundaryCurveOfSurface(InputMesh, BounaryPointIndexList, 30);
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
