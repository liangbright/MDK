clear
MatlabQtVTKDir='C:\Research\MatlabQtVTK\MatlabQtVTK_Code\matlab';
addpath(genpath(MatlabQtVTKDir))
%%
TestDataPath='C:/Research/MDK/MDK_Build/Test/Test_ShapeModeling/Test_ShapeDictionaryBuilder/TestData/';
%%
SimilarityTable=ReadDenseMatrixFromJsonDataFile([TestDataPath 'SimilarityTable.json']);
%%
BasisSimilarity_init=ReadDenseMatrixFromJsonDataFile([TestDataPath 'BasisSimilarity_init.json']);
BasisSimilarity_select=ReadDenseMatrixFromJsonDataFile([TestDataPath 'BasisSimilarity_select.json']);
