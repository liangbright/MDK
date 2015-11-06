function data = read_data(FilePathAndName)

fid=fopen(FilePathAndName, 'r');
if fid == -1    
    error('can not open file')        
end
data={};
while 1
    LineStr = fgets(fid);
    if ~ischar(LineStr)
        break;
    end
    
    temp = textscan(LineStr,'%f, ');
    temp=temp{1}';
    data{end+1}=temp;
end