function [PrincipalComponents, SingularValues] = more_efficient_pca( F, k )
   %
   %  Compute the top k Principal Components of a n x p matrix F when n << p.
   %
   %  NOTE:  k <= n is needed for this function to work properly. 
   %
   %  The function returns the k-th rank approximations to the singular values
   %  and principal components of cov(F).
   %
   %  Let Fbar is the matrix filled with column means of F.
   %
   %  Let  Y = (F-Fbar) / sqrt(n-1)  and suppose the SVD of Y  =  U  S  V'.
   %
   %  Then:
   %     cov(F) = 1/(n-1)  (F-Fbar)'  (F-Fbar)
   %            =  (   Y    )'  (   Y    )
   %            =  (U  S  V')'  (U  S  V')
   %            =   V  S' U'     U  S  V'
   %            =   V  S'           S  V'
   %            =   V  (S' S)  V' .
   %
   %  So the singular values of cov(F) are the diagonal of (S' S),
   %  and  V is the  matrix of principal components of F.
   %
   %  This function goes one step further and computes the rank-k-approximation.
   %  Let  U_k  S_k  V_k'  be the rank-k approximation of the SVD of  Y = (F-Fbar)/sqrt(n-1).
   %  Then the first k singular values of cov(F) are the diagonal of S^2,
   %  and  V_k is the  p x k  matrix of principal components of F.
   %

   n = size(F,1);
   p = size(F,2);
   if n > p, disp('it would be more efficient to perform PCA using some other method'), end
   if k > n, disp('this method will only compute the first n components, not k'), end

   %% Fbar = ones(size(F)) * diag(mean(F)); %% slow when p is large
   Fbar = kron( ones(n,1), mean(F) );

   Y = (F-Fbar) / sqrt(n-1);

   [U_k  S_k  V_k] = svds( Y, k );   %%  k cannot be larger than n or p

   PrincipalComponents = V_k;
   SingularValues = diag(S_k).^2;

