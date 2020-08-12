function [idx, err, value] = parse_float32(bytes, bytes_length, idx)

err = true;
value = single(0);

if bytes_length < idx+4
    return
end

if bytes(idx) == hex2dec('ca')
    val_bytes = microbuf.from_big_endian(bytes(idx+1:idx+4));
    value = typecast(val_bytes, 'single');

    idx = idx+5;
    err = false;
end

end