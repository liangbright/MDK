#ifndef __mdkFileIO_DenseMatrix_hpp
#define __mdkFileIO_DenseMatrix_hpp

namespace mdk
{

template<typename ScalarType>
bool SaveDenseMatrixAsJsonDataFile(const DenseMatrix<ScalarType>& InputMatrix, const String& FilePathAndName)
{
	String FilePath = ExtractFilePath(FilePathAndName);
	String FileName = ExtractFileName(FilePathAndName);

	String FileName_ScalarArray = FileName + ".ScalarArray.data";

	JsonObject JObject;
	JObject["ObjectType"] = "DenseMatrix";
	JObject["ScalarType"] = GetScalarTypeName(ScalarType(0));
	JObject["RowNumber"] = InputMatrix.GetRowNumber();
	JObject["ColNumber"] = InputMatrix.GetColNumber();
	JObject["ScalarArray"] = FileName_ScalarArray;

	bool IsOK = true;

	if (JsonFile::Save(JObject, FilePathAndName) == false)
	{
		IsOK = false;
	}

	if (SaveScalarArrayAsDataFile(InputMatrix.GetElementPointer(), InputMatrix.GetElementNumber(), FilePath + FileName_ScalarArray) == false)
	{
		IsOK = false;
	}

	return true;
}


template<typename ScalarType>
bool LoadDenseMatrixFromJsonDataFile(DenseMatrix<ScalarType>& OutputMatrix, const String& FilePathAndName)
{
	JsonObject JObject;
	JsonFile::Load(JObject, FilePathAndName);
	//-------------------------------------------------
	auto it = JObject.find("ObjectType");
	if (it != JObject.end())
	{
		auto ObjectType = it->second.ToString();
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
		ScalarTypeInDataFile = it->second.ToString();
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
	String FilePathAndName_ScalarArray = ExtractFilePath(FilePathAndName);
	it = JObject.find("ScalarArray");
	if (it != JObject.end())
	{
		FilePathAndName_ScalarArray += it->second.ToString();
	}
	else
	{
		MDK_Error("Couldn't get ColNumber @ LoadDenseMatrixFromJsonDataFile(...)")
		return false;
	}
	//-------------------------------------------------
	OutputMatrix.FastResize(RowNumber, ColNumber);
	// Read data
	return LoadScalarArrayFromDataFile(OutputMatrix.GetElementPointer(), OutputMatrix.GetElementNumber(), FilePathAndName_ScalarArray, ScalarTypeInDataFile);
}

}//namespace mdk

#endif
