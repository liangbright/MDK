%%
clear
clc
CandidateConflictTable=read_data('C:\Research\Mesh\Remeshing\TestData\candidate_conflict_table.txt');
CandidateScoreList=importdata('C:\Research\Mesh\Remeshing\TestData\candidate_score.txt');
CandidateIndicator=importdata('C:\Research\Mesh\Remeshing\TestData\candidate_indicator.txt');

ObjMin=sum(CandidateIndicator.*CandidateScoreList);

candidate_count=length(CandidateScoreList);

ConflictTable_Count=length(CandidateConflictTable);
if ConflictTable_Count ~=  candidate_count
   error('abc') 
end
%%
RowIndexList=[];
ColIndexList=[];
ValueList=[];
% A=sparse(RowIndexList, ColIndexList, ValueList, candidate_count, candidate_count)
% A(RowIndexList(k), ColIndexList(k)) is ValueList(k)
B=[];
ValueCounter=0;
for k=1:candidate_count    
    Set=CandidateConflictTable{k}+1;
    
    constraint=zeros(1, candidate_count);
    constraint(Set)=1;
    constraint(k)=length(Set);
    tempColIndexList_t=find(constraint>0);
    tempRowIndexList_t=k*ones(1, length(tempColIndexList_t));    
    RowIndexList(ValueCounter+1:ValueCounter+length(tempRowIndexList_t))=tempRowIndexList_t;
    ColIndexList(ValueCounter+1:ValueCounter+length(tempColIndexList_t))=tempColIndexList_t;
    ValueList(ValueCounter+1:ValueCounter+length(tempColIndexList_t))=constraint(tempColIndexList_t);
    ValueCounter=ValueCounter+length(tempColIndexList_t);    
    
    B=[B length(Set)];           
end
A=sparse(RowIndexList, ColIndexList, ValueList, candidate_count, candidate_count);
C=CandidateScoreList(:);
contypes='<';
%%
%X_init=CandidateIndicator;
X_init=[];
tic
[X, val, result]=MIPSolver_v6(A, B, C, contypes, X_init);
toc
%
WriteDenseMatrixAsJsonDataFile(X, 'C:\Research\Mesh\Remeshing\TestData\FlagList.json');
%%
figure; plot(CandidateScoreList, 'o')
figure; plot(X, 'o')
%%
Y=CandidateIndicator;
figure; plot(1:candidate_count, A*Y-B, 'o');
%%
a8=full(A(8,:));
y=Y(a8>0);
