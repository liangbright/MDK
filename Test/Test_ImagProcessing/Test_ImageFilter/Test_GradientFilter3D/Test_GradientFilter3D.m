FilePath='G:/AorticValveData/2014_7_25/P2115937/phase0/';
[Volume, GeometryInfo] = ReadDenseImage3DFromDICOMSeries(FilePath);
%% read GradientMag
Test_Path = 'C:/Research/MDK/MDK_Build/Test/Test_ImageProcessing/Test_ImageFilter/Test_GradientFilter3D/TestData/';
[GradientMagImage, GradientMagImageInfo]=ReadDenseImage3DFromJsonDataFile([Test_Path 'GradientMagImage.json']);
%% show 3D ResampledImage
FigureHandle= vtkfigure();
%vtkshowvolume(FigureHandle, 'Volume', Volume, GeometryInfo.Origin, GeometryInfo.Spacing, [100, 1000]);
vtkshowvolume(FigureHandle, 'GradientMagImageInfo', GradientMagImage, GradientMagImageInfo.Origin, GradientMagImageInfo.Spacing, [200, 800]);
vtkshowaxes(FigureHandle)
%%
vtkshowvolume(FigureHandle, 'Volume', Volume, GeometryInfo.Origin, GeometryInfo.Spacing, [100, 1000]);

%%
implay(Volume)
%%
implay(GradientMagImage)
%%
imtool(GradientMagImage(:,:,end))

%% read BoundaryImage
[BoundaryImage, BoundaryImageInfo]=ReadImageFromJsonDataFile('H:/AorticValveData/2014_7_25/P2115937/Detection/BoundaryImage.json');
%%
FigureHandle= vtkfigure();
vtkshowvolume(FigureHandle, 'BoundaryImage', BoundaryImage, BoundaryImageInfo.Origin, BoundaryImageInfo.Spacing, [0.5, 1]);
vtkshowaxes(FigureHandle)
%%
implay(BoundaryImage)
%%
imtool(BoundaryImage(:,:,end))
%%
imtool(GradientMagImage(:,:,1))
imtool(GradientMagImage(:,:,2))
%%
implay(GradientMagImage)
%%
implay(Volume)