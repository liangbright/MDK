clear
clc
FilePath='C:/Research/MDK/MDK_Build/Test/Test_FeatureCoding/Test_KNNAverageOnlineDictionaryBuilder/TestData/ImageDenoising/';
%%
NoiseStd=10;
FeatureData = NoiseStd*randn(2048, 5000);
[VL, Num]=size(FeatureData);
for k=1:Num
    temp=FeatureData(:,k);  
    temp=temp-mean(temp);        
    L1Norm = sum(abs(temp));    
    L2Norm = sqrt(sum(temp.^2));

    temp=temp/L1Norm;
    %temp=temp/L2Norm;
    
    FeatureData(:,k)=temp;
end
% save FeatureData
WriteDenseMatrixAsJsonDataFile(FeatureData, [FilePath 'NoisyImagePatch.json'])
%% read VectorSimilarityMatrix
VectorSimilarityMatrix= ReadDenseMatrixFromJsonDataFile([FilePath 'VectorSimilarityMatrix.json']);
prob=sum(VectorSimilarityMatrix, 2);
prob=prob/sum(prob);
[Num, ~]=size(VectorSimilarityMatrix);
Entropy=sum(-prob.*log2(prob))/ log2(Num);
Sav=mean(VectorSimilarityMatrix(:));
%
figure; plot(1:length(prob), prob)
figure; hist(VectorSimilarityMatrix(:), 100)
%%
%save('VectorSimilarityMatrix_10000.mat', 'VectorSimilarityMatrix')