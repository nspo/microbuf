function bytes = gen_bool(b)

if b
    bytes = uint8(hex2dec('c3'));
else
    bytes = uint8(hex2dec('c2'));
end

end

