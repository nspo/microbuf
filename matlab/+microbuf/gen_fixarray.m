function bytes = gen_fixarray(array_length)
%gen_fixarray Generate bytes of fixarray
%array_length [0, 2^4-1]

bytes = uint8(bitor(bin2dec('10010000'), array_length));

end

