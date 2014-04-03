clear all; 
%
X=zeros(3,3);
for i=1:3
    value = (i-1)/3;
    X(:,i)=value;
    X(i,:)=value;
end
%
D=zeros(3,3);
for i=1:3
    D(:,i)=i-1;
    D(i,:)=i-1;
end


param.L=3;
param.eps=0;
param.lambda=0;
param.numThreads=1;

tic
A = full(mexOMP(X,D, param));
t=toc;
