t0 = cputime;
for i = 1:100
    Fn = fft(y(1:n));
end
t1 = cputime;

>> diffFny = t1-t0;

>> t0_4964 = cputime;
for i = 1:100
    Fn = fft(y(1:n-4964));
end
t1_4964 = cputime;

>> diffFn_4964 = t1_4964-t0_4964


>> diffFny-diffFn_4964

ans =

    0.8700
		
		