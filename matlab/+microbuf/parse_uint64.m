function [idx, err, value] = parse_uint64(bytes, bytes_length, idx)

err = true;

value = uint64(0);

if bytes_length < idx+8
    return
end

if bytes(idx) == hex2dec('cf')
    if microbuf.is_big_endian()
        value = typecast(bytes(idx+1:idx+8), 'uint64');
    else
        value = typecast(flip(bytes(idx+1:idx+8)), 'uint64');
    end

    idx = idx+9;
    err = false;
end

end

