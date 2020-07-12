function [idx, err] = check_array(bytes, bytes_length, array_length, idx)
%PARSE_ARRAY Check whether bytes contains an array of a specific length at
%the current position
err = true;

% check whether fixarray fits
if bytes_length < idx+0
    return
end

if array_length <= 2^4-1
    % try fixarray
    if bitand(bin2dec('11110000'), bytes(idx)) == bin2dec('10010000')
        % prefix matches
        encoded_length = bitand(bin2dec('00001111'), bytes(idx));
        if encoded_length == array_length
            err = false;
            idx = idx+1;
        end
        return
    end
end % try other array types if prefix did not match

% check whether array16 fits
if bytes_length < idx+2
    return
end

if array_length <= 2^16-1
    % try array16
    if bytes(idx) == hex2dec('dc')
        encoded_length = uint16(bytes(idx+1))*2^8 + uint16(bytes(idx+2));
        if encoded_length == array_length
            idx = idx+3;
            err = false;
        end
        return
    end % still try array32
end

% check whether array32 fits
if bytes_length < idx+4
    return
end

if array_length <= 2^32-1
    % try array32
    if bytes(idx) == hex2dec('dd')
        encoded_length = uint32(bytes(idx+1))*2^24 + ...
            uint32(bytes(idx+2))*2^16 + uint32(bytes(idx+3))*2^8 + ...
            uint32(bytes(idx+4));
        if encoded_length == array_length
            idx = idx+5;
            err = false;
        end
        return
    end
end

end

