function [idx, err, value] = parse_uint16(bytes, bytes_length, idx)

err = true;

value = uint16(0);
    
if bytes_length < idx+2
    return
end

if bytes(idx) == hex2dec('cd')
    val_bytes = microbuf.from_big_endian(bytes(idx+1:idx+2));
    value = typecast(val_bytes, 'uint16');

    idx = idx+3;
    err = false;
end

end

