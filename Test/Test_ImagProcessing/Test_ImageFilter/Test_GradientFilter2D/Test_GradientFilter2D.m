[ImageA, ImageInfoA]=ReadDenseImage2DFromJsonDataFile('C:/Research/MDK/MDK_Build/Test/Test_ImageProcessing/Test_ImageFilter/Test_GradientFilter2D/TestData/GradientMagImage.json');
%%
imtool(ImageA')
%%
figure; hist(ImageA(:), 100)