function [idx, err] = check_array16(bytes, bytes_length, array_length, idx)
%PARSE_ARRAY16 Check whether bytes contains an array16 of a specific length at
%the current position
err = true;

% check whether array16 fits
if bytes_length < idx+2
    return
end

if array_length <= 2^16-1
    % try array16
    if bytes(idx) == hex2dec('dc')
        val_bytes = microbuf.from_big_endian(bytes(idx+1:idx+2));
        encoded_length = typecast(val_bytes, 'uint16');
        if encoded_length == array_length
            idx = idx+3;
            err = false;
        end
    end
end

end

