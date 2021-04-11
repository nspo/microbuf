function bytes = gen_float64(d)
%gen_float64 Convert a float64/double value to uint8 array

bytes = repmat(uint8(0), 1, 9);
bytes(1) = hex2dec('cb');
bytes(2:9) = microbuf.uint_to_big_endian(typecast(double(d), 'uint64'));

end