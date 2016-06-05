%%
FilePath='C:/Research/MDK/MDK_Build/Test/Test_ImageProcessing/Test_ImageFilter/Test_ConvolutionImageFilter3D/TestData/';
%%
I0=ReadDenseImage3DFromJsonDataFile([FilePath 'test_GaussianFilter3D_InputImage.json']);
%%
I1=ReadDenseImage3DFromJsonDataFile([FilePath 'test_GaussianFilter3D_OutputImage.json']);
%%
I2=ReadDenseImage3DFromJsonDataFile([FilePath 'test_LoGFilter3D_OutputImage.json']);
%%
implay(-I2)
%%
for k=84:-1:0
    Info=dicominfo(['G:/AorticValveData/2014_7_25/P2115937/phase0/i00' num2str(k) '_0000b.dcm']);    
    I=I2(:,:,85-k);
    dicomwrite(int16(I), [FilePath num2str(85-k) '.dcm'], info);
end
%%
imtool(I0(:,:,1))
%%
imtool(I1(:,:,1))
%%
imtool(I2(:,:,20))
%%
h = rand(23,21,21);
%%
tic
Im_matlab = imfilter(I0, h, 'replicate');
toc