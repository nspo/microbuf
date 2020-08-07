function [idx, err, value] = parse_float64(bytes, bytes_length, idx)

err = true;
value = double(0);

if bytes_length < idx+8
    return
end

if bytes(idx) == hex2dec('cb')
    if microbuf.is_big_endian()
        value = typecast(bytes(idx+1:idx+8), 'double');
    else
        value = typecast(flip(bytes(idx+1:idx+8)), 'double');
    end

    idx = idx+9;
    err = false;
end

end