function [idx, err, value] = parse_float32(bytes, bytes_length, idx)

err = true;
value = single(0);

if bytes_length < idx+4
    return
end

if bytes(idx) == hex2dec('ca')
    if microbuf.is_big_endian()
        value = typecast(bytes(idx+1:idx+4), 'single');
    else
        value = typecast(flip(bytes(idx+1:idx+4)), 'single');
    end

    idx = idx+5;
    err = false;
end

end