%%
count=length(candidatescore);
A=zeros(count, count);
B=ones(count,1);
contypes=[];
for k=1:count
    constraint=candidateconflicttable(k,:);
    constraint=constraint(~isnan(constraint));
    for n=1:length(constraint)
        A(k, constraint(n)+1)=1;
    end
    A(k,k)=1;
end
A=sparse(A);
C=candidatescore;
contypes='<';
%%
[X, val, exitflag, output]=MIPSolver(A, B, C, contypes, []);
%%
WriteDenseMatrixAsJsonDataFile(X, 'C:\Research\Mesh\Remeshing\Build\Test_SurfaceRemesher2\TestData\FlagList.json');