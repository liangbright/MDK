%% read VectorSimilarityMatrix
VectorSimilarityMatrix= ReadDenseMatrixFromJsonDataFile([FilePath 'VectorSimilarityMatrix.json']);

%% read VectorProbabilityList
VectorProbabilityList= ReadDenseMatrixFromJsonDataFile([FilePath 'VectorProbabilityList.json']);
%
Hv=sum(-VectorProbabilityList.*log2(VectorProbabilityList))/ log2(length(VectorProbabilityList))

figure; plot(1:length(VectorProbabilityList), VectorProbabilityList)

%% read SimilarityMatrix
SimilarityMatrix= ReadDenseMatrixFromJsonDataFile([FilePath 'NoisyImage_SimilarityMatrix.json']);
figure; hist(SimilarityMatrix(:), 100)
%% read SimilarityMatrix_init
SimilarityMatrix= ReadDenseMatrixFromJsonDataFile([FilePath 'NoisyImage_SimilarityMatrix_init.json']);
figure; hist(SimilarityMatrix(:), 100)
%%
prob=sum(VectorSimilarityMatrix, 2);
prob=prob/sum(prob);

[Num, ~]=size(VectorSimilarityMatrix);

H=sum(-prob.*log2(prob))/ log2(Num)
Sav=mean(VectorSimilarityMatrix(:))
%%
figure; plot(1:length(prob), prob)
%
figure; hist(VectorSimilarityMatrix(:), 100)
%%
save('VectorSimilarityMatrix_10000.mat', 'VectorSimilarityMatrix')
