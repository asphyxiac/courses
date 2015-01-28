filenames = dir('*.bmp');
[row,col] = size(imread(filenames(1).name));
   %% row = 64
   %% col = 64
f = zeros(row*col,5);
n = row*col;

%  as a demo:  read in and show 5 face images
numfiles = size(filenames,1);
rp = randperm(numfiles);
samplesize = 5;
sample = rp(1:samplesize);

for i = 1:samplesize
   figure;
   Image_File = filenames(sample(i)).name;
   Face_Matrix = imread(Image_File);
   imshow(Face_Matrix)
   title(Image_File)
   f(:,i) = double(reshape(Face_Matrix, row*col, 1));
   % we reshape the Face_Matrix to a column vector,
   % converting to double in order to permit arithmetic.
end
average_f = sum(f,2)/samplesize;
Average_Face = reshape(uint8(average_f), row, col);

