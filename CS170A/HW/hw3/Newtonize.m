function g = Newtonize(f)

%% we assume f() is a function handle, such as   f  = @(x) x.^4 - 3
%%
%% Newtonize(f) yields a function handle g() that computes Newton iterates.
%%
%% For example
%%
%%    n = 8;
%%    x = ones(n,1);
%%    for i=1:(n-1)
%%      x(i+1) = g(x(i));
%%    end
%%
%% would perform a Newton iteration.
%% Afterwards, with the definition of f() above, we would get the result:
%%
%%  >> format long
%%  >> x
%%  x =
%%     1.000000000000000
%%     1.500000000000000
%%     1.347222222222222
%%     1.317137693803440
%%     1.316075300754006
%%     1.316074012954383
%%     1.316074012952492
%%     1.316074012952492
%%
%% >> 3^(1/4)
%% ans =
%%    1.316074012952492

  x_symbol = sym('x')
  f_derivative = simplify(diff(f(x_symbol)))
  df = @(x) subs(f_derivative, x_symbol, x)
  g = @(x) x - f(x)/df(x)

