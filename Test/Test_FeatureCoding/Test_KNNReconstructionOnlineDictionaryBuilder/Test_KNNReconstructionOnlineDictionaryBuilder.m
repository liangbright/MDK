%% Test_SimpleCase ---------------------------------------------------
DataSample=zeros(1, 1010);
DataSample(1:1000) = randn(1, 1000)+10;
DataSample(1001:1010) = randn(1,10)+20;

FilePathAndName='C:/Research/MDK_Build/Test/Test_FeatureCoding/Test_KNNAverageOnlineDictionaryBuilder/Debug/DataSample.json';
WriteDenseMatrixAsJsonDataFile(DataSample, FilePathAndName);
%%
FilePathAndName='C:/Research/MDK_Build/Test/Test_FeatureCoding/Test_KNNAverageOnlineDictionaryBuilder/Debug/BasisMatrix.json';
BasisMatrix= ReadDenseMatrixFromJsonDataFile(FilePathAndName);
%%
FilePathAndName='C:/Research/MDK_Build/Test/Test_FeatureCoding/Test_KNNAverageOnlineDictionaryBuilder/Debug/VectorSimilarityMatrix.json';
VectorSimilarityMatrix= ReadDenseMatrixFromJsonDataFile(FilePathAndName);
imtool(VectorSimilarityMatrix)
%%
FilePathAndName='C:/Research/MDK_Build/Test/Test_FeatureCoding/Test_KNNAverageOnlineDictionaryBuilder/Debug/L1DistanceStd.json';
L1DistanceStd= ReadDenseMatrixFromJsonDataFile(FilePathAndName);

%%
figure; hist(DataSample, 1000)
figure; hist(BasisMatrix, 1000)

%%---------------------------------------------------------------------

