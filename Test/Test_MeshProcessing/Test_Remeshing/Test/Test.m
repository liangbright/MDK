Constraint=ReadDenseMatrixFromJsonDataFile('C:\Research\Mesh\Remeshing\Build\TestData\Constraint.json');
ScoreList=ReadDenseMatrixFromJsonDataFile('C:\Research\Mesh\Remeshing\Build\TestData\ScoreList.json');
%%
figure; plot(ScoreList, 'o')
%%
A=sparse(Constraint);
B=ones(length(ScoreList),1);
C=ScoreList;
contypes='<';
[X, val, exitflag, output]=MIPSolver(A, B, C, contypes, []);
%%
figure; plot(X, 'o')
%%
WriteDenseMatrixAsJsonDataFile(X, 'C:\Research\Mesh\Remeshing\Build\TestData\IndicatorList.json');