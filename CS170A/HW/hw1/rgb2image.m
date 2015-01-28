function X = rgb2image(R,G,B)
      X(:,:,1) = uint8(R);
      X(:,:,2) = uint8(G);
      X(:,:,3) = uint8(B);
      %  equivalent:  X = cat(3, uint8(R),uint8(G),uint8(B))
