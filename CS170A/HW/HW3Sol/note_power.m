function binned = note_power(power)
c = 2^(1/12);
NoteFreq = 131 * c.^(0:60); % the kth note is c^k Hz from C.
AvgBFreq = (NoteFreq/c + NoteFreq)/2;
Div = AvgBFreq;
for i=1:length(power)
    for j=1:60
        if ( power(i) <= AvgBFreq(j) )
            if( power(i) > Div(j) )
                Div(j) = power(i);
            end
        end
    end
end
binned = Div;