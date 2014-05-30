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


template<typename ScalarType>
vtkSmartPointer<vtkPolyData> ConvertMDKTriangleMeshToVTKPolyData(const TriangleMesh<ScalarType>& InputMesh)
{
    int_max PointNumber = InputMesh.GetVertexNumber();

    auto PointData = vtkSmartPointer<vtkPoints>::New();

    PointData->SetNumberOfPoints(PointNumber);

    auto ReferenceScalar = ScalarType(0);
    auto ScalarTypeName = FindScalarTypeName(ReferenceScalar);

    if (ScalarTypeName == "double")
    {
        PointData->SetDataType(VTK_DOUBLE);

        double pos[3] = { 0, 0, 0 };

        for (int i = 0; i < PointNumber; ++i)
        {
            pos[0] = InputMesh.Vertex()(0, i);
            pos[1] = InputMesh.Vertex()(1, i);
            pos[2] = InputMesh.Vertex()(2, i);

            PointData->InsertPoint(i, pos);
        }
    }
    else if (ScalarTypeName == "float")
    {
        PointData->SetDataType(VTK_FLOAT);

        float pos[3] = { 0, 0, 0 };

        for (int i = 0; i < PointNumber; ++i)
        {
            pos[0] = float(InputMesh.Vertex()(0, i));
            pos[1] = float(InputMesh.Vertex()(1, i));
            pos[2] = float(InputMesh.Vertex()(2, i));

            PointData->InsertPoint(i, pos);
        }
    }
    else
    {
        MDK_Warning("ScalarTypeName is not double or float @ ConvertMDKTriangleMeshToVTKPolyData(...)")

        PointData->SetDataType(VTK_FLOAT);

        float pos[3] = { 0, 0, 0 };

        for (int i = 0; i < PointNumber; ++i)
        {
            pos[0] = float(InputMesh.Vertex()(0, i));
            pos[1] = float(InputMesh.Vertex()(1, i));
            pos[2] = float(InputMesh.Vertex()(2, i));

            PointData->InsertPoint(i, pos);
        }
    }

    //------------------------------------------

    int_max CellNumber = InputMesh.GetTriangleNumber();

    auto CellData = vtkSmartPointer<vtkCellArray>::New();

    CellData->SetNumberOfCells(CellNumber);

    for (int i = 0; i < CellNumber; ++i)
    {
        CellData->InsertNextCell(3);

        for (int n = 0; n < 3; ++n)
        {
            auto PointIndex = InputMesh.Triangle(n, i);

            CellData->InsertCellPoint(PointIndex);
        }
    }

    //---------------------------------------------------

    auto MeshData = vtkSmartPointer<vtkPolyData>::New();

    MeshData->SetPoints(PointData);

    MeshData->SetPolys(CellData);

    return MeshData;
}


template<typename ScalarType>
TriangleMesh<ScalarType> ConvertVTKPolyDataToMDKTriangleMesh(vtkPolyData* VTKPolyMesh, bool Flag_BuildLinkAndAjacency)
{
    TriangleMesh<ScalarType> OutputMesh;

    auto VTKTriangleMeshFilter = vtkSmartPointer<vtkTriangleFilter>::New();
    VTKTriangleMeshFilter->SetInputData(VTKPolyMesh);
    VTKTriangleMeshFilter->Update();
    auto VTKTriangleMesh = VTKTriangleMeshFilter->GetOutput();

    auto PointNumber = VTKTriangleMesh->GetNumberOfPoints();

    if (PointNumber == 0)
    {
        return OutputMesh;
    }

    DenseMatrix<ScalarType> Vertex(3, PointNumber);

    for (int_max k = 0; k < PointNumber; ++k)
    {
        double pos[3];

        VTKTriangleMesh->GetPoint(k, pos);

        Vertex.SetCol(k, pos);
    }

    auto CellNumber = VTKTriangleMesh->GetNumberOfCells();

    DenseMatrix<int_max> Triangle(3, CellNumber);

    for (int_max k = 0; k < CellNumber; ++k)
    {
        auto Cell = VTKTriangleMesh->GetCell(k);
        
        Triangle.SetCol(k, { Cell->GetPointId(0), Cell->GetPointId(1), Cell->GetPointId(2) });
    }

    OutputMesh.Construct(std::move(Vertex), std::move(Triangle), Flag_BuildLinkAndAjacency);

    return OutputMesh;
}


template<typename ScalarType>
vtkSmartPointer<vtkPolyData> ConvertMDKPolygonMeshToVTKPolyData(const PolygonMesh<ScalarType>& InputMesh)
{
    int_max PointNumber = InputMesh.GetVertexNumber();

    auto PointData = vtkSmartPointer<vtkPoints>::New();

    PointData->SetNumberOfPoints(PointNumber);

    auto ReferenceScalar = ScalarType(0);
    auto ScalarTypeName = FindScalarTypeName(ReferenceScalar);

    if (ScalarTypeName == "double")
    {
        PointData->SetDataType(VTK_DOUBLE);

        double pos[3] = { 0, 0, 0 };

        for (int i = 0; i < PointNumber; ++i)
        {
            pos[0] = InputMesh.Vertex()(0, i);
            pos[1] = InputMesh.Vertex()(1, i);
            pos[2] = InputMesh.Vertex()(2, i);

            PointData->InsertPoint(i, pos);
        }
    }
    else if (ScalarTypeName == "float")
    {
        PointData->SetDataType(VTK_FLOAT);

        float pos[3] = { 0, 0, 0 };

        for (int i = 0; i < PointNumber; ++i)
        {
            pos[0] = float(InputMesh.Vertex()(0, i));
            pos[1] = float(InputMesh.Vertex()(1, i));
            pos[2] = float(InputMesh.Vertex()(2, i));

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
            pos[0] = float(InputMesh.Vertex()(0, i));
            pos[1] = float(InputMesh.Vertex()(1, i));
            pos[2] = float(InputMesh.Vertex()(2, i));

            PointData->InsertPoint(i, pos);
        }
    }

    //------------------------------------------

    int_max CellNumber = InputMesh.GetPolygonNumber();

    auto CellData = vtkSmartPointer<vtkCellArray>::New();

    CellData->SetNumberOfCells(CellNumber);

    for (int i = 0; i < CellNumber; ++i)
    {
        auto PointNumberInCell = InputMesh.Polygon()[i].GetElementNumber();

        CellData->InsertNextCell(PointNumberInCell);

        for (int n = 0; n < PointNumberInCell; ++n)
        {
            auto PointIndex = InputMesh.Polygon()[i][n];

            CellData->InsertCellPoint(PointIndex);
        }
    }

    //---------------------------------------------------

    auto MeshData = vtkSmartPointer<vtkPolyData>::New();

    MeshData->SetPoints(PointData);

    MeshData->SetPolys(CellData);

    return MeshData;
}


template<typename ScalarType>
PolygonMesh<ScalarType> ConvertVTKPolyDataToMDKPolygonMesh(vtkPolyData* VTKPolyMesh, bool Flag_BuildLinkAndAjacency)
{
    PolygonMesh<ScalarType> OutputMesh;
    
    auto PointNumber = int_max(VTKPolyMesh->GetNumberOfPoints());

    if (PointNumber == 0)
    {
        return OutputMesh;
    }

    DenseMatrix<ScalarType> Vertex(3, PointNumber);

    for (int_max k = 0; k < PointNumber; ++k)
    {
        double pos[3];

        VTKPolyMesh->GetPoint(k, pos);

        Vertex.SetCol(k, pos);
    }

    auto CellNumber = int_max(VTKPolyMesh->GetNumberOfCells());

    DataContainer<SimpleDataContainer<int_max>> Polygon;
    Polygon.FastResize(CellNumber);

    for (int_max k = 0; k < CellNumber; ++k)
    {
        auto Cell = VTKPolyMesh->GetCell(k);

        auto PointNumberInCell = int_max(Cell->GetNumberOfPoints());

        for (int_max n = 0; n < PointNumberInCell; ++n)
        {
            Polygon[k].Append(int_max(Cell->GetPointId(n)));
        }
    }

    OutputMesh.Construct(std::move(Vertex), std::move(Polygon), Flag_BuildLinkAndAjacency);

    return OutputMesh;
}



}// namespace mdk


#endif