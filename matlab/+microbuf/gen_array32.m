function bytes = gen_array32(array_length)
%gen_array32 Generate bytes of array32
%array_length [0, 2^32-1]

bytes = repmat(uint8(0), 1, 5);
bytes(1) = hex2dec('dd');
bytes(2:5) = microbuf.uint_to_big_endian(uint32(array_length));

end

