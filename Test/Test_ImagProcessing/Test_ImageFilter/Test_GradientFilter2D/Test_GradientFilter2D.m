I=dicomread('C:/Research/LumbarSpine/TestData/Patient1/T2W/T2W0005.dcm');
imtool(I)
%%
[ImageA, ImageInfoA]=ReadDenseImage2DFromJsonDataFile('C:/Research/MDK/MDK_Build/Test/Test_ImageProcessing/Test_ImageFilter/Test_GradientFilter2D/TestData/GradientMagImage.json');
%%
imtool(ImageA')
%%
figure; hist(ImageA(:), 100)
%%
[ImageB, ImageInfoB]=ReadDenseImage2DFromJsonDataFile('C:/Research/MDK/MDK_Build/Test/Test_ImageProcessing/Test_ImageFilter/Test_GradientFilter2D/TestData/test_b_GradientMagImage.json');
%%
imtool(ImageB')