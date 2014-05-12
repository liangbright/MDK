%% Test_2D_Image_GaussianObject

FilePath='C:/Research/MDK_Build/Test/Test_FeatureCoding/Test_KNNAverageOnlineDictionaryBuilder/Debug/';

I=double(imread([FilePath 'Lena.tif']));

NoiseStd=10;

In = I+ NoiseStd*randn(size(I));
%%
PatchData = im2col(I, [7 7], 'sliding');
PatchData_ALL = displayPatches(PatchData);
imtool(PatchData_ALL)
%%
NoisyPatchData = im2col(In, [7 7], 'sliding');

FeatureData=NoisyPatchData;

[VL, Num]=size(FeatureData);

MeanOfFeatureData=zeros(1, Num);

for k=1:Num
    temp=FeatureData(:,k);
    
    MeanOfFeatureData(k)=mean(temp);
    
    temp=temp-MeanOfFeatureData(k);
    
    %temp = wthresh(temp, 's', NoiseStd);
    %temp=temp+eps;
    
    if sum(abs(temp)<=NoiseStd) > 0.6*VL
        temp(:)=1;
    end
    
    L2Norm = sqrt(sum(temp.^2));

    temp=temp/L2Norm;
    
    FeatureData(:,k)=temp;
end
%%
Similarity_1_2=exp(-0.5*sum((FeatureData(:,1)-FeatureData(:,2)).^2));
%%
FeatureData_ALL = displayPatches(FeatureData);
imtool(FeatureData_ALL)

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
%% Matlab Kmeans : fail to converge
[Label, Center] = kmeans(FeatureData', 500);
Center=Center';
tmp = displayPatches(Center);
imtool(tmp)
%% read reconstructed data
ReconstructedFeatureData = ReadDenseMatrixFromJsonDataFile([FilePath 'NoisyImage_ReconstructedPatch.json']);
%
[~, Num]=size(ReconstructedFeatureData);

for k=1:Num
    ReconstructedFeatureData(:,k) = ReconstructedFeatureData(:,k)+MeanOfFeatureData(k);
end

tmp = displayPatches(ReconstructedFeatureData);
imtool(tmp)

