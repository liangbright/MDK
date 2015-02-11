#ifndef mdk_VTKDataStructureConversion_hpp
#define mdk_VTKDataStructureConversion_hpp

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

//--------------------------------------- 3D Curve ------------------------------------------------------------//

template<typename ScalarType>
vtkSmartPointer<vtkPolyData> ConvertSingleMDK3DCurveToVTKPolyData(const DenseMatrix<ScalarType>& MDK3DCurve)
{
	auto VTKCurve = vtkSmartPointer<vtkPolyData>::New();
	ConvertSingleMDK3DCurveToVTKPolyData(MDK3DCurve, VTKCurve.GetPointer());
	return VTKCurve;
}


template<typename ScalarType>
bool ConvertSingleMDK3DCurveToVTKPolyData(const DenseMatrix<ScalarType>& MDK3DCurve, vtkPolyData& VTKCurve)
{
	return ConvertSingleMDK3DCurveToVTKPolyData(MDK3DCurve, &VTKCurve);
}


template<typename ScalarType>
bool ConvertSingleMDK3DCurveToVTKPolyData(const DenseMatrix<ScalarType>& MDK3DCurve, vtkPolyData* VTKCurve)
{
	if (VTKCurve == nullptr)
	{
		MDK_Error("input is nullptr @ ConvertSingleMDK3DCurveToVTKPolyData(...)")
		return false;
	}

	auto PointNumber = MDK3DCurve.GetColNumber();
	if (MDK3DCurve.GetRowNumber() != 3)
	{
		MDK_Error("Invalid MDK3DCurve @ mdkFileIO ConvertSingleMDK3DCurveToVTKPolyData")
		return false;
	}

	auto VtkDataType = GetVTKScalarType(ScalarType(0));

	auto PointSet = vtkSmartPointer<vtkPoints>::New();
	PointSet->SetDataType(VtkDataType);

	auto CellList = vtkSmartPointer<vtkCellArray>::New();
	CellList->InsertNextCell(PointNumber);

	for (int n = 0; n < PointNumber; ++n)
	{
		ScalarType Pos[3];
		MDK3DCurve.GetCol(n, Pos);
		CellList->InsertCellPoint(n);
		PointSet->InsertPoint(n, Pos);
	}

	VTKCurve->SetPoints(PointSet);
	VTKCurve->SetLines(CellList);
	return true;
}


template<typename ScalarType>
vtkSmartPointer<vtkPolyData> ConvertMultipleMDK3DCurveToVTKPolyData(const ObjectArray<DenseMatrix<ScalarType>>& MDK3DCurveList)
{
	auto VTKCurve = vtkSmartPointer<vtkPolyData>::New();
	ConvertMultipleMDK3DCurveToVTKPolyData(MDK3DCurveList, VTKCurve.GetPointer());
	return VTKCurve;
}


template<typename ScalarType>
bool ConvertMultipleMDK3DCurveToVTKPolyData(const ObjectArray<DenseMatrix<ScalarType>>& MDK3DCurveList, vtkPolyData& VTKCurve)
{
	return ConvertMultipleMDK3DCurveToVTKPolyData(MDK3DCurveList, &VTKCurve);
}


template<typename ScalarType>
bool ConvertMultipleMDK3DCurveToVTKPolyData(const ObjectArray<DenseMatrix<ScalarType>>& MDK3DCurveList, vtkPolyData* VTKCurve)
{
	if (VTKCurve == nullptr)
	{
		MDK_Error("input is nullptr @ ConvertMultipleMDK3DCurveToVTKPolyData(...)")
		return false;
	}
	
	if (MDK3DCurveList.IsEmpty() == true)
	{
		MDK_Warning("MDK3DCurveList is empty @ mdkFileIO ConvertSingleMDK3DCurveToVTKPolyData(...)")
		return false;
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
		auto PointNumber = MDK3DCurveList[k].GetColNumber();

		CellList->InsertNextCell(PointNumber);

		if (MDK3DCurveList[k].GetRowNumber() != 3)
		{
			MDK_Error("Invalid InputCurve @ mdkFileIO ConvertSingleMDK3DCurveToVTKPolyData")
			return VTKCurve;
		}

		for (int_max n = 0; n < PointNumber; ++n)
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
	return true;
}


template<typename ScalarType>
bool ConvertVTKPolyDataToMDK3DCurve(vtkPolyData& VTKCurveData, ObjectArray<DenseMatrix<ScalarType>>& MDK3DCurveData)
{
	return ConvertVTKPolyDataToMDK3DCurve(&VTKCurveData, MDK3DCurveData);
}


template<typename ScalarType>
bool ConvertVTKPolyDataToMDK3DCurve(vtkPolyData* VTKCurveData, ObjectArray<DenseMatrix<ScalarType>>& MDK3DCurveData)
{
	if (VTKCurveData == nullptr)
	{
		MDK_Error("VTKCurveData is nullptr @ ConvertVTKPolyDataToMDK3DCurve(...)")
		return false;
	}
	
	auto CurveNumber = int_max(VTKCurveData->GetNumberOfLines());
	//auto CellNumber = int_max(VTKCurveData->GetNumberOfCells());
	
	auto PointSet = VTKCurveData->GetPoints();
	//auto VTKCurveList = VTKCurveData->GetLines();

	MDK3DCurveData.FastResize(CurveNumber);

	for (int_max k = 0; k < CurveNumber; ++k)
	{
		auto Cell = VTKCurveData->GetCell(k);
		auto PointNumber = int_max(Cell->GetNumberOfPoints());

		auto& OutputCurve_k = MDK3DCurveData[k];
		OutputCurve_k.Resize(3, PointNumber);

		for (int_max n = 0; n < PointNumber; ++n)
		{
			double Pos[3];
			auto PointID = Cell->GetPointId(n);
			PointSet->GetPoint(PointID, Pos);
			OutputCurve_k.SetCol(n, Pos);
		}
	}
	return true;
}

//------------------------------------------------- 3D Scalar Image --------------------------------------------------//

template<typename PixelType>
vtkSmartPointer<vtkImageData> ConvertMDK3DScalarImageToVTK3DScalarImage(const DenseImage3D<PixelType>& MDKImage)
{
	auto VTKImage = vtkSmartPointer<vtkImageData>::New();
	ConvertMDK3DScalarImageToVTK3DScalarImage(MDKImage, VTKImage.GetPointer());
	return VTKImage;
}


template<typename PixelType>
bool ConvertMDK3DScalarImageToVTK3DScalarImage(const DenseImage3D<PixelType>& MDKImage, vtkImageData& VTKImage)
{
	return ConvertMDK3DScalarImageToVTK3DScalarImage(MDKImage, &VTKImage)
}


template<typename PixelType>
bool ConvertMDK3DScalarImageToVTK3DScalarImage(const DenseImage3D<PixelType>& MDKImage, vtkImageData* VTKImage)
{
	if (VTKImage == nullptr)
	{
		MDK_Error("input is nullptr @ ConvertMDK3DScalarImageToVTK3DScalarImage(...)")
		return false;
	}

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

		for (int_max k = 0; k < MDKImage.GetPixelNumber(); ++k)
        {
			PtrOfVTKImage[k] = PtrOfMDKImage[k];
        }
    }
    else if (ScalarTypeName == "float")
    {
        VTKImage->AllocateScalars(VTK_FLOAT, 1);

        auto PtrOfVTKImage = static_cast<float*>(VTKImage->GetScalarPointer());

		for (int_max k = 0; k < MDKImage.GetPixelNumber(); ++k)
        {
			PtrOfVTKImage[k] = PtrOfMDKImage[k];
        }
    }
    else if (ScalarTypeName == "int8")
    {
        VTKImage->AllocateScalars(VTK_CHAR, 1);

        auto PtrOfVTKImage = static_cast<int8*>(VTKImage->GetScalarPointer());

		for (int_max k = 0; k < MDKImage.GetPixelNumber(); ++k)
        {
			PtrOfVTKImage[k] = PtrOfMDKImage[k];
        }
    }
    else if (ScalarTypeName == "int16")
    {
        VTKImage->AllocateScalars(VTK_SHORT, 1);

        auto PtrOfVTKImage = static_cast<int16*>(VTKImage->GetScalarPointer());

		for (int_max k = 0; k < MDKImage.GetPixelNumber(); ++k)
        {
			PtrOfVTKImage[k] = PtrOfMDKImage[k];
        }
    }
    else if (ScalarTypeName == "int32")
    {
        VTKImage->AllocateScalars(VTK_INT, 1);

        auto PtrOfVTKImage = static_cast<int32*>(VTKImage->GetScalarPointer());

		for (int_max k = 0; k < MDKImage.GetPixelNumber(); ++k)
        {
			PtrOfVTKImage[k] = PtrOfMDKImage[k];
        }
    }
    else if (ScalarTypeName == "int64")
    {
        VTKImage->AllocateScalars(VTK_LONG_LONG, 1);

        auto PtrOfVTKImage = static_cast<int64*>(VTKImage->GetScalarPointer());

		for (int_max k = 0; k < MDKImage.GetPixelNumber(); ++k)
        {
			PtrOfVTKImage[k] = PtrOfMDKImage[k];
        }
    }
    else if (ScalarTypeName == "uint8")
    {
        VTKImage->AllocateScalars(VTK_UNSIGNED_CHAR, 1);

        auto PtrOfVTKImage = static_cast<uint8*>(VTKImage->GetScalarPointer());

		for (int_max k = 0; k < MDKImage.GetPixelNumber(); ++k)
        {
			PtrOfVTKImage[k] = PtrOfMDKImage[k];
        }
    }
    else if (ScalarTypeName == "uint16")
    {
        VTKImage->AllocateScalars(VTK_UNSIGNED_SHORT, 1);

        auto PtrOfVTKImage = static_cast<uint16*>(VTKImage->GetScalarPointer());

		for (int_max k = 0; k < MDKImage.GetPixelNumber(); ++k)
        {
			PtrOfVTKImage[k] = PtrOfMDKImage[k];
        }
    }
    else if (ScalarTypeName == "uint32")
    {
        VTKImage->AllocateScalars(VTK_UNSIGNED_INT, 1);

        auto PtrOfVTKImage = static_cast<uint32*>(VTKImage->GetScalarPointer());

		for (int_max k = 0; k < MDKImage.GetPixelNumber(); ++k)
        {
			PtrOfVTKImage[k] = PtrOfMDKImage[k];
        }
    }
    else if (ScalarTypeName == "uint64")
    {
        VTKImage->AllocateScalars(VTK_UNSIGNED_LONG_LONG, 1);

        auto PtrOfVTKImage = static_cast<uint64*>(VTKImage->GetScalarPointer());

		for (int_max k = 0; k < MDKImage.GetPixelNumber(); ++k)
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


template<typename PixelType>
bool ConvertVTK3DScalarImageToMDK3DScalarImage(const vtkImageData& VTKImage, DenseImage3D<PixelType>& MDKImage)
{
	return ConvertVTK3DScalarImageToMDK3DScalarImage(&VTKImage, MDKImage);
}


template<typename PixelType>
bool ConvertVTK3DScalarImageToMDK3DScalarImage(const vtkImageData* VTKImage, DenseImage3D<PixelType>& MDKImage)
{
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

		for (int_max k = 0; k < MDKImage.GetPixelNumber(); ++k)
        {
			PtrOfMDKImage[k] = PtrOfVTKImage[k];
        }
    }
    else if (VTKScalarType == VTK_FLOAT)
    {
        auto PtrOfVTKImage = static_cast<float*>(VTKImage->GetScalarPointer());

		for (int_max k = 0; k < MDKImage.GetPixelNumber(); ++k)
        {
			PtrOfMDKImage[k] = PtrOfVTKImage[k];
        }
    }
    else if (VTKScalarType == VTK_CHAR)
    {
        auto PtrOfVTKImage = static_cast<int8*>(VTKImage->GetScalarPointer());

		for (int_max k = 0; k < MDKImage.GetPixelNumber(); ++k)
        {
			PtrOfMDKImage[k] = PtrOfVTKImage[k];
        }
    }
    else if (VTKScalarType == VTK_SHORT)
    {
        auto PtrOfVTKImage = static_cast<int16*>(VTKImage->GetScalarPointer());

		for (int_max k = 0; k < MDKImage.GetPixelNumber(); ++k)
        {
			PtrOfMDKImage[k] = PtrOfVTKImage[k];
        }
    }
    else if (VTKScalarType == VTK_INT)
    {
        auto PtrOfVTKImage = static_cast<int32*>(VTKImage->GetScalarPointer());

		for (int_max k = 0; k < MDKImage.GetPixelNumber(); ++k)
        {
			PtrOfMDKImage[k] = PtrOfVTKImage[k];
        }
    }
    else if (VTKScalarType == VTK_LONG_LONG)
    {
        auto PtrOfVTKImage = static_cast<int64*>(VTKImage->GetScalarPointer());

		for (int_max k = 0; k < MDKImage.GetPixelNumber(); ++k)
        {
			PtrOfMDKImage[k] = PtrOfVTKImage[k];
        }
    }
    else if (VTKScalarType == VTK_UNSIGNED_CHAR)
    {
        auto PtrOfVTKImage = static_cast<uint8*>(VTKImage->GetScalarPointer());

		for (int_max k = 0; k < MDKImage.GetPixelNumber(); ++k)
        {
			PtrOfMDKImage[k] = PtrOfVTKImage[k];
        }
    }
    else if (VTKScalarType == VTK_UNSIGNED_SHORT)
    {
        auto PtrOfVTKImage = static_cast<uint16*>(VTKImage->GetScalarPointer());

		for (int_max k = 0; k < MDKImage.GetPixelNumber(); ++k)
        {
			PtrOfMDKImage[k] = PtrOfVTKImage[k];
        }
    }
    else if (VTKScalarType == VTK_UNSIGNED_INT)
    {
        auto PtrOfVTKImage = static_cast<uint32*>(VTKImage->GetScalarPointer());

		for (int_max k = 0; k < MDKImage.GetPixelNumber(); ++k)
        {
			PtrOfMDKImage[k] = PtrOfVTKImage[k];
        }
    }
    else if (VTKScalarType == VTK_UNSIGNED_LONG_LONG)
    {
        auto PtrOfVTKImage = static_cast<uint64*>(VTKImage->GetScalarPointer());

		for (int_max k = 0; k < MDKImage.GetPixelNumber(); ++k)
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

//------------------------------------------- PolygonMesh ------------------------------------------------------//

template<typename MeshAttributeType>
vtkSmartPointer<vtkPolyData> ConvertMDKPolygonMeshToVTKPolyData(const PolygonMesh<MeshAttributeType>& MDKMesh)
{
	auto VTKMesh = vtkSmartPointer<vtkPolyData>::New();
	ConvertMDKPolygonMeshToVTKPolyData(MDKMesh, VTKMesh.GetPointer());
	return VTKMesh;
}


template<typename MeshAttributeType>
bool ConvertMDKPolygonMeshToVTKPolyData(const PolygonMesh<MeshAttributeType>& MDKMesh, vtkPolyData& VTKMesh)
{
	return ConvertMDKPolygonMeshToVTKPolyData(MDKMesh, &VTKMesh);
}


template<typename MeshAttributeType>
bool ConvertMDKPolygonMeshToVTKPolyData(const PolygonMesh<MeshAttributeType>& MDKMesh, vtkPolyData* VTKMesh)
{
	if (VTKMesh == nullptr)
	{
		MDK_Error("input is nullptr @ ConvertMDKPolygonMeshToVTKPolyData(...)")
		return false;
	}
	
    typedef PolygonMesh<MeshAttributeType>::ScalarType ScalarType;

    auto ReferenceScalar = ScalarType(0);
	auto ScalarTypeName = GetScalarTypeName(ReferenceScalar);

    DenseMatrix<ScalarType> PointPositionTable;
	ObjectArray<DenseVector<int_max>> CellTable;
    MDKMesh.GetPointPositionMatrixAndCellTable(PointPositionTable, CellTable);

    int_max PointNumber = PointPositionTable.GetColNumber();
    int_max CellNumber = CellTable.GetLength();

    auto PointData = vtkSmartPointer<vtkPoints>::New();

    if (ScalarTypeName == "double")
    {
        PointData->SetDataType(VTK_DOUBLE);
        PointData->SetNumberOfPoints(PointNumber);
       
        for (int i = 0; i < PointNumber; ++i)
        {
			double pos[3];
			pos[0] = double(PointPositionTable(0, i));
			pos[1] = double(PointPositionTable(1, i));
			pos[2] = double(PointPositionTable(2, i));
            PointData->SetPoint(i, pos);
        }
    }
    else if (ScalarTypeName == "float")
    {
        PointData->SetDataType(VTK_FLOAT);
        PointData->SetNumberOfPoints(PointNumber);        

        for (int i = 0; i < PointNumber; ++i)
        {
			float pos[3];
            pos[0] = float(PointPositionTable(0, i));
            pos[1] = float(PointPositionTable(1, i));
            pos[2] = float(PointPositionTable(2, i));
            PointData->SetPoint(i, pos);
        }
    }
    else
    {
        MDK_Warning("ScalarTypeName is not double or float @ ConvertMDKPolygonMeshToVTKPolyData(...)")

        PointData->SetDataType(VTK_DOUBLE);
        PointData->SetNumberOfPoints(PointNumber);

        for (int i = 0; i < PointNumber; ++i)
        {
			double pos[3];
			pos[0] = double(PointPositionTable(0, i));
			pos[1] = double(PointPositionTable(1, i));
			pos[2] = double(PointPositionTable(2, i));
            PointData->SetPoint(i, pos);
        }
    }

    //------------------------------------------

    auto CellData = vtkSmartPointer<vtkCellArray>::New();

    for (int i = 0; i < CellNumber; ++i)
    {
        auto PointNumberInCell = CellTable[i].GetElementNumber();

        CellData->InsertNextCell(PointNumberInCell);

        for (int n = 0; n < PointNumberInCell; ++n)
        {
            auto PointIndex = CellTable[i][n];

            CellData->InsertCellPoint(PointIndex);
        }
    }
    //---------------------------------------------------
    VTKMesh->SetPoints(PointData);
    VTKMesh->SetPolys(CellData);
    return true;
}


template<typename MeshAttributeType>
bool ConvertVTKPolyDataToMDKPolygonMesh(vtkPolyData& VTKMesh, PolygonMesh<MeshAttributeType>& MDKMesh)
{
	return ConvertVTKPolyDataToMDKPolygonMesh(&VTKMesh, MDKMesh);
}


template<typename MeshAttributeType>
bool ConvertVTKPolyDataToMDKPolygonMesh(vtkPolyData* VTKMesh, PolygonMesh<MeshAttributeType>& MDKMesh)
{
	if (VTKMesh == nullptr)
	{
		MDK_Error("VTKMesh is nullptr @ ConvertVTKPolyDataToMDKPolygonMesh(...)")
		return false;
	}
	
    typedef PolygonMesh<MeshAttributeType>::ScalarType ScalarType;
    
	auto PointNumber = int_max(VTKMesh->GetNumberOfPoints());

    if (PointNumber == 0)
    {
		MDKMesh.Clear();
        return true;
    }

    DenseMatrix<ScalarType> PointData(3, PointNumber);

    for (int_max k = 0; k < PointNumber; ++k)
    {
        double pos[3];
		VTKMesh->GetPoint(k, pos);
        PointData.SetCol(k, pos);
    }

	auto CellNumber = int_max(VTKMesh->GetNumberOfCells());

	ObjectArray<DenseVector<int_max>> CellData;
    CellData.FastResize(CellNumber);

    for (int_max k = 0; k < CellNumber; ++k)
    {
		auto Cell = VTKMesh->GetCell(k);
        auto PointNumberInCell = int_max(Cell->GetNumberOfPoints());

        for (int_max n = 0; n < PointNumberInCell; ++n)
        {
            CellData[k].Append(int_max(Cell->GetPointId(n)));
        }
    }

	MDKMesh.Construct(std::move(PointData), CellData);

    return true;
}

//--------------------------------------------- TriangleMesh ------------------------------------------------------//

template<typename MeshAttributeType>
vtkSmartPointer<vtkPolyData> ConvertMDKTriangleMeshToVTKPolyData(const TriangleMesh<MeshAttributeType>& MDKMesh)
{
	auto VTKMesh = vtkSmartPointer<vtkPolyData>::New();
	ConvertMDKTriangleMeshToVTKPolyData(MDKMesh, VTKMesh.GetPointer());
	return VTKMesh;
}


template<typename MeshAttributeType>
bool ConvertMDKTriangleMeshToVTKPolyData(const TriangleMesh<MeshAttributeType>& MDKMesh, vtkPolyData& VTKMesh)
{
	return ConvertMDKTriangleMeshToVTKPolyData(MDKMesh, &VTKMesh);
}


template<typename MeshAttributeType>
bool ConvertMDKTriangleMeshToVTKPolyData(const TriangleMesh<MeshAttributeType>& MDKMesh, vtkPolyData* VTKMesh)
{
	if (VTKMesh == nullptr)
	{
		MDK_Error("input is nullptr @ ConvertMDKTriangleMeshToVTKPolyData(...)")
		return false;
	}

	typedef TriangleMesh<MeshAttributeType>::ScalarType ScalarType;

	auto ReferenceScalar = ScalarType(0);
	auto ScalarTypeName = GetScalarTypeName(ReferenceScalar);

	DenseMatrix<ScalarType> PointPositionTable;
	ObjectArray<DenseVector<int_max>> CellTable;
	MDKMesh.GetPointPositionMatrixAndCellTable(PointPositionTable, CellTable);

	int_max PointNumber = PointPositionTable.GetColNumber();
	int_max CellNumber = CellTable.GetLength();

	auto PointData = vtkSmartPointer<vtkPoints>::New();

	if (ScalarTypeName == "double")
	{
		PointData->SetDataType(VTK_DOUBLE); // this will clear internal memory of PointData
		PointData->SetNumberOfPoints(PointNumber);

		for (int i = 0; i < PointNumber; ++i)
		{
			double pos[3];
			pos[0] = double(PointPositionTable(0, i));
			pos[1] = double(PointPositionTable(1, i));
			pos[2] = double(PointPositionTable(2, i));
			PointData->SetPoint(i, pos);
		}
	}
	else if (ScalarTypeName == "float")
	{
		PointData->SetDataType(VTK_FLOAT);
		PointData->SetNumberOfPoints(PointNumber);

		for (int i = 0; i < PointNumber; ++i)
		{
			float pos[3];
			pos[0] = float(PointPositionTable(0, i));
			pos[1] = float(PointPositionTable(1, i));
			pos[2] = float(PointPositionTable(2, i));
			PointData->SetPoint(i, pos);
		}
	}
	else
	{
		MDK_Warning("ScalarTypeName is not double or float @ ConvertMDKPolygonMeshToVTKPolyData(...)")

		PointData->SetDataType(VTK_DOUBLE);
		PointData->SetNumberOfPoints(PointNumber);
		
		for (int i = 0; i < PointNumber; ++i)
		{
			double pos[3];
			pos[0] = double(PointPositionTable(0, i));
			pos[1] = double(PointPositionTable(1, i));
			pos[2] = double(PointPositionTable(2, i));
			PointData->SetPoint(i, pos);
		}
	}

	//------------------------------------------

	auto CellData = vtkSmartPointer<vtkCellArray>::New();

	for (int i = 0; i < CellNumber; ++i)
	{
		auto PointNumberInCell = CellTable[i].GetElementNumber();

		CellData->InsertNextCell(PointNumberInCell);

		for (int n = 0; n < PointNumberInCell; ++n)
		{
			auto PointIndex = CellTable[i][n];

			CellData->InsertCellPoint(PointIndex);
		}
	}

	//---------------------------------------------------
	VTKMesh->SetPoints(PointData);
	VTKMesh->SetPolys(CellData);
	return true;
}


template<typename MeshAttributeType>
bool ConvertVTKPolyDataToMDKTriangleMesh(vtkPolyData& VTKMesh, TriangleMesh<MeshAttributeType>& MDKMesh)
{
	return ConvertVTKPolyDataToMDKTriangleMesh(&VTKMesh, MDKMesh);
}


template<typename MeshAttributeType>
bool ConvertVTKPolyDataToMDKTriangleMesh(vtkPolyData* VTKMesh, TriangleMesh<MeshAttributeType>& MDKMesh)
{
	if (VTKMesh == nullptr)
	{
		MDK_Error("VTKMesh is nullptr @ ConvertVTKPolyDataToMDKTriangleMesh(...)")
			return false;
	}

	typedef TriangleMesh<MeshAttributeType>::ScalarType ScalarType;

	auto VTKTriangleMeshFilter = vtkSmartPointer<vtkTriangleFilter>::New();
	VTKTriangleMeshFilter->SetInputData(VTKMesh);
	VTKTriangleMeshFilter->Update();
	auto VTKTriangleMesh = VTKTriangleMeshFilter->GetOutput();

	auto PointNumber = VTKTriangleMesh->GetNumberOfPoints();

	if (PointNumber == 0)
	{
		MDKMesh.Clear();
		return true;
	}

	DenseMatrix<ScalarType> PointData(3, PointNumber);
	for (int_max k = 0; k < PointNumber; ++k)
	{
		double pos[3];
		VTKTriangleMesh->GetPoint(k, pos);
		PointData.SetCol(k, pos);
	}

	auto CellNumber = VTKTriangleMesh->GetNumberOfCells();

	ObjectArray<DenseVector<int_max>> CellData;
	CellData.FastResize(CellNumber);

	for (int_max k = 0; k < CellNumber; ++k)
	{
		auto Cell = VTKTriangleMesh->GetCell(k);

		CellData[k].Append(int_max(Cell->GetPointId(0)));
		CellData[k].Append(int_max(Cell->GetPointId(1)));
		CellData[k].Append(int_max(Cell->GetPointId(2)));
	}

	MDKMesh.Construct(PointData, CellData);

	return true;
}


}// namespace mdk


#endif