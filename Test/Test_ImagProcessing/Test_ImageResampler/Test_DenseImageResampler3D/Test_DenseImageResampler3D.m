FilePath='H:/AorticValveData/2014_7_25/P2115937/phase0/';
[Volume, GeometryInfo] = ReadImageFromDICOMSeries(FilePath);
%% read MembershipImage
[ResampledImage, ResampledImageInfo]=ReadImageFromJsonDataFile('H:/AorticValveData/2014_7_25/P2115937/Detection/ResampledImage.json');
%%
FigureHandle= vtkfigure();
%% show 3D MembershipImage
vtkshowvolume(FigureHandle, 'Volume', Volume, GeometryInfo.Origin, GeometryInfo.Spacing, [100, 1000]);
vtkshowvolume(FigureHandle, 'ResampledImageInfo', ResampledImage, ResampledImageInfo.Origin, ResampledImageInfo.Spacing, [200, 1000]);
vtkshowaxes(FigureHandle)
%%
implay(Volume)