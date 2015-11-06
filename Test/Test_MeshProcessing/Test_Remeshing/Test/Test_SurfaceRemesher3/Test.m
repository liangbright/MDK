%%
small_triangle_count= size(CandidateIndexSetAtSmallTriangle,1);
big_triangle_count=small_triangle_count/6;
intput_edge_count= size(candidateconflicttableTJunctionEdge,1);
candidate_count=length(candidatescore);
contypes=[];
alpha_count=candidate_count;
beta_count=small_triangle_count;
gama_count=2*intput_edge_count;

A=[];
B=[];
% beta-beta relation
for k=1:big_triangle_count    
    index0 = alpha_count + 6*(k-1)+1;
    index1 = alpha_count + 6*(k-1)+2;
    index2 = alpha_count + 6*(k-1)+3;
    index3 = alpha_count + 6*(k-1)+4;
    index4 = alpha_count + 6*(k-1)+5;
    index5 = alpha_count + 6*(k-1)+6;
    constraint0=zeros(1, alpha_count+beta_count+gama_count);
    constraint0([index0, index3, index5])=1;
    constraint1=zeros(1, alpha_count+beta_count+gama_count);
    constraint1([index0, index2, index5])=1;
    constraint2=zeros(1, alpha_count+beta_count+gama_count);
    constraint2([index0, index2, index4])=1;
    constraint3=zeros(1, alpha_count+beta_count+gama_count);
    constraint3([index1, index2, index4])=1;
    constraint4=zeros(1, alpha_count+beta_count+gama_count);
    constraint4([index1, index3, index4])=1;
    constraint5=zeros(1, alpha_count+beta_count+gama_count);
    constraint5([index1, index3, index5])=1;
    
    A(:,end+1)=constraint0;
    A(:,end+1)=constraint1;
    A(:,end+1)=constraint2;
    A(:,end+1)=constraint3;
    A(:,end+1)=constraint4;
    A(:,end+1)=constraint5;    
    contypes=[contypes '======'];
    B=[B ones(1,6)];
end

% alpha-beta relation
for k=1:small_triangle_count 
    Set=CandidateIndexSetAtSmallTriangle(k,:);
    Set=Set(~isnan(Set));
    Set=Set+1;%index 0 -> 1
    constraint=zeros(1, alpha_count+beta_count+gama_count);
    constraint(Set)=1;
    constraint(alpha_count+k)=-1;
    A(:,end+1)=constraint;    
    contypes=[contypes '='];
    B=[B 0];
end

% alpha-gama relation : TJunction
for k=1:intput_edge_count
    Set_a=candidateconflicttableTJunctionEdge(k,:);
    Set_a=Set_a(~isnan(Set_a));
    Set_a=Set_a+1;%index 0 -> 1
    Set_b=candidateconflicttableTJunctionMiddlePoint(k,:);
    Set_b=Set_b(~isnan(Set_b));
    Set_b=Set_b+1;%index 0 -> 1    
    constraint_a=zeros(1, alpha_count+beta_count+gama_count);% sum(alpha) = gama_1
    constraint_a(Set_a)=1;
    constraint_a(alpha_count+beta_count+k)=-1;
    constraint_b=zeros(1, alpha_count+beta_count+gama_count);% sum(alpha) = gama_2
    constraint_b(Set_b)=1;
    constraint_b(alpha_count+beta_count+intput_edge_count+k)=-1;
    constraint_c=zeros(1, alpha_count+beta_count+gama_count);%gama_1 + gama_2 <=1
    constraint_c(alpha_count+beta_count+k)=1;
    constraint_c(alpha_count+beta_count+intput_edge_count+k)=1;
    A(:,end+1)=constraint_a;    
    A(:,end+1)=constraint_b;    
    A(:,end+1)=constraint_c;    
    contypes=[contypes '==<'];    
    B=[B 0 0 1];
end
A=sparse(A)';
B=B';
C=[candidatescore(:); zeros(beta_count+gama_count,1)];
%%
[X, val, exitflag, output]=MIPSolver(A, B, C, contypes, []);
%%
WriteDenseMatrixAsJsonDataFile(X, 'C:\Research\Mesh\Remeshing\Build\Test_SurfaceRemesher2\TestData\FlagList.json');
%%
X=zeros(64,1);
X([1,2,3])=1;
D=A*X;
figure; plot(D, 'ro');
hold on
plot(B, 'bo')