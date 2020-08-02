function [err] = check_crc(bytes, bytes_length, idx)
%CHECK_CRC Check whether bytes contains the correct CRC16 checksum
%at the current position

orig_idx = idx;

% find expected CRC value
[~, err, expected_crc] = microbuf.parse_uint16(bytes, bytes_length, idx);
if err
    return
end

err = true; % still need to verify checksum
num_payload_bytes = orig_idx-1; % bytes_length could possibly be larger
actual_crc = microbuf.crc16_aug_ccitt(bytes, num_payload_bytes);

if actual_crc == expected_crc
    err = false;
end

end

