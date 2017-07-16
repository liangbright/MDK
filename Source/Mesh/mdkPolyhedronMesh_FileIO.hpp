#pragma once

namespace mdk
{

template<typename ScalarType>
bool SavePolyhedronMeshAsJsonDataFile(const PolyhedronMesh<ScalarType>& InputMesh, const String& FilePathAndName)
{
	//---------------------------------------------
	if (GetByteCountOfScalar(ScalarType(0)) <= 0)
	{
		MDK_Error("Unknown ScalarType @ SavePolyhedronMeshAsJsonDataFile(...)")
		return false;
	}
	//-------------------------------------------------------------------//
	JsonObject JObject;

	JObject["ObjectType"] = "PolyhedronMesh";
	JObject["Name"] = InputMesh.GetName();
	JObject["ScalarType"] = GetScalarTypeName(ScalarType(0));
	JObject["IndexType"] = GetScalarTypeName(int_max(0));
	//----------------------------------------
	auto PointCount = InputMesh.GetPointCount();
	auto CellCount = InputMesh.GetCellCount();
	auto FaceCount = InputMesh.GetFaceCount();
	int_max NamedPointCount = InputMesh.GetNamedPointCount();
	int_max NamedFaceCount = InputMesh.GetNamedFaceCount();
	int_max NamedCellCount = InputMesh.GetNamedCellCount();
	auto PointSetCount = InputMesh.GetPointSetCount();
	auto FaceSetCount = InputMesh.GetFaceSetCount();
	auto CellSetCount = InputMesh.GetCellSetCount();
	JObject["PointCount"] = PointCount;
	JObject["FaceCount"] = FaceCount;
	JObject["CellCount"] = CellCount;
	JObject["NamedPointCount"] = NamedPointCount;
	JObject["NamedFaceCount"] = NamedFaceCount;
	JObject["NamedCellCount"] = NamedCellCount;
	JObject["PointSetCount"] = PointSetCount;
	JObject["FaceSetCount"] = FaceSetCount;
	JObject["CellSetCount"] = CellSetCount;
	//----------------------------------------
	if (NamedPointCount > 0)
	{
		JsonObject tempJObject;
		auto PointNameList = InputMesh.GetValidPointNameList();
		for (int_max k = 0; k < PointNameList.GetLength(); ++k)
		{
			auto PointIndex = InputMesh.GetPointIndexByName(PointNameList[k]);
		    tempJObject[PointNameList[k]] = PointIndex;
		}
		JsonValue tempJValue = std::move(tempJObject);
		JObject["NamedPointList"] = tempJValue;
	}
	//----------------------------------------
	if (NamedFaceCount > 0)
	{
		JsonObject tempJObject;
		auto FaceNameList = InputMesh.GetValidFaceNameList();
		for (int_max k = 0; k < FaceNameList.GetLength(); ++k)
		{
			auto FaceIndex = InputMesh.GetFaceIndexByName(FaceNameList[k]);
			tempJObject[FaceNameList[k]] = FaceIndex;
		}
		JsonValue tempJValue = std::move(tempJObject);
		JObject["NamedFaceList"] = tempJValue;
	}
	//----------------------------------------
	if (NamedCellCount > 0)
	{
		JsonObject tempJObject;
		auto CellNameList = InputMesh.GetValidCellNameList();
		for (int_max k = 0; k < CellNameList.GetLength(); ++k)
		{
			auto CellIndex = InputMesh.GetCellIndexByName(CellNameList[k]);
			tempJObject[CellNameList[k]] = CellIndex;
		}
		JsonValue tempJValue = std::move(tempJObject);
		JObject["CellNameList"] = tempJValue;
	}
	//----------------------------------------	
	if (PointSetCount > 0)
	{// MUST use JsonArray to maintain the order in PointSetList		
		JsonArray tempJArray;		
		tempJArray.Resize(PointSetCount);
		for (int_max PointSetIndex = 0; PointSetIndex < PointSetCount; ++PointSetIndex)
		{
			JsonObject tempJObject;
			auto PointSet = InputMesh.GetPointSet(PointSetIndex);
			auto Name = InputMesh.GetPointSetName(PointSetIndex);
			tempJObject[Name] = PointSet;
			tempJArray[PointSetIndex] = tempJObject;			
		}		
		JsonValue tempJValue = std::move(tempJArray);
		JObject["PointSetList"] = tempJValue;
	}	
	//----------------------------------------
	if (FaceSetCount > 0)
	{// MUST use JsonArray to maintain the order in FaceSetList
		JsonArray tempJArray;		
		tempJArray.Resize(FaceSetCount);		
		for (int_max FaceSetIndex = 0; FaceSetIndex < FaceSetCount; ++FaceSetIndex)
		{
			JsonObject tempJObject;
			auto FaceSet = InputMesh.GetFaceSet(FaceSetIndex);
			auto Name = InputMesh.GetFaceSetName(FaceSetIndex);
			tempJObject[Name] = FaceSet;
			tempJArray[FaceSetIndex] = tempJObject;			
		}
		JsonValue tempJValue = std::move(tempJArray);
		JObject["FaceSetList"] = tempJValue;
	}
	//----------------------------------------
	if (CellSetCount > 0)
	{// MUST use JsonArray to maintain the order in CellSetList
		JsonArray tempJArray;
		tempJArray.Resize(CellSetCount);		
		for (int_max CellSetIndex = 0; CellSetIndex < CellSetCount; ++CellSetIndex)
		{
			JsonObject tempJObject;
			auto CellSet = InputMesh.GetCellSet(CellSetIndex);
			auto Name = InputMesh.GetCellSetName(CellSetIndex);
			tempJObject[Name] = CellSet;
			tempJArray[CellSetIndex] = tempJObject;
		}
		JsonValue tempJValue = std::move(tempJArray);
		JObject["CellSetList"] = tempJValue;
	}
	//----------------------------------------
	String FilePath = ExtractFilePath(FilePathAndName);
	String FileName_VTKMesh = ExtractFileName(FilePathAndName) + ".vtk";
	JObject["Point_Face_Cell"] = FileName_VTKMesh;
	//----------------------------------------------------------------------------------//
	bool IsOK = true;
	if (SavePolyhedronMeshAsVTKFile(InputMesh, FilePath + FileName_VTKMesh) == false)
	{
		MDK_Error("Error1 @ SavePolyhedronMeshAsJsonDataFile(...)")
		IsOK = false;
	}
	if (JsonFile::Save(JObject, FilePathAndName) == false)
	{
		MDK_Error("Error2 @ SavePolyhedronMeshAsJsonDataFile(...)")
		IsOK = false;
	}
	return IsOK;
}


template<typename ScalarType>
bool LoadPolyhedronMeshFromJsonDataFile(PolyhedronMesh<ScalarType>& OutputMesh, const String& FilePathAndName)
{
	if (GetByteCountOfScalar(ScalarType(0)) <= 0)
	{
		MDK_Error("Unknown ScalarType @ SavePolyhedronMeshAsJsonDataFile(...)")
		return false;
	}
	//---------------------------------------------
	JsonObject JObject;
	if (JsonFile::Load(JObject, FilePathAndName) == false)
	{
		MDK_Error("Json file is invalid @ LoadPolyhedronMeshFromJsonDataFile(...)")
		return false;
	}

	if (JObject.IsEmpty() == true)
	{
		MDK_Warning("Json file is empty @ LoadPolyhedronMeshFromJsonDataFile(...)")
		return true;
	}
	//----------------------------------------------------------//
	{
		auto it = JObject.find("ObjectType");
		if (it != JObject.end())
		{
			auto ObjectType = it->second.GetString();
			if (ObjectType != "PolyhedronMesh" && ObjectType != "TetrahedronMesh")
			{
				MDK_Error("ObjectType is not PolyhedronMesh or TetrahedronMesh @ LoadPolyhedronMeshFromJsonDataFile(...)")
				return false;
			}
		}
		else
		{
			MDK_Error("Couldn't get ObjectType @ LoadPolyhedronMeshFromJsonDataFile(...)")
			return false;
		}
	}
	//----------------------------------------------------------//
	{
		auto it = JObject.find("Point_Face_Cell");
		if (it != JObject.end())
		{
			String FilePath = ExtractFilePath(FilePathAndName);
			String FileName_VTKMesh = it->second.GetString();
			if (LoadPolyhedronMeshFromVTKFile(OutputMesh, FilePath + FileName_VTKMesh) == false)
			{
				return false;
			}
		}
	}
	//----------------------------------------------------------//	
	{
		auto it = JObject.find("Name");
		if (it != JObject.end())
		{
			OutputMesh.SetName(it->second.GetString());
		}
		else
		{
			MDK_Error("Couldn't get Name @ LoadPolyhedronMeshFromJsonDataFile(...)")
			return false;
		}
	}
	//----------------------------------------------------------//
	String InputScalarTypeName;
	{
		auto it = JObject.find("ScalarType");
		if (it != JObject.end())
		{
			InputScalarTypeName = it->second.GetString();
		}
		else
		{
			MDK_Error("Couldn't get ScalarType @ LoadPolyhedronMeshFromJsonDataFile(...)")
			return false;
		}
	}
	//----------------------------------------------------
	String InputIndexTypeName;
	{
		auto it = JObject.find("IndexType");
		if (it != JObject.end())
		{
			InputIndexTypeName = it->second.GetString();
		}
		else
		{
			MDK_Error("Couldn't get IndexType @ LoadPolyhedronMeshFromJsonDataFile(...)")
			return false;
		}
	}
	//----------------------------------------------------------//
	int_max PointCount;
	{
		auto it = JObject.find("PointCount");
		if (it != JObject.end())
		{
			PointCount = it->second.ToScalar<int_max>();
		}
		else
		{
			MDK_Error("Couldn't get PointCount @ LoadPolyhedronMeshFromJsonDataFile(...)")
			return false;
		}
	}
	//------------------------------------------------
	int_max FaceCount;
	{
		auto it = JObject.find("FaceCount");
		if (it != JObject.end())
		{
			FaceCount = it->second.ToScalar<int_max>();
		}
		else
		{
			MDK_Error("Couldn't get FaceCount @ LoadPolyhedronMeshFromJsonDataFile(...)")
			return false;
		}
	}
	//------------------------------------------------
	int_max CellCount;
	{
		auto it = JObject.find("CellCount");
		if (it != JObject.end())
		{
			CellCount = it->second.ToScalar<int_max>();
		}
		else
		{
			MDK_Error("Couldn't get CellCount @ LoadPolyhedronMeshFromJsonDataFile(...)")
			return false;
		}
	}
	//---------------------------------------------
	int_max PointSetCount;
	{
		auto it = JObject.find("PointSetCount");
		if (it != JObject.end())
		{
			PointSetCount = it->second.ToScalar<int_max>();
		}
		else
		{
			MDK_Error("Couldn't get PointSetCount @ LoadPolyhedronMeshFromJsonDataFile(...)")
			return false;
		}
	}
	//----------------------------------------------
	int_max FaceSetCount;
	{
		auto it = JObject.find("FaceSetCount");
		if (it != JObject.end())
		{
			FaceSetCount = it->second.ToScalar<int_max>();
		}
		else
		{
			MDK_Error("Couldn't get FaceSetCount @ LoadPolyhedronMeshFromJsonDataFile(...)")
			return false;
		}
	}
	//----------------------------------------------
	int_max CellSetCount;
	{
		auto it = JObject.find("CellSetCount");
		if (it != JObject.end())
		{
			CellSetCount = it->second.ToScalar<int_max>();
		}
		else
		{
			MDK_Error("Couldn't get CellSetCount @ LoadPolyhedronMeshFromJsonDataFile(...)")
			return false;
		}
	}
	//----------------------------------------------------------//
	{
		auto it = JObject.find("NamedPointList");
		if (it != JObject.end())
		{			
			auto tempJObject = it->second.GetJsonObject();
			for (auto it = tempJObject.begin(); it != tempJObject.end(); ++it)
			{
				const auto& PointName = it->first;
				auto PointIndex = it->second.ToScalar<int_max>();				
				OutputMesh.Point(PointIndex).SetName(PointName);
			}
		}
	}
	//----------------------------------------------------------//
	{
		auto it = JObject.find("NamedFaceList");
		if (it != JObject.end())
		{
			auto tempJObject = it->second.GetJsonObject();
			for (auto it = tempJObject.begin(); it != tempJObject.end(); ++it)
			{
				const auto& FaceName = it->first;
				auto FaceIndex = it->second.ToScalar<int_max>();
				OutputMesh.Face(FaceIndex).SetName(FaceName);
			}
		}
	}
	//----------------------------------------------------------//
	{
		auto it = JObject.find("NamedCellList");
		if (it != JObject.end())
		{
			auto tempJObject = it->second.GetJsonObject();
			for (auto it = tempJObject.begin(); it != tempJObject.end(); ++it)
			{
				const auto& CellName = it->first;
				auto CellIndex = it->second.ToScalar<int_max>();
				OutputMesh.Cell(CellIndex).SetName(CellName);
			}
		}
	}
	//----------------------------------------
	{
		auto it = JObject.find("PointSetList");
		if (it != JObject.end())
		{
			auto tempJArray = it->second.GetJsonArray();			
			for (int_max k = 0; k < tempJArray.GetLength(); ++k)
			{
				auto tempJObject = tempJArray[k].GetJsonObject();
				auto it = tempJObject.begin();
				if(it != tempJObject.end())
				{
					const auto& PointSetName = it->first;
					auto PointIndexList = it->second.ToScalarArray<int_max>();
					OutputMesh.AddPointSet(PointSetName, PointIndexList);					
				}
			}
		}
	}
	//----------------------------------------
	{
		auto it = JObject.find("FaceSetList");
		if (it != JObject.end())
		{
			auto tempJArray = it->second.GetJsonArray();
			for (int_max k = 0; k < tempJArray.GetLength(); ++k)
			{
				auto tempJObject = tempJArray[k].GetJsonObject();
				auto it = tempJObject.begin(); 
				if(it != tempJObject.end())
				{
					const auto& FaceSetName = it->first;
					auto FaceIndexList = it->second.ToScalarArray<int_max>();
					OutputMesh.AddFaceSet(FaceSetName, FaceIndexList);
				}
			}
		}
	}
	//----------------------------------------
	{
		auto it = JObject.find("CellSetList");
		if (it != JObject.end())
		{
			auto tempJArray = it->second.GetJsonArray();
			for (int_max k = 0; k < tempJArray.GetLength(); ++k)
			{
				auto tempJObject = tempJArray[k].GetJsonObject();
				auto it = tempJObject.begin();
				if (it != tempJObject.end())
				{
					const auto& CellSetName = it->first;
					auto CellIndexList = it->second.ToScalarArray<int_max>();
					OutputMesh.AddCellSet(CellSetName, CellIndexList);
				}
			}
		}
	}
	//-----------------
	return true;
}


template<typename ScalarType>
bool SavePolyhedronMeshAsVTKFile(const PolyhedronMesh<ScalarType>& InputMesh, const String& FilePathAndName)
{
	auto VTKMesh = ConvertMDKPolyhedronMeshToVTKUnstructuredGrid(InputMesh);
	return SaveVTKUnstructuredGridAsVTKFile(VTKMesh, FilePathAndName);
}


template<typename ScalarType>
bool LoadPolyhedronMeshFromVTKFile(PolyhedronMesh<ScalarType>& OutputMesh, const String& FilePathAndName)
{
	auto VTKMesh = LoadVTKUnstructuredGridFromVTKFile(FilePathAndName);
	return ConvertVTKUnstructuredGridToMDKPolyhedronMesh(VTKMesh, OutputMesh);
}

}//namespace mdk

