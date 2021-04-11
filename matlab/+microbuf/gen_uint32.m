function bytes = gen_uint32(u)

bytes = repmat(uint8(0), 1, 5);
bytes(1) = uint8(hex2dec('ce'));
bytes(2:5) = microbuf.uint_to_big_endian(uint32(u));

end

