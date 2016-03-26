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
	//-------------------------------------------------------------------------------------
	JsonObject JObject;
	JObject["ObjectType"] = "PolygonMesh";
	JObject["ScalarType"] = GetScalarTypeName(ScalarType(0));
	JObject["IndexType"] = GetScalarTypeName(int_max(0));
	JObject["PointCount"] = InputMesh.GetPointCount();
	JObject["FaceCount"] = InputMesh.GetFaceCount();	
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
	//------------------
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

//--------------------------------------- standard ----------------------------------------------------------------------//

template<typename ScalarType>
bool SavePolygonMeshAsJsonDataFile(const PolygonMesh<PolygonMeshStandardAttributeType<ScalarType>>& InputMesh, const String& FilePathAndName)
{
	//---------------------------------------------
	if (GetByteCountOfScalar(ScalarType(0)) <= 0)
	{
		MDK_Error("Unknown ScalarType @ SavePolygonMeshAsJsonDataFile(...)")
		return false;
	}
	//-------------------------------------------------------------------//
	JsonObject JObject;

	JObject["ObjectType"] = "PolygonMesh";
	JObject["ID"] = InputMesh.Attribute().ID;
	JObject["Name"] = InputMesh.Attribute().Name;
	JObject["ScalarType"] = GetScalarTypeName(ScalarType(0));
	JObject["IndexType"] = GetScalarTypeName(int_max(0));
	//----------------------------------------
	auto PointCount = InputMesh.GetPointCount();
	auto FaceCount = InputMesh.GetFaceCount();
	int_max NamePointCount = InputMesh.Attribute().Map_PointName_to_PointHandle.size();
	int_max NameFaceCount = InputMesh.Attribute().Map_FaceName_to_FaceHandle.size();
	auto PointSetCount = InputMesh.Attribute().PointSetList.GetLength();
	auto FaceSetCount = InputMesh.Attribute().FaceSetList.GetLength();	
	JObject["PointCount"] = PointCount;
	JObject["FaceCount"] = FaceCount;
	JObject["NamePointCount"] = NamePointCount;
	JObject["NameFaceCount"] = NameFaceCount;
	JObject["PointSetCount"] = PointSetCount;
	JObject["FaceSetCount"] = FaceSetCount;
	//----------------------------------------
	if (InputMesh.Attribute().Map_PointName_to_PointHandle.empty() == false)
	{
		JsonObject tempJObject;
		for (auto it = InputMesh.Attribute().Map_PointName_to_PointHandle.begin(); it != InputMesh.Attribute().Map_PointName_to_PointHandle.end(); ++it)
		{
			const auto& PointName = it->first;
			auto PointIndex = it->second.GetIndex();
		    tempJObject[PointName] = PointIndex;			
		}
		JsonValue tempJValue = std::move(tempJObject);
		JObject["NamedPointList"] = tempJValue;
	}
	else
	{
		MDK_Error("PointNameList is wrong @ SavePolygonMeshAsJsonDataFile(...)")
		return false;
	}
	//----------------------------------------
	if (InputMesh.Attribute().Map_FaceName_to_FaceHandle.empty() == false)
	{
		JsonObject tempJObject;
		for (auto it = InputMesh.Attribute().Map_FaceName_to_FaceHandle.begin(); it != InputMesh.Attribute().Map_FaceName_to_FaceHandle.end(); ++it)
		{
			const auto& FaceName = it->first;
			auto FaceIndex = it->second.GetIndex();
			tempJObject[FaceName] = FaceIndex;
		}
		JsonValue tempJValue = std::move(tempJObject);
		JObject["NamedFaceList"] = tempJValue;
	}
	else
	{
		MDK_Error("FaceNameList is wrong @ SavePolygonMeshAsJsonDataFile(...)")
		return false;
	}
	//----------------------------------------
	if (InputMesh.Attribute().PointSetList.IsEmpty() == false)
	{// MUST use JsonArray to maintain the order in PointSetList		
		JsonArray tempJArray;		
		tempJArray.Resize(PointSetCount);
		DenseVector<int> FlagList;
		FlagList.Resize(PointSetCount);
		FlagList.Fill(0);		
		for (auto it = InputMesh.Attribute().Map_PointSetName_to_PointSetIndex.begin(); it != InputMesh.Attribute().Map_PointSetName_to_PointSetIndex.end(); ++it)
		{
			const auto& PointSetName = it->first;
			auto PointSetIndex = it->second;
			if (PointSetIndex >= PointSetCount)
			{
				MDK_Error("Map_PointSetName_to_PointSetIndex is wrong @  SavePolygonMeshAsJsonDataFile(...)")
				return false;
			}
			JsonObject tempJObject;
			tempJObject[PointSetName] = InputMesh.ConvertHandleToIndex(InputMesh.Attribute().PointSetList[PointSetIndex]);
			tempJArray[PointSetIndex] = tempJObject;
			FlagList[PointSetIndex] = 1;
		}
		for (int_max k = 0; k < PointSetCount; ++k)
		{
			if (FlagList[k] == 0)
			{
				JsonObject tempJObject;
				tempJObject[""] = InputMesh.ConvertHandleToIndex(InputMesh.Attribute().PointSetList[k]);
				tempJArray[k] = tempJObject;
			}
		}
		JsonValue tempJValue = std::move(tempJArray);
		JObject["PointSetList"] = tempJValue;
	}
	else
	{
		MDK_Error("PointSetList and PointSetNameList NOT the same length @ SavePolygonMeshAsJsonDataFile(...)")
		return false;
	}
	//----------------------------------------
	if (InputMesh.Attribute().Map_FaceSetName_to_FaceSetIndex.empty() == false)
	{// MUST use JsonArray to maintain the order in FaceSetList
		JsonArray tempJArray;		
		tempJArray.Resize(FaceSetCount);
		DenseVector<int> FlagList;
		FlagList.Resize(FaceSetCount);
		FlagList.Fill(0);		
		for (auto it = InputMesh.Attribute().Map_FaceSetName_to_FaceSetIndex.begin(); it != InputMesh.Attribute().Map_FaceSetName_to_FaceSetIndex.end(); ++it)
		{
			const auto& FaceSetName = it->first;
			auto FaceSetIndex = it->second;
			if (FaceSetIndex >= FaceSetCount)
			{
				MDK_Error("Map_FaceSetName_to_FaceSetIndex is wrong @ SavePolygonMeshAsJsonDataFile(...)")
				return false;
			}
			JsonObject tempJObject;
			tempJObject[FaceSetName] = InputMesh.ConvertHandleToIndex(InputMesh.Attribute().FaceSetList[FaceSetIndex]);
			tempJArray[FaceSetIndex] = tempJObject;
			FlagList[FaceSetIndex] = 1;
		}
		for (int_max k = 0; k < FaceSetCount; ++k)
		{
			if (FlagList[k] == 0)
			{
				JsonObject tempJObject;
				tempJObject[""] = InputMesh.ConvertHandleToIndex(InputMesh.Attribute().FaceSetList[k]);
				tempJArray[k] = tempJObject;
			}
		}
		JsonValue tempJValue = std::move(tempJArray);
		JObject["FaceSetList"] = tempJValue;
	}
	else
	{
		MDK_Error("FaceSetList and FaceSetNameList NOT the same length @ SavePolygonMeshAsJsonDataFile(...)")
		return false;
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


template<typename ScalarType>
bool LoadPolygonMeshFromJsonDataFile(PolygonMesh<PolygonMeshStandardAttributeType<ScalarType>>& OutputMesh, const String& FilePathAndName)
{
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
			OutputMesh.Attribute().ID = it->second.ToScalar<int_max>();
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
			OutputMesh.Attribute().Name = it->second.GetString();
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
				OutputMesh.Attribute().Map_PointName_to_PointHandle[PointName].SetIndex(PointIndex);
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
				const auto& PointName = it->first;
				auto PointIndex = it->second.ToScalar<int_max>();
				OutputMesh.Attribute().Map_FaceName_to_FaceHandle[PointName].SetIndex(PointIndex);
			}
		}
	}
	//----------------------------------------
	{
		auto it = JObject.find("PointSetList");
		if (it != JObject.end())
		{
			auto tempJArray = it->second.GetJsonArray();
			OutputMesh.Attribute().PointSetList.Resize(tempJArray.GetLength());
			for (int_max k = 0; k < tempJArray.GetLength(); ++k)
			{
				auto tempJObject = tempJArray[k].GetJsonObject();
				auto it = tempJObject.begin();
				if(it != tempJObject.end())
				{
					const auto& PointSetName = it->first;
					auto PointIndexList = it->second.ToScalarArray<int_max>();
					OutputMesh.Attribute().Map_PointSetName_to_PointSetIndex[PointSetName] = k;
					OutputMesh.Attribute().PointSetList[k] = OutputMesh.ConvertPointIndexToPointHandle(PointIndexList);
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
			OutputMesh.Attribute().FaceSetList.Resize(tempJArray.GetLength());
			for (int_max k = 0; k < tempJArray.GetLength(); ++k)
			{
				auto tempJObject = tempJArray[k].GetJsonObject();
				auto it = tempJObject.begin(); 
				if(it != tempJObject.end())
				{
					const auto& FaceSetName = it->first;
					auto FaceIndexListOfFaceSet = it->second.ToScalarArray<int_max>();
					OutputMesh.Attribute().Map_FaceSetName_to_FaceSetIndex[FaceSetName] = k;
					OutputMesh.Attribute().FaceSetList[k] = OutputMesh.ConvertFaceIndexToFaceHandle(FaceIndexListOfFaceSet);
				}
			}
		}
	}
	//-----------------
	return true;
}

}//namespace mdk
