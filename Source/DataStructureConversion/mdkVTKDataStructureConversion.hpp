#pragma once

namespace mdk
{

inline VTKDataTypeEnum TranslateMDKScalarTypeToVTKScalarType(const String& MDKScalarTypeName)
{
	if (MDKScalarTypeName == "double")
	{
		return VTKDataTypeEnum::VALUE_DOUBLE;
	}
	else if (MDKScalarTypeName == "float")
	{
		return VTKDataTypeEnum::VALUE_FLOAT;
	}
	else if (MDKScalarTypeName == "int8")
	{
		return VTKDataTypeEnum::VALUE_CHAR;
	}
	else if (MDKScalarTypeName == "int16")
	{
		return VTKDataTypeEnum::VALUE_SHORT;
	}
	else if (MDKScalarTypeName == "int32")
	{
		return VTKDataTypeEnum::VALUE_INT;
	}
	else if (MDKScalarTypeName == "int64")
	{
		return VTKDataTypeEnum::VALUE_LONG_LONG;
	}
	else if (MDKScalarTypeName == "uint8")
	{
		return VTKDataTypeEnum::VALUE_UNSIGNED_CHAR;
	}
	else if (MDKScalarTypeName == "uint16")
	{
		return VTKDataTypeEnum::VALUE_UNSIGNED_SHORT;
	}
	else if (MDKScalarTypeName == "uint32")
	{
		return VTKDataTypeEnum::VALUE_UNSIGNED_INT;
	}
	else if (MDKScalarTypeName == "uint64")
	{
		return VTKDataTypeEnum::VALUE_UNSIGNED_LONG_LONG;
	}
	else
	{
		MDK_Error("Input ScalarType is unknown" << MDKScalarTypeName << '\n')
		return VTKDataTypeEnum::VALUE_UNKNOWN;
	}
}

template<typename ScalarType>
VTKDataTypeEnum GetVTKScalarType(const ScalarType& ReferenceScalar)
{
	auto Name = GetScalarTypeName(ReferenceScalar);
	return TranslateMDKScalarTypeToVTKScalarType(Name);
}

//-------------------------------------- convert mdk Curve/Matrix to vtkPolyData ----------------------------------------------------------//

template<typename ScalarType>
vtkSmartPointer<vtkPolyData> ConvertSingleMDK3DCurveToVTKPolyData(const DenseMatrix<ScalarType>& MDK3DCurve)
{
	auto VTKCurve = vtkSmartPointer<vtkPolyData>::New();

	auto PointCount = MDK3DCurve.GetColCount();
	if (PointCount == 0)
	{
		MDK_Warning("input curve is empty @ ConvertSingleMDK3DCurveToVTKPolyData(...)")
		return VTKCurve;
	}

	if (MDK3DCurve.GetRowCount() != 3)
	{
		MDK_Error("Invalid MDK3DCurve @ mdkFileIO ConvertSingleMDK3DCurveToVTKPolyData")
		return VTKCurve;
	}

	auto VtkDataType = GetVTKScalarType(ScalarType(0));

	auto PointSet = vtkSmartPointer<vtkPoints>::New();
	PointSet->SetDataType(VtkDataType);

	auto CellList = vtkSmartPointer<vtkCellArray>::New();
	CellList->InsertNextCell(PointCount);

	for (int n = 0; n < PointCount; ++n)
	{
		ScalarType Pos[3];
		MDK3DCurve.GetCol(n, Pos);
		CellList->InsertCellPoint(n);
		PointSet->InsertPoint(n, Pos);
	}

	VTKCurve->SetPoints(PointSet);
	VTKCurve->SetLines(CellList);
	return VTKCurve;
}


template<typename ScalarType>
vtkSmartPointer<vtkPolyData> ConvertMultipleMDK3DCurveToVTKPolyData(const ObjectArray<DenseMatrix<ScalarType>>& MDK3DCurveList)
{
	auto VTKCurve = vtkSmartPointer<vtkPolyData>::New();
	
	if (MDK3DCurveList.IsEmpty() == true)
	{
		MDK_Warning("MDK3DCurveList is empty @ mdkFileIO ConvertSingleMDK3DCurveToVTKPolyData(...)")
		return VTKCurve;
	}

	auto VtkDataType = GetVTKScalarType(ScalarType(0));

	auto PointSet = vtkSmartPointer<vtkPoints>::New();
	PointSet->SetDataType(VtkDataType);
	auto CellList = vtkSmartPointer<vtkCellArray>::New();
	auto VTKCurveData = vtkSmartPointer<vtkPolyData>::New();

	int_max PointCounter = 0;
	auto CurveNumber = MDK3DCurveList.GetLength();
	for (int_max k = 0; k < CurveNumber; ++k)
	{
		auto PointCount = MDK3DCurveList[k].GetColCount();

		CellList->InsertNextCell(PointCount);

		if (MDK3DCurveList[k].GetRowCount() != 3)
		{
			MDK_Error("Invalid InputCurve @ mdkFileIO ConvertSingleMDK3DCurveToVTKPolyData")
			return VTKCurve;
		}

		for (int_max n = 0; n < PointCount; ++n)
		{
			ScalarType Pos[3];
			MDK3DCurveList[k].GetCol(n, Pos);
			CellList->InsertCellPoint(PointCounter);
			PointSet->InsertPoint(PointCounter, Pos);
			PointCounter += 1;
		}
	}

	VTKCurve->SetPoints(PointSet);
	VTKCurve->SetLines(CellList);
	return VTKCurve;
}

//-------------------------------------- convert vtkPolyData to mdk Curve/Matrix ----------------------------------------------------------//

template<typename ScalarType>
bool ConvertVTKPolyDataToMultipleMDK3DCurve(vtkPolyData* VTKCurveData, ObjectArray<DenseMatrix<ScalarType>>& MDK3DCurveData)
{
	if (VTKCurveData == nullptr)
	{
		MDK_Error("VTKCurveData is nullptr @ ConvertVTKPolyDataToMultipleMDK3DCurve(...)")
		return false;
	}
	
	auto CurveCount = int_max(VTKCurveData->GetNumberOfLines());
	//auto CellCount = int_max(VTKCurveData->GetNumberOfCells());
	
	auto PointSet = VTKCurveData->GetPoints();
	//auto VTKCurveList = VTKCurveData->GetLines();

	MDK3DCurveData.FastResize(CurveCount);

	for (int_max k = 0; k < CurveCount; ++k)
	{
		auto Cell = VTKCurveData->GetCell(k);
		auto PointCount = int_max(Cell->GetNumberOfPoints());

		auto& OutputCurve_k = MDK3DCurveData[k];
		OutputCurve_k.FastResize(3, PointCount);

		for (int_max n = 0; n < PointCount; ++n)
		{
			double Pos[3];
			auto PointID = Cell->GetPointId(n);
			PointSet->GetPoint(PointID, Pos);
			OutputCurve_k.SetCol(n, Pos);
		}
	}
	return true;
}


template<typename ScalarType>
bool ConvertVTKPolyDataToSingleMDK3DCurve(vtkPolyData* VTKCurveData, DenseMatrix<ScalarType>& MDK3DCurveData)
{
	ObjectArray<DenseMatrix<ScalarType>> MDK3DCurveData_init;
	ConvertVTKPolyDataToMultipleMDK3DCurve(VTKCurveData, MDK3DCurveData_init);
	// put all curve into one matrix
	auto SegmentCount = MDK3DCurveData_init.GetLength();
	MDK3DCurveData.FastResize(0, 0);
	MDK3DCurveData.SetCapacity(3*SegmentCount*2);
	for (int_max k = 0; k < SegmentCount; ++k)
	{
		auto& Segmente_k = MDK3DCurveData_init[k];
		for (int_max n = 0; n < Segmente_k.GetColCount(); ++n)
		{
			MDK3DCurveData.AppendCol(Segmente_k.GetPointerOfCol(n), 3);
		}
	}
	return true;
}

//-------------------------------------- convert mdk 3D image to vtk 3D image (copy data) -----------------------------------------------------//

template<typename PixelType>
vtkSmartPointer<vtkImageData> ConvertMDK3DScalarImageToVTK3DScalarImage(const DenseImage3D<PixelType>& MDKImage)
{
	auto VTKImage = vtkSmartPointer<vtkImageData>::New();

	auto Size = MDKImage.GetSize();
	auto Origin = MDKImage.GetOrigin();
	auto Spacing = MDKImage.GetSpacing();

	auto PtrOfMDKImage = MDKImage.GetPixelPointer();

    VTKImage->SetExtent(0, Size[0] - 1, 0, Size[0] - 1, 0, Size[0] - 1);
    VTKImage->SetOrigin(Origin[0], Origin[1], Origin[2]);    
    VTKImage->SetSpacing(Spacing[0], Spacing[1], Spacing[2]);

    auto ReferenceScalar = PixelType(0);
	auto ScalarTypeName = GetScalarTypeName(ReferenceScalar);

    if (ScalarTypeName == "double")
    {
        VTKImage->AllocateScalars(VTK_DOUBLE, 1);

        auto PtrOfVTKImage = static_cast<double*>(VTKImage->GetScalarPointer());

		for (int_max k = 0; k < MDKImage.GetPixelCount(); ++k)
        {
			PtrOfVTKImage[k] = PtrOfMDKImage[k];
        }
    }
    else if (ScalarTypeName == "float")
    {
        VTKImage->AllocateScalars(VTK_FLOAT, 1);

        auto PtrOfVTKImage = static_cast<float*>(VTKImage->GetScalarPointer());

		for (int_max k = 0; k < MDKImage.GetPixelCount(); ++k)
        {
			PtrOfVTKImage[k] = PtrOfMDKImage[k];
        }
    }
    else if (ScalarTypeName == "int8")
    {
        VTKImage->AllocateScalars(VTK_CHAR, 1);

        auto PtrOfVTKImage = static_cast<int8*>(VTKImage->GetScalarPointer());

		for (int_max k = 0; k < MDKImage.GetPixelCount(); ++k)
        {
			PtrOfVTKImage[k] = PtrOfMDKImage[k];
        }
    }
    else if (ScalarTypeName == "int16")
    {
        VTKImage->AllocateScalars(VTK_SHORT, 1);

        auto PtrOfVTKImage = static_cast<int16*>(VTKImage->GetScalarPointer());

		for (int_max k = 0; k < MDKImage.GetPixelCount(); ++k)
        {
			PtrOfVTKImage[k] = PtrOfMDKImage[k];
        }
    }
    else if (ScalarTypeName == "int32")
    {
        VTKImage->AllocateScalars(VTK_INT, 1);

        auto PtrOfVTKImage = static_cast<int32*>(VTKImage->GetScalarPointer());

		for (int_max k = 0; k < MDKImage.GetPixelCount(); ++k)
        {
			PtrOfVTKImage[k] = PtrOfMDKImage[k];
        }
    }
    else if (ScalarTypeName == "int64")
    {
        VTKImage->AllocateScalars(VTK_LONG_LONG, 1);

        auto PtrOfVTKImage = static_cast<int64*>(VTKImage->GetScalarPointer());

		for (int_max k = 0; k < MDKImage.GetPixelCount(); ++k)
        {
			PtrOfVTKImage[k] = PtrOfMDKImage[k];
        }
    }
    else if (ScalarTypeName == "uint8")
    {
        VTKImage->AllocateScalars(VTK_UNSIGNED_CHAR, 1);

        auto PtrOfVTKImage = static_cast<uint8*>(VTKImage->GetScalarPointer());

		for (int_max k = 0; k < MDKImage.GetPixelCount(); ++k)
        {
			PtrOfVTKImage[k] = PtrOfMDKImage[k];
        }
    }
    else if (ScalarTypeName == "uint16")
    {
        VTKImage->AllocateScalars(VTK_UNSIGNED_SHORT, 1);

        auto PtrOfVTKImage = static_cast<uint16*>(VTKImage->GetScalarPointer());

		for (int_max k = 0; k < MDKImage.GetPixelCount(); ++k)
        {
			PtrOfVTKImage[k] = PtrOfMDKImage[k];
        }
    }
    else if (ScalarTypeName == "uint32")
    {
        VTKImage->AllocateScalars(VTK_UNSIGNED_INT, 1);

        auto PtrOfVTKImage = static_cast<uint32*>(VTKImage->GetScalarPointer());

		for (int_max k = 0; k < MDKImage.GetPixelCount(); ++k)
        {
			PtrOfVTKImage[k] = PtrOfMDKImage[k];
        }
    }
    else if (ScalarTypeName == "uint64")
    {
        VTKImage->AllocateScalars(VTK_UNSIGNED_LONG_LONG, 1);

        auto PtrOfVTKImage = static_cast<uint64*>(VTKImage->GetScalarPointer());

		for (int_max k = 0; k < MDKImage.GetPixelCount(); ++k)
        {
			PtrOfVTKImage[k] = PtrOfMDKImage[k];
        }
    }
    else
    {
        MDK_Error("unknown ScalarType @ ConvertMDK3DScalarImageToVTK3DScalarImage(...) ")
    }

    return VTKImage;
}

//-------------------------------------- convert vtk 3D image to mdk 3D image (copy data) -----------------------------------------------------//

template<typename PixelType>
bool ConvertVTK3DScalarImageToMDK3DScalarImage(const vtkImageData* VTKImage, DenseImage3D<PixelType>& MDKImage)
{
	if (VTKImage == nullptr)
	{
		MDK_Error("input is nullptr @ ConvertVTK3DScalarImageToMDK3DScalarImage(...)")
		return false;
	}

    int Extent[6];
    VTKImage->GetExtent(Extent);

    double Spacing[3];
    VTKImage->GetSpacing(Spacing);

    double Origin[3];
    VTKImage->GetOrigin(Origin);

    auto VTKScalarType = VTKImage->GetScalarType();

	MDKImage.SetSize(Extent[1] + 1, Extent[3] + 1, Extent[5] + 1);
	MDKImage.SetOrigin(Origin[0], Origin[1], Origin[2]);
	MDKImage.SetSpacing(Spacing[0], Spacing[1], Spacing[2]);

	auto PtrOfMDKImage = MDKImage.GetPixelPointer();

    if (VTKScalarType == VTK_DOUBLE)
    {
        auto PtrOfVTKImage = static_cast<double*>(VTKImage->GetScalarPointer());
		for (int_max k = 0; k < MDKImage.GetPixelCount(); ++k)
        {
			PtrOfMDKImage[k] = PtrOfVTKImage[k];
        }
    }
    else if (VTKScalarType == VTK_FLOAT)
    {
        auto PtrOfVTKImage = static_cast<float*>(VTKImage->GetScalarPointer());
		for (int_max k = 0; k < MDKImage.GetPixelCount(); ++k)
        {
			PtrOfMDKImage[k] = PtrOfVTKImage[k];
        }
    }
    else if (VTKScalarType == VTK_CHAR)
    {
        auto PtrOfVTKImage = static_cast<int8*>(VTKImage->GetScalarPointer());
		for (int_max k = 0; k < MDKImage.GetPixelCount(); ++k)
        {
			PtrOfMDKImage[k] = PtrOfVTKImage[k];
        }
    }
    else if (VTKScalarType == VTK_SHORT)
    {
        auto PtrOfVTKImage = static_cast<int16*>(VTKImage->GetScalarPointer());
		for (int_max k = 0; k < MDKImage.GetPixelCount(); ++k)
        {
			PtrOfMDKImage[k] = PtrOfVTKImage[k];
        }
    }
    else if (VTKScalarType == VTK_INT)
    {
        auto PtrOfVTKImage = static_cast<int32*>(VTKImage->GetScalarPointer());
		for (int_max k = 0; k < MDKImage.GetPixelCount(); ++k)
        {
			PtrOfMDKImage[k] = PtrOfVTKImage[k];
        }
    }
    else if (VTKScalarType == VTK_LONG_LONG)
    {
        auto PtrOfVTKImage = static_cast<int64*>(VTKImage->GetScalarPointer());
		for (int_max k = 0; k < MDKImage.GetPixelCount(); ++k)
        {
			PtrOfMDKImage[k] = PtrOfVTKImage[k];
        }
    }
    else if (VTKScalarType == VTK_UNSIGNED_CHAR)
    {
        auto PtrOfVTKImage = static_cast<uint8*>(VTKImage->GetScalarPointer());
		for (int_max k = 0; k < MDKImage.GetPixelCount(); ++k)
        {
			PtrOfMDKImage[k] = PtrOfVTKImage[k];
        }
    }
    else if (VTKScalarType == VTK_UNSIGNED_SHORT)
    {
        auto PtrOfVTKImage = static_cast<uint16*>(VTKImage->GetScalarPointer());
		for (int_max k = 0; k < MDKImage.GetPixelCount(); ++k)
        {
			PtrOfMDKImage[k] = PtrOfVTKImage[k];
        }
    }
    else if (VTKScalarType == VTK_UNSIGNED_INT)
    {
        auto PtrOfVTKImage = static_cast<uint32*>(VTKImage->GetScalarPointer());
		for (int_max k = 0; k < MDKImage.GetPixelCount(); ++k)
        {
			PtrOfMDKImage[k] = PtrOfVTKImage[k];
        }
    }
    else if (VTKScalarType == VTK_UNSIGNED_LONG_LONG)
    {
        auto PtrOfVTKImage = static_cast<uint64*>(VTKImage->GetScalarPointer());

		for (int_max k = 0; k < MDKImage.GetPixelCount(); ++k)
        {
			PtrOfMDKImage[k] = PtrOfVTKImage[k];
        }
    }
    else
    {
        MDK_Error("unknown ScalarType @ ConvertVTK3DScalarImageToMDK3DScalarImage(...) ")
    }

    return true;
}

//--------------------------------------- convert mdk PolygonMesh to vtkPolyData -----------------------------------------------//

template<typename ScalarType>
vtkSmartPointer<vtkPolyData> ConvertMDKPolygonMeshToVTKPolyData(const PolygonMesh<ScalarType>& MDKMesh)
{
	auto VTKMesh = vtkSmartPointer<vtkPolyData>::New();

	if (MDKMesh.IsEmpty() == true)
	{
		return VTKMesh;
	}

	if (MDKMesh.Check_If_DataStructure_is_Clean() == false)
	{
		MDK_Error("MDKMesh DataStructure is NOT Clean @ ConvertMDKPolygonMeshToVTKPolyData(...)")
		return VTKMesh;
	}

    auto ReferenceScalar = ScalarType(0);
	auto ScalarTypeName = GetScalarTypeName(ReferenceScalar);

    DenseMatrix<ScalarType> PointPositionTable;
	ObjectArray<DenseVector<int_max>> FaceTable;
    MDKMesh.GetPointPositionMatrixAndFaceTable(PointPositionTable, FaceTable, false);//Flag_Clean=false;

    int_max PointCount = PointPositionTable.GetColCount();
    int_max FaceCount = FaceTable.GetLength();

	//----------------------------------------------------------
    auto PointArray_vtk = vtkSmartPointer<vtkPoints>::New();

    if (ScalarTypeName == "double")
    {
		PointArray_vtk->SetDataType(VTK_DOUBLE);
		PointArray_vtk->SetNumberOfPoints(PointCount);       
        for (int i = 0; i < PointCount; ++i)
        {
			double pos[3];
			pos[0] = double(PointPositionTable(0, i));
			pos[1] = double(PointPositionTable(1, i));
			pos[2] = double(PointPositionTable(2, i));
			PointArray_vtk->SetPoint(i, pos);
        }
    }
    else if (ScalarTypeName == "float")
    {
		PointArray_vtk->SetDataType(VTK_FLOAT);
		PointArray_vtk->SetNumberOfPoints(PointCount);
        for (int i = 0; i < PointCount; ++i)
        {
			float pos[3];
            pos[0] = float(PointPositionTable(0, i));
            pos[1] = float(PointPositionTable(1, i));
            pos[2] = float(PointPositionTable(2, i));
			PointArray_vtk->SetPoint(i, pos);
        }
    }
    else
    {
        MDK_Warning("ScalarTypeName is not double or float @ ConvertMDKPolygonMeshToVTKPolyData(...)")

		PointArray_vtk->SetDataType(VTK_DOUBLE);
		PointArray_vtk->SetNumberOfPoints(PointCount);
        for (int i = 0; i < PointCount; ++i)
        {
			double pos[3];
			pos[0] = double(PointPositionTable(0, i));
			pos[1] = double(PointPositionTable(1, i));
			pos[2] = double(PointPositionTable(2, i));
			PointArray_vtk->SetPoint(i, pos);
        }
    }
    //-------------------------------------------------------
    auto CellArray_vtk = vtkSmartPointer<vtkCellArray>::New();
    for (int i = 0; i < FaceCount; ++i)
    {
        auto PointCount_i = FaceTable[i].GetElementCount();
		CellArray_vtk->InsertNextCell(PointCount_i);
		for (int n = 0; n < PointCount_i; ++n)
        {
            auto PointIndex = FaceTable[i][n];
			CellArray_vtk->InsertCellPoint(PointIndex);
        }
    }    
	//-----------------------------------------------------
    VTKMesh->SetPoints(PointArray_vtk);
    VTKMesh->SetPolys(CellArray_vtk);
	//-----------------------------------------------------
	for (int_max k = 0; k < MDKMesh.GetPointDataSetCount(); ++k)
	{
		auto DataSet = MDKMesh.GetPointDataSet(k);
		auto Name = MDKMesh.GetPointDataSetName(k);
		auto DataArray_vtk = vtkSmartPointer<vtkDoubleArray>::New();
		DataArray_vtk->SetNumberOfComponents(DataSet.GetRowCount());//SetNumberOfComponents before SetNumberOfTuples
		DataArray_vtk->SetNumberOfTuples(DataSet.GetColCount());		
		DataArray_vtk->SetName(Name.StdString().c_str());
		for (int j = 0; j < DataSet.GetColCount(); ++j)
		{
			for (int i = 0; i < DataSet.GetRowCount(); ++i)
			{
				DataArray_vtk->SetComponent(j, i, double(DataSet(i, j)));
			}
		}
		VTKMesh->GetPointData()->AddArray(DataArray_vtk);
	}
	//-----------------------------------------------------
	for (int_max k = 0; k < MDKMesh.GetFaceDataSetCount(); ++k)
	{
		auto DataSet = MDKMesh.GetFaceDataSet(k);
		auto Name = MDKMesh.GetFaceDataSetName(k);
		auto DataArray_vtk = vtkSmartPointer<vtkDoubleArray>::New();
		DataArray_vtk->SetNumberOfComponents(DataSet.GetRowCount());//SetNumberOfComponents before SetNumberOfTuples
		DataArray_vtk->SetNumberOfTuples(DataSet.GetColCount());
		DataArray_vtk->SetName(Name.StdString().c_str());
		for (int j = 0; j < DataSet.GetColCount(); ++j)
		{
			for (int i = 0; i < DataSet.GetRowCount(); ++i)
			{
				DataArray_vtk->SetComponent(j, i, double(DataSet(i, j)));
			}
		}
		VTKMesh->GetCellData()->AddArray(DataArray_vtk);
	}
	//-----------------------------------------------------
    return VTKMesh;
}

//--------------------------------------- convert vtkPolyData to mdk PolygonMesh ------------------------------------------------//

template<typename ScalarType>
bool ConvertVTKPolyDataToMDKPolygonMesh(vtkPolyData* VTKMesh, PolygonMesh<ScalarType>& MDKMesh)
{
	if (VTKMesh == nullptr)
	{
		MDK_Error("VTKMesh is nullptr @ ConvertVTKPolyDataToMDKPolygonMesh(...)")
		return false;
	}
	//---------------------------------------------------------    
	auto PointCount = int_max(VTKMesh->GetNumberOfPoints());
    if (PointCount == 0)
    {
		MDKMesh.Clear();
        return true;
    }

    DenseMatrix<ScalarType> PointMatrix(3, PointCount);
    for (int_max k = 0; k < PointCount; ++k)
    {
        double pos[3];
		VTKMesh->GetPoint(k, pos);
		PointMatrix.SetCol(k, pos);
    }
	//---------------------------------------------------------
	auto CellCount = int_max(VTKMesh->GetNumberOfCells());

	ObjectArray<DenseVector<int_max>> FaceTable;
	FaceTable.FastResize(CellCount);
    for (int_max k = 0; k < CellCount; ++k)
    {
		auto Cell = VTKMesh->GetCell(k);
        auto PointCount_k = int_max(Cell->GetNumberOfPoints());
		for (int_max n = 0; n < PointCount_k; ++n)
        {
			FaceTable[k].Append(int_max(Cell->GetPointId(n)));
        }
    }
	//---------------------------------------------------------
	MDKMesh.Construct(std::move(PointMatrix), FaceTable);
	//---------------------------------------------------------
	auto PointDataSetCount = VTKMesh->GetPointData()->GetNumberOfArrays();
	for (int k = 0; k < PointDataSetCount; ++k)
	{
		auto DataArray_vtk = VTKMesh->GetPointData()->GetArray(k);
		String Name = DataArray_vtk->GetName();
		DenseMatrix<ScalarType> DataSet;
		DataSet.Resize(DataArray_vtk->GetNumberOfComponents(), DataArray_vtk->GetNumberOfTuples());
		for (int j = 0; j < DataSet.GetColCount(); ++j)
		{
			for (int i = 0; i < DataSet.GetRowCount(); ++i)
			{
				DataSet(i, j) = DataArray_vtk->GetComponent(j, i);
			}
		}
		MDKMesh.AddPointDataSet(Name, DataSet);
	}
	//---------------------------------------------------------
	auto FaceDataSetCount = VTKMesh->GetCellData()->GetNumberOfArrays();
	for (int k = 0; k < FaceDataSetCount; ++k)
	{
		auto DataArray_vtk = VTKMesh->GetCellData()->GetArray(k);
		String Name = DataArray_vtk->GetName();
		DenseMatrix<ScalarType> DataSet;
		DataSet.Resize(DataArray_vtk->GetNumberOfComponents(), DataArray_vtk->GetNumberOfTuples());
		for (int j = 0; j < DataSet.GetColCount(); ++j)
		{
			for (int i = 0; i < DataSet.GetRowCount(); ++i)
			{
				DataSet(i, j) = DataArray_vtk->GetComponent(j, i);
			}
		}
		MDKMesh.AddFaceDataSet(Name, DataSet);
	}
	//---------------------------------------------------------
    return true;
}

//--------------------------------------- convert mdk PolyhedronMesh to vtkUnstructuredGrid ------------------------------------------------//
template<typename ScalarType>
vtkSmartPointer<vtkUnstructuredGrid> ConvertMDKPolyhedronMeshToVTKUnstructuredGrid(const PolyhedronMesh<ScalarType>& MDKMesh)
{
	auto VTKMesh = ConvertMDKMeshToVTKUnstructuredGrid(MDKMesh);	

	if (MDKMesh.IsEmpty() == true)
	{
		return VTKMesh;
	}

	if (MDKMesh.Check_If_DataStructure_is_Clean() == false)
	{
		MDK_Error("MDKMesh DataStructure is NOT Clean @ ConvertMDKPolyhedronMeshToVTKUnstructuredGrid(...)")
		return VTKMesh;
	}

	//-----------------------------------------------------
	for (int_max k = 0; k < MDKMesh.GetPointDataSetCount(); ++k)
	{
		auto DataSet = MDKMesh.GetPointDataSet(k);
		auto Name = MDKMesh.GetPointDataSetName(k);
		auto DataArray_vtk = vtkSmartPointer<vtkDoubleArray>::New();
		DataArray_vtk->SetNumberOfComponents(DataSet.GetRowCount());//SetNumberOfComponents before SetNumberOfTuples
		DataArray_vtk->SetNumberOfTuples(DataSet.GetColCount());
		DataArray_vtk->SetName(Name.StdString().c_str());
		for (int j = 0; j < DataSet.GetColCount(); ++j)
		{
			for (int i = 0; i < DataSet.GetRowCount(); ++i)
			{
				DataArray_vtk->SetComponent(j, i, double(DataSet(i, j)));
			}
		}
		VTKMesh->GetPointData()->AddArray(DataArray_vtk);
	}
	//-----------------------------------------------------
	for (int_max k = 0; k < MDKMesh.GetCellDataSetCount(); ++k)
	{
		auto DataSet = MDKMesh.GetCellDataSet(k);
		auto Name = MDKMesh.GetCellDataSetName(k);
		auto DataArray_vtk = vtkSmartPointer<vtkDoubleArray>::New();
		DataArray_vtk->SetNumberOfComponents(DataSet.GetRowCount());//SetNumberOfComponents before SetNumberOfTuples
		DataArray_vtk->SetNumberOfTuples(DataSet.GetColCount());
		DataArray_vtk->SetName(Name.StdString().c_str());
		for (int j = 0; j < DataSet.GetColCount(); ++j)
		{
			for (int i = 0; i < DataSet.GetRowCount(); ++i)
			{
				DataArray_vtk->SetComponent(j, i, double(DataSet(i, j)));
			}
		}
		VTKMesh->GetCellData()->AddArray(DataArray_vtk);
	}
	//-----------------------------------------------------
	return VTKMesh;
}

//--------------------------------------- convert vtkUnstructuredGrid to mdk PolyhedronMesh ------------------------------------------------//
template<typename ScalarType>
bool ConvertVTKUnstructuredGridToMDKPolyhedronMesh(vtkUnstructuredGrid* VTKMesh, PolyhedronMesh<ScalarType>& MDKMesh)
{
	auto Flag = ConvertVTKUnstructuredGridToMDKMesh(VTKMesh, MDKMesh);
	if (Flag == false)
	{
		return false;
	}
	//---------------------------------------------------------
	auto CellDataSetCount = VTKMesh->GetCellData()->GetNumberOfArrays();
	for (int k = 0; k < CellDataSetCount; ++k)
	{
		auto DataArray_vtk = VTKMesh->GetCellData()->GetArray(k);
		String Name = DataArray_vtk->GetName();
		DenseMatrix<ScalarType> DataSet;
		DataSet.Resize(DataArray_vtk->GetNumberOfComponents(), DataArray_vtk->GetNumberOfTuples());
		for (int j = 0; j < DataSet.GetColCount(); ++j)
		{
			for (int i = 0; i < DataSet.GetRowCount(); ++i)
			{
				DataSet(i, j) = DataArray_vtk->GetComponent(j, i);
			}
		}
		MDKMesh.AddCellDataSet(Name, DataSet);
	}
	//---------------------------------------------------------
	return true;
}

//--------------------------------------- convert mdk Mesh to vtkUnstructuredGrid ------------------------------------------------//
template<typename ScalarType>
vtkSmartPointer<vtkUnstructuredGrid> ConvertMDKMeshToVTKUnstructuredGrid(const Mesh<ScalarType>& MDKMesh)
{	
	auto VTKMesh = vtkSmartPointer<vtkUnstructuredGrid>::New();

	if (MDKMesh.IsEmpty() == true)
	{
		return VTKMesh;
	}

	if (MDKMesh.Check_If_DataStructure_is_Clean() == false)
	{
		MDK_Error("MDKMesh DataStructure is NOT Clean @ ConvertMDKMeshToVTKUnstructuredGrid(...)" )
		return VTKMesh;
	}

	auto ReferenceScalar = ScalarType(0);
	auto ScalarTypeName = GetScalarTypeName(ReferenceScalar);

	auto PointCount = MDKMesh.GetPointCount();
	auto EdgeCount = MDKMesh.GetEdgeCount();
	auto FaceCount = MDKMesh.GetFaceCount();
	auto CellCount_mdk = MDKMesh.GetCellCount();

	auto PointArray_vtk = vtkSmartPointer<vtkPoints>::New();

	if (ScalarTypeName == "double")
	{
		PointArray_vtk->SetDataType(VTK_DOUBLE);
		PointArray_vtk->SetNumberOfPoints(PointCount);
		for (int i = 0; i < PointCount; ++i)
		{
			auto Position = MDKMesh.GetPointPosition(i);
			double pos[3];
			pos[0] = double(Position(0));
			pos[1] = double(Position(1));
			pos[2] = double(Position(2));
			PointArray_vtk->SetPoint(i, pos);
		}
	}
	else if (ScalarTypeName == "float")
	{
		PointArray_vtk->SetDataType(VTK_FLOAT);
		PointArray_vtk->SetNumberOfPoints(PointCount);
		for (int i = 0; i < PointCount; ++i)
		{
			auto Position = MDKMesh.GetPointPosition(i);
			float pos[3];
			pos[0] = float(Position(0));
			pos[1] = float(Position(1));
			pos[2] = float(Position(2));
			PointArray_vtk->SetPoint(i, pos);
		}
	}
	else
	{
		MDK_Warning("ScalarTypeName is not double or float @ ConvertMDKPolyhedronMeshToVTKUnstructuredGrid(...)")

		PointArray_vtk->SetDataType(VTK_DOUBLE);
		PointArray_vtk->SetNumberOfPoints(PointCount);
		for (int i = 0; i < PointCount; ++i)
		{
			auto Position = MDKMesh.GetPointPosition(i);
			double pos[3];
			pos[0] = double(Position(0));
			pos[1] = double(Position(1));
			pos[2] = double(Position(2));
			PointArray_vtk->SetPoint(i, pos);
		}
	}

	//------------------------------------------

	auto CellArray_vtk = vtkSmartPointer<vtkCellArray>::New();

	DenseVector<int> CellTypeList;

	for (int i = 0; i < CellCount_mdk; ++i)
	{
		auto Element = MDKMesh.Cell(i).GetPointIndexList();
		auto MeshCellType = MDKMesh.Cell(i).GetType();
		auto PointCountInCell = Element.GetLength();

		//not necessary, VTK support both order
		//if (MeshCellType == MeshCellTypeEnum::Wedge)
		//{
		//	 swap [0, 1, 2] <-> [3, 4, 5]			
		//	Element = { Element[3], Element[4], Element[5], Element[0], Element[1], Element[2] };
		//}

		CellArray_vtk->InsertNextCell(PointCountInCell);
		for (int n = 0; n < PointCountInCell; ++n)
		{
			auto PointIndex = Element[n];
			CellArray_vtk->InsertCellPoint(PointIndex);
		}
		
		switch (MeshCellType)
		{
		case MeshCellTypeEnum::Tetrahedron:
			CellTypeList.Append(VTKCellType::VTK_TETRA);
			break;
		case MeshCellTypeEnum::Wedge:
			CellTypeList.Append(VTKCellType::VTK_WEDGE);
			break;
		case MeshCellTypeEnum::Hexahedron:
			CellTypeList.Append(VTKCellType::VTK_HEXAHEDRON);
			break;
		default:
			CellTypeList.Append(VTKCellType::VTK_CONVEX_POINT_SET);
			// do NOT use this, weird result
			//CellTypeList[i] = VTKCellType::VTK_POLYHEDRON;			
		}
	}

	for (int_max k = 0; k < FaceCount; ++k)
	{
		if (MDKMesh.Face(k).GetAdjacentCellCount() == 0)
		{
			auto Element = MDKMesh.Face(k).GetPointIndexList();
			auto PointCountInCell = Element.GetLength();

			CellArray_vtk->InsertNextCell(PointCountInCell);
			for (int n = 0; n < PointCountInCell; ++n)
			{
				auto PointIndex = Element[n];
				CellArray_vtk->InsertCellPoint(PointIndex);
			}

			if (PointCountInCell == 3)
			{
				CellTypeList.Append(VTKCellType::VTK_TRIANGLE);
			}
			else if (PointCountInCell == 4)
			{
				CellTypeList.Append(VTKCellType::VTK_QUAD);
			}
			else
			{
				CellTypeList.Append(VTKCellType::VTK_POLYGON);
			}
		}
	}

	for (int_max k = 0; k < EdgeCount; ++k)
	{
		if (MDKMesh.Edge(k).GetAdjacentCellCount() == 0)
		{
			auto Element = MDKMesh.Edge(k).GetPointIndexList();
			auto PointCountInCell = Element.GetLength();
			CellArray_vtk->InsertNextCell(PointCountInCell);
			for (int n = 0; n < PointCountInCell; ++n)
			{
				auto PointIndex = Element[n];
				CellArray_vtk->InsertCellPoint(PointIndex);
			}
			CellTypeList.Append(VTKCellType::VTK_LINE);
		}
	}

	//---------------------------------------------------
	VTKMesh->SetPoints(PointArray_vtk);
	VTKMesh->SetCells(CellTypeList.GetPointer(), CellArray_vtk);
	//---------------------------------------------------
	//PointData, CellData NOT added
	//---------------------------------------------------
	return VTKMesh;
}

//--------------------------------------- convert vtkUnstructuredGrid to mdk Mesh ------------------------------------------------//
template<typename ScalarType>
bool ConvertVTKUnstructuredGridToMDKMesh(vtkUnstructuredGrid* VTKMesh, Mesh<ScalarType>& MDKMesh)
{
	if (VTKMesh == nullptr)
	{
		MDK_Error("VTKMesh is nullptr @ ConvertVTKUnstructuredGridToMDKPolyhedronMesh(...)")
		return false;
	}

	auto PointCount = int_max(VTKMesh->GetNumberOfPoints());
	auto CellCount = int_max(VTKMesh->GetNumberOfCells());
	MDKMesh.SetCapacity(PointCount, PointCount, PointCount, CellCount);

	if (PointCount == 0)
	{
		MDKMesh.Clear();
		return true;
	}
	//---------------------------------------------------------
	for (int_max k = 0; k < PointCount; ++k)
	{
		double pos[3];
		VTKMesh->GetPoint(k, pos);
		MDKMesh.AddPoint(ScalarType(pos[0]), ScalarType(pos[1]), ScalarType(pos[2]));
	}
	//---------------------------------------------------------
	for (int_max k = 0; k < CellCount; ++k)
	{
		auto Cell = VTKMesh->GetCell(k);
		auto PointCountInCell = int_max(Cell->GetNumberOfPoints());

		DenseVector<int_max> PointIndexList;
		PointIndexList.Resize(PointCountInCell);
		for (int_max n = 0; n < PointCountInCell; ++n)
		{
			PointIndexList[n] = int_max(Cell->GetPointId(n));
		}

		DenseVector<int_max> FaceIndexList;

		auto CellType = Cell->GetCellType();
		switch (CellType)
		{
		case VTKCellType::VTK_LINE:
		{
			MDKMesh.AddEdge(PointIndexList[0], PointIndexList[1]);
			break;
		}
		case VTKCellType::VTK_POLY_LINE:
		{		
			for (int_max n = 0; n < PointIndexList.GetLength()-1; ++n)
			{
				MDKMesh.AddEdge(PointIndexList[n], PointIndexList[n + 1]);
			}
			break;
		}
		case VTKCellType::VTK_TRIANGLE:
		{
			auto ElementType = MeshFaceTypeEnum::Triangle;
			MDKMesh.AddFaceByPoint(PointIndexList);
			break;
		}
		case VTKCellType::VTK_QUAD:
		{
			auto ElementType = MeshFaceTypeEnum::Quad;
			MDKMesh.AddFaceByPoint(PointIndexList);
			break;
		}
		case VTKCellType::VTK_POLYGON:
		{
			auto ElementType = MeshFaceTypeEnum::Polygon;
			MDKMesh.AddFaceByPoint(PointIndexList);
			break;
		}
		case VTKCellType::VTK_TETRA:
		{
			auto ElementType = MeshCellTypeEnum::Tetrahedron;
			MDKMesh.AddCellByPoint(ElementType, PointIndexList);
			break;
		}
		case VTKCellType::VTK_WEDGE:
		{
			auto ElementType = MeshCellTypeEnum::Wedge;			
			// swap [0, 1, 2] <-> [3, 4, 5] is not necessary
			MDKMesh.AddCellByPoint(ElementType, PointIndexList);
			break;
		}
		case VTKCellType::VTK_HEXAHEDRON:
		{
			auto ElementType = MeshCellTypeEnum::Hexahedron;
			MDKMesh.AddCellByPoint(ElementType, PointIndexList);
			break;
		}
		default:
		{
			auto ElementType = MeshCellTypeEnum::Polyhedron;
			MDK_Error("ElementType is Polyhedron, can not deduce Face from point @ ConvertVTKUnstructuredGridToMDKPolyhedronMesh(...)")
		}
		}
	}
	//---------------------------------------------------------
	auto PointDataSetCount = VTKMesh->GetPointData()->GetNumberOfArrays();
	for (int k = 0; k < PointDataSetCount; ++k)
	{
		auto DataArray_vtk = VTKMesh->GetPointData()->GetArray(k);
		String Name = DataArray_vtk->GetName();
		DenseMatrix<ScalarType> DataSet;
		DataSet.Resize(DataArray_vtk->GetNumberOfComponents(), DataArray_vtk->GetNumberOfTuples());
		for (int j = 0; j < DataSet.GetColCount(); ++j)
		{
			for (int i = 0; i < DataSet.GetRowCount(); ++i)
			{
				DataSet(i, j) = DataArray_vtk->GetComponent(j, i);
			}
		}
		MDKMesh.AddPointDataSet(Name, DataSet);
	}

	return true;
}

}// namespace mdk
