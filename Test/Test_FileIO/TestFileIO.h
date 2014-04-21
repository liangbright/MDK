#ifndef __TestFileIO_h
#define __TestFileIO_h

#include "mdkFileIO.h"

namespace mdk
{

void Test_ImageIO()
{

    DenseMatrix<double> A = {};

    std::string FilePath;
    
    //FilePath = "E:/HeartData/P1943091-im_6-phase10-close-leaflet/im_6/phase0";

    FilePath = "H:/Heart_backup/Data/Yale_CT/P1940650/phase0";

    //FilePath = "C:/Research/Test_ITK/SpineData/1";


    auto InputImage = LoadGrayScale3DImageFromDICOMSeries<double>(FilePath);

    std::string OutputFilePathAndName = "H:/Heart_backup/Data/Yale_CT/P1940650/phase0_temp";

    Image3D<double>& OutputImage = InputImage;

    SaveGrayScale3DImageAsJsonDataFile(OutputImage, OutputFilePathAndName);
}


void Test_MatrixIO()
{
    std::string FilePath = "E:/HeartData/P1943091-im_6-phase10-close-leaflet/im_6/Test_InputMatrix";

    auto InputMatrix = LoadScalarDenseMatrixFromJsonDataFile<float>(FilePath);

    DisplayMatrix("InputMatrix", InputMatrix);

    DenseMatrix<double> OutputMatrix(10, 10);

    OutputMatrix.Fill(0);

    OutputMatrix.FillDiangonal(1);

    std::string OutputFilePathAndName ="E:/HeartData/P1943091-im_6-phase10-close-leaflet/im_6/Test_OutputMatrix";

    SaveScalarDenseMatrixAsJsonDataFile(OutputMatrix, OutputFilePathAndName);


}


void Test_Image_Matrix_IO()
{
    std::string FilePath;

    FilePath = "H:/Heart_backup/Data/Yale_CT/P1940650/phase0/";

    auto InputImage = LoadGrayScale3DImageFromDICOMSeries<double>(FilePath);

    auto Size = InputImage.GetSize();

    DenseMatrix<double> Mean(1, Size.Lz);
    Mean.Fill(0);

    for (int_max z = 0; z < Size.Lz; ++z)
    {
        double temp = 0;

        for (int_max y = 0; y < Size.Ly; ++y)
        {
            for (int_max x = 0; x < Size.Lx; ++x)
            {
                temp += InputImage(x, y, z);
            }
        }

        temp /= Size.Lx*Size.Ly;

        Mean[z] = temp;
    }

    std::string OutputFilePathAndName = "H:/Heart_backup/Data/Yale_CT/P1940650/phase0_mean";

    SaveScalarDenseMatrixAsJsonDataFile(Mean, OutputFilePathAndName);
}

}//end of namespace


#endif