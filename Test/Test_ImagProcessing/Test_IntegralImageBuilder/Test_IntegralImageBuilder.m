TestPath = 'C:/Research/MDK/MDK_Build/Test/Test_ImageProcessing/Test_IntegralImageBuilder/TestData/';

[AverageImage1, AverageImageImageInfo1]=ReadDenseImage3DFromJsonDataFile([TestPath 'AverageImage1.json']);
%% show 3D MembershipImage
FigureHandle2= vtkfigure();
%vtkshowvolume(FigureHandle2, 'Volume', Volume, GeometryInfo.Origin, GeometryInfo.Spacing, [100, 1000]);
vtkshowvolume(FigureHandle2, 'AverageImage1', AverageImage1, AverageImageImageInfo1.Origin, AverageImageImageInfo1.Spacing, [200, 1000]);
vtkshowaxes(FigureHandle2)
%%
[AverageImage2, AverageImageImageInfo2]=ReadDenseImage3DFromJsonDataFile([TestPath 'AverageImage2.json']);
%% show 3D MembershipImage
FigureHandle2= vtkfigure();
%vtkshowvolume(FigureHandle2, 'Volume', Volume, GeometryInfo.Origin, GeometryInfo.Spacing, [100, 1000]);
vtkshowvolume(FigureHandle2, 'AverageImage2', AverageImage2, AverageImageImageInfo2.Origin, AverageImageImageInfo2.Spacing, [100, 1000]);
vtkshowaxes(FigureHandle2)