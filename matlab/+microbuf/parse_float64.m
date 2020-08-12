function [idx, err, value] = parse_float64(bytes, bytes_length, idx)

err = true;
value = double(0);

if bytes_length < idx+8
    return
end

if bytes(idx) == hex2dec('cb')
    val_bytes = microbuf.from_big_endian(bytes(idx+1:idx+8));
    value = typecast(val_bytes, 'double');

    idx = idx+9;
    err = false;
end

end