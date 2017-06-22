#include "mdkPolygonMeshProcessing.h"

using namespace mdk;

void Test1()
{	
	TriangleMesh<double> InputMesh;
	LoadPolygonMeshFromVTKFile(InputMesh, "C:/Research/MDK/MDK_Build/Test/Test_MeshProcessing/Test_ResampleMeshBoundary/TestData/Leaflet.vtk");
	auto BounaryPointIndexList_all = TraceMeshBoundaryCurve(InputMesh, 0);
	DenseVector<int_max> BounaryPointIndexList;
	
	// this is good
	///*
	for (int_max k = 0; k < BounaryPointIndexList_all.GetLength(); ++k)
	{
		BounaryPointIndexList.Append(BounaryPointIndexList_all[k]);
		if (BounaryPointIndexList_all[k] == 1)
		{
			break;
		}
	}	
	//*/

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
	int_max PointCountOfBounary_output = 10*BounaryPointIndexList.GetLength()+1;
	auto OutputMesh = ResampleMeshOpenBoundary(InputMesh, BounaryPointIndexList, PointCountOfBounary_output);
	SavePolygonMeshAsVTKFile(OutputMesh, "C:/Research/MDK/MDK_Build/Test/Test_MeshProcessing/Test_ResampleMeshBoundary/TestData/Leaflet_out.vtk");
}

void main()
{
	Test1();
}
