function bytes = gen_uint16(u)

bytes = repmat(uint8(0), 1, 3);
bytes(1) = uint8(hex2dec('cd'));
bytes(2:3) = microbuf.uint_to_big_endian(uint16(u));

end

