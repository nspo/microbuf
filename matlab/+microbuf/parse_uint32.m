function [idx, err, value] = parse_uint32(bytes, bytes_length, idx)

err = true;

value = uint32(0);
    
if bytes_length < idx+4
    return
end

if bytes(idx) == hex2dec('ce')
    val_bytes = microbuf.from_big_endian(bytes(idx+1:idx+4));
    value = typecast(val_bytes, 'uint32');

    idx = idx+5;
    err = false;
end

end

