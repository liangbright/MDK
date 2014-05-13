x=0:0.001:4;
y=exp(-0.2.*x.*x);
figure; plot(x,y); grid on
%%
L2Norm=zeros(1, 256035);
for k=1:256035
    tempk=FeatureData(:,k);
    
    while 1
        n=randi(256035, 1);
    
        if (n ~= k)    
            tempn=FeatureData(:,n);        
            temp=tempk-tempn;
        
            L2Norm(k)=sum(temp.*temp);
            
            break;
        end
    end
end
%%