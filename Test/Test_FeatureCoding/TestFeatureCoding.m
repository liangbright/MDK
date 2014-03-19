A=[1, 2, 3, 4, 5
   0, 5, 0, 2, 1
   0, 0, 9, 1, 0
   0, 1, 0, 1, 0
   1, 0, 1, 0, 1];
%%
subA2=A([1,2],:)
subA3=A([1,2],:)*10
subA4=A(:, [1,2])
subA5 = A([1,2], [1, 2, 3]) * A([1, 2, 3], [1, 2])
subA6=A([1,2],:)*A(:,[1, 2])
%%
A=[1 2 0
   1 2 0
   3 3 3];

B=[1 2 3
   1 2 3];

C=[0 2 1 
   0 2 1];

C1=[1 0 1 
    1 0 1];

D = 1.0*A([1, 2], :) + 2.0*B - 3.0*C + 4.0*C1;

All = A(:, [1,2])*(B + 1.0 + C - 3.0*C + 4.0*C1);
%%
A=rand(512, 512);
B=rand(512, 512);
C=rand(512, 512);
C2=rand(512, 512);
D=rand(512, 512);

tic
for i = 1:10000

    temp= 1.0*A + 2.0*B + 3.0*C + 4.0*C2 + 5.0*D + 6.0*D;
    
    D=D+temp;
end
toc