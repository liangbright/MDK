function Similarity = ComputeSimilarity(DataA, DataB)

diff=mean(abs(DataA(:)-DataB(:)));
%A=sum(abs(DataA(:))); 
%B=sum(abs(DataB(:))); 
%AB=(A+B)/2;
%NMD=diff/AB;
%Similarity=max(1-NMD, 0);
Similarity=exp(-2*diff);