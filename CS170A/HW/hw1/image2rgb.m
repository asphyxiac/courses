function [R,G,B] = image2rgb(X)
      R = X(:,:,1);
      G = X(:,:,2);
      B = X(:,:,3);
