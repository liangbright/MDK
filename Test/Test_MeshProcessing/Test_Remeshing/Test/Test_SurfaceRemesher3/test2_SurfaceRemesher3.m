%%
clear
clc
CandidateConflictTable_TJunction=read_data('C:\Research\Mesh\Remeshing\TestData\candidate_conflict_table_TJunction.txt');
CandidateScoreList=importdata('C:\Research\Mesh\Remeshing\TestData\candidate_score.txt');
QuadCandidateIndexSetAtSmallTriangle=read_data('C:\Research\Mesh\Remeshing\TestData\QuadCandidateIndexSet_At_SmallTriangle.txt');
QuadCandidateIndexSetAtBigTriangle=read_data('C:\Research\Mesh\Remeshing\TestData\QuadCandidateIndexSet_At_BigTriangle.txt');
%%
big_triangle_count=length(QuadCandidateIndexSetAtBigTriangle);
small_triangle_count= 6*big_triangle_count;
intput_edge_count= size(CandidateConflictTable_TJunction,1);
candidate_count=length(CandidateScoreList);
contypes=[];
%
A=[];
B=[];
% constraint: full cover
for k=1:big_triangle_count  
    
    Set0=[k, QuadCandidateIndexSetAtBigTriangle{k}+1];
    Set1=[big_triangle_count+6*(k-1)+1, QuadCandidateIndexSetAtSmallTriangle{6*(k-1)+1}+1];
    Set2=[big_triangle_count+6*(k-1)+2, QuadCandidateIndexSetAtSmallTriangle{6*(k-1)+2}+1];
    Set3=[big_triangle_count+6*(k-1)+3, QuadCandidateIndexSetAtSmallTriangle{6*(k-1)+3}+1];
    Set4=[big_triangle_count+6*(k-1)+4, QuadCandidateIndexSetAtSmallTriangle{6*(k-1)+4}+1];
    Set5=[big_triangle_count+6*(k-1)+5, QuadCandidateIndexSetAtSmallTriangle{6*(k-1)+5}+1];
    Set6=[big_triangle_count+6*(k-1)+6, QuadCandidateIndexSetAtSmallTriangle{6*(k-1)+6}+1];
    
    constraint1=zeros(1, candidate_count);
    constraint1([Set0, Set1, Set4, Set6])=1;   
    constraint2=zeros(1, candidate_count);
    constraint2([Set0, Set1, Set3, Set6])=1;   
    constraint3=zeros(1, candidate_count);
    constraint3([Set0, Set1, Set3, Set5])=1;   
    constraint4=zeros(1, candidate_count);
    constraint4([Set0, Set2, Set3, Set5])=1;   
    constraint5=zeros(1, candidate_count);
    constraint5([Set0, Set2, Set4, Set5])=1;   
    constraint6=zeros(1, candidate_count);
    constraint6([Set0, Set2, Set4, Set6])=1;   
    
    A(:,end+1)=constraint1;
    A(:,end+1)=constraint2;
    A(:,end+1)=constraint3;
    A(:,end+1)=constraint4;
    A(:,end+1)=constraint5;
    A(:,end+1)=constraint6;
     
    contypes=[contypes '======'];
    B=[B ones(1,6)];
end

%TJunction
for k=1:length(CandidateConflictTable_TJunction)    
    Set=CandidateConflictTable_TJunction{k}+1;
    constraint=zeros(1, candidate_count);
    constraint(k)=1;
    constraint(Set)=1;
    A(:,end+1)=constraint;
    contypes=[contypes '<'];
    B=[B 0];
end

A=sparse(A)';
B=B';
C=CandidateScoreList(:);
%%
[X, val, exitflag, output]=MIPSolver(A, B, C, contypes, []);
FlagList=X(:);
%%
WriteDenseMatrixAsJsonDataFile(FlagList, 'C:\Research\Mesh\Remeshing\Build\Test_SurfaceRemesher3\TestData\FlagList.json');
%%
figure; plot(CandidateScoreList, 'o')
figure; plot(FlagList, 'o')
%%
Y=zeros(size(X));
Y([1,2,3])=1;
figure; plot(A*Y-B, 'o');