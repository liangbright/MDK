clear
MatlabQtVTKDir='C:\Research\MatlabQtVTK\MatlabQtVTK_Code\matlab';
addpath(genpath(MatlabQtVTKDir))
%%
TestDataPath='C:/Research/MDK/MDK_Build/Test/Test_ShapeModeling/Test_DistanceMinimizationBasedShapeAligner/TestData/';
for k=0:124
    AortaMesh=ReadPolygonMeshFromVTKFile([TestDataPath num2str(k) '_AortaModel_Pimg.vtk']);
    X=rand; 
    Y=rand;
    Z=rand;
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
    
    Source=AortaMesh.Point;
    Target=R*Source;   
    Target=bsxfun(@plus, Target, T);
    AortaMesh.Point=Target;
    
    WritePolygonMeshAsVTKFile(AortaMesh,[TestDataPath num2str(k) '_AortaModel_Pimg_rand.vtk']);
end
%%
SimilarityTable=ReadDenseMatrixFromJsonDataFile([TestDataPath 'SimilarityTable.json']);