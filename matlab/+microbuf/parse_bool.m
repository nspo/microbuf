function [idx, err, value] = parse_bool(bytes, bytes_length, idx)

err = true;

value = false;

if bytes_length < idx
    return
end

if bytes(idx) == hex2dec('c3')
    value = true;
    idx = idx+1;
    err = false;
elseif bytes(idx) == hex2dec('c2')
    value = false;
    idx = idx+1;
    err = false;
end

end

