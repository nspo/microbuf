function [idx, err] = check_fixarray(bytes, bytes_length, array_length, idx)
%PARSE_FIXARRAY Check whether bytes contains a fixarray of a specific length at
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
    end
end

end

