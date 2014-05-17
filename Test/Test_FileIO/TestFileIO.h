#ifndef __TestFileIO_h
#define __TestFileIO_h

#include "mdkFileIO.h"
#include "mdkImage3D.h"
#include "mdkDenseMatrix.h"

namespace mdk
{

void Test_ImageIO()
{

    DenseMatrix<double> A = {};

    std::string FilePath;
    
    //FilePath = "E:/HeartData/P1943091-im_6-phase10-close-leaflet/im_6/phase0";

    FilePath = "H:/Heart_backup/Data/Yale_CT/P1940650/phase0";

    //FilePath = "C:/Research/Test_ITK/SpineData/1";


    auto InputImage = Load3DScalarImageFromDICOMSeries<double>(FilePath);

    std::string OutputFilePathAndName = "H:/Heart_backup/Data/Yale_CT/P1940650/phase0_temp";

    Image3D<double>& OutputImage = InputImage;

    Save3DScalarImageAsJsonDataFile(OutputImage, OutputFilePathAndName);
}


void Test_MatrixIO()
{
    std::string FilePath = "E:/HeartData/P1943091-im_6-phase10-close-leaflet/im_6/Test_InputMatrix";

    auto InputMatrix = LoadDenseMatrixFromJsonDataFile<float>(FilePath);

    DisplayMatrix("InputMatrix", InputMatrix);

    DenseMatrix<double> OutputMatrix(10, 10);

    OutputMatrix.Fill(0);

    OutputMatrix.FillDiagonal(1);

    std::string OutputFilePathAndName ="E:/HeartData/P1943091-im_6-phase10-close-leaflet/im_6/Test_OutputMatrix";

    SaveDenseMatrixAsJsonDataFile(OutputMatrix, OutputFilePathAndName);


}


void Test_Image_Matrix_IO()
{
    std::string FilePath;

    FilePath = "H:/Heart_backup/Data/Yale_CT/P1940650/phase0/";

    auto InputImage = Load3DScalarImageFromDICOMSeries<double>(FilePath);

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

    SaveDenseMatrixAsJsonDataFile(Mean, OutputFilePathAndName);
}

}//end of namespace


#endif