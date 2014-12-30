#ifndef __mdkDenseMatrix_FileIO_hpp
#define __mdkDenseMatrix_FileIO_hpp

namespace mdk
{

template<typename ScalarType>
bool SaveDenseMatrixAsJsonDataFile(const DenseMatrix<ScalarType>& InputMatrix, const String& FilePathAndName)
{
	String FilePath = ExtractFilePath(FilePathAndName);
	String FileName = ExtractFileName(FilePathAndName);

	String DataFileName = FileName + ".Scalar.data";

	JsonObject JObject;
	JObject["ObjectType"] = "DenseMatrix";
	JObject["ScalarType"] = GetScalarTypeName(ScalarType(0));
	JObject["RowNumber"] = InputMatrix.GetRowNumber();
	JObject["ColNumber"] = InputMatrix.GetColNumber();
	JObject["ScalarArray"] = DataFileName;

	bool IsOK = true;
	if (JsonFile::Save(JObject, FilePathAndName) == false)
	{
		IsOK = false;
	}
	if (SaveScalarArrayAsDataFile(InputMatrix.GetElementPointer(), InputMatrix.GetElementNumber(), FilePath + DataFileName) == false)
	{
		IsOK = false;
	}
	return IsOK;
}


template<typename ScalarType>
bool LoadDenseMatrixFromJsonDataFile(DenseMatrix<ScalarType>& OutputMatrix, const String& FilePathAndName)
{
	JsonObject JObject;
	if (JsonFile::Load(JObject, FilePathAndName) == false)
	{
		MDK_Error("Json file is invalid @ LoadDenseMatrixFromJsonDataFile(...)")
		return false;
	}

	if (JObject.IsEmpty() == true)
	{
		return true;
	}
	//-------------------------------------------------
	auto it = JObject.find("ObjectType");
	if (it != JObject.end())
	{
		auto ObjectType = it->second.GetString();
		if (ObjectType != "DenseMatrix")
		{
			MDK_Error("ObjectType is not DenseMatrix @ LoadDenseMatrixFromJsonDataFile(...)")
			return false;
		}
	}
	else
	{
		MDK_Error("Couldn't get ObjectType @ LoadDenseMatrixFromJsonDataFile(...)")
		return false;
	}
	//-------------------------------------------------
	String ScalarTypeInDataFile;
	it = JObject.find("ScalarType");
	if (it != JObject.end())
	{
		ScalarTypeInDataFile = it->second.GetString();
	}
	else
	{
		MDK_Error("Couldn't get ScalarType @ LoadDenseMatrixFromJsonDataFile(...)")
		return false;
	}
	//-------------------------------------------------
	int_max RowNumber = 0;
	it = JObject.find("RowNumber");
	if (it != JObject.end())
	{
		RowNumber = it->second.ToScalar<int_max>();
	}
	else
	{
		MDK_Error("Couldn't get RowNumber @ LoadDenseMatrixFromJsonDataFile(...)")
		return false;
	}
	//-------------------------------------------------
	int_max ColNumber = 0;
	it = JObject.find("ColNumber");
	if (it != JObject.end())
	{
		ColNumber = it->second.ToScalar<int_max>();
	}
	else
	{
		MDK_Error("Couldn't get ColNumber @ LoadDenseMatrixFromJsonDataFile(...)")
		return false;
	}
	//-------------------------------------------------
	String DataFilePathAndName= ExtractFilePath(FilePathAndName);
	it = JObject.find("ScalarArray");
	if (it != JObject.end())
	{
		DataFilePathAndName += it->second.GetString();
	}
	else
	{
		MDK_Error("Couldn't get ScalarArray @ LoadDenseMatrixFromJsonDataFile(...)")
		return false;
	}
	//-------------------------------------------------
	OutputMatrix.FastResize(RowNumber, ColNumber);
	// Read data
	if (LoadScalarArrayFromDataFile(OutputMatrix.GetElementPointer(), OutputMatrix.GetElementNumber(), DataFilePathAndName, ScalarTypeInDataFile) == false)
	{
		OutputMatrix.Clear();
		return false;
	}
	return true;
}

}//namespace mdk

#endif
