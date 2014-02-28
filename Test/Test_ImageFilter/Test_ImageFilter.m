InverseCovarianceMatrix=diag([1/9, 1/16, 1/25]);

Mask=[];

RatioList=[];

for z= -16:16
    for y= -16:16
        for x= -16:16
            
            Ratio = [x, y, z] * InverseCovarianceMatrix * [x; y; z];
                       
            RatioList(end+1)=Ratio;
            
            if (Ratio < 4)
                
                Mask(:,end+1) =[x; y; z];
            end
        end
    end
end