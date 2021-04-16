function bytes = gen_uint64(u)

bytes = repmat(uint8(0), 1, 9);
bytes(1) = uint8(hex2dec('cf'));
bytes(2:9) = microbuf.uint_to_big_endian(uint64(u));

end

