A=rand(10,100);
WriteDenseMatrixAsJsonDataFile(A, 'C:/Research/MDK/MDK_Build/Test/Test_LinearAlgebra/Test_Eigen/TestData/A.json');
%%
pinvA1=pinv(A);
pinvA2=ReadDenseMatrixFromJsonDataFile('C:/Research/MDK/MDK_Build/Test/Test_LinearAlgebra/Test_Eigen/TestData/pinvA.json');
pinvError=mean(abs(pinvA1(:)-pinvA2(:)));
%%
A=rand(100,100);
WriteDenseMatrixAsJsonDataFile(A, 'C:/Research/MDK/MDK_Build/Test/Test_LinearAlgebra/Test_Eigen/TestData/A.json');
%%
invA1=inv(A);
invA2=ReadDenseMatrixFromJsonDataFile('C:/Research/MDK/MDK_Build/Test/Test_LinearAlgebra/Test_Eigen/TestData/invA.json');
invError=mean(abs(invA1(:)-invA2(:)));
%%
A=rand(100,100);
A=A+A';
WriteDenseMatrixAsJsonDataFile(A, 'C:/Research/MDK/MDK_Build/Test/Test_LinearAlgebra/Test_Eigen/TestData/A.json');
%%
[eigV1, eigS1]=eig(A);
eigS1=diag(eigS1);
%%
eigS2=ReadDenseMatrixFromJsonDataFile('C:/Research/MDK/MDK_Build/Test/Test_LinearAlgebra/Test_Eigen/TestData/EigenA_S.json');
eigV2=ReadDenseMatrixFromJsonDataFile('C:/Research/MDK/MDK_Build/Test/Test_LinearAlgebra/Test_Eigen/TestData/EigenA_V.json');
eigSError=mean(abs(eigS1(:)-eigS2(:)));
eigVError=mean(abs(abs(eigV1(:))-abs(eigV2(:))));
%%
A=rand(100,100);
%A=[A;
WriteDenseMatrixAsJsonDataFile(A, 'C:/Research/MDK/MDK_Build/Test/Test_LinearAlgebra/Test_Eigen/TestData/A.json');
detA=det(A);