#include <vtkSmartPointer.h>
#include <vtkDICOMImageReader.h>
#include <vtkImageData.h>

#include <QFile.h>
#include <QTextStream.h>

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

    auto vtkImage = reader->GetOutput();

    int dims[3];

    vtkImage->GetDimensions(dims);

    double Origin[3];

    vtkImage->GetOrigin(Origin);

    double Spacing[3];

    vtkImage->GetSpacing(Spacing);

    tempImage.Initialize(dims[0],    dims[1],    dims[2],
                         Origin[0],  Origin[1],  Origin[2],
                         Spacing[0], Spacing[1], Spacing[2]);

    for (int z = 0; z < dims[2]; ++z)
    {
        for (int y = 0; y < dims[1]; ++y)
        {
            for (int x = 0; x < dims[0]; ++x)
            {
                tempImage(x, y, z) = vtkImage->GetScalarComponentAsDouble(x, y, z, 0);
            }
        }
    }

    return tempImage;

}


void SaveGrayScale3DImageAsRawDataFile(const std::string& FilePathAndName, const mdk3DImage<double>& Image)
{
    std::vector<NameValueQStringPair> PairList(10);

    auto Size = Image.GetImageSize();

    PairList[0].Name = "DataFileName";
    PairList[0].Value = QString(FilePathAndName.c_str()) + ".data";

    //QFile::remove(PairList[0].Value);

    PairList[1].Name = "ImageSize_x";
    PairList[1].Value = QString::number(Size.Lx);

    PairList[2].Name = "ImageSize_y";
    PairList[2].Value = QString::number(Size.Ly);

    PairList[3].Name = "ImageSize_z";
    PairList[3].Value = QString::number(Size.Lz);

    auto Origin = Image.GetPhysicalOrigin();

    PairList[4].Name = "Origin_x";
    PairList[4].Value = QString::number(Origin.x);

    PairList[5].Name = "Origin_y";
    PairList[5].Value = QString::number(Origin.y);

    PairList[6].Name = "Origin_z";
    PairList[6].Value = QString::number(Origin.z);

    auto Spacing = Image.GetVoxelPhysicalSize();

    PairList[7].Name = "Spacing_x";
    PairList[7].Value = QString::number(Spacing.Vx);

    PairList[8].Name = "Spacing_y";
    PairList[8].Value = QString::number(Spacing.Vy);

    PairList[9].Name = "Spacing_z";
    PairList[9].Value = QString::number(Spacing.Vz);

    WritePairListAsJsonFile(PairList, QString(FilePathAndName.c_str()));

    //--------------------------------------------------

    QFile DataFile(PairList[0].Value);

    if (!DataFile.open(QIODevice::WriteOnly))
    {
        mdkError << "Couldn't open file to write image data @ SaveGrayScale3DImageAsBinaryFile" << '\n';
        return;
    }

    uint64 L = Size.Lx * Size.Ly * Size.Lz;

    auto RawPointer = (char*) Image.GetVoxelDataRawPointer();

    DataFile.write(RawPointer, L*8);

    DataFile.flush();
}


bool WritePairListAsJsonFile(const std::vector<NameValueQStringPair>& PairList, const QString& FilePathAndName)
{    
    //QFile::remove(FilePathAndName + "~temp~.json");

    QFile JsonFile(FilePathAndName + "~temp~.json");

    if (!JsonFile.open(QIODevice::WriteOnly))
    {
        mdkError << "Couldn't open file to save result @ WritePairListAsJsonFile" << '\n';
        return false;
    }

    QTextStream out(&JsonFile);

    uint64 s = PairList.size();

    out << "{\n";
    for (uint64 i = 0; i < s; ++i)
    {
        out << "\"" << PairList[i].Name << "\"" << " : " << "\"" << PairList[i].Value << "\"";

        if (i < s - 1)
        {
            out << "," << "\n";
        }
        else
        {
            out << "\n";
        }
    }
    out << "}\n";

    out.flush();

    QFile::remove(FilePathAndName + ".json");

    return JsonFile.rename(FilePathAndName + ".json");
}


}//namespace mdk