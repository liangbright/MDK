%% Test_SimpleCase ---------------------------------------------------
DataSample=zeros(1, 1010);
DataSample(1:1000) = randn(1, 1000)+10;
DataSample(1001:1010) = randn(1,10)+20;

FilePathAndName='C:/Research/MDK_Build/Test/Test_FeatureCoding/Test_KNNBasisSelectionOnlineDictionaryBuilder/Debug/DataSample.json';
WriteDenseMatrixAsJsonDataFile(DataSample, FilePathAndName);
%%
FilePathAndName='C:/Research/MDK_Build/Test/Test_FeatureCoding/Test_KNNBasisSelectionOnlineDictionaryBuilder/Debug/VectorSimilarityMatrix.json';
VectorSimilarityMatrix= ReadDenseMatrixFromJsonDataFile(FilePathAndName);
imtool(VectorSimilarityMatrix)
%%
FilePathAndName='C:/Research/MDK_Build/Test/Test_FeatureCoding/Test_KNNBasisSelectionOnlineDictionaryBuilder/Debug/BasisMatrix.json';
BasisMatrix= ReadDenseMatrixFromJsonDataFile(FilePathAndName);
%%
FilePathAndName='C:/Research/MDK_Build/Test/Test_FeatureCoding/Test_KNNBasisSelectionOnlineDictionaryBuilder/Debug/SimilarityMatrix.json';
SimilarityMatrix= ReadDenseMatrixFromJsonDataFile(FilePathAndName);
imtool(SimilarityMatrix)
%%
FilePathAndName='C:/Research/MDK_Build/Test/Test_FeatureCoding/Test_KNNBasisSelectionOnlineDictionaryBuilder/Debug/L1DistanceStd.json';
L1DistanceStd= ReadDenseMatrixFromJsonDataFile(FilePathAndName);

%%
figure; hist(DataSample, 1000)
figure; hist(BasisMatrix, 1000)
%%---------------------------------------------------------------------
%% Test_2D_Image_GaussianObject

SignalPeak=100;
NoiseStd=10;
BgIntensity=0;

Iobj=zeros(20, 20, 10);
for R = 1:10
Iobj(:,:,R)=GaussMix(10, 10, SignalPeak, R, 20, 20, 10)+BgIntensity;
end

I=zeros(20,20,100);
for k=1:100
    
    index = ceil(k/10);
    
    I(:,:,k)=Iobj(:,:,index) + NoiseStd*randn(20, 20);
end
%%
imtool(I(:,:,1))
%% %% convert I to zero mean and unit variance
for k=1:100    
    temp = I(:,:,k);
    
    M=mean(temp(:));
    S=std(temp(:));
    temp=(temp-M)/S;
    
    I(:,:,k)=temp;
end
imtool(I(:,:,1))
%% convert I to Matrix and save FeatureData
FeatureData=zeros(400, 100);
for k=1:100
    temp=I(:,:,k);
    FeatureData(:,k)=temp(:);
end
%
FilePath='C:/Research/MDK_Build/Test/Test_FeatureCoding/Test_KNNBasisSelectionOnlineDictionaryBuilder/Debug/';
WriteDenseMatrixAsJsonDataFile(FeatureData, [FilePath 'GaussianObjectImage.json'])
%%
R = corrcoef(FeatureData);
imtool(R)

%% read VectorSimilarityMatrix
VectorSimilarityMatrix= ReadDenseMatrixFromJsonDataFile([FilePath 'VectorSimilarityMatrix.json']);
imtool(VectorSimilarityMatrix)
%% read BasisMatrix
BasisMatrix= ReadDenseMatrixFromJsonDataFile([FilePath 'GaussianObjectImage_BasisMatrix.json']);
%
I_ALL=zeros(50, 150);
for k=1:10

    temp=BasisMatrix(:,k);    
    temp=reshape(temp, [20, 20]);

    if k == 1
        I_ALL(1:20, 1:20)=temp;
    elseif k <= 5    
        Index_s = (k-1)*30;
        I_ALL(1:20, Index_s:Index_s+19)=temp;
    elseif k==6
        I_ALL(31:50, 1:20)=temp;
    else        
        Index_s = (k-6)*30;
        I_ALL(31:50, Index_s:Index_s+19)=temp;
    end    
end

imtool(I_ALL)
%%

