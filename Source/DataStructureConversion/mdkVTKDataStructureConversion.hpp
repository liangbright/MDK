#ifndef __mdkVTKDataStructureConversion_hpp
#define __mdkVTKDataStructureConversion_hpp


namespace mdk
{

template<typename PixelType>
vtkSmartPointer<vtkImageData> ConvertMDK3DScalarImageToVTK3DScalarImage(const Image3D<PixelType>& InputImage)
{
    auto VTKImage = vtkSmartPointer<vtkImageData>::New();

    auto Size = InputImage.GetSize();
    auto Origin = InputImage.GetOrigin();
    auto Spacing = InputImage.GetSpacing();

    auto PtrOfInputImage = InputImage.GetPixelPointer();

    VTKImage->SetExtent(0, Size.Lx - 1, 0, Size.Ly - 1, 0, Size.Lz - 1);
    VTKImage->SetOrigin(Origin.x, Origin.y, Origin.z);    
    VTKImage->SetSpacing(Spacing.Sx, Spacing.Sy, Spacing.Sz);

    auto ReferenceScalar = PixelType(0);
    auto ScalarTypeName = FindScalarTypeName(ReferenceScalar);

    if (ScalarTypeName == "double")
    {
        VTKImage->AllocateScalars(VTK_DOUBLE, 1);

        auto PtrOfVTKImage = static_cast<double*>(VTKImage->GetScalarPointer());

        for (int_max k = 0; k < InputImage.GetPixelNumber(); ++k)
        {
            PtrOfVTKImage[k] = PtrOfInputImage[k];
        }
    }
    else if (ScalarTypeName == "float")
    {
        VTKImage->AllocateScalars(VTK_FLOAT, 1);

        auto PtrOfVTKImage = static_cast<float*>(VTKImage->GetScalarPointer());

        for (int_max k = 0; k < InputImage.GetPixelNumber(); ++k)
        {
            PtrOfVTKImage[k] = PtrOfInputImage[k];
        }
    }
    else if (ScalarTypeName == "int8")
    {
        VTKImage->AllocateScalars(VTK_CHAR, 1);

        auto PtrOfVTKImage = static_cast<int8*>(VTKImage->GetScalarPointer());

        for (int_max k = 0; k < InputImage.GetPixelNumber(); ++k)
        {
            PtrOfVTKImage[k] = PtrOfInputImage[k];
        }
    }
    else if (ScalarTypeName == "int16")
    {
        VTKImage->AllocateScalars(VTK_SHORT, 1);

        auto PtrOfVTKImage = static_cast<int16*>(VTKImage->GetScalarPointer());

        for (int_max k = 0; k < InputImage.GetPixelNumber(); ++k)
        {
            PtrOfVTKImage[k] = PtrOfInputImage[k];
        }
    }
    else if (ScalarTypeName == "int32")
    {
        VTKImage->AllocateScalars(VTK_INT, 1);

        auto PtrOfVTKImage = static_cast<int32*>(VTKImage->GetScalarPointer());

        for (int_max k = 0; k < InputImage.GetPixelNumber(); ++k)
        {
            PtrOfVTKImage[k] = PtrOfInputImage[k];
        }
    }
    else if (ScalarTypeName == "int64")
    {
        VTKImage->AllocateScalars(VTK_LONG_LONG, 1);

        auto PtrOfVTKImage = static_cast<int64*>(VTKImage->GetScalarPointer());

        for (int_max k = 0; k < InputImage.GetPixelNumber(); ++k)
        {
            PtrOfVTKImage[k] = PtrOfInputImage[k];
        }
    }
    else if (ScalarTypeName == "uint8")
    {
        VTKImage->AllocateScalars(VTK_UNSIGNED_CHAR, 1);

        auto PtrOfVTKImage = static_cast<uint8*>(VTKImage->GetScalarPointer());

        for (int_max k = 0; k < InputImage.GetPixelNumber(); ++k)
        {
            PtrOfVTKImage[k] = PtrOfInputImage[k];
        }
    }
    else if (ScalarTypeName == "uint16")
    {
        VTKImage->AllocateScalars(VTK_UNSIGNED_SHORT, 1);

        auto PtrOfVTKImage = static_cast<uint16*>(VTKImage->GetScalarPointer());

        for (int_max k = 0; k < InputImage.GetPixelNumber(); ++k)
        {
            PtrOfVTKImage[k] = PtrOfInputImage[k];
        }
    }
    else if (ScalarTypeName == "uint32")
    {
        VTKImage->AllocateScalars(VTK_UNSIGNED_INT, 1);

        auto PtrOfVTKImage = static_cast<uint32*>(VTKImage->GetScalarPointer());

        for (int_max k = 0; k < InputImage.GetPixelNumber(); ++k)
        {
            PtrOfVTKImage[k] = PtrOfInputImage[k];
        }
    }
    else if (ScalarTypeName == "uint64")
    {
        VTKImage->AllocateScalars(VTK_UNSIGNED_LONG_LONG, 1);

        auto PtrOfVTKImage = static_cast<uint64*>(VTKImage->GetScalarPointer());

        for (int_max k = 0; k < InputImage.GetPixelNumber(); ++k)
        {
            PtrOfVTKImage[k] = PtrOfInputImage[k];
        }
    }
    else
    {
        MDK_Error("unknown ScalarType @ ConvertMDK3DScalarImageToVTK3DScalarImage(...) ")
    }

    return VTKImage;
}


template<typename PixelType>
Image3D<PixelType> ConvertVTK3DScalarImageToMDK3DScalarImage(const vtkImageData* VTKImage)
{
    int Extent[6];
    VTKImage->GetExtent(Extent);

    double Spacing[3];
    VTKImage->GetSpacing(Spacing);

    double Origin[3];
    VTKImage->GetOrigin(Origin);

    auto VTKScalarType = VTKImage->GetScalarType();

    Image3D<PixelType> OutputImage;
    OutputImage.SetSize(Extent[1] + 1, Extent[3] + 1, Extent[5] + 1);
    OutputImage.SetOrigin(Origin[0], Origin[1], Origin[2]);
    OutputImage.SetSpacing(Spacing[0], Spacing[1], Spacing[2]);

    auto PtrOfOutputImage = OutputImage.GetPixelPointer();

    if (VTKScalarType == VTK_DOUBLE)
    {
        auto PtrOfVTKImage = static_cast<double*>(VTKImage->GetScalarPointer());

        for (int_max k = 0; k < OutputImage.GetPixelNumber(); ++k)
        {
            PtrOfOutputImage[k] = PtrOfVTKImage[k];
        }
    }
    else if (VTKScalarType == VTK_FLOAT)
    {
        auto PtrOfVTKImage = static_cast<float*>(VTKImage->GetScalarPointer());

        for (int_max k = 0; k < OutputImage.GetPixelNumber(); ++k)
        {
            PtrOfOutputImage[k] = PtrOfVTKImage[k];
        }
    }
    else if (VTKScalarType == VTK_CHAR)
    {
        auto PtrOfVTKImage = static_cast<int8*>(VTKImage->GetScalarPointer());

        for (int_max k = 0; k < OutputImage.GetPixelNumber(); ++k)
        {
            PtrOfOutputImage[k] = PtrOfVTKImage[k];
        }
    }
    else if (VTKScalarType == VTK_SHORT)
    {
        auto PtrOfVTKImage = static_cast<int16*>(VTKImage->GetScalarPointer());

        for (int_max k = 0; k < OutputImage.GetPixelNumber(); ++k)
        {
            PtrOfOutputImage[k] = PtrOfVTKImage[k];
        }
    }
    else if (VTKScalarType == VTK_INT)
    {
        auto PtrOfVTKImage = static_cast<int32*>(VTKImage->GetScalarPointer());

        for (int_max k = 0; k < OutputImage.GetPixelNumber(); ++k)
        {
            PtrOfOutputImage[k] = PtrOfVTKImage[k];
        }
    }
    else if (VTKScalarType == VTK_LONG_LONG)
    {
        auto PtrOfVTKImage = static_cast<int64*>(VTKImage->GetScalarPointer());

        for (int_max k = 0; k < OutputImage.GetPixelNumber(); ++k)
        {
            PtrOfOutputImage[k] = PtrOfVTKImage[k];
        }
    }
    else if (VTKScalarType == VTK_UNSIGNED_CHAR)
    {
        auto PtrOfVTKImage = static_cast<uint8*>(VTKImage->GetScalarPointer());

        for (int_max k = 0; k < OutputImage.GetPixelNumber(); ++k)
        {
            PtrOfOutputImage[k] = PtrOfVTKImage[k];
        }
    }
    else if (VTKScalarType == VTK_UNSIGNED_SHORT)
    {
        auto PtrOfVTKImage = static_cast<uint16*>(VTKImage->GetScalarPointer());

        for (int_max k = 0; k < OutputImage.GetPixelNumber(); ++k)
        {
            PtrOfOutputImage[k] = PtrOfVTKImage[k];
        }
    }
    else if (VTKScalarType == VTK_UNSIGNED_INT)
    {
        auto PtrOfVTKImage = static_cast<uint32*>(VTKImage->GetScalarPointer());

        for (int_max k = 0; k < OutputImage.GetPixelNumber(); ++k)
        {
            PtrOfOutputImage[k] = PtrOfVTKImage[k];
        }
    }
    else if (VTKScalarType == VTK_UNSIGNED_LONG_LONG)
    {
        auto PtrOfVTKImage = static_cast<uint64*>(VTKImage->GetScalarPointer());

        for (int_max k = 0; k < OutputImage.GetPixelNumber(); ++k)
        {
            PtrOfOutputImage[k] = PtrOfVTKImage[k];
        }
    }
    else
    {
        MDK_Error("unknown ScalarType @ ConvertVTK3DScalarImageToMDK3DScalarImage(...) ")
    }

    return OutputImage;
}


template<typename MeshAttributeType>
vtkSmartPointer<vtkPolyData> ConvertMDKTriangleMeshToVTKPolyData(const TriangleMesh<MeshAttributeType>& InputMesh)
{
    typedef TriangleMesh<MeshAttributeType>::ScalarType ScalarType;

    int_max PointNumber = InputMesh.GetPointNumber();

    auto PointData = vtkSmartPointer<vtkPoints>::New();

    PointData->SetNumberOfPoints(PointNumber);

    auto ReferenceScalar = ScalarType(0);
    auto ScalarTypeName = FindScalarTypeName(ReferenceScalar);

    DenseMatrix<ScalarType> PointPositionTable;
    DataArray<DenseVector<int_max>> CellTable;
    InputMesh.GetPointPositionTableAndCellTable(PointPositionTable, CellTable);

    if (ScalarTypeName == "double")
    {
        PointData->SetDataType(VTK_DOUBLE);

        double pos[3] = { 0, 0, 0 };

        for (int i = 0; i < PointNumber; ++i)
        {
            pos[0] = PointPositionTable(0, i);
            pos[1] = PointPositionTable(1, i);
            pos[2] = PointPositionTable(2, i);

            PointData->InsertPoint(i, pos);
        }
    }
    else if (ScalarTypeName == "float")
    {
        PointData->SetDataType(VTK_FLOAT);

        float pos[3] = { 0, 0, 0 };

        for (int i = 0; i < PointNumber; ++i)
        {
            pos[0] = float(PointPositionTable(0, i));
            pos[1] = float(PointPositionTable(1, i));
            pos[2] = float(PointPositionTable(2, i));

            PointData->InsertPoint(i, pos);
        }
    }
    else
    {
        MDK_Warning("ScalarTypeName is not double or float @ ConvertMDKPolygonMeshToVTKPolyData(...)")

        PointData->SetDataType(VTK_FLOAT);

        float pos[3] = { 0, 0, 0 };

        for (int i = 0; i < PointNumber; ++i)
        {
            pos[0] = float(PointPositionTable(0, i));
            pos[1] = float(PointPositionTable(1, i));
            pos[2] = float(PointPositionTable(2, i));

            PointData->InsertPoint(i, pos);
        }
    }

    //------------------------------------------

    int_max CellNumber = CellTable.GetLength();

    auto CellData = vtkSmartPointer<vtkCellArray>::New();

    CellData->SetNumberOfCells(CellNumber);

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
    auto MeshData = vtkSmartPointer<vtkPolyData>::New();
    MeshData->SetPoints(PointData);
    MeshData->SetPolys(CellData);

    return MeshData;
}


template<typename MeshAttributeType>
TriangleMesh<MeshAttributeType> ConvertVTKPolyDataToMDKTriangleMesh(vtkPolyData* VTKPolyMesh)
{
    typedef TriangleMesh<MeshAttributeType>::ScalarType ScalarType;

    TriangleMesh<MeshAttributeType> OutputMesh;

    auto VTKTriangleMeshFilter = vtkSmartPointer<vtkTriangleFilter>::New();
    VTKTriangleMeshFilter->SetInputData(VTKPolyMesh);
    VTKTriangleMeshFilter->Update();
    auto VTKTriangleMesh = VTKTriangleMeshFilter->GetOutput();

    auto PointNumber = VTKTriangleMesh->GetNumberOfPoints();

    if (PointNumber == 0)
    {
        return OutputMesh;
    }

    DenseMatrix<ScalarType> PointData(3, PointNumber);

    for (int_max k = 0; k < PointNumber; ++k)
    {
        double pos[3];

        VTKTriangleMesh->GetPoint(k, pos);

        PointData.SetCol(k, pos);
    }

    auto CellNumber = VTKTriangleMesh->GetNumberOfCells();

    DataArray<DenseVector<int_max>> CellData;
    CellData.FastResize(CellNumber);

    for (int_max k = 0; k < CellNumber; ++k)
    {
        auto Cell = VTKPolyMesh->GetCell(k);

        CellData[k].Append(int_max(Cell->GetPointId(0)));
        CellData[k].Append(int_max(Cell->GetPointId(1)));
        CellData[k].Append(int_max(Cell->GetPointId(2)));
    }

    OutputMesh.Construct(PointData, CellData);

    return OutputMesh;
}


template<typename MeshAttributeType>
vtkSmartPointer<vtkPolyData> ConvertMDKPolygonMeshToVTKPolyData(const PolygonMesh<MeshAttributeType>& InputMesh)
{
    typedef PolygonMesh<MeshAttributeType>::ScalarType ScalarType;

    int_max PointNumber = InputMesh.GetPointNumber();

    auto PointData = vtkSmartPointer<vtkPoints>::New();

    PointData->SetNumberOfPoints(PointNumber);

    auto ReferenceScalar = ScalarType(0);
    auto ScalarTypeName = FindScalarTypeName(ReferenceScalar);

    DenseMatrix<ScalarType> PointPositionTable;
    DataArray<DenseVector<int_max>> CellTable;
    InputMesh.GetPointPositionTableAndCellTable(PointPositionTable, CellTable);

    if (ScalarTypeName == "double")
    {
        PointData->SetDataType(VTK_DOUBLE);

        double pos[3] = { 0, 0, 0 };

        for (int i = 0; i < PointNumber; ++i)
        {
            pos[0] = PointPositionTable(0, i);
            pos[1] = PointPositionTable(1, i);
            pos[2] = PointPositionTable(2, i);

            PointData->InsertPoint(i, pos);
        }
    }
    else if (ScalarTypeName == "float")
    {
        PointData->SetDataType(VTK_FLOAT);

        float pos[3] = { 0, 0, 0 };

        for (int i = 0; i < PointNumber; ++i)
        {
            pos[0] = float(PointPositionTable(0, i));
            pos[1] = float(PointPositionTable(1, i));
            pos[2] = float(PointPositionTable(2, i));

            PointData->InsertPoint(i, pos);
        }
    }
    else
    {
        MDK_Warning("ScalarTypeName is not double or float @ ConvertMDKPolygonMeshToVTKPolyData(...)")

        PointData->SetDataType(VTK_FLOAT);

        float pos[3] = { 0, 0, 0 };

        for (int i = 0; i < PointNumber; ++i)
        {
            pos[0] = float(PointPositionTable(0, i));
            pos[1] = float(PointPositionTable(1, i));
            pos[2] = float(PointPositionTable(2, i));

            PointData->InsertPoint(i, pos);
        }
    }

    //------------------------------------------

    int_max CellNumber = CellTable.GetLength();

    auto CellData = vtkSmartPointer<vtkCellArray>::New();

    CellData->SetNumberOfCells(CellNumber);

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
    auto MeshData = vtkSmartPointer<vtkPolyData>::New();
    MeshData->SetPoints(PointData);
    MeshData->SetPolys(CellData);

    return MeshData;
}


template<typename MeshAttributeType>
PolygonMesh<MeshAttributeType> ConvertVTKPolyDataToMDKPolygonMesh(vtkPolyData* VTKPolyMesh)
{
    typedef PolygonMesh<MeshAttributeType>::ScalarType ScalarType;

    PolygonMesh<MeshAttributeType> OutputMesh;
    
    auto PointNumber = int_max(VTKPolyMesh->GetNumberOfPoints());

    if (PointNumber == 0)
    {
        return OutputMesh;
    }

    DenseMatrix<ScalarType> PointData(3, PointNumber);

    for (int_max k = 0; k < PointNumber; ++k)
    {
        double pos[3];
        VTKPolyMesh->GetPoint(k, pos);
        PointData.SetCol(k, pos);
    }

    auto CellNumber = int_max(VTKPolyMesh->GetNumberOfCells());

    DataArray<DenseVector<int_max>> CellData;
    CellData.FastResize(CellNumber);

    for (int_max k = 0; k < CellNumber; ++k)
    {
        auto Cell = VTKPolyMesh->GetCell(k);
        auto PointNumberInCell = int_max(Cell->GetNumberOfPoints());

        for (int_max n = 0; n < PointNumberInCell; ++n)
        {
            CellData[k].Append(int_max(Cell->GetPointId(n)));
        }
    }

    OutputMesh.Construct(PointData, CellData);

    return OutputMesh;
}



}// namespace mdk


#endif