function C = MatMatOuter(A,B)
% This computes the matrix-matrix product C = A*B (via outer products) where
% A is an m-by-p matrix, B is a p-by-n matrix.
[m,p] = size(A);
[p,n] = size(B);
C = zeros(m,n);
for k=1:p
    % Add in k-th outer product
    C = C + A(:,k)*B(k,:);
end
