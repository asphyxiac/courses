%----------------------------------------------------------------------------------
%        Advanced computation of Eigenfaces
%----------------------------------------------------------------------------------

samplesize = 150;    %  the number of sample faces used in obtaining the eigenfaces

k = 30;              %  the number of the eigenfaces we use to represent faces
                     %  (k = 20 works but does not give as good results)

%----------------------------------------------------------------------------------
%        Determine size of the images
%----------------------------------------------------------------------------------

filenames = dir('*.bmp');
[row,col] = size(imread(filenames(1).name));
   %% row = 64
   %% col = 64

%----------------------------------------------------------------------------------
%        Useful functions for converting  images  <-->  vectors
%----------------------------------------------------------------------------------
%
%   These bitmap images are 64x64 matrices of uint8 (unsigned 8-bit) values;
%   we convert these to double values in order to permit arithmetic.
%
%   We also convert 64 x 64 image matrices to 64^2 x 1 vectors (using reshape())
%   so that we can derive eigenfaces using Matlab's SVD/matrix functions.
%
%----------------------------------------------------------------------------------

image_vector = @(Bitmap) double(reshape(Bitmap,1,row*col));

vector_image = @(Vec) reshape( uint8( min(max(Vec,0),255) ), row, col);

vector_render = @(Vec) imshow(vector_image(Vec));

%----------------------------------------------------------------------------------
%        Demo:  read in and show <samplesize> face images
%----------------------------------------------------------------------------------

F = zeros(samplesize,row*col); % the array of sample images (stored as vectors)

numfiles = size(filenames,1);
rp = randperm(numfiles);       %  random permutation of the list of image filenames

sample = rp(1:samplesize);     %  use the first <samplesize> images as our sample

for i = 1:samplesize
   Image_File = filenames(sample(i)).name;
   Face_Matrix = imread(Image_File);
   %% figure
   %% imshow(Face_Matrix)
   %% title(Image_File)
   F(i,:) = image_vector(Face_Matrix);  % the i-th row of F is the i-th image
end

%----------------------------------------------------------------------------------
%        fbar = the average face
%----------------------------------------------------------------------------------

fbar = sum(F,1)/samplesize;   % average of all rows in F

figure
vector_render(fbar)
title('The Average Face')
xlabel('(average of our sample)')

%----------------------------------------------------------------------------------
%        Subtract the average face from each face in F
%----------------------------------------------------------------------------------

for i = 1:samplesize
   F(i,:) = F(i,:) - fbar;
end

%----------------------------------------------------------------------------------
%       Obtain the eigenfaces U with PCA
%----------------------------------------------------------------------------------

[U, S, V] = svds( cov(F), k);

%----------------------------------------------------------------------------------
%   The columns of the U matrix obtained with PCA are our "eigenfaces".
%     
%   That is, e_1, e_2, ..., e_k are the first k eigenfaces:
%
%          U  =  ( e_1 | e_2 | ... | e_k | ... )
%
%   These eigenfaces are orthonormal, and define our axes of "face space".
%
%----------------------------------------------------------------------------------

singular_values = diag(S);

figure
plot( 1:k, singular_values, 'r.' )   %  the first k singular values for the eigenfaces
title('largest singular values of the face matrix F')
xlabel('we want k large enough that singular values after the k-th are "negligible"')

%----------------------------------------------------------------------------------
%   Finding the eigenface representation of a face
%----------------------------------------------------------------------------------
%
%   Any face image (here, a row vector) f can be represented as a sum
%
%       f  =  fbar   +   c_1 * e_1' +  c_2 * e_2'  +  ...  +  c_k * e_k'
%
%   where the c_j's are the  "coordinates in face space"  for f, or equivalently,
%   if  c  = [c_1; c_2; ...; c_k; 0; ...; 0]  is a row vector of coefficients then
%
%       f  =  fbar   +   c * U'.
%
%   Thus an image f (a 64 x 64 array) is represented by a vector c
%   containing only k numbers.   Impressive compression!
%
%----------------------------------------------------------------------------------

some_arbitrary_face = 'face117.bmp';  % an arbitrarily-chosen face

f = image_vector(imread(some_arbitrary_face));

figure
vector_render( f )
title('An arbitrarily-chosen face')
xlabel(sprintf('actually: %s', some_arbitrary_face))

%----------------------------------------------------------------------------------
%
%   To represent     f  =  fbar  +  c * U'
%   we can obtain the vector c of coordinates
%   by multiplying  t = (f - fbar)  by U
%   (which projects t onto the eigenfaces):   c = t * U
%
%----------------------------------------------------------------------------------


t = f - fbar;    %  remove the average face before finding eigencoefficients

c = t * U;      %  eigencoefficients for f:    c_j  =  t * e_j


reconstructed_f  =  fbar  +  c * U';

figure
vector_render( reconstructed_f )
title('reconstruction of the arbitrarily-chosen face')
xlabel(sprintf('using the first %d eigenfaces',k))

reconstruction_error  =  norm( f - reconstructed_f )

%----------------------------------------------------------------------------------
%
%   By varying the vector c of coefficients/coordinates,
%   we can easily produce new faces.
%
%----------------------------------------------------------------------------------
