#ifndef mdk_PolygonMesh_FileIO_hpp
#define mdk_PolygonMesh_FileIO_hpp

namespace mdk
{

template<typename MeshAttributeType>
bool SavePolygonMeshAsJsonDataFile(const PolygonMesh<MeshAttributeType>& InputMesh, const String& FilePathAndName)
{
	typedef MeshAttributeType::ScalarType  ScalarType;
	//---------------------------------------------
	if (GetByteCountOfScalar(ScalarType(0)) <= 0)
	{
		MDK_Error("Unknown ScalarType @ SavePolygonMeshAsJsonDataFile_Header(...)")
		return false;
	}
	//-------------------------------------------------------------------------------------
	JsonObject JObject;

	JObject["ObjectType"] = "PolygonMesh";
	JObject["ScalarType"] = GetScalarTypeName(ScalarType(0));
	JObject["IndexType"] = GetScalarTypeName(int_max(0));
	JObject["PointCount"] = InputMesh.GetPointCount();
	JObject["CellCount"] = InputMesh.GetCellCount();
	
	//--------------------------------------------------------------------
	ObjectArray<DenseVector<int_max>> CellData;
	DenseMatrix<ScalarType> PointData;
	InputMesh.GetPointPositionMatrixAndCellTable(PointData, CellData);

	String FilePath = ExtractFilePath(FilePathAndName);
	String FileName = ExtractFileName(FilePathAndName);
	String CellDataFileName = FileName + ".Cell.json";
	String PointDataFileName = FileName + ".Point.data";
	JObject["CellData"] = CellDataFileName;
	JObject["PointData"] = PointDataFileName;
	//--------------------------------------------------------------------
	//write cell to JsonArray
	JsonArray JArray_CellData;
	JArray_CellData.Resize(CellData.GetLength());
	if (CellData.IsEmpty() == false)
	{
		for (int_max i = 0; i < CellData.GetLength(); ++i)
		{
			const DenseVector<int_max>& Cell_i = CellData[i];

			JArray_CellData[i] = Cell_i;
		}
	}
	//----------------------------------------------------------------------------------------------
	bool IsOK = true;
	if (JsonFile::Save(JObject, FilePathAndName) == false)
	{
		IsOK = false;
	}
	if (SaveScalarArrayAsDataFile(PointData.GetElementPointer(), PointData.GetElementCount(), FilePath + PointDataFileName) == false)
	{
		IsOK = false;
	}	
	if (JsonFile::Save(JArray_CellData, FilePath + CellDataFileName) == false)
	{
		IsOK = false;
	}
	return IsOK;
}


template<typename MeshAttributeType>
bool LoadPolygonMeshFromJsonDataFile(PolygonMesh<MeshAttributeType>& OutputMesh, const String& FilePathAndName)
{
	typedef MeshAttributeType::ScalarType  ScalarType;
	//---------------------------------------------
	JsonObject JObject;
	if (JsonFile::Load(JObject, FilePathAndName) == false)
	{
		MDK_Error("Json file is invalid @ LoadPolygonMeshFromJsonDataFile(...)")
		return false;
	}

	if (JObject.IsEmpty() == true)
	{
		MDK_Warning("Json file is empty @ LoadPolygonMeshFromJsonDataFile(...)")
		return true;
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
		MDK_Error("Couldn't get IndexType @ LoadPolygonMeshFromJsonDataFile(...)")
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
		MDK_Error("Couldn't get PointCount @ LoadPolygonMeshFromJsonDataFile(...)")
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
		MDK_Error("Couldn't get CellCount @ LoadPolygonMeshFromJsonDataFile(...)")
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
		MDK_Error("Couldn't get PointData @ LoadPolygonMeshFromJsonDataFile(...)")
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
		MDK_Error("Couldn't get CellData @ LoadPolygonMeshFromJsonDataFile(...)")
		return false;
	}
	//----------------------------------------------
	String FilePath = ExtractFilePath(FilePathAndName);
	
	if (PointCount > 0)
	{
		bool IsOK = true;
		DenseMatrix<ScalarType> PointData(3, PointCount);
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

		OutputMesh.Construct(std::move(PointData), CellData);
		return IsOK;
	}
	else// empty mesh
	{
		OutputMesh.Clear();
		return true;
	}
}


template<typename MeshAttributeType>
bool SavePolygonMeshAsVTKFile(const PolygonMesh<MeshAttributeType>& InputMesh, const String& FilePathAndName)
{
	auto VTKMesh = ConvertMDKPolygonMeshToVTKPolyData(InputMesh);

	auto writer = vtkSmartPointer<vtkPolyDataWriter>::New();
	writer->SetFileName(FilePathAndName.StdString().c_str());
	writer->SetInputData(VTKMesh);

	try
	{
		writer->Write();
	}
	catch (...)
	{
		MDK_Error(" Can not write data @ SavePolygonMeshAsVTKFile(...) ")
		return false;
	}

	return true;
}


template<typename MeshAttributeType>
bool LoadPolygonMeshFromVTKFile(PolygonMesh<MeshAttributeType>& OutputMesh, const String& FilePathAndName)
{
	typedef MeshAttributeType::ScalarType ScalarType;

	auto Reader = vtkSmartPointer<vtkPolyDataReader>::New();
	Reader->SetFileName(FilePathAndName.StdString().c_str());

	try
	{
		Reader->Update();
	}
	catch (...)
	{
		MDK_Error(" Can not read data @ LoadPolygonMeshFromVTKFile(...) ")
		return false;
	}

	auto VTKMesh = Reader->GetOutput();
	return ConvertVTKPolyDataToMDKPolygonMesh(VTKMesh, OutputMesh);
}

}//namespace mdk

#endif