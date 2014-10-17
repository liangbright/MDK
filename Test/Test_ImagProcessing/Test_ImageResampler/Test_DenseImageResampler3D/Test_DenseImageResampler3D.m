FilePath='H:/AorticValveData/2014_7_25/P2115937/phase0/';
[Volume, GeometryInfo] = ReadImageFromDICOMSeries(FilePath);
%% read ResampledImage
[ResampledImage, ResampledImageInfo]=ReadImageFromJsonDataFile('H:/AorticValveData/2014_7_25/P2115937/Detection/ResampledImage.json');
%% show 3D ResampledImage
FigureHandle= vtkfigure();
vtkshowvolume(FigureHandle, 'Volume', Volume, GeometryInfo.Origin, GeometryInfo.Spacing, [100, 1000]);
vtkshowvolume(FigureHandle, 'ResampledImageInfo', ResampledImage, ResampledImageInfo.Origin, ResampledImageInfo.Spacing, [200, 800]);
vtkshowaxes(FigureHandle)
%%
vtkshowvolume(FigureHandle, 'Volume', Volume, GeometryInfo.Origin, GeometryInfo.Spacing, [100, 1000]);

%%
implay(Volume)
%%
implay(ResampledImage)
%%
imtool(ResampledImage(:,:,end))

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
imtool(ResampledImage(:,:,1))
imtool(ResampledImage(:,:,2))
%%
implay(ResampledImage)
%%
implay(Volume)