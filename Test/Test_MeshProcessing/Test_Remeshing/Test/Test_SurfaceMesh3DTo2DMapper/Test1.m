UVTableOfBoundary=zeros(2, 4*17-4);
for k=1:64
    theta=(k-1)*2*pi/64;
    u=10*cos(theta);
    v=10*sin(theta);
    UVTableOfBoundary(:,k)=[u,v];
end
File='C:/Research/Mesh/Remeshing/Build/Test_SurfaceMesh3DTo2DMapper/TestData/UVTableOfBoundary.json';
WriteDenseMatrixAsJsonDataFile(UVTableOfBoundary,File);
%%
Leaflet=ReadPolygonMeshFromJsonDataFile('C:/Research/Mesh/Remeshing/Build/Test_SurfaceMesh3DTo2DMapper/TestData/Leaflet.json.Surface.json');
%%
Curve_LeftCommissure_to_Tip = [0, 34, 35, 36, 37, 38, 39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 3] +1;
Curve_LeftCommissure_to_Hinge = [0, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18, 2] +1;
Curve_RightCommissure_to_Tip = [1, 49, 50, 51, 52, 53, 54, 55, 56, 57, 58, 59, 60, 61, 62, 63, 3] +1;
Curve_RightCommissure_to_Hinge = [1, 19, 20, 21, 22, 23, 24, 25, 26, 27, 28, 29, 30, 31, 32, 33, 2] +1;
Curve_Tip_to_Hinge = [3, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179, 180, 181, 182, 183, 2] +1;
 
Bounary=[Curve_LeftCommissure_to_Tip, Curve_RightCommissure_to_Tip(end-1:-1:1), ...
        Curve_RightCommissure_to_Hinge(2:end), Curve_LeftCommissure_to_Hinge(end-1:-1:2)];
%%
UVTableOfBoundary=Leaflet.Point(:,Bounary);
UVTableOfBoundary=UVTableOfBoundary(2:3,:);
figure; plot(UVTableOfBoundary(1,:), UVTableOfBoundary(2,:))
%%
File='C:/Research/Mesh/Remeshing/Build/Test_SurfaceMesh3DTo2DMapper/TestData/UVTableOfBoundary.json';
WriteDenseMatrixAsJsonDataFile(UVTableOfBoundary,File);
%%
UVTableOfBoundary=zeros(2, 4*17-4);
for k=1:33
    u=-10 + (k-1)*20/32;
    v=0;
    UVTableOfBoundary(:,k)=[u,v];    
end
for k=34:64
    theta=-(k+1-34)*pi/(64-34+2);
    u=10*cos(theta);
    v=10*sin(theta);
    UVTableOfBoundary(:,k)=[u,v];
end
%UVTableOfBoundary(2, end-15)=-10.5;
figure; plot(UVTableOfBoundary(1,:), UVTableOfBoundary(2,:), '-o'); axis equal
%%
File='C:/Research/Mesh/Remeshing/Build/Test_SurfaceMesh3DTo2DMapper/TestData/UVTableOfBoundary.json';
WriteDenseMatrixAsJsonDataFile(UVTableOfBoundary,File);