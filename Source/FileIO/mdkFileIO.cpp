#include <vtkSmartPointer.h>
#include <vtkDICOMImageReader.h>
#include <vtkImageData.h>

#include "mdkFileIO.h"

namespace mdk
{

mdk3DImage<double> ReadGrayScale3DImageFromDICOMFile(const std::string& FilePath)
{
    mdk3DImage<double> tempImage;

    tempImage.SetTobeTemporaryImage();

    auto reader = vtkSmartPointer<vtkDICOMImageReader>::New();

    reader->SetDirectoryName(FilePath.c_str());

    reader->Update();

    auto Image = reader->GetOutput();

    int dims[3];

    Image->GetDimensions(dims);

    double Origin[3];

    Image->GetOrigin(Origin);

    double Spacing[3];

    Image->GetSpacing(Spacing);

    tempImage.Initialize(dims[0],    dims[1],    dims[2],
                         Origin[0],  Origin[1],  Origin[2],
                         Spacing[0], Spacing[1], Spacing[2]);

    for (int z = 0; z < dims[2]; ++z)
    {
        for (int y = 0; y < dims[1]; ++y)
        {
            for (int x = 0; x < dims[0]; ++x)
            {
                tempImage(x, y, z) = Image->GetScalarComponentAsDouble(x, y, z, 0);
            }
        }
    }

    return tempImage;

}


void SaveGrayScale3DImageToBinaryFile(const std::string& FilePathAndName, const std::string& FileFormat, const mdk3DImage<double>& Image)
{

}

}