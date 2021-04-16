function bytes = gen_array16(array_length)
%gen_array16 Generate bytes of array16
%array_length [0, 2^16-1]

bytes = repmat(uint8(0), 1, 3);
bytes(1) = hex2dec('dc');
bytes(2:3) = microbuf.uint_to_big_endian(uint16(array_length));

end

