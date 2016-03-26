#pragma once

namespace mdk
{

template<typename MeshAttributeType>
bool SaveTriangleMeshAsJsonDataFile(const TriangleMesh<MeshAttributeType>& InputMesh, const String& FilePathAndName)
{
	return SavePolygonMeshAsJsonDataFile(InputMesh, FilePathAndName);
}

template<typename MeshAttributeType>
bool LoadTriangleMeshFromJsonDataFile(TriangleMesh<MeshAttributeType>& OutputMesh, const String& FilePathAndName)
{
	return 	LoadPolygonMeshFromJsonDataFile(OutputMesh, FilePathAndName);
}

template<typename MeshAttributeType>
bool SaveTriangleMeshAsVTKFile(const TriangleMesh<MeshAttributeType>& InputMesh, const String& FilePathAndName)
{
    auto VTKMesh = ConvertMDKTriangleMeshToVTKPolyData(InputMesh);
	return SaveVTKPolyDataAsVTKFile(VTKMesh, FilePathAndName);
}

template<typename MeshAttributeType>
bool LoadTriangleMeshFromVTKFile(TriangleMesh<MeshAttributeType>& OutputMesh, const String& FilePathAndName)
{
	auto VTKMesh = LoadVTKPolyDataFromVTKFile(FilePathAndName);
	return ConvertVTKPolyDataToMDKTriangleMesh(VTKMesh, OutputMesh);
}

}//namespace mdk
