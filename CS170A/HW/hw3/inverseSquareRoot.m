function invSqRt = invSqRoot(in, g)
x_old = 1.0;
x = in;
i = 0;

errormarg = 1.0*10^(-15);
error = abs((x-x_old)/x);

while ( error > errormarg )
    i = i+1;
    x_old = x(i);
    x(end+1) = g(x_old);
    error = abs((x(i+1)-x_old)/x(i+1));
end

invSqRt = x;
end