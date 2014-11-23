A=ReadDenseMatrixFromJsonDataFile('C:\Research\MDK\MDK_Build\Test\Test_CombinationalMath\Test_NChooseK\a.json');
%
B=nchoosek(1:36, 3);
%
A=A+1;
B=B';

error=sum(abs(A(:)-B(:)));

