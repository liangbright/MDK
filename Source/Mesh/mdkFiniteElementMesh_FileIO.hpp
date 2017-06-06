#pragma once

namespace mdk
{

template<typename ScalarType>
vtkSmartPointer<vtkUnstructuredGrid> ConvertFiniteElementMeshToVTKUnstructuredGrid(const FiniteElementMesh<ScalarType>& InputMesh)
{
	auto VTKMesh = vtkSmartPointer<vtkUnstructuredGrid>::New();
	ConvertFiniteElementMeshToVTKUnstructuredGrid(InputMesh, VTKMesh);
	return VTKMesh;
}


template<typename ScalarType>
bool ConvertFiniteElementMeshToVTKUnstructuredGrid(const FiniteElementMesh<ScalarType>& InputMesh, vtkUnstructuredGrid* VTKMesh)
{
	if (VTKMesh == nullptr)
	{
		MDK_Error("VTKMesh is nullptr @ ConvertFiniteElementMeshToVTKUnstructuredGrid(...)")
		return false;
	}

	if (InputMesh.IsEmpty() == true)
	{
		return true;
	}

	auto ReferenceScalar = ScalarType(0);
	auto ScalarTypeName = GetScalarTypeName(ReferenceScalar);

	auto PointCount = InputMesh.GetNodeCount();
	auto CellCount = InputMesh.GetElementCount();

	auto PointData = vtkSmartPointer<vtkPoints>::New();

	if (ScalarTypeName == "double")
	{
		PointData->SetDataType(VTK_DOUBLE);
		PointData->SetNumberOfPoints(PointCount);

		for (int i = 0; i < PointCount; ++i)
		{
			auto Position = InputMesh.GetNode(i);
			double pos[3];
			pos[0] = double(Position(0));
			pos[1] = double(Position(1));
			pos[2] = double(Position(2));
			PointData->SetPoint(i, pos);
		}
	}
	else if (ScalarTypeName == "float")
	{
		PointData->SetDataType(VTK_FLOAT);
		PointData->SetNumberOfPoints(PointCount);

		for (int i = 0; i < PointCount; ++i)
		{
			auto Position = InputMesh.GetNode(i);
			float pos[3];
			pos[0] = float(Position(0));
			pos[1] = float(Position(1));
			pos[2] = float(Position(2));
			PointData->SetPoint(i, pos);
		}
	}
	else
	{
		MDK_Warning("ScalarTypeName is not double or float @ ConvertFiniteElementMeshToVTKUnstructuredGrid(...)")

		PointData->SetDataType(VTK_DOUBLE);
		PointData->SetNumberOfPoints(PointCount);

		for (int i = 0; i < PointCount; ++i)
		{
			auto Position = InputMesh.GetNode(i);
			double pos[3];
			pos[0] = double(Position(0));
			pos[1] = double(Position(1));
			pos[2] = double(Position(2));
			PointData->SetPoint(i, pos);
		}
	}

	//------------------------------------------

	auto CellData = vtkSmartPointer<vtkCellArray>::New();

	DenseVector<int> CellTypeList;
	CellTypeList.Resize(CellCount);

	for (int i = 0; i < CellCount; ++i)
	{
		auto Element = InputMesh.GetElement(i);
		auto PointCountInCell = Element.GetLength();
		auto ElementType = InputMesh.GetElementType(i);

		CellData->InsertNextCell(PointCountInCell);		
		for (int n = 0; n < PointCountInCell; ++n)
		{
			auto PointIndex = Element[n];

			CellData->InsertCellPoint(PointIndex);
		}

		switch (ElementType)
		{
		case FiniteElementType::S3:
			CellTypeList[i] = VTKCellType::VTK_TRIANGLE;
			break;
		case FiniteElementType::S3R:
			CellTypeList[i] = VTKCellType::VTK_TRIANGLE;
			break;
		case FiniteElementType::S4:
			CellTypeList[i] = VTKCellType::VTK_QUAD;
			break;
		case FiniteElementType::S4R:
			CellTypeList[i] = VTKCellType::VTK_QUAD;
			break;
		case FiniteElementType::C3D4:
			CellTypeList[i] = VTKCellType::VTK_TETRA;
			break;
		case FiniteElementType::C3D4R:
			CellTypeList[i] = VTKCellType::VTK_TETRA;
			break;
		case FiniteElementType::C3D6:
			CellTypeList[i] = VTKCellType::VTK_WEDGE;
			break;
		case FiniteElementType::C3D6R:
			CellTypeList[i] = VTKCellType::VTK_WEDGE;
			break;
		case FiniteElementType::C3D8:
			CellTypeList[i] = VTKCellType::VTK_HEXAHEDRON;
			break;
		case FiniteElementType::C3D8R:
			CellTypeList[i] = VTKCellType::VTK_HEXAHEDRON;
			break;
		default:
			CellTypeList[i] = VTKCellType::VTK_POLYHEDRON;
		}
	}	
	//---------------------------------------------------
	VTKMesh->SetPoints(PointData);
	VTKMesh->SetCells(CellTypeList.GetPointer(), CellData);
	
	return true;
}

template<typename ScalarType>
vtkSmartPointer<vtkPolyData> ConvertFiniteElementMeshToVTKPolyData(const FiniteElementMesh<ScalarType>& InputMesh)
{
	auto VTKMesh = vtkSmartPointer<vtkPolyData>::New();

	//------------------------------------------
	auto ReferenceScalar = ScalarType(0);
	auto ScalarTypeName = GetScalarTypeName(ReferenceScalar);

	auto PointCount = InputMesh.GetNodeCount();
	auto CellCount = InputMesh.GetElementCount();

	auto PointData = vtkSmartPointer<vtkPoints>::New();

	if (ScalarTypeName == "double")
	{
		PointData->SetDataType(VTK_DOUBLE);
		PointData->SetNumberOfPoints(PointCount);

		for (int i = 0; i < PointCount; ++i)
		{
			auto Position = InputMesh.GetNode(i);
			double pos[3];
			pos[0] = double(Position(0));
			pos[1] = double(Position(1));
			pos[2] = double(Position(2));
			PointData->SetPoint(i, pos);
		}
	}
	else if (ScalarTypeName == "float")
	{
		PointData->SetDataType(VTK_FLOAT);
		PointData->SetNumberOfPoints(PointCount);

		for (int i = 0; i < PointCount; ++i)
		{
			auto Position = InputMesh.GetNode(i);
			float pos[3];
			pos[0] = float(Position(0));
			pos[1] = float(Position(1));
			pos[2] = float(Position(2));
			PointData->SetPoint(i, pos);
		}
	}
	else
	{
		MDK_Warning("ScalarTypeName is not double or float @ ConvertFiniteElementMeshToVTKPolyData(...)")

		PointData->SetDataType(VTK_DOUBLE);
		PointData->SetNumberOfPoints(PointCount);

		for (int i = 0; i < PointCount; ++i)
		{
			auto Position = InputMesh.GetNode(i);
			double pos[3];
			pos[0] = double(Position(0));
			pos[1] = double(Position(1));
			pos[2] = double(Position(2));
			PointData->SetPoint(i, pos);
		}
	}

	//------------------------------------------

	auto CellData = vtkSmartPointer<vtkCellArray>::New();

	for (int i = 0; i < CellCount; ++i)
	{
		auto Element = InputMesh.GetElement(i);
		auto PointCountInCell = Element.GetLength();

		CellData->InsertNextCell(PointCountInCell);

		for (int n = 0; n < PointCountInCell; ++n)
		{
			auto PointIndex = Element[n];

			CellData->InsertCellPoint(PointIndex);
		}
	}
	//---------------------------------------------------
	VTKMesh->SetPoints(PointData);
	VTKMesh->SetPolys(CellData);
	return VTKMesh;
}


template<typename ScalarType>
bool ConvertVTKUnstructuredGridToFiniteElementMesh(vtkUnstructuredGrid* VTKMesh, FiniteElementMesh<ScalarType>& OutputMesh)
{
	if (VTKMesh == nullptr)
	{
		MDK_Error("VTKMesh is nullptr @ ConvertVTKUnstructuredGridToFiniteElementMesh(...)")
		return false;
	}

	auto PointCount = int_max(VTKMesh->GetNumberOfPoints());
	auto CellCount = int_max(VTKMesh->GetNumberOfCells());
	OutputMesh.SetCapacity(PointCount, CellCount);

	if (PointCount == 0)
	{
		OutputMesh.Clear();
		return true;
	}

	for (int_max k = 0; k < PointCount; ++k)
	{
		double pos[3];
		VTKMesh->GetPoint(k, pos);
		OutputMesh.AddNode(pos);
	}
	
	for (int_max k = 0; k < CellCount; ++k)
	{
		auto Cell = VTKMesh->GetCell(k);
		auto PointNumberInCell = int_max(Cell->GetNumberOfPoints());

		DenseVector<int_max> IndexList;
		IndexList.Resize(PointNumberInCell);
		for (int_max n = 0; n < PointNumberInCell; ++n)
		{
			IndexList[n] = int_max(Cell->GetPointId(n));
		}

		OutputMesh.AddElement(IndexList);
	}

	return true;
}


template<typename ScalarType>
bool SaveFiniteElementMeshAsVTKFile(const FiniteElementMesh<ScalarType>& InputMesh, const String& FilePathAndName)
{
	auto VTKMesh = ConvertFiniteElementMeshToVTKUnstructuredGrid(InputMesh);
	return SaveVTKUnstructuredGridAsVTKFile(VTKMesh, FilePathAndName);
}


template<typename ScalarType>
bool LoadFiniteElementMeshFromVTKFile(FiniteElementMesh<ScalarType>& OutputMesh, const String& FilePathAndName)
{
	auto VTKMesh = LoadVTKUnstructuredGridFromVTKFile(FilePathAndName);
	return ConvertVTKUnstructuredGridToFiniteElementMesh(VTKMesh, OutputMesh);
}


template<typename ScalarType>
bool SaveFiniteElementMeshAsJsonDataFile(const FiniteElementMesh<ScalarType>& InputMesh, const String& FilePathAndName)
{
	//---------------------------------------------
	if (GetByteCountOfScalar(ScalarType(0)) <= 0)
	{
		MDK_Error("Unknown ScalarType @ SaveFiniteElementMeshAsJsonDataFile(...)")
		return false;
	}

	//-------------------------------------------------------------------//
	JsonObject JObject;

	JObject["ObjectType"] = "FiniteElementMesh";
	JObject["ID"] = InputMesh.GetID();
	JObject["Name"] = InputMesh.GetName();
	JObject["ScalarType"] = GetScalarTypeName(ScalarType(0));
	JObject["IndexType"] = GetScalarTypeName(int_max(0));
	//----------------------------------------
	JObject["NodeCount"] = InputMesh.GetNodeCount();
	JObject["ElementCount"] = InputMesh.GetElementCount();
	JObject["NodeSetCount"] = InputMesh.GetNodeSetCount();
	JObject["ElementSetCount"] = InputMesh.GetElementSetCount();
	//----------------------------------------
	{
		JsonObject tempJObject;
		for (int_max k = 0; k < InputMesh.GetNodeCount(); ++k)
		{
			auto NodeName = InputMesh.GetNodeName(k);
			if (NodeName.IsEmpty() == false)
			{
				tempJObject[NodeName] = k;
			}
		}
		JsonValue tempJValue = std::move(tempJObject);
		JObject["NamedNodeList"] = tempJValue;
	}
	//----------------------------------------
	{
		JsonObject tempJObject;
		for (int_max k = 0; k < InputMesh.GetElementCount(); ++k)
		{
			auto ElementName = InputMesh.GetElementName(k);
			if (ElementName.IsEmpty() == false)
			{
				tempJObject[ElementName] = k;
			}
		}
		JsonValue tempJValue = std::move(tempJObject);
		JObject["NamedElementList"] = tempJValue;
	}
	//----------------------------------------
	{
		JsonArray tempJArray;		
		tempJArray.Resize(InputMesh.GetNodeSetCount());
		for (int_max k = 0; k < InputMesh.GetNodeSetCount(); ++k)
		{
			JsonObject tempJObject;
			tempJObject[InputMesh.GetNodeSetName(k)] = InputMesh.GetNodeSet(k);
			tempJArray[k] = tempJObject;
		}
		JsonValue tempJValue = std::move(tempJArray);
		JObject["NodeSetList"] = tempJValue;
	}
	//----------------------------------------
	{
		JsonArray tempJArray;
		tempJArray.Resize(InputMesh.GetElementSetCount());
		for (int_max k = 0; k < InputMesh.GetElementSetCount(); ++k)
		{
			JsonObject tempJObject;
			tempJObject[InputMesh.GetElementSetName(k)] = InputMesh.GetElementSet(k);
			tempJArray[k] = tempJObject;
		}
		JsonValue tempJValue = std::move(tempJArray);
		JObject["ElementSetList"] = tempJValue;
	}
	//----------------------------------------------------------------------------------//
	{
		JsonArray ElementTypeList;
		ElementTypeList.Resize(InputMesh.GetElementCount());
		for (int_max k = 0; k < InputMesh.GetElementCount(); ++k)
		{
			ElementTypeList[k] = InputMesh.GetElementTypeAsString(k);
		}
		JObject["ElementTypeList"] = ElementTypeList;
	}

	//----------------------------------------
	String FilePath = ExtractFilePath(FilePathAndName);
	String FileName_VTKMesh = ExtractFileName(FilePathAndName) + ".vtk";
	JObject["NodeAndElement"] = FileName_VTKMesh;
	//----------------------------------------------------------------------------------//
	bool IsOK = true;
	if (SaveFiniteElementMeshAsVTKFile(InputMesh, FilePath + FileName_VTKMesh) == false)
	{
		MDK_Error("Error1 @ SaveFiniteElementMeshAsJsonDataFile(...)")
	}
	if (JsonFile::Save(JObject, FilePathAndName) == false)
	{
		MDK_Error("Error2 @ SaveFiniteElementMeshAsJsonDataFile(...)")
		IsOK = false;
	}
	return true;
}


template<typename ScalarType>
bool LoadFiniteElementMeshFromJsonDataFile(FiniteElementMesh<ScalarType>& OutputMesh, const String& FilePathAndName)
{
	JsonObject JObject;
	if (JsonFile::Load(JObject, FilePathAndName) == false)
	{
		MDK_Error("Json file is invalid @ LoadFiniteElementMeshFromJsonDataFile(...)")
		return false;
	}

	if (JObject.IsEmpty() == true)
	{
		MDK_Warning("Json file is empty @ LoadFiniteElementMeshFromJsonDataFile(...)")
		return true;
	}
	//----------------------------------------------------------//
	auto it = JObject.find("ObjectType");
	if (it != JObject.end())
	{
		auto ObjectType = it->second.GetString();
		if (ObjectType != "FiniteElementMesh")
		{
			MDK_Error("ObjectType is not FiniteElementMesh @ LoadFiniteElementMeshFromJsonDataFile(...)")
			return false;
		}
	}
	else
	{
		MDK_Error("Couldn't get ObjectType @ LoadFiniteElementMeshFromJsonDataFile(...)")
		return false;
	}
	//----------------------------------------------------------//
	it = JObject.find("ID");
	if (it != JObject.end())
	{
		auto ID = it->second.ToScalar<int_max>();
		OutputMesh.SetID(ID);
	}
	else
	{
		MDK_Error("Couldn't get ID @ LoadFiniteElementMeshFromJsonDataFile(...)")
		return false;
	}
	//----------------------------------------------------------//
	it = JObject.find("Name");
	if (it != JObject.end())
	{
		auto Name = it->second.GetString();
		OutputMesh.SetName(Name);
	}
	else
	{
		MDK_Error("Couldn't get Name @ LoadFiniteElementMeshFromJsonDataFile(...)")
		return false;
	}
	//----------------------------------------------------------//
	String InputScalarTypeName;
	it = JObject.find("ScalarType");
	if (it != JObject.end())
	{
		InputScalarTypeName = it->second.GetString();
	}
	else
	{
		MDK_Error("Couldn't get ScalarType @ LoadFiniteElementMeshFromJsonDataFile(...)")
		return false;
	}
	//----------------------------------------------------
	String InputIndexTypeName;
	it = JObject.find("IndexType");
	if (it != JObject.end())
	{
		InputIndexTypeName = it->second.GetString();
	}
	else
	{
		MDK_Error("Couldn't get IndexType @ LoadFiniteElementMeshFromJsonDataFile(...)")
		return false;
	}
	//----------------------------------------------------------//
	int_max NodeCount;
	it = JObject.find("NodeCount");
	if (it != JObject.end())
	{
		NodeCount = it->second.ToScalar<int_max>();
	}
	else
	{
		MDK_Error("Couldn't get NodeCount @ LoadFiniteElementMeshFromJsonDataFile(...)")
		return false;
	}
	//------------------------------------------------
	int_max ElementCount;
	it = JObject.find("ElementCount");
	if (it != JObject.end())
	{
		ElementCount = it->second.ToScalar<int_max>();
	}
	else
	{
		MDK_Error("Couldn't get ElementCount @ LoadFiniteElementMeshFromJsonDataFile(...)")
		return false;
	}
	//---------------------------------------------
	int_max NodeSetCount;
	it = JObject.find("NodeSetCount");
	if (it != JObject.end())
	{
		NodeSetCount = it->second.ToScalar<int_max>();
	}
	else
	{
		MDK_Error("Couldn't get NodeSetCount @ LoadFiniteElementMeshFromJsonDataFile(...)")
		return false;
	}
	//----------------------------------------------
	int_max ElementSetCount;
	it = JObject.find("ElementSetCount");
	if (it != JObject.end())
	{
		ElementSetCount = it->second.ToScalar<int_max>();
	}
	else
	{
		MDK_Error("Couldn't get ElementSetCount @ LoadFiniteElementMeshFromJsonDataFile(...)")
		return false;
	}
	OutputMesh.SetCapacity(NodeCount, ElementCount, NodeSetCount, ElementSetCount);
	//----------------------------------------------------------//
	String FilePath = ExtractFilePath(FilePathAndName);
	String FileName_VTKMesh = ExtractFileName(FilePathAndName) + ".vtk";
	JObject["NodeAndElement"] = FileName_VTKMesh;
	bool IsOK = true;
	if (LoadFiniteElementMeshFromVTKFile(OutputMesh, FilePath + FileName_VTKMesh) == false)
	{
		return false;
	}
	//----------------------------------------------------------//
	{
		auto it = JObject.find("NamedNodeList");
		if (it != JObject.end())
		{
			auto tempJObject = it->second.GetJsonObject();
			for (auto it = tempJObject.begin(); it != tempJObject.end(); ++it)
			{
				auto NodeName = it->first;
				auto NodeIndex = it->second.ToScalar<int_max>();
				OutputMesh.SetNodeName(NodeIndex, NodeName);
			}
		}
	}
	//----------------------------------------------------------//
	{
		auto it = JObject.find("NamedElementList");
		if (it != JObject.end())
		{
			auto tempJObject = it->second.GetJsonObject();
			for (auto it = tempJObject.begin(); it != tempJObject.end(); ++it)
			{
				auto NodeName = it->first;
				auto NodeIndex = it->second.ToScalar<int_max>();
				OutputMesh.SetNodeName(NodeIndex, NodeName);
			}
		}
	}
	//----------------------------------------
	{
		auto it = JObject.find("NodeSetList");
		if (it != JObject.end())
		{
			auto tempJArray = it->second.GetJsonArray();
			for (int_max k = 0; k < tempJArray.GetLength(); ++k)
			{
				auto tempJObject = tempJArray[k].GetJsonObject();
				for (auto it = tempJObject.begin(); it != tempJObject.end(); ++it)
				{
					auto NodeSetName = it->first;
					auto NodeIndexListOfNodeSet = it->second.ToScalarArray<int_max>();
					OutputMesh.AddNodeSet(NodeSetName, NodeIndexListOfNodeSet);
				}
			}
		}
	}
	//----------------------------------------
	{
		auto it = JObject.find("ElementSetList");
		if (it != JObject.end())
		{
			auto tempJArray = it->second.GetJsonArray();
			for (int_max k = 0; k < tempJArray.GetLength(); ++k)
			{
				auto tempJObject = tempJArray[k].GetJsonObject();
				for (auto it = tempJObject.begin(); it != tempJObject.end(); ++it)
				{
					auto ElementSetName = it->first;
					auto ElementIndexListOfElementSet = it->second.ToScalarArray<int_max>();
					OutputMesh.AddElementSet(ElementSetName, ElementIndexListOfElementSet);
				}
			}
		}
	}
	//----------------------------------------
	{
		auto it = JObject.find("ElementTypeList");
		if (it != JObject.end())
		{
			auto ElementTypeList = it->second.GetJsonArray();
			for (int_max k = 0; k < ElementTypeList.GetLength(); ++k)
			{
				auto ElementType = ElementTypeList[k].GetString();
				OutputMesh.SetElementType(k, ElementType);
			}
		}
	}
	//-----------------
	return true;
}


template<typename ScalarType>
bool SaveFiniteElementMeshAsAbaqusINPFile(const FiniteElementMesh<ScalarType>& InputMesh, const String& FilePathAndName)
{
	//--------------------------------------------------------------//
	auto as_string =[](ScalarType value, const int n = 15)
	{
		std::ostringstream out;
		out << std::setprecision(n) << value;
		String str = out.str();
		return str;
	};
	//--------------------------------------------------------------//

	TextFile OutputINPFile(FilePathAndName);
	OutputINPFile.Clear();

	OutputINPFile << "** ABAQUS Input Deck Generated by SaveFiniteElementMeshAsAbaqusINPFile @ mdk" << '\n';
	OutputINPFile << "*Part, name = PART-1" << '\n';

	//------------ write node and element----------------------------
	OutputINPFile << "*NODE" << '\n';
	for (int_max k = 0; k < InputMesh.GetNodeCount(); ++k)
	{
		auto Pos = InputMesh.GetNode(k);
		String line = as_string(k + 1) + ", " + as_string(Pos[0]) + ", " + as_string(Pos[1]) + ", " + as_string(Pos[2]);
		OutputINPFile << line << '\n';
	}

	String ElementType_prev, ElementType_cur;
	for (int_max k=0; k<InputMesh.GetElementCount(); ++k)
	{
		auto Element = InputMesh.GetElement(k);
		ElementType_cur = InputMesh.GetElementTypeAsString(k);
		if (ElementType_cur != ElementType_prev)
		{
			OutputINPFile << "*ELEMENT, TYPE = " << ElementType_cur << '\n';
			ElementType_prev = ElementType_cur;
		}
		String line;
		for (int_max n = 0; n < Element.GetLength(); ++n)
		{
			line += as_string(n+1);
			if (n < Element.GetLength() - 1)
			{
				line += ", ";
			}
		}
		OutputINPFile << line << '\n';
	}

	return true;
}

}//namespace mdk

