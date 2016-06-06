%%
FilePath='C:/Research/MDK/MDK_Build/Test/Test_ImageProcessing/Test_ImageFilter/Test_ConvolutionImageFilter3D/TestData/';
%%
I0=ReadDenseImage3DFromJsonDataFile([FilePath 'test_DiscreteGaussianFilter3D_InputImage.json']);
%%
I1=ReadDenseImage3DFromJsonDataFile([FilePath 'test_DiscreteGaussianFilter3D_OutputImage.json']);
%%
I2=ReadDenseImage3DFromJsonDataFile([FilePath 'test_DiscreteLoGFilter3D_OutputImage.json']);
%%
I3=ReadDenseImage3DFromJsonDataFile([FilePath 'test_GenericGaussianFilter3D_InputImage.json']);
%%
I4=ReadDenseImage3DFromJsonDataFile([FilePath 'test_GenericGaussianFilter3D_OutputImage.json']);
%%
I5=ReadDenseImage3DFromJsonDataFile([FilePath 'test_GenericLoGFilter3D_OutputImage.json']);
%%
implay(-I2)
%%
for k=84:-1:0
    Info=dicominfo(['G:/AorticValveData/2014_7_25/P2115937/phase0/i00' num2str(k) '_0000b.dcm']);    
    I=-I2(:,:,85-k);
    dicomwrite(int16(I), [FilePath num2str(85-k) '.dcm'], Info);
end
%%
imtool(I0(:,:,1))
%%
imtool(I1(:,:,1))
%%
imtool(-I2(:,:,55))
%%
h = rand(23,21,21);
%%
tic
Im_matlab = imfilter(I0, h, 'replicate');
toc