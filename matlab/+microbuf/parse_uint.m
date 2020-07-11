function [idx, err, value] = parse_uint(bytes, bytes_length, uint_size_bits, idx)

err = true;

if uint_size_bits == 8
    % uint8
    value = uint8(0);
    
    if bytes_length < idx+1
        return
    end
    
    if bytes(idx) == hex2dec('cc')
        value = typecast(bytes(idx+1), 'uint8');
        idx = idx+2;
        err = false;
    end
elseif uint_size_bits == 16
    % uint16
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
elseif uint_size_bits == 32
    % uint32
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
elseif uint_size_bits == 64
    % uint64
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

end

