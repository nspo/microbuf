function [idx, err, value] = parse_uint16(bytes, bytes_length, idx)

err = true;

value = uint16(0);
    
if bytes_length < idx+2
    return
end

if bytes(idx) == hex2dec('cd')
    if microbuf.is_big_endian()
        value = typecast(bytes(idx+1:idx+2), 'uint16');
    else
        value = typecast(flip(bytes(idx+1:idx+2)), 'uint16');
    end

    idx = idx+3;
    err = false;
end

end

