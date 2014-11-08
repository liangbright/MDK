#ifndef __mdkFileIO_DenseMatrix_hpp
#define __mdkFileIO_DenseMatrix_hpp

namespace mdk
{

template<typename ScalarType>
bool SaveDenseMatrixAsJsonDataFile(const DenseMatrix<ScalarType>& InputMatrix, const std::string& JsonFilePathAndName)
{
	if (SaveDenseMatrixAsJsonDataFile_Header(InputMatrix, JsonFilePathAndName) == false)
	{
		return false;
	}

	std::string DataFilePathAndName = JsonFilePathAndName + ".data";
	return SaveDenseMatrixAsJsonDataFile_Data(InputMatrix, DataFilePathAndName);
}


template<typename ScalarType>
bool SaveDenseMatrixAsJsonDataFile_Header(const DenseMatrix<ScalarType>& InputMatrix, const std::string& JsonFilePathAndName)
{
	std::vector<NameValueQStringPair> PairList(4);

	auto Size = InputMatrix.GetSize();

	PairList[0].Name = "ObjectType";
	PairList[0].Value = "DenseMatrix";

	auto ScalarTypeName = GetScalarTypeName(ScalarType(0));
	QString QScalarTypeName(ScalarTypeName.c_str());

	PairList[1].Name = "ScalarType";
	PairList[1].Value = QScalarTypeName;

	PairList[2].Name = "RowNumber";
	PairList[2].Value = QString::number(Size.RowNumber);

	PairList[3].Name = "ColNumber";
	PairList[3].Value = QString::number(Size.ColNumber);

	// write header file (json) --------------------------------------------------
	QString QFilePathAndName(JsonFilePathAndName.c_str());
	return SaveNameValuePairListAsJsonFile(PairList, QFilePathAndName);
}


template<typename ScalarType>
bool SaveDenseMatrixAsJsonDataFile_Data(const DenseMatrix<ScalarType>& InputMatrix, const std::string& DataFilePathAndName)
{
	auto Size = InputMatrix.GetSize();
	if (Size.ColNumber == 0)
	{
		MDK_Warning("InputMatrix is empty @ SaveDenseMatrixAsJsonDataFile_Data(...)")
		return true;
	}

	int_max ByteNumber = GetByteNumberOfScalar(ScalarType(0));
	if (ByteNumber <= 0)
	{
		MDK_Error("Unknown type of matrix @ SaveDenseMatrixAsJsonDataFile_Data(...)")
		return false;
	}

	QFile DataFile(DataFilePathAndName.c_str());
	if (!DataFile.open(QIODevice::WriteOnly))
	{
		MDK_Error("Couldn't open file " << DataFilePathAndName << " @ SaveDenseMatrixAsJsonDataFile_Data(...)")
		return false;
	}
	DataFile.write((char*)InputMatrix.GetElementPointer(), Size.ColNumber*Size.RowNumber*ByteNumber);
	DataFile.flush();
	DataFile.close();
	return true;
}


template<typename ScalarType>
bool LoadDenseMatrixFromJsonDataFile(DenseMatrix<ScalarType>& OutputMatrix, const std::string& JsonFilePathAndName)
{
    // Read header
	auto HeaderInfo = LoadDenseMatrixFromJsonDataFile_Header(JsonFilePathAndName);
	auto OutputMatrixSize = HeaderInfo.first;
	auto InputScalarTypeName = HeaderInfo.second;

	if (OutputMatrixSize.RowNumber == 0 || OutputMatrixSize.ColNumber == 0)
	{
		OutputMatrix.Clear();
		return true;
	}

    // Read data

	OutputMatrix.FastResize(OutputMatrixSize);

	std::string DataFilePathAndName = JsonFilePathAndName + ".data";

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

    return true;
}


inline std::pair<MatrixSize, std::string> LoadDenseMatrixFromJsonDataFile_Header(const std::string& FilePathAndName)
{
	std::pair<MatrixSize, std::string> HeaderInfo;
	HeaderInfo.first.RowNumber = 0;
	HeaderInfo.first.ColNumber = 0;

	QString QFilePathAndName(FilePathAndName.c_str());
    QFile HeaderFile(QFilePathAndName);
    if (!HeaderFile.open(QIODevice::ReadOnly))
    {
        MDK_Error("Couldn't open Header File @ LoadDenseMatrixFromJsonDataFile_Header(...)")
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
			MDK_Error("ObjectType is not DenseMatrix @ LoadDenseMatrixFromJsonDataFile_Header(...)")
			HeaderFile.close();
			return HeaderInfo;
		}
	}
	else
	{
		MDK_Error("Couldn't get ObjectType @ LoadDenseMatrixFromJsonDataFile_Header(...)")
		HeaderFile.close();
		return HeaderInfo;
	}
	//-------------------------------------------------
    std::string InputScalarTypeName;
    it = HeaderObject.find("ScalarType");
    if (it != HeaderObject.end())
    {
        InputScalarTypeName = it.value().toString().toStdString();
    }
    else
    {
        MDK_Error("Couldn't get ScalarType @ LoadDenseMatrixFromJsonDataFile_Header(...)")
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
        MDK_Error("Couldn't get RowNumber @ LoadDenseMatrixFromJsonDataFile_Header(...)")
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
        MDK_Error("Couldn't get ColNumber @ LoadDenseMatrixFromJsonDataFile_Header(...)")
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
bool LoadDenseMatrixFromJsonDataFile_Data(DenseMatrix<OutputScalarType>& OutputMatrix, const std::string& DataFilePathAndName, const std::string& InputScalarTypeName)
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
bool LoadDenseMatrixFromJsonDataFile_Data(DenseMatrix<OutputScalarType>& OutputMatrix, const std::string& DataFilePathAndName)
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
