clear
MatlabQtVTKDir='C:\Research\MatlabQtVTK\MatlabQtVTK_Code\matlab';
addpath(genpath(MatlabQtVTKDir))
%% 2D
Source=rand(2,10);

Z=2;
cosZ=cos(Z);
sinZ=sin(Z);
R=[cosZ, -sinZ
   sinZ, cosZ];

T=10*[rand; rand];

Target=R*Source;   
Target=bsxfun(@plus, Target, T);
%Tareget=Tareget+ 0.01*rand(size(Tareget));
%
FilePath='C:/Research/MDK/MDK_Build/Test/Test_Geometry/Test_CoordinateTransform/Test_RigidTransform/TestData/';
WriteDenseMatrixAsJsonDataFile(Source, [FilePath 'Source2D.json']);
WriteDenseMatrixAsJsonDataFile(Target, [FilePath 'Target2D.json']);
%%
Source=rand(3,10);

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
    
T=10*[rand; rand; rand];

Target=R*Source;   
Target=bsxfun(@plus, Target, T);
%Tareget=Tareget+ 0.01*rand(size(Tareget));
%
FilePath='C:/Research/MDK/MDK_Build/Test/Test_Geometry/Test_CoordinateTransform/Test_RigidTransform/TestData/';
WriteDenseMatrixAsJsonDataFile(Source, [FilePath 'Source3D.json']);
WriteDenseMatrixAsJsonDataFile(Target, [FilePath 'Target3D.json']);