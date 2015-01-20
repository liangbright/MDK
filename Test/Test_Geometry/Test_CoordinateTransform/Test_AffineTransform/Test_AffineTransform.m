clear
MatlabQtVTKDir='C:\Research\MatlabQtVTK\MatlabQtVTK_Code\matlab';
addpath(genpath(MatlabQtVTKDir))
%%
Source=rand(4,10);
Source(4,:)=1;
Affine=[0.3101 0.2101 0.1101 1.101
        0.1101 0.3101 0.9101 2.101
        0.9101 0.8101 0.7101 3.101
        0    0    0    1];
    
Tareget=Affine*Source;   

Source(4,:)=[];
Tareget(4,:)=[];

%Tareget=Tareget+ 0.01*rand(size(Tareget));
%%
FilePath='C:/Research/MDK/MDK_Build/Test/Test_Geometry/Test_CoordinateTransform/Test_AffineTransform/TestData/';
WriteDenseMatrixAsJsonDataFile(Source, [FilePath 'Source.json']);
WriteDenseMatrixAsJsonDataFile(Tareget, [FilePath 'Target.json']);