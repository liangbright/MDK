#ifndef __mdkFileIO_DenseMatrix_hpp
#define __mdkFileIO_DenseMatrix_hpp

namespace mdk
{

template<typename ScalarType>
bool SaveDenseMatrixAsJsonDataFile(const DenseMatrix<ScalarType>& InputMatrix, const String& FilePathAndName)
{
	auto JsonContent = SaveDenseMatrixAsJsonDataFile_Json(InputMatrix, FilePathAndName);
	if (JsonContent.IsEmpty() == true)
	{

		return false;
	}	

	QString QFilePathAndName(FilePathAndName.c_str());
	return SaveNameValuePairListAsJsonFile(PairList, QFilePathAndName);

	return SaveDenseMatrixAsJsonDataFile_Data(InputMatrix, FilePathAndName);
}


template<typename ScalarType>
DataArray<String> SaveDenseMatrixAsJsonDataFile_Json(const DenseMatrix<ScalarType>& InputMatrix, const String& FilePathAndName)
{
	DataArray<String> JsonContent;
	JsonContent.Resize(4);
	JsonContent[0] = ConvertNameValuePairToStringInJsonFile("ObjectType", "DenseMatrix");
	JsonContent[1] = ConvertNameValuePairToStringInJsonFile("ScalarType", GetScalarTypeName(ScalarType(0));
	JsonContent[2] = ConvertNameValuePairToStringInJsonFile("RowNumber", InputMatrix.GetRowNumber());
	JsonContent[3] = ConvertNameValuePairToStringInJsonFile("RowNumber", InputMatrix.GetColNumber());
	return JsonContent;
}


template<typename ScalarType>
bool SaveDenseMatrixAsJsonDataFile_Data(const DenseMatrix<ScalarType>& InputMatrix, const String& FilePathAndName)
{
	String DataFilePathAndName = FilePathAndName + ".data";

	int_max ByteNumber = GetByteNumberOfScalar(ScalarType(0));
	if (ByteNumber <= 0)
	{
		MDK_Error("Unknown type of matrix @ SaveDenseMatrixAsJsonDataFile_Data(...)")
		return false;
	}

	QFile DataFile(DataFilePathAndName.c_str());
	if (!DataFile.open(QIODevice::WriteOnly))
	{
		MDK_Error("Couldn't create file " << DataFilePathAndName << " @ SaveDenseMatrixAsJsonDataFile_Data(...)")
		return false;
	}

	auto Size = InputMatrix.GetSize();
	if (Size.ColNumber > 0)
	{
		DataFile.write((char*)InputMatrix.GetElementPointer(), Size.ColNumber*Size.RowNumber*ByteNumber);
		DataFile.flush();		
	}
	DataFile.close();
	return true;
}


template<typename ScalarType>
bool LoadDenseMatrixFromJsonDataFile(DenseMatrix<ScalarType>& OutputMatrix, const String& FilePathAndName)
{
    // Read header
	auto HeaderInfo = LoadDenseMatrixFromJsonDataFile_Json(FilePathAndName);
	auto OutputMatrixSize = HeaderInfo.first;
	auto InputScalarTypeName = HeaderInfo.second;

	if (OutputMatrixSize.RowNumber == 0 || OutputMatrixSize.ColNumber == 0)
	{
		OutputMatrix.Clear();
		return true;
	}

	OutputMatrix.FastResize(OutputMatrixSize);
	// Read data
	if (OutputMatrix.IsEmpty() == false)
	{
		String DataFilePathAndName = FilePathAndName + ".data";

		auto OutputScalarTypeName = GetScalarTypeName(ScalarType(0));

		if (OutputScalarTypeName == InputScalarTypeName)
		{
			if (LoadDenseMatrixFromJsonDataFile_Data<ScalarType, ScalarType>(OutputMatrix, DataFilePathAndName) == false)
			{
				OutputMatrix.Clear();
				return false;
			}
		}
		else
		{
			MDK_Warning("OutputScalarTypeName != InputScalarTypeName, Output may be inaccurate @ LoadDenseMatrixFromJsonDataFile(...)")
			if (LoadDenseMatrixFromJsonDataFile_Data<ScalarType>(OutputMatrix, DataFilePathAndName, InputScalarTypeName) == false)
			{
				OutputMatrix.Clear();
				return false;
			}
		}
	}
    return true;
}


inline std::pair<MatrixSize, String> LoadDenseMatrixFromJsonDataFile_Json(const String& FilePathAndName)
{
	std::pair<MatrixSize, String> HeaderInfo;
	HeaderInfo.first.RowNumber = 0;
	HeaderInfo.first.ColNumber = 0;

	QString QFilePathAndName(FilePathAndName.c_str());
    QFile HeaderFile(QFilePathAndName);
    if (!HeaderFile.open(QIODevice::ReadOnly))
    {
        MDK_Error("Couldn't open Header File @ LoadDenseMatrixFromJsonDataFile_Json(...)")
        return HeaderInfo;
    }

    //----------------------------------------------------------//
    QByteArray HeaderContent = HeaderFile.readAll();
    QJsonDocument HeaderDoc(QJsonDocument::fromJson(HeaderContent));
    QJsonObject HeaderObject = HeaderDoc.object();
    //-----------------------------------------------------------//

	auto it = HeaderObject.find("ObjectType");
	if (it != HeaderObject.end())
	{
		auto ObjectType = it.value().toString();

		if (ObjectType != "DenseMatrix")
		{
			MDK_Error("ObjectType is not DenseMatrix @ LoadDenseMatrixFromJsonDataFile_Json(...)")
			HeaderFile.close();
			return HeaderInfo;
		}
	}
	else
	{
		MDK_Error("Couldn't get ObjectType @ LoadDenseMatrixFromJsonDataFile_Json(...)")
		HeaderFile.close();
		return HeaderInfo;
	}
	//-------------------------------------------------
    String InputScalarTypeName;
    it = HeaderObject.find("ScalarType");
    if (it != HeaderObject.end())
    {
        InputScalarTypeName = it.value().toString().toStdString();
    }
    else
    {
        MDK_Error("Couldn't get ScalarType @ LoadDenseMatrixFromJsonDataFile_Json(...)")
        HeaderFile.close();
		return HeaderInfo;
    }
	//-------------------------------------------------
    int_max RowNumber = 0;
    it = HeaderObject.find("RowNumber");
    if (it != HeaderObject.end())
    {
        RowNumber = it.value().toString().toLongLong();
    }
    else
    {
        MDK_Error("Couldn't get RowNumber @ LoadDenseMatrixFromJsonDataFile_Json(...)")
        HeaderFile.close();
		return HeaderInfo;
    }
	//-------------------------------------------------
    int_max ColNumber = 0;
    it = HeaderObject.find("ColNumber");
    if (it != HeaderObject.end())
    {
        ColNumber = it.value().toString().toLongLong();
    }
    else
    {
        MDK_Error("Couldn't get ColNumber @ LoadDenseMatrixFromJsonDataFile_Json(...)")
        HeaderFile.close();
		return HeaderInfo;
    }
	//-------------------------------------------------
    HeaderFile.close();

	HeaderInfo.first.RowNumber = RowNumber;
	HeaderInfo.first.ColNumber = ColNumber;
	HeaderInfo.second = InputScalarTypeName;

	return HeaderInfo;
}


template<typename OutputScalarType>
bool LoadDenseMatrixFromJsonDataFile_Data(DenseMatrix<OutputScalarType>& OutputMatrix, const String& DataFilePathAndName, const String& InputScalarTypeName)
{
    if (InputScalarTypeName == "double")
    {
		return LoadDenseMatrixFromJsonDataFile_Data<OutputScalarType, double>(OutputMatrix, DataFilePathAndName);
    }
    else if (InputScalarTypeName == "float")
    {
		return LoadDenseMatrixFromJsonDataFile_Data<OutputScalarType, float>(OutputMatrix, DataFilePathAndName);
    }
    else if (InputScalarTypeName == "int8")
    {
		return LoadDenseMatrixFromJsonDataFile_Data<OutputScalarType, int8>(OutputMatrix, DataFilePathAndName);
    }
    else if (InputScalarTypeName == "int16")
    {
		return LoadDenseMatrixFromJsonDataFile_Data<OutputScalarType, int16>(OutputMatrix, DataFilePathAndName);
    }
    else if (InputScalarTypeName == "int32")
    {
		return LoadDenseMatrixFromJsonDataFile_Data<OutputScalarType, int32>(OutputMatrix, DataFilePathAndName);
    }
    else if (InputScalarTypeName == "int64")
    {
		return LoadDenseMatrixFromJsonDataFile_Data<OutputScalarType, int64>(OutputMatrix, DataFilePathAndName);
    }
    else if (InputScalarTypeName == "uint8")
    {
		return LoadDenseMatrixFromJsonDataFile_Data<OutputScalarType, uint8>(OutputMatrix, DataFilePathAndName);
    }
    else if (InputScalarTypeName == "uint16")
    {
		return LoadDenseMatrixFromJsonDataFile_Data<OutputScalarType, uint16>(OutputMatrix, DataFilePathAndName);
    }
    else if (InputScalarTypeName == "uint32")
    {
		return LoadDenseMatrixFromJsonDataFile_Data<OutputScalarType, uint32>(OutputMatrix, DataFilePathAndName);
    }
    else if (InputScalarTypeName == "uint64")
    {
		return LoadDenseMatrixFromJsonDataFile_Data<OutputScalarType, uint64>(OutputMatrix, DataFilePathAndName);
    }
    else
    {
        MDK_Error("unknown ScalarType of data file @ LoadDenseMatrixFromJsonDataFile_Data(...) ")
		return false;
    }	
}


template<typename OutputScalarType, typename InputScalarType>
bool LoadDenseMatrixFromJsonDataFile_Data(DenseMatrix<OutputScalarType>& OutputMatrix, const String& DataFilePathAndName)
{
	QFile DataFile(DataFilePathAndName.c_str());
	if (!DataFile.open(QIODevice::ReadOnly))
	{
		MDK_Error("Couldn't open data file:" << DataFilePathAndName)
		return false;
	}
	int_max BypesofDataFile = DataFile.size();

	auto OutputMatrixSize = OutputMatrix.GetSize();
	int_max ElementNumber = OutputMatrix.GetElementNumber();
	int_max ByteNumberOfInputScalarType = GetByteNumberOfScalar(InputScalarType(0));   

    if (BypesofDataFile != ElementNumber * ByteNumberOfInputScalarType)
    {
        MDK_Error("Data file size is not equal to matrix size @ LoadDenseMatrixFromJsonDataFile_Data(...)")
		DataFile.close();
		return false;
    }

    for (int_max i = 0; i < ElementNumber; ++i)
    {
		auto tempScalar = InputScalarType(0);

        DataFile.read((char*)(&tempScalar), ByteNumberOfInputScalarType);

		OutputMatrix[i] = OutputScalarType(tempScalar);
    }
	DataFile.close();

	return true;
}

}//namespace mdk

#endif
