%
%  Images as matrices
%

% street1 = imread('street1.jpg')
% galaxy = imread('ngc6543a.jpg')

% load('mandrill')
% load('durer')
load('flujet')
whos
size(X)
size(map)

imshow( X, map )
figure
rgbplot( map )

J = colormap(jet)
rgbshow(J)
size(J)

load('cape')
figure
imshow( X, map )
figure
rgbplot( map )

