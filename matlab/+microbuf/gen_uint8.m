function bytes = gen_uint8(u)

bytes = repmat(uint8(0), 1, 2);
bytes(1) = uint8(hex2dec('cc'));
bytes(2) = uint8(u);

end

