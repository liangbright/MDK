#ifndef __mdkFileIO_DenseMatrix_hpp
#define __mdkFileIO_DenseMatrix_hpp

namespace mdk
{

template<typename ElementType>
bool SaveDenseMatrixAsJsonDataFile(const DenseMatrix<ElementType>& InputMatrix, const std::string& JsonFilePathAndName)
{
	if (SaveDenseMatrixAsJsonDataFile_Header(InputMatrix, JsonFilePathAndName) == false)
	{
		return false;
	}

	std::string DataFilePathAndName = JsonFilePathAndName + ".data";
	return SaveDenseMatrixAsJsonDataFile_Data(InputMatrix, DataFilePathAndName);
}


template<typename ElementType>
bool SaveDenseMatrixAsJsonDataFile_Header(const DenseMatrix<ElementType>& InputMatrix, const std::string& JsonFilePathAndName)
{
	std::vector<NameValueQStringPair> PairList(4);

	auto Size = InputMatrix.GetSize();

	PairList[0].Name = "ObjectType";
	PairList[0].Value = "DenseMatrix";

	auto ElementTypeName = GetScalarTypeName(ElementType(0));
	QString QElementTypeName(ElementTypeName.c_str());

	PairList[1].Name = "ElementType";
	PairList[1].Value = QElementTypeName;

	PairList[2].Name = "RowNumber";
	PairList[2].Value = QString::number(Size.RowNumber);

	PairList[3].Name = "ColNumber";
	PairList[3].Value = QString::number(Size.ColNumber);

	// write header file (json) --------------------------------------------------
	QString QFilePathAndName(JsonFilePathAndName.c_str());
	return SaveNameValuePairListAsJsonFile(PairList, QFilePathAndName);
}


template<typename ElementType>
bool SaveDenseMatrixAsJsonDataFile_Data(const DenseMatrix<ElementType>& InputMatrix, const std::string& DataFilePathAndName)
{
	auto Size = InputMatrix.GetSize();
	if (Size.ColNumber == 0)
	{
		MDK_Warning("InputMatrix is empty @ SaveDenseMatrixAsJsonDataFile_Data(...)")
		return true;
	}

	int_max ByteNumber = GetByteNumberOfScalar(ElementType(0));
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


template<typename ElementType>
DenseMatrix<ElementType> LoadDenseMatrixFromJsonDataFile(const std::string& JsonFilePathAndName)
{
    DenseMatrix<ElementType> OutputMatrix;

    // Read header
	auto HeaderInfo = LoadDenseMatrixFromJsonDataFile_Header(JsonFilePathAndName);
	auto OutputMatrixSize = HeaderInfo.first;
	auto InputElementTypeName = HeaderInfo.second;

	if (OutputMatrixSize.RowNumber == 0 || OutputMatrixSize.ColNumber == 0)
	{
		return OutputMatrix;
	}

    // Read data

	std::string DataFilePathAndName = JsonFilePathAndName + ".data";

	auto OutputElementTypeName = GetScalarTypeName(ElementType(0));

    if (OutputElementTypeName == InputElementTypeName)
    {
		OutputMatrix = LoadDenseMatrixFromJsonDataFile_Data<ElementType, ElementType>(DataFilePathAndName, OutputMatrixSize);
    }
    else
    {
        MDK_Warning("OutputElementTypeName != InputElementTypeName, Output may be inaccurate @ LoadDenseMatrixFromJsonDataFile(...)")

		OutputMatrix = LoadDenseMatrixFromJsonDataFile_Data<ElementType>(DataFilePathAndName, OutputMatrixSize, InputElementTypeName);
    }

    return OutputMatrix;
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
    std::string InputElementTypeName;
    it = HeaderObject.find("ElementType");
    if (it != HeaderObject.end())
    {
        InputElementTypeName = it.value().toString().toStdString();
    }
    else
    {
        MDK_Error("Couldn't get ElementType @ LoadDenseMatrixFromJsonDataFile_Header(...)")
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
	HeaderInfo.second = InputElementTypeName;

	return HeaderInfo;
}


template<typename OutputElementType>
DenseMatrix<OutputElementType> 
LoadDenseMatrixFromJsonDataFile_Data(const std::string& DataFilePathAndName, MatrixSize OutputMatrixSize, const std::string& InputElementTypeName)
{
	DenseMatrix<OutputElementType> OutputMatrix;

    if (InputElementTypeName == "double")
    {
		OutputMatrix = LoadDenseMatrixFromJsonDataFile_Data<OutputElementType, double>(DataFilePathAndName, OutputMatrixSize);
    }
    else if (InputElementTypeName == "float")
    {
		OutputMatrix = LoadDenseMatrixFromJsonDataFile_Data<OutputElementType, float>(DataFilePathAndName, OutputMatrixSize);
    }
    else if (InputElementTypeName == "int8")
    {
		OutputMatrix = LoadDenseMatrixFromJsonDataFile_Data<OutputElementType, int8>(DataFilePathAndName, OutputMatrixSize);
    }
    else if (InputElementTypeName == "int16")
    {
		OutputMatrix = LoadDenseMatrixFromJsonDataFile_Data<OutputElementType, int16>(DataFilePathAndName, OutputMatrixSize);
    }
    else if (InputElementTypeName == "int32")
    {
		OutputMatrix = LoadDenseMatrixFromJsonDataFile_Data<OutputElementType, int32>(DataFilePathAndName, OutputMatrixSize);
    }
    else if (InputElementTypeName == "int64")
    {
		OutputMatrix = LoadDenseMatrixFromJsonDataFile_Data<OutputElementType, int64>(DataFilePathAndName, OutputMatrixSize);
    }
    else if (InputElementTypeName == "uint8")
    {
		OutputMatrix = LoadDenseMatrixFromJsonDataFile_Data<OutputElementType, uint8>(DataFilePathAndName, OutputMatrixSize);
    }
    else if (InputElementTypeName == "uint16")
    {
		OutputMatrix = LoadDenseMatrixFromJsonDataFile_Data<OutputElementType, uint16>(DataFilePathAndName, OutputMatrixSize);
    }
    else if (InputElementTypeName == "uint32")
    {
		OutputMatrix = LoadDenseMatrixFromJsonDataFile_Data<OutputElementType, uint32>(DataFilePathAndName, OutputMatrixSize);
    }
    else if (InputElementTypeName == "uint64")
    {
		OutputMatrix = LoadDenseMatrixFromJsonDataFile_Data<OutputElementType, uint64>(DataFilePathAndName, OutputMatrixSize);
    }
    else
    {
        MDK_Error("unknown ElementType of data file @ LoadDenseMatrixFromJsonDataFile_Data(...) ")
    }

	return OutputMatrix;
}


template<typename OutputElementType, typename InputElementType>
DenseMatrix<OutputElementType> LoadDenseMatrixFromJsonDataFile_Data(const std::string& DataFilePathAndName, MatrixSize OutputMatrixSize)
{
	DenseMatrix<OutputElementType> OutputMatrix;

	QFile DataFile(DataFilePathAndName.c_str());
	if (!DataFile.open(QIODevice::ReadOnly))
	{
		MDK_Error("Couldn't open data file:" << DataFilePathAndName)
		return OutputMatrix;
	}
	int_max BypesofDataFile = DataFile.size();

	OutputMatrix.Resize(OutputMatrixSize.RowNumber, OutputMatrixSize.ColNumber);
	int_max ElementNumber = OutputMatrix.GetElementNumber();
	int_max ByteNumberOfInputElementType = GetByteNumberOfScalar(InputElementType(0));   

    if (BypesofDataFile != ElementNumber * ByteNumberOfInputElementType)
    {
        MDK_Error("Data file size is not equal to matrix size @ LoadDenseMatrixFromJsonDataFile_Data(...)")
		DataFile.close();
		return OutputMatrix;
    }

    for (int_max i = 0; i < ElementNumber; ++i)
    {
		auto tempScalar = InputElementType(0);

        DataFile.read((char*)(&tempScalar), ByteNumberOfInputElementType);

		OutputMatrix[i] = OutputElementType(tempScalar);
    }
	DataFile.close();

	return OutputMatrix;
}

}//namespace mdk

#endif
