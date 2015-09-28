clear
MatlabQtVTKDir='C:\Research\MatlabQtVTK\MatlabQtVTK_Code\matlab';
addpath(genpath(MatlabQtVTKDir))
%% 2D
Source=rand(2,1000);

X=0.5; 
cosX=cos(X);
sinX=sin(X);

R=[cosX, -sinX
   sinX cosX];
    
s=1.5;
T=10;
%Source=bsxfun(@minus, Source, mean(Source, 2));
Target=s*R*Source +T;   

%Tareget=Tareget+ 0.01*rand(size(Tareget));
%
FilePath='C:/Research/MDK/MDK_Build/Test/Test_Geometry/Test_CoordinateTransform/Test_RotationTransform/TestData/';
WriteDenseMatrixAsJsonDataFile(Source, [FilePath 'Source2D.json']);
WriteDenseMatrixAsJsonDataFile(Target, [FilePath 'Target2D.json']);
%%
Source=rand(3,1000);

X=0.5; 
Y=1.5;
Z=2;
cosX=cos(X);
sinX=sin(X);
cosY=cos(Y);
sinY=sin(Y);
cosZ=cos(Z);
sinZ=sin(Z);
Rx=[1 0 0
    0 cosX, -sinX
    0 sinX cosX];
Ry=[cosY 0 sinY
    0  1 0
    -sinY 0 cosY];
Rz=[cosZ, -sinZ, 0
    sinZ, cosZ, 0
    0 0 1];
R=Rz*Ry*Rx;
    
s=1.5;
T=10;
%Source=bsxfun(@minus, Source, mean(Source, 2));
Target=s*R*Source +T;   

%Tareget=Tareget+ 0.01*rand(size(Tareget));
%
FilePath='C:/Research/MDK/MDK_Build/Test/Test_Geometry/Test_CoordinateTransform/Test_RotationTransform/TestData/';
WriteDenseMatrixAsJsonDataFile(Source, [FilePath 'Source.json']);
WriteDenseMatrixAsJsonDataFile(Target, [FilePath 'Target.json']);