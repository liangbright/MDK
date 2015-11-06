%%
A=zeros(length(candidateconstraint(1,:)), length(candidatescore));
B=ones(1127,1);
contypes=[];
for k=1:1127
    constraint=candidateconstraint(k,:);
    constraint=constraint(~isnan(constraint));
    for n=1:length(constraint)
        A(k, constraint(n)+1)=1;
    end
    contypes=[contypes '<'];
end
A=sparse(A);
C=candidatescore;
%%
[X, val, exitflag, output]=MIPSolver(A, B, C, contypes, []);
%%
WriteDenseMatrixAsJsonDataFile(X, 'C:\Research\Mesh\Remeshing\Build\Test_SurfaceRemesher1\TestData\FlagList.json');