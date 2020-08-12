function [idx, err] = check_array32(bytes, bytes_length, array_length, idx)
%PARSE_ARRAY32 Check whether bytes contains an array32 of a specific length at
%the current position
err = true;

% check whether array32 fits
if bytes_length < idx+4
    return
end

if array_length <= 2^32-1
    % try array32
    if bytes(idx) == hex2dec('dd')
        val_bytes = microbuf.from_big_endian(bytes(idx+1:idx+4));
        encoded_length = typecast(val_bytes, 'uint32');
        if encoded_length == array_length
            idx = idx+5;
            err = false;
        end
    end
end

end

