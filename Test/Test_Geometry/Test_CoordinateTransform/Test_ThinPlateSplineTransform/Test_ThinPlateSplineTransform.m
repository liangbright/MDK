clear
MatlabQtVTKDir='C:\Research\MatlabQtVTK\MatlabQtVTK_Code\matlab';
addpath(genpath(MatlabQtVTKDir))
%% 2D
N=10;
Source=rand(2,N);
a=[1 2 3];
b=[4 5 6];
F=ones(1,N)/N;
G=F;
Target=[];
for k=1:N
    x=Source(1,k);
    y=Source(2,k);
    
    x_new=a(1)+a(2)*x + a(3)*y + sum(F.*sqrt((x-Source(1,:)).^2+(y-Source(2,:)).^2));
    y_new=b(1)+b(2)*x + b(3)*y + sum(G.*sqrt((x-Source(1,:)).^2+(y-Source(2,:)).^2));
    
    Target(:,end+1)=[x_new, y_new];    
end
%Tareget=Tareget+ 0.01*rand(size(Tareget));
%
FilePath='C:/Research/MDK/MDK_Build/Test/Test_Geometry/Test_CoordinateTransform/Test_ThinPlateSplineTransform/TestData/';
WriteDenseMatrixAsJsonDataFile(Source, [FilePath 'Source2D.json']);
WriteDenseMatrixAsJsonDataFile(Target, [FilePath 'Target2D.json']);
%% 3D
N=100;
Source=rand(3,N);
a=[1 1 2 3];
b=[2 1 3 2];
c=[3 2 3 1];
F=ones(1,N)/N;
G=F;
H=F;
Target=[];
for k=1:N
    x=Source(1,k);
    y=Source(2,k);
    z=Source(3,k);
    
   % z=0;
    
    x_new=a(1)+a(2)*x + a(3)*y + a(4)*z + sum(F.*sqrt((x-Source(1,:)).^2+(y-Source(2,:)).^2+(z-Source(3,:)).^2));
    y_new=b(1)+b(2)*x + b(3)*y + b(4)*z + sum(G.*sqrt((x-Source(1,:)).^2+(y-Source(2,:)).^2+(z-Source(3,:)).^2));
    z_new=c(1)+c(2)*x + c(3)*y + c(4)*z + sum(H.*sqrt((x-Source(1,:)).^2+(y-Source(2,:)).^2+(z-Source(3,:)).^2));
    
    %z_new=0;

    Target(:,end+1)=[x_new, y_new, z_new];    
end
%Tareget=Tareget+ 0.01*rand(size(Tareget));
%
FilePath='C:/Research/MDK/MDK_Build/Test/Test_Geometry/Test_CoordinateTransform/Test_ThinPlateSplineTransform/TestData/';
WriteDenseMatrixAsJsonDataFile(Source, [FilePath 'Source.json']);
WriteDenseMatrixAsJsonDataFile(Target, [FilePath 'Target.json']);
%%
a=[1 6 0 0 ];
b=[2 6 0 0];
c=[3 6 0 0];
TargetA=[];
for k=1:N
    x=Source(1,k);
    y=Source(1,k);
    z=Source(1,k);
    x_new=a(1)+a(2)*x + a(3)*y + a(4)*z;% + sum(F.*sqrt((x-Source(1,:)).^2+(y-Source(1,:)).^2+(z-Source(3,:)).^2));
    y_new=b(1)+b(2)*x + b(3)*y + b(4)*z;% + sum(G.*sqrt((x-Source(1,:)).^2+(y-Source(1,:)).^2+(z-Source(3,:)).^2));
    z_new=c(1)+c(2)*x + c(3)*y + c(4)*z;% + sum(H.*sqrt((x-Source(1,:)).^2+(y-Source(1,:)).^2+(z-Source(3,:)).^2));
    TargetA(:,end+1)=[x_new, y_new, z_new];    
end