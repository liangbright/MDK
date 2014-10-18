#ifndef __mdkFileIO_PolygonMesh_hpp
#define __mdkFileIO_PolygonMesh_hpp

namespace mdk
{

template<typename MeshAttributeType>
bool SavePolygonMeshAsJsonDataFile(const PolygonMesh<MeshAttributeType>& InputMesh, const std::string& JsonFilePathAndName)
{
	if (SavePolygonMeshAsJsonDataFile_Header(InputMesh, JsonFilePathAndName) == false)
	{
		return false;
	}
	std::string DataFilePathAndName = JsonFilePathAndName + ".data";
	return SavePolygonMeshAsJsonDataFile_Data(InputMesh, DataFilePathAndName);
}


template<typename MeshAttributeType>
bool SavePolygonMeshAsJsonDataFile_Header(const PolygonMesh<MeshAttributeType>& InputMesh, const std::string& JsonFilePathAndName)
{
	typedef PolygonMesh<MeshAttributeType>::ScalarType  ScalarType;

	if (GetByteNumberOfScalar(ScalarType(0)) <= 0)
	{
		MDK_Error("Unknown ScalarType @ SavePolygonMeshAsJsonDataFile_Header(...)")
		return false;
	}
	//-------------------------------------------------------------------------------------

	std::vector<NameValueQStringPair> PairList;
	PairList.resize(5);

	PairList[0].Name = "ObjectType";
	PairList[0].Value = "PolygonMesh";

	auto ScalarTypeName = GetScalarTypeName(ScalarType(0));
	QString QScalarTypeName(ScalarTypeName.c_str());
	PairList[1].Name = "ScalarType";
	PairList[1].Value = QScalarTypeName;

	auto IndexTypeName = GetScalarTypeName(int_max(0));
	QString QIndexTypeName(IndexTypeName.c_str());
	PairList[2].Name = "IndexType";
	PairList[2].Value = QIndexTypeName;

	auto PointNumber = InputMesh.GetPointNumber();
	PairList[3].Name = "PointNumber";
	PairList[3].Value = QString::number(PointNumber);

	auto CellNumber = InputMesh.GetCellNumber();
	PairList[4].Name = "CellNumber";
	PairList[4].Value = QString::number(CellNumber);

	// write header file (json)
	QString QFilePathAndName(JsonFilePathAndName.c_str());
	return SaveNameValuePairListAsJsonFile(PairList, QFilePathAndName);
}


template<typename MeshAttributeType>
bool SavePolygonMeshAsJsonDataFile_Data(const PolygonMesh<MeshAttributeType>& InputMesh, const std::string& DataFilePathAndName)
{
	if (InputMesh.IsEmpty() == true)
	{
		MDK_Warning("InputMesh is empty @ SavePolygonMeshAsJsonDataFile_Data(...)")
		return true;
	}

	// get data --------------------------------------------------------------------
	typedef typename MeshAttributeType::ScalarType ScalarType;

	DataArray<DenseVector<int_max>> CellData;
	DenseMatrix<ScalarType> PointData;
	InputMesh.GetPointPositionMatrixAndCellTable(PointData, CellData);

	QString QDataFilePathAndName(DataFilePathAndName.c_str());

	// write point to data file
	QFile PointDataFile(QDataFilePathAndName + ".point");
	if (!PointDataFile.open(QIODevice::WriteOnly))
	{
		MDK_Error("Couldn't open file to write point data @ SavePolygonMeshAsJsonDataFile_Data(...)")
		return false;
	}
	PointDataFile.write((char*)PointData.GetElementPointer(), PointData.GetElementNumber()*GetByteNumberOfScalar(ScalarType(0)));
	PointDataFile.flush();
	PointDataFile.close();

	//write cell to data file
	QFile CellDataFile(QDataFilePathAndName + ".cell");
	if (!CellDataFile.open(QIODevice::WriteOnly))
	{
		MDK_Error("Couldn't open file to write cell data @ SavePolygonMeshAsJsonDataFile_Data(...)")
		return false;
	}

	QTextStream Stream_out(&CellDataFile);

	for (int_max i = 0; i < CellData.GetLength(); ++i)
	{
		const DenseVector<int_max>& Cell_i = CellData[i];

		for (int_max n = 0; n < Cell_i.GetElementNumber(); ++n)
		{
			Stream_out << QString::number(Cell_i[n]);

			if (n < Cell_i.GetLength() - 1)
			{
				Stream_out << ", ";
			}
		}

		if (i < CellData.GetLength() - 1)
		{
			Stream_out << "\n";
		}
	}

	Stream_out.flush();
	CellDataFile.close();

	return true;
}


template<typename MeshAttributeType>
PolygonMesh<MeshAttributeType> LoadPolygonMeshFromJsonDataFile(const std::string& JsonFilePathAndName)
{
	PolygonMesh<MeshAttributeType> OutputMesh;

	//---------------------------------------------- Read header --------------------------------------------------------//
	QFile HeaderFile(JsonFilePathAndName.c_str());
	if (!HeaderFile.open(QIODevice::ReadOnly))
	{
		MDK_Error("Couldn't open Header File @ LoadPolygonMeshFromJsonDataFile(...)")
		return OutputMesh;
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

		if (ObjectType != "PolygonMesh")
		{
			MDK_Error("ObjectType is not PolygonMesh @ LoadPolygoneMeshFromJsonDataFile(...)")
			HeaderFile.close();
			return OutputMesh;
		}
	}
	else
	{
		MDK_Error("Couldn't get ObjectType @ LoadPolygoneMeshFromJsonDataFile(...)")
		HeaderFile.close();
		return OutputMesh;
	}
	//------------------------------------------
	std::string InputScalarTypeName;
	it = HeaderObject.find("ScalarType");
	if (it != HeaderObject.end())
	{
		InputScalarTypeName = it.value().toString().toStdString();
	}
	else
	{
		MDK_Error("Couldn't get ScalarType @ LoadPolygoneMeshFromJsonDataFile(...)")
		HeaderFile.close();
		return OutputMesh;
	}
	//-------------------------------------------
	std::string IndexTypeName;
	it = HeaderObject.find("IndexType");
	if (it != HeaderObject.end())
	{
		IndexTypeName = it.value().toString().toStdString();
	}
	else
	{
		MDK_Error("Couldn't get IndexType @ LoadPolygonMeshFromJsonDataFile(...)")
		HeaderFile.close();
		return OutputMesh;
	}
	//----------------------------------------------
	int_max PointNumber = 0;
	it = HeaderObject.find("PointNumber");
	if (it != HeaderObject.end())
	{
		PointNumber = it.value().toString().toLongLong();
	}
	else
	{
		MDK_Error("Couldn't get PointNumber @ LoadPolygonMeshFromJsonDataFile(...)")
		HeaderFile.close();
		return OutputMesh;
	}
	//----------------------------------------------
	int_max CellNumber = 0;
	it = HeaderObject.find("CellNumber");
	if (it != HeaderObject.end())
	{
		CellNumber = it.value().toString().toLongLong();
	}
	else
	{
		MDK_Error("Couldn't get CellNumber @ LoadPolygonMeshFromJsonDataFile(...)")
		HeaderFile.close();
		return OutputMesh;
	}
	//----------------------------------------------
	std::string DataFilePathAndName = JsonFilePathAndName + ".data";
	return LoadPolygonMeshFromJsonDataFile_Data<MeshAttributeType>(DataFilePathAndName, PointNumber, CellNumber, InputScalarTypeName);
}


template<typename MeshAttributeType>
PolygonMesh<MeshAttributeType> LoadPolygonMeshFromJsonDataFile_Data(const std::string& DataFilePathAndName,
																	int_max PointNumber, int_max CellNumber,
																	const std::string& ScalarTypeName)
{
	PolygonMesh<MeshAttributeType> OutputMesh;

	if (PointNumber <= 0 || CellNumber <= 0)
	{
		return OutputMesh;
	}

	typedef typename MeshAttributeType::ScalarType ScalarType;

	std::string PointDataFilePathAndName = DataFilePathAndName + ".point";
	MatrixSize PointDataMatrixSize;
	PointDataMatrixSize.RowNumber = 3;
	PointDataMatrixSize.ColNumber = PointNumber;
	auto PointData = LoadDenseMatrixFromJsonDataFile_Data<ScalarType>(PointDataFilePathAndName, PointDataMatrixSize, ScalarTypeName);

	std::string CellDataFilePathAndName = DataFilePathAndName + ".cell";
	QFile CellDataFile(CellDataFilePathAndName.c_str());
    if (!CellDataFile.open(QIODevice::ReadOnly))
    {
		MDK_Error("Couldn't open cell data file:" << CellDataFilePathAndName)
        return OutputMesh;
    }

    DataArray<DenseVector<int_max>> CellData;
    CellData.FastResize(CellNumber);

    QTextStream stream_in(&CellDataFile);

    for (int_max i = 0; i < CellNumber; ++i)
    {
        DenseVector<int_max>& Cell_i = CellData[i];

        auto ValueList = stream_in.readLine().split(",");
        
        auto tempsize = ValueList.size();

        for (int_max n = 0; n < tempsize; ++n)
        {
            auto tempIndex = ValueList[n].toLongLong();
            Cell_i.Append(tempIndex);
        }
    }

    CellDataFile.close();

    OutputMesh.Construct(PointData, CellData);
    return OutputMesh;
}


template<typename MeshAttributeType>
bool SavePolygonMeshAsVTKFile(const PolygonMesh<MeshAttributeType>& InputMesh, const std::string& FilePathAndName)
{
	auto VTKMesh = ConvertMDKPolygonMeshToVTKPolyData(InputMesh);

	auto writer = vtkSmartPointer<vtkPolyDataWriter>::New();
	writer->SetFileName(FilePathAndName.c_str());
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
PolygonMesh<MeshAttributeType> LoadPolygonMeshFromVTKFile(const std::string& FilePathAndName)
{
	typedef PolygonMesh<MeshAttributeType>::ScalarType ScalarType;

	PolygonMesh<MeshAttributeType> OutputMesh;

	auto Reader = vtkSmartPointer<vtkPolyDataReader>::New();
	Reader->SetFileName(FilePathAndName.c_str());

	try
	{
		Reader->Update();
	}
	catch (...)
	{
		MDK_Error(" Can not read data @ LoadPolygonMeshFromVTKFile(...) ")
		return OutputMesh;
	}

	auto VTKPolyMesh = Reader->GetOutput();
	OutputMesh = ConvertVTKPolyDataToMDKPolygonMesh<MeshAttributeType>(VTKPolyMesh);
	return OutputMesh;
}

}//namespace mdk

#endif