function [idx, err, value] = parse_uint32(bytes, bytes_length, idx)

err = true;

value = uint32(0);
    
if bytes_length < idx+4
    return
end

if bytes(idx) == hex2dec('ce')
    if microbuf.is_big_endian()
        value = typecast(bytes(idx+1:idx+4), 'uint32');
    else
        value = typecast(flip(bytes(idx+1:idx+4)), 'uint32');
    end

    idx = idx+5;
    err = false;
end

end

