function bytes = gen_float32(d)
%gen_float32 Convert a float32/single value to uint8 array

bytes = repmat(uint8(0), 1, 5);
bytes(1) = hex2dec('ca');
bytes(2:5) = microbuf.uint_to_big_endian(typecast(single(d), 'uint32'));

end