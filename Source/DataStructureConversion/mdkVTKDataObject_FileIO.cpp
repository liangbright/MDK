#pragma once

#include "mdkVTKDataObject_FileIO.h"

namespace mdk
{
bool SaveVTKPolyDataAsVTKFile(vtkPolyData* OutputVTKMesh, const String& FilePathAndName)
{
	if (OutputVTKMesh == nullptr)
	{
		MDK_Error("input is nullptr @ SaveVTKPolyDataAsVTKFile(...)")
		return false;
	}

	auto writer = vtkSmartPointer<vtkPolyDataWriter>::New();
	writer->SetFileName(FilePathAndName.StdString().c_str());
	writer->SetInputData(OutputVTKMesh);
	writer->SetFileTypeToASCII();
	try
	{
		writer->Write();
	}
	catch (...)
	{
		MDK_Error(" Can not write data @ SaveVTKPolyDataAsVTKFile(...) ")
		return false;
	}
	return true;
}


vtkSmartPointer<vtkPolyData> LoadVTKPolyDataFromVTKFile(const String& FilePathAndName)
{
	auto VTKMesh = vtkSmartPointer<vtkPolyData>::New();
	LoadVTKPolyDataFromVTKFile(VTKMesh, FilePathAndName);
	return VTKMesh;
}


bool LoadVTKPolyDataFromVTKFile(vtkPolyData* OutputVTKMesh, const String& FilePathAndName)
{
	if (OutputVTKMesh == nullptr)
	{
		MDK_Error("input is nullptr @ LoadVTKPolyDataFromVTKFile(...)")
		return false;
	}

	auto Reader = vtkSmartPointer<vtkPolyDataReader>::New();
	Reader->SetFileName(FilePathAndName.StdString().c_str());
	try
	{
		Reader->Update();
	}
	catch (...)
	{
		MDK_Error(" Can not read data @ LoadVTKPolyDataFromVTKFile(...) ")
		return false;
	}
	OutputVTKMesh->ShallowCopy(Reader->GetOutput());
	return true;
}


bool SaveVTKUnstructuredGridAsVTKFile(vtkUnstructuredGrid* OutputVTKMesh, const String& FilePathAndName)
{
	if (OutputVTKMesh == nullptr)
	{
		MDK_Error("input is nullptr @ SaveVTKUnstructuredGridAsVTKFile(...)")
		return false;
	}

	auto writer = vtkSmartPointer<vtkUnstructuredGridWriter>::New();
	writer->SetFileName(FilePathAndName.StdString().c_str());
	writer->SetInputData(OutputVTKMesh);
	writer->SetFileTypeToASCII();
	try
	{
		writer->Write();
	}
	catch (...)
	{
		MDK_Error(" Can not write data @ SaveVTKUnstructuredGridAsVTKFile(...) ")
		return false;
	}
	return true;
}


vtkSmartPointer<vtkUnstructuredGrid> LoadVTKUnstructuredGridFromVTKFile(const String& FilePathAndName)
{
	auto VTKMesh = vtkSmartPointer<vtkUnstructuredGrid>::New();
	LoadVTKUnstructuredGridFromVTKFile(VTKMesh, FilePathAndName);
	return VTKMesh;
}


bool LoadVTKUnstructuredGridFromVTKFile(vtkUnstructuredGrid* OutputVTKMesh, const String& FilePathAndName)
{
	if (OutputVTKMesh == nullptr)
	{
		MDK_Error("input is nullptr @ LoadVTKUnstructuredGridFromVTKFile(...)")
		return false;
	}

	auto Reader = vtkSmartPointer<vtkUnstructuredGridReader>::New();
	Reader->SetFileName(FilePathAndName.StdString().c_str());
	try
	{
		Reader->Update();
	}
	catch (...)
	{
		MDK_Error(" Can not read data @ LoadVTKUnstructuredGridFromVTKFile(...) ")
		return false;
	}
	OutputVTKMesh->ShallowCopy(Reader->GetOutput());
	return true;
}

/*
bool LoadVTKPolygonMeshFromJsonDataFile(vtkPolyData* OutputVTKMesh, const String& FilePathAndName)
{
	if (OutputVTKMesh == nullptr)
	{
		MDK_Error("input is nullptr @ LoadVTKPolygonMeshFromJsonDataFile(...)")
		return false;
	}
	
	JsonObject JObject;
	if (JsonFile::Load(JObject, FilePathAndName) == false)
	{
		MDK_Error("Json file is invalid @ LoadVTKPolygonMeshFromJsonDataFile(...)")
		return false;
	}

	if (JObject.IsEmpty() == true)
	{
		MDK_Warning("Json file is empty @ LoadVTKPolygonMeshFromJsonDataFile(...)")
		return false;
	}
	//---------------------------------------------
	auto it = JObject.find("ObjectType");
	if (it != JObject.end())
	{
		auto ObjectType = it->second.GetString();
		if (ObjectType != "PolygonMesh" && ObjectType != "TriangleMesh")
		{
			MDK_Error("ObjectType is not PolygonMesh or TriangleMesh @ LoadPolygonMeshFromJsonDataFile(...)")
			return false;
		}
	}
	else
	{
		MDK_Error("Couldn't get ObjectType @ LoadPolygonMeshFromJsonDataFile(...)")
		return false;
	}
	//------------------------------------------
	String ScalarTypeInDataFile;
	it = JObject.find("ScalarType");
	if (it != JObject.end())
	{
		ScalarTypeInDataFile = it->second.GetString();
	}
	else
	{
		MDK_Error("Couldn't get ScalarType @ LoadPolygonMeshFromJsonDataFile(...)")
		return false;
	}
	//-------------------------------------------
	String IndexTypeName;
	it = JObject.find("IndexType");
	if (it != JObject.end())
	{
		IndexTypeName = it->second.GetString();
	}
	else
	{
		MDK_Error("Couldn't get IndexType @ LoadVTKPolygonMeshFromJsonDataFile(...)")
		return false;
	}
	//----------------------------------------------
	int_max PointCount = 0;
	it = JObject.find("PointCount");
	if (it != JObject.end())
	{
		PointCount = it->second.ToScalar<int_max>();
	}
	else
	{
		MDK_Error("Couldn't get PointCount @ LoadVTKPolygonMeshFromJsonDataFile(...)")
		return false;
	}
	//----------------------------------------------
	int_max CellCount = 0;
	it = JObject.find("CellCount");
	if (it != JObject.end())
	{
		CellCount = it->second.ToScalar<int_max>();
	}
	else
	{
		MDK_Error("Couldn't get CellCount @ LoadVTKPolygonMeshFromJsonDataFile(...)")
		return false;
	}
	//----------------------------------------------	
	String PointDataFileName;
	it = JObject.find("PointData");
	if (it != JObject.end())
	{
		PointDataFileName = it->second.GetString();
	}
	else
	{
		MDK_Error("Couldn't get PointData @ LoadVTKPolygonMeshFromJsonDataFile(...)")
		return false;
	}
	//----------------------------------------------	
	String CellDataFileName;
	it = JObject.find("CellData");
	if (it != JObject.end())
	{
		CellDataFileName = it->second.GetString();
	}
	else
	{
		MDK_Error("Couldn't get CellData @ LoadVTKPolygonMeshFromJsonDataFile(...)")
		return false;
	}
	//----------------------------------------------	
	if (PointCount > 0)
	{
		String FilePath = ExtractFilePath(FilePathAndName);

		bool IsOK = true;
		DenseMatrix<double> PointData(3, PointCount);
		if (LoadScalarArrayFromDataFile(PointData.GetElementPointer(), PointData.GetElementCount(), FilePath + PointDataFileName, ScalarTypeInDataFile) == false)
		{
			IsOK = false;
		}

		//Get CellData from JsonArray
		ObjectArray<DenseVector<int_max>> CellData;
		CellData.Resize(CellCount);
		if (CellCount > 0)
		{
			JsonArray JArray_CellData;
			if (JsonFile::Load(JArray_CellData, FilePath + CellDataFileName) == false)
			{
				IsOK = false;
			}

			if (JArray_CellData.IsEmpty() == false)
			{
				for (int_max i = 0; i < JArray_CellData.GetLength(); ++i)
				{
					CellData[i] = JArray_CellData[i].ToScalarArray<int_max>();
				}
			}
		}

		if (IsOK == false)
		{
			return false;
		}

		//----------------- construct OutputVTKMesh ----------------------------------//
	
		auto VTKPointData = vtkSmartPointer<vtkPoints>::New();
		if (ScalarTypeInDataFile == "double")
		{
			VTKPointData->SetDataType(VTK_DOUBLE);
			VTKPointData->SetNumberOfPoints(PointCount);
			for (int i = 0; i < PointCount; ++i)
			{
				double pos[3];
				pos[0] = PointData(0, i);
				pos[1] = PointData(1, i);
				pos[2] = PointData(2, i);
				VTKPointData->SetPoint(i, pos);
			}
		}
		else if (ScalarTypeInDataFile == "float")
		{
			VTKPointData->SetDataType(VTK_FLOAT);
			VTKPointData->SetNumberOfPoints(PointCount);
			for (int i = 0; i < PointCount; ++i)
			{
				float pos[3];
				pos[0] = float(PointData(0, i));
				pos[1] = float(PointData(1, i));
				pos[2] = float(PointData(2, i));
				VTKPointData->SetPoint(i, pos);
			}
		}
		else
		{
			MDK_Warning("ScalarTypeInDataFile is not double or float @ LoadVTKPolygonMeshFromJsonDataFile(...)")

			VTKPointData->SetDataType(VTK_DOUBLE);
			VTKPointData->SetNumberOfPoints(PointCount);
			for (int i = 0; i < PointCount; ++i)
			{
				double pos[3];
				pos[0] = PointData(0, i);
				pos[1] = PointData(1, i);
				pos[2] = PointData(2, i);
				VTKPointData->SetPoint(i, pos);
			}
		}
		//------------------------------------------
		auto VTKCellData = vtkSmartPointer<vtkCellArray>::New();
		for (int i = 0; i < CellCount; ++i)
		{
			auto PointCountInCell = CellData[i].GetElementCount();
			VTKCellData->InsertNextCell(PointCountInCell);
			for (int n = 0; n < PointCountInCell; ++n)
			{
				auto PointIndex = CellData[i][n];
				VTKCellData->InsertCellPoint(PointIndex);
			}
		}
		//---------------------------------------------------
		OutputVTKMesh->SetPoints(VTKPointData);
		OutputVTKMesh->SetPolys(VTKCellData);
		return true;
	}
	else// empty mesh
	{
		return false;
	}
}
*/
/*
bool LoadVTK3DScalarImageFromJsonDataFile(vtkImageData* OutputVTKImage, VTKDataTypeEnum OutputPixelType, const String& FilePathAndName)
{
	if (OutputVTKImage == nullptr)
	{
		MDK_Error("invalid input: nullptr @ LoadVTK3DScalarImageFromJsonDataFile(...)")
		return false;
	}

	JsonObject JObject;
	if (JsonFile::Load(JObject, FilePathAndName) == false)
	{
		MDK_Error("Json file is invalid @ LoadVTK3DScalarImageFromJsonDataFile(...)")
		return false;
	}

	if (JObject.IsEmpty() == true)
	{
		MDK_Warning("Json file is empty @ LoadVTK3DScalarImageFromJsonDataFile(...)")
		return false;
	}
	//---------------------------------------------
	auto it = JObject.find("ObjectType");
	if (it != JObject.end())
	{
		auto ObjectType = it->second.GetString();
		if (ObjectType != "DenseImage3D")
		{
			MDK_Error("ObjectType is not DenseImage3D @ LoadVTK3DScalarImageFromJsonDataFile(...)")
			return false;
		}
	}
	else
	{
		MDK_Error("Couldn't get ObjectType @ LoadVTK3DScalarImageFromJsonDataFile(...)")
		return false;
	}
	//---------------------------------------------------
	String PixelTypeInDataFile;
	it = JObject.find("PixelType");
	if (it != JObject.end())
	{
		PixelTypeInDataFile = it->second.GetString();
	}
	else
	{
		MDK_Error("Couldn't get PixelType @ LoadVTK3DScalarImageFromJsonDataFile(...)")
		return false;
	}
	//---------------------------------------------------
	DenseVector<int_max, 3> Size;
	it = JObject.find("Size");
	if (it != JObject.end())
	{
		auto tempArray = it->second.ToScalarArray<int_max>();
		if (tempArray.GetElementCount() != 3)
		{
			MDK_Error("Size vector is wrong @ LoadVTK3DScalarImageFromJsonDataFile(...)")
			return false;
		}
		Size[0] = tempArray[0];
		Size[1] = tempArray[1];
		Size[2] = tempArray[2];
	}
	else
	{
		MDK_Error("Couldn't get Size @ LoadVTK3DScalarImageFromJsonDataFile(...)")
		return false;
	}
	//---------------------------------------------
	DenseVector<double, 3> Spacing;
	it = JObject.find("Spacing");
	if (it != JObject.end())
	{
		auto tempArray = it->second.ToScalarArray<double>();
		if (tempArray.GetElementCount() != 3)
		{
			MDK_Error("Spacing vector is wrong @ LoadVTK3DScalarImageFromJsonDataFile(...)")
			return false;
		}
		Spacing[0] = tempArray[0];
		Spacing[1] = tempArray[1];
		Spacing[2] = tempArray[2];
	}
	else
	{
		MDK_Error("Couldn't get Spacing @ LoadVTK3DScalarImageFromJsonDataFile(...)")
		return false;
	}
	//--------------------------------------
	DenseVector<double, 3> Origin;
	it = JObject.find("Origin");
	if (it != JObject.end())
	{
		auto tempArray = it->second.ToScalarArray<double>();
		if (tempArray.GetElementCount() != 3)
		{
			MDK_Error("Origin vector is wrong @ LoadVTK3DScalarImageFromJsonDataFile(...)")
			return false;
		}
		Origin[0] = tempArray[0];
		Origin[1] = tempArray[1];
		Origin[2] = tempArray[2];
	}
	else
	{
		MDK_Error("Couldn't get Origin @ LoadVTK3DScalarImageFromJsonDataFile(...)")
		return false;
	}
	//------------------------------------
	DenseMatrix<double> Orientation;
	it = JObject.find("Orientation");
	if (it != JObject.end())
	{
		auto tempOrientation = it->second.ToScalarArray<double>();
		if (tempOrientation.GetElementCount() != 9)
		{
			MDK_Error("Orientation size is wrong @ LoadVTK3DScalarImageFromJsonDataFile(...)")
			return false;
		}
		Orientation = tempOrientation;
		Orientation.Reshape(3, 3);
	}
	else
	{
		MDK_Error("Couldn't get Orientation @ LoadVTK3DScalarImageFromJsonDataFile(...)")
		return false;
	}
	//------------------------------------
	String DataFilePathAndName = ExtractFilePath(FilePathAndName);
	it = JObject.find("PixelArray");
	if (it != JObject.end())
	{
		DataFilePathAndName += it->second.GetString();
	}
	else
	{
		MDK_Error("Couldn't get PixelArray @ LoadVTK3DScalarImageFromJsonDataFile(...)")
		return false;
	}
	//--------------------------------------------------
	OutputVTKImage->SetExtent(0, Size[0] - 1, 0, Size[0] - 1, 0, Size[0] - 1);
	OutputVTKImage->SetOrigin(Origin[0], Origin[1], Origin[2]);
	OutputVTKImage->SetSpacing(Spacing[0], Spacing[1], Spacing[2]);

	int_max OutputPixelNumber = Size[0] * Size[1] * Size[2];

	switch (OutputPixelType)
	{
	case VTKDataTypeEnum::VALUE_DOUBLE:
	{
		OutputVTKImage->AllocateScalars(OutputPixelType, 1);
		auto PtrOfVTKImage = static_cast<double*>(OutputVTKImage->GetScalarPointer());
		return LoadScalarArrayFromDataFile(PtrOfVTKImage, OutputPixelNumber, DataFilePathAndName, PixelTypeInDataFile);
	}
	case VTKDataTypeEnum::VALUE_FLOAT:
	{
		OutputVTKImage->AllocateScalars(OutputPixelType, 1);
		auto PtrOfVTKImage = static_cast<float*>(OutputVTKImage->GetScalarPointer());
		return LoadScalarArrayFromDataFile(PtrOfVTKImage, OutputPixelNumber, DataFilePathAndName, PixelTypeInDataFile);
	}
	case VTKDataTypeEnum::VALUE_LONG_LONG:
	{
		OutputVTKImage->AllocateScalars(OutputPixelType, 1);
		auto PtrOfVTKImage = static_cast<long long*>(OutputVTKImage->GetScalarPointer());
		return LoadScalarArrayFromDataFile(PtrOfVTKImage, OutputPixelNumber, DataFilePathAndName, PixelTypeInDataFile);
	}
	case VTKDataTypeEnum::VALUE_LONG:
	{
		OutputVTKImage->AllocateScalars(OutputPixelType, 1);
		auto PtrOfVTKImage = static_cast<long*>(OutputVTKImage->GetScalarPointer());
		return LoadScalarArrayFromDataFile(PtrOfVTKImage, OutputPixelNumber, DataFilePathAndName, PixelTypeInDataFile);
	}
	case VTKDataTypeEnum::VALUE_INT:
	{
		OutputVTKImage->AllocateScalars(OutputPixelType, 1);
		auto PtrOfVTKImage = static_cast<int*>(OutputVTKImage->GetScalarPointer());
		return LoadScalarArrayFromDataFile(PtrOfVTKImage, OutputPixelNumber, DataFilePathAndName, PixelTypeInDataFile);
	}
	case VTKDataTypeEnum::VALUE_SHORT:
	{
		OutputVTKImage->AllocateScalars(OutputPixelType, 1);
		auto PtrOfVTKImage = static_cast<short*>(OutputVTKImage->GetScalarPointer());
		return LoadScalarArrayFromDataFile(PtrOfVTKImage, OutputPixelNumber, DataFilePathAndName, PixelTypeInDataFile);
	}
	case VTKDataTypeEnum::VALUE_CHAR:
	{
		OutputVTKImage->AllocateScalars(OutputPixelType, 1);
		auto PtrOfVTKImage = static_cast<char*>(OutputVTKImage->GetScalarPointer());
		return LoadScalarArrayFromDataFile(PtrOfVTKImage, OutputPixelNumber, DataFilePathAndName, PixelTypeInDataFile);
	}
	default:
		MDK_Error("unknown scalar type @ LoadVTK3DScalarImageFromJsonDataFile(...)")
		return false;
	}
}
*/

}// namespace mdk
