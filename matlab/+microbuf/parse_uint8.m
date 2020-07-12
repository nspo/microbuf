function [idx, err, value] = parse_uint8(bytes, bytes_length, idx)

err = true;

value = uint8(0);

if bytes_length < idx+1
    return
end

if bytes(idx) == hex2dec('cc')
    value = typecast(bytes(idx+1), 'uint8');
    idx = idx+2;
    err = false;
end

end

