FilePath='G:/AorticValveData/2014_7_25/P2115937/phase0/';
[Volume, GeometryInfo] = ReadDenseImage3DFromDICOMSeries(FilePath);
%% read ResampledImage
Test_Path = 'C:/Research/MDK/MDK_Build/Test/Test_ImageProcessing/Test_ImageResampler/Test_DenseImageResampler2D/TestData/';
[ResampledImage, ResampledImageInfo]=ReadDenseImage2DFromJsonDataFile([Test_Path 'ResampledImage.json']);
% show ResampledImage
imtool(ResampledImage')