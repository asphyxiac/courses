function reshaped = detectAnomalies(image, p)
imageD = double(image);
m = size(imageD, 1);
n = size(imageD, 2);
imageR = reshape(imageD, m*n, p);
[Cimg Simg] = pca(imageR);
imgP = Cimg(:,2)'*imageR';
reshaped = reshape(imgP, m, n);
figure
imagesc(reshaped)
colormap(gray)
end