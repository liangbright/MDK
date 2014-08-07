I= 100 + 10*randn(100,100);

FilePath='C:/Research/MDK_Build/Test/Test_ImageFilter/Test_ScalarImageFilter/Debug/';

WriteImageAsJsonDataFile(I, [], [FilePath 'TestImage.json'])
%%
Im=ReadImageFromJsonDataFile([FilePath 'FilteredTestImage.json']);
imtool(I);
imtool(Im)
%%
h = fspecial('gaussian', 9, 3);

Im_matlab = imfilter(I, h, 'replicate');

imtool(Im_matlab)