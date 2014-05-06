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
    
    tempI=Iobj(:,:,index) + NoiseStd*randn(20, 20);
    % convert I to zero mean and unit variance
    M=mean(tempI(:));
    tempI = tempI-M; 
    L2Norm= sqrt(sum(tempI(:).^2));    
    tempI=tempI/L2Norm;
    
    I(:,:,k)=tempI;
end

I_ALL=zeros(50, 150);
for k=1:10

    index=(k-1)*10 + 1;
    
    temp=I(:,:,index);    
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

% convert I to Matrix FeatureData
FeatureData=zeros(400, 100);
for k=1:100
    temp=I(:,:,k);
    FeatureData(:,k)=temp(:);
end
% save FeatureData
FilePath='C:/Research/MDK_Build/Test/Test_FeatureCoding/Test_KNNAverageOnlineDictionaryBuilder/Debug/';
WriteDenseMatrixAsJsonDataFile(FeatureData, [FilePath 'GaussianObjectImage.json'])
%%
R = corrcoef(FeatureData);
imtool(R)

%% read VectorSimilarityMatrix
VectorSimilarityMatrix= ReadDenseMatrixFromJsonDataFile([FilePath 'VectorSimilarityMatrix.json']);
imtool(VectorSimilarityMatrix)
%% read BasisMatrix_init
BasisMatrix_init= ReadDenseMatrixFromJsonDataFile([FilePath 'GaussianObjectImage_BasisMatrix_init.json']);
%
Basis_init_ALL=zeros(50, 150);
for k=1:10

    temp=BasisMatrix_init(:,k);    
    temp=reshape(temp, [20, 20]);

    if k == 1
        Basis_init_ALL(1:20, 1:20)=temp;
    elseif k <= 5    
        Index_s = (k-1)*30;
        Basis_init_ALL(1:20, Index_s:Index_s+19)=temp;
    elseif k==6
        Basis_init_ALL(31:50, 1:20)=temp;
    else        
        Index_s = (k-6)*30;
        Basis_init_ALL(31:50, Index_s:Index_s+19)=temp;
    end    
end

imtool(Basis_init_ALL)
%% read BasisMatrix
BasisMatrix= ReadDenseMatrixFromJsonDataFile([FilePath 'GaussianObjectImage_BasisMatrix.json']);
%
Basis_ALL=zeros(50, 150);
for k=1:10

    temp=BasisMatrix(:,k);    
    temp=reshape(temp, [20, 20]);

    if k == 1
        Basis_ALL(1:20, 1:20)=temp;
    elseif k <= 5    
        Index_s = (k-1)*30;
        Basis_ALL(1:20, Index_s:Index_s+19)=temp;
    elseif k==6
        Basis_ALL(31:50, 1:20)=temp;
    else        
        Index_s = (k-6)*30;
        Basis_ALL(31:50, Index_s:Index_s+19)=temp;
    end    
end

imtool(Basis_ALL)
%% Matlab Kmeans
[Label, Center] = kmeans(FeatureData', 10);
Center=Center';
Center_ALL=zeros(50, 150);
for k=1:10

    temp=Center(:,k);    
    temp=reshape(temp, [20, 20]);

    if k == 1
        Center_ALL(1:20, 1:20)=temp;
    elseif k <= 5    
        Index_s = (k-1)*30;
        Center_ALL(1:20, Index_s:Index_s+19)=temp;
    elseif k==6
        Center_ALL(31:50, 1:20)=temp;
    else        
        Index_s = (k-6)*30;
        Center_ALL(31:50, Index_s:Index_s+19)=temp;
    end    
end

imtool(Center_ALL)
%% smooth FeatureData

SmoothedFeatureData=zeros(400, 10);

for k=1:10
   
    IndexList= (k-1)*10 +1 : (k-1)*10 + 10;
    
    for n=IndexList
    
        SmoothedFeatureData(:,k)= SmoothedFeatureData(:,k) + FeatureData(:,n);
    end
end

SmoothedFeatureData = SmoothedFeatureData/10;

Is_ALL=zeros(50, 150);
for k=1:10
    
    temp=SmoothedFeatureData(:,k);    
    temp=reshape(temp, [20, 20]);

    if k == 1
        Is_ALL(1:20, 1:20)=temp;
    elseif k <= 5    
        Index_s = (k-1)*30;
        Is_ALL(1:20, Index_s:Index_s+19)=temp;
    elseif k==6
        Is_ALL(31:50, 1:20)=temp;
    else        
        Index_s = (k-6)*30;
        Is_ALL(31:50, Index_s:Index_s+19)=temp;
    end    
end

imtool(Is_ALL)
