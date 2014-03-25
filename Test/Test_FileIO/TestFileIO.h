#ifndef __TestFileIO_h
#define __TestFileIO_h

#include "mdkFileIO.h"

namespace mdk
{

template<typename T>
void DisplayMatrix(const std::string& Name, const DenseMatrix<T>& Matrix, uint32 value_std_setw = 6, uint32 precision = 4)
{
    std::cout << Name << " = " << '\n';

    for (int64 i = 0; i < Matrix.GetRowNumber(); ++i)
    {
        for (int64 j = 0; j < Matrix.GetColNumber(); ++j)
        {
            //std::cout << std::fixed << std::setprecision(precision) << Matrix(i, j) << ' ';

            std::cout << std::setw(value_std_setw + precision) << std::setprecision(precision) << Matrix(i, j) << ' ';
        }
        std::cout << '\n';
    }
}


void Test_ImageIO()
{
    std::string FilePath("E:/HeartData/P1943091-im_6-phase10-close-leaflet/im_6/phase0");

    auto InputImage = LoadGrayScaleImageFromDICOMFile<double>(FilePath);

    std::string OutputFilePathAndName("E:/HeartData/P1943091-im_6-phase10-close-leaflet/im_6/phase0_OutputImage");

    Image<double>& OutputImage = InputImage;

    SaveGrayScaleImageAsDataFile(OutputFilePathAndName, OutputImage);

    
}


void Test_MatrixIO()
{
    std::string FilePath("E:/HeartData/P1943091-im_6-phase10-close-leaflet/im_6/Test_InputMatrix");

    auto InputMatrix = LoadScalarDenseMatrixFromDataFile<double>(FilePath);

    DisplayMatrix("InputMatrix", InputMatrix);

    DenseMatrix<double >OutputMatrix(10, 10);

    OutputMatrix.Fill(0);

    OutputMatrix.FillDiangonal(1);

    std::string OutputFilePathAndName("E:/HeartData/P1943091-im_6-phase10-close-leaflet/im_6/Test_OutputMatrix");

    SaveScalarDenseMatrixAsDataFile(OutputFilePathAndName, OutputMatrix);


}

}//end of namespace


#endif