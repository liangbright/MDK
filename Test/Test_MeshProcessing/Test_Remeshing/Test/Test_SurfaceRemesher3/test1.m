%%
clear
clc
candidateconflicttableTJunctionEdge=read_data('C:\Research\Mesh\Remeshing\Build\Test_SurfaceRemesher3\TestData\candidate_conflict_table_TJunction_Edge.txt');
candidateconflicttableTJunctionMiddlePoint=read_data('C:\Research\Mesh\Remeshing\Build\Test_SurfaceRemesher3\TestData\candidate_conflict_table_TJunction_MiddlePoint.txt');
candidate_score=importdata('C:\Research\Mesh\Remeshing\Build\Test_SurfaceRemesher3\TestData\candidate_score.txt');
CandidateIndexSetAtSmallTriangle=read_data('C:\Research\Mesh\Remeshing\Build\Test_SurfaceRemesher3\TestData\CandidateIndexSet_At_SmallTriangle.txt');
CandidateIndexSetAtBigTriangle=read_data('C:\Research\Mesh\Remeshing\Build\Test_SurfaceRemesher3\TestData\CandidateIndexSet_At_BigTriangle.txt');
%%
big_triangle_count=length(CandidateIndexSetAtBigTriangle);
small_triangle_count= 6*big_triangle_count;
intput_edge_count= size(candidateconflicttableTJunctionEdge,1);
candidate_count=length(candidatescore);
contypes=[];
alpha_count=candidate_count;
beta_count=small_triangle_count+big_triangle_count;
%
A=[];
B=[];
% beta-beta relation
for k=1:big_triangle_count    
    index0 = alpha_count + 7*(k-1)+1;
    index1 = alpha_count + 7*(k-1)+2;
    index2 = alpha_count + 7*(k-1)+3;
    index3 = alpha_count + 7*(k-1)+4;
    index4 = alpha_count + 7*(k-1)+5;
    index5 = alpha_count + 7*(k-1)+6;
    index6 = alpha_count + 7*(k-1)+7;
    constraint0=zeros(1, alpha_count+beta_count);
    constraint0([index0, index3, index5, index6])=1;
    constraint1=zeros(1, alpha_count+beta_count);
    constraint1([index0, index2, index5, index6])=1;
    constraint2=zeros(1, alpha_count+beta_count);
    constraint2([index0, index2, index4, index6])=1;
    constraint3=zeros(1, alpha_count+beta_count);
    constraint3([index1, index2, index4, index6])=1;
    constraint4=zeros(1, alpha_count+beta_count);
    constraint4([index1, index3, index4, index6])=1;
    constraint5=zeros(1, alpha_count+beta_count);
    constraint5([index1, index3, index5, index6])=1;
    
    A(:,end+1)=constraint0;
    A(:,end+1)=constraint1;
    A(:,end+1)=constraint2;
    A(:,end+1)=constraint3;
    A(:,end+1)=constraint4;
    A(:,end+1)=constraint5;    
    contypes=[contypes '======'];
    B=[B ones(1,6)];
end

for k=1:small_triangle_count
    Set = CandidateIndexSetAtSmallTriangle{k};
    Set=Set(~isnan(Set));
    Set=Set+1;%index 0 -> 1
    CandidateIndexSetAtSmallTriangle{k}=Set;
end

for k=1:big_triangle_count
    Set = CandidateIndexSetAtBigTriangle{k};
    Set=Set(~isnan(Set));
    Set=Set+1;%index 0 -> 1
    CandidateIndexSetAtBigTriangle{k}=Set;
end
    
CandidateIndexSetAtTriangle={};
for k=1:big_triangle_count    
    index0 = 6*(k-1)+1;
    index1 = 6*(k-1)+2;
    index2 = 6*(k-1)+3;
    index3 = 6*(k-1)+4;
    index4 = 6*(k-1)+5;
    index5 = 6*(k-1)+6;    
    CandidateIndexSetAtTriangle{end+1}=CandidateIndexSetAtSmallTriangle{index0};
    CandidateIndexSetAtTriangle{end+1}=CandidateIndexSetAtSmallTriangle{index1};
    CandidateIndexSetAtTriangle{end+1}=CandidateIndexSetAtSmallTriangle{index2};
    CandidateIndexSetAtTriangle{end+1}=CandidateIndexSetAtSmallTriangle{index3};
    CandidateIndexSetAtTriangle{end+1}=CandidateIndexSetAtSmallTriangle{index4};
    CandidateIndexSetAtTriangle{end+1}=CandidateIndexSetAtSmallTriangle{index5};
    CandidateIndexSetAtTriangle{end+1}=CandidateIndexSetAtBigTriangle{k};
end

% alpha-beta relation
for k=1:beta_count 
    Set=CandidateIndexSetAtTriangle{k};
    constraint=zeros(1, alpha_count+beta_count);
    constraint(Set)=1;
    constraint(alpha_count+k)=-1;
    A(:,end+1)=constraint;    
    contypes=[contypes '='];
    B=[B 0];
end

A=sparse(A)';
B=B';
C=[candidatescore(:); zeros(beta_count,1)];
%%
[X, val, exitflag, output]=MIPSolver(A, B, C, contypes, []);
FlagList=X(1:alpha_count);
%%
WriteDenseMatrixAsJsonDataFile(FlagList, 'C:\Research\Mesh\Remeshing\Build\Test_SurfaceRemesher3\TestData\FlagList.json');
%%
figure; plot(candidatescore, 'o')
figure; plot(FlagList, 'o')
%%
Y=zeros(size(X));
Y([1,2,3])=1;
figure; plot(A*Y-B, 'o');