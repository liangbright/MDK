%% Test_2D_Image_GaussianObject

FilePath='C:/Research/MDK_Build/Test/Test_FeatureCoding/Test_KNNReconstructionSparseEncoder/Debug/';


%% save FeatureData
VectorLength=100;
DataNumber=1000;

FeatureData=100*randn(VectorLength,DataNumber);

WriteDenseMatrixAsJsonDataFile(FeatureData, [FilePath 'FeatureData.json'])
%%
KNNBasisMatrix = 100*randn(VectorLength, 5);
WriteDenseMatrixAsJsonDataFile(KNNBasisMatrix, [FilePath 'KNNBasisMatrix.json'])
%% read KNNCodeMatrix
KNNCodeMatrix= ReadDenseMatrixFromJsonDataFile([FilePath 'KNNCodeMatrix.json']);

%% Compute KNNCodeMatrix_Matlab
KNNCodeMatrix_Matlab=zeros(5, DataNumber);

CodeNonnegative=true;
CodeSumToOne=true;

Aeq=[1, 1, 1, 1, 1];
beq=1;

lb=zeros(5,1);

for k=1:DataNumber

    d=FeatureData(:,k);
    
    if CodeNonnegative == false && CodeSumToOne == false
        
        KNNCodeMatrix_Matlab(:,k) = lsqlin(KNNBasisMatrix, d);
    
    elseif CodeNonnegative == true && CodeSumToOne == false

        KNNCodeMatrix_Matlab(:,k) = lsqlin(KNNBasisMatrix, d, [], [], [], [], lb, []);

    elseif  CodeNonnegative == true && CodeSumToOne == true
    
        KNNCodeMatrix_Matlab(:,k) = lsqlin(KNNBasisMatrix, d, [], [], Aeq, beq, lb, []);
        
    else % CodeNonnegative is false, CodeSumToOne is true
    
        KNNCodeMatrix_Matlab(:,k) = lsqlin(KNNBasisMatrix, d, [], [], Aeq, beq);
    end
end
%%
CodeDiff = KNNCodeMatrix(:)-KNNCodeMatrix_Matlab(:);
figure; hist(CodeDiff, 100);
%%
