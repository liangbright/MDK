%%
clear
clc
CandidateConflictTable_TJunction=read_data('C:\Research\Mesh\Remeshing\TestData\candidate_conflict_table_TJunction.txt');
CandidateScoreList=importdata('C:\Research\Mesh\Remeshing\TestData\candidate_score.txt');
QuadCandidateIndexSetAtSmallTriangle=read_data('C:\Research\Mesh\Remeshing\TestData\QuadCandidateIndexSet_At_SmallTriangle.txt');
QuadCandidateIndexSetAtBigTriangle=read_data('C:\Research\Mesh\Remeshing\TestData\QuadCandidateIndexSet_At_BigTriangle.txt');
%
big_triangle_count=length(QuadCandidateIndexSetAtBigTriangle);
small_triangle_count= length(QuadCandidateIndexSetAtSmallTriangle);
if small_triangle_count/big_triangle_count ~= 6
    error('abd')
end
candidate_count=length(CandidateScoreList);

%length(CandidateConflictTable_TJunction) must = candidate_count
ConflictTable_Count=length(CandidateConflictTable_TJunction);
if ConflictTable_Count ~=  candidate_count
   error('abc') 
end
%%
RowIndexList=[];
ColIndexList=[];
ValueList=[];
% A=sparse(RowIndexList, ColIndexList, ValueList, 6*big_triangle_count+candidate_count, 2*candidate_count)
% A(RowIndexList(k), ColIndexList(k)) is ValueList(k)
B=[];
contypes=[];
% constraint: full cover
RowCounter=0;
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
    tempColIndexList1=find(constraint1>0);
    RowCounter=RowCounter+1;
    tempRowIndexList1=RowCounter*ones(1, length(tempColIndexList1));    
    RowIndexList=[RowIndexList, tempRowIndexList1];
    ColIndexList=[ColIndexList, tempColIndexList1];
    ValueList=[ValueList ones(1, length(tempColIndexList1))];
    
    constraint2=zeros(1, candidate_count);
    constraint2([Set0, Set1, Set3, Set6])=1;   
    tempColIndexList2=find(constraint2>0);
    RowCounter=RowCounter+1;
    tempRowIndexList2=RowCounter*ones(1, length(tempColIndexList2));    
    RowIndexList=[RowIndexList, tempRowIndexList2];
    ColIndexList=[ColIndexList, tempColIndexList2];
    ValueList=[ValueList ones(1, length(tempColIndexList2))];
    
    constraint3=zeros(1, candidate_count);
    constraint3([Set0, Set1, Set3, Set5])=1;       
    tempColIndexList3=find(constraint3>0);
    RowCounter=RowCounter+1;
    tempRowIndexList3=RowCounter*ones(1, length(tempColIndexList3));    
    RowIndexList=[RowIndexList, tempRowIndexList3];
    ColIndexList=[ColIndexList, tempColIndexList3];
    ValueList=[ValueList ones(1, length(tempColIndexList3))];
    
    constraint4=zeros(1, candidate_count);
    constraint4([Set0, Set2, Set3, Set5])=1;   
    tempColIndexList4=find(constraint4>0);
    RowCounter=RowCounter+1;
    tempRowIndexList4=RowCounter*ones(1, length(tempColIndexList4));    
    RowIndexList=[RowIndexList, tempRowIndexList4];
    ColIndexList=[ColIndexList, tempColIndexList4];
    ValueList=[ValueList ones(1, length(tempColIndexList4))];
    
    constraint5=zeros(1, candidate_count);
    constraint5([Set0, Set2, Set4, Set5])=1;   
    tempColIndexList5=find(constraint5>0);
    RowCounter=RowCounter+1;
    tempRowIndexList5=RowCounter*ones(1, length(tempColIndexList5));    
    RowIndexList=[RowIndexList, tempRowIndexList5];
    ColIndexList=[ColIndexList, tempColIndexList5];
    ValueList=[ValueList ones(1, length(tempColIndexList5))];
    
    constraint6=zeros(1, candidate_count);
    constraint6([Set0, Set2, Set4, Set6])=1;   
    tempColIndexList6=find(constraint6>0);
    RowCounter=RowCounter+1;
    tempRowIndexList6=RowCounter*ones(1, length(tempColIndexList6));    
    RowIndexList=[RowIndexList, tempRowIndexList6];
    ColIndexList=[ColIndexList, tempColIndexList6];
    ValueList=[ValueList ones(1, length(tempColIndexList6))];
    
    %A(:,end+1)=constraint1;
    %A(:,end+1)=constraint2;
    %A(:,end+1)=constraint3;
    %A(:,end+1)=constraint4;
    %A(:,end+1)=constraint5;
    %A(:,end+1)=constraint6;
    
    contypes=[contypes '======'];
    B=[B ones(1,6)];
end

%TJunction
for k=1:candidate_count    
    Set=CandidateConflictTable_TJunction{k}+1;
    
    constraint_t=zeros(1, candidate_count);
    constraint_t(Set)=1;
    constraint_t(k)=length(Set);
    tempColIndexList_b=find(constraint_t>0);
    RowCounter=RowCounter+1;
    tempRowIndexList_b=RowCounter*ones(1, length(tempColIndexList_b));    
    RowIndexList=[RowIndexList, tempRowIndexList_b];
    ColIndexList=[ColIndexList, tempColIndexList_b];
    ValueList=[ValueList constraint_t(tempColIndexList_b)];
    contypes=[contypes '<'];
    B=[B length(Set)];
        
    %A(:,end+1)=constraint_a;
    %A(:,end+1)=constraint_b;
end

A=sparse(RowIndexList, ColIndexList, ValueList, 6*big_triangle_count+candidate_count, candidate_count);
B=B(:);
C=CandidateScoreList(:);
%%
tic
[X, val, result]=MIPSolver_v6(A, B, C, contypes, []);
FlagList=X(1:candidate_count);
toc
%
WriteDenseMatrixAsJsonDataFile(FlagList, 'C:\Research\Mesh\Remeshing\TestData\FlagList.json');
%%
figure; plot(CandidateScoreList, 'o')
figure; plot(FlagList, 'o')
%%
Y=zeros(size(X));
Y([1,2,3])=1;
figure; plot(A*Y-B, 'o');