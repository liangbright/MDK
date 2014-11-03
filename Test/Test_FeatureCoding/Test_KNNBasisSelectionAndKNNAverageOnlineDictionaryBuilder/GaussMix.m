function Iobj=GaussMix(x, y, A, R, Lx, Ly, WindowRadius)

% fast  1s/100
%--------------------------------------------
Iobj=zeros(Ly, Lx);
ParticleNum=length(A);

A_thresh=0.002;
if nargin == 7
    delta=WindowRadius;
else
    %delta=1.4142*R.*sqrt(log(A)-log(0.0001*A))=4.2919*R;
    tempA=max(A, A_thresh);
    delta=1.4142*R.*sqrt(log(tempA)-log(A_thresh));
    delta=ceil(delta);
    delta=min(delta, 10*R);
end

for k=1:ParticleNum
    if A(k) > A_thresh
        x1=round(x(k)-delta(k)); x1=min(max(x1, 1), Lx);
        x2=round(x(k)+delta(k)); x2=min(max(x2, 1), Lx);
        y1=round(y(k)-delta(k)); y1=min(max(y1, 1), Ly);
        y3=round(y(k)+delta(k)); y3=min(max(y3, 1), Ly);
    
        [X, Y]=meshgrid(x1:x2, y1:y3);                    
        tempI=A(k)*exp(-((x(k)-X).^2+(y(k)-Y).^2)/(2*R(k)^2));    
        
        Iobj(y1:y3, x1:x2)=Iobj(y1:y3, x1:x2)+tempI;     
        
    end
end

%{
[X Y]=meshgrid(1:Lx, 1:Ly);
for k=1:ParticleNum                   
    tempEXP=exp(-((x(k)-X).^2+(y(k)-Y).^2)/(2*R(k)^2));    
    Iobj=Iobj+A(k)*tempEXP;     
end
%}