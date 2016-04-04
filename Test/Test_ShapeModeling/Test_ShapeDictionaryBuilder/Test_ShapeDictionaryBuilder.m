clear
MatlabQtVTKDir='C:\Research\MatlabQtVTK\MatlabQtVTK_Code\matlab';
addpath(genpath(MatlabQtVTKDir))
%%
TestDataPath='C:/Research/MDK/MDK_Build/Test/Test_ShapeModeling/Test_ShapeDictionaryBuilder/TestData/';
%%
SimilarityTable=ReadDenseMatrixFromJsonDataFile([TestDataPath 'SimilarityTable.json']);