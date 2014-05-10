%% Test_2D_Image_GaussianObject

FilePath='C:/Research/MDK_Build/Test/Test_FeatureCoding/Test_KNNReconstructionOnlineDictionaryBuilder/Debug/';

I=double(imread([FilePath 'Lena.tif']));

In = I+ 10*randn(size(I));
%%
PatchData = im2col(I, [7 7], 'sliding');
PatchData_ALL = displayPatches(PatchData);
imtool(PatchData_ALL)
%%
NoisyPatchData = im2col(In, [7 7], 'sliding');

FeatureData=NoisyPatchData;

[~, Num]=size(FeatureData);

MeanOfFeatureData=zeros(1, Num);

for k=1:Num
    temp=FeatureData(:,k);
    
    MeanOfFeatureData(k)=mean(temp);
    
    temp=temp-MeanOfFeatureData(k);
    
    L2Norm = sqrt(sum(temp.^2));

    temp=temp/L2Norm;
    
    FeatureData(:,k)=temp;
end
%%
Similarity_1_2=exp(-0.5*sum((FeatureData(:,1)-FeatureData(:,2)).^2));
%%
NoisyPatchData_ALL = displayPatches(NoisyPatchData);
imtool(NoisyPatchData_ALL)

%% save FeatureData

WriteDenseMatrixAsJsonDataFile(FeatureData, [FilePath 'NoisyImagePatch.json'])

%% read VectorSimilarityMatrix
VectorSimilarityMatrix= ReadDenseMatrixFromJsonDataFile([FilePath 'VectorSimilarityMatrix.json']);
imtool(VectorSimilarityMatrix)
%% read BasisMatrix_init
BasisMatrix_init= ReadDenseMatrixFromJsonDataFile([FilePath 'NoisyImage_BasisMatrix_init.json']);
%
BasisMatrix_init_patch = displayPatches(BasisMatrix_init);
imtool(BasisMatrix_init_patch)
%% read BasisExperience_init
BasisExperience_init= ReadDenseMatrixFromJsonDataFile([FilePath 'NoisyImage_BasisExperience_init.json']);

figure; plot(BasisExperience_init, '-o'); grid on
%% read BasisMatrix
BasisMatrix= ReadDenseMatrixFromJsonDataFile([FilePath 'NoisyImage_BasisMatrix.json']);
%
BasisMatrix_patch = displayPatches(BasisMatrix);
imtool(BasisMatrix_patch)
%% read BasisExperience
BasisExperience= ReadDenseMatrixFromJsonDataFile([FilePath 'NoisyImage_BasisExperience.json']);
%
figure; plot(BasisExperience, '-o'); grid on

%% read reconstructed data
ReconstructedFeatureData = ReadDenseMatrixFromJsonDataFile([FilePath 'NoisyImage_ReconstructedPatch.json']);
%
[~, Num]=size(ReconstructedFeatureData);

for k=1:Num
    ReconstructedFeatureData(:,k) = ReconstructedFeatureData(:,k)+MeanOfFeatureData(k);
end

ReconstructedPatch = displayPatches(ReconstructedFeatureData);
imtool(ReconstructedPatch)
%% use spams

param.K=256;  % learns a dictionary with 100 elements
param.lambda=0.15;
param.numThreads=-1; % number of threads
param.batchsize=400;
param.verbose=false;

param.iter=1000; 

D=mexTrainDL(FeatureData, param);
%%
D_Patch = displayPatches(D);
imtool(D_Patch)
%%
alpha=mexLasso(FeatureData,D,param);
alpha=full(alpha);
ReconstructedFeatureData=D*alpha;
%%
[~, Num]=size(ReconstructedFeatureData);

for k=1:Num
    ReconstructedFeatureData(:,k) = ReconstructedFeatureData(:,k)+MeanOfFeatureData(k);
end

ReconstructedPatch = displayPatches(ReconstructedFeatureData);
imtool(ReconstructedPatch)


