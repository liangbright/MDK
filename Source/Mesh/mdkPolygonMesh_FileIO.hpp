#pragma once

namespace mdk
{

template<typename MeshAttributeType>
bool SavePolygonMeshAsJsonDataFile(const PolygonMesh<MeshAttributeType>& InputMesh, const String& FilePathAndName)
{
	
	typedef MeshAttributeType::ScalarType  ScalarType;
	//---------------------------------------------
	if (GetByteCountOfScalar(ScalarType(0)) <= 0)
	{
		MDK_Error("Unknown ScalarType @ SavePolygonMeshAsJsonDataFile(...)")
		return false;
	}
	//-------------------------------------------------------------------//
	JsonObject JObject;

	JObject["ObjectType"] = "PolygonMesh";
	JObject["ID"] = InputMesh.GetID();
	JObject["Name"] = InputMesh.GetName();
	JObject["ScalarType"] = GetScalarTypeName(ScalarType(0));
	JObject["IndexType"] = GetScalarTypeName(int_max(0));
	//----------------------------------------
	auto PointCount = InputMesh.GetPointCount();
	auto FaceCount = InputMesh.GetFaceCount();
	int_max NamedPointCount = InputMesh.GetNamedPointCount();
	int_max NamedFaceCount = InputMesh.GetNamedFaceCount();
	auto PointSetCount = InputMesh.GetPointSetCount();
	auto FaceSetCount = InputMesh.GetFaceSetCount();
	JObject["PointCount"] = PointCount;
	JObject["FaceCount"] = FaceCount;
	JObject["NamedPointCount"] = NamedPointCount;
	JObject["NamedFaceCount"] = NamedFaceCount;
	JObject["PointSetCount"] = PointSetCount;
	JObject["FaceSetCount"] = FaceSetCount;
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
	if (PointSetCount > 0)
	{// MUST use JsonArray to maintain the order in PointSetList		
		JsonArray tempJArray;		
		tempJArray.Resize(PointSetCount);
		auto PointSetNameList = InputMesh.GetPointSetName(ALL);	
		for (int_max PointSetIndex = 0; PointSetIndex < PointSetCount; ++PointSetIndex)
		{
			JsonObject tempJObject;
			auto PointSet = InputMesh.GetPointSet(PointSetIndex);
			tempJObject[PointSetNameList[PointSetIndex]] = PointSet;
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
		auto FaceSetNameList = InputMesh.GetFaceSetName(ALL);
		for (int_max FaceSetIndex = 0; FaceSetIndex < FaceSetCount; ++FaceSetIndex)
		{
			JsonObject tempJObject;
			auto FaceSet = InputMesh.GetFaceSet(FaceSetIndex);
			tempJObject[FaceSetNameList[FaceSetIndex]] = FaceSet;
			tempJArray[FaceSetIndex] = tempJObject;			
		}
		JsonValue tempJValue = std::move(tempJArray);
		JObject["FaceSetList"] = tempJValue;
	}	
	//----------------------------------------
	String FilePath = ExtractFilePath(FilePathAndName);
	String FileName_VTKMesh = ExtractFileName(FilePathAndName) + ".vtk";
	JObject["PointAndFace"] = FileName_VTKMesh;
	//----------------------------------------------------------------------------------//
	bool IsOK = true;
	if (SavePolygonMeshAsVTKFile(InputMesh, FilePath + FileName_VTKMesh) == false)
	{
		MDK_Error("Error1 @ SavePolygonMeshAsJsonDataFile(...)")
		IsOK = false;
	}
	if (JsonFile::Save(JObject, FilePathAndName) == false)
	{
		MDK_Error("Error2 @ SavePolygonMeshAsJsonDataFile(...)")
		IsOK = false;
	}
	return IsOK;
}


template<typename MeshAttributeType>
bool LoadPolygonMeshFromJsonDataFile(PolygonMesh<MeshAttributeType>& OutputMesh, const String& FilePathAndName)
{
	typedef MeshAttributeType::ScalarType  ScalarType;
	//---------------------------------------------
	if (GetByteCountOfScalar(ScalarType(0)) <= 0)
	{
		MDK_Error("Unknown ScalarType @ SavePolygonMeshAsJsonDataFile(...)")
		return false;
	}
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
	//----------------------------------------------------------//
	{
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
	}
	//----------------------------------------------------------//
	{
		auto it = JObject.find("PointAndFace");
		if (it != JObject.end())
		{
			String FilePath = ExtractFilePath(FilePathAndName);
			String FileName_VTKMesh = it->second.GetString();
			if (LoadPolygonMeshFromVTKFile(OutputMesh, FilePath + FileName_VTKMesh) == false)
			{
				return false;
			}
		}
	}
	//----------------------------------------------------------//	
	{
		auto it = JObject.find("ID");
		if (it != JObject.end())
		{
			OutputMesh.SetID(it->second.ToScalar<int_max>());
		}
		else
		{
			MDK_Error("Couldn't get ID @ LoadPolygonMeshFromJsonDataFile(...)")
			return false;
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
			MDK_Error("Couldn't get Name @ LoadPolygonMeshFromJsonDataFile(...)")
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
			MDK_Error("Couldn't get ScalarType @ LoadPolygonMeshFromJsonDataFile(...)")
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
			MDK_Error("Couldn't get IndexType @ LoadPolygonMeshFromJsonDataFile(...)")
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
			MDK_Error("Couldn't get PointCount @ LoadPolygonMeshFromJsonDataFile(...)")
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
			MDK_Error("Couldn't get FaceCount @ LoadPolygonMeshFromJsonDataFile(...)")
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
			MDK_Error("Couldn't get PointSetCount @ LoadPolygonMeshFromJsonDataFile(...)")
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
			MDK_Error("Couldn't get FaceSetCount @ LoadPolygonMeshFromJsonDataFile(...)")
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
					OutputMesh.SetPointSet(PointSetName, PointIndexList);					
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
					OutputMesh.SetFaceSet(FaceSetName, FaceIndexList);
				}
			}
		}
	}
	//-----------------
	return true;
}


template<typename MeshAttributeType>
bool SavePolygonMeshAsVTKFile(const PolygonMesh<MeshAttributeType>& InputMesh, const String& FilePathAndName)
{
	auto VTKMesh = ConvertMDKPolygonMeshToVTKPolyData(InputMesh);
	return SaveVTKPolyDataAsVTKFile(VTKMesh, FilePathAndName);
}


template<typename MeshAttributeType>
bool LoadPolygonMeshFromVTKFile(PolygonMesh<MeshAttributeType>& OutputMesh, const String& FilePathAndName)
{
	auto VTKMesh = LoadVTKPolyDataFromVTKFile(FilePathAndName);
	return ConvertVTKPolyDataToMDKPolygonMesh(VTKMesh, OutputMesh);
}


template<typename MeshAttributeType>
bool SavePolygonMeshAsPLYFile(const PolygonMesh<MeshAttributeType>& InputMesh, const String& FilePathAndName)
{
	auto VTKMesh = ConvertMDKPolygonMeshToVTKPolyData(InputMesh);
	return SaveVTKPolyDataAsPLYFile(VTKMesh, FilePathAndName);
}

template<typename MeshAttributeType>
bool LoadPolygonMeshMeshFromPLYFile(PolygonMesh<MeshAttributeType>& OutputMesh, const String& FilePathAndName)
{
	auto VTKMesh = LoadVTKPolyDataFromPLYFile(FilePathAndName);
	return ConvertVTKPolyDataToMDKPolygonMesh(VTKMesh, OutputMesh);
}

}//namespace mdk

