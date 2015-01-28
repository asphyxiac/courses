>> f = @(x) x^(-2)-6;
>> g = Newtonize(f);
 
x_symbol =
 
x
 
 
f_derivative =
 
-2/x^3
 

df = 

    @(x)subs(f_derivative,x_symbol,x)


g = 

    @(x)x-f(x)/df(x)

>> inv = inverseSquareRoot(0.3, g);
>> format long
>> inv

inv =

  Columns 1 through 4

   0.300000000000000   0.369000000000000   0.402769773000000   0.408138504720978

  Columns 5 through 8

   0.408248246182616   0.408248290463856   0.408248290463863   0.408248290463863