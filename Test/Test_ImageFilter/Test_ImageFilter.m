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
%%
Lx=512;
Ly=512;
Lz=233;

Origin=[0, 0, 0]; % x0, y0, z0
Spacing=[1, 1, 1];

FilePathAndName='E:\HeartData\P1943091-im_6-phase10-close-leaflet\im_6\phase0_OutputImage.data';
%
ImageData=ReadImageDataFromRawDataFile(FilePathAndName, 'double', Lx, Ly, Lz);
%%
implay(ImageData)
%%
imtool(ImageData(:,:,100))